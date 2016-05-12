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

#include "ParTestAgentBase.h"

//< static member variable defines
eColor ParTestAgentBase::STV_ECOLOR_0 = WHITE;
bool ParTestAgentBase::STV_BOOL_0 = false;
char ParTestAgentBase::STV_CHAR_0 = L'\0';

behaviac::vector<eColor> ParTestAgentBase::STV_LIST_ECOLOR_0;
behaviac::vector<bool> ParTestAgentBase::STV_LIST_BOOL_0;
behaviac::vector<char> ParTestAgentBase::STV_LIST_CHAR_0;
behaviac::vector<signed char> ParTestAgentBase::STV_LIST_SBYTE_0;

ParTestAgentBase::ParTestAgentBase()
{
    TV_ECOLOR_0 = WHITE;
    TV_BOOL_0 = false;
    TV_CHAR_0 = L'\0';
    TV_BYTE_0 = 0;
    TV_SBYTE_0 = 0;
}

ParTestAgentBase::~ParTestAgentBase()
{
}

void ParTestAgentBase::resetProperties()
{
    TV_ECOLOR_0 = WHITE;
    STV_ECOLOR_0 = WHITE;
    TV_BOOL_0 = false;
    STV_BOOL_0 = false;
    TV_CHAR_0 = L'\0';
    STV_CHAR_0 = L'\0';
    TV_BYTE_0 = 0;
    TV_SBYTE_0 = 0;

    TV_LIST_ECOLOR_0.clear();
    STV_LIST_ECOLOR_0.clear();
    TV_LIST_BOOL_0.clear();
    STV_LIST_BOOL_0.clear();
    TV_LIST_CHAR_0.clear();
    STV_LIST_CHAR_0.clear();
    TV_LIST_BYTE_0.clear();
    TV_LIST_SBYTE_0.clear();
    STV_LIST_SBYTE_0.clear();
}

