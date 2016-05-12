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

#ifndef BEHAVIAC_CORE_XML_BASE64_H
#define BEHAVIAC_CORE_XML_BASE64_H

#include "behaviac/base/base.h"

namespace XmlBase64
{
    void BEHAVIAC_API EncodeBuff(const int8_t* inBuff, int8_t*& outBuff, int32_t nbBytes);
    void BEHAVIAC_API DecodeBuff(const int8_t* inBuff, int8_t*& outBuff);
    int32_t BEHAVIAC_API GetEncodedBuffSize(int32_t nbBytes);
};

#endif//BEHAVIAC_CORE_XML_BASE64_H
