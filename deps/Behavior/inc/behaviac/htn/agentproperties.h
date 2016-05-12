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

#ifndef BEHAVIAC_HTN_AGENTPROPERTIES_H
#define BEHAVIAC_HTN_AGENTPROPERTIES_H
#include "behaviac/base/base.h"
#include "behaviac/behaviortree/behaviortree.h"
#include "behaviac/behaviortree/behaviortree_task.h"
#include "behaviac/property/property.h"

namespace behaviac
{
    class BEHAVIAC_API CppBehaviorLoader
    {
    public:
        virtual ~CppBehaviorLoader() {}
        virtual bool load() = 0;
        virtual void RegisterCustomizedTypes_() = 0;
        virtual void UnRegisterCustomizedTypes_() = 0;
    };

    class BEHAVIAC_API AgentProperties
    {
    public:
        BEHAVIAC_DECLARE_MEMORY_OPERATORS(AgentProperties);
        AgentProperties(const char* agentType);
        ~AgentProperties();

        void Instantiate(behaviac::Agent* pAgent);
        Property* GetLocal(const char* variableName);
        Property* AddProperty(const char* typeName, bool bIsStatic, const char* variableName, const char* valueStr, const char* agentType);

        static Property* AddLocal(const char* agentType, const char* typeName, const char* variableName, const char* valueStr);
        static Property* GetProperty(const char* agentType, const char* variableName);
        static Property* GetProperty(const char* agentType, uint32_t variableId);
        static AgentProperties* Get(const char* agentType);

		static void AddPropertyInstance(const char* agentType, Property* pPropertyInstance);

        static void UnloadLocals();

        static void Cleanup();

        static bool Load();
        static void RegisterCustomizedTypes();
        static void UnRegisterCustomizedTypes();

        static void SetInstance(CppBehaviorLoader* cppBehaviorLoader);
        static void SetAgentTypeBlackboards(const char* agentType, AgentProperties* agentProperties);

    private:
        static CppBehaviorLoader* ms_cppBehaviorLoader;
        static map<behaviac::string, AgentProperties*> agent_type_blackboards;

        behaviac::map<uint32_t, Property*> m_properties;
        behaviac::map<uint32_t, Property*> m_locals;

		//some properties can't be shared, so they are cloned and kept here to free them
        behaviac::vector<Property*> m_properties_instance;

        Property* GetProperty(uint32_t variableId);
        Property* Getproperty(const char* variableName);
        Property* AddLocal(const char* typeName, const char* variableName, const char* valueStr);

		void AddPropertyInstance(Property* pPropertyInstance);

        void ClearLocals();
        void cleanup();

        static bool load_xml(char* pBuffer);
        static bool load_bson(const char* pBuffer);
        static bool load_cpp();
        static bool load_agent(int version, BsonDeserizer* d);
        static void load_methods(BsonDeserizer* d, const char* agentType, BsonDeserizer::BsonTypes type);
    };
}
#endif
