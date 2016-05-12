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

#ifndef BEHAVIAC_ENGINESERVICES_FILEUITYPE_H
#define BEHAVIAC_ENGINESERVICES_FILEUITYPE_H

#include "behaviac/base/object/uitype.h"

struct UiFileType : public UiBasicType
{
public:
    BEHAVIAC_DECLARE_MEMORY_OPERATORS(UiFileType);

private:
    const char* m_fileFilter;
    const char* m_defaultFolder;
    const char* m_replaceExtension;
    bool m_removePath;

public:

    UiFileType() : m_fileFilter(NULL), m_defaultFolder(NULL), m_replaceExtension(NULL), m_removePath(false) {}
    UiFileType(const char* fileFilter, const char* defaultFolder = NULL, const char* replaceExtension = NULL, bool removePath = false, const char* description = NULL) : UiBasicType(Ui_None, description), m_fileFilter(fileFilter), m_defaultFolder(defaultFolder), m_replaceExtension(replaceExtension), m_removePath(removePath) {}

    virtual void SaveDescription(behaviac::XmlNodeRef& xmlNode)
    {
        UiBasicType::SaveDescription(xmlNode);

        if (m_fileFilter)
        {
            xmlNode->setAttr("FileFilter", m_fileFilter);
        }

        if (m_defaultFolder)
        {
            xmlNode->setAttr("DefaultFolder", m_defaultFolder);
        }

        if (m_replaceExtension)
        {
            xmlNode->setAttr("ReplaceExtension", m_replaceExtension);
        }

        if (m_removePath)
        {
            xmlNode->setAttr("RemovePath", m_removePath);
        }
    }
};

#define UiDescriptorAllocate_UiFileType BEHAVIAC_NEW UiFileType

#endif // #ifndef BEHAVIAC_ENGINESERVICES_FILEUITYPE_H
