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

#ifndef BEHAVIAC_BASE_MAP_H
#define BEHAVIAC_BASE_MAP_H

#include "behaviac/base/core/config.h"
#include "behaviac/base/core/assert_t.h"

#include <map>
#include "behaviac/base/core/memory/stl_allocator.h"

namespace behaviac
{
    template < class _Kty, class _Ty,
             class _Pr = std::less<_Kty>,
             class _Alloc = behaviac::stl_allocator<std::pair<const _Kty, _Ty> > >
    class map : public std::map<_Kty, _Ty, _Pr, _Alloc>
    {
    };
}//namespace behaviac

#endif //#ifndef BEHAVIAC_BASE_MAP_H
