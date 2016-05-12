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

#ifndef BEHAVIAC_CORE_TEXTFILE_H
#define BEHAVIAC_CORE_TEXTFILE_H

#include "behaviac/base/base.h"
#include "behaviac/base/core/container/string_t.h"
#include "behaviac/base/core/container/vector_t.h"

#include <vector>
#include <string>

namespace behaviac
{
	BEHAVIAC_API char* LoadTextFileAsBuffer(const char* fileName);
	BEHAVIAC_API bool LoadTextFileAsStringArray(const char* fileName, behaviac::vector<behaviac::string>& stringArray);
	BEHAVIAC_API void ConvertTextBufferAsStringArray(const char* buffer, behaviac::vector<behaviac::string>& stringArray);
}//namespace behaviac

#endif // #ifndef BEHAVIAC_CORE_TEXTFILE_H
