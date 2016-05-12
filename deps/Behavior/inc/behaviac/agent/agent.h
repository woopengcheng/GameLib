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

#ifndef BEHAVIAC_AGENT_H
#define BEHAVIAC_AGENT_H

#include "behaviac/base/base.h"

#include "behaviac/base/dynamictype.h"
#include "behaviac/base/core/factory.h"
#include "behaviac/base/object/tagobject.h"
#include "behaviac/base/object/typehandler.h"
#include "behaviac/base/core/string/stringid.h"
#include "behaviac/base/core/string/formatstring.h"
#include "behaviac/base/string/stringutils.h"
#include "behaviac/behaviortree/behaviortree.h"
#include "behaviac/behaviortree/behaviortree_task.h"
#include "behaviac/property/properties.h"
#include "behaviac/base/meta/meta.h"
#include "behaviac/base/meta/types.h"
#include "behaviac/htn/agentstate.h"
#include "behaviac/base/object/member.h"

namespace behaviac
{
    class CNamedEvent;
    class Property;
    class BehaviorTreeTask;
    class State_t;
    /*! \addtogroup Agent
     * @{
     * \addtogroup Agent
     * @{ */

    /// The Agent class is the base class to manage(load/unload/exec) behaviors.
    /*!
    Each agent belongs to a context, specified by a context id, the default context's id is 0.
    Agent should be created by Agent::Create<YourAgentClass>() and destroyed by Agent::Destory(pAgent).
    */
    class BEHAVIAC_API Agent : public CTagObject
    {
    public:
        DECLARE_BEHAVIAC_AGENT(behaviac::Agent, CTagObject);

        bool SaveDataToFile(const char* fileName);
        bool LoadDataFromFile(const char* fileName);
        bool SaveDataToFile(IFile* file);
        bool LoadDataFromFile(IFile* file);
        template <typename VariableType>
        const VariableType* GetVariableRegistry(const char* staticClassName, const CMemberBase* pMember, uint32_t variableId) const;
        //bool SaveTypeToFile(IFile* file);

        //bool LoadTypeFromFile(IFile* file);
        //bool SaveTypeToFile(const char* fileName);
        //bool LoadTypeFromFile(const char* fileName);

        bool btload(const State_t& state);
        bool btsave(State_t& state);
        void LogJumpTree(string newTree)
        {
            BEHAVIAC_UNUSED_VAR(newTree);
#if !BEHAVIAC_RELEASE
            string msg = newTree + ".xml";
            LogManager::GetInstance()->Log(this, msg.c_str(), EAR_none, ELM_jump);
#endif
        }
        void LogReturnTree(string returnFromTree)
        {
            BEHAVIAC_UNUSED_VAR(returnFromTree);
#if !BEHAVIAC_RELEASE
            string msg = returnFromTree + ".xml";
            LogManager::GetInstance()->Log(this, msg.c_str(), EAR_none, ELM_return);
#endif
        }

        /**
        @param relativePath, relativePath is relative to the workspace exported path. relativePath should not include extension.
        the file format(xml/bson) is specified by Init.
        @param bForce, the loaded BT is kept in the cache so the subsequent loading will just return it from the cache.
        if bForce is true, it will not check the cache and force to load it.

        @return
        return true if successfully loaded
        */
        bool btload(const char* relativePath, bool bForce = false);

        /**
        destory the specified bt created by this agent

        @param relativePath, is relative to the workspace epported path. relativePath should not include extension.
        */
        void btunload(const char* relativePath);

        /**
        destory all the BTs created by this agent
        */
        void btunloadall();

        /**
        called when hotreloaded

        the default implementation is unloading all pars.

        it can be overridden to do some clean up, like to reset some internal states, etc.
        */
        virtual void bthotreloaded(const BehaviorTree* bt);

        /**
        reload all the loaded behavior trees
        */
        virtual void btreloadall();

        /**
        if 'relativePath' is empty, it uses the first one

        before set the found one as the current bt,
        it aborts the current one and pushes it on the stack so that it will be continued later when the
        new one is finished.
        */
        void btsetcurrent(const char* relativePath);

        void btreferencetree(const char* relativePath);

        void bteventtree(const char* relativePath, TriggerMode triggerMode);

