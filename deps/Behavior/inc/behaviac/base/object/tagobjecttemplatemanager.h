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

#ifndef BEHAVIAC_ENGINESERVICES_TAGOBJECTTEMPLATEMANAGER_H
#define BEHAVIAC_ENGINESERVICES_TAGOBJECTTEMPLATEMANAGER_H

#include "behaviac/base/core/singleton.h"
#include "behaviac/base/serialization/serializablenode.h"
class CTagObject;

class BEHAVIAC_API CTagObjectTemplateManager
{
public:
    BEHAVIAC_DECLARE_MEMORY_OPERATORS(CTagObjectTemplateManager);

public:
    void ParseTemplates();

    virtual const behaviac::ISerializableNode* GetTemplate(const behaviac::CNoCaseStringID& templateId);
    const behaviac::ISerializableNode* GetTemplate(const char* templateName)
    {
        return GetTemplate(behaviac::CNoCaseStringID(templateName));
    }
    const behaviac::ISerializableNode* GetTemplate(const behaviac::string& templateName)
    {
        return GetTemplate(templateName.c_str());
    }

    const char* GetTemplatesPath() const;

    void NotifyTemplateChanged(const behaviac::string& templateName);
    void RegisterTemplateChangeListener(const char* templateName, CTagObject* tagObject);
    void UnregisterTemplateChangeListener(const char* templateName);
    void UnregisterTemplateChangeListener(CTagObject* tagObject);

protected:
    CTagObjectTemplateManager();
    virtual ~CTagObjectTemplateManager();

    behaviac::SerializableNodeRef m_root;
    behaviac::map<behaviac::CNoCaseStringID, const behaviac::ISerializableNode*> m_templates;
    behaviac::map<behaviac::CNoCaseStringID, CTagObject*> m_templateChangeListeners;

    BEHAVIAC_DELCARE_SINGLETON(CTagObjectTemplateManager);
};

#endif //BEHAVIAC_ENGINESERVICES_TAGOBJECTTEMPLATEMANAGER_H
