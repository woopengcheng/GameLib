/////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Tencent is pleased to support the open source community by making behaviac available.
//
// Copyright (C) 2015 THL A29 Limited, a Tencent company. All rights reserved.
//
// Licensed under the BSD 3-Clause License (the "License"); you may not use this file except in compliance with
// the License. You may obtain a copy of the License at http://opensource.org/licenses/BSD-3-Clause
//
// Unless required by applicable law or agreed to in writing, software distributed under the License is
// distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and limitations under the License.
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef BEHAVIAC_MEMEORY_OBJECTPOOL_H
#define BEHAVIAC_MEMEORY_OBJECTPOOL_H

#include "behaviac/base/core/config.h"
#include "behaviac/base/core/system.h"
#include "behaviac/base/core/memory/memory.h"
#include "behaviac/base/core/container/slist.h"
#include "behaviac/base/core/container/dlist.h"

namespace behaviac
{
    BEHAVIAC_API IMemAllocator& GetMemoryAllocator();
#define BEHAVIAC_OBJECTPOOL_INFINIT 0xFFFFFFFF

    /*! \addtogroup Memory
     * @{
     * \addtogroup pool
     * @{ */
    //////////////////////////////////////////////////////////////////////////////////////////////////////
    //
    //                  Object Pool
    //

    /// BasicObjectPool is a template that manages thread safe allocations in a pool of object in a really fast way.
    /*! The BasicObjectPool offer really high speed efficiency while minimizing memory fragmentation.
    	Use this structure when you know that you'll have a high rate of allocation.
    	For example: Particles in a particle system, file request in a file system...

    	The basic concept is it creates linked segments (array) of object in system memory,
    	and then logically manages allocation/deallocation of objects into these segments (array).
    	So a BasicObjectPool can be either a pool of contiguous object in memory (BasicObjectPool with only one segment) or
    	a list of sub-pools/segments of contiguous object in memory (BasicObjectPool with many segments)

    	At creation time of the BasicObjectPool, you can specify the count of object per segment with
    	objectCountPerSegment, the maximum number of object in the pool with
    	maximumNumberOfObjectInPool and a count of already physically created objects
    	you want with initialyCreatedRatio.

    	When allocating an object with Allocate() or AllocateDefault(), the BasicObjectPool checks if a new segment
    	is needed and create one if so else it uses the current segment to logically allocate your new object.
    	The constructor of the new logically allocated object is not called unless you used AllocateDefault()
    	whitch call the default empty constructor. So if your BasicObjectPool is a pool of some class where each
    	instances needs their constructor to be called, you'll have to call the placement new manually giving
    	the call to Allocate() to the placement new.
    	ex: CParticle* myNewParticle = new(particlePool.Allocate())CParticle(m_particleColor);

    	When freeing an object calling Free(TYPE* object), the BasicObjectPool finds the segments where the object resides
    	and removes it and call the object's destructor. If the segment is empty, it is physically deleted.

    	- To create a fixed sized pool with 100 objects max, all created at pool's creation time : BasicObjectPool(100, 100, 1.0);
    	- To create a dynamic sized pool of maximum 100 objects, with 10 objects per segment,
    	with only the first segment created at pool's creation time  : BasicObjectPool(10, 100, 10/100.0);
    	- To create a dynamic sized pool with no maximum count, with 10 object's per segment : (10, BEHAVIAC_OBJECTPOOL_INFINIT, 0.0);
    	In the case of an Infinite size pool, the initialyCreatedRatio parameter is ignored
    	*/
    template <typename TYPE, typename TALLOCATOR = behaviac::IMemAllocator, typename MUTEX_TYPE = behaviac::Mutex>
    class BasicObjectPool
    {
    public:

        BasicObjectPool(TALLOCATOR* pAllocator = 0);
        BasicObjectPool(uint32_t objectCountPerSegment, uint32_t maximumNumberOfObjectInPool, float initialyCreatedRatio = 0.33f, uint32_t extraDataSpacePerObject = 0, TALLOCATOR* pAllocator = 0);
        virtual ~BasicObjectPool();

