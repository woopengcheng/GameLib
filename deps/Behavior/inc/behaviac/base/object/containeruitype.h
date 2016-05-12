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

#ifndef BEHAVIAC_ENGINESERVICES_CONTAINERUITYPE_H
#define BEHAVIAC_ENGINESERVICES_CONTAINERUITYPE_H

#include "behaviac/base/object/uitype.h"

struct UiContainerType : public UiBasicType
{
public:
    BEHAVIAC_DECLARE_MEMORY_OPERATORS(UiContainerType);

private:
    bool m_resizable;
    const char* m_elementName;

public:

    UiContainerType() : m_resizable(false), m_elementName(NULL) {}
    UiContainerType(bool resizable, const char* description = NULL) : UiBasicType(Ui_None, description), m_resizable(resizable), m_elementName(NULL) {}

    virtual void SetMemberName(const char* memberName)
    {
        m_elementName = memberName;
    }
    virtual void SaveDescription(behaviac::XmlNodeRef& xmlNode)
    {
        UiBasicType::SaveDescription(xmlNode);

        if (m_elementName)
        {
            xmlNode->setAttr("Container", m_elementName);
        }

        if (m_resizable)
        {
            xmlNode->setAttr("Resizable", true);
        }
    }
};

#define UiDescriptorAllocate_UiContainerType BEHAVIAC_NEW UiContainerType

#endif // #ifndef BEHAVIAC_ENGINESERVICES_CONTAINERUITYPE_H
