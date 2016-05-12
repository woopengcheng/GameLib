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

#include "behaviac/agent/agent.h"
#include "behaviac/agent/registermacros.h"
#include "behaviac/agent/context.h"
#include "behaviac/agent/state.h"

#include "behaviac/property/property.h"
#include "behaviac/property/property_t.h"
#include "behaviac/base/file/filesystem.h"
#include "behaviac/base/object/method.h"
#include "behaviac/behaviortree/behaviortree.h"
#include "behaviac/behaviortree/behaviortree_task.h"

#include "behaviac/base/core/profiler/profiler.h"
#include "behaviac/htn/agentproperties.h"

#include "./propertynode.h"

#if BEHAVIAC_COMPILER_MSVC
#define _BEHAVIAC_M_STRING_COMPILER_NAME_ BEHAVIAC_JOIN_TOKENS("compiler ", BEHAVIAC_COMPILER_NAME)
#pragma message (_BEHAVIAC_M_STRING_COMPILER_NAME_)
#endif

BEGIN_PROPERTIES_DESCRIPTION(IList)
{
    //
}
END_PROPERTIES_DESCRIPTION()

BEGIN_PROPERTIES_DESCRIPTION(System::Object)
{
    //
}
END_PROPERTIES_DESCRIPTION()

BEGIN_PROPERTIES_DESCRIPTION(behaviac::Agent)
{
    //REGISTER_MEMBER
    REGISTER_METHOD(VectorLength);
    REGISTER_METHOD(VectorAdd);
    REGISTER_METHOD(VectorRemove);
    REGISTER_METHOD(VectorContains);
    REGISTER_METHOD(VectorClear);
}
END_PROPERTIES_DESCRIPTION()

namespace behaviac
{
	bool TryStart();

    uint32_t Agent::ms_idMask = 0xffffffff;
    Agent::AgentTypeIndexMap_t* Agent::ms_agent_type_index = 0;

    uint32_t Agent::IdMask()
    {
        return Agent::ms_idMask;
    }

    void Agent::SetIdMask(uint32_t idMask)
    {
        Agent::ms_idMask = idMask;
    }

    int Agent::ms_agent_index = 0;
    CFactory<Agent>* Agent::ms_factory;

    Agent::AgentMetas_t* Agent::ms_metas;
    Agent::AgentMetas_t& Agent::Metas()
    {
        if (!ms_metas)
        {
            ms_metas = BEHAVIAC_NEW AgentMetas_t;
        }

        BEHAVIAC_ASSERT(ms_metas);
        return *ms_metas;
    }

    Agent::AgentNames_t* Agent::ms_names;
    Agent::AgentNames_t& Agent::Names()
    {
        if (!ms_names)
        {
            ms_names = BEHAVIAC_NEW AgentNames_t;
        }

        BEHAVIAC_ASSERT(ms_names);

        return *ms_names;
    }

    CFactory<Agent>& Agent::Factory()
    {
        if (!ms_factory)
        {
            typedef CFactory<Agent> FactoryAgent;
            ms_factory = BEHAVIAC_NEW FactoryAgent;
        }

        return *ms_factory;
    }

    //m_id == -1, not a valid agent
    Agent::Agent() : m_context_id(-1), m_currentBT(0), m_id(-1), m_priority(0), m_bActive(1), m_referencetree(false), _balckboard_bound(false), m_idFlag(0xffffffff), m_planningTop(-1)
    {
		bool bOk = TryStart();
		BEHAVIAC_ASSERT(bOk);
		BEHAVIAC_UNUSED_VAR(bOk);

#if !BEHAVIAC_RELEASE
        this->m_debug_verify = 0;
#endif//#if !BEHAVIAC_RELEASE
    }

    void Agent::SetName(const char* instanceName)
    {
        if (!instanceName)
        {
            uint32_t	typeId = 0;
            const char* typeFullName = this->GetObjectTypeName();

            const char* typeName = typeFullName;
            const char* pIt = strrchr(typeFullName, ':');

            if (pIt)
            {
                typeName = pIt + 1;
            }

            if (!ms_agent_type_index)
            {
                ms_agent_type_index = BEHAVIAC_NEW AgentTypeIndexMap_t;
            }

            AgentTypeIndexMap_t::iterator it = ms_agent_type_index->find(typeFullName);

            if (it == ms_agent_type_index->end())
            {
                typeId = 0;
                (*ms_agent_type_index)[typeFullName] = 1;

            }
            else
            {
                typeId = (*ms_agent_type_index)[typeFullName]++;
            }

            this->m_name += FormatString("%s_%d_%d", typeName, typeId, this->m_id);

        }
        else
        {
            this->m_name = instanceName;
        }
    }

    Agent::~Agent()
    {
#if BEHAVIAC_ENABLE_NETWORKD
        this->UnSubsribeToNetwork();
#endif//#if BEHAVIAC_ENABLE_NETWORKD

#if !BEHAVIAC_RELEASE
        Agent::Agents_t* agents = Agents(true);

        if (agents)
        {
            const char* agentClassName = this->GetObjectTypeName();
            const behaviac::string& instanceName = this->GetName();

            behaviac::string aName = FormatString("%s#%s", agentClassName, instanceName.c_str());

            agents->erase(aName);
        }

#endif

        for (BehaviorTreeTasks_t::iterator it = this->m_behaviorTreeTasks.begin(); it != m_behaviorTreeTasks.end(); ++it)
        {
            BehaviorTreeTask* bt = *it;

            Workspace::GetInstance()->DestroyBehaviorTreeTask(bt, this);
        }

        this->m_behaviorTreeTasks.clear();

        for (AgentEvents_t::iterator it = this->m_eventInfos.begin(); it != this->m_eventInfos.end(); ++it)
        {
            CNamedEvent* p = it->second;
            BEHAVIAC_DELETE(p);
        }

        this->m_eventInfos.clear();

        this->m_variables.Clear(true);
    }

	void Agent::destroy_()
	{
		int contextId = this->GetContextId();
		Context& c = Context::GetContext(contextId);

		c.RemoveAgent(this);

		// It should be deleted absolutely here.
		if (!c.IsExecuting())
		{
			delete this;
		}
	}