        /**
        reset the current bt, do nothing if there is no current bt
        */
        void btresetcurrent();
        BehaviorTreeTask* btgetcurrent();
        const BehaviorTreeTask* btgetcurrent() const;

        /**
        exec the BT specified by 'btName'. if 'btName' is null, exec the current behavior tree specified by 'btsetcurrent'.
        */
        virtual EBTStatus btexec();

        /**
        to respond to event 'btEvent' to switch to the corresonding behavior tree
        */
        void btonevent(const char* btEvent);
        ///////////////////////////////////////////////////////////////////////////////////////
        ///////////////////////////////////////////////////////////////////////////////////////
        int GetId() const
        {
            return this->m_id;
        }

        int GetPriority() const
        {
            return (int)this->m_priority;
        }

        /**
        Each agent can be assigned to an id flag by 'SetIdFlag'. A global mask can be specified by SetIdMask.
        the id flag will be checked with this global mask.

        @sa SetIdFlag SetIdMask
        */
        bool IsMasked() const
        {
            return (this->m_idFlag & Agent::IdMask()) != 0;
        }

        /**
        @sa SetIdMask IsMasked
        */
        void SetIdFlag(uint32_t idMask)
        {
            this->m_idFlag = idMask;
        }

        /**
        @sa SetIdFlag IsMasked
        */
        static void SetIdMask(uint32_t idMask);

        static uint32_t	IdMask();

        const behaviac::string& GetName() const
        {
            return this->m_name;
        }

        void SetName(const char* instanceName);

        int GetContextId() const
        {
            return this->m_context_id;
        }

        /**
        return if the agent is active or not.

        an active agent is ticked automatiacally by the world it is added.
        if it is inactive, it is not ticked automatiacally by the world it is added.

        @sa SetActive
        */
        bool IsActive() const
        {
            return this->m_bActive == 0 ? false : true;
        }

        /**
        set the agent active or inactive
        */
        void SetActive(bool bActive)
        {
            this->m_bActive = bActive ? 1 : 0;
        }
        ///////////////////////////////////////////////////////////////////////////////////////
        ///////////////////////////////////////////////////////////////////////////////////////
        static void FireEvent(Agent* pAgent, const char* eventName);

        template<class ParamType1>
        static void FireEvent(Agent* pAgent, const char* eventName, const ParamType1& param1);

        template<class ParamType1, class ParamType2>
        static void FireEvent(Agent* pAgent, const char* eventName, const ParamType1& param1, const ParamType2& param2);

        template<class ParamType1, class ParamType2, class ParamType3>
        static void FireEvent(Agent* pAgent, const char* eventName, const ParamType1& param1, const ParamType2& param2, const ParamType3& param3);

        //static void FireEvent(int contextId, const char* eventName);

        //template<class ParamType1>
        //static void FireEvent(int contextId, const char* eventName, const ParamType1& param1);

        //template<class ParamType1, class ParamType2>
        //static void FireEvent(int contextId, const char* eventName, const ParamType1& param1, const ParamType2& param2);

        //template<class ParamType1, class ParamType2, class ParamType3>
        //static void FireEvent(int contextId, const char* eventName, const ParamType1& param1, const ParamType2& param2, const ParamType3& param3);

        void FireEvent(const char* eventName);

        template<class ParamType1>
        void FireEvent(const char* eventName, const ParamType1& param1);

        template<class ParamType1, class ParamType2>
        void FireEvent(const char* eventName, const ParamType1& param1, const ParamType2& param2);

        template<class ParamType1, class ParamType2, class ParamType3>
        void FireEvent(const char* eventName, const ParamType1& param1, const ParamType2& param2, const ParamType3& param3);

        bool IsFired(const char* eventName);
        static void ResetEvent(Agent* pAgent, const char* eventName);
        //static void ResetEvent(int contextId, const char* eventName);
        void ResetEvent(const char* eventName);

        const CNamedEvent* FindEvent(const char* eventName, const char* className = 0) const;
        ///////////////////////////////////////////////////////////////////////////////////////
        ///////////////////////////////////////////////////////////////////////////////////////
        /**
        return true if a variable named 'variableName'exists
        */
        BEHAVIAC_FORCEINLINE bool IsVariableExisting(const char* variableName) const;

