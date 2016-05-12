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

#ifndef BEHAVIAC_BASE_DLIST_H
#define BEHAVIAC_BASE_DLIST_H

#include "behaviac/base/core/config.h"
#include "behaviac/base/core/assert_t.h"
#include "behaviac/base/core/container/slist.h"

namespace behaviac
{
    /// Double Linked list declaration for elements that belong to a list
    /// \sa DList
#define BEHAVIAC_DECLARE_DLIST(CLASSNAME)                           \
public:                                                         \
    enum {LLIST_MAX_LINKS = 1};                                 \
    behaviac::DList::DNode<CLASSNAME, 1>     m_llist

    /// Double Linked list declaration for elements that belong to 2 different lists
    /// \sa DList
#define BEHAVIAC_DECLARE_DLIST2(CLASSNAME, RName1, RName2)          \
public:                                                         \
    enum {RName1            = 0};                               \
    enum {RName2            = 1};                               \
    enum {LLIST_MAX_LINKS   = 2};                               \
    behaviac::DList::DNode<CLASSNAME, 2>     m_llist

    /// Double Linked list declaration for elements that belong to 3 different lists
    /// \sa DList
#define BEHAVIAC_DECLARE_DLIST3(CLASSNAME, RName1, RName2, RName3)  \
public:                                                         \
    enum {RName1            = 0};                               \
    enum {RName2            = 1};                               \
    enum {RName3            = 2};                               \
    enum {LLIST_MAX_LINKS   = 3};                               \
    behaviac::DList::DNode<CLASSNAME, 3>     m_llist

    /// Double Linked list declaration to put in your class declaration.
    /*! This is a typical double linked list class. Its main advantage resides in the fact that there is no
    	dynamic allocation involved, since all your element objects will have in their previous & next pointer
    	space already reserved in their object. To reserve this space in your element object, use the BEHAVIAC_DECLARE_DLIST
    	macro. (You can use BEHAVIAC_DECLARE_DLIST2, BEHAVIAC_DECLARE_DLIST3, etc. if you want your object to be the element of
    	more than a list at the time).
    	Here's an example how to declare your object as a double linked list node.
    	\code
    	class MyClass
    	{
    	BEHAVIAC_DECLARE_DLIST(MyClass)

    	public:
    	MyClass() {  }
    	};
    	\endcode

    	Note:

    	Howard Hinnant Jul 4 1998, 2:00 am

    	You're not doing anything wrong.  This is a limitation of the current
    	compiler.  Nested classes of template classes must be defined within the
    	class.  Work is underway to remove this restriction.

    	-Howard

    	Senior Library and Performance Engineer
    	Metrowerks

    	---
    	This is the case on CodeWarrior on GameCube.
    	*/
    class BEHAVIAC_API DList : public SList
    {
    public:

        ///Basic entity (node) in the list
        template <typename NODETYPE, int NBLIST>
        class DNode
        {
        public:
            /// Constructor
            inline DNode()
            {
                Clear();
            }

            /// Destructor
            /** The List must be unlinked first, with Clear, RemoveHead.
            *    \sa Clear, DRoot::RemoveHead, DRoot::Clear
            */
            inline ~DNode()
            {
                BEHAVIAC_ASSERT(!IsLinked());
            }

            /// Clear all the Node
            inline void Clear(void)
            {
                for (int i = 0; i < NBLIST; i++)
                {
                    Clear(i);
                }
            }

            /// Clear the node from the list[ListNb]
            inline void Clear(int ListNb)
            {
                BEHAVIAC_ASSERT(ListNb < NBLIST);
                m_pPrev[ListNb] = m_pNext[ListNb] = 0;
            }

            /// Returns true if the node has its previous or next pointer link to another Node in the list.
            /// WARNING: When returns false, it doesn't mean that Node is NOT in a linked list.
            inline bool IsLinked(void) const
            {
                for (int i = 0; i < NBLIST; i++)
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
                return m_pPrev[ListNb] || m_pNext[ListNb];
            }

            /// Next pointer
            NODETYPE*      m_pNext[NBLIST];
            /// Previous pointer
            NODETYPE*      m_pPrev[NBLIST];
        }; // Class Node

