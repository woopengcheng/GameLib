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

#ifndef BEHAVIAC_ASYNCVALUE_H
#define BEHAVIAC_ASYNCVALUE_H

#include "behaviac/base/core/sharedptr.h"
#include "behaviac/base/meta/types.h"
#include "behaviac/base/functions.h"

namespace behaviac
{
    class IAsyncValue
    {
    public:
        IAsyncValue()
        {
        }
        virtual ~IAsyncValue()
        {
        }
        virtual IAsyncValue* clone() = 0;
        virtual bool IsVoid() const = 0;
        virtual void GetTypeName(behaviac::string& typeName) = 0;
        virtual const char* GetString() const = 0;
		virtual double GetDouble() const
		{
			BEHAVIAC_ASSERT(false);
			return 0.0;
		}
		//virtual double GetInteger() const = 0;
    };

    /**
     * Class that holds a value and a valid flag, allowing to check whether
     * the value has been explicitly set or not
     */
    template<typename T, typename RC = reference_counter> class AsyncValue : public IAsyncValue
    {
        typedef VALUE_TYPE(T) TTYPE;
    protected:
        struct Value
        {
        public:
            TTYPE value;
            bool set;
        public:
            Value(const TTYPE& v, bool s) : value(v), set(s) {}
        private:
            Value() {}

            Value& operator=(const Value& v)
            {
                value = v.value;
                set = v.set;
            }
        };
        typedef shared_ptr<Value, RC> SharedT;
    protected:
        SharedT mValue;
    public:
        BEHAVIAC_DECLARE_MEMORY_OPERATORS(AsyncValue<T>);

		inline AsyncValue() : mValue(BEHAVIAC_NEW Value(TTYPE(), false))
        {
        }
        inline AsyncValue(const TTYPE& value, bool set = false) :
            mValue(BEHAVIAC_NEW Value(value, set))
        {
        }
        inline AsyncValue(const AsyncValue& value) : mValue(value)
        {
        }
        virtual ~AsyncValue()
        {
            //BEHAVIAC_DELETE(mValue);
        }
        inline bool isValid() const
        {
            return mValue->set;
        }
        inline void unset()
        {
            mValue->set = false;
        }
        inline void set(const TTYPE& value, bool set)
        {
            mValue->value = value;
            mValue->set = set;
        }
        inline TTYPE& get()
        {
            return mValue->value;
        }
		virtual double GetDouble() const
		{
			BEHAVIAC_ASSERT(GetClassTypeNumberId<double>() == GetClassTypeNumberId<TTYPE>() || 
				GetClassTypeNumberId<float>() == GetClassTypeNumberId<TTYPE>());

			double r = ConvertToDouble(mValue->value);
			return r;
		}
        inline AsyncValue& operator=(const AsyncValue& v)
        {
            mValue = v;
            return *this;
        }
        //        template <typename T>
        inline AsyncValue& operator=(const TTYPE& v)
        {
            mValue->value = v;
            mValue->set = true;
            return *this;
        }

        virtual IAsyncValue* clone()
        {
            return BEHAVIAC_NEW AsyncValue(*this);
        }

        virtual bool IsVoid() const
        {
            return false;
        }

        virtual void GetTypeName(behaviac::string& typeName)
        {
            typeName = ::GetTypeDescString<T>();
        }

        virtual const char* GetString() const
        {
            BEHAVIAC_ASSERT(false);
            return 0;
        }

    protected:
        inline operator SharedT() const
        {
            return mValue;
        }
    };

    template<> class AsyncValue<void> : public IAsyncValue
    {
    public:
        BEHAVIAC_DECLARE_MEMORY_OPERATORS(AsyncValue<void>);

        inline AsyncValue()
        {
        }
        inline AsyncValue(const AsyncValue&)
        {
        }
        inline bool isValid() const
        {
            return false;
        }
        inline void unset() {  }
        inline AsyncValue& operator=(const AsyncValue&)
        {
            return *this;
        }

        virtual IAsyncValue* clone()
        {
            return BEHAVIAC_NEW AsyncValue(*this);
        }

        virtual bool IsVoid() const
        {
            return true;
        }

        virtual void GetTypeName(behaviac::string& typeName)
        {
            typeName = "void";
        }
        virtual const char* GetString() const
        {
            BEHAVIAC_ASSERT(false);
            return 0;
        }
    };

    template<> class AsyncValue<const char*> : public IAsyncValue
    {
    protected:
        struct Value
        {
        public:
            const char* value;
            bool set;
        public:
            Value(const char* v, bool s) : value(v), set(s) {}
        private:
            Value() {}

            Value& operator=(const Value& v)
            {
                value = v.value;
                set = v.set;

                return *this;
            }
        };
        typedef shared_ptr<Value, reference_counter> SharedT;
    protected:
        SharedT mValue;
    public:
        BEHAVIAC_DECLARE_MEMORY_OPERATORS(AsyncValue<const char*>);

        inline AsyncValue() : mValue(BEHAVIAC_NEW Value(0, false))
        {
        }
        inline AsyncValue(const char* value, bool set = false) :
            mValue(BEHAVIAC_NEW Value(value, set))
        {
        }
        inline AsyncValue(const AsyncValue& value) : mValue(value)
        {
        }
        virtual ~AsyncValue()
        {
            //BEHAVIAC_DELETE(mValue);
        }
        inline bool isValid() const
        {
            return mValue->set;
        }
        inline void unset()
        {
            mValue->set = false;
        }
        inline void set(const char* value, bool set)
        {
            mValue->value = value;
            mValue->set = set;
        }
        inline const char*& get()
        {
            return mValue->value;
        }
        inline AsyncValue& operator=(const AsyncValue& v)
        {
            mValue = v;
            return *this;
        }
        inline AsyncValue& operator=(const char* v)
        {
            mValue->value = v;
            mValue->set = true;
            return *this;
        }

        virtual IAsyncValue* clone()
        {
            return BEHAVIAC_NEW AsyncValue(*this);
        }

        virtual bool IsVoid() const
        {
            return false;
        }

        virtual void GetTypeName(behaviac::string& typeName)
        {
            typeName = ::GetTypeDescString<const char*>();
        }

        virtual const char* GetString() const
        {
            return this->mValue->value;
        }

    protected:
        inline operator SharedT() const
        {
            return mValue;
        }
    };

