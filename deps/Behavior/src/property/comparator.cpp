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

#include "behaviac/property/comparator.h"

namespace behaviac
{
    E_VariableComparisonType VariableComparator::ParseComparisonType(const char* comparionOperator)
    {
        if (!strcmp(comparionOperator, "Assignment"))
        {
            return VariableComparisonType_Assignment;

        }
        else if (!strcmp(comparionOperator, "Equal"))
        {
            return VariableComparisonType_Equal;

        }
        else if (!strcmp(comparionOperator, "NotEqual"))
        {
            return VariableComparisonType_NotEqual;

        }
        else if (!strcmp(comparionOperator, "Greater"))
        {
            return VariableComparisonType_Greater;

        }
        else if (!strcmp(comparionOperator, "GreaterEqual"))
        {
            return VariableComparisonType_GreaterEqual;

        }
        else if (!strcmp(comparionOperator, "Less"))
        {
            return VariableComparisonType_Less;

        }
        else if (!strcmp(comparionOperator, "LessEqual"))
        {
            return VariableComparisonType_LessEqual;

        }
        else
        {
            BEHAVIAC_ASSERT(0);
        }

        return VariableComparisonType_Equal;
    }
}