        /// create the pool with min/max and ratio. Use this if the creation of the BasicObjectPool was made using the default empty constructor.
        /*! \param objectCountPerSegment The count of objects per segment in the pool. Minimum is 2.
        	\param maximumNumberOfObjectInPool The maximum count of object the pool can allocate (BEHAVIAC_OBJECTPOOL_INFINIT or 0xFFFFFFF for infinite size)
        	\param initialyCreatedRatio The count of object the pool will physically create when Create is called.
        	\param extraDataSpacePerObject Size of user data that can be inserted after the end of each object's allocation. It is defaulted to 0.
        	*/
        virtual bool Create(uint32_t objectCountPerSegment, uint32_t maximumNumberOfObjectInPool, float initialyCreatedRatio = 0.33f, uint32_t extraDataSpacePerObject = 0);

        /// Allocate one object. No constructor is called. IF YOUR POOL MANAGES CLASS INSTANCES YOU MUST CALL THE PLACEMENT NEW WITH THIS METHOD TO INITIALISE YOUR OBJECT ex: new(particlePool.Allocate())CParticle();
        inline TYPE* Allocate();

        /// Allocate one object calling the object's default constructor if the pool manages Class instances.
        inline TYPE* AllocateDefault();

        /// Free an allocated object calling the destructor if any. Will call destructor.
        /*!
        	\param object Object to free
        	*/
        virtual void  Free(TYPE* object);

        /// Free an allocated object calling the destructor if any. Does call destructor only if callDestructor if true
        /*!
        	\param object         Object to free
        	\param callDestructor Call destructor if true
        	*/
        virtual void  Free(TYPE* object, bool callDestructor);

        /// Destroy the pool by freeing all it's segments. Does not call the destructor of object still allocated.
        /*! \param forceDestroy If false, checks that all object in the pool have been released before deleting the segment
         */
        virtual void Destroy(bool forceDestroy = false);

        /// Get the number of objects allocated in the pool
        inline uint32_t GetAllocCount() const;
        /// Get the number of free objects in the pool. In the case of a dynamic sized pool, this value is the available object in the current segment.
        inline uint32_t GetFreeCount()  const;
        /// Get the higher count of allocation so far
        inline uint32_t GetMaxAllocCount()  const;

        inline uint32_t GetMemoryUsage() const
        {
            return sizeof(*this) + this->m_segmentCount * this->m_objectCountsInSegment * m_objectSize;
        }

#ifdef _DEBUG
        /// Get allocation statistics (only in Debug target)
        uint32_t GetPeekCount()
        {
            return m_peakAllocatedCount;
        }
#endif

    protected:

        /// Allocate one object. No constructor is called.
        virtual void AllocateInternal(TYPE*& ptr);

        struct SLink
        {
            SLink* next;

            void Initialize()
            {
                next = 0;
            }

            bool Empty()
            {
                return next == 0;
            }

            SLink* Pop()
            {
                SLink* ret = next;
                next = next->next;
                return ret;
            }

            void Push(SLink* link)
            {
                link->next = next;
                next = link;
            }
        };

        class PoolSegment
        {
            BEHAVIAC_DECLARE_DLIST(PoolSegment);

        public:
#ifdef _DEBUG
            enum     ESafety { Val = 0xBEBA0B0B };
            ESafety  m_safety;
#endif
            SLink   m_freeObjectList;
            uint32_t     m_freeObjectCount;
        };

        struct BlockHeader
        {
#ifdef _DEBUG
            enum    ESafety { Val = 0x01D0ADDE };
            ESafety m_safety;
#endif
            uint32_t m_index;
        };

        inline bool AllocateSegment();
        inline void FreeSegment(PoolSegment* segment);

        // Member variables
        DList::DRoot<PoolSegment>   m_segmentList;
        PoolSegment*                m_segmentCandidateForDeletion;
        MUTEX_TYPE                  m_lock;

