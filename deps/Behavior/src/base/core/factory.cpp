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

#include "behaviac/base/core/config.h"
#include "behaviac/base/core/memory/memory.h"
#include "behaviac/base/core/factory.h"
#include "behaviac/base/core/logging/log.h"

bool FactoryNewUnregisterSub(FactoryContainer* creators, const behaviac::CStringID& typeID)
{
    creators->Lock();
    SFactoryBucket bucket(typeID, NULL);
    CreatorIt itEnd(creators->end());
    CreatorIt itFound(std::find(creators->begin(), itEnd, bucket));

    if (itFound != itEnd)
    {
        SFactoryBucket& item = *itFound;
        BEHAVIAC_FREE(item.m_typeConstructor);
        creators->erase(itFound);
        creators->Unlock();
        return true;
    }

    BEHAVIAC_ASSERT("Cannot find the specified factory entry\n");
    creators->Unlock();
    return false;
}

bool FactoryNewRegisterSub(FactoryContainer* creators, const behaviac::CStringID& typeID, void* typeConstructor)
{
    SFactoryBucket bucket(typeID, typeConstructor);
    creators->Lock();
    CreatorIt itEnd(creators->end());
    CreatorIt itFound(std::find(creators->begin(), itEnd, bucket));
    bool wasThere = (itFound != itEnd);

    //Add it only once
    if (!wasThere)
    {
        creators->push_back(bucket);

    }
    else
    {
#if STRINGID_USESTRINGCONTENT
        BEHAVIAC_ASSERT(0);
        BEHAVIAC_LOG2(BEHAVIAC_LOG_WARNING, "Trying to register an already registered type %d -- %s\n", typeID.GetUniqueID(), typeID.c_str());
#else

        BEHAVIAC_ASSERT(0, "Trying to register an already registered type %d", typeID.GetUniqueID());
#endif // #if STRINGID_USESTRINGCONTENT
    }

    creators->Unlock();
    return !wasThere;
}
