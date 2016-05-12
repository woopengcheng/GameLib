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

#ifndef BEHAVIAC_ENGINESERVICES_UITYPE_H
#define BEHAVIAC_ENGINESERVICES_UITYPE_H

#include "behaviac/base/serialization/textnode.h"
#include "behaviac/base/object/uitypeinterface.h"

enum UiFlags
{
    Ui_None = 0x0000,
    Ui_Hidden = 0x0001,
    Ui_Disable = 0x0002
};

struct UiBasicType : public UiGenericType
{
public:
    BEHAVIAC_DECLARE_MEMORY_OPERATORS(UiBasicType);

private:
    struct IMinMax
    {
        BEHAVIAC_DECLARE_MEMORY_OPERATORS(IMinMax);
        virtual void SaveToXml(behaviac::XmlNodeRef& xmlNode) const = 0;
        virtual ~IMinMax()
        {
        }
    };

    template<class T>
    class CAutoTypeMinMax : public IMinMax
    {
    public:
        BEHAVIAC_DECLARE_MEMORY_OPERATORS(CAutoTypeMinMax);

    public:
        CAutoTypeMinMax(const T& min, const T& max)
        {
            minimum = min;
            maximum = max;
        }

        virtual ~CAutoTypeMinMax()
        {
        }

        virtual void SaveToXml(behaviac::XmlNodeRef& xmlNode) const
        {
            behaviac::CTextNode textNode(xmlNode);
            GenericTypeHandler<T>::Save(&textNode, minimum, behaviac::CSerializationID(0x99365B30, "Min"));
            GenericTypeHandler<T>::Save(&textNode, maximum, behaviac::CSerializationID(0xA53B6469, "Max"));
        }
    private:
        T minimum;
        T maximum;
    };

    uint32_t m_UiFlags;
    IMinMax* m_minMax;
    const char* m_description;

public:

    UiBasicType() : m_UiFlags(Ui_None), m_minMax(NULL), m_description(NULL)
    {
    }

    UiBasicType(uint32_t UiFlags, const char* description = NULL) :
        m_UiFlags(UiFlags), m_minMax(NULL), m_description(description)
    {
    }

    template<class PropertyType>
    UiBasicType(uint32_t UiFlags, const char* description, const PropertyType& min, const PropertyType& max) :
        m_UiFlags(Ui_None), m_description(description)
    {
        typedef CAutoTypeMinMax<PropertyType> PropertyTypeAuto;
        m_minMax = BEHAVIAC_NEW PropertyTypeAuto(min, max);
    }

    virtual ~UiBasicType()
    {
        BEHAVIAC_DELETE(m_minMax);
    }

    virtual void SaveDescription(behaviac::XmlNodeRef& xmlNode)
    {
        if (m_UiFlags & Ui_Hidden)
        {
            behaviac::CTextNode textNode(xmlNode);
            GenericTypeHandler<bool>::Save(&textNode, true, 0, behaviac::CSerializationID(0x8FF1EC8B, "Hidden"));
        }

        if (m_UiFlags & Ui_Disable)
        {
            behaviac::CTextNode textNode(xmlNode);
            GenericTypeHandler<bool>::Save(&textNode, true, 0, behaviac::CSerializationID(0x80F874CD, "Disable"));
        }

        if (m_description)
        {
            behaviac::CTextNode textNode(xmlNode);
            GenericTypeHandler<const char*>::Save(&textNode, m_description, 0, behaviac::CSerializationID(0xEB78CFF1, "Description"));
        }

        if (m_minMax)
        {
            m_minMax->SaveToXml(xmlNode);
        }
    }
};

#define UiDescriptorAllocate_UiBasicType           BEHAVIAC_NEW UiBasicType

static const uint32_t DefaultUiFlags = Ui_None;

#endif // #ifndef BEHAVIAC_ENGINESERVICES_UITYPE_H
