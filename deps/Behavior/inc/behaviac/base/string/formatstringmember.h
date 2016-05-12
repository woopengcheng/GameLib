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

#ifndef BEHAVIAC_CORE_FORMATSTRINGMEMBER_H
#define BEHAVIAC_CORE_FORMATSTRINGMEMBER_H

#include "behaviac/base/file/formatstring.h"

template< typename CONTAINER, void (CONTAINER::*fct)(const char*) >
class CFormatStringMember
{
public:
    // never has a generic implementation, always a specialized implementation
    inline CONTAINER& GetContainer();

    inline void operator()(const char* format)
    {
        CONTAINER& cont = GetContainer();
        (cont.*fct)(format);
    }

#define DECLARE_TYPENAME( num ) typename T##num
#define DECLARE_PARAM( num ) const T##num p##num
#define USE_PARAM( num ) p##num
#define DEFINE_FORMATSTRING( num ) \
    template< REPEAT( DECLARE_TYPENAME, num ) > \
    inline void operator()( const char* format, REPEAT( DECLARE_PARAM, num ) ) \
    { \
        CONTAINER& cont = GetContainer(); \
        (cont.*fct)( FormatString( format, REPEAT( USE_PARAM, num ) ) ); \
    }

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
};


#endif // BEHAVIAC_CORE_FORMATSTRINGMEMBER_H
