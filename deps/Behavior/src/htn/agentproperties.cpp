/////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Tencent is pleased to support the open source community by making behaviac available.
//
// Copyright (C) 2015 THL A29 Limited, a Tencent company. All rights reserved->
//
// Licensed under the BSD 3-Clause License (the "License"); you may not use this file except in compliance with
// the License. You may obtain a copy of the License at http://opensource.org/licenses/BSD-3-Clause
//
// Unless required by applicable law or agreed to in writing, software distributed under the License is
// distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied->
// See the License for the specific language governing permissions and limitations under the License.
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#include "behaviac/base/base.h"
#include "behaviac/agent/agent.h"
#include "behaviac/behaviortree/nodes/conditions/condition.h"
#include "behaviac/agent/taskmethod.h"
#include "behaviac/htn/agentproperties.h"

namespace behaviac
{
    CppBehaviorLoader* AgentProperties::ms_cppBehaviorLoader = NULL;

    BEHAVIAC_API uint32_t MakeVariableId(const char* idString);

    Property* AgentProperties::AddProperty(const char* typeName, bool bIsStatic, const char* variableName, const char* valueStr, const char* agentType)
    {
        Property* pProperty = Property::Create(typeName, "Self", agentType, variableName, valueStr);
        pProperty->m_bIsStatic = bIsStatic;

        uint32_t variableId = MakeVariableId(variableName);
        this->m_properties[variableId] = pProperty;
        return pProperty;
    }

    Property* AgentProperties::GetProperty(uint32_t variableId)
    {
        if (this->m_properties.size() > 0)
        {
			behaviac::map<uint32_t, Property*>::const_iterator it = this->m_properties.find(variableId);
			if (it != this->m_properties.end()) {
				Property* p = it->second;
				return p;
			}
        }

        if (this->m_locals.size() > 0)
        {
			behaviac::map<uint32_t, Property*>::const_iterator it = this->m_locals.find(variableId);
			if (it != this->m_locals.end()) {
				Property* p = it->second;
				return p;
			}
        }

        return NULL;
    }

    Property* AgentProperties::Getproperty(const char* variableName)
    {
        uint32_t variableId = MakeVariableId(variableName);
        Property* p = this->GetProperty(variableId);
        return p;
    }

	void AgentProperties::AddPropertyInstance(Property* pPropertyInstance)
	{
		this->m_properties_instance.push_back(pPropertyInstance);
	}

    Property* AgentProperties::AddLocal(const char* typeName, const char* variableName, const char* valueStr)
    {
        BEHAVIAC_ASSERT(variableName[strlen(variableName - 1)] != ']');
        const char* agentType = NULL;
        Property* pProperty = Property::Create(typeName, "Self", agentType, variableName, valueStr);
        pProperty->m_bIsLocal = true;

        uint32_t variableId = MakeVariableId(variableName);

        this->m_locals[variableId] = pProperty;
        return pProperty;
    }

    void AgentProperties::ClearLocals()
    {
        for (behaviac::map<uint32_t, Property*>::iterator it = this->m_locals.begin(); it != this->m_locals.end(); ++it)
        {
            BEHAVIAC_DELETE it->second;
        }

        this->m_locals.clear();
    }

    void AgentProperties::cleanup()
    {
        for (behaviac::map<uint32_t, Property*>::iterator it = this->m_properties.begin(); it != this->m_properties.end(); ++it)
        {
            BEHAVIAC_DELETE it->second;
        }

		for (behaviac::vector<Property*>::iterator it = this->m_properties_instance.begin(); it != this->m_properties_instance.end(); ++it)
		{
			BEHAVIAC_DELETE *it;
		}

		this->m_properties_instance.clear();

        this->m_properties.clear();

        for (behaviac::map<uint32_t, Property*>::iterator it = this->m_locals.begin(); it != this->m_locals.end(); ++it)
        {
            Property* pProperty = it->second;
            BEHAVIAC_DELETE pProperty;
        }

        this->m_locals.clear();
    }

    behaviac::map<behaviac::string, AgentProperties*> AgentProperties::agent_type_blackboards;

