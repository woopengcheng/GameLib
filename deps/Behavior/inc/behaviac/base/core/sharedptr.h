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

#ifndef BEHAVIAC_SHAREPTR_H
#define BEHAVIAC_SHAREPTR_H

namespace behaviac
{
    /// Simple reference_counter object, does not delete itself when count reaches 0.
    class reference_counter
    {
    public:
        reference_counter() : _cnt(1)
        {
        }

        void duplicate()
        {
            ++_cnt;
        }

        int release()
        {
            return --_cnt;
        }

        int referenceCount() const
        {
            return _cnt;
        }

    private:
        volatile int _cnt;
    };

    /// The default release policy for shared_ptr, which simply uses the delete operator to delete an object.
    template <class C>
    class release_policy
    {
    public:
        static void release(C* pObj)
        {
            BEHAVIAC_DELETE(pObj);
        }
    };

    /// The release policy for shared_ptr holding arrays.
    template <class C>
    class release_array_policy
    {
    public:
        static void release(C* pObj)
        {
            BEHAVIAC_DELETE_ARRAY pObj;
        }
    };

    /// shared_ptr is a "smart" pointer for classes implementing reference counting based garbage collection.
    template <class C, class RC = reference_counter, class RP = release_policy<C> >
    class shared_ptr
    {
    public:
        shared_ptr() : _pCounter(BEHAVIAC_NEW(RC)), _ptr(0)
        {
        }

        shared_ptr(C* ptr) : _pCounter(BEHAVIAC_NEW(RC)), _ptr(ptr)
        {
        }

        template <class Other, class OtherRP>
        shared_ptr(const shared_ptr<Other, RC, OtherRP>& ptr) : _pCounter(ptr._pCounter), _ptr(const_cast<Other*>(ptr.get()))
        {
            _pCounter->duplicate();
        }

        shared_ptr(const shared_ptr& ptr) : _pCounter(ptr._pCounter), _ptr(ptr._ptr)
        {
            _pCounter->duplicate();
        }

        ~shared_ptr()
        {
            release();
        }

        shared_ptr& assign(C* ptr)
        {
            if (get() != ptr)
            {
                RC* pTmp = BEHAVIAC_NEW(RC);
                release();
                _pCounter = pTmp;
                _ptr = ptr;
            }

            return *this;
        }

        shared_ptr& assign(const shared_ptr& ptr)
        {
            if (&ptr != this)
            {
                shared_ptr tmp(ptr);
                swap(tmp);
            }

            return *this;
        }

        template <class Other, class OtherRP>
        shared_ptr& assign(const shared_ptr<Other, RC, OtherRP>& ptr)
        {
            if (ptr.get() != _ptr)
            {
                shared_ptr tmp(ptr);
                swap(tmp);
            }

            return *this;
        }

        shared_ptr& operator = (C* ptr)
        {
            return assign(ptr);
        }

        shared_ptr& operator = (const shared_ptr& ptr)
        {
            return assign(ptr);
        }

        template <class Other, class OtherRP>
        shared_ptr& operator = (const shared_ptr<Other, RC, OtherRP>& ptr)
        {
            return assign<Other>(ptr);
        }

        void swap(shared_ptr& ptr)
        {
            std::swap(_ptr, ptr._ptr);
            std::swap(_pCounter, ptr._pCounter);
        }

        //template <class Other>
        //shared_ptr<Other, RC, RP> cast() const
        ///// Casts the shared_ptr via a dynamic cast to the given type.
        ///// Returns an shared_ptr containing NULL if the cast fails.
        ///// Example: (assume class Sub: public Super)
        /////    shared_ptr<Super> super(new Sub());
        /////    shared_ptr<Sub> sub = super.cast<Sub>();
        /////    poco_assert (sub.get());
        //{
        //    Other* pOther = dynamic_cast<Other*>(_ptr);

        //    if (pOther)
        //    {
        //        return shared_ptr<Other, RC, RP>(_pCounter, pOther);
        //    }

