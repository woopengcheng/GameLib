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

#ifndef BEHAVIAC_ENGINESERVICES_DYNAMICTYPEFACTORYUTILS_H
#define BEHAVIAC_ENGINESERVICES_DYNAMICTYPEFACTORYUTILS_H

#include "behaviac/base/dynamictypefactory.h"

namespace DynTypeFactoryUtils
{
    //---------------------------------------------------------------------------------------
    // Name: DynTypeFactoryUtils::CreateObjectOfKind()
    // Desc: Uses the dynamic type factory to create an object that should be a kind
    // of T, and returns it as type T*.
    // If the newly created object is not an object of type T, NULL is returned.
    //---------------------------------------------------------------------------------------
    template<class T> T* CreateObjectOfKind(const CStringID& typeID)
    {
        CDynamicType* dynType = CDynamicTypeFactory::GetInstance()->Create(typeID);

        if (dynType != NULL)
        {
            T* newObj = T::DynamicCast(dynType);
            BEHAVIAC_ASSERT(newObj != NULL, "%s is not a type of %s. Creation failed!", typeID.LogStr().c_str(), T::GetClassTypeId().LogStr().c_str());

            if (newObj == NULL)
            {
                // The object was created but is not of the right type.
                BEHAVIAC_DELETE(dynType);
            }

            return newObj;
        }

        return NULL;
    }
};

#endif//BEHAVIAC_ENGINESERVICES_DYNAMICTYPEFACTORYUTILS_H