        TALLOCATOR*		m_allocator;

        uint32_t m_objectCountsInSegment;
        uint32_t m_maxSegmentCount;
        uint32_t m_objectSize;
        uint32_t m_segmentCount;
        uint32_t m_freeObjectCount;
        uint32_t m_candidateDeletionLimit;
        uint32_t m_candidateDeletionCountLimit;
        uint32_t m_segmentAlign;
        uint32_t m_objectAlign;

#ifdef _DEBUG
        uint32_t m_peakAllocatedCount;
        uint32_t m_totalAllocatedCount;
#endif
    };

#if BEHAVIAC_COMPILER_MSVC
#pragma warning(push)
#pragma warning(disable:4127) // conditional expression is constant
#endif
    //////////////////////////////////////////////////////////////////////////////////////
    //
    //              Inline
    //
    template <typename TYPE, typename TALLOCATOR, typename MUTEX_TYPE>
    inline uint32_t BasicObjectPool<TYPE, TALLOCATOR, MUTEX_TYPE>::GetAllocCount()  const
    {
        return (m_segmentCount * m_objectCountsInSegment) - m_freeObjectCount;
    }

    template <typename TYPE, typename TALLOCATOR, typename MUTEX_TYPE>
    inline uint32_t BasicObjectPool<TYPE, TALLOCATOR, MUTEX_TYPE>::GetFreeCount()  const
    {
        return m_segmentCount == 0 ? m_maxSegmentCount * m_objectCountsInSegment : m_freeObjectCount;
    }

    template <typename TYPE, typename TALLOCATOR, typename MUTEX_TYPE>
    inline uint32_t BasicObjectPool<TYPE, TALLOCATOR, MUTEX_TYPE>::GetMaxAllocCount()  const
    {
        return m_segmentCount * m_objectCountsInSegment;
    }

    template <typename TYPE, typename TALLOCATOR, typename MUTEX_TYPE>
    inline BasicObjectPool<TYPE, TALLOCATOR, MUTEX_TYPE>::BasicObjectPool(TALLOCATOR* pAllocator) :
        m_segmentCandidateForDeletion(0), m_objectCountsInSegment(0), m_maxSegmentCount(0),
        m_objectSize(0), m_segmentCount(0), m_freeObjectCount(0),
        m_candidateDeletionLimit(0), m_candidateDeletionCountLimit(0), m_allocator(pAllocator)
    {
        if (!m_allocator)
        {
            m_allocator = (TALLOCATOR*)&GetMemoryAllocator();
        }
    }

    template <typename TYPE, typename TALLOCATOR, typename MUTEX_TYPE>
    inline BasicObjectPool<TYPE, TALLOCATOR, MUTEX_TYPE>::BasicObjectPool(
        uint32_t objectCountPerSegment, uint32_t maximumNumberOfObjectInPool,
        float initialyCreatedRatio, uint32_t extraDataSpacePerObject, TALLOCATOR* pAllocator)
    {
        m_objectSize = 0;
        m_segmentCount = 0;
        m_candidateDeletionCountLimit = 0;
        m_allocator = pAllocator;

        if (!m_allocator)
        {
            m_allocator = (TALLOCATOR*)&GetMemoryAllocator();
        }

        Create(objectCountPerSegment, maximumNumberOfObjectInPool, initialyCreatedRatio, extraDataSpacePerObject);
    }

    template <typename TYPE, typename TALLOCATOR, typename MUTEX_TYPE>
    inline BasicObjectPool<TYPE, TALLOCATOR, MUTEX_TYPE>::~BasicObjectPool()
    {
        Destroy(true);
    }