        /**
        get a variable by its name

        it is invalid to call GetVariable<Type>("par_name") before exec the BT or SetVariableGetVariable<Type>("par_name")
        as "par_name" would not had been created then
        */
        template<typename VariableType>
        BEHAVIAC_FORCEINLINE const VariableType& GetVariable(const char* variableName) const;

        template<typename VariableType>
        BEHAVIAC_FORCEINLINE const VariableType& GetVariable(uint32_t variableId) const;

        /**
        set a variable by its name, its type(agent/par/singleton) is determined by the name
        */
        template<typename VariableType>
        BEHAVIAC_FORCEINLINE void SetVariable(const char* variableName, const VariableType& value);

        template<typename VariableType>
        BEHAVIAC_FORCEINLINE void SetVariable(const char* variableName, const VariableType& value, uint32_t variableId);

        BEHAVIAC_FORCEINLINE void SetVariableFromString(const char* variableName, const char* valueStr);

        /**
        if staticClassName is no null, it is for static variable
        */
        template<typename VariableType>
        void SetVariableRegistry(bool bLocal, const CMemberBase* pMember, const char* variableName, const VariableType& value, const char* staticClassName, uint32_t varableId);

        /**
        instantiate a variable by its name, its type(agent/par/singleton) is determined by the name

        if that variable with that name has already been existed, it does nothing.
        */
        template<typename VariableType>
        BEHAVIAC_FORCEINLINE void Instantiate(const VariableType& value, const Property* property_);

        /**
        uninstantiate a variable by its name
        */
        template<typename VariableType>
        BEHAVIAC_FORCEINLINE void UnInstantiate(const char* variableName);

        /**
        unload a variable by its name
        */
        template<typename VariableType>
        void UnLoad(const char* variableName);

        /**
        log changed variables(propery and par)
        */
        void LogVariables(bool bForce);

        static bool Invoke(Agent* pAgent, const char* methodName);

        template <typename P1>
        static bool Invoke(Agent* pAgent, const char* methodName, P1 p1);

        template <typename P1, typename P2>
        static bool Invoke(Agent* pAgent, const char* methodName, P1 p1, P2 p2);

        template <typename P1, typename P2, typename P3>
        static bool Invoke(Agent* pAgent, const char* methodName, P1 p1, P2 p2, P3 p3);

        /**
        you first call 'Invoke', if it returns true, then you can call 'GetInvokeReturn' to retrieve the return value if any
        */
        template <typename R>
        static bool GetInvokeReturn(Agent* pAgent, const char* methodName, R& returnValue);
        ///////////////////////////////////////////////////////////////////////////////////////
        ///////////////////////////////////////////////////////////////////////////////////////
        ///////////////////////////////////////////////////////////////////////////////////////
        ///////////////////////////////////////////////////////////////////////////////////////
        /**
        register agent derived class

        only registered agent derived class can be created/managed correctly.

        @param bSingleton
        indicating a singleton class, which is allowed to create only one instance, like the world, player, etc.
        */
        template<typename TAGENT>
        static bool Register();

        template<typename TAGENT>
        static void UnRegister();

        static bool IsRegistered(const char* agentClassName);

        template<typename TAGENT>
        static bool IsRegistered();

        /**
        return true if 'agentClassName' is an agent class name or agent derived class name

        IsAgentClassName is different from IsRegistered that IsRegistered only returns true for those classes directly registered by 'Register'
        IsAgentClassName also returns true for those base classes.
        */
        static bool IsAgentClassName(const CStringID& agentClassId);
        static bool IsAgentClassName(const char* agentClassName);
        ///////////////////////////////////////////////////////////////////////////////////////
        ///////////////////////////////////////////////////////////////////////////////////////
        /**
        An instance name can be bound to an instance. before an instance name is bound to an instance,
        that instance name has to be registered by 'RegisterInstanceName'

        @param agentInstanceName
        the specified name to be used to access an instance of type 'TAGENT' or its derivative.
        if 'agentInstanceName' is 0, the class name of 'TAGENT' will be used to be registered.

        @sa Create
        */
        template<typename TAGENT>
        static bool RegisterInstanceName(const char* agentInstanceName = 0, const wchar_t* displayName = 0, const wchar_t* desc = 0);

