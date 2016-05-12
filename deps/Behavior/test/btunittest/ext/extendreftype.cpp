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

#include "behaviac/behaviac.h"

#include "extendstruct.h"
#include "extendreftype.h"

BEGIN_PROPERTIES_DESCRIPTION(myFloat2)
{
	CLASS_DISPLAYNAME(L"")
	CLASS_DESC(L"")

	REGISTER_PROPERTY(x);
	REGISTER_PROPERTY(y);
}
END_PROPERTIES_DESCRIPTION()

BEGIN_PROPERTIES_DESCRIPTION(myNode)
{
	CLASS_DISPLAYNAME(L"");
	CLASS_DESC(L"");

	REGISTER_PROPERTY(name);
}
END_PROPERTIES_DESCRIPTION()