    template <typename TYPE, typename TALLOCATOR, typename MUTEX_TYPE>
    inline bool BasicObjectPool<TYPE, TALLOCATOR, MUTEX_TYPE>::Create(uint32_t objectCountPerSegment, uint32_t maximumNumberOfObjectInPool, float initialyCreatedRatio, uint32_t extraDataSpacePerObject)
    {
        //check if the pool is already allocator as not been previously been created !
        //You don't need to call create after a destroy. Just start reusing it. Unless you want to change the object count by segment
        BEHAVIAC_ASSERT(m_segmentCount == 0);
        //validate params
        BEHAVIAC_STATIC_ASSERT(sizeof(TYPE) != 0);
        BEHAVIAC_ASSERT(objectCountPerSegment <= maximumNumberOfObjectInPool &&
                        objectCountPerSegment >= 2 &&
                        initialyCreatedRatio >= 0.0f && initialyCreatedRatio <= 1.0f);
        m_freeObjectCount = 0;
        m_objectSize = (uint32_t)Max(sizeof(TYPE), sizeof(SLink));// at least 'link' size
        //add extra user data at the end of the object
        m_objectSize += extraDataSpacePerObject;
        //we must make sure that the pointers returned by the pool respect the platform alignment,
        //or else we might run into trouble!
        //first, check what is needed to align the first object of the pool
        m_segmentAlign = BEHAVIAC_HW_MEM_ALIGN - ((sizeof(BlockHeader) + sizeof(PoolSegment)) % BEHAVIAC_HW_MEM_ALIGN);

        if (m_segmentAlign == BEHAVIAC_HW_MEM_ALIGN)
        {
            m_segmentAlign = 0;    // Already aligned, add no bytes!
        }

        //next, see if individual objects need to be aligned
        m_objectAlign = BEHAVIAC_HW_MEM_ALIGN - ((sizeof(BlockHeader) + m_objectSize) % BEHAVIAC_HW_MEM_ALIGN);

        if (m_objectAlign == BEHAVIAC_HW_MEM_ALIGN)
        {
            m_objectAlign = 0;    // Already aligned, add no bytes!
        }

        // Align the MAX to the initial (the initial is the the segment len)
        uint64_t maxObjCnt = maximumNumberOfObjectInPool / objectCountPerSegment;

        if ((maximumNumberOfObjectInPool % objectCountPerSegment) != 0)
        {
            maxObjCnt++;
        }

        maxObjCnt *= objectCountPerSegment;

        if (maxObjCnt > U32_Max)
        {
            maximumNumberOfObjectInPool = U32_Max;

        }
        else
        {
            maximumNumberOfObjectInPool = (uint32_t)maxObjCnt;
        }

        //calculate the max segment count based on the ratio between initial and max
        uint32_t ratio = maximumNumberOfObjectInPool / objectCountPerSegment;
        m_maxSegmentCount = ratio < 1 ? 1 : ratio;

        //adjust the initial count to the max count if the ratio give just one segment
        if (ratio < 2)
        {
            objectCountPerSegment = maximumNumberOfObjectInPool;
        }

        m_objectCountsInSegment = objectCountPerSegment;
        m_segmentCount = 0;
        m_segmentCandidateForDeletion = 0;
        //when we reach 60% of segment and there is a candidate for deletion, we delete the candidate.
        //We do this to prevent deleting a segment once it it became full empty. We always want to keep one full empty segment to avoid alloc/free of a segment on short period of time.
        m_candidateDeletionLimit = (uint32_t)(objectCountPerSegment * 0.60);

        //don't do create initial segments if the MaxCount is Infinite !
        if (maximumNumberOfObjectInPool != U32_Max)
        {
            //allocate needed segments (default : 33% of the total segment count)
            uint32_t initialSegmentCount = ((uint32_t)GetInteger(((float)m_maxSegmentCount * (0.5f + initialyCreatedRatio))));

            for (uint32_t i = 0; i < initialSegmentCount; i++)
            {
                if (!AllocateSegment())
                {
                    BEHAVIAC_ASSERT(false);
                }
            }
        }

        BEHAVIAC_DEBUGCODE(m_peakAllocatedCount = 0);
        BEHAVIAC_DEBUGCODE(m_totalAllocatedCount = 0);
        return true;
    }