    void Agent::Init_(int contextId, Agent* pAgent, short priority, const char* agentInstanceName)
    {
#if !BEHAVIAC_RELEASE
        pAgent->m_debug_verify = kAGENT_DEBUG_VERY;
#endif//#if !BEHAVIAC_RELEASE

        BEHAVIAC_ASSERT(contextId >= 0, "invalid context id");

        pAgent->m_context_id = contextId;
        pAgent->m_id = ms_agent_index++;
        pAgent->m_priority = priority;

        pAgent->SetName(agentInstanceName);
        pAgent->InitVariableRegistry();

        Context& c = Context::GetContext(contextId);
        c.AddAgent(pAgent);

#if !BEHAVIAC_RELEASE
        Agent::Agents_t* agents = Agents(false);
        BEHAVIAC_ASSERT(agents);

        const char* agentClassName = pAgent->GetObjectTypeName();
        const behaviac::string& instanceName = pAgent->GetName();

        behaviac::string aName = FormatString("%s#%s", agentClassName, instanceName.c_str());

        (*agents)[aName] = pAgent;
#endif//BEHAVIAC_RELEASE

#if BEHAVIAC_ENABLE_NETWORKD
        pAgent->SubsribeToNetwork();
#endif//#if BEHAVIAC_ENABLE_NETWORKD
    }

    bool Agent::IsRegistered(const char* agentClassName)
    {
        CStringID classId(agentClassName);
        return Factory().IsRegistered(classId);
    }

    bool Agent::IsAgentClassName(const char* agentClassName)
    {
        CStringID agentClassId(agentClassName);

        bool bResult = IsAgentClassName(agentClassId);

        return bResult;
    }

    bool Agent::IsAgentClassName(const CStringID& agentClassId)
    {
        //CStringID classId(agentClassName);
        //return Factory().IsRegistered(classId);
        AgentMetas_t::iterator it = Agent::ms_metas->find(agentClassId);

        if (it != Agent::ms_metas->end())
        {
            return true;
        }

        return false;
    }

    bool Agent::SaveDataToFile(const char* fileName)
    {
        BEHAVIAC_ASSERT(fileName);
        const char* className = this->GetObjectTypeName();
        XmlNodeRef xmlInfo = CreateXmlNode(className);
        this->SaveToXML(xmlInfo);

        CFileSystem::MakeSureDirectoryExist(fileName);
        return xmlInfo->saveToFile(fileName);
    }

    bool Agent::LoadDataFromFile(const char* fileName)
    {
        BEHAVIAC_ASSERT(fileName);
        const char* className = this->GetObjectTypeName();
        XmlNodeRef xmlInfo = CreateXmlNode(className);
        CTextNode node(xmlInfo);

        if (node.LoadFromFile(fileName))
        {
            this->Load(&node);
            return true;
        }

        return false;
    }

    bool Agent::SaveDataToFile(IFile* file)
    {
        const char* className = this->GetObjectTypeName();
        XmlNodeRef xmlInfo = CreateXmlNode(className);
        this->SaveToXML(xmlInfo);

        return xmlInfo->saveToFile(file);
    }

    bool Agent::LoadDataFromFile(IFile* file)
    {
        const char* className = this->GetObjectTypeName();
        XmlNodeRef xmlInfo = CreateXmlNode(className);
        CTextNode node(xmlInfo);

        if (node.LoadFromFile(file))
        {
            this->Load(&node);
            return true;
        }

        return false;
    }

    //bool Agent::SaveStateToFile(const char* fileName)
    //{
    //    BEHAVIAC_ASSERT(fileName);
    //    const char* className = this->GetObjectTypeName();
    //    XmlNodeRef xmlInfo = CreateXmlNode(className);
    //    CTextNode node(xmlInfo);
    //    this->SaveState(&node);
    //    CFileSystem::MakeSureDirectoryExist(fileName);
    //    return xmlInfo->saveToFile(fileName);
    //}

    //bool Agent::SaveTypeToFile(const char* fileName)
    //{
    //    const char* className = this->GetObjectTypeName();
    //    XmlNodeRef xmlInfo = CreateXmlNode(className);
    //    this->GetMembersDescription(xmlInfo);
    //    this->GetMethodsDescription(xmlInfo);
    //    CFileSystem::MakeSureDirectoryExist(fileName);
    //    return xmlInfo->saveToFile(fileName);
    //}

    bool Agent::MetaComparator::operator()(const Agent::MetaInfo_t* _left, const Agent::MetaInfo_t* _right)
    {
        if (_left->bInternal && !_right->bInternal)
        {
            return true;

        }
        else if (!_left->bInternal && _right->bInternal)
        {
            return false;
        }

        if (!_left->baseClassFullName && _right->baseClassFullName)
        {
            return true;

        }
        else if (_left->baseClassFullName && !_right->baseClassFullName)
        {
            return false;
        }

        int pos = strcmp(_left->classFullName, _right->classFullName);

        return pos <= 0;
    }

    bool Agent::NameComparator::operator()(const Agent::AgentName_t* _left, const Agent::AgentName_t* _right)
    {
        return _left->instantceName_ < _right->instantceName_;
    }

    struct ObjectDescriptorComparator
    {
        bool operator()(const char* _left, const char* _right)
        {
            if (string_icmp(_left, _right) < 0)
            {
                return true;
            }

            return false;
        }
    };


