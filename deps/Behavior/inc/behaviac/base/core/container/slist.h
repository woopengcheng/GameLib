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

#ifndef BEHAVIAC_BASE_SLIST_H
#define BEHAVIAC_BASE_SLIST_H

#include "behaviac/base/core/config.h"
#include "behaviac/base/core/assert_t.h"

namespace behaviac
{
    /// Single Linked list declaration for elements that belong to a list
    /// \sa SList
#define BEHAVIAC_DECLARE_SLIST(CLASSNAME)                           \
public:                                                         \
    enum {LLIST_MAX_LINKS = 1};                                 \
    behaviac::SList::SNode<CLASSNAME, 1>     m_llist;

    /// Single Linked list declaration for elements that belong to 2 different lists
    /// \sa SList
#define BEHAVIAC_DECLARE_SLIST2(CLASSNAME, RName1, RName2)          \
public:                                                         \
    enum {RName1            = 0};                               \
    enum {RName2            = 1};                               \
    enum {LLIST_MAX_LINKS   = 2};                               \
    behaviac::SList::SNode<CLASSNAME, 2>     m_llist;

    /// Single Linked list declaration for elements that belong to 3 different lists
    /// \sa SList
#define BEHAVIAC_DECLARE_SLIST3(CLASSNAME, RName1, RName2, RName3)  \
public:                                                         \
    enum {RName1            = 0};                               \
    enum {RName2            = 1};                               \
    enum {RName3            = 2};                               \
    enum {LLIST_MAX_LINKS   = 3};                               \
    behaviac::SList::SNode<CLASSNAME, 3>     m_llist;

    /// Single Linked list declaration to put in your class declaration.
    /*! This is a typical single linked list class. Its main advantage reside in the fact the there is no
    	dynamic allocation involved, since all your elements object will have in their  next pointer
    	space already reserved in their object. To reserve this space in your element object, use the BEHAVIAC_DECLARE_SLIST
    	macro. (You can use BEHAVIAC_DECLARE_SLIST2, BEHAVIAC_DECLARE_SLIST3, etc. if you want your object to be the element of
    	more than a list at the time).
    	Here's an example how to declare your object as a single linked list node.
    	\code
    	class MyClass
    	{
    	BEHAVIAC_DECLARE_SLIST(MyClass)

    	public:
    	MyClass() {  }
    	};
    	\endcode
    	*/
    class BEHAVIAC_API SList
    {
    public:

        ///Basic entity (node) in the list
        template <typename NODETYPE, int NBLIST>
        class SNode
        {
        public:
            /// Constructor
            inline SNode()
            {
                Clear();
            }

            /// Destructor
            /** The List must be unlinked first, with Clear, RemoveHead.
              *    \sa Clear, SRoot::RemoveHead, SRoot::Clear
              */
            inline ~SNode()
            {
                BEHAVIAC_ASSERT(!IsLinked());
            }

            /// Clear all the Node
            inline void Clear(void)
            {
                for (unsigned i = 0; i < NBLIST; i++)
                {
                    Clear(i);
                }
            }

            /// Clear the node from the list[ListNb]
            inline void Clear(int ListNb)
            {
                BEHAVIAC_ASSERT(ListNb < NBLIST);
                m_pNext[ListNb] = 0;
            }

            /// Returns true if the node has its next pointer link to another Node in the list.
            /// WARNING: When returns false, it doesn't mean that Node is NOT in a linked list.
            inline bool IsLinked(void) const
            {
                for (unsigned i = 0; i < NBLIST; i++)
                    if (IsLinked(i))
                    {
                        return true;
                    }

                return false;
            }

            /// Returns true if the node is in a linked list
            /** WARNING: When returns false, it doesn't mean that Node is NOT in a linked list.
              * \param ListNb The number of the node you want to reset
              */
            inline bool IsLinked(int ListNb) const
            {
                BEHAVIAC_ASSERT(ListNb < NBLIST);
                return  m_pNext[ListNb] != 0;
            }

            /// Next pointer
            NODETYPE*      m_pNext[NBLIST];
        }; // Class Node