    //Allocate one object calling the object's default constructor.
    template <typename TYPE, typename TALLOCATOR, typename MUTEX_TYPE>
    inline TYPE* BasicObjectPool<TYPE, TALLOCATOR, MUTEX_TYPE>::AllocateDefault()
    {
        TYPE* ptr;
        AllocateInternal(ptr);

        if (ptr)
        {
            return new(ptr)TYPE;

        }
        else
        {
            return NULL;
        }
    }

    //Allocate one object.
    //NOTE: YOU MUST CALL THE PLACEMENT NEW ON THIS POINTER TO INITIALISE YOUR OBJECT. ex: new(pool.Allocate())Class;
    template <typename TYPE, typename TALLOCATOR, typename MUTEX_TYPE>
    inline TYPE* BasicObjectPool<TYPE, TALLOCATOR, MUTEX_TYPE>::Allocate()
    {
        TYPE* ptr;
        AllocateInternal(ptr);
        return ptr;
    }

    template <typename TYPE, typename TALLOCATOR, typename MUTEX_TYPE>
    inline void BasicObjectPool<TYPE, TALLOCATOR, MUTEX_TYPE>::AllocateInternal(TYPE*& ptr)
    {
        PoolSegment*   segment;
        void*          object = 0;
        //lock
        behaviac::ScopedLock criticalSection(m_lock);

        if (!m_freeObjectCount)
        {
            // no objects left then check if there is a candidate left, if not allocate new segment
            if (m_segmentCount < m_maxSegmentCount)
            {
                if (m_segmentCandidateForDeletion != 0)
                {
                    // use the candidate for deletion
                    m_segmentList.PushFront(*m_segmentCandidateForDeletion);
                    m_segmentCandidateForDeletion = 0;
                    // correct counts
                    m_freeObjectCount += m_objectCountsInSegment;
                    m_segmentCount++;

                }
                else
                {
                    bool Valid = AllocateSegment();
                    BEHAVIAC_ASSERT(Valid);

                    if (!Valid)
                    {
                        ptr = 0;
                        return;
                    }
                }
            }
            else
            {
                ptr = 0;
                return;
            }
        }

        //at least one object free if here
        BEHAVIAC_ASSERT(m_freeObjectCount != 0);
        //there is at least one object free the first segment
        segment = m_segmentList.Front();
        BEHAVIAC_DEBUGCODE(BEHAVIAC_ASSERT(segment->m_safety == PoolSegment::Val));
        //pop the object
        object = segment->m_freeObjectList.Pop();

        if (!--segment->m_freeObjectCount)
        {
            //the segment is all taken, send it to the end of the list.
            BEHAVIAC_ASSERT(segment->m_freeObjectList.Empty());
            BEHAVIAC_ASSERT(!m_segmentList.Empty());
            m_segmentList.Remove(*segment);
            m_segmentList.PushBack(*segment);
        }

        m_freeObjectCount--;
#ifdef _DEBUG
        m_totalAllocatedCount++;

        if (m_totalAllocatedCount > m_peakAllocatedCount)
        {
            m_peakAllocatedCount = m_totalAllocatedCount;
        }

#endif
        ptr = (TYPE*)object;
    }

    //Free one object from the object pool
    template <typename TYPE, typename TALLOCATOR, typename MUTEX_TYPE>
    inline void BasicObjectPool<TYPE, TALLOCATOR, MUTEX_TYPE>::Free(TYPE* object)
    {
        Free(object, true);
    }

