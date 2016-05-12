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

#ifndef BEHAVIAC_FORMATSTRING_H
#define BEHAVIAC_FORMATSTRING_H

#ifndef _CRT_SECURE_NO_WARNINGS
#define _CRT_SECURE_NO_WARNINGS	1
#endif//_CRT_SECURE_NO_WARNINGS

#include "behaviac/base/core/string/repeat.h"
#include <stdio.h>

namespace behaviac
{
    namespace Private
    {
        struct STempResultString
        {
            STempResultString()
            {
                // force last character to be zero right now, since it's hidden
                m_characters[STempResultString::ms_maxStringLength] = 0;
            }

            operator const char* () const
            {
                return m_characters;
            }

            operator char* () const
            {
                return m_characters;
            }

            static const unsigned int ms_maxStringLength = 2048;
            mutable char m_characters[ms_maxStringLength + 1];
        };

#define DECLARE_VALIDATETYPE( type ) \
    inline type ValidateType( type v ) \
    { \
        return v; \
    }

        DECLARE_VALIDATETYPE(bool)
        DECLARE_VALIDATETYPE(char)
        DECLARE_VALIDATETYPE(unsigned char)
        DECLARE_VALIDATETYPE(signed char)
        DECLARE_VALIDATETYPE(unsigned short)
        DECLARE_VALIDATETYPE(signed short)
        DECLARE_VALIDATETYPE(unsigned int)
        DECLARE_VALIDATETYPE(signed int)
        DECLARE_VALIDATETYPE(unsigned long)
        DECLARE_VALIDATETYPE(signed long)
        DECLARE_VALIDATETYPE(unsigned long long)
        DECLARE_VALIDATETYPE(signed long long)
        DECLARE_VALIDATETYPE(float)
        DECLARE_VALIDATETYPE(double)

#undef DECLARE_VALIDATETYPE

        inline void const* ValidateType(void const* v)
        {
            return v;
        }
    }//namespace Private
}//namespace behaviac

inline const char* FormatString()
{
    return "";
}

inline const char* FormatString(const char* format)
{
    return format;
}

#define DECLARE_TYPENAME( num ) typename T##num
#define DECLARE_PARAM( num ) const T##num p##num
#define USE_PARAM( num ) behaviac::Private::ValidateType( p##num )

// Milo: temp fix
#if BEHAVIAC_COMPILER_MSVC
#define DEFINE_FORMATSTRING( num ) \
    template< REPEAT( DECLARE_TYPENAME, num ) > \
    inline const char* FormatString( const char* format, REPEAT( DECLARE_PARAM, num ), const behaviac::Private::STempResultString& tempString = behaviac::Private::STempResultString() ) \
    { \
        string_snprintf( tempString, behaviac::Private::STempResultString::ms_maxStringLength, format, REPEAT( USE_PARAM, num ) ); \
        return tempString; \
    }
#else
#define DEFINE_FORMATSTRING( num ) \
    template< REPEAT( DECLARE_TYPENAME, num ) > \
    inline const char* FormatString( const char* format, REPEAT( DECLARE_PARAM, num ), const behaviac::Private::STempResultString& tempString = behaviac::Private::STempResultString() ) \
    { \
        string_snprintf( tempString, behaviac::Private::STempResultString::ms_maxStringLength, format, REPEAT( USE_PARAM, num ) ); \
        return tempString; \
    }
#endif

DEFINE_FORMATSTRING(1)
DEFINE_FORMATSTRING(2)
DEFINE_FORMATSTRING(3)
DEFINE_FORMATSTRING(4)
DEFINE_FORMATSTRING(5)
DEFINE_FORMATSTRING(6)
DEFINE_FORMATSTRING(7)
DEFINE_FORMATSTRING(8)
DEFINE_FORMATSTRING(9)
DEFINE_FORMATSTRING(10)
DEFINE_FORMATSTRING(11)
DEFINE_FORMATSTRING(12)
DEFINE_FORMATSTRING(13)
DEFINE_FORMATSTRING(14)
DEFINE_FORMATSTRING(15)

#undef DEFINE_FORMATSTRING
#undef USE_PARAM
#undef DECLARE_PARAM
#undef DECLARE_TYPENAME

#endif // BEHAVIAC_FORMATSTRING_H