BEGIN_PROPERTIES_DESCRIPTION(ParTestAgentBase)
{
    CLASS_DISPLAY_INFO(L"ParTestAgentBase", L"测试behaviac::Agent的最底层基类");

    REGISTER_PROPERTY(TV_ECOLOR_0).DISPLAY_INFO(L"0 # TV_ECOLOR_0", L"A");
    REGISTER_PROPERTY(STV_ECOLOR_0).DISPLAYNAME(L"0 # STV_ECOLOR_0").DESC(L"A");
    REGISTER_PROPERTY(TV_BOOL_0).DISPLAYNAME(L"0 # TV_BOOL_0").DESC(L"A");
    REGISTER_PROPERTY(STV_BOOL_0).DISPLAYNAME(L"0 # STV_BOOL_0").DESC(L"A");;
    REGISTER_PROPERTY(TV_CHAR_0).DISPLAYNAME(L"0 # TV_CHAR_0").DESC(L"A");
    REGISTER_PROPERTY(STV_CHAR_0).DISPLAYNAME(L"0 # STV_CHAR_0").DESC(L"A");
    REGISTER_PROPERTY(TV_BYTE_0).DISPLAYNAME(L"0 # TV_BYTE_0").DESC(L"A");
    REGISTER_PROPERTY(TV_SBYTE_0).DISPLAYNAME(L"0 # TV_SBYTE_0").DESC(L"A");
    REGISTER_PROPERTY(TV_LIST_ECOLOR_0).DISPLAYNAME(L"0 # TV_LIST_ECOLOR_0").DESC(L"A");
    REGISTER_PROPERTY(STV_LIST_ECOLOR_0).DISPLAYNAME(L"0 # STV_LIST_ECOLOR_0").DESC(L"A");
    REGISTER_PROPERTY(TV_LIST_BOOL_0).DISPLAYNAME(L"0 # TV_LIST_BOOL_0").DESC(L"A");
    REGISTER_PROPERTY(STV_LIST_BOOL_0).DISPLAYNAME(L"0 # STV_LIST_BOOL_0").DESC(L"A");
    REGISTER_PROPERTY(TV_LIST_CHAR_0).DISPLAYNAME(L"0 # TV_LIST_CHAR_0").DESC(L"A");
    REGISTER_PROPERTY(STV_LIST_CHAR_0).DISPLAYNAME(L"0 # STV_LIST_CHAR_0").DESC(L"A");
    REGISTER_PROPERTY(TV_LIST_BYTE_0).DISPLAYNAME(L"0 # TV_LIST_BYTE_0").DESC(L"A");
    REGISTER_PROPERTY(TV_LIST_SBYTE_0).DISPLAYNAME(L"0 # TV_LIST_SBYTE_0").DESC(L"A");
    REGISTER_PROPERTY(STV_LIST_SBYTE_0).DISPLAYNAME(L"0 # STV_LIST_SBYTE_0").DESC(L"A");

    REGISTER_METHOD(Func_eColorRef).DISPLAYNAME(L"0 # UPR_eColor").DESC(L"A");
    REGISTER_METHOD(Func_BooleanRef).DISPLAYNAME(L"0 # UPR_Boolean").DESC(L"A");
    REGISTER_METHOD(Func_CharRef).DISPLAYNAME(L"0 # UPR_Char").DESC(L"A");
    REGISTER_METHOD(Func_ByteRef).DISPLAYNAME(L"0 # UPR_Byte").DESC(L"A");
    REGISTER_METHOD(Func_SByteRef).DISPLAYNAME(L"0 # UPR_SByte").DESC(L"A");
    REGISTER_METHOD(Func_eColorListRef).DISPLAYNAME(L"0 # UPR_eColorList").DESC(L"A");
    REGISTER_METHOD(Func_BooleanListRef).DISPLAYNAME(L"0 # UPR_BooleanList").DESC(L"A");
    REGISTER_METHOD(Func_CharListRef).DISPLAYNAME(L"0 # UPR_CharList").DESC(L"A");
    REGISTER_METHOD(Func_ByteListRef).DISPLAYNAME(L"0 # UPR_ByteList").DESC(L"A");
    REGISTER_METHOD(Func_SByteListRef).DISPLAYNAME(L"0 # UPR_SByteList").DESC(L"A");

    REGISTER_METHOD(Func_eColorIR).DISPLAYNAME(L"0 # PIR_eColor").DESC(L"A");
    REGISTER_METHOD(Func_BooleanIR).DISPLAYNAME(L"0 # PIR_Boolean").DESC(L"A");
    REGISTER_METHOD(Func_CharIR).DISPLAYNAME(L"0 # PIR_Char").DESC(L"A");
    REGISTER_METHOD(Func_ByteIR).DISPLAYNAME(L"0 # PIR_Byte").DESC(L"A");
    REGISTER_METHOD(Func_SByteIR).DISPLAYNAME(L"0 # PIR_SByte").DESC(L"A");
    REGISTER_METHOD(Func_eColorListIR).DISPLAYNAME(L"0 # PIR_eColorList").DESC(L"A");
    REGISTER_METHOD(Func_BooleanListIR).DISPLAYNAME(L"0 # PIR_BooleanList").DESC(L"A");
    REGISTER_METHOD(Func_CharListIR).DISPLAYNAME(L"0 # PIR_CharList").DESC(L"A");
    REGISTER_METHOD(Func_ByteListIR).DISPLAYNAME(L"0 # PIR_ByteList").DESC(L"A");
    REGISTER_METHOD(Func_SByteListIR).DISPLAYNAME(L"0 # PIR_SByteList").DESC(L"A");

    REGISTER_METHOD(Func_CharSMF).DISPLAYNAME(L"0 # SMF_Char").DESC(L"A");
    REGISTER_METHOD(Func_ByteSMF).DISPLAYNAME(L"0 # SMF_Byte").DESC(L"A");
    REGISTER_METHOD(Func_SByteSMF).DISPLAYNAME(L"0 # SMF_SByte").DESC(L"A");
    REGISTER_METHOD(Func_CharListSMF).DISPLAYNAME(L"0 # SMF_CharList").DESC(L"A");
    REGISTER_METHOD(Func_SByteListSMF).DISPLAYNAME(L"0 # SMF_SByteList").DESC(L"A");
}
END_PROPERTIES_DESCRIPTION()
