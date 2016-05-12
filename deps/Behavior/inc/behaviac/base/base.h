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

#ifndef BEHAVIAC_BASE_H
#define BEHAVIAC_BASE_H

#include "behaviac/base/core/config.h"
#include "behaviac/base/core/compiler.h"
#include "behaviac/base/core/assert_t.h"

#include "behaviac/base/core/memory/memory.h"

#include "behaviac/base/core/string/stringid.h"
#include "behaviac/base/core/logging/log.h"

#include "behaviac/base/core/container/string_t.h"
#include "behaviac/base/core/container/vector_t.h"
#include "behaviac/base/core/container/map_t.h"

#include <functional>
#include <algorithm>

namespace behaviac
{
    BEHAVIAC_API void SetMainThread();
    BEHAVIAC_API bool IsMainThread();

    typedef void(*BreakpointPromptHandler_fn)(const char* breakPointDesc);
    BEHAVIAC_API void SetBreakpointPromptHandler(BreakpointPromptHandler_fn fn);

    BEHAVIAC_API BreakpointPromptHandler_fn GetBreakpointPromptHandler();

	BEHAVIAC_API const char* GetVersionString();
}//namespace behaviac

#define ASSERT_MAIN_THREAD()  BEHAVIAC_ASSERT(behaviac::IsMainThread(), "called in a thread different from the creating thread")

#endif // BEHAVIAC_BASE_H