        template<typename TAGENT>
        static void UnRegisterInstanceName(const char* agentInstanceName = 0);

        /**
        return if 'agentInstanceName' is registerd.

        @sa RegisterInstanceName
        */
        static bool IsInstanceNameRegistered(const char* agentInstanceName);

        /**
        return the registered class name

        @sa RegisterInstanceName
        */
        static const char* GetRegisteredClassName(const char* agentInstanceName);

        /**
        create an Agent with type 'TAGENT', with priority and agentInstanceName as name,
        if agentInstanceName is 0, the TAGENT class name is used

        higher priority are ticked earlier

        @contextId
        by default, it is 0

        @sa Destroy
        */
        template<typename TAGENT>
        static TAGENT* Create(const char* agentInstanceName = 0, int contextId = 0, short priority = 0);

		template<typename TAGENT, typename T1>
        static TAGENT* Create(T1 p1, const char* agentInstanceName, int contextId, short priority);

		template<typename TAGENT, typename T1, typename T2>
		static TAGENT* Create(T1 p1, T2 p2, const char* agentInstanceName, int contextId, short priority);

		template<typename TAGENT, typename T1, typename T2, typename T3>
		static TAGENT* Create(T1 p1, T2 p2, T3 p3, const char* agentInstanceName, int contextId, short priority);

		template<typename TAGENT, typename T1, typename T2, typename T3, typename T4>
		static TAGENT* Create(T1 p1, T2 p2, T3 p3, T4 p4, const char* agentInstanceName, int contextId, short priority);

        /**
        destroy the agent created by 'Create'

        @sa Create
        */
        static void Destroy(Agent* pAgent);

#if !BEHAVIAC_RELEASE
        static Agent* GetAgent(const char* agentName);
#endif//BEHAVIAC_RELEASE

        /**
        bind 'agentInstanceName' to 'pAgentInstance'.
        'agentInstanceName' should have been registered to the class of 'pAgentInstance' or its parent class.

        if 'agentInstanceName' is not specified, the class type name of 'pAgentInstance' will be used.

        @sa RegisterInstanceName
        */
        static bool BindInstance(Agent* pAgentInstance, const char* agentInstanceName = 0, int contextId = 0);

        /**
        unbind 'agentInstanceName' from 'pAgentInstance'.
        'agentInstanceName' should have been bound to 'pAgentInstance'.

        @sa RegisterInstanceName, BindInstance, Create
        */
        static bool UnbindInstance(const char* agentInstanceName, int contextId = 0);

        template<typename TAGENT>
        static bool UnbindInstance();

		static Agent* GetInstance(const Agent* pSelf, const char* agentInstanceName);

        static Agent* GetInstance(const char* agentInstanceName, int contextId);

        template<typename TAGENT>
        static TAGENT* GetInstance(const char* agentInstanceName = 0, int contextId = 0);

        static const CMemberBase* FindMemberBase(const char* propertyName);
        static const CMethodBase* FindMethodBase(const char* propertyName);

        static const CMemberBase* FindMemberBase(const CStringID& agentClassId, const CStringID& propertyId);
        static const CMethodBase* FindMethodBase(const CStringID& agentClassId, const CStringID& propertyId);

        static Property* CreateProperty(const char* typeName, const char* propertyName, const char* defaultValue);
        static const CTagObjectDescriptor* GetDescriptorByName(const char* agentTypeClass);

        ///////////////////////////////////////////////////////////////////////////////////////
        ///////////////////////////////////////////////////////////////////////////////////////
        static CMethodBase* CreateMethod(const CStringID& agentClassId, const CStringID& methodClassId);
        ///////////////////////////////////////////////////////////////////////////////////////
        ///////////////////////////////////////////////////////////////////////////////////////

        /**
        find the property's meta
        */
        const CMemberBase* FindMember(const char* property_name) const;
        const CMemberBase* FindMember(const CStringID& propertyId) const;
#if BEHAVIAC_ENABLE_NETWORKD
        void ReplicateProperties();
#endif//#if BEHAVIAC_ENABLE_NETWORKD

