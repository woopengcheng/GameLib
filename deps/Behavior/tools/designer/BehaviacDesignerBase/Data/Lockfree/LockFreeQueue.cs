using System;
using System.Threading;
using System.Collections.Generic;

namespace Concurrent
{
    ///SPSCQueue{T} is an efficient queue which allows one thread to use the Enqueue and another one use the Dequeue without locking
    public class SPSCQueue<T>
    {
        class Chunk
        {
            public long distance;
            public volatile int head_pos;
            public volatile Chunk next;
            public volatile int tail_pos;
            public T[] values;

            #region .ctor
            /// <summary>
            /// Initializes a new instance of the <see cref="Chunk"/> class by using
            /// the specified capacity.
            /// </summary>
            /// <param name="capacity">
            /// The number of elements that the chunk can hold.
            /// </param>
            public Chunk(int capacity) {
                values = new T[capacity];
                head_pos = 0;
                tail_pos = 0;
                next = null;
                distance = 0;
            }
            #endregion
        }

        const int kDefaultCapacity = 1000;
        volatile Chunk divider_;
        readonly int granularity_;
        volatile Chunk tail_chunk_;

        #region .ctor
        /// <summary>
        /// Initializes a new instance of the <see cref="SPSCQueue{T}"/> class
        /// that has the default capacity.
        /// </summary>
        public SPSCQueue()
            : this(kDefaultCapacity) {
        }

        /// <summary>
        /// Initializes a new instance of the <see cref="SPSCQueue{T}"/> class
        /// by using the specified granularity.
        /// </summary>
        /// <param name="granularity">
        /// A number that defines the granularity of the queue(how many pushes
        /// have to be done till actual memory allocation is required).
        /// </param>
        public SPSCQueue(int granularity) {
            granularity_ = granularity;
            divider_ = new Chunk(granularity);

            // make sure that the divider will not be used to store elements.
            divider_.tail_pos = granularity - 1;
            divider_.head_pos = granularity;

            tail_chunk_ = divider_;
        }
        #endregion

        /// <summary>
        /// Adds an element to the back end of the queue.
        /// </summary>
        /// <param name="element">
        /// The element to be added to the back end of the queue.
        /// </param>
        public void Enqueue(T element) {
            int tail_pos = tail_chunk_.tail_pos;

            // If either the queue is not empty or the tail chunk is not full, adds
            // the specified element to the back end of the current tail chunk.
            if (tail_chunk_ != divider_ && ++tail_pos < granularity_) {
                tail_chunk_.values[tail_pos] = element;

                // Prevents any kind of instruction reorderring or caching.
                Thread.MemoryBarrier();

                // "Commit" the newly added item and "publish" it atomically
                // to the consumer thread.
                tail_chunk_.tail_pos = tail_pos;
                return;
            }

            // Create a new chunk if a cached one does not exists and links it
            // to the current last node.
            Chunk chunk = new Chunk(granularity_);
            tail_chunk_.next = chunk;

            // Reset the chunk and append the specified element to the first slot.
            chunk.tail_pos = 0; // An unconsumed element is added to the first slot.
            chunk.head_pos = 0;
            chunk.next = null;
            chunk.values[0] = element;
            chunk.distance = tail_chunk_.distance + 1;

            // Make sure that the new chunk is fully initialized before it is
            // assigned to the tail chunk.
            Thread.MemoryBarrier();

            // At this point the newly created chunk(or the last cached chunk) is
            // not yet shared, but still private to the producer; the consumer will
            // not follow the linked chunk unless the value of |tail_chunk_| says
            // it may follow. The line above "commit" the update and publish it
            // atomically to the consumer thread.
            tail_chunk_ = tail_chunk_.next;
        }

