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

#ifndef BEHAVIAC_ENGINESERVICES_CONTAINERMEMBEREX_H
#define BEHAVIAC_ENGINESERVICES_CONTAINERMEMBEREX_H

#include "behaviac/base/object/containermember.h"
#include "behaviac/base/object/containertypehandlerex.h"

/////////////////////////////////////////////////////////
//////////     EXTENDED CONTAINER MEMBER     ////////////
/////////////////////////////////////////////////////////

#define REGISTER_CONTAINER_MEMBER_EX(containerName, elementName, valueName, memberName, propertyFlags, containerProvider, containedTypeHandler, UiDescriptor) \
    { \
        DECLARE_UIWRAPPER(UiDescriptor); \
        CMemberBase* property_ = CContainerMemberFactory< GenericContainerHandler, containerProvider, containedTypeHandler, propertyFlags, true >::Create( \
                                 &objectType::memberName, objectType::GetClassTypeName(), containerName, elementName, valueName, "", localWrapper); \
        CTagObjectDescriptor::PushBackMember(ms_members, property_); \
    }

#define REGISTER_CONTAINER_MEMBER_NOCHILD(containerName, elementName, valueName, memberName, propertyFlags, containerProvider, containedTypeHandler, UiDescriptor) \
    { \
        DECLARE_UIWRAPPER(UiDescriptor); \
        CMemberBase* property_ = CContainerMemberFactory< GenericContainerHandler, containerProvider, containedTypeHandler, propertyFlags, false >::Create( \
                                 &objectType::memberName, objectType::GetClassTypeName(), containerName, elementName, valueName, "", localWrapper); \
        CTagObjectDescriptor::PushBackMember(ms_members, property_); \
    }

#define REGISTER_CONTAINER_MEMBER_BYID(containerName, elementName, valueName, idName, memberName, propertyFlags, createChildNode, containerProvider, containedTypeHandler, UiDescriptor) \
    { \
        DECLARE_UIWRAPPER(UiDescriptor); \
        CMemberBase* property_ = CContainerMemberFactory< ByIDContainerHandler, containerProvider, containedTypeHandler, propertyFlags, createChildNode >::Create( \
                                 &objectType::memberName, objectType::GetClassTypeName(), containerName, elementName, valueName, idName, localWrapper); \
        CTagObjectDescriptor::PushBackMember(ms_members, property_); \
    }

#define REGISTER_CONTAINER_MEMBER_USETAGASID(containerName, valueName, memberName, propertyFlags, createChildNode, containerProvider, containedTypeHandler, UiDescriptor) \
    { \
        DECLARE_UIWRAPPER(UiDescriptor); \
        CMemberBase* property_ = CContainerMemberFactory< UseTagAsIDContainerHandler, containerProvider, containedTypeHandler, propertyFlags, createChildNode>::Create( \
                                 &objectType::memberName, objectType::GetClassTypeName(), containerName, "", valueName, "", localWrapper); \
        CTagObjectDescriptor::PushBackMember(ms_members, property_); \
    }

#define REGISTER_CONTAINER_MEMBER_CUSTOM_CONTAINERHANDLER(containerName, elementName, valueName, memberName, propertyFlags, createChildNode, containerHandler, containerProvider, containedTypeHandler, UiDescriptor) \
    { \
        DECLARE_UIWRAPPER(UiDescriptor); \
        CMemberBase* property_ = CContainerMemberFactory< containerHandler, containerProvider, containedTypeHandler, propertyFlags, createChildNode>::Create( \
                                 &objectType::memberName, objectType::GetClassTypeName(), containerName, elementName, valueName, "", localWrapper); \
        CTagObjectDescriptor::PushBackMember(ms_members, property_); \
    }

#endif // #ifndef BEHAVIAC_ENGINESERVICES_CONTAINERMEMBEREX_H