        static int VectorLength(const IList& vector);
        static void VectorAdd(IList& vector, const System::Object& element);
        static void VectorRemove(IList& vector, const System::Object& element);
        static bool VectorContains(IList& vector, const System::Object& element);
        static void VectorClear(IList& vector);

    protected:
        Agent();
        virtual ~Agent();

		void destroy_();

        int	m_context_id;

    private:
        //to stop a class from being able to be copied, either via copy constructor or assignment.
        Agent(const Agent&);
        Agent& operator=(const Agent&);

		template<typename TAGENT>
		static TAGENT* GetAgentInstance(const char* agentInstanceName, int contextId, bool& bToBind);

		static void InitAgent(Agent* pAgent, const char* agentInstanceName, const char* agentInstanceNameAny, bool bToBind, int contextId, short priority);

        static CFactory<Agent>* ms_factory;
        static CFactory<Agent>& Factory();

        static const int kAGENT_DEBUG_VERY = 0x01010101;
        void InstantiateProperties();

        /**
        create an Agent with type 'agentClassName', with priority and agentInstanceName as name

        higher priority are ticked earlier
        */
        static void Init_(int contextId, Agent* pAgent, short priority, const char* agentInstanceName);

        EBTStatus btexec_();

        void _btsetcurrent(const char* relativePath, TriggerMode triggerMode = TM_Transfer, bool bByEvent = false);

        void btunload_pars(const BehaviorTree* bt);

#if BEHAVIAC_ENABLE_NETWORKD
        void SubsribeToNetwork();
        void UnSubsribeToNetwork();
#endif//#if BEHAVIAC_ENABLE_NETWORKD
        void InitVariableRegistry();
        void UpdateVariableRegistry();

        /**
        reset changed variables(propery and par)
        */
        void ResetChangedVariables();

        CNamedEvent* findEvent(const char* eventName);

        //access ExportMetas and Cleanup
        friend class Workspace;
        static bool ExportMetas(const char* xmlMetaFilePath);
        static void Cleanup();

        static const CNamedEvent* findEventStatic(const char* eventName, const char* className, int context_id);
        static void insertEventGlobal(const char* className, CNamedEvent* pEvent, int context_id);
        static CNamedEvent* findNamedEventTemplate(const CTagObject::MethodsContainer& methods, const char* eventName, int context_id);

        template<typename TAGENT>
        static void RegisterTypeToMetas(bool bInternal);

        template<typename TAGENT, bool bAgent>
        struct AgentSuperRegister
        {
            static bool Register()
            {
                return false;
            }
        };

        template<typename TAGENT>
        struct AgentSuperRegister < TAGENT, true >
        {
            static bool Register()
            {
                Agent::RegisterTypeToMetas<TAGENT>(true);

                return true;
            }
        };

#if !BEHAVIAC_RELEASE
        typedef behaviac::map<behaviac::string, Agent*> Agents_t;
        static Agents_t* ms_agents;
        static Agents_t* Agents(bool bCleanup);
#endif//BEHAVIAC_RELEASE

        struct MetaInfo_t
        {
            const char*						classFullName;
            const char*						baseClassFullName;
            const CTagObjectDescriptor*		descriptor;
            bool							bInternal;

            MetaInfo_t() : classFullName(0), baseClassFullName(0), descriptor(0), bInternal(false)
            {}

            MetaInfo_t(const CTagObjectDescriptor* d, const char* typeFullName, const char* baseTypeFullName, bool i) :
                classFullName(typeFullName), baseClassFullName(baseTypeFullName), descriptor(d), bInternal(i)
            {}
        };

        struct MetaComparator
        {
            bool operator()(const Agent::MetaInfo_t* _left, const Agent::MetaInfo_t* _right);
        };

        typedef behaviac::map<CStringID, MetaInfo_t> AgentMetas_t;

        static AgentMetas_t*			ms_metas;
        static AgentMetas_t& Metas();

        struct AgentName_t
        {
            behaviac::string			instantceName_;
            behaviac::string			classFullName_;
            behaviac::wstring			displayName_;
            behaviac::wstring			desc_;

            AgentName_t()
            {}