    AgentProperties::AgentProperties(const char* agentType)
    {
        BEHAVIAC_UNUSED_VAR(agentType);
    }

    AgentProperties::~AgentProperties()
    {
    }
    void AgentProperties::Instantiate(behaviac::Agent* pAgent)
    {
        for (map<uint32_t, Property*>::iterator it = this->m_properties.begin(); it != this->m_properties.end(); ++it)
        {
            Property* _property = it->second;
            _property->Instantiate(pAgent);
        }
    }
    Property* AgentProperties::GetLocal(const char* variableName)
    {
		if (this->m_locals.size() > 0) {
			uint32_t variableid = MakeVariableId(variableName);

			behaviac::map<uint32_t, Property*>::const_iterator it = this->m_locals.find(variableid);
			if (it != this->m_locals.end())
			{
				Property* p = it->second;
				return p;
			}
		}

        return 0;
    }

    Property* AgentProperties::AddLocal(const char* agentType, const char* typeName, const char* variableName, const char* valueStr)
    {
        AgentProperties* bb = AgentProperties::Get(agentType);

        //if agent type has no property and custom property
        if (bb == NULL)
        {
            bb = BEHAVIAC_NEW AgentProperties(agentType);
            agent_type_blackboards[agentType] = bb;
        }

        Property* pProperty = bb->AddLocal(typeName, variableName, valueStr);
        return pProperty;
    }

    Property* AgentProperties::GetProperty(const char* agentType, const char* variableName)
    {
        AgentProperties* bb = AgentProperties::Get(agentType);

        if (bb)
        {
            Property* pProperty = bb->Getproperty(variableName);
            return pProperty;
        }
        else
        {
			//cpp, there is no corresponding AgentProperties
            //BEHAVIAC_LOGWARNING("behaviac.bb is not loaded? Is SetVariable/GetVariable invoked too early?\n");
        }

        return 0;
    }

    Property* AgentProperties::GetProperty(const char* agentType, uint32_t variableId)
    {
        AgentProperties* bb = AgentProperties::Get(agentType);

        if (bb)
        {
            Property* pProperty = bb->GetProperty(variableId);
            return pProperty;
        }
        else
        {
			//cpp, there is no corresponding AgentProperties
            //BEHAVIAC_LOGWARNING("behaviac.bb is not loaded? Is SetVariable/GetVariable invoked too early?\n");
        }

        return 0;
    }

    AgentProperties* AgentProperties::Get(const char* agentType)
    {
		behaviac::map<behaviac::string, AgentProperties*>::iterator it = agent_type_blackboards.find(agentType);

		if (it != agent_type_blackboards.end())
        {
			return it->second;
        }

        return NULL;
    }

	void AgentProperties::AddPropertyInstance(const char* agentType, Property* pPropertyInstance)
	{
		AgentProperties* bb = AgentProperties::Get(agentType);

		if (bb)
		{
			bb->AddPropertyInstance(pPropertyInstance);
		}
		else
		{
			//cpp, there is no corresponding AgentProperties
			//BEHAVIAC_LOGWARNING("behaviac.bb is not loaded? Is SetVariable/GetVariable invoked too early?\n");
		}
	}

    void AgentProperties::UnloadLocals()
    {
        for (map<behaviac::string, AgentProperties*>::iterator it = agent_type_blackboards.begin(); it != agent_type_blackboards.end(); it++)
        {
            AgentProperties* bb = it->second;

			if (bb)
			{
				bb->ClearLocals();
			}
        }
    }

    void AgentProperties::Cleanup()
    {
        for (map<behaviac::string, AgentProperties*>::iterator it = agent_type_blackboards.begin(); it != agent_type_blackboards.end(); it++)
        {
            AgentProperties* bb = it->second;

            if (bb)
            {
                bb->cleanup();
                BEHAVIAC_DELETE(bb);
            }
        }

        agent_type_blackboards.clear();
    }