        //find could be very slow
        //#define _SLOW_ASSERT(x) BEHAVIAC_ASSERT(x)
#define _SLOW_ASSERT(x)

        /// DList DRoot is the root object that will handle the double linked list
        /*! The root object is a template, and receive an object type as template parameter. This object
        	type must have the BEHAVIAC_DECLARE_DLIST in his declaration.
        	*/
        template <typename NODETYPE, int LISTNB = 0>
        class DRoot : public SList::SRoot<NODETYPE, LISTNB>
        {
        public:

            /// Constructor sets the SRoot as an empty list
            inline DRoot() : SList::SRoot<NODETYPE, LISTNB>()
            {
                BEHAVIAC_STATIC_ASSERT(LISTNB < NODETYPE::LLIST_MAX_LINKS);
            }

            /// Add an new element at the beginning (Head) of the Double linked list
            /*! The Element must be initialized and not currently in a list
            \param NewNode A reference on the node to add
            \return None
            */
            inline void PushFront(NODETYPE& NewNode)
            {
                BEHAVIAC_ASSERT(!IsLinked(NewNode, LISTNB));
                _SLOW_ASSERT(!this->Find(NewNode));

                if (this->m_pHead)
                {
                    InsertBefore(*this->m_pHead, NewNode);

                }
                else
                {
                    this->m_pHead = this->m_pTail = &NewNode;
                }
            }

            /// Add an new element at the end (Tail) of the Double linked list
            /*! The Element must be initialized and not currently in a list
            \param NewNode A reference on the node to add
            \return None
            */
            inline void PushBack(NODETYPE& NewNode)
            {
                BEHAVIAC_ASSERT(!IsLinked(NewNode, LISTNB));
                _SLOW_ASSERT(!this->Find(NewNode));
                NODETYPE* node = this->m_pTail;

                if (node)
                {
                    InsertAfter(*(node), NewNode);

                }
                else
                {
                    this->m_pHead = this->m_pTail = &NewNode;
                }
            }

            /// Insert an element in a double linked list before a specified node
            /*! The Element must be initialized and not currently in a list
            	\param CurrentNode The Node currently in the list, that will be linked after the new node
            	\param NewNode A reference on the node to add
            	\return None
            	\sa InsertAfter()
            	*/
            inline void InsertBefore(NODETYPE& CurrentNode, NODETYPE& NewNode)
            {
                BEHAVIAC_ASSERT(!IsLinked(NewNode, LISTNB));
                _SLOW_ASSERT(!this->Find(NewNode));
                _SLOW_ASSERT(this->Find(CurrentNode));
                NewNode.m_llist.m_pNext[LISTNB] = &CurrentNode;
                NewNode.m_llist.m_pPrev[LISTNB] = CurrentNode.m_llist.m_pPrev[LISTNB];
                CurrentNode.m_llist.m_pPrev[LISTNB] = &NewNode;

                // The previous node of NewNode must have its next pointer point to NewNode
                if (NewNode.m_llist.m_pPrev[LISTNB])
                {
                    NewNode.m_llist.m_pPrev[LISTNB]->m_llist.m_pNext[LISTNB] = &NewNode;

                }
                else
                {
                    this->m_pHead = &NewNode;
                }
            }

            /// Insert an element in a double linked list after a specified node
            /*! The Element must be initialized and not currently in a list
            	\param CurrentNode The Node currently in the list, that will be linked before the new node
            	\param NewNode A reference on the node to add
            	\return None
            	\sa InsertBefore()
            	*/
            inline void InsertAfter(NODETYPE& CurrentNode, NODETYPE& NewNode)
            {
                BEHAVIAC_ASSERT(!IsLinked(NewNode, LISTNB));
                _SLOW_ASSERT(!this->Find(NewNode));
                _SLOW_ASSERT(this->Find(CurrentNode));
                NewNode.m_llist.m_pPrev[LISTNB] = &CurrentNode;
                NewNode.m_llist.m_pNext[LISTNB] = CurrentNode.m_llist.m_pNext[LISTNB];
                CurrentNode.m_llist.m_pNext[LISTNB] = &NewNode;

                // The next node of NewNode must have its previous pointer point to NewNode
                if (NewNode.m_llist.m_pNext[LISTNB])
                {
                    NewNode.m_llist.m_pNext[LISTNB]->m_llist.m_pPrev[LISTNB] = &NewNode;

                }
                else
                {
                    this->m_pTail = &NewNode;
                }
            }