    bool Agent::ExportMetas(const char* xmlMetaFilePath)
    {
        BEHAVIAC_UNUSED_VAR(xmlMetaFilePath);
#if !BEHAVIAC_RELEASE

        if (Config::IsDesktopPlatform())
        {
            behaviac::vector<const MetaInfo_t*> metasSorted;

            const AgentMetas_t& metas = Metas();

            if (metas.size() > 1)
            {
                for (AgentMetas_t::const_iterator it = metas.begin(); it != metas.end(); ++it)
                {
                    //const CStringID& classId = it->first;
                    const MetaInfo_t& m = it->second;

                    metasSorted.push_back(&m);
                }
            }

            std::sort(metasSorted.begin(), metasSorted.end(), MetaComparator());

            XmlNodeRef metasNode = CreateXmlNode("metas");
            metasNode->setAttr("version", "3.0");
            metasNode->setAttr("language", "cpp");

            // collect all types
            CTagTypeDescriptor::TypesMap_t exportedTypes;
            behaviac::vector<const char*> exportedTypesVector;

            for (behaviac::vector<const MetaInfo_t*>::iterator it = metasSorted.begin(); it != metasSorted.end(); ++it)
            {
                const MetaInfo_t& m = *(*it);
                const CTagObjectDescriptor* pObjectDesc = m.descriptor;

                pObjectDesc->GetMembersDescription(&exportedTypes, 0, NULL);
                pObjectDesc->GetMethodsDescription(&exportedTypes, 0, NULL);
            }

            for (CTagTypeDescriptor::TypesMap_t::iterator it = exportedTypes.begin(); it != exportedTypes.end(); ++it)
            {
                exportedTypesVector.push_back(it->first);
            }

            std::sort(exportedTypesVector.begin(), exportedTypesVector.end(), ObjectDescriptorComparator());

            // export all enums
            XmlNodeRef typesInfo = metasNode->newChild("types");

            for (behaviac::vector<const char*>::iterator it = exportedTypesVector.begin(); it != exportedTypesVector.end(); ++it)
            {
                const char* pTypeName = *it;
                const CTagTypeDescriptor* pTypeDesc = exportedTypes[pTypeName];
                const EnumClassDescription_t* pEnumDesc = EnumClassDescription_t::DynamicCast(pTypeDesc);

                if (pEnumDesc)
                {
                    XmlNodeRef enumNode = typesInfo->newChild("enumtype");
                    enumNode->setAttr("Type", pTypeName);

                    pEnumDesc->GetMembersDescription(NULL, 0, enumNode);
                }
            }

            // export all structs
            for (behaviac::vector<const char*>::iterator it = exportedTypesVector.begin(); it != exportedTypesVector.end(); ++it)
            {
                const char* pTypeName = *it;
                const CTagTypeDescriptor* pTypeDesc = exportedTypes[pTypeName];
                const CTagObjectDescriptor* pObjectDesc = CTagObjectDescriptor::DynamicCast(pTypeDesc);

                if (pObjectDesc)
                {
                    XmlNodeRef structNode = typesInfo->newChild("struct");
                    structNode->setAttr("Type", pTypeName);

                    pObjectDesc->GetMembersDescription(NULL, 0, structNode);
                }
            }

            // export all agents
            XmlNodeRef xmlInfo = metasNode->newChild("agents");

            for (behaviac::vector<const MetaInfo_t*>::iterator it = metasSorted.begin(); it != metasSorted.end(); ++it)
            {
                const MetaInfo_t& m = *(*it);

                const CTagObjectDescriptor* pObjectDesc = m.descriptor;

                XmlNodeRef agentInfo = xmlInfo->newChild("agent");

                agentInfo->setAttr("classfullname", m.classFullName);

                if (m.baseClassFullName)
                {
                    agentInfo->setAttr("base", m.baseClassFullName);

                }
                else
                {
                    BEHAVIAC_ASSERT(m.bInternal);

                    ////classFullName is behaviac::Agent, don't add base
                    //if (strcmp(m.classFullName, "behaviac::Agent") != 0)
                    //{
                    //	agentInfo->setAttr("base", "behaviac::Agent");
                    //}
                }

                if (m.bInternal)
                {
                    agentInfo->setAttr("inherited", "true");
                }

                pObjectDesc->GetMembersDescription(NULL, 0, agentInfo);
                pObjectDesc->GetMethodsDescription(NULL, 0, agentInfo);
            }

            // export all instances
            Agent::AgentNames_t& names = Agent::Names();

            if (names.size() > 0)
            {
                behaviac::vector<const AgentName_t*> namesSorted;

                for (AgentNames_t::iterator it = names.begin(); it != names.end(); ++it)
                {
                    AgentName_t& m = it->second;

                    namesSorted.push_back(&m);
                }

                std::sort(namesSorted.begin(), namesSorted.end(), NameComparator());

                XmlNodeRef instances = metasNode->newChild("instances");

                for (behaviac::vector<const AgentName_t*>::const_iterator it = namesSorted.begin(); it != namesSorted.end(); ++it)
                {
                    //const behaviac::string& instanceName = it->first;
                    const AgentName_t& m = *(*it);

                    XmlNodeRef instance = instances->newChild("instance");

                    instance->setAttr("name", m.instantceName_);
                    instance->setAttr("class", m.classFullName_);
                    instance->setAttr("DisplayName", m.displayName_);
                    instance->setAttr("Desc", m.desc_);
                }
            }

            CFileSystem::MakeSureDirectoryExist(xmlMetaFilePath);
            return metasNode->saveToFile(xmlMetaFilePath);
        }

#endif//BEHAVIAC_RELEASE
        return false;
    }

    void Agent::Cleanup()
    {
        //agent meta + struct meta
        CleanupTagObjectDescriptorMaps();

#if !BEHAVIAC_RELEASE

        if (ms_agents)
        {
            ms_agents->clear();
            BEHAVIAC_DELETE(ms_agents);
            ms_agents = 0;
        }

#endif//

        //agent metas is part of TagObjectDescriptorMaps
        if (ms_metas)
        {
            ms_metas->clear();
            BEHAVIAC_DELETE(ms_metas);
            ms_metas = 0;
        }

        if (ms_names)
        {
            ms_names->clear();
            BEHAVIAC_DELETE(ms_names);
            ms_names = 0;
        }

        if (ms_agent_type_index)
        {
            ms_agent_type_index->clear();
            BEHAVIAC_DELETE(ms_agent_type_index);
            ms_agent_type_index = 0;
        }

        //enums meta
        CleanupEnumValueNameMaps();

        BEHAVIAC_DELETE(Agent::ms_factory);
        Agent::ms_factory = 0;

        Agent::ms_idMask = 0;
    }

#if BEHAVIAC_ENABLE_NETWORKD
    void Agent::SubsribeToNetwork()
    {
        behaviac::Network* pNw = behaviac::Network::GetInstance();

        if (pNw && !pNw->IsSinglePlayer())
        {
            const CTagObjectDescriptor& od = this->GetDescriptor();

            MethodsContainer::const_iterator it = od.ms_methods.begin();
            MethodsContainer::const_iterator itEnd = od.ms_methods.end();

            for (; it != itEnd; ++it)
            {
                CMethodBase* m = *it;
                //m->GetUiInfo(parent, xmlNode);
                m->SubsribeToNetwork(this);
            }
        }
    }