    //Free one object from the object pool
    template <typename TYPE, typename TALLOCATOR, typename MUTEX_TYPE>
    inline void BasicObjectPool<TYPE, TALLOCATOR, MUTEX_TYPE>::Free(TYPE* object, bool callDestructor)
    {
        BEHAVIAC_ASSERT(object);
        PoolSegment* segment;
        //lock
        behaviac::ScopedLock criticalSection(m_lock);
        //validate safety val
        BEHAVIAC_DEBUGCODE(BEHAVIAC_ASSERT(((BlockHeader*)(((uint8_t*)object) - sizeof(BlockHeader)))->m_safety == BlockHeader::Val));
        uint8_t* p = (uint8_t*)object;
        p -= sizeof(BlockHeader);
        segment = (PoolSegment*)(p - (((BlockHeader*)p)->m_index * (sizeof(BlockHeader) + m_objectSize + m_objectAlign)) - m_segmentAlign) - 1;
        //check validity of segment
        BEHAVIAC_DEBUGCODE(BEHAVIAC_ASSERT(segment->m_safety == PoolSegment::Val));

        //call the object's destructor
        if (callDestructor)
        {
            object->~TYPE();
        }

        //push freed object to segment free objects list
        segment->m_freeObjectList.Push((SLink*)object);
        segment->m_freeObjectCount++;

        //if this segment is not front of the segments list insert it to front (optimization)
        if (segment != m_segmentList.Front())
        {
            m_segmentList.Remove(*segment);
            m_segmentList.PushFront(*segment);
        }

        // check if we can delete the candidate for deletion segment (because the this segment is the next candidate)
        if (m_segmentCandidateForDeletion != 0 && segment->m_freeObjectCount > m_candidateDeletionLimit)
        {
            BEHAVIAC_ALLOCATOR_FREEALIGNED(this->m_allocator, m_segmentCandidateForDeletion, BEHAVIAC_ALIGNOF(TYPE));
            m_segmentCandidateForDeletion = 0;
        }

        if (segment->m_freeObjectCount == m_objectCountsInSegment)
        {
            // if all the segment is freed and the candidate is still allocated, forget this segment and count on the candidate
            if (m_segmentCandidateForDeletion != 0)
            {
                FreeSegment(segment);

            }
            else
            {
                // no candidate segment for deletion
                // use this segment as a candidate segment for deletion
                m_segmentCandidateForDeletion = segment;
                m_segmentList.Remove(*m_segmentCandidateForDeletion);
                DList::Clear(m_segmentCandidateForDeletion);
                m_freeObjectCount -= m_objectCountsInSegment;
                m_segmentCount--;
            }
        }

        m_freeObjectCount++;
        BEHAVIAC_DEBUGCODE(m_totalAllocatedCount--);
    }

    template <typename TYPE, typename TALLOCATOR, typename MUTEX_TYPE>
    inline void BasicObjectPool<TYPE, TALLOCATOR, MUTEX_TYPE>::Destroy(bool forceDestroy)
    {
        //lock
        behaviac::ScopedLock criticalSection(m_lock);
        typename DList::DRoot<PoolSegment>::DIterator it(m_segmentList);

        if (!forceDestroy)
        {
            BEHAVIAC_ASSERT(m_freeObjectCount == (m_segmentCount * m_objectCountsInSegment));
        }

        //free all segments
        while (!m_segmentList.Empty())
        {
            PoolSegment* segment = it.Current();
            BEHAVIAC_DEBUGCODE(BEHAVIAC_ASSERT(segment->m_safety == PoolSegment::Val));

            if (forceDestroy)
            {
                segment->m_freeObjectCount = m_objectCountsInSegment;
            }

            ++it;
            FreeSegment(segment);
        }

        //free candidate for deletion if there is one
        if (m_segmentCandidateForDeletion != 0)
        {
            BEHAVIAC_ALLOCATOR_FREEALIGNED(this->m_allocator, m_segmentCandidateForDeletion, BEHAVIAC_ALIGNOF(TYPE));
            m_segmentCandidateForDeletion = 0;
        }

        if (forceDestroy)
        {
            m_freeObjectCount = 0;
        }
    }