    void AgentProperties::load_methods(BsonDeserizer* d, const char* agentType, BsonDeserizer::BsonTypes type)
    {
        CTagObjectDescriptor* objectDesc = (CTagObjectDescriptor*)Agent::GetDescriptorByName(agentType);

        d->OpenDocument();
        type = d->ReadType();

        while (type == BsonDeserizer::BT_MethodElement)
        {
            d->OpenDocument();

            const char* methodName = d->ReadString();
            //string returnTypeStr = d->ReadString();
            //returnTypeStr = returnTypeStr.Replace("::", ".");
            //string isStatic = d->ReadString();
            //string eventStr = d->ReadString();
            //bool bEvent = (eventStr == "true");
            const char* agentStr = d->ReadString();
            BEHAVIAC_UNUSED_VAR(agentStr);

            behaviac::vector<behaviac::string> params;

            type = d->ReadType();

            while (type == BsonDeserizer::BT_ParameterElement)
            {
                d->OpenDocument();
                const char* paramName = d->ReadString();
                BEHAVIAC_UNUSED_VAR(paramName);
                const char* paramType = d->ReadString();

                params.push_back(paramType);

                d->CloseDocument(true);
                type = d->ReadType();
            }

            CCustomMethod* customeMethod = BEHAVIAC_NEW CTaskMethod(agentType, methodName);

            customeMethod->AddParams(params);

            objectDesc->ms_methods.push_back(customeMethod);

            d->CloseDocument(false);
            type = d->ReadType();
        }

        d->CloseDocument(false);
    }

    bool AgentProperties::load_agent(int version, BsonDeserizer* d)
    {
        BEHAVIAC_UNUSED_VAR(version);
        d->OpenDocument();

        const char* agentType = d->ReadString();
        const char* pBaseName = d->ReadString();
        BEHAVIAC_UNUSED_VAR(pBaseName);
        AgentProperties* bb = BEHAVIAC_NEW AgentProperties(agentType);
        agent_type_blackboards[agentType] = bb;

        BsonDeserizer::BsonTypes type = d->ReadType();

        while (type != BsonDeserizer::BT_None)
        {
            if (type == BsonDeserizer::BT_PropertiesElement)
            {
                d->OpenDocument();
                type = d->ReadType();

                while (type != BsonDeserizer::BT_None)
                {
                    if (type == BsonDeserizer::BT_PropertyElement)
                    {
                        d->OpenDocument();
                        const char* variableName = d->ReadString();
                        const char* typeName = d->ReadString();
                        const char* memberStr = d->ReadString();
                        bool bIsMember = false;

                        if (!StringUtils::IsNullOrEmpty(memberStr) && strcmp(memberStr, "true") == 0)
                        {
                            bIsMember = true;
                        }

                        const char* isStatic = d->ReadString();
                        bool bIsStatic = false;

                        if (!StringUtils::IsNullOrEmpty(isStatic) && strcmp(isStatic, "true") == 0)
                        {
                            bIsStatic = true;
                        }

                        const char* valueStr = NULL;
                        //string agentTypeMember = agentType;
                        const char* agentTypeMember = NULL;

                        if (!bIsMember)
                        {
                            valueStr = d->ReadString();

                        }
                        else
                        {
                            agentTypeMember = d->ReadString();
                        }

                        d->CloseDocument(true);

                        bb->AddProperty(typeName, bIsStatic, variableName, valueStr, agentTypeMember);

                    }
                    else
                    {
                        BEHAVIAC_ASSERT(false);
                    }

                    type = d->ReadType();
                }//end of while

                d->CloseDocument(false);

            }
            else if (type == BsonDeserizer::BT_MethodsElement)
            {
                load_methods(d, agentType, type);

            }
            else
            {
                BEHAVIAC_ASSERT(type == BsonDeserizer::BT_None);
            }

            type = d->ReadType();
        }

        d->CloseDocument(false);
        return true;
    }