        //    return shared_ptr<Other, RC, RP>();
        //}

        //template <class Other>
        //shared_ptr<Other, RC, RP> unsafeCast() const
        ///// Casts the shared_ptr via a static cast to the given type.
        ///// Example: (assume class Sub: public Super)
        /////    shared_ptr<Super> super(new Sub());
        /////    shared_ptr<Sub> sub = super.unsafeCast<Sub>();
        /////    poco_assert (sub.get());
        //{
        //    Other* pOther = static_cast<Other*>(_ptr);
        //    return shared_ptr<Other, RC, RP>(_pCounter, pOther);
        //}

        C* operator -> ()
        {
            return deref();
        }

        const C* operator -> () const
        {
            return deref();
        }

        C& operator * ()
        {
            return *deref();
        }

        const C& operator * () const
        {
            return *deref();
        }

        C* get()
        {
            return _ptr;
        }

        const C* get() const
        {
            return _ptr;
        }

        operator C* ()
        {
            return _ptr;
        }

        operator const C* () const
        {
            return _ptr;
        }

        bool operator !() const
        {
            return _ptr == 0;
        }

        bool isNull() const
        {
            return _ptr == 0;
        }

        bool operator == (const shared_ptr& ptr) const
        {
            return get() == ptr.get();
        }

        bool operator == (const C* ptr) const
        {
            return get() == ptr;
        }

        bool operator == (C* ptr) const
        {
            return get() == ptr;
        }

        bool operator != (const shared_ptr& ptr) const
        {
            return get() != ptr.get();
        }

        bool operator != (const C* ptr) const
        {
            return get() != ptr;
        }

        bool operator != (C* ptr) const
        {
            return get() != ptr;
        }

        bool operator < (const shared_ptr& ptr) const
        {
            return get() < ptr.get();
        }

        bool operator < (const C* ptr) const
        {
            return get() < ptr;
        }

        bool operator < (C* ptr) const
        {
            return get() < ptr;
        }

        bool operator <= (const shared_ptr& ptr) const
        {
            return get() <= ptr.get();
        }

        bool operator <= (const C* ptr) const
        {
            return get() <= ptr;
        }

        bool operator <= (C* ptr) const
        {
            return get() <= ptr;
        }

        bool operator > (const shared_ptr& ptr) const
        {
            return get() > ptr.get();
        }

        bool operator > (const C* ptr) const
        {
            return get() > ptr;
        }

        bool operator > (C* ptr) const
        {
            return get() > ptr;
        }

        bool operator >= (const shared_ptr& ptr) const
        {
            return get() >= ptr.get();
        }

        bool operator >= (const C* ptr) const
        {
            return get() >= ptr;
        }

        bool operator >= (C* ptr) const
        {
            return get() >= ptr;
        }

        int referenceCount() const
        {
            return _pCounter->referenceCount();
        }

    private:
        C* deref() const
        {
            if (!_ptr)
            {
                BEHAVIAC_ASSERT(0);
            }

            return _ptr;
        }

        void release()
        {
            BEHAVIAC_ASSERT(_pCounter);
            int i = _pCounter->release();

            if (i == 0)
            {
                RP::release(_ptr);
                _ptr = 0;
                BEHAVIAC_DELETE(_pCounter);
                _pCounter = 0;
            }
        }

        shared_ptr(RC* pCounter, C* ptr) : _pCounter(pCounter), _ptr(ptr)
        {
            poco_assert_dbg(_pCounter);
            _pCounter->duplicate();
        }

    private:
        RC* _pCounter;
        C*  _ptr;

        template <class OtherC, class OtherRC, class OtherRP> friend class shared_ptr;
    };

    template <class C, class RC, class RP>
    inline void swap(shared_ptr<C, RC, RP>& p1, shared_ptr<C, RC, RP>& p2)
    {
        p1.swap(p2);
    }
}//namespace behaviac

#endif//BEHAVIAC_SHAREPTR_H