        // Returns true if the node is in a linked list
        // WARNING: When returns false, it doesn't mean that Node is NOT in a linked list.
        template <typename NODETYPE>
        inline static bool IsLinked(const NODETYPE& node);

        // Returns true if the node is in a linked list
        // WARNING: When returns false, it doesn't mean that Node is NOT in a linked list.
        template <typename NODETYPE>
        inline static bool IsLinked(const NODETYPE& node, int ListNb);

        /// SList SRoot is the root object that will handle the single linked list
        /*! The root object is a template, and receive an object type as template parameter. This object
        	type must have the BEHAVIAC_DECLARE_SLIST in his declaration.
        	*/
        template <typename NODETYPE, int LISTNB = 0>
        class SRoot
        {
        public:
            /// Constructor sets the SRoot as an empty list
            inline SRoot() : m_pHead(0), m_pTail(0)
            {
                BEHAVIAC_STATIC_ASSERT(LISTNB < NODETYPE::LLIST_MAX_LINKS);
            }

            /// Add an new element at the beginning (Head) of the single linked list
            /*! The Element must be initialized and not currently in a list
            \param NewNode A reference on the node to add
            \return None
            */
            inline void PushFront(NODETYPE& NewNode)
            {
                BEHAVIAC_ASSERT(!IsLinked(NewNode, LISTNB));
                BEHAVIAC_ASSERT_EX(!Find(NewNode));

                if (this->m_pHead)
                {
                    NewNode.m_llist.m_pNext[LISTNB] = this->m_pHead;
                    this->m_pHead = &NewNode;

                }
                else
                {
                    this->m_pHead = this->m_pTail = &NewNode;
                }
            }

            /// Add an new element at the end (Tail) of the single linked list
            /*! The Element must be initialized and not currently in a list
            \param NewNode A reference on the node to add
            \return None
            */
            inline void PushBack(NODETYPE& NewNode)
            {
                BEHAVIAC_ASSERT(!IsLinked(NewNode, LISTNB));
                BEHAVIAC_ASSERT_EX(!Find(NewNode));

                if (this->m_pTail)
                {
                    this->m_pTail->m_llist.m_pNext[LISTNB] = &NewNode;
                    this->m_pTail = &NewNode;

                }
                else
                {
                    this->m_pHead = this->m_pTail = &NewNode;
                }
            }

            /// Insert an element in a single linked list after a specified node
            /*! The Element must be initialized and not currently in a list
            	\param CurrentNode The Node currently in the list, that will be linked before the new node
            	\param NewNode A reference on the node to add
            	\return None
            	*/
            inline void InsertAfter(NODETYPE& CurrentNode, NODETYPE& NewNode)
            {
                BEHAVIAC_ASSERT(!IsLinked(NewNode, LISTNB));
                BEHAVIAC_ASSERT_EX(!Find(NewNode));
                BEHAVIAC_ASSERT_EX(this->Find(CurrentNode));
                NewNode.m_llist.m_pNext[LISTNB] = CurrentNode.m_llist.m_pNext[LISTNB];
                CurrentNode.m_llist.m_pNext[LISTNB] = &NewNode;

                // If NewNode has no next pointer, its the Tail
                if (!NewNode.m_llist.m_pNext[LISTNB])
                {
                    this->m_pTail = &NewNode;
                }
            }

            /// Remove an element in a single linked list after a specified node
            /*! \param pPreviousNode A pointer on the node just before the node to remove.
            						 Null an be sent if you remove the head.
            						 \return The Element after the specified element or NULL if pPreviousNode is the
            						 last element of the list.
            						 */
            inline NODETYPE* PopAfter(NODETYPE* pPreviousNode)
            {
                NODETYPE* pResult = 0;

                if (pPreviousNode == 0)
                {
                    // If this is the head, just pop the firs element
                    pResult = PopFront();

                }
                else if (pPreviousNode != this->m_pTail)
                {
                    // Otherwise, just unlink the element. If we are the tail, that
                    // means we have no next.
                    pResult = pPreviousNode->m_llist.m_pNext[LISTNB];
                    BEHAVIAC_ASSERT(pResult);
                    pPreviousNode->m_llist.m_pNext[LISTNB] = pResult->m_llist.m_pNext[LISTNB];
                    pResult->m_llist.Clear(LISTNB);

                    if (pResult == this->m_pTail)
                    {
                        this->m_pTail = pPreviousNode;
                    }
                }

                return pResult;
            }

