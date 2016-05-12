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

#include "behaviac/base/object/tagobjecttemplatemanager.h"
#include "behaviac/base/object/tagobject.h"

BEHAVIAC_IMPLEMNT_SINGLETON(CTagObjectTemplateManager);

CTagObjectTemplateManager::CTagObjectTemplateManager()
{
}

CTagObjectTemplateManager::~CTagObjectTemplateManager()
{
}

const behaviac::ISerializableNode* CTagObjectTemplateManager::GetTemplate(const behaviac::CNoCaseStringID& templateId)
{
    behaviac::map<behaviac::CNoCaseStringID, const behaviac::ISerializableNode*>::const_iterator iter = m_templates.find(templateId);

    if (iter != m_templates.end())
    {
        return iter->second;
    }

    return NULL;
}

void CTagObjectTemplateManager::ParseTemplates()
{
}

void CTagObjectTemplateManager::RegisterTemplateChangeListener(const char* templateName, CTagObject* tagObject)
{
    m_templateChangeListeners.insert(std::make_pair(behaviac::CNoCaseStringID(templateName), tagObject));
}

void CTagObjectTemplateManager::UnregisterTemplateChangeListener(const char* templateName)
{
    m_templateChangeListeners.erase(behaviac::CNoCaseStringID(templateName));
}

void CTagObjectTemplateManager::UnregisterTemplateChangeListener(CTagObject* tagObject)
{
    behaviac::map<behaviac::CNoCaseStringID, CTagObject*>::iterator it(m_templateChangeListeners.begin());
    behaviac::map<behaviac::CNoCaseStringID, CTagObject*>::iterator itEnd(m_templateChangeListeners.end());

    for (; it != itEnd; ++it)
    {
        CTagObject* compareTest = it->second;

        if (compareTest == tagObject)
        {
            m_templateChangeListeners.erase(it);
            break;
        }
    }
}

void CTagObjectTemplateManager::NotifyTemplateChanged(const behaviac::string& templateName)
{
    // Retrieve template that changed
    const behaviac::ISerializableNode* node = GetTemplate(templateName);

    if (!node)
    {
        BEHAVIAC_ASSERT(!"Template changed, but could not find it!");
        return;
    }

    behaviac::map<behaviac::CNoCaseStringID, CTagObject*>::iterator it(m_templateChangeListeners.find(behaviac::CNoCaseStringID(templateName.c_str())));

    if (it != m_templateChangeListeners.end())
    {
        it->second->Load(node);
    }
}

const char* CTagObjectTemplateManager::GetTemplatesPath() const
{
    return "generated\\tagobjecttemplates.fcb";
}
