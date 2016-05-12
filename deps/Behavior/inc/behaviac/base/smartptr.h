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

#ifndef BEHAVIAC_SMART_PTR_H
#define BEHAVIAC_SMART_PTR_H

#include "behaviac/base/base.h"
//////////////////////////////////////////////////////////////////
// SMART POINTER
//////////////////////////////////////////////////////////////////
template <class T> class CSmartPtr
{
private:
    T* p;
public:
    BEHAVIAC_DECLARE_MEMORY_OPERATORS(CSmartPtr);
public:
    CSmartPtr() : p(NULL) {}
    CSmartPtr(T* p_)
    {
        p = p_;

        if (p)
        {
            p->AddRef();
        }
    }

    CSmartPtr(const CSmartPtr& p_)
    {
        p = p_.p;

        if (p)
        {
            p->AddRef();
        }
    }
    ~CSmartPtr()
    {
        if (p)
        {
            p->Release();
        }
    }
    operator T* () const
    {
        return p;
    }
    T& operator*() const
    {
        return *p;
    }
    T* operator->(void) const
    {
        return p;
    }
    CSmartPtr&  operator=(T* newp)
    {
        if (newp)
        {
            newp->AddRef();
        }

        if (p)
        {
            p->Release();
        }

        p = newp;
        return *this;
    }
    CSmartPtr&  operator=(const CSmartPtr& newp)
    {
        if (newp.p)
        {
            newp.p->AddRef();
        }

        if (p)
        {
            p->Release();
        }

        p = newp.p;
        return *this;
    }
    operator bool() const
    {
        return p != NULL;
    };
    bool operator !() const
    {
        return p == NULL;
    };
    bool  operator ==(const T* p2) const
    {
        return p == p2;
    };
    bool  operator ==(T* p2) const
    {
        return p == p2;
    };
    bool operator ==(CSmartPtr& p2) const
    {
        return p == p2.p;
    };
    bool operator ==(const CSmartPtr& p2) const
    {
        return p == p2.p;
    };
    bool  operator !=(const T* p2) const
    {
        return p != p2;
    };
    bool  operator !=(T* p2) const
    {
        return p != p2;
    };
    bool  operator !=(const CSmartPtr& p2) const
    {
        return p != p2.p;
    };
    bool  operator <(const CSmartPtr& p2) const
    {
        return p < p2.p;
    };
    bool  operator >(const CSmartPtr& p2) const
    {
        return p > p2.p;
    };
};

template <class T>
inline bool operator ==(const CSmartPtr<T>& p1, int null)
{
    return !(bool)p1;
}
template <class T>
inline bool operator !=(const CSmartPtr<T>& p1, int null)
{
    return (bool)p1;
}
template <class T>
inline bool operator ==(int null, const CSmartPtr<T>& p1)
{
    return !(bool)p1;
}
template <class T>
inline bool operator !=(int null, const CSmartPtr<T>& p1)
{
    return (bool)p1;
}

// reference target for smart pointer
// implements AddRef() and Release() strategy using reference counter of the specified type
class BEHAVIAC_API CRefCounted
{
public:
    BEHAVIAC_DECLARE_MEMORY_OPERATORS(CRefCounted);

public:
    CRefCounted()
        : m_nRefCounter(0)
    {}
    CRefCounted(const CRefCounted& other)
        : m_nRefCounter(0)   // All CRefCounted objects must be constructed with 0 refcount, do NOT copy other's count!
    {
        BEHAVIAC_UNUSED_VAR(other);
    }

    virtual ~CRefCounted()
    {
        BEHAVIAC_ASSERT(!m_nRefCounter);
    }

    CRefCounted& operator=(const CRefCounted& other)
    {
        BEHAVIAC_UNUSED_VAR(other);
        // Do NOT copy m_nRefCounter
        return *this;
    }

    void AddRef()
    {
        ++m_nRefCounter;
    }

    void Release()
    {
        if (--m_nRefCounter <= 0)
        {
            DeleteRefCounted();
        }
    }

    // Warning: use for debugging/statistics purposes only!
    long NumRefs()
    {
        return m_nRefCounter;
    }

protected:

    virtual void DeleteRefCounted()
    {
        BEHAVIAC_DELETE(this);
    }

    long m_nRefCounter;
};

// TYPEDEF_AUTOPTR macro, declares Class_AutoPtr, which is the smart pointer to the given class,
// and Class_AutoArray, which is the array(STL vector) of autopointers
#define TYPEDEF_AUTOPTR(T) typedef CSmartPtr<T> T##_AutoPtr;

#endif //BEHAVIAC_SMART_PTR_H