    //Allocates a Segment
    template <typename TYPE, typename TALLOCATOR, typename MUTEX_TYPE>
    inline bool BasicObjectPool<TYPE, TALLOCATOR, MUTEX_TYPE>::AllocateSegment()
    {
        PoolSegment* segment;
        //calculate segment size
        uint32_t segment_size = m_segmentAlign + sizeof(PoolSegment) + m_objectCountsInSegment * (sizeof(BlockHeader) + m_objectSize + m_objectAlign);
        //allocate segment
        segment = (PoolSegment*)BEHAVIAC_ALLOCATOR_MALLOCALIGNED(this->m_allocator, segment_size, BEHAVIAC_ALIGNOF(TYPE));
        BEHAVIAC_ASSERT(segment);

        if (segment == 0)
        {
            return false;
        }

        BEHAVIAC_DEBUGCODE(segment->m_safety = PoolSegment::Val);
        //init free object list
        segment->m_freeObjectList.Initialize();
        //initialize segment's dlist chain
        DList::Clear(segment);
        //all objects segment are initially free
        segment->m_freeObjectCount = m_objectCountsInSegment;
        //prepare a counterptr and (skip the PoolSegment's structure)
        uint8_t* p = (uint8_t*)(segment + 1);
        p += m_segmentAlign;

        for (uint32_t i = 0; i < m_objectCountsInSegment; i++)
        {
            BEHAVIAC_DEBUGCODE(((BlockHeader*)p)->m_safety = BlockHeader::Val);
            ((BlockHeader*)p)->m_index = i;
            //use the object itself for the linked list of free object the segment
            p += sizeof(BlockHeader);
            segment->m_freeObjectList.Push((SLink*)p);
            p += m_objectSize;
            p += m_objectAlign;
        }

        //insert segment to segments list
        m_segmentList.PushFront(*segment);
        m_freeObjectCount += m_objectCountsInSegment;
        m_segmentCount++;
        return true;
    }

    template <typename TYPE, typename TALLOCATOR, typename MUTEX_TYPE>
    inline void BasicObjectPool<TYPE, TALLOCATOR, MUTEX_TYPE>::FreeSegment(PoolSegment* segment)
    {
        BEHAVIAC_ASSERT(segment != 0);
        BEHAVIAC_ASSERT(segment->m_freeObjectCount == m_objectCountsInSegment);
        BEHAVIAC_DEBUGCODE(BEHAVIAC_ASSERT(segment->m_safety == PoolSegment::Val));
        m_freeObjectCount -= m_objectCountsInSegment;
        m_segmentCount--;
        m_segmentList.Remove(*segment);
        BEHAVIAC_ALLOCATOR_FREEALIGNED(this->m_allocator, segment, BEHAVIAC_ALIGNOF(TYPE));
        segment = 0;
    }

#if BEHAVIAC_COMPILER_MSVC
#pragma warning(pop)
#endif

    //The default object pool
    template<typename TYPE, typename TALLOCATOR = behaviac::IMemAllocator, typename MUTEX_TYPE = behaviac::Mutex>
    class ObjectPool : public BasicObjectPool<TYPE, TALLOCATOR, MUTEX_TYPE>
    {
    public:
        ObjectPool(TALLOCATOR* pAllocator = &GetMemoryAllocator()) : BasicObjectPool<TYPE, TALLOCATOR, MUTEX_TYPE>(pAllocator) {}

        ObjectPool(uint32_t objectCountPerSegment, uint32_t maximumNumberOfObjectInPool,
                   float initialyCreatedRatio = 0.33f, uint32_t extraDataSpacePerObject = 0,
                   TALLOCATOR* pAllocator = &GetMemoryAllocator()) :
            BasicObjectPool<TYPE, TALLOCATOR, MUTEX_TYPE>(
                objectCountPerSegment, maximumNumberOfObjectInPool,
                initialyCreatedRatio, extraDataSpacePerObject, pAllocator)
        {}
    };
}//end of ns

/*! @} */
/*! @} */

#endif //BEHAVIAC_MEMEORY_OBJECTPOOL_H