    bool AgentProperties::load_bson(const char* pBuffer)
    {
        BsonDeserizer* d = BEHAVIAC_NEW BsonDeserizer();

        if (d->Init(pBuffer))
        {
            BsonDeserizer::BsonTypes type = d->ReadType();

            if (type == BsonDeserizer::BT_AgentsElement)
            {
                bool bOk = d->OpenDocument();
                BEHAVIAC_UNUSED_VAR(bOk);
                BEHAVIAC_ASSERT(bOk);

                const char* verStr = d->ReadString();
                int version = atoi(verStr);

                {
                    type = d->ReadType();

                    while (type != BsonDeserizer::BT_None)
                    {
                        if (type == BsonDeserizer::BT_AgentElement)
                        {
                            load_agent(version, d);
                        }

                        type = d->ReadType();
                    }

                    BEHAVIAC_ASSERT(type == BsonDeserizer::BT_None);
                }

                d->CloseDocument(false);
                return true;
            }

            BEHAVIAC_DELETE d;
        }

        BEHAVIAC_ASSERT(false);
        return false;
    }

    bool AgentProperties::load_xml(char* pBuffer)
    {
        BEHAVIAC_ASSERT(pBuffer != NULL);
        rapidxml::xml_document<> doc;
        doc.parse<0>(pBuffer);
        rapidxml::xml_node<>* rootNode = doc.first_node("agents");

        if (rootNode == 0 || !StringUtils::StrEqual(rootNode->name(), "agents"))
        {
            return false;
        }

        rapidxml::xml_node<>* children = rootNode->first_node();

        if (children == 0)
        {
            return false;
        }

        // const char* versionStr = rootNode->first_attribute("version")->value();

        for (rapidxml::xml_node<>* bbNode = children; bbNode; bbNode = bbNode->next_sibling())
        {
            if (StringUtils::StrEqual(bbNode->name(), "agent") && bbNode->first_node() != NULL)
            {
                const char* agentType = bbNode->first_attribute("type")->value();

                AgentProperties* bb = BEHAVIAC_NEW AgentProperties(agentType);
                agent_type_blackboards[agentType] = bb;

                for (rapidxml::xml_node<>* propertiesNode = bbNode->first_node(); propertiesNode; propertiesNode = propertiesNode->next_sibling())
                {
                    const char* propertiesNodeTag = propertiesNode->name();

                    if (strcmp(propertiesNodeTag, "properties") == 0 && propertiesNode->first_node() != NULL)
                    {
                        for (rapidxml::xml_node<>* propertyNode = propertiesNode->first_node(); propertyNode; propertyNode = propertyNode->next_sibling())
                        {
                            if (strcmp(propertyNode->name(), "property") == 0)
                            {
                                const char* name = propertyNode->first_attribute("name")->value();
                                const char* type = propertyNode->first_attribute("type")->value();
                                const char*  memberStr = propertyNode->first_attribute("member")->value();
                                bool bIsMember = false;

                                if (!StringUtils::IsNullOrEmpty(memberStr) && strcmp(memberStr, "true") == 0)
                                {
                                    bIsMember = true;
                                }

                                const char* isStatic = propertyNode->first_attribute("static")->value();
                                bool bIsStatic = false;

                                if (!StringUtils::IsNullOrEmpty(isStatic) && strcmp(isStatic, "true") == 0)
                                {
                                    bIsStatic = true;
                                }

                                //string agentTypeMember = agentType;
                                char* agentTypeMember = NULL;
                                char* valueStr = NULL;

                                if (!bIsMember)
                                {
                                    valueStr = propertyNode->first_attribute("defaultvalue")->value();

                                }
                                else
                                {
                                    agentTypeMember = propertyNode->first_attribute("agent")->value();
                                }

                                bb->AddProperty(type, bIsStatic, name, valueStr, agentTypeMember);
                            }
                        }
                    }
                    else if (strcmp(propertiesNodeTag, "methods") == 0 && propertiesNode->first_node() != NULL)
                    {
                        CTagObjectDescriptor* objectDesc = (CTagObjectDescriptor*)Agent::GetDescriptorByName(agentType);

                        for (rapidxml::xml_node<>* methodNode = propertiesNode->first_node(); methodNode; methodNode = methodNode->next_sibling())
                        {
                            const char* methodNodeTag = methodNode->name();

                            if (strcmp(methodNodeTag, "method") == 0)
                            {
                                const char* methodName = methodNode->first_attribute("name")->value();

                                behaviac::vector<behaviac::string> params;

                                for (rapidxml::xml_node<>* paramNode = methodNode->first_node(); paramNode; paramNode = paramNode->next_sibling())
                                {
                                    const char* paramNodeTag = paramNode->name();

                                    if (strcmp(paramNodeTag, "parameter") == 0)
                                    {
                                        // const char* paramName = paramNode->first_attribute("name")->value();
                                        const char* paramType = paramNode->first_attribute("type")->value();

                                        params.push_back(paramType);
                                    }
                                }

                                CCustomMethod* customeMethod = BEHAVIAC_NEW CTaskMethod(agentType, methodName);

                                customeMethod->AddParams(params);

                                objectDesc->ms_methods.push_back(customeMethod);
                            }
                        }//end of for methodNode
                    }//end of methods
                }//end of for propertiesNode
            }
        }//end of for bbNode

        return true;
    }

