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

#include "HTNAgentHouse.h"

HTNAgentHouse::HTNAgentHouse()
{
}

HTNAgentHouse::~HTNAgentHouse()
{
}

BEGIN_PROPERTIES_DESCRIPTION(HTNAgentHouse)
{
    //CLASS_DISPLAYNAME(L"HTNAgentHouse DisplayName")
    //CLASS_DESC(L"HTNAgentHouse Desc")
    REGISTER_METHOD(HireBuilder);
    REGISTER_METHOD(PayBuilder);
    REGISTER_METHOD(BuildFoundation);
    REGISTER_METHOD(BuildFrame);
    REGISTER_METHOD(BuildRoof);
    REGISTER_METHOD(BuildWalls);
    REGISTER_METHOD(BuildInterior);
    REGISTER_METHOD(CutLogs);
    REGISTER_METHOD(GetFriend);
    REGISTER_METHOD(BuyLand);
    REGISTER_METHOD(GetLoan);
}
END_PROPERTIES_DESCRIPTION()