            AgentName_t(const char* instanceName, const char* classFullName,
                        const wchar_t* displayName, const wchar_t* desc) : instantceName_(instanceName), classFullName_(classFullName)
            {
                if (displayName)
                {
                    displayName_ = displayName;

                }
                else
                {
                    displayName_ = StringUtils::Char2Wide(instantceName_);
                }

                if (desc)
                {
                    desc_ = desc;

                }
                else
                {
                    desc_ = displayName_;
                }
            }
        };

        struct NameComparator
        {
            bool operator()(const Agent::AgentName_t* _left, const Agent::AgentName_t* _right);
        };

        typedef behaviac::map<behaviac::string, AgentName_t> AgentNames_t;

        static AgentNames_t*			ms_names;
        static AgentNames_t& Names();

        typedef behaviac::map<CStringID, CNamedEvent*> AgentEvents_t;

        static int					ms_agent_index;

        typedef behaviac::map<behaviac::string, uint32_t> AgentTypeIndexMap_t;
        static AgentTypeIndexMap_t*	ms_agent_type_index;
        ///////////////////////////////////////////////////////////////////////////////////////
        AgentEvents_t				m_eventInfos;

        struct BehaviorTreeStackItem_t
        {
            BehaviorTreeTask*	bt;
            TriggerMode			triggerMode;
            bool				triggerByEvent;

            BehaviorTreeStackItem_t(BehaviorTreeTask* bt_, TriggerMode tm, bool bByEvent) : bt(bt_), triggerMode(tm), triggerByEvent(bByEvent)
            {
            }
        };

        typedef behaviac::vector<BehaviorTreeStackItem_t> BehaviorTreeStack_t;
        typedef behaviac::vector<BehaviorTreeTask*> BehaviorTreeTasks_t;

        BehaviorTreeTasks_t			m_behaviorTreeTasks;

        BehaviorTreeStack_t			m_btStack;
        BehaviorTreeTask*			m_currentBT;

        int							m_id;
        short						m_priority;
        char						m_bActive;
        bool						m_referencetree;
        behaviac::string			m_name;

//this condition compiling might cause the lib and app different
#if !BEHAVIAC_RELEASE
        int							m_debug_verify;
public:
        int							m_debug_count;
private:
#endif//

        bool						_balckboard_bound;
        //debug
    public:
        AgentState					m_variables;
        uint32_t					m_idFlag;
        int                         m_planningTop;
        static uint32_t				ms_idMask;
    };
    /*! @} */
    /*! @} */
}

#if BEHAVIAC_ENABLE_NETWORKD
#include "behaviac/network/network.h"

namespace behaviac
{
    template<typename T>
    class Any_t<T, true, true> : public IAny
    {
    public:
        typedef REAL_BASETYPE(T)		BaseType;

        Any_t() : IAny(GetClassTypeNumberId<BaseType>())
        {}

        Any_t(BaseType* d) : IAny(GetClassTypeNumberId<BaseType>()), data(d)
        {
        }

        virtual void* GetData() const
        {
            return (void*)this->data;
        }

        void SetValue(BaseType* v)
        {
            data = v;
        }

    protected:
        BaseType*	data;
    };
}//namespace behaviac
#endif//#if BEHAVIAC_ENABLE_NETWORKD

//after agent
#include "behaviac/base/custommethod.h"
#include "behaviac/agent/context.h"
#include "behaviac/htn/agentproperties.h"
#include "behaviac/property/vector_ext.h"
#include "agent.inl"

#include "behaviac/base/object/method.h"

template<class ObjectType>
class CGenericMethod<behaviac::EBTStatus, ObjectType> : public CGenericMethod_R<behaviac::EBTStatus, ObjectType>
{
public:
    CGenericMethod(behaviac::EBTStatus(ObjectType::*methodPtr)(void),
                   const char* className, const char* propertyName) : CGenericMethod_R<behaviac::EBTStatus, ObjectType>(methodPtr, className, propertyName)
    {}
    CGenericMethod(behaviac::EBTStatus(ObjectType::*methodPtr)(void) const,
                   const char* className, const char* propertyName) : CGenericMethod_R<behaviac::EBTStatus, ObjectType>(methodPtr, className, propertyName)
    {}

    virtual ~CGenericMethod()
    {
    }

    virtual CMethodBase* clone() const
    {
        return BEHAVIAC_NEW CGenericMethod(*this);
    }