    void Agent::UnSubsribeToNetwork()
    {
        behaviac::Network* pNw = behaviac::Network::GetInstance();

        if (pNw && !pNw->IsSinglePlayer())
        {
            const CTagObjectDescriptor& od = this->GetDescriptor();

            MethodsContainer::const_iterator it = od.ms_methods.begin();
            MethodsContainer::const_iterator itEnd = od.ms_methods.end();

            for (; it != itEnd; ++it)
            {
                CMethodBase* m = *it;
                //m->GetUiInfo(parent, xmlNode);
                m->UnSubsribeToNetwork(this);
            }
        }
    }

    void Agent::ReplicateProperties()
    {
        behaviac::Network* pNw = behaviac::Network::GetInstance();

        if (pNw && !pNw->IsSinglePlayer())
        {
            const CTagObjectDescriptor& od = this->GetDescriptor();
            od.ReplicateProperties(this);
        }
    }
#endif//#if BEHAVIAC_ENABLE_NETWORKD
    void Agent::LogVariables(bool bForce)
    {
        BEHAVIAC_UNUSED_VAR(bForce);
#if !BEHAVIAC_RELEASE

        if (Config::IsLoggingOrSocketing())
        {
            BEHAVIAC_PROFILE("Agent::LogVariables");

            this->m_variables.Log(this, bForce);
        }

#endif//BEHAVIAC_RELEASE
    }

    void Agent::ResetChangedVariables()
    {
        this->m_variables.Reset();
    }

    void Agent::InitVariableRegistry()
    {
        this->ResetChangedVariables();

#if !BEHAVIAC_RELEASE

        if (Config::IsLoggingOrSocketing())
        {
            const char* className = this->GetObjectTypeName();

            CPropertyNode properyNode(this, className);

            this->Save(&properyNode);
        }

#endif
    }

    void Agent::UpdateVariableRegistry()
    {
        //#if !BEHAVIAC_RELEASE
        //		if (Config::IsLoggingOrSocketing())
        //		{
        //			BEHAVIAC_PROFILE("Agent::UpdateVariableRegistry");
        //
        //			const char* className = this->GetObjectTypeName();
        //
        //			CPropertyNode properyNode(this, className);
        //
        //			this->Save(&properyNode);
        //		}
        //#endif
#if BEHAVIAC_ENABLE_NETWORKD
        this->ReplicateProperties();
#endif//#if BEHAVIAC_ENABLE_NETWORKD
    }

    struct bt_finder
    {
        behaviac::string bt;

        bool operator()(const behaviac::string& it) const
        {
            return it == bt;
        }

        bt_finder(const char* btName) : bt(btName)
        {}
    };

    bool IsValidPath(const char* relativePath);

    void Agent::_btsetcurrent(const char* relativePath, TriggerMode triggerMode, bool bByEvent)
    {
        bool bEmptyPath = (!relativePath || *relativePath == '\0');
        BEHAVIAC_ASSERT(bEmptyPath || behaviac::StringUtils::FindExtension(relativePath) == 0);
        BEHAVIAC_ASSERT(IsValidPath(relativePath));

        if (!bEmptyPath)
        {
            //if (this->m_currentBT != 0 && this->m_currentBT->GetName() == relativePath)
            //{
            //	//the same bt is set again
            //	return;
            //}

            bool bLoaded = Workspace::GetInstance()->Load(relativePath);

            if (!bLoaded)
            {
                behaviac::string agentName = this->GetClassTypeName();
                agentName += "::";
                agentName += this->m_name;

                BEHAVIAC_ASSERT(false);
                BEHAVIAC_LOGINFO("%s is not a valid loaded behavior tree of %s", relativePath, agentName.c_str());
            }
            else
            {
                Workspace::GetInstance()->RecordBTAgentMapping(relativePath, this);

                if (this->m_currentBT)
                {
                    //if trigger mode is 'return', just push the current bt 'oldBt' on the stack and do nothing more
                    //'oldBt' will be restored when the new triggered one ends
                    if (triggerMode == TM_Return)
                    {
                        BehaviorTreeStackItem_t item(this->m_currentBT, triggerMode, bByEvent);
                        BEHAVIAC_ASSERT(this->m_btStack.size() < 200, "recursive?");
                        this->m_btStack.push_back(item);
                    }
                    else if (triggerMode == TM_Transfer)
                    {
                        //don't use the bt stack to restore, we just abort the current one.
                        //as the bt node has onenter/onexit, the abort can make them paired
                        //BEHAVIAC_ASSERT(this->m_currentBT->GetName() != relativePath);

                        this->m_currentBT->abort(this);
                        this->m_currentBT->reset(this);
                    }
                }

                BehaviorTreeTask* pTask = 0;

                for (BehaviorTreeTasks_t::iterator it = this->m_behaviorTreeTasks.begin(); it != this->m_behaviorTreeTasks.end(); ++it)
                {
                    BehaviorTreeTask* bt = *it;
                    BEHAVIAC_ASSERT(bt);

                    if (bt->GetName() == relativePath)
                    {
                        pTask = bt;
                        break;
                    }
                }

                bool bRecursive = false;

                if (pTask)
                {
                    for (BehaviorTreeStack_t::iterator it = this->m_btStack.begin(); it != this->m_btStack.end(); ++it)
                    {
                        BehaviorTreeStackItem_t& item = *it;

                        if (item.bt->GetName() == relativePath)
                        {
                            bRecursive = true;
                            break;
                        }
                    }

					if (pTask->GetStatus() != BT_INVALID) {
						pTask->reset(this);
					}
                }

                if (pTask == 0 || bRecursive)
                {
                    pTask = Workspace::GetInstance()->CreateBehaviorTreeTask(relativePath);
                    BEHAVIAC_ASSERT(pTask != 0);
                    this->m_behaviorTreeTasks.push_back(pTask);
                }

                this->m_currentBT = pTask;

				//this->_balckboard_bound = false;
				this->m_variables.Clear(false);
            }
        }
        else
        {
            BEHAVIAC_ASSERT(true);
        }
    }