            /// Unlink the first element of the single linked list (Head).
            /*! If the list is empty, 0 is returned.
            	\return A pointer on the first element, removed from the list or 0 if the list is empty.
            	\sa Empty()
            	*/
            inline NODETYPE* PopFront(void)
            {
                NODETYPE* pResult = this->m_pHead;

                if (this->m_pHead)
                {
                    this->m_pHead = this->m_pHead->m_llist.m_pNext[LISTNB];

                    if (!this->m_pHead)
                    {
                        this->m_pTail = 0;
                    }

                    pResult->m_llist.Clear(LISTNB);
                }

                return(pResult);
            }

            /// Unlinks all elements of a single linked list.
            /*!
            \return None
            \sa PopFront()
            */
            inline void Clear(void)
            {
                while (PopFront() != 0) {}
            }

            /// Return the status of the list
            /*! \return true if the list is empty, false if in contains at least 1 element
            */
            inline bool Empty(void) const
            {
                return (this->m_pHead == 0);
            }

            /// Find if a node is in the list or not.
            /*! \param ToFind A reference on the node to find.
            	\return true if the node passed in parameter is in the list, false if the node haven't been found.
            	*/
            inline bool Find(const NODETYPE& ToFind)
            {
                NODETYPE*  pIndex = this->m_pHead;

                while ((pIndex) && (pIndex != &ToFind))
                {
                    pIndex = pIndex->m_llist.m_pNext[LISTNB];
                }

                return(pIndex != 0);
            }

            /// Returns the number of elements in the list
            /*! Scan the SList & Count the number of elements in it.
            	WARNING: This function can be slow since it pass all the elements in the list.
            	Should be used only for testing & stats purpose.
            	*/
            inline size_t size(void) const
            {
                unsigned    Result = 0;
                NODETYPE*  pIndex = this->m_pHead;

                while (pIndex)
                {
                    pIndex = pIndex->m_llist.m_pNext[LISTNB];
                    Result++;
                }

                return Result;
            }

            /// Return a pointer on the first element of a single linked list (Head) without removing it.
            /*! \return A pointer on the first element or 0 if the list is empty
            */
            NODETYPE* Front(void) const
            {
                return this->m_pHead;
            }

            /// Return a pointer on the last element of a single linked list (Tail) without removing it.
            /*! \return A pointer on the last element or 0 if the list is empty
            */
            NODETYPE* Back(void) const
            {
                return this->m_pTail;
            }

            /// Return the ListNb (sub list name). This is mainly for the iterator
            int       GetListNb()
            {
                return LISTNB;
            }

        protected:

            NODETYPE*  m_pHead;            /// A pointer on the first node of the list
            NODETYPE*  m_pTail;            /// A Pointer on the last node of the list

        public:
            /// Iterator is used to access a single linked list
            /*! An Iterator is always created from a root single linked list. By default, it internal index
            will always point on the first element of the single linked list (the head).
            */
            class SIterator
            {
            public:

                /// The only Constructor available accept a SRoot as parameter
                inline SIterator(SRoot<NODETYPE, LISTNB>& Head) : m_pHead(&Head)
                {
                    this->Begin();
                }

                /// Reset the Iterator's internal index to the first element (Head) of the single linked list
                inline void Begin(void)
                {
                    this->m_pIndex = this->m_pHead->Front();
                }

                /// Reset the Iterator's internal index to the last element (Tail) of the single linked list
                inline void End(void)
                {
                    this->m_pIndex = this->m_pHead->Back();
                }

                /// Returns true if the Iterator has reach the end
                inline bool Empty(void) const
                {
                    return (this->m_pIndex == 0);
                }

                /// The * operator returns a pointer on the current element (or 0 if none).
                /*! \return a pointer o the current element
                \sa Current()
                */
                inline NODETYPE* operator*(void) const
                {
                    return this->m_pIndex;
                }

                /// Returns a pointer on the current element (or 0 if none).
                /*! \sa operator*()
                */
                inline NODETYPE* Current(void) const
                {
                    return this->m_pIndex;
                }