    virtual int vRun(const CTagObject* parent, behaviac::IAsyncValue& returnResult)
    {
        BEHAVIAC_UNUSED_VAR(returnResult);
        int result = (int)(((ObjectType*)parent)->*this->m_methodPtr)();

        return result;
    }
};

template<class ObjectType, class ParamType>
class CGenericMethod1<behaviac::EBTStatus, ObjectType, ParamType> : public CGenericMethod1_R<behaviac::EBTStatus, ObjectType, ParamType>
{
public:
    typedef typename CGenericMethod1_R<behaviac::EBTStatus, ObjectType, ParamType>::ParamBaseType ParamBaseType;
    CGenericMethod1(behaviac::EBTStatus(ObjectType::*methodPtr)(ParamType),
                    const char* className, const char* propertyName) : CGenericMethod1_R<behaviac::EBTStatus, ObjectType, ParamType>(methodPtr, className, propertyName)
    {}

    CGenericMethod1(behaviac::EBTStatus(ObjectType::*methodPtr)(ParamType) const,
                    const char* className, const char* propertyName) : CGenericMethod1_R<behaviac::EBTStatus, ObjectType, ParamType>(methodPtr, className, propertyName)
    {}

    virtual ~CGenericMethod1()
    {
    }

    virtual CMethodBase* clone() const
    {
        return BEHAVIAC_NEW CGenericMethod1(*this);
    }

    virtual int vRun(const CTagObject* parent, behaviac::IAsyncValue& returnResult)
    {
        const ParamBaseType& returnValue = ((behaviac::AsyncValue<ParamType>*)&returnResult)->get();

        int result = (int)(((ObjectType*)parent)->*this->m_methodPtr)((PARAM_CALLEDTYPE(ParamType))returnValue);

        return result;
    }
};

template<>
class CGenericMethodStatic<behaviac::EBTStatus> : public CGenericMethodStatic_R<behaviac::EBTStatus>
{
public:
    CGenericMethodStatic(behaviac::EBTStatus(*methodPtr)(void), const char* className, const char* propertyName) : CGenericMethodStatic_R<behaviac::EBTStatus>(methodPtr, className, propertyName)
    {}

    virtual CMethodBase* clone() const
    {
        return BEHAVIAC_NEW CGenericMethodStatic(*this);
    }

    virtual int vRun(const CTagObject* parent, behaviac::IAsyncValue& returnResult)
    {
        BEHAVIAC_UNUSED_VAR(parent);
        BEHAVIAC_UNUSED_VAR(returnResult);
        behaviac::EBTStatus result = (*this->m_methodPtr)();

        return (int)result;
    }
};

template<class ParamType>
class CGenericMethodStatic1<behaviac::EBTStatus, ParamType> : public CGenericMethodStatic1_R<behaviac::EBTStatus, ParamType>
{
public:
    typedef typename CGenericMethodStatic1_R<behaviac::EBTStatus, ParamType>::ParamBaseType ParamBaseType;

    CGenericMethodStatic1(behaviac::EBTStatus(*methodPtr)(ParamType), const char* className, const char* propertyName) :
        CGenericMethodStatic1_R<behaviac::EBTStatus, ParamType>(methodPtr, className, propertyName)
    {}

    virtual CMethodBase* clone() const
    {
        return BEHAVIAC_NEW CGenericMethodStatic1(*this);
    }

    virtual int vRun(const CTagObject* parent, behaviac::IAsyncValue& returnResult)
    {
        BEHAVIAC_UNUSED_VAR(parent);
        const ParamBaseType& returnValue = ((behaviac::AsyncValue<ParamType>*)&returnResult)->get();

        behaviac::EBTStatus result = ((*this->m_methodPtr)((PARAM_CALLEDTYPE(ParamType))returnValue));

        return (int)result;
    }
};

//namespace StringUtils
//{
//	namespace Private
//	{
//		template<>
//		inline bool FromString(const char* str, behaviac::Agent*& val)
//		{
//			BEHAVIAC_ASSERT(string_nicmp(str, "null", 4) == 0);
//			val = 0;
//			return true;
//		}
//	}
//}

BEHAVIAC_DECLARE_TYPE_VECTOR_HANDLER(behaviac::Agent*);

#endif//#ifndef BEHAVIAC_AGENT_H