    void Agent::btsetcurrent(const char* relativePath)
    {
        this->_btsetcurrent(relativePath, TM_Transfer, false);
    }

    void Agent::btreferencetree(const char* relativePath)
    {
        this->m_referencetree = true;
        this->_btsetcurrent(relativePath, TM_Return, false);
    }

    void Agent::bteventtree(const char* relativePath, TriggerMode triggerMode)
    {
        this->_btsetcurrent(relativePath, triggerMode, true);
    }

    void Agent::btresetcurrent()
    {
        if (this->m_currentBT != 0)
        {
            this->m_currentBT->reset(this);
        }
    }

    EBTStatus Agent::btexec_()
    {
        if (this->m_currentBT != NULL)
        {
            BehaviorTreeTask* pCurrent = this->m_currentBT;
            EBTStatus s = this->m_currentBT->exec(this);

            while (s != BT_RUNNING)
            {
                //this->m_currentBT->reset(this);
                if (this->m_btStack.size() > 0)
                {
                    //get the last one
                    BehaviorTreeStackItem_t& lastOne = this->m_btStack.back();
                    this->m_btStack.pop_back();
                    this->m_currentBT = lastOne.bt;


					bool bExecCurrent = false;

                    if (lastOne.triggerMode == TM_Return) {
                        if (!lastOne.triggerByEvent)
                        {
                            if (this->m_currentBT != pCurrent)
                            {
                                s = this->m_currentBT->resume(this, s);
                            }
                            else
                            {
                                BEHAVIAC_ASSERT(true);
                            }
                        }
						else {
							bExecCurrent = true;
						}
                    }
					else {
						bExecCurrent = true;
					}
                    
					if (bExecCurrent) {
                        pCurrent = this->m_currentBT;
                        s = this->m_currentBT->exec(this);
                        break;
                    }
                }
                else
                {
                    //don't clear it
                    //this->m_currentBT = 0;
                    break;
                }
            }

            return s;
        }
        else
        {
            //BEHAVIAC_LOGWARNING("NO ACTIVE BT!\n");
        }

        return BT_INVALID;
    }
    void Agent::InstantiateProperties()
    {
        if (!_balckboard_bound)
        {
			//this->m_variables.Clear(true);

            AgentProperties* bb = AgentProperties::Get(this->GetObjectTypeName());

            if (bb != NULL)
            {
                bb->Instantiate(this);
            }

            _balckboard_bound = true;
        }
    }
    EBTStatus Agent::btexec()
    {
#if BEHAVIAC_ENABLE_PROFILING
        BEHAVIAC_PROFILE("Agent::btexec");
#endif

        if (this->m_bActive)
        {
#if !BEHAVIAC_RELEASE
            BEHAVIAC_ASSERT(this->m_debug_verify == kAGENT_DEBUG_VERY, "Agent can only be created by Agent::Create or Agent::Create!");
			this->m_debug_count = 0;
#endif//#if !BEHAVIAC_RELEASE
            this->InstantiateProperties();

            this->UpdateVariableRegistry();

            EBTStatus s = this->btexec_();

            while (this->m_referencetree && s == BT_RUNNING)
            {
                this->m_referencetree = false;
                s = this->btexec_();
            }

            if (this->IsMasked())
            {
                this->LogVariables(false);
            }

            return s;
        }

        return BT_INVALID;
    }

    void Agent::btonevent(const char* btEvent)
    {
        if (this->m_currentBT)
        {
            this->m_currentBT->onevent(this, btEvent);
        }
    }

    BehaviorTreeTask* Agent::btgetcurrent()
    {
        return m_currentBT;
    }

    const BehaviorTreeTask* Agent::btgetcurrent() const
    {
        return m_currentBT;
    }

    bool Agent::btload(const char* relativePath, bool bForce)
    {
        bool bOk = Workspace::GetInstance()->Load(relativePath, bForce);

        if (bOk)
        {
            Workspace::GetInstance()->RecordBTAgentMapping(relativePath, this);
        }

        return bOk;
    }

    void Agent::btunload(const char* relativePath)
    {
        BEHAVIAC_ASSERT(behaviac::StringUtils::FindExtension(relativePath) == 0, "no extention to specify");
        BEHAVIAC_ASSERT(IsValidPath(relativePath));

        //clear the current bt if it is the current bt
        if (this->m_currentBT && this->m_currentBT->GetName() == relativePath)
        {
            const BehaviorNode* btNode = this->m_currentBT->GetNode();
            BEHAVIAC_ASSERT(BehaviorTree::DynamicCast(btNode) != 0);
            const BehaviorTree* bt = (const BehaviorTree*)btNode;
            this->btunload_pars(bt);

            this->m_currentBT = 0;
        }

        //remove it from stack
        for (BehaviorTreeStack_t::iterator it = this->m_btStack.begin(); it != this->m_btStack.end(); ++it)
        {
            BehaviorTreeStackItem_t& item = *it;

            if (item.bt->GetName() == relativePath)
            {
                this->m_btStack.erase(it);
                break;
            }
        }

        for (BehaviorTreeTasks_t::iterator iti = this->m_behaviorTreeTasks.begin(); iti != m_behaviorTreeTasks.end(); ++iti)
        {
            BehaviorTreeTask* task = *iti;

            if (task->GetName() == relativePath)
            {
                Workspace::GetInstance()->DestroyBehaviorTreeTask(task, this);

                this->m_behaviorTreeTasks.erase(iti);
                break;
            }
        }

		Workspace::GetInstance()->UnLoad(relativePath);
    }