                /// Go the next next element in the single linked list (move from head toward the tail)
                /*! Will Assert if the Iterator has reach the end.
                \sa Empty()
                */
                inline SIterator&  operator++()
                {
                    BEHAVIAC_ASSERT(this->m_pIndex);
                    this->m_pIndex = this->m_pIndex->m_llist.m_pNext[this->m_pHead->GetListNb()];
                    return *this;
                }

                inline SIterator   operator++(int)
                {
                    BEHAVIAC_ASSERT(this->m_pIndex);
                    SIterator Backup(*this);
                    ++*this;
                    return Backup;
                }

                /// Go the next nth element in the single linked list (move from head toward the tail)
                /*! Will Assert if the Iterator has reach the end.
                \sa Empty()
                */
                inline SIterator&  operator+=(int cnt)
                {
                    BEHAVIAC_ASSERT(this->m_pIndex);

                    if (cnt < 0)
                    {
                        cnt = -cnt;
                    }

                    while (cnt && this->m_pIndex)
                    {
                        this->m_pIndex = this->m_pIndex->m_llist.m_pNext[this->m_pHead->GetListNb()];
                        cnt--;
                    }

                    return *this;
                }
                /// Compare two Iterator index
                /*! \return Returns true if they point on the same element in the single linked list.
                \sa operator!=()
                */
                inline bool operator==(const SIterator& Other) const
                {
                    return this->m_pIndex == Other.m_pIndex;
                }

                /// Compare two Iterator index
                /*! \return Returns false if they point on the same element in the single linked list.
                \sa operator!=()
                */
                inline bool operator!=(const SIterator& Other) const
                {
                    return !(*this == Other);
                }

                /// Search for an element in the single linked list, going from head to tail.
                /*!If the element is found, true is returned, and the internal Iterator index will point
                on the found  element (See Current()). If the element is not found, Current will
                point on null.
                \param ToFind A reference on the node to find
                \return Returns true if the element has been found, false if not found.
                \sa Current(), Begin()
                */
                inline bool FindForward(const NODETYPE& ToFind)
                {
                    while (!this->Empty() && (this->m_pIndex != &ToFind))
                    {
                        ++(*this);
                    }

                    return(!this->Empty());
                }

            protected:

                SRoot<NODETYPE, LISTNB>*      m_pHead;        /// A pointer on the root object
                NODETYPE*                    m_pIndex;       /// The Iterator internal index
            }; // class SIterator
        }; // class SRoot

        //// Generic Services
        template <typename NODETYPE>
        inline static void Clear(NODETYPE* node);

        template <typename NODETYPE>
        inline static void Clear(NODETYPE& node, int LISTNB);

        template <typename NODETYPE>
        inline static NODETYPE* GetNext(NODETYPE& node);

        template <typename NODETYPE>
        inline static NODETYPE* GetNext(NODETYPE& node, int LISTNB);
    }; // class SList

    /////////////////////////////////////////////////////////////////////////////////////////////////////////
    //
    //                      Inlines For Class SList
    //

    template <typename NODETYPE>
    inline bool SList::IsLinked(const NODETYPE& node)
    {
        return node.m_llist.IsLinked();
    }

    template <typename NODETYPE>
    inline bool SList::IsLinked(const NODETYPE& node, int ListNb)
    {
        return node.m_llist.IsLinked(ListNb);
    }

    //// Generic Services
    template <typename NODETYPE>
    inline void SList::Clear(NODETYPE* node)
    {
        node->m_llist.Clear();
    }

    template <typename NODETYPE>
    inline void SList::Clear(NODETYPE& node, int LISTNB)
    {
        node.m_llist.Clear(LISTNB);
    }

    template <typename NODETYPE>
    inline NODETYPE* SList::GetNext(NODETYPE& node)
    {
        return node.m_llist.m_pNext[0];
    }

    template <typename NODETYPE>
    inline NODETYPE* SList::GetNext(NODETYPE& node, int LISTNB)
    {
        return node.m_llist.m_pNext[LISTNB];
    }
}//end of namespace

#endif // #ifndef BEHAVIAC_BASE_SLIST_H