            /// Unlink the first element of the double linked list (Head).
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

                    if (this->m_pHead)
                    {
                        this->m_pHead->m_llist.m_pPrev[LISTNB] = 0;

                    }
                    else
                    {
                        this->m_pTail = 0;
                    }

                    pResult->m_llist.Clear(LISTNB);
                }

                return(pResult);
            }

            /// Unlink the last element of the double linked list (Tail).
            /*! If the list is empty, 0 is returned.
            	\return A pointer on the last element, removed from the list or 0 if the list is empty.
            	\sa Empty()
            	*/
            inline NODETYPE* PopBack(void)
            {
                NODETYPE* pResult = this->m_pTail;

                if (this->m_pTail)
                {
                    this->m_pTail = this->m_pTail->m_llist.m_pPrev[LISTNB];

                    if (this->m_pTail)
                    {
                        this->m_pTail->m_llist.m_pNext[LISTNB] = 0;

                    }
                    else
                    {
                        this->m_pHead = 0;
                    }

                    pResult->m_llist.Clear(LISTNB);
                }

                return(pResult);
            }

            /// Unlink an element of a double linked list.
            /*! The Element must be in the specified list.
            	\return None
            	\sa PopFront(), PopBack()
            	*/
            inline void Remove(NODETYPE& OldNode)
            {
                _SLOW_ASSERT(this->Find(OldNode));

                // If OldNode have a previous node, her next pointer must point to OldNode next pointer.
                // Else, OldNode is the Head, and her next pointer must be the new Head.
                if (OldNode.m_llist.m_pPrev[LISTNB])
                {
                    OldNode.m_llist.m_pPrev[LISTNB]->m_llist.m_pNext[LISTNB] = OldNode.m_llist.m_pNext[LISTNB];

                }
                else if (this->m_pHead == &OldNode)
                {
                    this->m_pHead = OldNode.m_llist.m_pNext[LISTNB];

                }
                else
                {
                    BEHAVIAC_ASSERT(OldNode.m_llist.m_pNext[LISTNB] == 0);
                }

                // If, OldNode have a next node, her previous pointer must point to OldNode previous pointer.
                // Else, OldNode is the Tail, and her previous pointer must be the new Tail.
                if (OldNode.m_llist.m_pNext[LISTNB])
                {
                    OldNode.m_llist.m_pNext[LISTNB]->m_llist.m_pPrev[LISTNB] = OldNode.m_llist.m_pPrev[LISTNB];

                }
                else if (this->m_pTail == &OldNode)
                {
                    this->m_pTail = OldNode.m_llist.m_pPrev[LISTNB];

                }
                else
                {
                    BEHAVIAC_ASSERT(OldNode.m_llist.m_pPrev[LISTNB] == 0);
                }

                OldNode.m_llist.Clear(LISTNB);
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

            /// Unlink an element & put it back to the back of the list
            /*! The Element must be in the specified list.
            	\return None
            	\sa Remove(), PushBack()
            	*/
            inline void MoveToBack(NODETYPE& NodeToMove)
            {
                _SLOW_ASSERT(this->Find(NodeToMove));

                if (NodeToMove.m_llist.m_pNext[LISTNB])
                {
                    NodeToMove.m_llist.m_pNext[LISTNB]->m_llist.m_pPrev[LISTNB] = NodeToMove.m_llist.m_pPrev[LISTNB];

                    if (NodeToMove.m_llist.m_pPrev[LISTNB])
                    {
                        NodeToMove.m_llist.m_pPrev[LISTNB]->m_llist.m_pNext[LISTNB] = NodeToMove.m_llist.m_pNext[LISTNB];

                    }
                    else
                    {
                        this->m_pHead = NodeToMove.m_llist.m_pNext[LISTNB];
                    }

                    NodeToMove.m_llist.m_pPrev[LISTNB] = this->m_pTail;
                    NodeToMove.m_llist.m_pNext[LISTNB] = 0;
                    this->m_pTail->m_llist.m_pNext[LISTNB] = &NodeToMove;
                    this->m_pTail = &NodeToMove;
                }
            }
        public:

            /// Iterator is used to access a double linked list
            /*! An Iterator is always created from a root double linked list. By default, it internal index
            will always point on the first element of the double linked list (the head).
            */
            class DIterator : public SList::SRoot<NODETYPE, LISTNB>::SIterator
            {
            public:

                /// The only Constructor available accept a SRoot as parameter
                inline DIterator(DRoot<NODETYPE, LISTNB>& Head) : SList::SRoot<NODETYPE, LISTNB>::SIterator(Head)
                {
                    this->Begin();
                }

                /// Go to the previous element in the double linked list (move from tail toward the head)
                /*! Will Assert if the Iterator had already reached the last element.
                \sa Empty(), operator++()
                */
                inline DIterator&  operator--()
                {
                    BEHAVIAC_ASSERT(this->m_pIndex);
                    this->m_pIndex = this->m_pIndex->m_llist.m_pPrev[this->m_pHead->GetListNb()];
                    return *this;
                }

                inline DIterator   operator--(int)
                {
                    BEHAVIAC_ASSERT(this->m_pIndex);
                    DIterator Backup(*this);
                    --*this;
                    return Backup;
                }

                /// Go to the nth previous element in the double linked list (move from tail toward the head)
                /*! Will Assert if the Iterator had already reached the last element.
                \sa Empty(), operator++()
                */
                inline DIterator&  operator-=(int cnt)
                {
                    BEHAVIAC_ASSERT(this->m_pIndex);

                    if (cnt < 0)
                    {
                        cnt = -cnt;
                    }

                    while (cnt && this->m_pIndex)
                    {
                        this->m_pIndex = this->m_pIndex->m_llist.m_pPrev[this->m_pHead->GetListNb()];
                        cnt--;
                    }

                    return *this;
                }

                /// Search for an element in the double linked list, going from tail toward the head.
                /*! If the element is found, true is returned, and the internal Iterator index will point on
                the found  element (See Current()). If the element is not found, Current will point on null.
                \param ToFind A reference on the node to find
                \return Returns true if the element has been found, false if not found.
                \sa FindForward(), Current(), Begin()
                */
                inline bool FindReverse(const NODETYPE& ToFind)
                {
                    while (!this->Empty() && (this->m_pIndex != &ToFind))
                    {
                        --(*this);
                    }

                    return(!this->Empty());
                }

                /// Removed the current element pointed by the Iterator internal index from the double linked list.
                /*! The function will assert if there is no current element (Current() returns null or Empty()
                returns true).
                \return Returns a pointer on the removed element
                \sa Current(), Empty()
                */
                NODETYPE* RemoveCurrent(void)
                {
                    BEHAVIAC_ASSERT(this->m_pIndex);
                    NODETYPE* pTmp = this->m_pIndex;
                    ++(*this);
                    ((DRoot<NODETYPE, LISTNB>*)(this->m_pHead))->Remove(*pTmp);
                    return(pTmp);
                }
            }; // class DIterator
        }; // class DRoot

        //// Generic Services
        template <typename NODETYPE>
        inline static NODETYPE* GetPrev(NODETYPE& node);

        template <typename NODETYPE>
        inline static NODETYPE* GetPrev(NODETYPE& node, int LISTNB);
    }; // class DList

    /////////////////////////////////////////////////////////////////////////////////////////////////////////
    //
    //                      Inlines For Class DList
    //

    template <typename NODETYPE>
    inline NODETYPE* DList::GetPrev(NODETYPE& node)
    {
        return node.m_llist.m_pPrev[0];
    }

    template <typename NODETYPE>
    inline NODETYPE* DList::GetPrev(NODETYPE& node, int LISTNB)
    {
        return node.m_llist.m_pPrev[LISTNB];
    }
}//end of namespace

#endif // #ifndef BEHAVIAC_BASE_DLIST_H
