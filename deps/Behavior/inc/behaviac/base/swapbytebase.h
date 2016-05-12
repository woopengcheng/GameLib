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

#ifndef BEHAVIAC_CORE_SWAPBYTEBASE_H
#define BEHAVIAC_CORE_SWAPBYTEBASE_H
#include "behaviac/base/base.h"
#include "behaviac/base/core/container/string_t.h"
#include "behaviac/base/core/container/vector_t.h"
#include "behaviac/base/core/container/list_t.h"
#include "behaviac/base/core/container/map_t.h"
#include "behaviac/base/core/container/set_t.h"

#include "behaviac/base/string/pathid.h"
#include "behaviac/base/core/string/stringid.h"

#include "swapbyte.h"

//////////////////////////////////////////////////////////////////////////
// SwapByteTempl helpers

template< typename SWAPPER >
inline void SwapByteTempl(const char*& s)
{
    BEHAVIAC_UNUSED_VAR(s);
}

template< typename SWAPPER >
inline void SwapByteTempl(behaviac::string& s)
{
    BEHAVIAC_UNUSED_VAR(s);
}

template<typename SWAPPER>
inline void SwapByteTempl(behaviac::CStringID& value)
{
#if STRINGID_USESTRINGCONTENT
    const char* s = value.c_str();
    value.SetContent(s);
#else
    behaviac::CStringID::IDType id = value.GetUniqueID();
    SwapByte(id);

    value.SetUniqueID(id);
#endif//#if STRINGID_USESTRINGCONTENT
}

template<typename SWAPPER>
inline void SwapByteTempl(behaviac::CPathID& value)
{
#if STRINGID_USESTRINGCONTENT
    const char* s = value.c_str();
    value.SetContent(s);
#else
    behaviac::CPathID::IDType id = value.GetUniqueID();
    SwapByte(id);

    value = behaviac::CPathID(id);
#endif//#if STRINGID_USESTRINGCONTENT
}

//////////////////////////////////////////////////////////////////////////
template<typename SWAPPER, typename T>
inline void SwapByteTempl(behaviac::vector<T>& value)
{
    for (unsigned int i = 0; i < value.size(); ++i)
    {
        SwapByteTempl< SWAPPER >(value[i]);
    }
}

template<typename SWAPPER>
inline void SwapByteTempl(behaviac::vector<bool>& value)
{
    BEHAVIAC_UNUSED_VAR(value);
}

#endif // #ifndef BEHAVIAC_CORE_SWAPBYTEBASE_H