    void AgentProperties::SetAgentTypeBlackboards(const char* agentType, AgentProperties* agentProperties)
    {
        agent_type_blackboards[agentType] = agentProperties;
    }

    void AgentProperties::SetInstance(CppBehaviorLoader* cppBehaviorLoader)
    {
        ms_cppBehaviorLoader = cppBehaviorLoader;
    }

    bool AgentProperties::load_cpp()
    {
        if (ms_cppBehaviorLoader)
        {
            return ms_cppBehaviorLoader->load();
        }

        return false;
    }

    void AgentProperties::RegisterCustomizedTypes()
    {
        if (ms_cppBehaviorLoader)
        {
            return ms_cppBehaviorLoader->RegisterCustomizedTypes_();
        }
    }

    void AgentProperties::UnRegisterCustomizedTypes()
    {
        if (ms_cppBehaviorLoader)
        {
            return ms_cppBehaviorLoader->UnRegisterCustomizedTypes_();
        }
    }

    bool AgentProperties::Load()
    {
        //the workspace export path is provided by Workspace::GetFilePath
        //the file format(xml / bson) is provided by Workspace::GetFileFormat
        //generally, you need to derive Workspace and override GetFilePath and GetFileFormat,
        //then, instantiate your derived Workspace at the very beginning
        string relativePath = "behaviac.bb";

        string fullPath = StringUtils::CombineDir(Workspace::GetInstance()->GetFilePath(), relativePath.c_str());

        bool bLoadResult = false;
        Workspace::EFileFormat f = Workspace::GetInstance()->GetFileFormat();
        string ext = "";

        Workspace::GetInstance()->HandleFileFormat(fullPath, ext, f);

        switch (f)
        {
            case Workspace::EFF_default:
                BEHAVIAC_ASSERT(false);
                break;

            case Workspace::EFF_xml:
            {
                char* pBuffer = Workspace::GetInstance()->ReadFileToBuffer(fullPath.c_str(), ext.c_str());

                if (pBuffer != NULL)
                {
                    bLoadResult = load_xml(pBuffer);

                    Workspace::GetInstance()->PopFileFromBuffer(fullPath.c_str(), ext.c_str(), pBuffer);
                }
                else
                {
                    BEHAVIAC_LOGERROR("'%s%s' doesn't exist!, Please check the file name or override Workspace and its GetFilePath()\n", fullPath.c_str(), ext.c_str());
                    BEHAVIAC_ASSERT(false);
                }
            }
            break;

            case Workspace::EFF_bson:
            {
                char* pBuffer = Workspace::GetInstance()->ReadFileToBuffer(fullPath.c_str(), ext.c_str());

                if (pBuffer != NULL)
                {
                    bLoadResult = load_bson(pBuffer);

                    Workspace::GetInstance()->PopFileFromBuffer(fullPath.c_str(), ext.c_str(), pBuffer);
                }
                else
                {
                    BEHAVIAC_ASSERT(false);
                }
            }
            break;

            case Workspace::EFF_cpp:
            {
                load_cpp();
            }
            break;

            default:
                BEHAVIAC_ASSERT(false);
                break;
        }

        return bLoadResult;
    }
}