    void Agent::bthotreloaded(const BehaviorTree* bt)
    {
        this->btunload_pars(bt);
    }

    void Agent::btunload_pars(const BehaviorTree* bt)
    {
        if (bt->m_pars)
        {
            for (BehaviorNode::Properties_t::iterator it = bt->m_pars->begin(); it != bt->m_pars->end(); ++it)
            {
                Property* property_ = *it;

                property_->UnLoad(this);
            }
        }
    }

    void Agent::btunloadall()
    {
        vector<const BehaviorTree*> bts;

        for (BehaviorTreeTasks_t::iterator it = this->m_behaviorTreeTasks.begin(); it != m_behaviorTreeTasks.end(); ++it)
        {
            BehaviorTreeTask* btTask = *it;

            const BehaviorNode* btNode = btTask->GetNode();
            BEHAVIAC_ASSERT(BehaviorTree::DynamicCast(btNode) != 0);
            const BehaviorTree* bt = (const BehaviorTree*)btNode;
            bool bFound = false;

            for (uint32_t i = 0; i < bts.size(); ++it)
            {
                const BehaviorTree* it1 = bts[i];

                if (it1 == bt)
                {
                    bFound = true;
                    break;
                }
            }

            if (!bFound)
            {
                bts.push_back(bt);
            }

            Workspace::GetInstance()->DestroyBehaviorTreeTask(btTask, this);
        }

        for (uint32_t i = 0; i < bts.size(); ++i)
        {
            const BehaviorTree* it = bts[i];

            this->btunload_pars(it);

            const behaviac::string& btName = it->GetName();

            Workspace::GetInstance()->UnLoad(btName.c_str());
        }

        this->m_behaviorTreeTasks.clear();

        this->m_currentBT = 0;
        this->m_btStack.clear();

        this->m_variables.Unload();
		this->_balckboard_bound = false;
    }

    void Agent::btreloadall()
    {
        this->m_currentBT = 0;
        this->m_btStack.clear();

        behaviac::vector<behaviac::string> bts;

        //collect the bts
        for (BehaviorTreeTasks_t::iterator it = this->m_behaviorTreeTasks.begin(); it != m_behaviorTreeTasks.end(); ++it)
        {
            BehaviorTreeTask* bt = *it;

            const behaviac::string& btName = bt->GetName();
            bool bFound = false;

            for (unsigned int i = 0; i < bts.size(); ++i)
            {
                if (bts[i] == btName)
                {
                    bFound = true;
                    break;
                }
            }

            if (!bFound)
            {
                bts.push_back(btName);
            }

            Workspace::GetInstance()->DestroyBehaviorTreeTask(bt, this);
        }

        for (unsigned int i = 0; i < bts.size(); ++i)
        {
            const behaviac::string& btName = bts[i];
            Workspace::GetInstance()->Load(btName.c_str(), true);
        }

        this->m_behaviorTreeTasks.clear();
        this->m_variables.Unload();
    }


    bool Agent::IsFired(const char* eventName)
    {
        const CNamedEvent* pEvent = this->FindEvent(eventName);

        if (pEvent)
        {
            return pEvent->IsFired();
        }

        return false;
    }

    void Agent::ResetEvent(Agent* pAgent, const char* eventName)
    {
        CStringID eventID(eventName);

        if (pAgent)
        {
            AgentEvents_t::iterator it = pAgent->m_eventInfos.find(eventID);

            if (it != pAgent->m_eventInfos.end())
            {
                CNamedEvent* pEvent = it->second;

                pEvent->SetFired(pAgent, false);

                return;
            }

			int contextId = pAgent->GetContextId();
			const CTagObjectDescriptor& meta = pAgent->GetDescriptor();
			CNamedEvent* pEvent = findNamedEventTemplate(meta.ms_methods, eventName, contextId);

			if (pEvent)
			{
				pEvent->SetFired(pAgent, false);
			}
        }
    }

    //void Agent::ResetEvent(int contextId, const char* eventName)
    //{
    //	CStringID eventID(eventName);

    //	for (AgentMetas_t::iterator it = Agent::ms_metas->begin(); it != Agent::ms_metas->end(); ++it)
    //	{
    //		const MetaInfo_t&  metaInfo = it->second;
    //		const CTagObjectDescriptor* meta = metaInfo.descriptor;
    //		CNamedEvent* pEvent = findNamedEventTemplate(meta->ms_methods, eventName, contextId);
    //		if (pEvent)
    //		{
    //			pEvent->SetFired(0, false);
    //			break;
    //		}
    //	}
    //}

    void Agent::ResetEvent(const char* eventName)
    {
        Agent::ResetEvent(this, eventName);
    }

    const CNamedEvent* Agent::FindEvent(const char* eventName, const char* className) const
    {
        if (className)
        {
            int contextId = this->GetContextId();
            const CNamedEvent* pEvent = Agent::findEventStatic(eventName, className, contextId);
            BEHAVIAC_ASSERT(!pEvent || pEvent->IsStatic());

            return pEvent;

        }
        else
        {
            CStringID eventID(eventName);
            AgentEvents_t::const_iterator it = this->m_eventInfos.find(eventID);

            if (it != this->m_eventInfos.end())
            {
                const CNamedEvent* pEvent = it->second;

                BEHAVIAC_ASSERT(!pEvent->IsStatic());

                return pEvent;
            }
        }

        return 0;
    }

    const CNamedEvent* Agent::findEventStatic(const char* eventName, const char* className, int context_id)
    {
        Context& c = Context::GetContext(context_id);

        return c.FindEventStatic(eventName, className);
    }

    void Agent::insertEventGlobal(const char* className, CNamedEvent* pEvent, int context_id)
    {
        Context& c = Context::GetContext(context_id);

        return c.InsertEventGlobal(className, pEvent);
    }

