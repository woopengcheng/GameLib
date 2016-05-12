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

#include "behaviac/base/dynamictype.h"

char* GenerateString1(const char* aT1, const char* aT2)
{
    size_t size = strlen(aT1) + strlen(aT2);
    char* str = (char*)BEHAVIAC_MALLOC_WITHTAG(size + 1, "CDynamicType");
    string_cpy(str, aT1);
    strcat(str, aT2);
    return str;
}
char* GenerateString2(const char* aT1, const char* aT2, const char* aT3)
{
	size_t size = strlen(aT1) + strlen(aT2) + strlen(aT3);
    char* str = (char*)BEHAVIAC_MALLOC_WITHTAG(size + 1, "CDynamicType");
    string_cpy(str, aT1);
    strcat(str, aT2);
    strcat(str, aT3);
    return str;
}
char* GenerateString3(const char* aT1, const char* aT2, const char* aT3, const char* aT4)
{
	size_t size = strlen(aT1) + strlen(aT2) + strlen(aT3) + strlen(aT4);
    char* str = (char*)BEHAVIAC_MALLOC_WITHTAG(size + 1, "CDynamicType");
    string_cpy(str, aT1);
    strcat(str, aT2);
    strcat(str, aT3);
    strcat(str, aT4);
    return str;
}
char* GenerateString4(const char* aT1, const char* aT2, const char* aT3, const char* aT4, const char* aT5)
{
	size_t size = strlen(aT1) + strlen(aT2) + strlen(aT3) + strlen(aT4) + strlen(aT5);
    char* str = (char*)BEHAVIAC_MALLOC_WITHTAG(size + 1, "CDynamicType");
    string_cpy(str, aT1);
    strcat(str, aT2);
    strcat(str, aT3);
    strcat(str, aT4);
    strcat(str, aT5);
    return str;
}
char* GenerateString5(const char* aT1, const char* aT2, const char* aT3, const char* aT4, const char* aT5, const char* aT6)
{
	size_t size = strlen(aT1) + strlen(aT2) + strlen(aT3) + strlen(aT4) + strlen(aT5) + strlen(aT6);
    char* str = (char*)BEHAVIAC_MALLOC_WITHTAG(size + 1, "CDynamicType");
    string_cpy(str, aT1);
    strcat(str, aT2);
    strcat(str, aT3);
    strcat(str, aT4);
    strcat(str, aT5);
    strcat(str, aT6);
    return str;
}

CDynamicTypeAutoFreeChar::~CDynamicTypeAutoFreeChar()
{
    BEHAVIAC_FREE(m_str);
}

template<>
void CDynamicType::SHierarchyInfoDecl<1>::InitClassHierarchyInfo(char const* typeName, const CDynamicType::SHierarchyInfoBase* parent_)
{
    InternalInitClassHierarchyInfo(typeName, parent_);
}

template<>
void CDynamicType::SHierarchyInfoDecl<2>::InitClassHierarchyInfo(char const* typeName, const CDynamicType::SHierarchyInfoBase* parent_)
{
    InternalInitClassHierarchyInfo(typeName, parent_);
}
template<>
void CDynamicType::SHierarchyInfoDecl<3>::InitClassHierarchyInfo(char const* typeName, const CDynamicType::SHierarchyInfoBase* parent_)
{
    InternalInitClassHierarchyInfo(typeName, parent_);
}
template<>
void CDynamicType::SHierarchyInfoDecl<4>::InitClassHierarchyInfo(char const* typeName, const CDynamicType::SHierarchyInfoBase* parent_)
{
    InternalInitClassHierarchyInfo(typeName, parent_);
}
template<>
void CDynamicType::SHierarchyInfoDecl<5>::InitClassHierarchyInfo(char const* typeName, const CDynamicType::SHierarchyInfoBase* parent_)
{
    InternalInitClassHierarchyInfo(typeName, parent_);
}
template<>
void CDynamicType::SHierarchyInfoDecl<6>::InitClassHierarchyInfo(char const* typeName, const CDynamicType::SHierarchyInfoBase* parent_)
{
    InternalInitClassHierarchyInfo(typeName, parent_);
}
template<>
void CDynamicType::SHierarchyInfoDecl<7>::InitClassHierarchyInfo(char const* typeName, const CDynamicType::SHierarchyInfoBase* parent_)
{
    InternalInitClassHierarchyInfo(typeName, parent_);
}
template<>
void CDynamicType::SHierarchyInfoDecl<8>::InitClassHierarchyInfo(char const* typeName, const CDynamicType::SHierarchyInfoBase* parent_)
{
    InternalInitClassHierarchyInfo(typeName, parent_);
}
template<>
void CDynamicType::SHierarchyInfoDecl<9>::InitClassHierarchyInfo(char const* typeName, const CDynamicType::SHierarchyInfoBase* parent_)
{
    InternalInitClassHierarchyInfo(typeName, parent_);
}
