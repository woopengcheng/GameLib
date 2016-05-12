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

#ifndef BEHAVIAC_MEMORY_LINKEDOBJECTPOOL_H
#define BEHAVIAC_MEMORY_LINKEDOBJECTPOOL_H

#include "behaviac/base/core/memory/mempool.h"

namespace behaviac
{
    /*! \addtogroup Memory
     * @{
     * \addtogroup LinkedObjectPool
     * @{ */

    ///LinkedObjectPool
    /**
    derives from BasicObjectPool, the objects allocated by this linked pool can be retrived by the iterator

    \code
    behaviac::LinkedObjectPool<SomeStruct> objectPool;
    objectPool.Create(5, 10);

    behaviac::LinkedObjectPool<SomeStruct>::Iterator it = objectPool.Begin();

    while (!it.Empty())
    {
    SomeStruct* s = (*it);

    ++it;
    }

    \endcode
    */
    template <typename TYPE, typename TALLOCATOR = behaviac::IMemAllocator, typename MUTEX_TYPE = behaviac::Mutex>
    class LinkedObjectPool : public BasicObjectPool<TYPE, TALLOCATOR, MUTEX_TYPE>
    {
    private:
        /// Node shadow that represent a derivative class of TYPE + some data (dlist node)
        class NodeShadow
        {
        public:
            uint8_t m_shadow[sizeof(TYPE)];

            //BEHAVIAC_DECLARE_DLIST must be at the end because BasicObjectPool add the extra space at the end
            BEHAVIAC_DECLARE_DLIST(NodeShadow);
        }; // class NodeShadow

        typedef BasicObjectPool<TYPE, TALLOCATOR, MUTEX_TYPE> Super;

    public:
        // Iterator for Alive object
        class Iterator
        {
        public:

            inline Iterator(DList::DRoot<NodeShadow>& root) : m_iter(root)
            {
            }

            inline Iterator(const Iterator& it) : m_iter(it.m_iter)
            {
            }

            inline ~Iterator()
            {
            }

            inline TYPE* operator*(void)
            {
                return (TYPE*)m_iter.Current();
            }

            inline Iterator& operator++()
            {
                ++m_iter;
                return *this;
            }

            inline Iterator& operator--()
            {
                --m_iter;
                return *this;
            }

            inline Iterator& operator+=(uint32_t count)
            {
                m_iter += count;
                return *this;
            }

            inline Iterator& operator-=(uint32_t count)
            {
                m_iter -= count;
                return *this;
            }

            inline bool operator == (const Iterator& iter)
            {
                return m_iter == iter;
            }

            inline bool operator != (const Iterator& iter)
            {
                return m_iter != iter;
            }

            inline bool Empty()
            {
                return m_iter.Empty();
            }

        protected:

            typename DList::DRoot<NodeShadow>::DIterator m_iter;
        }; //class Iterator

        /// Begin iterator
        inline Iterator Begin();

        LinkedObjectPool();

        // Constructor will force extraDataSpacePerObject to be sizeof dlist node
        LinkedObjectPool(uint32_t objectCountPerSegment, uint32_t maximumNumberOfObjectInPool,
                         Float32 initialyCreatedRatio = 0.33f, TALLOCATOR* pAllocator = &GetMemoryAllocator());

        int GetElementSize() const
        {
            return sizeof(NodeShadow);
        }
        //////////////
        // OVERRIDES
        virtual bool Create(uint32_t objectCountPerSegment, uint32_t maximumNumberOfObjectInPool, Float32 initialyCreatedRatio = 0.33f, uint32_t extraDataSpacePerObject = 0);
        virtual void Free(TYPE* object);
        virtual void Free(TYPE* object, bool callDestructor);

    protected:
        //////////////////////////////
        //      OVERRIDE
        virtual void AllocateInternal(TYPE*& ptr);

        //list of living/allocated objects
        DList::DRoot<NodeShadow> m_root;
    }; //class LinkedObjectPool

    template <typename TYPE, typename TALLOCATOR, typename MUTEX_TYPE>
    inline typename LinkedObjectPool<TYPE, TALLOCATOR, MUTEX_TYPE>::Iterator LinkedObjectPool<TYPE, TALLOCATOR, MUTEX_TYPE>::Begin()
    {
        return Iterator(m_root);
    }

    template<typename TYPE, typename TALLOCATOR, typename MUTEX_TYPE>
    inline LinkedObjectPool<TYPE, TALLOCATOR, MUTEX_TYPE>::LinkedObjectPool() : BasicObjectPool<TYPE, TALLOCATOR, MUTEX_TYPE>()
    {
    }

    template<typename TYPE, typename TALLOCATOR, typename MUTEX_TYPE>
    inline LinkedObjectPool<TYPE, TALLOCATOR, MUTEX_TYPE>::LinkedObjectPool(uint32_t objectCountPerSegment, uint32_t maximumNumberOfObjectInPool, Float32 initialyCreatedRatio, TALLOCATOR* pAllocator) :
        BasicObjectPool<TYPE, TALLOCATOR, MUTEX_TYPE>(objectCountPerSegment, maximumNumberOfObjectInPool,
                                                      initialyCreatedRatio, sizeof(((NodeShadow*)0)->m_llist), pAllocator)
    {
    }

    template<typename TYPE, typename TALLOCATOR, typename MUTEX_TYPE>
    inline bool LinkedObjectPool<TYPE, TALLOCATOR, MUTEX_TYPE>::Create(uint32_t objectCountPerSegment, uint32_t maximumNumberOfObjectInPool, Float32 initialyCreatedRatio, uint32_t extraDataSpacePerObject)
    {
        BEHAVIAC_UNUSED_VAR(extraDataSpacePerObject);
        //override extraDataSpacePerObject, so clients could not pass bad values to the pool create method
        return Super::Create(objectCountPerSegment, maximumNumberOfObjectInPool, initialyCreatedRatio, sizeof(((NodeShadow*)0)->m_llist));
    }

    template<typename TYPE, typename TALLOCATOR, typename MUTEX_TYPE>
    inline void LinkedObjectPool<TYPE, TALLOCATOR, MUTEX_TYPE>::Free(TYPE* object)
    {
        Free(object, true);
    }

    template<typename TYPE, typename TALLOCATOR, typename MUTEX_TYPE>
    inline void LinkedObjectPool<TYPE, TALLOCATOR, MUTEX_TYPE>::Free(TYPE* object, bool callDestructor)
    {
        m_root.Remove(*(NodeShadow*)object);
        Super::Free(object, callDestructor);
    }

    template<typename TYPE, typename TALLOCATOR, typename MUTEX_TYPE>
    inline void LinkedObjectPool<TYPE, TALLOCATOR, MUTEX_TYPE>::AllocateInternal(TYPE*& ptr)
    {
        Super::AllocateInternal(ptr);

        if (ptr)
        {
            NodeShadow* listNode = (NodeShadow*)ptr;
            listNode->m_llist.Clear();
            m_root.PushBack(*listNode);
        }
    }
}//end of ns

/*! @} */
/*! @} */

#endif//BEHAVIAC_MEMORY_LINKEDOBJECTPOOL_H