    CNamedEvent* Agent::findNamedEventTemplate(const CTagObject::MethodsContainer& methods, const char* eventName, int context_id)
    {
        Context& c = Context::GetContext(context_id);

        return c.FindNamedEventTemplate(methods, eventName);
    }

    CNamedEvent* Agent::findEvent(const char* eventName)
    {
        const CTagObjectDescriptor& meta = this->GetDescriptor();

        int contextId = this->GetContextId();
        CNamedEvent* pNamedMethod = Agent::findNamedEventTemplate(meta.ms_methods, eventName, contextId);

        if (pNamedMethod)
        {
			CNamedEvent* pEvent = 0;
			CStringID eventID(eventName);
			AgentEvents_t::iterator it = this->m_eventInfos.find(eventID);
			if (it == this->m_eventInfos.end())
			{
				pEvent = (CNamedEvent*)pNamedMethod->clone();
				this->m_eventInfos[eventID] = pEvent;
			}
			else
			{
				pEvent = it->second;
			}

            return pEvent;
        }

        return 0;
    }

	Agent* Agent::GetInstance(const Agent* pSelf, const char* agentInstanceName)
	{
		Agent* pParent = (Agent*)pSelf;

		if (agentInstanceName[0] != '\0' && strcmp(agentInstanceName, "Self") != 0)
		{
			// global
			pParent = Agent::GetInstance(agentInstanceName, pSelf ? pSelf->GetContextId() : 0);

			// member
			if (!pParent && pSelf)
			{
				pParent = pSelf->GetVariable<Agent*>(agentInstanceName);
			}

			BEHAVIAC_ASSERT(pParent);
		}

		return pParent;
	}

    Agent* Agent::GetInstance(const char* agentInstanceName, int contextId)
    {
        Context& c = Context::GetContext(contextId);

        return c.GetInstance(agentInstanceName);
    }

    bool Agent::IsInstanceNameRegistered(const char* agentInstanceName)
    {
        AgentNames_t::iterator it = Agent::Names().find(agentInstanceName);

        if (it != Agent::Names().end())
        {
            return true;
        }

        return false;
    }

    const char* Agent::GetRegisteredClassName(const char* agentInstanceName)
    {
        AgentNames_t::iterator it = Agent::Names().find(agentInstanceName);

        if (it != Agent::Names().end())
        {
            AgentName_t& agentName = it->second;

            return agentName.classFullName_.c_str();
        }

        return 0;
    }

    bool Agent::BindInstance(Agent* pAgentInstance, const char* agentInstanceName, int contextId)
    {
        Context& c = Context::GetContext(contextId);

        if (!agentInstanceName)
        {
            agentInstanceName = pAgentInstance->GetClassTypeName();
        }

        return c.BindInstance(agentInstanceName, pAgentInstance);
    }

    bool Agent::UnbindInstance(const char* agentInstanceName, int contextId)
    {
        Context& c = Context::GetContext(contextId);

        return c.UnbindInstance(agentInstanceName);
    }

    ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    //no type preceding to the namve
    //if it looks like Agent::HP, it is not a par
    bool IsParVar(const char* variableName)
    {
        const char* pSep = strchr(variableName, ':');

        if (pSep && pSep[1] == ':')
        {
            return false;
        }

        return true;
    }

    CMethodBase* Agent::CreateMethod(const CStringID& agentClassId, const CStringID& methodClassId)
    {
        AgentMetas_t::iterator it = Agent::ms_metas->find(agentClassId);

        if (it != Agent::ms_metas->end())
        {
            MetaInfo_t& m = it->second;
            const CTagObjectDescriptor* pObejctDesc = m.descriptor;

            const CTagObjectDescriptor::MethodsContainer& methods = pObejctDesc->ms_methods;

            for (CTagObjectDescriptor::MethodsContainer::const_iterator it1 = methods.begin();
                 it1 != methods.end(); ++it1)
            {
                const CMethodBase* pM = *it1;

                if (pM->GetID().GetID() == methodClassId)
                {
                    return pM->clone();
                }
            }
        }

        return 0;
    }

    const CMemberBase* Agent::FindMember(const char* property_name) const
    {
        CStringID propertyId(property_name);

        const CMemberBase* m = this->FindMember(propertyId);
        return m;
    }

    const CMemberBase* Agent::FindMember(const CStringID& propertyId) const
    {
        const CTagObjectDescriptor& obejctDesc = this->GetDescriptor();

        return obejctDesc.GetMember(propertyId);
    }

	static const size_t kNameLength = 256;
    static const char* ParsePropertyNames(const char* fullPropertnName, char* agentClassName)
    {
        //http://action.tenpay.com/2013/legua/?ADTAG=MSG.CUIFEI.LEGUA.TX_OK_PIC
        //test_ns::AgentActionTest::Property1
        const char* pBeginAgentClass = fullPropertnName;

        const char* pBeginProperty = strrchr(pBeginAgentClass, ':');

        if (pBeginProperty)
        {
            //skip '::'
            BEHAVIAC_ASSERT(pBeginProperty[0] == ':' && pBeginProperty[-1] == ':');
            pBeginProperty += 1;

			size_t pos = pBeginProperty - 2 - pBeginAgentClass;
            BEHAVIAC_ASSERT(pos < kNameLength);

            string_ncpy(agentClassName, pBeginAgentClass, pos);
            agentClassName[pos] = '\0';

            return pBeginProperty;
        }

        return 0;
    }

    const CMemberBase* Agent::FindMemberBase(const char* propertyName)
    {
        char agentClassName[kNameLength];
        const char* propertyBaseName = ParsePropertyNames(propertyName, agentClassName);

        if (propertyBaseName)
        {
            CStringID agentClassId(agentClassName);
            CStringID propertyId(propertyBaseName);

            const CMemberBase* m = FindMemberBase(agentClassId, propertyId);
            return m;
        }

        return 0;
    }

    const CMethodBase* Agent::FindMethodBase(const char* propertyName)
    {
        char agentClassName[kNameLength];
        const char* propertyBaseName = ParsePropertyNames(propertyName, agentClassName);

        if (propertyBaseName)
        {
            CStringID agentClassId(agentClassName);
            CStringID propertyId((propertyBaseName));

            const CMethodBase* m = FindMethodBase(agentClassId, propertyId);
            return m;
        }

        return 0;
    }