        /// <summary>
        /// Removes all elements from the <see cref="SPSCQueue{T}"/>.
        /// </summary>
        /// <remarks>
        /// <para>
        /// <see cref="Clear"/> removes the elements that are not currently
        /// present in the queue. Elements added to the queue after
        /// <see cref="Clear"/> is called and while <see cref="Clear"/> is running,
        /// will not be cleared.
        /// </para>
        /// This operation should be sychronized with the <see cref="Dequeue()"/>
        /// and <see cref="Dequeue(out T)"/> operations.
        /// </remarks>
        public void Clear() {
            // Save the current tail chunk to ensure that the future elements are
            // not cleared.
            Chunk current_tail_chunk = tail_chunk_;

            while (divider_ != current_tail_chunk) {
                divider_ = divider_.next;
            }
        }

        /// <summary>
        /// Removes and returns the object at the beginning of the
        /// <see cref="SPSCQueue{T}"/>.
        /// </summary>
        /// <returns><typeparamref name="T"/> The object that is removed from the
        /// <see cref="SPSCQueue{T}"/></returns>
        /// <exception cref="InvalidOperationException">The
        /// <see cref="SPSCQueue{T}"/> is empty.</exception>
        public T Dequeue() {
            T t;
            bool ok = Dequeue(out t);

            if (!ok) {
                throw new InvalidOperationException("invalid operation");
            }

            return t;
        }

        /// <summary>
        /// Removes and returns the object at the beginning of the
        /// <see cref="SPSCQueue&lt;T&gt;"/>.
        /// </summary>
        /// <param name="t">When this method returns, contains the object that was
        /// removed from the beginning of the <see cref="SPSCQueue&lt;T&gt;"/>, if
        /// the object was successfully removed; otherwise the default value
        /// of the type <typeparamref name="T"/>.</param>
        /// <returns><c>true</c> if the queue is not empty and the object at the
        /// beginning of it was successfully removed; otherwise, false.
        /// </returns>
        public bool Dequeue(out T t) {
            // checks if the queue is empty
            while (divider_ != tail_chunk_) {
                // The chunks that sits between the |divider_| and the |tail_chunk_|,
                // excluding the |divider_| and including the |tail_chunk_|, are
                // unconsumed.
                Chunk current_chunk = divider_.next;

                // We need to compare the current chunk |tail_pos| with the |head_pos|
                // and |granularity|. Since, the |tail_pos| can be modified by the
                // producer thread we need to cache it instantaneous value.
                int tail_pos;
                tail_pos = current_chunk.tail_pos;

                if (current_chunk.head_pos > tail_pos) {
                    if (tail_pos == granularity_ - 1) {
                        // we have reached the end of the chunk, go to the next chunk and
                        // frees the unused chunk.
                        divider_ = current_chunk;
                        //head_chunk_ = head_chunk_.next;

                    } else {
                        // we already consume all the available itens.
                        t = default(T);
                        return false;
                    }

                } else {
                    // Ensure that we are reading the freshness value from the chunk
                    // values array.
                    Thread.MemoryBarrier();

                    // Here the |head_pos| is less than or equals to |tail_pos|, get
                    // the first unconsumed element and increments |head_pos| to publish
                    // the queue item removal.
                    t = current_chunk.values[current_chunk.head_pos];

                    // keep the order between assignment and publish operations.
                    Thread.MemoryBarrier();

                    current_chunk.head_pos++;
                    return true;
                }
            }

            t = default(T);
            return false;
        }

        /// <summary>
        /// Gets a value indicating whether the <see cref="SPSCQueue{T}"/> is empty.
        /// </summary>
        /// <remarks>
        /// Since this collection is intended to be accessed concurrently by two
        /// threads in a producer/consumer pattern, it may be the case that another
        /// thread will modify the collection after <see cref="IsEmpty"/> returns,
        /// thus invalidatind the result.
        /// </remarks>
        public bool IsEmpty {
            get
            {
                Chunk divider = divider_;
                Chunk tail = tail_chunk_;

                return (divider.next == tail || divider == tail) &&
                tail.head_pos > tail.tail_pos;
            }
        }
    }
}