    template<> class AsyncValue<char*> : public IAsyncValue
    {
    protected:
        struct Value
        {
        public:
            char* value;
            bool set;
        public:
            Value(char* v, bool s) : value(v), set(s) {}
        private:
            Value() {}

            Value& operator=(const Value& v)
            {
                value = v.value;
                set = v.set;
                return *this;
            }
        };
        typedef shared_ptr<Value, reference_counter> SharedT;
    protected:
        SharedT mValue;
    public:
        BEHAVIAC_DECLARE_MEMORY_OPERATORS(AsyncValue<char*>);

        inline AsyncValue() : mValue(BEHAVIAC_NEW Value(0, false))
        {
        }
        inline AsyncValue(char* value, bool set = false) :
            mValue(BEHAVIAC_NEW Value(value, set))
        {
        }
        inline AsyncValue(const AsyncValue& value) : mValue(value)
        {
        }
        virtual ~AsyncValue()
        {
            //BEHAVIAC_DELETE(mValue);
        }
        inline bool isValid() const
        {
            return mValue->set;
        }
        inline void unset()
        {
            mValue->set = false;
        }
        inline void set(char* value, bool set)
        {
            mValue->value = value;
            mValue->set = set;
        }
        inline char*& get()
        {
            return mValue->value;
        }
        inline AsyncValue& operator=(const AsyncValue& v)
        {
            mValue = v;
            return *this;
        }
        inline AsyncValue& operator=(char* v)
        {
            mValue->value = v;
            mValue->set = true;
            return *this;
        }

        virtual IAsyncValue* clone()
        {
            return BEHAVIAC_NEW AsyncValue(*this);
        }

        virtual bool IsVoid() const
        {
            return false;
        }

        virtual void GetTypeName(behaviac::string& typeName)
        {
            typeName = ::GetTypeDescString<char*>();
        }

        virtual const char* GetString() const
        {
            return this->mValue->value;
        }

    protected:
        inline operator SharedT() const
        {
            return mValue;
        }
    };

    template<> class AsyncValue<behaviac::string> : public IAsyncValue
    {
    protected:
        struct Value
        {
        public:
            behaviac::string value;
            bool set;
        public:
            Value(const behaviac::string& v, bool s) : value(v), set(s) {}
        private:
            Value() {}

            Value& operator=(const Value& v)
            {
                value = v.value;
                set = v.set;

                return *this;
            }
        };
        typedef shared_ptr<Value, reference_counter> SharedT;
    protected:
        SharedT mValue;
    public:
        BEHAVIAC_DECLARE_MEMORY_OPERATORS(AsyncValue<behaviac::string>);

        inline AsyncValue() : mValue(BEHAVIAC_NEW Value(behaviac::string(), false))
        {
        }
        inline AsyncValue(const behaviac::string& value, bool set = false) :
            mValue(BEHAVIAC_NEW Value(value, set))
        {
        }
        inline AsyncValue(const AsyncValue& value) : mValue(value)
        {
        }
        virtual ~AsyncValue()
        {
            //BEHAVIAC_DELETE(mValue);
        }
        inline bool isValid() const
        {
            return mValue->set;
        }
        inline void unset()
        {
            mValue->set = false;
        }
        inline void set(const behaviac::string& value, bool set)
        {
            mValue->value = value;
            mValue->set = set;
        }
        inline const behaviac::string& get()
        {
            return mValue->value;
        }
        inline AsyncValue& operator=(const AsyncValue& v)
        {
            mValue = v;
            return *this;
        }
        inline AsyncValue& operator=(const behaviac::string& v)
        {
            mValue->value = v;
            mValue->set = true;
            return *this;
        }

        virtual IAsyncValue* clone()
        {
            return BEHAVIAC_NEW AsyncValue(*this);
        }

        virtual bool IsVoid() const
        {
            return false;
        }

        virtual void GetTypeName(behaviac::string& typeName)
        {
            typeName = ::GetTypeDescString<char*>();
        }

        virtual const char* GetString() const
        {
            return this->mValue->value.c_str();
        }

    protected:
        inline operator SharedT() const
        {
            return mValue;
        }
    };

    // Common value types
    typedef AsyncValue<void>    AsyncVoid;
    typedef AsyncValue<bool>    AsyncBool;
    typedef AsyncValue<int>     AsyncInt;
    typedef AsyncValue<float>   AsyncFloat;
    typedef AsyncValue<double>  AsyncDouble;
}

#endif // BEHAVIAC_ASYNCVALUE_H