    const CMemberBase* Agent::FindMemberBase(const CStringID& agentClassId, const CStringID& propertyId)
    {
        if (Agent::ms_metas)
        {
            AgentMetas_t::iterator it = Agent::ms_metas->find(agentClassId);

            if (it != Agent::ms_metas->end())
            {
                //const behaviac::string& className = it->first;

                MetaInfo_t& m = it->second;
                const CTagObjectDescriptor* pObejctDesc = m.descriptor;

                return pObejctDesc->GetMember(propertyId);
            }//if (it != ms_metas->end())
        }

        return 0;
    }

    const CMethodBase* Agent::FindMethodBase(const CStringID& agentClassId, const CStringID& propertyId)
    {
        AgentMetas_t::iterator it = Agent::ms_metas->find(agentClassId);

        if (it != Agent::ms_metas->end())
        {
            //const behaviac::string& className = it->first;

            MetaInfo_t& m = it->second;
            const CTagObjectDescriptor* pObejctDesc = m.descriptor;

            for (MethodsContainer::const_iterator it1 = pObejctDesc->ms_methods.begin();
                 it1 != pObejctDesc->ms_methods.end(); ++it1)
            {
                const CMethodBase* pMethod = *it1;

                if (pMethod->GetID().GetID() == propertyId)
                {
                    //ParentType pt = pMethod->GetParentType();

                    return pMethod;
                }
            }//for
        }//if (it != ms_metas->end())

        return 0;
    }

    Property* Agent::CreateProperty(const char* typeName, const char* propertyName, const char* defaultValue)
    {
        BEHAVIAC_UNUSED_VAR(typeName);

        const CMemberBase* pMember = Agent::FindMemberBase(propertyName);

        if (pMember)
        {
            Property* pProperty = pMember->CreateProperty(defaultValue, false);

            return pProperty;
        }

        //else
        //{
        //	const CMethodBase* pMethod = 0;
        //	if (propertyName && propertyName[0] != '\0')
        //	{
        //		pMethod = Agent::FindMethodBase(propertyName);
        //	}

        //	if (pMethod)
        //	{
        //		return pMethod->CreateProperty(defaultValue, true);
        //	}
        //}

        return 0;
    }
    const CTagObjectDescriptor* Agent::GetDescriptorByName(const char* agentTypeClass)
    {
        CStringID agentTypeClassId(agentTypeClass);

        AgentMetas_t::iterator it = Agent::ms_metas->find(agentTypeClassId);

        if (it != Agent::ms_metas->end())
        {
            return it->second.descriptor;
        }

        return 0;
    }

    bool Agent::btsave(State_t& state)
    {
#if BEHAVIAC_ENABLE_PROFILING
        BEHAVIAC_PROFILE("Agent::btsave");
#endif
        state.m_agentType = this->GetObjectTypeName();
        this->m_variables.CopyTo(0, state.m_vars);

        if (this->m_currentBT)
        {
            Workspace::GetInstance()->DestroyBehaviorTreeTask(state.m_bt, this);

            const BehaviorNode* pNode = this->m_currentBT->GetNode();
            state.m_bt = (BehaviorTreeTask*)pNode->CreateAndInitTask();
            this->m_currentBT->CopyTo(state.m_bt);

            return true;
        }

        return false;
    }

    bool Agent::btload(const State_t& state)
    {
#if BEHAVIAC_ENABLE_PROFILING
        BEHAVIAC_PROFILE("Agent::btload");
#endif
        state.m_vars.CopyTo(this, this->m_variables);

        if (state.m_bt)
        {
            if (this->m_currentBT)
            {
                for (BehaviorTreeTasks_t::iterator iti = this->m_behaviorTreeTasks.begin(); iti != m_behaviorTreeTasks.end(); ++iti)
                {
                    BehaviorTreeTask* task = *iti;

                    if (task == this->m_currentBT)
                    {
                        Workspace::GetInstance()->DestroyBehaviorTreeTask(task, this);

                        this->m_behaviorTreeTasks.erase(iti);
                        break;
                    }
                }
            }

            const BehaviorNode* pNode = state.m_bt->GetNode();
            this->m_currentBT = (BehaviorTreeTask*)pNode->CreateAndInitTask();
            state.m_bt->CopyTo(this->m_currentBT);

            return true;
        }

        return false;
    }

#if !BEHAVIAC_RELEASE
    Agent::Agents_t* Agent::ms_agents = 0;
    Agent::Agents_t* Agent::Agents(bool bCleanup)
    {
        if (!bCleanup)
        {
            if (!ms_agents)
            {
                ms_agents = BEHAVIAC_NEW Agent::Agents_t;
            }

            return ms_agents;
        }

        if (ms_agents)
        {
            return ms_agents;
        }

        return 0;
    }

    Agent* Agent::GetAgent(const char* agentName)
    {
        Agent* pAgent = Agent::GetInstance(agentName, 0);

        if (pAgent)
        {
            return pAgent;
        }

        Agent::Agents_t* agents = Agents(false);
        BEHAVIAC_ASSERT(agents);
        Agent::Agents_t::iterator it = agents->find(agentName);

        if (it != agents->end())
        {
            Agent* pA = it->second;
            return pA;
        }

        return 0;
    }
#endif//BEHAVIAC_RELEASE

    int Agent::VectorLength(const IList& vector)
    {
        int n = vector.size();

        (*(IList*)&vector).release();

        return n;
    }

    void Agent::VectorAdd(IList& vector, const System::Object& element)
    {
        vector.add(element);
        vector.release();
    }

    void Agent::VectorRemove(IList& vector, const System::Object& element)
    {
        vector.remove(element);
        vector.release();
    }

    bool Agent::VectorContains(IList& vector, const System::Object& element)
    {
        bool bOk = vector.contains(element);
        vector.release();

        return bOk;
    }

    void Agent::VectorClear(IList& vector)
    {
        vector.clear();
        vector.release();
    }
}
