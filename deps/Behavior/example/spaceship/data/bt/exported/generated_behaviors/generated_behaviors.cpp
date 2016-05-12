// ---------------------------------------------------------------------
/* This file is auto-generated, so please don't modify it by yourself!
Usage: include it in a certain cpp accordingly(RELATIVE_PATH is the path where it is generated):

      #include "RELATIVE_PATH/generated_behaviors.cpp"
*/

// Export file: bt/exported/generated_behaviors/generated_behaviors.cpp
// ---------------------------------------------------------------------

// You should set the include path of the behaviac lib in your project
// for using the following header files :
#include "behaviac/behaviortree/behaviortree.h"
#include "behaviac/agent/taskmethod.h"

#include "behaviac/behaviortree/nodes/actions/action.h"
#include "behaviac/behaviortree/nodes/actions/assignment.h"
#include "behaviac/behaviortree/nodes/actions/compute.h"
#include "behaviac/behaviortree/nodes/actions/noop.h"
#include "behaviac/behaviortree/nodes/actions/wait.h"
#include "behaviac/behaviortree/nodes/actions/waitforsignal.h"
#include "behaviac/behaviortree/nodes/actions/waitframes.h"
#include "behaviac/behaviortree/nodes/composites/compositestochastic.h"
#include "behaviac/behaviortree/nodes/composites/ifelse.h"
#include "behaviac/behaviortree/nodes/composites/parallel.h"
#include "behaviac/behaviortree/nodes/composites/query.h"
#include "behaviac/behaviortree/nodes/composites/referencebehavior.h"
#include "behaviac/behaviortree/nodes/composites/selector.h"
#include "behaviac/behaviortree/nodes/composites/selectorloop.h"
#include "behaviac/behaviortree/nodes/composites/selectorprobability.h"
#include "behaviac/behaviortree/nodes/composites/selectorstochastic.h"
#include "behaviac/behaviortree/nodes/composites/sequence.h"
#include "behaviac/behaviortree/nodes/composites/sequencestochastic.h"
#include "behaviac/behaviortree/nodes/composites/withprecondition.h"
#include "behaviac/behaviortree/nodes/conditions/and.h"
#include "behaviac/behaviortree/nodes/conditions/conditionbase.h"
#include "behaviac/behaviortree/nodes/conditions/condition.h"
#include "behaviac/behaviortree/nodes/conditions/false.h"
#include "behaviac/behaviortree/nodes/conditions/or.h"
#include "behaviac/behaviortree/nodes/conditions/true.h"
#include "behaviac/behaviortree/nodes/decorators/decoratoralwaysfailure.h"
#include "behaviac/behaviortree/nodes/decorators/decoratoralwaysrunning.h"
#include "behaviac/behaviortree/nodes/decorators/decoratoralwayssuccess.h"
#include "behaviac/behaviortree/nodes/decorators/decoratorcount.h"
#include "behaviac/behaviortree/nodes/decorators/decoratorcountlimit.h"
#include "behaviac/behaviortree/nodes/decorators/decoratorfailureuntil.h"
#include "behaviac/behaviortree/nodes/decorators/decoratorframes.h"
#include "behaviac/behaviortree/nodes/decorators/decoratoriterator.h"
#include "behaviac/behaviortree/nodes/decorators/decoratorlog.h"
#include "behaviac/behaviortree/nodes/decorators/decoratorloop.h"
#include "behaviac/behaviortree/nodes/decorators/decoratorloopuntil.h"
#include "behaviac/behaviortree/nodes/decorators/decoratornot.h"
#include "behaviac/behaviortree/nodes/decorators/decoratorrepeat.h"
#include "behaviac/behaviortree/nodes/decorators/decoratorsuccessuntil.h"
#include "behaviac/behaviortree/nodes/decorators/decoratortime.h"
#include "behaviac/behaviortree/nodes/decorators/decoratorweight.h"
#include "behaviac/behaviortree/attachments/event.h"
#include "behaviac/behaviortree/attachments/attachaction.h"
#include "behaviac/behaviortree/attachments/precondition.h"
#include "behaviac/behaviortree/attachments/effector.h"
#include "behaviac/htn/task.h"
#include "behaviac/fsm/state.h"
#include "behaviac/fsm/startcondition.h"
#include "behaviac/fsm/transitioncondition.h"

// You should set the agent header files of your game
// when exporting cpp files in the behaviac editor:

using namespace behaviac;

// Agent property and method handlers

namespace behaviac
{
	struct METHOD_TYPE_behaviac_Agent_VectorAdd { };
	template<> BEHAVIAC_FORCEINLINE void Agent::_Execute_Method_<METHOD_TYPE_behaviac_Agent_VectorAdd>(IList& p0, System::Object& p1)
	{
		this->behaviac::Agent::VectorAdd(p0, p1);
	}

	struct METHOD_TYPE_behaviac_Agent_VectorClear { };
	template<> BEHAVIAC_FORCEINLINE void Agent::_Execute_Method_<METHOD_TYPE_behaviac_Agent_VectorClear>(IList& p0)
	{
		this->behaviac::Agent::VectorClear(p0);
	}

	struct METHOD_TYPE_behaviac_Agent_VectorContains { };
	template<> BEHAVIAC_FORCEINLINE bool Agent::_Execute_Method_<METHOD_TYPE_behaviac_Agent_VectorContains>(IList& p0, System::Object& p1)
	{
		return this->behaviac::Agent::VectorContains(p0, p1);
	}

	struct METHOD_TYPE_behaviac_Agent_VectorLength { };
	template<> BEHAVIAC_FORCEINLINE int Agent::_Execute_Method_<METHOD_TYPE_behaviac_Agent_VectorLength>(IList& p0)
	{
		return this->behaviac::Agent::VectorLength(p0);
	}

	struct METHOD_TYPE_behaviac_Agent_VectorRemove { };
	template<> BEHAVIAC_FORCEINLINE void Agent::_Execute_Method_<METHOD_TYPE_behaviac_Agent_VectorRemove>(IList& p0, System::Object& p1)
	{
		this->behaviac::Agent::VectorRemove(p0, p1);
	}

}

namespace framework
{
	struct PROPERTY_TYPE_framework_GameObject_age { };
	template<> BEHAVIAC_FORCEINLINE long& GameObject::_Get_Property_<PROPERTY_TYPE_framework_GameObject_age>()
	{
		unsigned char* pc = (unsigned char*)this;
		pc += (int)offsetof(framework::GameObject, framework::GameObject::age);
		return *(reinterpret_cast<long*>(pc));
	}

	struct PROPERTY_TYPE_framework_GameObject_HP { };
	template<> BEHAVIAC_FORCEINLINE unsigned int& GameObject::_Get_Property_<PROPERTY_TYPE_framework_GameObject_HP>()
	{
		unsigned char* pc = (unsigned char*)this;
		pc += (int)offsetof(framework::GameObject, framework::GameObject::HP);
		return *(reinterpret_cast<unsigned int*>(pc));
	}

	struct METHOD_TYPE_framework_GameObject_alignedWithPlayer { };
	template<> BEHAVIAC_FORCEINLINE bool GameObject::_Execute_Method_<METHOD_TYPE_framework_GameObject_alignedWithPlayer>()
	{
		return this->framework::GameObject::alignedWithPlayer();
	}

	struct METHOD_TYPE_framework_GameObject_distanceToPlayer { };
	template<> BEHAVIAC_FORCEINLINE float GameObject::_Execute_Method_<METHOD_TYPE_framework_GameObject_distanceToPlayer>()
	{
		return this->framework::GameObject::distanceToPlayer();
	}

	struct METHOD_TYPE_framework_GameObject_GoStraight { };
	template<> BEHAVIAC_FORCEINLINE void GameObject::_Execute_Method_<METHOD_TYPE_framework_GameObject_GoStraight>(int& p0)
	{
		this->framework::GameObject::GoStraight(p0);
	}

	struct METHOD_TYPE_framework_GameObject_playerIsAligned { };
	template<> BEHAVIAC_FORCEINLINE bool GameObject::_Execute_Method_<METHOD_TYPE_framework_GameObject_playerIsAligned>()
	{
		return this->framework::GameObject::playerIsAligned();
	}

	struct METHOD_TYPE_framework_GameObject_projectileNearby { };
	template<> BEHAVIAC_FORCEINLINE bool GameObject::_Execute_Method_<METHOD_TYPE_framework_GameObject_projectileNearby>(float& p0)
	{
		return this->framework::GameObject::projectileNearby(p0);
	}

	struct METHOD_TYPE_framework_GameObject_TurnTowardsTarget { };
	template<> BEHAVIAC_FORCEINLINE int GameObject::_Execute_Method_<METHOD_TYPE_framework_GameObject_TurnTowardsTarget>(float& p0)
	{
		return this->framework::GameObject::TurnTowardsTarget(p0);
	}

}

namespace framework
{
	struct METHOD_TYPE_framework_Ship_checkresult { };
	template<> BEHAVIAC_FORCEINLINE behaviac::EBTStatus Ship::_Execute_Method_<METHOD_TYPE_framework_Ship_checkresult>(bool& p0)
	{
		return this->framework::Ship::checkresult(p0);
	}

	struct METHOD_TYPE_framework_Ship_DestroyAllNearbyProjectiles { };
	template<> BEHAVIAC_FORCEINLINE void Ship::_Execute_Method_<METHOD_TYPE_framework_Ship_DestroyAllNearbyProjectiles>(float& p0)
	{
		this->framework::Ship::DestroyAllNearbyProjectiles(p0);
	}

	struct METHOD_TYPE_framework_Ship_Fire { };
	template<> BEHAVIAC_FORCEINLINE bool Ship::_Execute_Method_<METHOD_TYPE_framework_Ship_Fire>()
	{
		return this->framework::Ship::Fire();
	}

	struct METHOD_TYPE_framework_Ship_getXPosition { };
	template<> BEHAVIAC_FORCEINLINE float Ship::_Execute_Method_<METHOD_TYPE_framework_Ship_getXPosition>()
	{
		return this->framework::Ship::getXPosition();
	}

	struct METHOD_TYPE_framework_Ship_getYPosition { };
	template<> BEHAVIAC_FORCEINLINE float Ship::_Execute_Method_<METHOD_TYPE_framework_Ship_getYPosition>()
	{
		return this->framework::Ship::getYPosition();
	}

	struct METHOD_TYPE_framework_Ship_GotoPoint { };
	template<> BEHAVIAC_FORCEINLINE bool Ship::_Execute_Method_<METHOD_TYPE_framework_Ship_GotoPoint>(float& p0, float& p1, float& p2)
	{
		return this->framework::Ship::GotoPoint(p0, p1, p2);
	}

	struct METHOD_TYPE_framework_Ship_GotoPointRelative { };
	template<> BEHAVIAC_FORCEINLINE bool Ship::_Execute_Method_<METHOD_TYPE_framework_Ship_GotoPointRelative>(float& p0, float& p1, float& p2)
	{
		return this->framework::Ship::GotoPointRelative(p0, p1, p2);
	}

}

namespace framework
{
	struct PROPERTY_TYPE_framework_WorldState_HealthHP { };
	template<> BEHAVIAC_FORCEINLINE unsigned int& WorldState::_Get_Property_<PROPERTY_TYPE_framework_WorldState_HealthHP>()
	{
		unsigned char* pc = (unsigned char*)this;
		pc += (int)offsetof(framework::WorldState, framework::WorldState::HealthHP);
		return *(reinterpret_cast<unsigned int*>(pc));
	}

	struct PROPERTY_TYPE_framework_WorldState_SyncSignal { };
	template<> BEHAVIAC_FORCEINLINE bool& WorldState::_Get_Property_<PROPERTY_TYPE_framework_WorldState_SyncSignal>()
	{
		unsigned char* pc = (unsigned char*)this;
		pc += (int)offsetof(framework::WorldState, framework::WorldState::SyncSignal);
		return *(reinterpret_cast<bool*>(pc));
	}

	struct PROPERTY_TYPE_framework_WorldState_time { };
	template<> BEHAVIAC_FORCEINLINE unsigned int& WorldState::_Get_Property_<PROPERTY_TYPE_framework_WorldState_time>()
	{
		unsigned char* pc = (unsigned char*)this;
		pc += (int)offsetof(framework::WorldState, framework::WorldState::time);
		return *(reinterpret_cast<unsigned int*>(pc));
	}

	struct METHOD_TYPE_framework_WorldState_NextWave { };
	template<> BEHAVIAC_FORCEINLINE bool WorldState::_Execute_Method_<METHOD_TYPE_framework_WorldState_NextWave>()
	{
		return this->framework::WorldState::NextWave();
	}

	struct METHOD_TYPE_framework_WorldState_PlayerHP { };
	template<> BEHAVIAC_FORCEINLINE int WorldState::_Execute_Method_<METHOD_TYPE_framework_WorldState_PlayerHP>()
	{
		return this->framework::WorldState::PlayerHP();
	}

	struct METHOD_TYPE_framework_WorldState_SpawnShip { };
	template<> BEHAVIAC_FORCEINLINE void WorldState::_Execute_Method_<METHOD_TYPE_framework_WorldState_SpawnShip>(int& p0)
	{
		this->framework::WorldState::SpawnShip(p0);
	}

}

namespace behaviac
{
	// Source file: base/boring

	class BEHAVIAC_API Action_bt_base_boring_node1 : public Action
	{
	public:
		BEHAVIAC_DECLARE_DYNAMIC_TYPE(Action_bt_base_boring_node1, Action);
		Action_bt_base_boring_node1()
		{
			method_p0 = 10;
		}
	protected:
		virtual EBTStatus update_impl(Agent* pAgent, EBTStatus childStatus)
		{
			BEHAVIAC_UNUSED_VAR(pAgent);
			BEHAVIAC_UNUSED_VAR(childStatus);
			((framework::GameObject*)pAgent)->_Execute_Method_<framework::METHOD_TYPE_framework_GameObject_GoStraight, void, int >(method_p0);
			return BT_RUNNING;
		}
		int method_p0;
	};

	BEHAVIAC_API bool Create_bt_base_boring(BehaviorTree* pBT)
	{
		pBT->SetClassNameString("BehaviorTree");
		pBT->SetId(-1);
		pBT->SetName("base/boring");
		pBT->SetIsFSM(false);
#if !defined(BEHAVIAC_RELEASE)
		pBT->SetAgentType("framework::GameObject");
#endif
		// children
		{
			Action_bt_base_boring_node1* node1 = BEHAVIAC_NEW Action_bt_base_boring_node1;
			node1->SetClassNameString("Action");
			node1->SetId(1);
#if !defined(BEHAVIAC_RELEASE)
			node1->SetAgentType("framework::GameObject");
#endif
			pBT->AddChild(node1);
			pBT->SetHasEvents(pBT->HasEvents() | node1->HasEvents());
		}
		return true;
	}

	// Source file: base/homing

	class BEHAVIAC_API Parallel_bt_base_homing_node1 : public Parallel
	{
	public:
		BEHAVIAC_DECLARE_DYNAMIC_TYPE(Parallel_bt_base_homing_node1, Parallel);
		Parallel_bt_base_homing_node1()
		{
			m_failPolicy = FAIL_ON_ONE;
			m_succeedPolicy = SUCCEED_ON_ALL;
			m_exitPolicy = EXIT_NONE;
			m_childFinishPolicy = CHILDFINISH_LOOP;
		}
	protected:
	};

	class BEHAVIAC_API Action_bt_base_homing_node2 : public Action
	{
	public:
		BEHAVIAC_DECLARE_DYNAMIC_TYPE(Action_bt_base_homing_node2, Action);
		Action_bt_base_homing_node2()
		{
			method_p0 = 2;
		}
	protected:
		virtual EBTStatus update_impl(Agent* pAgent, EBTStatus childStatus)
		{
			BEHAVIAC_UNUSED_VAR(pAgent);
			BEHAVIAC_UNUSED_VAR(childStatus);
			((framework::GameObject*)pAgent)->_Execute_Method_<framework::METHOD_TYPE_framework_GameObject_TurnTowardsTarget, int, float >(method_p0);
			return BT_RUNNING;
		}
		float method_p0;
	};

	class BEHAVIAC_API Action_bt_base_homing_node3 : public Action
	{
	public:
		BEHAVIAC_DECLARE_DYNAMIC_TYPE(Action_bt_base_homing_node3, Action);
		Action_bt_base_homing_node3()
		{
			method_p0 = 10;
		}
	protected:
		virtual EBTStatus update_impl(Agent* pAgent, EBTStatus childStatus)
		{
			BEHAVIAC_UNUSED_VAR(pAgent);
			BEHAVIAC_UNUSED_VAR(childStatus);
			((framework::GameObject*)pAgent)->_Execute_Method_<framework::METHOD_TYPE_framework_GameObject_GoStraight, void, int >(method_p0);
			return BT_RUNNING;
		}
		int method_p0;
	};

	BEHAVIAC_API bool Create_bt_base_homing(BehaviorTree* pBT)
	{
		pBT->SetClassNameString("BehaviorTree");
		pBT->SetId(-1);
		pBT->SetName("base/homing");
		pBT->SetIsFSM(false);
#if !defined(BEHAVIAC_RELEASE)
		pBT->SetAgentType("framework::GameObject");
#endif
		// pars
		pBT->AddPar("framework::GameObject", "int", "par_homing_a", "0");
		// children
		{
			Parallel_bt_base_homing_node1* node1 = BEHAVIAC_NEW Parallel_bt_base_homing_node1;
			node1->SetClassNameString("Parallel");
			node1->SetId(1);
#if !defined(BEHAVIAC_RELEASE)
			node1->SetAgentType("framework::GameObject");
#endif
			pBT->AddChild(node1);
			{
				Action_bt_base_homing_node2* node2 = BEHAVIAC_NEW Action_bt_base_homing_node2;
				node2->SetClassNameString("Action");
				node2->SetId(2);
#if !defined(BEHAVIAC_RELEASE)
				node2->SetAgentType("framework::GameObject");
#endif
				node1->AddChild(node2);
				node1->SetHasEvents(node1->HasEvents() | node2->HasEvents());
			}
			{
				Action_bt_base_homing_node3* node3 = BEHAVIAC_NEW Action_bt_base_homing_node3;
				node3->SetClassNameString("Action");
				node3->SetId(3);
#if !defined(BEHAVIAC_RELEASE)
				node3->SetAgentType("framework::GameObject");
#endif
				node1->AddChild(node3);
				node1->SetHasEvents(node1->HasEvents() | node3->HasEvents());
			}
			pBT->SetHasEvents(pBT->HasEvents() | node1->HasEvents());
		}
		return true;
	}

	// Source file: base/slowboring

	class BEHAVIAC_API Action_bt_base_slowboring_node1 : public Action
	{
	public:
		BEHAVIAC_DECLARE_DYNAMIC_TYPE(Action_bt_base_slowboring_node1, Action);
		Action_bt_base_slowboring_node1()
		{
			method_p0 = 10;
		}
	protected:
		virtual EBTStatus update_impl(Agent* pAgent, EBTStatus childStatus)
		{
			BEHAVIAC_UNUSED_VAR(pAgent);
			BEHAVIAC_UNUSED_VAR(childStatus);
			((framework::GameObject*)pAgent)->_Execute_Method_<framework::METHOD_TYPE_framework_GameObject_GoStraight, void, int >(method_p0);
			return BT_RUNNING;
		}
		int method_p0;
	};

	BEHAVIAC_API bool Create_bt_base_slowboring(BehaviorTree* pBT)
	{
		pBT->SetClassNameString("BehaviorTree");
		pBT->SetId(-1);
		pBT->SetName("base/slowboring");
		pBT->SetIsFSM(false);
#if !defined(BEHAVIAC_RELEASE)
		pBT->SetAgentType("framework::GameObject");
#endif
		// children
		{
			Action_bt_base_slowboring_node1* node1 = BEHAVIAC_NEW Action_bt_base_slowboring_node1;
			node1->SetClassNameString("Action");
			node1->SetId(1);
#if !defined(BEHAVIAC_RELEASE)
			node1->SetAgentType("framework::GameObject");
#endif
			pBT->AddChild(node1);
			pBT->SetHasEvents(pBT->HasEvents() | node1->HasEvents());
		}
		return true;
	}

	// Source file: base/sync_homing

	class BEHAVIAC_API Condition_bt_base_sync_homing_node0 : public Condition
	{
	public:
		BEHAVIAC_DECLARE_DYNAMIC_TYPE(Condition_bt_base_sync_homing_node0, Condition);
		Condition_bt_base_sync_homing_node0()
		{
		}
	protected:
		virtual EBTStatus update_impl(Agent* pAgent, EBTStatus childStatus)
		{
			BEHAVIAC_UNUSED_VAR(pAgent);
			BEHAVIAC_UNUSED_VAR(childStatus);
			Agent* pAgent_opl = Agent::GetInstance("framework::WorldState", pAgent->GetContextId());
			BEHAVIAC_ASSERT(pAgent_opl);
			bool opl = ((framework::WorldState*)pAgent_opl)->_Get_Property_<framework::PROPERTY_TYPE_framework_WorldState_SyncSignal, bool >();
			bool opr = true;
			bool op = Details::Equal(opl, opr);
			return op ? BT_SUCCESS : BT_FAILURE;
		}
	};

	class BEHAVIAC_API Parallel_bt_base_sync_homing_node4 : public Parallel
	{
	public:
		BEHAVIAC_DECLARE_DYNAMIC_TYPE(Parallel_bt_base_sync_homing_node4, Parallel);
		Parallel_bt_base_sync_homing_node4()
		{
			m_failPolicy = FAIL_ON_ONE;
			m_succeedPolicy = SUCCEED_ON_ALL;
			m_exitPolicy = EXIT_NONE;
			m_childFinishPolicy = CHILDFINISH_LOOP;
		}
	protected:
	};

	class BEHAVIAC_API Action_bt_base_sync_homing_node5 : public Action
	{
	public:
		BEHAVIAC_DECLARE_DYNAMIC_TYPE(Action_bt_base_sync_homing_node5, Action);
		Action_bt_base_sync_homing_node5()
		{
			method_p0 = 2;
		}
	protected:
		virtual EBTStatus update_impl(Agent* pAgent, EBTStatus childStatus)
		{
			BEHAVIAC_UNUSED_VAR(pAgent);
			BEHAVIAC_UNUSED_VAR(childStatus);
			((framework::GameObject*)pAgent)->_Execute_Method_<framework::METHOD_TYPE_framework_GameObject_TurnTowardsTarget, int, float >(method_p0);
			return BT_RUNNING;
		}
		float method_p0;
	};

	class BEHAVIAC_API Action_bt_base_sync_homing_node6 : public Action
	{
	public:
		BEHAVIAC_DECLARE_DYNAMIC_TYPE(Action_bt_base_sync_homing_node6, Action);
		Action_bt_base_sync_homing_node6()
		{
			method_p0 = 10;
		}
	protected:
		virtual EBTStatus update_impl(Agent* pAgent, EBTStatus childStatus)
		{
			BEHAVIAC_UNUSED_VAR(pAgent);
			BEHAVIAC_UNUSED_VAR(childStatus);
			((framework::GameObject*)pAgent)->_Execute_Method_<framework::METHOD_TYPE_framework_GameObject_GoStraight, void, int >(method_p0);
			return BT_RUNNING;
		}
		int method_p0;
	};

	BEHAVIAC_API bool Create_bt_base_sync_homing(BehaviorTree* pBT)
	{
		pBT->SetClassNameString("BehaviorTree");
		pBT->SetId(-1);
		pBT->SetName("base/sync_homing");
		pBT->SetIsFSM(false);
#if !defined(BEHAVIAC_RELEASE)
		pBT->SetAgentType("framework::GameObject");
#endif
		// pars
		pBT->AddPar("framework::GameObject", "int", "par_homing_a", "0");
		// children
		{
			Sequence* node1 = BEHAVIAC_NEW Sequence;
			node1->SetClassNameString("Sequence");
			node1->SetId(1);
#if !defined(BEHAVIAC_RELEASE)
			node1->SetAgentType("framework::GameObject");
#endif
			pBT->AddChild(node1);
			{
				WaitforSignal* node2 = BEHAVIAC_NEW WaitforSignal;
				node2->SetClassNameString("WaitforSignal");
				node2->SetId(2);
#if !defined(BEHAVIAC_RELEASE)
				node2->SetAgentType("framework::GameObject");
#endif
				node1->AddChild(node2);
				{
					Condition_bt_base_sync_homing_node0* node0 = BEHAVIAC_NEW Condition_bt_base_sync_homing_node0;
					node0->SetClassNameString("Condition");
					node0->SetId(0);
#if !defined(BEHAVIAC_RELEASE)
					node0->SetAgentType("framework::GameObject");
#endif
					node2->SetCustomCondition(node0);
					node2->SetHasEvents(node2->HasEvents() | node0->HasEvents());
				}
				node1->SetHasEvents(node1->HasEvents() | node2->HasEvents());
			}
			{
				Parallel_bt_base_sync_homing_node4* node4 = BEHAVIAC_NEW Parallel_bt_base_sync_homing_node4;
				node4->SetClassNameString("Parallel");
				node4->SetId(4);
#if !defined(BEHAVIAC_RELEASE)
				node4->SetAgentType("framework::GameObject");
#endif
				node1->AddChild(node4);
				{
					Action_bt_base_sync_homing_node5* node5 = BEHAVIAC_NEW Action_bt_base_sync_homing_node5;
					node5->SetClassNameString("Action");
					node5->SetId(5);
#if !defined(BEHAVIAC_RELEASE)
					node5->SetAgentType("framework::GameObject");
#endif
					node4->AddChild(node5);
					node4->SetHasEvents(node4->HasEvents() | node5->HasEvents());
				}
				{
					Action_bt_base_sync_homing_node6* node6 = BEHAVIAC_NEW Action_bt_base_sync_homing_node6;
					node6->SetClassNameString("Action");
					node6->SetId(6);
#if !defined(BEHAVIAC_RELEASE)
					node6->SetAgentType("framework::GameObject");
#endif
					node4->AddChild(node6);
					node4->SetHasEvents(node4->HasEvents() | node6->HasEvents());
				}
				node1->SetHasEvents(node1->HasEvents() | node4->HasEvents());
			}
			pBT->SetHasEvents(pBT->HasEvents() | node1->HasEvents());
		}
		return true;
	}

	// Source file: projectile/projectile_test

	class BEHAVIAC_API Condition_bt_projectile_projectile_test_node0 : public Condition
	{
	public:
		BEHAVIAC_DECLARE_DYNAMIC_TYPE(Condition_bt_projectile_projectile_test_node0, Condition);
		Condition_bt_projectile_projectile_test_node0()
		{
		}
	protected:
		virtual EBTStatus update_impl(Agent* pAgent, EBTStatus childStatus)
		{
			BEHAVIAC_UNUSED_VAR(pAgent);
			BEHAVIAC_UNUSED_VAR(childStatus);
			Agent* pAgent_opl = Agent::GetInstance("framework::GameObject", pAgent->GetContextId());
			BEHAVIAC_ASSERT(pAgent_opl);
			unsigned int opl = ((framework::GameObject*)pAgent_opl)->_Get_Property_<framework::PROPERTY_TYPE_framework_GameObject_HP, unsigned int >();
			unsigned int opr = 50;
			bool op = Details::Greater(opl, opr);
			return op ? BT_SUCCESS : BT_FAILURE;
		}
	};

	class BEHAVIAC_API Action_bt_projectile_projectile_test_node4 : public Action
	{
	public:
		BEHAVIAC_DECLARE_DYNAMIC_TYPE(Action_bt_projectile_projectile_test_node4, Action);
		Action_bt_projectile_projectile_test_node4()
		{
			method_p0 = 5;
		}
	protected:
		virtual EBTStatus update_impl(Agent* pAgent, EBTStatus childStatus)
		{
			BEHAVIAC_UNUSED_VAR(pAgent);
			BEHAVIAC_UNUSED_VAR(childStatus);
			((framework::GameObject*)pAgent)->_Execute_Method_<framework::METHOD_TYPE_framework_GameObject_GoStraight, void, int >(method_p0);
			return BT_SUCCESS;
		}
		int method_p0;
	};

	class BEHAVIAC_API Action_bt_projectile_projectile_test_node5 : public Action
	{
	public:
		BEHAVIAC_DECLARE_DYNAMIC_TYPE(Action_bt_projectile_projectile_test_node5, Action);
		Action_bt_projectile_projectile_test_node5()
		{
			method_p0 = 0;
		}
	protected:
		virtual EBTStatus update_impl(Agent* pAgent, EBTStatus childStatus)
		{
			BEHAVIAC_UNUSED_VAR(pAgent);
			BEHAVIAC_UNUSED_VAR(childStatus);
			((framework::GameObject*)pAgent)->_Execute_Method_<framework::METHOD_TYPE_framework_GameObject_GoStraight, void, int >(method_p0);
			return BT_SUCCESS;
		}
		int method_p0;
	};

	class BEHAVIAC_API Query_bt_projectile_projectile_test_node6 : public Query
	{
	public:
		BEHAVIAC_DECLARE_DYNAMIC_TYPE(Query_bt_projectile_projectile_test_node6, Query);
		Query_bt_projectile_projectile_test_node6()
		{
			this->Initialize("attack", "0:");
		}
	public:
		void Initialize(const char* domain, const char* descriptors)
		{
			this->m_domain = domain;
			this->SetDescriptors(descriptors);
		}
	};

	class BEHAVIAC_API Condition_bt_projectile_projectile_test_node7 : public Condition
	{
	public:
		BEHAVIAC_DECLARE_DYNAMIC_TYPE(Condition_bt_projectile_projectile_test_node7, Condition);
		Condition_bt_projectile_projectile_test_node7()
		{
		}
	protected:
		virtual EBTStatus update_impl(Agent* pAgent, EBTStatus childStatus)
		{
			BEHAVIAC_UNUSED_VAR(pAgent);
			BEHAVIAC_UNUSED_VAR(childStatus);
			Agent* pAgent_opl = Agent::GetInstance("framework::GameObject", pAgent->GetContextId());
			BEHAVIAC_ASSERT(pAgent_opl);
			unsigned int opl = ((framework::GameObject*)pAgent_opl)->_Get_Property_<framework::PROPERTY_TYPE_framework_GameObject_HP, unsigned int >();
			unsigned int opr = 0;
			bool op = Details::Equal(opl, opr);
			return op ? BT_SUCCESS : BT_FAILURE;
		}
	};

	class BEHAVIAC_API Condition_bt_projectile_projectile_test_node8 : public Condition
	{
	public:
		BEHAVIAC_DECLARE_DYNAMIC_TYPE(Condition_bt_projectile_projectile_test_node8, Condition);
		Condition_bt_projectile_projectile_test_node8()
		{
		}
	protected:
		virtual EBTStatus update_impl(Agent* pAgent, EBTStatus childStatus)
		{
			BEHAVIAC_UNUSED_VAR(pAgent);
			BEHAVIAC_UNUSED_VAR(childStatus);
			bool opl = ((framework::GameObject*)pAgent)->_Execute_Method_<framework::METHOD_TYPE_framework_GameObject_playerIsAligned, bool >();
			bool opr = false;
			bool op = Details::Equal(opl, opr);
			return op ? BT_SUCCESS : BT_FAILURE;
		}
	};

	class BEHAVIAC_API Assignment_bt_projectile_projectile_test_node9 : public Assignment
	{
	public:
		BEHAVIAC_DECLARE_DYNAMIC_TYPE(Assignment_bt_projectile_projectile_test_node9, Assignment);
		Assignment_bt_projectile_projectile_test_node9()
		{
		}
	protected:
		virtual EBTStatus update_impl(Agent* pAgent, EBTStatus childStatus)
		{
			BEHAVIAC_UNUSED_VAR(pAgent);
			BEHAVIAC_UNUSED_VAR(childStatus);
			EBTStatus result = BT_SUCCESS;
			bool opr = true;
			BEHAVIAC_ASSERT(behaviac::MakeVariableId("par_test_a") == 3889063143u);
			pAgent->SetVariable("par_test_a", opr, 3889063143u);
			return result;
		}
	};

	class BEHAVIAC_API Condition_bt_projectile_projectile_test_node11 : public Condition
	{
	public:
		BEHAVIAC_DECLARE_DYNAMIC_TYPE(Condition_bt_projectile_projectile_test_node11, Condition);
		Condition_bt_projectile_projectile_test_node11()
		{
		}
	protected:
		virtual EBTStatus update_impl(Agent* pAgent, EBTStatus childStatus)
		{
			BEHAVIAC_UNUSED_VAR(pAgent);
			BEHAVIAC_UNUSED_VAR(childStatus);
			BEHAVIAC_ASSERT(behaviac::MakeVariableId("par_test_a") == 3889063143u);
			bool& opl = (bool&)pAgent->GetVariable<bool >(3889063143u);
			bool opr = false;
			bool op = Details::Equal(opl, opr);
			return op ? BT_SUCCESS : BT_FAILURE;
		}
	};

	class BEHAVIAC_API Condition_bt_projectile_projectile_test_node12 : public Condition
	{
	public:
		BEHAVIAC_DECLARE_DYNAMIC_TYPE(Condition_bt_projectile_projectile_test_node12, Condition);
		Condition_bt_projectile_projectile_test_node12()
		{
		}
	protected:
		virtual EBTStatus update_impl(Agent* pAgent, EBTStatus childStatus)
		{
			BEHAVIAC_UNUSED_VAR(pAgent);
			BEHAVIAC_UNUSED_VAR(childStatus);
			Agent* pAgent_opl = Agent::GetInstance("framework::GameObject", pAgent->GetContextId());
			BEHAVIAC_ASSERT(pAgent_opl);
			unsigned int opl = ((framework::GameObject*)pAgent_opl)->_Get_Property_<framework::PROPERTY_TYPE_framework_GameObject_HP, unsigned int >();
			unsigned int opr = 0;
			bool op = Details::Equal(opl, opr);
			return op ? BT_SUCCESS : BT_FAILURE;
		}
	};

	class BEHAVIAC_API Condition_bt_projectile_projectile_test_node13 : public Condition
	{
	public:
		BEHAVIAC_DECLARE_DYNAMIC_TYPE(Condition_bt_projectile_projectile_test_node13, Condition);
		Condition_bt_projectile_projectile_test_node13()
		{
		}
	protected:
		virtual EBTStatus update_impl(Agent* pAgent, EBTStatus childStatus)
		{
			BEHAVIAC_UNUSED_VAR(pAgent);
			BEHAVIAC_UNUSED_VAR(childStatus);
			Agent* pAgent_opl = Agent::GetInstance("framework::WorldState", pAgent->GetContextId());
			BEHAVIAC_ASSERT(pAgent_opl);
			unsigned int opl = ((framework::WorldState*)pAgent_opl)->_Get_Property_<framework::PROPERTY_TYPE_framework_WorldState_time, unsigned int >();
			BEHAVIAC_ASSERT(behaviac::MakeVariableId("par_test_d") == 2595852450u);
			unsigned int& opr = (unsigned int&)pAgent->GetVariable<unsigned int >(2595852450u);
			bool op = Details::Equal(opl, opr);
			return op ? BT_SUCCESS : BT_FAILURE;
		}
	};

	class BEHAVIAC_API DecoratorLoop_bt_projectile_projectile_test_node14 : public DecoratorLoop
	{
	public:
		BEHAVIAC_DECLARE_DYNAMIC_TYPE(DecoratorLoop_bt_projectile_projectile_test_node14, DecoratorLoop);
		DecoratorLoop_bt_projectile_projectile_test_node14()
		{
			m_bDecorateWhenChildEnds = true;
		}
	protected:
		virtual int GetCount(Agent* pAgent) const
		{
			BEHAVIAC_UNUSED_VAR(pAgent);
			return -1;
		}
	};

	class BEHAVIAC_API Condition_bt_projectile_projectile_test_node15 : public Condition
	{
	public:
		BEHAVIAC_DECLARE_DYNAMIC_TYPE(Condition_bt_projectile_projectile_test_node15, Condition);
		Condition_bt_projectile_projectile_test_node15()
		{
		}
	protected:
		virtual EBTStatus update_impl(Agent* pAgent, EBTStatus childStatus)
		{
			BEHAVIAC_UNUSED_VAR(pAgent);
			BEHAVIAC_UNUSED_VAR(childStatus);
			bool opl = ((framework::GameObject*)pAgent)->_Execute_Method_<framework::METHOD_TYPE_framework_GameObject_alignedWithPlayer, bool >();
			bool opr = false;
			bool op = Details::Equal(opl, opr);
			return op ? BT_SUCCESS : BT_FAILURE;
		}
	};

	class BEHAVIAC_API DecoratorCountLimit_bt_projectile_projectile_test_node16 : public DecoratorCountLimit
	{
	public:
		BEHAVIAC_DECLARE_DYNAMIC_TYPE(DecoratorCountLimit_bt_projectile_projectile_test_node16, DecoratorCountLimit);
		DecoratorCountLimit_bt_projectile_projectile_test_node16()
		{
			m_bDecorateWhenChildEnds = true;
		}
	protected:
		virtual int GetCount(Agent* pAgent) const
		{
			BEHAVIAC_UNUSED_VAR(pAgent);
			return (int&)pAgent->GetVariable<int >(3437442852u);
		}
	};

	class BEHAVIAC_API Condition_bt_projectile_projectile_test_node17 : public Condition
	{
	public:
		BEHAVIAC_DECLARE_DYNAMIC_TYPE(Condition_bt_projectile_projectile_test_node17, Condition);
		Condition_bt_projectile_projectile_test_node17()
		{
		}
	protected:
		virtual EBTStatus update_impl(Agent* pAgent, EBTStatus childStatus)
		{
			BEHAVIAC_UNUSED_VAR(pAgent);
			BEHAVIAC_UNUSED_VAR(childStatus);
			Agent* pAgent_ = Agent::GetInstance("framework::WorldState", pAgent->GetContextId());
			BEHAVIAC_ASSERT(pAgent_);
			bool opl = ((framework::WorldState*)pAgent_)->_Execute_Method_<framework::METHOD_TYPE_framework_WorldState_NextWave, bool >();
			bool opr = true;
			bool op = Details::Equal(opl, opr);
			return op ? BT_SUCCESS : BT_FAILURE;
		}
	};

	class BEHAVIAC_API DecoratorLoop_bt_projectile_projectile_test_node18 : public DecoratorLoop
	{
	public:
		BEHAVIAC_DECLARE_DYNAMIC_TYPE(DecoratorLoop_bt_projectile_projectile_test_node18, DecoratorLoop);
		DecoratorLoop_bt_projectile_projectile_test_node18()
		{
			m_bDecorateWhenChildEnds = true;
		}
	protected:
		virtual int GetCount(Agent* pAgent) const
		{
			BEHAVIAC_UNUSED_VAR(pAgent);
			Agent* pAgent_ = Agent::GetInstance("framework::WorldState", pAgent->GetContextId());
			BEHAVIAC_ASSERT(pAgent_);
			return ((framework::WorldState*)pAgent_)->_Get_Property_<framework::PROPERTY_TYPE_framework_WorldState_time, unsigned int >();
		}
	};

	class BEHAVIAC_API DecoratorNot_bt_projectile_projectile_test_node21 : public DecoratorNot
	{
	public:
		BEHAVIAC_DECLARE_DYNAMIC_TYPE(DecoratorNot_bt_projectile_projectile_test_node21, DecoratorNot);
		DecoratorNot_bt_projectile_projectile_test_node21()
		{
			m_bDecorateWhenChildEnds = false;
		}
	protected:
	};

	class BEHAVIAC_API Condition_bt_projectile_projectile_test_node25 : public Condition
	{
	public:
		BEHAVIAC_DECLARE_DYNAMIC_TYPE(Condition_bt_projectile_projectile_test_node25, Condition);
		Condition_bt_projectile_projectile_test_node25()
		{
		}
	protected:
		virtual EBTStatus update_impl(Agent* pAgent, EBTStatus childStatus)
		{
			BEHAVIAC_UNUSED_VAR(pAgent);
			BEHAVIAC_UNUSED_VAR(childStatus);
			Agent* pAgent_opl = Agent::GetInstance("framework::GameObject", pAgent->GetContextId());
			BEHAVIAC_ASSERT(pAgent_opl);
			unsigned int opl = ((framework::GameObject*)pAgent_opl)->_Get_Property_<framework::PROPERTY_TYPE_framework_GameObject_HP, unsigned int >();
			unsigned int opr = 0;
			bool op = Details::Equal(opl, opr);
			return op ? BT_SUCCESS : BT_FAILURE;
		}
	};

	class BEHAVIAC_API Condition_bt_projectile_projectile_test_node26 : public Condition
	{
	public:
		BEHAVIAC_DECLARE_DYNAMIC_TYPE(Condition_bt_projectile_projectile_test_node26, Condition);
		Condition_bt_projectile_projectile_test_node26()
		{
		}
	protected:
		virtual EBTStatus update_impl(Agent* pAgent, EBTStatus childStatus)
		{
			BEHAVIAC_UNUSED_VAR(pAgent);
			BEHAVIAC_UNUSED_VAR(childStatus);
			float opl = ((framework::GameObject*)pAgent)->_Execute_Method_<framework::METHOD_TYPE_framework_GameObject_distanceToPlayer, float >();
			float opr = 0;
			bool op = Details::Equal(opl, opr);
			return op ? BT_SUCCESS : BT_FAILURE;
		}
	};

	BEHAVIAC_API bool Create_bt_projectile_projectile_test(BehaviorTree* pBT)
	{
		pBT->SetClassNameString("BehaviorTree");
		pBT->SetId(-1);
		pBT->SetName("projectile/projectile_test");
		pBT->SetIsFSM(false);
#if !defined(BEHAVIAC_RELEASE)
		pBT->SetAgentType("framework::Projectile");
#endif
		// pars
		pBT->AddPar("framework::Projectile", "bool", "par_test_a", "false");
		pBT->AddPar("framework::Projectile", "int", "par_test_b", "0");
		pBT->AddPar("framework::Projectile", "float", "par_test_c", "0");
		pBT->AddPar("framework::Projectile", "uint", "par_test_d", "0");
		// children
		{
			Selector* node1 = BEHAVIAC_NEW Selector;
			node1->SetClassNameString("Selector");
			node1->SetId(1);
#if !defined(BEHAVIAC_RELEASE)
			node1->SetAgentType("framework::Projectile");
#endif
			pBT->AddChild(node1);
			{
				WaitforSignal* node2 = BEHAVIAC_NEW WaitforSignal;
				node2->SetClassNameString("WaitforSignal");
				node2->SetId(2);
#if !defined(BEHAVIAC_RELEASE)
				node2->SetAgentType("framework::Projectile");
#endif
				node1->AddChild(node2);
				{
					Condition_bt_projectile_projectile_test_node0* node0 = BEHAVIAC_NEW Condition_bt_projectile_projectile_test_node0;
					node0->SetClassNameString("Condition");
					node0->SetId(0);
#if !defined(BEHAVIAC_RELEASE)
					node0->SetAgentType("framework::Projectile");
#endif
					node2->SetCustomCondition(node0);
					node2->SetHasEvents(node2->HasEvents() | node0->HasEvents());
				}
				{
					Action_bt_projectile_projectile_test_node4* node4 = BEHAVIAC_NEW Action_bt_projectile_projectile_test_node4;
					node4->SetClassNameString("Action");
					node4->SetId(4);
#if !defined(BEHAVIAC_RELEASE)
					node4->SetAgentType("framework::Projectile");
#endif
					node2->AddChild(node4);
					node2->SetHasEvents(node2->HasEvents() | node4->HasEvents());
				}
				node1->SetHasEvents(node1->HasEvents() | node2->HasEvents());
			}
			{
				Action_bt_projectile_projectile_test_node5* node5 = BEHAVIAC_NEW Action_bt_projectile_projectile_test_node5;
				node5->SetClassNameString("Action");
				node5->SetId(5);
#if !defined(BEHAVIAC_RELEASE)
				node5->SetAgentType("framework::Projectile");
#endif
				node1->AddChild(node5);
				node1->SetHasEvents(node1->HasEvents() | node5->HasEvents());
			}
			{
				Query_bt_projectile_projectile_test_node6* node6 = BEHAVIAC_NEW Query_bt_projectile_projectile_test_node6;
				node6->SetClassNameString("Query");
				node6->SetId(6);
#if !defined(BEHAVIAC_RELEASE)
				node6->SetAgentType("framework::Projectile");
#endif
				node1->AddChild(node6);
				{
					And* node3 = BEHAVIAC_NEW And;
					node3->SetClassNameString("And");
					node3->SetId(3);
#if !defined(BEHAVIAC_RELEASE)
					node3->SetAgentType("framework::Projectile");
#endif
					node6->SetCustomCondition(node3);
					{
						Condition_bt_projectile_projectile_test_node7* node7 = BEHAVIAC_NEW Condition_bt_projectile_projectile_test_node7;
						node7->SetClassNameString("Condition");
						node7->SetId(7);
#if !defined(BEHAVIAC_RELEASE)
						node7->SetAgentType("framework::Projectile");
#endif
						node3->AddChild(node7);
						node3->SetHasEvents(node3->HasEvents() | node7->HasEvents());
					}
					{
						Condition_bt_projectile_projectile_test_node8* node8 = BEHAVIAC_NEW Condition_bt_projectile_projectile_test_node8;
						node8->SetClassNameString("Condition");
						node8->SetId(8);
#if !defined(BEHAVIAC_RELEASE)
						node8->SetAgentType("framework::Projectile");
#endif
						node3->AddChild(node8);
						node3->SetHasEvents(node3->HasEvents() | node8->HasEvents());
					}
					node6->SetHasEvents(node6->HasEvents() | node3->HasEvents());
				}
				node1->SetHasEvents(node1->HasEvents() | node6->HasEvents());
			}
			{
				Assignment_bt_projectile_projectile_test_node9* node9 = BEHAVIAC_NEW Assignment_bt_projectile_projectile_test_node9;
				node9->SetClassNameString("Assignment");
				node9->SetId(9);
#if !defined(BEHAVIAC_RELEASE)
				node9->SetAgentType("framework::Projectile");
#endif
				node1->AddChild(node9);
				node1->SetHasEvents(node1->HasEvents() | node9->HasEvents());
			}
			{
				Sequence* node10 = BEHAVIAC_NEW Sequence;
				node10->SetClassNameString("Sequence");
				node10->SetId(10);
#if !defined(BEHAVIAC_RELEASE)
				node10->SetAgentType("framework::Projectile");
#endif
				node1->AddChild(node10);
				{
					Condition_bt_projectile_projectile_test_node11* node11 = BEHAVIAC_NEW Condition_bt_projectile_projectile_test_node11;
					node11->SetClassNameString("Condition");
					node11->SetId(11);
#if !defined(BEHAVIAC_RELEASE)
					node11->SetAgentType("framework::Projectile");
#endif
					node10->AddChild(node11);
					node10->SetHasEvents(node10->HasEvents() | node11->HasEvents());
				}
				{
					Condition_bt_projectile_projectile_test_node12* node12 = BEHAVIAC_NEW Condition_bt_projectile_projectile_test_node12;
					node12->SetClassNameString("Condition");
					node12->SetId(12);
#if !defined(BEHAVIAC_RELEASE)
					node12->SetAgentType("framework::Projectile");
#endif
					node10->AddChild(node12);
					node10->SetHasEvents(node10->HasEvents() | node12->HasEvents());
				}
				{
					Condition_bt_projectile_projectile_test_node13* node13 = BEHAVIAC_NEW Condition_bt_projectile_projectile_test_node13;
					node13->SetClassNameString("Condition");
					node13->SetId(13);
#if !defined(BEHAVIAC_RELEASE)
					node13->SetAgentType("framework::Projectile");
#endif
					node10->AddChild(node13);
					node10->SetHasEvents(node10->HasEvents() | node13->HasEvents());
				}
				node1->SetHasEvents(node1->HasEvents() | node10->HasEvents());
			}
			{
				DecoratorLoop_bt_projectile_projectile_test_node14* node14 = BEHAVIAC_NEW DecoratorLoop_bt_projectile_projectile_test_node14;
				node14->SetClassNameString("DecoratorLoop");
				node14->SetId(14);
#if !defined(BEHAVIAC_RELEASE)
				node14->SetAgentType("framework::Projectile");
#endif
				node1->AddChild(node14);
				{
					Condition_bt_projectile_projectile_test_node15* node15 = BEHAVIAC_NEW Condition_bt_projectile_projectile_test_node15;
					node15->SetClassNameString("Condition");
					node15->SetId(15);
#if !defined(BEHAVIAC_RELEASE)
					node15->SetAgentType("framework::Projectile");
#endif
					node14->AddChild(node15);
					node14->SetHasEvents(node14->HasEvents() | node15->HasEvents());
				}
				node1->SetHasEvents(node1->HasEvents() | node14->HasEvents());
			}
			{
				DecoratorCountLimit_bt_projectile_projectile_test_node16* node16 = BEHAVIAC_NEW DecoratorCountLimit_bt_projectile_projectile_test_node16;
				node16->SetClassNameString("DecoratorCountLimit");
				node16->SetId(16);
#if !defined(BEHAVIAC_RELEASE)
				node16->SetAgentType("framework::Projectile");
#endif
				node1->AddChild(node16);
				{
					Condition_bt_projectile_projectile_test_node17* node17 = BEHAVIAC_NEW Condition_bt_projectile_projectile_test_node17;
					node17->SetClassNameString("Condition");
					node17->SetId(17);
#if !defined(BEHAVIAC_RELEASE)
					node17->SetAgentType("framework::Projectile");
#endif
					node16->AddChild(node17);
					node16->SetHasEvents(node16->HasEvents() | node17->HasEvents());
				}
				node1->SetHasEvents(node1->HasEvents() | node16->HasEvents());
			}
			{
				DecoratorLoop_bt_projectile_projectile_test_node18* node18 = BEHAVIAC_NEW DecoratorLoop_bt_projectile_projectile_test_node18;
				node18->SetClassNameString("DecoratorLoop");
				node18->SetId(18);
#if !defined(BEHAVIAC_RELEASE)
				node18->SetAgentType("framework::Projectile");
#endif
				node1->AddChild(node18);
				{
					Noop* node19 = BEHAVIAC_NEW Noop;
					node19->SetClassNameString("Noop");
					node19->SetId(19);
#if !defined(BEHAVIAC_RELEASE)
					node19->SetAgentType("framework::Projectile");
#endif
					node18->AddChild(node19);
					node18->SetHasEvents(node18->HasEvents() | node19->HasEvents());
				}
				node1->SetHasEvents(node1->HasEvents() | node18->HasEvents());
			}
			{
				Sequence* node20 = BEHAVIAC_NEW Sequence;
				node20->SetClassNameString("Sequence");
				node20->SetId(20);
#if !defined(BEHAVIAC_RELEASE)
				node20->SetAgentType("framework::Projectile");
#endif
				node1->AddChild(node20);
				{
					DecoratorNot_bt_projectile_projectile_test_node21* node21 = BEHAVIAC_NEW DecoratorNot_bt_projectile_projectile_test_node21;
					node21->SetClassNameString("DecoratorNot");
					node21->SetId(21);
#if !defined(BEHAVIAC_RELEASE)
					node21->SetAgentType("framework::Projectile");
#endif
					node20->SetCustomCondition(node21);
					{
						And* node22 = BEHAVIAC_NEW And;
						node22->SetClassNameString("And");
						node22->SetId(22);
#if !defined(BEHAVIAC_RELEASE)
						node22->SetAgentType("framework::Projectile");
#endif
						node21->AddChild(node22);
						{
							Condition_bt_projectile_projectile_test_node25* node25 = BEHAVIAC_NEW Condition_bt_projectile_projectile_test_node25;
							node25->SetClassNameString("Condition");
							node25->SetId(25);
#if !defined(BEHAVIAC_RELEASE)
							node25->SetAgentType("framework::Projectile");
#endif
							node22->AddChild(node25);
							node22->SetHasEvents(node22->HasEvents() | node25->HasEvents());
						}
						{
							Condition_bt_projectile_projectile_test_node26* node26 = BEHAVIAC_NEW Condition_bt_projectile_projectile_test_node26;
							node26->SetClassNameString("Condition");
							node26->SetId(26);
#if !defined(BEHAVIAC_RELEASE)
							node26->SetAgentType("framework::Projectile");
#endif
							node22->AddChild(node26);
							node22->SetHasEvents(node22->HasEvents() | node26->HasEvents());
						}
						node21->SetHasEvents(node21->HasEvents() | node22->HasEvents());
					}
					node20->SetHasEvents(node20->HasEvents() | node21->HasEvents());
				}
				{
					Noop* node23 = BEHAVIAC_NEW Noop;
					node23->SetClassNameString("Noop");
					node23->SetId(23);
#if !defined(BEHAVIAC_RELEASE)
					node23->SetAgentType("framework::Projectile");
#endif
					node20->AddChild(node23);
					node20->SetHasEvents(node20->HasEvents() | node23->HasEvents());
				}
				{
					Noop* node24 = BEHAVIAC_NEW Noop;
					node24->SetClassNameString("Noop");
					node24->SetId(24);
#if !defined(BEHAVIAC_RELEASE)
					node24->SetAgentType("framework::Projectile");
#endif
					node20->AddChild(node24);
					node20->SetHasEvents(node20->HasEvents() | node24->HasEvents());
				}
				node1->SetHasEvents(node1->HasEvents() | node20->HasEvents());
			}
			pBT->SetHasEvents(pBT->HasEvents() | node1->HasEvents());
		}
		return true;
	}

	// Source file: ships/0_basic

	class BEHAVIAC_API Parallel_bt_ships_0_basic_node1 : public Parallel
	{
	public:
		BEHAVIAC_DECLARE_DYNAMIC_TYPE(Parallel_bt_ships_0_basic_node1, Parallel);
		Parallel_bt_ships_0_basic_node1()
		{
			m_failPolicy = FAIL_ON_ONE;
			m_succeedPolicy = SUCCEED_ON_ALL;
			m_exitPolicy = EXIT_NONE;
			m_childFinishPolicy = CHILDFINISH_LOOP;
		}
	protected:
	};

	class BEHAVIAC_API DecoratorLoop_bt_ships_0_basic_node2 : public DecoratorLoop
	{
	public:
		BEHAVIAC_DECLARE_DYNAMIC_TYPE(DecoratorLoop_bt_ships_0_basic_node2, DecoratorLoop);
		DecoratorLoop_bt_ships_0_basic_node2()
		{
			m_bDecorateWhenChildEnds = true;
		}
	protected:
		virtual int GetCount(Agent* pAgent) const
		{
			BEHAVIAC_UNUSED_VAR(pAgent);
			return -1;
		}
	};

	class BEHAVIAC_API Condition_bt_ships_0_basic_node4 : public Condition
	{
	public:
		BEHAVIAC_DECLARE_DYNAMIC_TYPE(Condition_bt_ships_0_basic_node4, Condition);
		Condition_bt_ships_0_basic_node4()
		{
		}
	protected:
		virtual EBTStatus update_impl(Agent* pAgent, EBTStatus childStatus)
		{
			BEHAVIAC_UNUSED_VAR(pAgent);
			BEHAVIAC_UNUSED_VAR(childStatus);
			bool opl = ((framework::GameObject*)pAgent)->_Execute_Method_<framework::METHOD_TYPE_framework_GameObject_alignedWithPlayer, bool >();
			bool opr = true;
			bool op = Details::Equal(opl, opr);
			return op ? BT_SUCCESS : BT_FAILURE;
		}
	};

	class BEHAVIAC_API Assignment_bt_ships_0_basic_node0 : public Assignment
	{
	public:
		BEHAVIAC_DECLARE_DYNAMIC_TYPE(Assignment_bt_ships_0_basic_node0, Assignment);
		Assignment_bt_ships_0_basic_node0()
		{
		}
	protected:
		virtual EBTStatus update_impl(Agent* pAgent, EBTStatus childStatus)
		{
			BEHAVIAC_UNUSED_VAR(pAgent);
			BEHAVIAC_UNUSED_VAR(childStatus);
			EBTStatus result = BT_SUCCESS;
			int opr = 4;
			BEHAVIAC_ASSERT(behaviac::MakeVariableId("par_b") == 1614475600u);
			pAgent->SetVariable("par_b", opr, 1614475600u);
			return result;
		}
	};

	class BEHAVIAC_API Action_bt_ships_0_basic_node5 : public Action
	{
	public:
		BEHAVIAC_DECLARE_DYNAMIC_TYPE(Action_bt_ships_0_basic_node5, Action);
		Action_bt_ships_0_basic_node5()
		{
		}
	protected:
		virtual EBTStatus update_impl(Agent* pAgent, EBTStatus childStatus)
		{
			BEHAVIAC_UNUSED_VAR(pAgent);
			BEHAVIAC_UNUSED_VAR(childStatus);
			bool result = ((framework::Ship*)pAgent)->_Execute_Method_<framework::METHOD_TYPE_framework_Ship_Fire, bool >();
			return ((framework::Ship*)pAgent)->framework::Ship::checkresult(result);
		}
	};

	class BEHAVIAC_API Wait_bt_ships_0_basic_node6 : public Wait
	{
	public:
		BEHAVIAC_DECLARE_DYNAMIC_TYPE(Wait_bt_ships_0_basic_node6, Wait);
		Wait_bt_ships_0_basic_node6()
		{
			m_ignoreTimeScale = false;
		}
	protected:
		virtual float GetTime(Agent* pAgent) const
		{
			BEHAVIAC_UNUSED_VAR(pAgent);
			return (float&)pAgent->GetVariable<float >(2032254993u);
		}
	};

	class BEHAVIAC_API Assignment_bt_ships_0_basic_node8 : public Assignment
	{
	public:
		BEHAVIAC_DECLARE_DYNAMIC_TYPE(Assignment_bt_ships_0_basic_node8, Assignment);
		Assignment_bt_ships_0_basic_node8()
		{
		}
	protected:
		virtual EBTStatus update_impl(Agent* pAgent, EBTStatus childStatus)
		{
			BEHAVIAC_UNUSED_VAR(pAgent);
			BEHAVIAC_UNUSED_VAR(childStatus);
			EBTStatus result = BT_SUCCESS;
			int opr = 11;
			BEHAVIAC_ASSERT(behaviac::MakeVariableId("par_b") == 1614475600u);
			pAgent->SetVariable("par_b", opr, 1614475600u);
			return result;
		}
	};

	class BEHAVIAC_API Assignment_bt_ships_0_basic_node9 : public Assignment
	{
	public:
		BEHAVIAC_DECLARE_DYNAMIC_TYPE(Assignment_bt_ships_0_basic_node9, Assignment);
		Assignment_bt_ships_0_basic_node9()
		{
			opr = (char*)("teststring");
		}
	protected:
		virtual EBTStatus update_impl(Agent* pAgent, EBTStatus childStatus)
		{
			BEHAVIAC_UNUSED_VAR(pAgent);
			BEHAVIAC_UNUSED_VAR(childStatus);
			EBTStatus result = BT_SUCCESS;
			BEHAVIAC_ASSERT(behaviac::MakeVariableId("par_s") == 870568512u);
			pAgent->SetVariable("par_s", opr, 870568512u);
			return result;
		}
		string opr;
	};

	class BEHAVIAC_API Action_bt_ships_0_basic_node7 : public Action
	{
	public:
		BEHAVIAC_DECLARE_DYNAMIC_TYPE(Action_bt_ships_0_basic_node7, Action);
		Action_bt_ships_0_basic_node7()
		{
			method_p0 = 5;
		}
	protected:
		virtual EBTStatus update_impl(Agent* pAgent, EBTStatus childStatus)
		{
			BEHAVIAC_UNUSED_VAR(pAgent);
			BEHAVIAC_UNUSED_VAR(childStatus);
			((framework::GameObject*)pAgent)->_Execute_Method_<framework::METHOD_TYPE_framework_GameObject_TurnTowardsTarget, int, float >(method_p0);
			return BT_RUNNING;
		}
		float method_p0;
	};

	BEHAVIAC_API bool Create_bt_ships_0_basic(BehaviorTree* pBT)
	{
		pBT->SetClassNameString("BehaviorTree");
		pBT->SetId(-1);
		pBT->SetName("ships/0_basic");
		pBT->SetIsFSM(false);
#if !defined(BEHAVIAC_RELEASE)
		pBT->SetAgentType("framework::Ship");
#endif
		// pars
		pBT->AddPar("framework::Ship", "bool", "par_a", "false");
		pBT->AddPar("framework::Ship", "int", "par_b", "500");
		pBT->AddPar("framework::Ship", "float", "par_c", "1000");
		pBT->AddPar("framework::Ship", "string", "par_s", "");
		// children
		{
			Parallel_bt_ships_0_basic_node1* node1 = BEHAVIAC_NEW Parallel_bt_ships_0_basic_node1;
			node1->SetClassNameString("Parallel");
			node1->SetId(1);
#if !defined(BEHAVIAC_RELEASE)
			node1->SetAgentType("framework::Ship");
#endif
			pBT->AddChild(node1);
			{
				DecoratorLoop_bt_ships_0_basic_node2* node2 = BEHAVIAC_NEW DecoratorLoop_bt_ships_0_basic_node2;
				node2->SetClassNameString("DecoratorLoop");
				node2->SetId(2);
#if !defined(BEHAVIAC_RELEASE)
				node2->SetAgentType("framework::Ship");
#endif
				node1->AddChild(node2);
				{
					Sequence* node3 = BEHAVIAC_NEW Sequence;
					node3->SetClassNameString("Sequence");
					node3->SetId(3);
#if !defined(BEHAVIAC_RELEASE)
					node3->SetAgentType("framework::Ship");
#endif
					node2->AddChild(node3);
					{
						Condition_bt_ships_0_basic_node4* node4 = BEHAVIAC_NEW Condition_bt_ships_0_basic_node4;
						node4->SetClassNameString("Condition");
						node4->SetId(4);
#if !defined(BEHAVIAC_RELEASE)
						node4->SetAgentType("framework::Ship");
#endif
						node3->AddChild(node4);
						node3->SetHasEvents(node3->HasEvents() | node4->HasEvents());
					}
					{
						Assignment_bt_ships_0_basic_node0* node0 = BEHAVIAC_NEW Assignment_bt_ships_0_basic_node0;
						node0->SetClassNameString("Assignment");
						node0->SetId(0);
#if !defined(BEHAVIAC_RELEASE)
						node0->SetAgentType("framework::Ship");
#endif
						node3->AddChild(node0);
						node3->SetHasEvents(node3->HasEvents() | node0->HasEvents());
					}
					{
						Action_bt_ships_0_basic_node5* node5 = BEHAVIAC_NEW Action_bt_ships_0_basic_node5;
						node5->SetClassNameString("Action");
						node5->SetId(5);
#if !defined(BEHAVIAC_RELEASE)
						node5->SetAgentType("framework::Ship");
#endif
						node3->AddChild(node5);
						node3->SetHasEvents(node3->HasEvents() | node5->HasEvents());
					}
					{
						Wait_bt_ships_0_basic_node6* node6 = BEHAVIAC_NEW Wait_bt_ships_0_basic_node6;
						node6->SetClassNameString("Wait");
						node6->SetId(6);
#if !defined(BEHAVIAC_RELEASE)
						node6->SetAgentType("framework::Ship");
#endif
						node3->AddChild(node6);
						node3->SetHasEvents(node3->HasEvents() | node6->HasEvents());
					}
					{
						Assignment_bt_ships_0_basic_node8* node8 = BEHAVIAC_NEW Assignment_bt_ships_0_basic_node8;
						node8->SetClassNameString("Assignment");
						node8->SetId(8);
#if !defined(BEHAVIAC_RELEASE)
						node8->SetAgentType("framework::Ship");
#endif
						node3->AddChild(node8);
						node3->SetHasEvents(node3->HasEvents() | node8->HasEvents());
					}
					{
						Assignment_bt_ships_0_basic_node9* node9 = BEHAVIAC_NEW Assignment_bt_ships_0_basic_node9;
						node9->SetClassNameString("Assignment");
						node9->SetId(9);
#if !defined(BEHAVIAC_RELEASE)
						node9->SetAgentType("framework::Ship");
#endif
						node3->AddChild(node9);
						node3->SetHasEvents(node3->HasEvents() | node9->HasEvents());
					}
					node2->SetHasEvents(node2->HasEvents() | node3->HasEvents());
				}
				node1->SetHasEvents(node1->HasEvents() | node2->HasEvents());
			}
			{
				Action_bt_ships_0_basic_node7* node7 = BEHAVIAC_NEW Action_bt_ships_0_basic_node7;
				node7->SetClassNameString("Action");
				node7->SetId(7);
#if !defined(BEHAVIAC_RELEASE)
				node7->SetAgentType("framework::Ship");
#endif
				node1->AddChild(node7);
				node1->SetHasEvents(node1->HasEvents() | node7->HasEvents());
			}
			pBT->SetHasEvents(pBT->HasEvents() | node1->HasEvents());
		}
		return true;
	}

	// Source file: ships/1_1_suicide

	class BEHAVIAC_API DecoratorLoopUntil_bt_ships_1_1_suicide_node2 : public DecoratorLoopUntil
	{
	public:
		BEHAVIAC_DECLARE_DYNAMIC_TYPE(DecoratorLoopUntil_bt_ships_1_1_suicide_node2, DecoratorLoopUntil);
		DecoratorLoopUntil_bt_ships_1_1_suicide_node2()
		{
			m_bDecorateWhenChildEnds = false;
			m_until = false;
		}
	protected:
		virtual int GetCount(Agent* pAgent) const
		{
			BEHAVIAC_UNUSED_VAR(pAgent);
			return -1;
		}
	};

	class BEHAVIAC_API Parallel_bt_ships_1_1_suicide_node3 : public Parallel
	{
	public:
		BEHAVIAC_DECLARE_DYNAMIC_TYPE(Parallel_bt_ships_1_1_suicide_node3, Parallel);
		Parallel_bt_ships_1_1_suicide_node3()
		{
			m_failPolicy = FAIL_ON_ONE;
			m_succeedPolicy = SUCCEED_ON_ALL;
			m_exitPolicy = EXIT_NONE;
			m_childFinishPolicy = CHILDFINISH_LOOP;
		}
	protected:
	};

	class BEHAVIAC_API Condition_bt_ships_1_1_suicide_node4 : public Condition
	{
	public:
		BEHAVIAC_DECLARE_DYNAMIC_TYPE(Condition_bt_ships_1_1_suicide_node4, Condition);
		Condition_bt_ships_1_1_suicide_node4()
		{
		}
	protected:
		virtual EBTStatus update_impl(Agent* pAgent, EBTStatus childStatus)
		{
			BEHAVIAC_UNUSED_VAR(pAgent);
			BEHAVIAC_UNUSED_VAR(childStatus);
			unsigned int opl = ((framework::GameObject*)pAgent)->_Get_Property_<framework::PROPERTY_TYPE_framework_GameObject_HP, unsigned int >();
			Agent* pAgent_opr = Agent::GetInstance("framework::WorldState", pAgent->GetContextId());
			BEHAVIAC_ASSERT(pAgent_opr);
			unsigned int opr = ((framework::WorldState*)pAgent_opr)->_Get_Property_<framework::PROPERTY_TYPE_framework_WorldState_HealthHP, unsigned int >();
			bool op = Details::Greater(opl, opr);
			return op ? BT_SUCCESS : BT_FAILURE;
		}
	};

	class BEHAVIAC_API Action_bt_ships_1_1_suicide_node6 : public Action
	{
	public:
		BEHAVIAC_DECLARE_DYNAMIC_TYPE(Action_bt_ships_1_1_suicide_node6, Action);
		Action_bt_ships_1_1_suicide_node6()
		{
		}
	protected:
		virtual EBTStatus update_impl(Agent* pAgent, EBTStatus childStatus)
		{
			BEHAVIAC_UNUSED_VAR(pAgent);
			BEHAVIAC_UNUSED_VAR(childStatus);
			bool result = ((framework::Ship*)pAgent)->_Execute_Method_<framework::METHOD_TYPE_framework_Ship_Fire, bool >();
			return ((framework::Ship*)pAgent)->framework::Ship::checkresult(result);
		}
	};

	class BEHAVIAC_API Wait_bt_ships_1_1_suicide_node7 : public Wait
	{
	public:
		BEHAVIAC_DECLARE_DYNAMIC_TYPE(Wait_bt_ships_1_1_suicide_node7, Wait);
		Wait_bt_ships_1_1_suicide_node7()
		{
			m_ignoreTimeScale = false;
		}
	protected:
		virtual float GetTime(Agent* pAgent) const
		{
			BEHAVIAC_UNUSED_VAR(pAgent);
			return 1000;
		}
	};

	class BEHAVIAC_API Assignment_bt_ships_1_1_suicide_node0 : public Assignment
	{
	public:
		BEHAVIAC_DECLARE_DYNAMIC_TYPE(Assignment_bt_ships_1_1_suicide_node0, Assignment);
		Assignment_bt_ships_1_1_suicide_node0()
		{
		}
	protected:
		virtual EBTStatus update_impl(Agent* pAgent, EBTStatus childStatus)
		{
			BEHAVIAC_UNUSED_VAR(pAgent);
			BEHAVIAC_UNUSED_VAR(childStatus);
			EBTStatus result = BT_SUCCESS;
			int opr = 10;
			BEHAVIAC_ASSERT(behaviac::MakeVariableId("par_b") == 1614475600u);
			pAgent->SetVariable("par_b", opr, 1614475600u);
			return result;
		}
	};

	class BEHAVIAC_API ReferencedBehavior_bt_ships_1_1_suicide_node8 : public ReferencedBehavior
	{
	public:
		BEHAVIAC_DECLARE_DYNAMIC_TYPE(ReferencedBehavior_bt_ships_1_1_suicide_node8, ReferencedBehavior);
		ReferencedBehavior_bt_ships_1_1_suicide_node8()
		{
			m_referencedBehaviorPath = "base/homing";
			bool result = Workspace::GetInstance()->Load(this->m_referencedBehaviorPath.c_str());
			BEHAVIAC_ASSERT(result);
		}
	protected:
	};

	BEHAVIAC_API bool Create_bt_ships_1_1_suicide(BehaviorTree* pBT)
	{
		pBT->SetClassNameString("BehaviorTree");
		pBT->SetId(-1);
		pBT->SetName("ships/1_1_suicide");
		pBT->SetIsFSM(false);
#if !defined(BEHAVIAC_RELEASE)
		pBT->SetAgentType("framework::Ship");
#endif
		// pars
		pBT->AddPar("framework::Ship", "bool", "par_a", "false");
		pBT->AddPar("framework::Ship", "int", "par_b", "0");
		pBT->AddPar("framework::Ship", "int", "par_homing_a", "0");
		// children
		{
			Selector* node1 = BEHAVIAC_NEW Selector;
			node1->SetClassNameString("Selector");
			node1->SetId(1);
#if !defined(BEHAVIAC_RELEASE)
			node1->SetAgentType("framework::Ship");
#endif
			pBT->AddChild(node1);
			{
				DecoratorLoopUntil_bt_ships_1_1_suicide_node2* node2 = BEHAVIAC_NEW DecoratorLoopUntil_bt_ships_1_1_suicide_node2;
				node2->SetClassNameString("DecoratorLoopUntil");
				node2->SetId(2);
#if !defined(BEHAVIAC_RELEASE)
				node2->SetAgentType("framework::Ship");
#endif
				node1->AddChild(node2);
				{
					Parallel_bt_ships_1_1_suicide_node3* node3 = BEHAVIAC_NEW Parallel_bt_ships_1_1_suicide_node3;
					node3->SetClassNameString("Parallel");
					node3->SetId(3);
#if !defined(BEHAVIAC_RELEASE)
					node3->SetAgentType("framework::Ship");
#endif
					node2->AddChild(node3);
					{
						Condition_bt_ships_1_1_suicide_node4* node4 = BEHAVIAC_NEW Condition_bt_ships_1_1_suicide_node4;
						node4->SetClassNameString("Condition");
						node4->SetId(4);
#if !defined(BEHAVIAC_RELEASE)
						node4->SetAgentType("framework::Ship");
#endif
						node3->AddChild(node4);
						node3->SetHasEvents(node3->HasEvents() | node4->HasEvents());
					}
					{
						Sequence* node5 = BEHAVIAC_NEW Sequence;
						node5->SetClassNameString("Sequence");
						node5->SetId(5);
#if !defined(BEHAVIAC_RELEASE)
						node5->SetAgentType("framework::Ship");
#endif
						node3->AddChild(node5);
						{
							Action_bt_ships_1_1_suicide_node6* node6 = BEHAVIAC_NEW Action_bt_ships_1_1_suicide_node6;
							node6->SetClassNameString("Action");
							node6->SetId(6);
#if !defined(BEHAVIAC_RELEASE)
							node6->SetAgentType("framework::Ship");
#endif
							node5->AddChild(node6);
							node5->SetHasEvents(node5->HasEvents() | node6->HasEvents());
						}
						{
							Wait_bt_ships_1_1_suicide_node7* node7 = BEHAVIAC_NEW Wait_bt_ships_1_1_suicide_node7;
							node7->SetClassNameString("Wait");
							node7->SetId(7);
#if !defined(BEHAVIAC_RELEASE)
							node7->SetAgentType("framework::Ship");
#endif
							node5->AddChild(node7);
							node5->SetHasEvents(node5->HasEvents() | node7->HasEvents());
						}
						node3->SetHasEvents(node3->HasEvents() | node5->HasEvents());
					}
					{
						Assignment_bt_ships_1_1_suicide_node0* node0 = BEHAVIAC_NEW Assignment_bt_ships_1_1_suicide_node0;
						node0->SetClassNameString("Assignment");
						node0->SetId(0);
#if !defined(BEHAVIAC_RELEASE)
						node0->SetAgentType("framework::Ship");
#endif
						node3->AddChild(node0);
						node3->SetHasEvents(node3->HasEvents() | node0->HasEvents());
					}
					node2->SetHasEvents(node2->HasEvents() | node3->HasEvents());
				}
				node1->SetHasEvents(node1->HasEvents() | node2->HasEvents());
			}
			{
				ReferencedBehavior_bt_ships_1_1_suicide_node8* node8 = BEHAVIAC_NEW ReferencedBehavior_bt_ships_1_1_suicide_node8;
				node8->SetClassNameString("ReferencedBehavior");
				node8->SetId(8);
#if !defined(BEHAVIAC_RELEASE)
				node8->SetAgentType("framework::Ship");
#endif
				node1->AddChild(node8);
				node1->SetHasEvents(node1->HasEvents() | node8->HasEvents());
			}
			pBT->SetHasEvents(pBT->HasEvents() | node1->HasEvents());
		}
		return true;
	}

	// Source file: ships/1_2_suicide

	class BEHAVIAC_API Parallel_bt_ships_1_2_suicide_node1 : public Parallel
	{
	public:
		BEHAVIAC_DECLARE_DYNAMIC_TYPE(Parallel_bt_ships_1_2_suicide_node1, Parallel);
		Parallel_bt_ships_1_2_suicide_node1()
		{
			m_failPolicy = FAIL_ON_ONE;
			m_succeedPolicy = SUCCEED_ON_ALL;
			m_exitPolicy = EXIT_NONE;
			m_childFinishPolicy = CHILDFINISH_LOOP;
		}
	protected:
	};

	class BEHAVIAC_API Condition_bt_ships_1_2_suicide_node3 : public Condition
	{
	public:
		BEHAVIAC_DECLARE_DYNAMIC_TYPE(Condition_bt_ships_1_2_suicide_node3, Condition);
		Condition_bt_ships_1_2_suicide_node3()
		{
		}
	protected:
		virtual EBTStatus update_impl(Agent* pAgent, EBTStatus childStatus)
		{
			BEHAVIAC_UNUSED_VAR(pAgent);
			BEHAVIAC_UNUSED_VAR(childStatus);
			unsigned int opl = ((framework::GameObject*)pAgent)->_Get_Property_<framework::PROPERTY_TYPE_framework_GameObject_HP, unsigned int >();
			Agent* pAgent_opr = Agent::GetInstance("framework::WorldState", pAgent->GetContextId());
			BEHAVIAC_ASSERT(pAgent_opr);
			unsigned int opr = ((framework::WorldState*)pAgent_opr)->_Get_Property_<framework::PROPERTY_TYPE_framework_WorldState_HealthHP, unsigned int >();
			bool op = Details::LessEqual(opl, opr);
			return op ? BT_SUCCESS : BT_FAILURE;
		}
	};

	class BEHAVIAC_API ReferencedBehavior_bt_ships_1_2_suicide_node4 : public ReferencedBehavior
	{
	public:
		BEHAVIAC_DECLARE_DYNAMIC_TYPE(ReferencedBehavior_bt_ships_1_2_suicide_node4, ReferencedBehavior);
		ReferencedBehavior_bt_ships_1_2_suicide_node4()
		{
			m_referencedBehaviorPath = "base/homing";
			bool result = Workspace::GetInstance()->Load(this->m_referencedBehaviorPath.c_str());
			BEHAVIAC_ASSERT(result);
		}
	protected:
	};

	class BEHAVIAC_API DecoratorLoop_bt_ships_1_2_suicide_node0 : public DecoratorLoop
	{
	public:
		BEHAVIAC_DECLARE_DYNAMIC_TYPE(DecoratorLoop_bt_ships_1_2_suicide_node0, DecoratorLoop);
		DecoratorLoop_bt_ships_1_2_suicide_node0()
		{
			m_bDecorateWhenChildEnds = true;
		}
	protected:
		virtual int GetCount(Agent* pAgent) const
		{
			BEHAVIAC_UNUSED_VAR(pAgent);
			return -1;
		}
	};

	class BEHAVIAC_API Action_bt_ships_1_2_suicide_node6 : public Action
	{
	public:
		BEHAVIAC_DECLARE_DYNAMIC_TYPE(Action_bt_ships_1_2_suicide_node6, Action);
		Action_bt_ships_1_2_suicide_node6()
		{
		}
	protected:
		virtual EBTStatus update_impl(Agent* pAgent, EBTStatus childStatus)
		{
			BEHAVIAC_UNUSED_VAR(pAgent);
			BEHAVIAC_UNUSED_VAR(childStatus);
			bool result = ((framework::Ship*)pAgent)->_Execute_Method_<framework::METHOD_TYPE_framework_Ship_Fire, bool >();
			return ((framework::Ship*)pAgent)->framework::Ship::checkresult(result);
		}
	};

	class BEHAVIAC_API Wait_bt_ships_1_2_suicide_node7 : public Wait
	{
	public:
		BEHAVIAC_DECLARE_DYNAMIC_TYPE(Wait_bt_ships_1_2_suicide_node7, Wait);
		Wait_bt_ships_1_2_suicide_node7()
		{
			m_ignoreTimeScale = false;
		}
	protected:
		virtual float GetTime(Agent* pAgent) const
		{
			BEHAVIAC_UNUSED_VAR(pAgent);
			return 1000;
		}
	};

	BEHAVIAC_API bool Create_bt_ships_1_2_suicide(BehaviorTree* pBT)
	{
		pBT->SetClassNameString("BehaviorTree");
		pBT->SetId(-1);
		pBT->SetName("ships/1_2_suicide");
		pBT->SetIsFSM(false);
#if !defined(BEHAVIAC_RELEASE)
		pBT->SetAgentType("framework::Ship");
#endif
		// pars
		pBT->AddPar("framework::Ship", "bool", "par_a", "false");
		pBT->AddPar("framework::Ship", "int", "par_b", "0");
		// children
		{
			Parallel_bt_ships_1_2_suicide_node1* node1 = BEHAVIAC_NEW Parallel_bt_ships_1_2_suicide_node1;
			node1->SetClassNameString("Parallel");
			node1->SetId(1);
#if !defined(BEHAVIAC_RELEASE)
			node1->SetAgentType("framework::Ship");
#endif
			pBT->AddChild(node1);
			{
				WaitforSignal* node2 = BEHAVIAC_NEW WaitforSignal;
				node2->SetClassNameString("WaitforSignal");
				node2->SetId(2);
#if !defined(BEHAVIAC_RELEASE)
				node2->SetAgentType("framework::Ship");
#endif
				node1->AddChild(node2);
				{
					Condition_bt_ships_1_2_suicide_node3* node3 = BEHAVIAC_NEW Condition_bt_ships_1_2_suicide_node3;
					node3->SetClassNameString("Condition");
					node3->SetId(3);
#if !defined(BEHAVIAC_RELEASE)
					node3->SetAgentType("framework::Ship");
#endif
					node2->SetCustomCondition(node3);
					node2->SetHasEvents(node2->HasEvents() | node3->HasEvents());
				}
				{
					ReferencedBehavior_bt_ships_1_2_suicide_node4* node4 = BEHAVIAC_NEW ReferencedBehavior_bt_ships_1_2_suicide_node4;
					node4->SetClassNameString("ReferencedBehavior");
					node4->SetId(4);
#if !defined(BEHAVIAC_RELEASE)
					node4->SetAgentType("framework::Ship");
#endif
					node2->AddChild(node4);
					node2->SetHasEvents(node2->HasEvents() | node4->HasEvents());
				}
				node1->SetHasEvents(node1->HasEvents() | node2->HasEvents());
			}
			{
				DecoratorLoop_bt_ships_1_2_suicide_node0* node0 = BEHAVIAC_NEW DecoratorLoop_bt_ships_1_2_suicide_node0;
				node0->SetClassNameString("DecoratorLoop");
				node0->SetId(0);
#if !defined(BEHAVIAC_RELEASE)
				node0->SetAgentType("framework::Ship");
#endif
				node1->AddChild(node0);
				{
					Sequence* node5 = BEHAVIAC_NEW Sequence;
					node5->SetClassNameString("Sequence");
					node5->SetId(5);
#if !defined(BEHAVIAC_RELEASE)
					node5->SetAgentType("framework::Ship");
#endif
					node0->AddChild(node5);
					{
						Action_bt_ships_1_2_suicide_node6* node6 = BEHAVIAC_NEW Action_bt_ships_1_2_suicide_node6;
						node6->SetClassNameString("Action");
						node6->SetId(6);
#if !defined(BEHAVIAC_RELEASE)
						node6->SetAgentType("framework::Ship");
#endif
						node5->AddChild(node6);
						node5->SetHasEvents(node5->HasEvents() | node6->HasEvents());
					}
					{
						Wait_bt_ships_1_2_suicide_node7* node7 = BEHAVIAC_NEW Wait_bt_ships_1_2_suicide_node7;
						node7->SetClassNameString("Wait");
						node7->SetId(7);
#if !defined(BEHAVIAC_RELEASE)
						node7->SetAgentType("framework::Ship");
#endif
						node5->AddChild(node7);
						node5->SetHasEvents(node5->HasEvents() | node7->HasEvents());
					}
					node0->SetHasEvents(node0->HasEvents() | node5->HasEvents());
				}
				node1->SetHasEvents(node1->HasEvents() | node0->HasEvents());
			}
			pBT->SetHasEvents(pBT->HasEvents() | node1->HasEvents());
		}
		return true;
	}

	// Source file: ships/1_3_suicide

	class BEHAVIAC_API Condition_bt_ships_1_3_suicide_node3 : public Condition
	{
	public:
		BEHAVIAC_DECLARE_DYNAMIC_TYPE(Condition_bt_ships_1_3_suicide_node3, Condition);
		Condition_bt_ships_1_3_suicide_node3()
		{
		}
	protected:
		virtual EBTStatus update_impl(Agent* pAgent, EBTStatus childStatus)
		{
			BEHAVIAC_UNUSED_VAR(pAgent);
			BEHAVIAC_UNUSED_VAR(childStatus);
			unsigned int opl = ((framework::GameObject*)pAgent)->_Get_Property_<framework::PROPERTY_TYPE_framework_GameObject_HP, unsigned int >();
			Agent* pAgent_opr = Agent::GetInstance("framework::WorldState", pAgent->GetContextId());
			BEHAVIAC_ASSERT(pAgent_opr);
			unsigned int opr = ((framework::WorldState*)pAgent_opr)->_Get_Property_<framework::PROPERTY_TYPE_framework_WorldState_HealthHP, unsigned int >();
			bool op = Details::LessEqual(opl, opr);
			return op ? BT_SUCCESS : BT_FAILURE;
		}
	};

	class BEHAVIAC_API ReferencedBehavior_bt_ships_1_3_suicide_node4 : public ReferencedBehavior
	{
	public:
		BEHAVIAC_DECLARE_DYNAMIC_TYPE(ReferencedBehavior_bt_ships_1_3_suicide_node4, ReferencedBehavior);
		ReferencedBehavior_bt_ships_1_3_suicide_node4()
		{
			m_referencedBehaviorPath = "base/homing";
			bool result = Workspace::GetInstance()->Load(this->m_referencedBehaviorPath.c_str());
			BEHAVIAC_ASSERT(result);
		}
	protected:
	};

	class BEHAVIAC_API DecoratorLoop_bt_ships_1_3_suicide_node0 : public DecoratorLoop
	{
	public:
		BEHAVIAC_DECLARE_DYNAMIC_TYPE(DecoratorLoop_bt_ships_1_3_suicide_node0, DecoratorLoop);
		DecoratorLoop_bt_ships_1_3_suicide_node0()
		{
			m_bDecorateWhenChildEnds = true;
		}
	protected:
		virtual int GetCount(Agent* pAgent) const
		{
			BEHAVIAC_UNUSED_VAR(pAgent);
			return -1;
		}
	};

	class BEHAVIAC_API Action_bt_ships_1_3_suicide_node8 : public Action
	{
	public:
		BEHAVIAC_DECLARE_DYNAMIC_TYPE(Action_bt_ships_1_3_suicide_node8, Action);
		Action_bt_ships_1_3_suicide_node8()
		{
		}
	protected:
		virtual EBTStatus update_impl(Agent* pAgent, EBTStatus childStatus)
		{
			BEHAVIAC_UNUSED_VAR(pAgent);
			BEHAVIAC_UNUSED_VAR(childStatus);
			bool result = ((framework::Ship*)pAgent)->_Execute_Method_<framework::METHOD_TYPE_framework_Ship_Fire, bool >();
			return ((framework::Ship*)pAgent)->framework::Ship::checkresult(result);
		}
	};

	class BEHAVIAC_API Wait_bt_ships_1_3_suicide_node9 : public Wait
	{
	public:
		BEHAVIAC_DECLARE_DYNAMIC_TYPE(Wait_bt_ships_1_3_suicide_node9, Wait);
		Wait_bt_ships_1_3_suicide_node9()
		{
			m_ignoreTimeScale = false;
		}
	protected:
		virtual float GetTime(Agent* pAgent) const
		{
			BEHAVIAC_UNUSED_VAR(pAgent);
			return 1000;
		}
	};

	BEHAVIAC_API bool Create_bt_ships_1_3_suicide(BehaviorTree* pBT)
	{
		pBT->SetClassNameString("BehaviorTree");
		pBT->SetId(-1);
		pBT->SetName("ships/1_3_suicide");
		pBT->SetIsFSM(false);
#if !defined(BEHAVIAC_RELEASE)
		pBT->SetAgentType("framework::Ship");
#endif
		// pars
		pBT->AddPar("framework::Ship", "bool", "par_a", "false");
		pBT->AddPar("framework::Ship", "int", "par_b", "0");
		// children
		{
			SelectorLoop* node1 = BEHAVIAC_NEW SelectorLoop;
			node1->SetClassNameString("SelectorLoop");
			node1->SetId(1);
#if !defined(BEHAVIAC_RELEASE)
			node1->SetAgentType("framework::Ship");
#endif
			pBT->AddChild(node1);
			{
				WithPrecondition* node2 = BEHAVIAC_NEW WithPrecondition;
				node2->SetClassNameString("WithPrecondition");
				node2->SetId(2);
#if !defined(BEHAVIAC_RELEASE)
				node2->SetAgentType("framework::Ship");
#endif
				node1->AddChild(node2);
				{
					Condition_bt_ships_1_3_suicide_node3* node3 = BEHAVIAC_NEW Condition_bt_ships_1_3_suicide_node3;
					node3->SetClassNameString("Condition");
					node3->SetId(3);
#if !defined(BEHAVIAC_RELEASE)
					node3->SetAgentType("framework::Ship");
#endif
					node2->AddChild(node3);
					node2->SetHasEvents(node2->HasEvents() | node3->HasEvents());
				}
				{
					ReferencedBehavior_bt_ships_1_3_suicide_node4* node4 = BEHAVIAC_NEW ReferencedBehavior_bt_ships_1_3_suicide_node4;
					node4->SetClassNameString("ReferencedBehavior");
					node4->SetId(4);
#if !defined(BEHAVIAC_RELEASE)
					node4->SetAgentType("framework::Ship");
#endif
					node2->AddChild(node4);
					node2->SetHasEvents(node2->HasEvents() | node4->HasEvents());
				}
				node1->SetHasEvents(node1->HasEvents() | node2->HasEvents());
			}
			{
				WithPrecondition* node5 = BEHAVIAC_NEW WithPrecondition;
				node5->SetClassNameString("WithPrecondition");
				node5->SetId(5);
#if !defined(BEHAVIAC_RELEASE)
				node5->SetAgentType("framework::Ship");
#endif
				node1->AddChild(node5);
				{
					True* node6 = BEHAVIAC_NEW True;
					node6->SetClassNameString("True");
					node6->SetId(6);
#if !defined(BEHAVIAC_RELEASE)
					node6->SetAgentType("framework::Ship");
#endif
					node5->AddChild(node6);
					node5->SetHasEvents(node5->HasEvents() | node6->HasEvents());
				}
				{
					DecoratorLoop_bt_ships_1_3_suicide_node0* node0 = BEHAVIAC_NEW DecoratorLoop_bt_ships_1_3_suicide_node0;
					node0->SetClassNameString("DecoratorLoop");
					node0->SetId(0);
#if !defined(BEHAVIAC_RELEASE)
					node0->SetAgentType("framework::Ship");
#endif
					node5->AddChild(node0);
					{
						Sequence* node7 = BEHAVIAC_NEW Sequence;
						node7->SetClassNameString("Sequence");
						node7->SetId(7);
#if !defined(BEHAVIAC_RELEASE)
						node7->SetAgentType("framework::Ship");
#endif
						node0->AddChild(node7);
						{
							Action_bt_ships_1_3_suicide_node8* node8 = BEHAVIAC_NEW Action_bt_ships_1_3_suicide_node8;
							node8->SetClassNameString("Action");
							node8->SetId(8);
#if !defined(BEHAVIAC_RELEASE)
							node8->SetAgentType("framework::Ship");
#endif
							node7->AddChild(node8);
							node7->SetHasEvents(node7->HasEvents() | node8->HasEvents());
						}
						{
							Wait_bt_ships_1_3_suicide_node9* node9 = BEHAVIAC_NEW Wait_bt_ships_1_3_suicide_node9;
							node9->SetClassNameString("Wait");
							node9->SetId(9);
#if !defined(BEHAVIAC_RELEASE)
							node9->SetAgentType("framework::Ship");
#endif
							node7->AddChild(node9);
							node7->SetHasEvents(node7->HasEvents() | node9->HasEvents());
						}
						node0->SetHasEvents(node0->HasEvents() | node7->HasEvents());
					}
					node5->SetHasEvents(node5->HasEvents() | node0->HasEvents());
				}
				node1->SetHasEvents(node1->HasEvents() | node5->HasEvents());
			}
			pBT->SetHasEvents(pBT->HasEvents() | node1->HasEvents());
		}
		return true;
	}

	// Source file: ships/2_basic_probability

	class BEHAVIAC_API Parallel_bt_ships_2_basic_probability_node1 : public Parallel
	{
	public:
		BEHAVIAC_DECLARE_DYNAMIC_TYPE(Parallel_bt_ships_2_basic_probability_node1, Parallel);
		Parallel_bt_ships_2_basic_probability_node1()
		{
			m_failPolicy = FAIL_ON_ONE;
			m_succeedPolicy = SUCCEED_ON_ALL;
			m_exitPolicy = EXIT_NONE;
			m_childFinishPolicy = CHILDFINISH_LOOP;
		}
	protected:
	};

	class BEHAVIAC_API DecoratorLoop_bt_ships_2_basic_probability_node2 : public DecoratorLoop
	{
	public:
		BEHAVIAC_DECLARE_DYNAMIC_TYPE(DecoratorLoop_bt_ships_2_basic_probability_node2, DecoratorLoop);
		DecoratorLoop_bt_ships_2_basic_probability_node2()
		{
			m_bDecorateWhenChildEnds = true;
		}
	protected:
		virtual int GetCount(Agent* pAgent) const
		{
			BEHAVIAC_UNUSED_VAR(pAgent);
			return -1;
		}
	};

	class BEHAVIAC_API SelectorProbability_bt_ships_2_basic_probability_node3 : public SelectorProbability
	{
	public:
		BEHAVIAC_DECLARE_DYNAMIC_TYPE(SelectorProbability_bt_ships_2_basic_probability_node3, SelectorProbability);
		SelectorProbability_bt_ships_2_basic_probability_node3()
		{
		}
	public:
		void Initialize(const char* method)
		{
			this->m_method = Action::LoadMethod(method);
		}
	};

	class BEHAVIAC_API DecoratorWeight_bt_ships_2_basic_probability_node4 : public DecoratorWeight
	{
	public:
		BEHAVIAC_DECLARE_DYNAMIC_TYPE(DecoratorWeight_bt_ships_2_basic_probability_node4, DecoratorWeight);
		DecoratorWeight_bt_ships_2_basic_probability_node4()
		{
			m_bDecorateWhenChildEnds = false;
		}
	protected:
		virtual int GetWeight(Agent* pAgent) const
		{
			BEHAVIAC_UNUSED_VAR(pAgent);
			return 4;
		}
	};

	class BEHAVIAC_API Condition_bt_ships_2_basic_probability_node6 : public Condition
	{
	public:
		BEHAVIAC_DECLARE_DYNAMIC_TYPE(Condition_bt_ships_2_basic_probability_node6, Condition);
		Condition_bt_ships_2_basic_probability_node6()
		{
		}
	protected:
		virtual EBTStatus update_impl(Agent* pAgent, EBTStatus childStatus)
		{
			BEHAVIAC_UNUSED_VAR(pAgent);
			BEHAVIAC_UNUSED_VAR(childStatus);
			bool opl = ((framework::GameObject*)pAgent)->_Execute_Method_<framework::METHOD_TYPE_framework_GameObject_alignedWithPlayer, bool >();
			bool opr = true;
			bool op = Details::Equal(opl, opr);
			return op ? BT_SUCCESS : BT_FAILURE;
		}
	};

	class BEHAVIAC_API Action_bt_ships_2_basic_probability_node7 : public Action
	{
	public:
		BEHAVIAC_DECLARE_DYNAMIC_TYPE(Action_bt_ships_2_basic_probability_node7, Action);
		Action_bt_ships_2_basic_probability_node7()
		{
		}
	protected:
		virtual EBTStatus update_impl(Agent* pAgent, EBTStatus childStatus)
		{
			BEHAVIAC_UNUSED_VAR(pAgent);
			BEHAVIAC_UNUSED_VAR(childStatus);
			bool result = ((framework::Ship*)pAgent)->_Execute_Method_<framework::METHOD_TYPE_framework_Ship_Fire, bool >();
			return ((framework::Ship*)pAgent)->framework::Ship::checkresult(result);
		}
	};

	class BEHAVIAC_API Wait_bt_ships_2_basic_probability_node0 : public Wait
	{
	public:
		BEHAVIAC_DECLARE_DYNAMIC_TYPE(Wait_bt_ships_2_basic_probability_node0, Wait);
		Wait_bt_ships_2_basic_probability_node0()
		{
			m_ignoreTimeScale = false;
		}
	protected:
		virtual float GetTime(Agent* pAgent) const
		{
			BEHAVIAC_UNUSED_VAR(pAgent);
			return 500;
		}
	};

	class BEHAVIAC_API DecoratorWeight_bt_ships_2_basic_probability_node9 : public DecoratorWeight
	{
	public:
		BEHAVIAC_DECLARE_DYNAMIC_TYPE(DecoratorWeight_bt_ships_2_basic_probability_node9, DecoratorWeight);
		DecoratorWeight_bt_ships_2_basic_probability_node9()
		{
			m_bDecorateWhenChildEnds = false;
		}
	protected:
		virtual int GetWeight(Agent* pAgent) const
		{
			BEHAVIAC_UNUSED_VAR(pAgent);
			return 1;
		}
	};

	class BEHAVIAC_API Condition_bt_ships_2_basic_probability_node11 : public Condition
	{
	public:
		BEHAVIAC_DECLARE_DYNAMIC_TYPE(Condition_bt_ships_2_basic_probability_node11, Condition);
		Condition_bt_ships_2_basic_probability_node11()
		{
		}
	protected:
		virtual EBTStatus update_impl(Agent* pAgent, EBTStatus childStatus)
		{
			BEHAVIAC_UNUSED_VAR(pAgent);
			BEHAVIAC_UNUSED_VAR(childStatus);
			float opl = ((framework::Ship*)pAgent)->_Execute_Method_<framework::METHOD_TYPE_framework_Ship_getXPosition, float >();
			float opr = 200;
			bool op = Details::Less(opl, opr);
			return op ? BT_SUCCESS : BT_FAILURE;
		}
	};

	class BEHAVIAC_API Action_bt_ships_2_basic_probability_node12 : public Action
	{
	public:
		BEHAVIAC_DECLARE_DYNAMIC_TYPE(Action_bt_ships_2_basic_probability_node12, Action);
		Action_bt_ships_2_basic_probability_node12()
		{
			method_p0 = 300;
			method_p1 = 50;
			method_p2 = 50;
		}
	protected:
		virtual EBTStatus update_impl(Agent* pAgent, EBTStatus childStatus)
		{
			BEHAVIAC_UNUSED_VAR(pAgent);
			BEHAVIAC_UNUSED_VAR(childStatus);
			bool result = ((framework::Ship*)pAgent)->_Execute_Method_<framework::METHOD_TYPE_framework_Ship_GotoPoint, bool, float, float, float >(method_p0, method_p1, method_p2);
			return ((framework::Ship*)pAgent)->framework::Ship::checkresult(result);
		}
		float method_p0;
		float method_p1;
		float method_p2;
	};

	class BEHAVIAC_API Wait_bt_ships_2_basic_probability_node8 : public Wait
	{
	public:
		BEHAVIAC_DECLARE_DYNAMIC_TYPE(Wait_bt_ships_2_basic_probability_node8, Wait);
		Wait_bt_ships_2_basic_probability_node8()
		{
			m_ignoreTimeScale = false;
		}
	protected:
		virtual float GetTime(Agent* pAgent) const
		{
			BEHAVIAC_UNUSED_VAR(pAgent);
			return 200;
		}
	};

	class BEHAVIAC_API DecoratorWeight_bt_ships_2_basic_probability_node14 : public DecoratorWeight
	{
	public:
		BEHAVIAC_DECLARE_DYNAMIC_TYPE(DecoratorWeight_bt_ships_2_basic_probability_node14, DecoratorWeight);
		DecoratorWeight_bt_ships_2_basic_probability_node14()
		{
			m_bDecorateWhenChildEnds = false;
		}
	protected:
		virtual int GetWeight(Agent* pAgent) const
		{
			BEHAVIAC_UNUSED_VAR(pAgent);
			return 1;
		}
	};

	class BEHAVIAC_API Condition_bt_ships_2_basic_probability_node16 : public Condition
	{
	public:
		BEHAVIAC_DECLARE_DYNAMIC_TYPE(Condition_bt_ships_2_basic_probability_node16, Condition);
		Condition_bt_ships_2_basic_probability_node16()
		{
		}
	protected:
		virtual EBTStatus update_impl(Agent* pAgent, EBTStatus childStatus)
		{
			BEHAVIAC_UNUSED_VAR(pAgent);
			BEHAVIAC_UNUSED_VAR(childStatus);
			float opl = ((framework::Ship*)pAgent)->_Execute_Method_<framework::METHOD_TYPE_framework_Ship_getXPosition, float >();
			float opr = 200;
			bool op = Details::Greater(opl, opr);
			return op ? BT_SUCCESS : BT_FAILURE;
		}
	};

	class BEHAVIAC_API Action_bt_ships_2_basic_probability_node17 : public Action
	{
	public:
		BEHAVIAC_DECLARE_DYNAMIC_TYPE(Action_bt_ships_2_basic_probability_node17, Action);
		Action_bt_ships_2_basic_probability_node17()
		{
			method_p0 = 100;
			method_p1 = 50;
			method_p2 = 50;
		}
	protected:
		virtual EBTStatus update_impl(Agent* pAgent, EBTStatus childStatus)
		{
			BEHAVIAC_UNUSED_VAR(pAgent);
			BEHAVIAC_UNUSED_VAR(childStatus);
			bool result = ((framework::Ship*)pAgent)->_Execute_Method_<framework::METHOD_TYPE_framework_Ship_GotoPoint, bool, float, float, float >(method_p0, method_p1, method_p2);
			return ((framework::Ship*)pAgent)->framework::Ship::checkresult(result);
		}
		float method_p0;
		float method_p1;
		float method_p2;
	};

	class BEHAVIAC_API Wait_bt_ships_2_basic_probability_node21 : public Wait
	{
	public:
		BEHAVIAC_DECLARE_DYNAMIC_TYPE(Wait_bt_ships_2_basic_probability_node21, Wait);
		Wait_bt_ships_2_basic_probability_node21()
		{
			m_ignoreTimeScale = false;
		}
	protected:
		virtual float GetTime(Agent* pAgent) const
		{
			BEHAVIAC_UNUSED_VAR(pAgent);
			return 200;
		}
	};

	class BEHAVIAC_API DecoratorLoop_bt_ships_2_basic_probability_node19 : public DecoratorLoop
	{
	public:
		BEHAVIAC_DECLARE_DYNAMIC_TYPE(DecoratorLoop_bt_ships_2_basic_probability_node19, DecoratorLoop);
		DecoratorLoop_bt_ships_2_basic_probability_node19()
		{
			m_bDecorateWhenChildEnds = true;
		}
	protected:
		virtual int GetCount(Agent* pAgent) const
		{
			BEHAVIAC_UNUSED_VAR(pAgent);
			return -1;
		}
	};

	class BEHAVIAC_API Action_bt_ships_2_basic_probability_node20 : public Action
	{
	public:
		BEHAVIAC_DECLARE_DYNAMIC_TYPE(Action_bt_ships_2_basic_probability_node20, Action);
		Action_bt_ships_2_basic_probability_node20()
		{
			method_p0 = 3;
		}
	protected:
		virtual EBTStatus update_impl(Agent* pAgent, EBTStatus childStatus)
		{
			BEHAVIAC_UNUSED_VAR(pAgent);
			BEHAVIAC_UNUSED_VAR(childStatus);
			((framework::GameObject*)pAgent)->_Execute_Method_<framework::METHOD_TYPE_framework_GameObject_TurnTowardsTarget, int, float >(method_p0);
			return BT_RUNNING;
		}
		float method_p0;
	};

	BEHAVIAC_API bool Create_bt_ships_2_basic_probability(BehaviorTree* pBT)
	{
		pBT->SetClassNameString("BehaviorTree");
		pBT->SetId(-1);
		pBT->SetName("ships/2_basic_probability");
		pBT->SetIsFSM(false);
#if !defined(BEHAVIAC_RELEASE)
		pBT->SetAgentType("framework::Ship");
#endif
		// pars
		pBT->AddPar("framework::Ship", "bool", "par_a", "false");
		pBT->AddPar("framework::Ship", "int", "par_b", "0");
		// children
		{
			Parallel_bt_ships_2_basic_probability_node1* node1 = BEHAVIAC_NEW Parallel_bt_ships_2_basic_probability_node1;
			node1->SetClassNameString("Parallel");
			node1->SetId(1);
#if !defined(BEHAVIAC_RELEASE)
			node1->SetAgentType("framework::Ship");
#endif
			pBT->AddChild(node1);
			{
				DecoratorLoop_bt_ships_2_basic_probability_node2* node2 = BEHAVIAC_NEW DecoratorLoop_bt_ships_2_basic_probability_node2;
				node2->SetClassNameString("DecoratorLoop");
				node2->SetId(2);
#if !defined(BEHAVIAC_RELEASE)
				node2->SetAgentType("framework::Ship");
#endif
				node1->AddChild(node2);
				{
					SelectorProbability_bt_ships_2_basic_probability_node3* node3 = BEHAVIAC_NEW SelectorProbability_bt_ships_2_basic_probability_node3;
					node3->SetClassNameString("SelectorProbability");
					node3->SetId(3);
#if !defined(BEHAVIAC_RELEASE)
					node3->SetAgentType("framework::Ship");
#endif
					node2->AddChild(node3);
					{
						DecoratorWeight_bt_ships_2_basic_probability_node4* node4 = BEHAVIAC_NEW DecoratorWeight_bt_ships_2_basic_probability_node4;
						node4->SetClassNameString("DecoratorWeight");
						node4->SetId(4);
#if !defined(BEHAVIAC_RELEASE)
						node4->SetAgentType("framework::Ship");
#endif
						node3->AddChild(node4);
						{
							Sequence* node5 = BEHAVIAC_NEW Sequence;
							node5->SetClassNameString("Sequence");
							node5->SetId(5);
#if !defined(BEHAVIAC_RELEASE)
							node5->SetAgentType("framework::Ship");
#endif
							node4->AddChild(node5);
							{
								Condition_bt_ships_2_basic_probability_node6* node6 = BEHAVIAC_NEW Condition_bt_ships_2_basic_probability_node6;
								node6->SetClassNameString("Condition");
								node6->SetId(6);
#if !defined(BEHAVIAC_RELEASE)
								node6->SetAgentType("framework::Ship");
#endif
								node5->AddChild(node6);
								node5->SetHasEvents(node5->HasEvents() | node6->HasEvents());
							}
							{
								Action_bt_ships_2_basic_probability_node7* node7 = BEHAVIAC_NEW Action_bt_ships_2_basic_probability_node7;
								node7->SetClassNameString("Action");
								node7->SetId(7);
#if !defined(BEHAVIAC_RELEASE)
								node7->SetAgentType("framework::Ship");
#endif
								node5->AddChild(node7);
								node5->SetHasEvents(node5->HasEvents() | node7->HasEvents());
							}
							{
								Wait_bt_ships_2_basic_probability_node0* node0 = BEHAVIAC_NEW Wait_bt_ships_2_basic_probability_node0;
								node0->SetClassNameString("Wait");
								node0->SetId(0);
#if !defined(BEHAVIAC_RELEASE)
								node0->SetAgentType("framework::Ship");
#endif
								node5->AddChild(node0);
								node5->SetHasEvents(node5->HasEvents() | node0->HasEvents());
							}
							node4->SetHasEvents(node4->HasEvents() | node5->HasEvents());
						}
						node3->SetHasEvents(node3->HasEvents() | node4->HasEvents());
					}
					{
						DecoratorWeight_bt_ships_2_basic_probability_node9* node9 = BEHAVIAC_NEW DecoratorWeight_bt_ships_2_basic_probability_node9;
						node9->SetClassNameString("DecoratorWeight");
						node9->SetId(9);
#if !defined(BEHAVIAC_RELEASE)
						node9->SetAgentType("framework::Ship");
#endif
						node3->AddChild(node9);
						{
							Sequence* node10 = BEHAVIAC_NEW Sequence;
							node10->SetClassNameString("Sequence");
							node10->SetId(10);
#if !defined(BEHAVIAC_RELEASE)
							node10->SetAgentType("framework::Ship");
#endif
							node9->AddChild(node10);
							{
								Condition_bt_ships_2_basic_probability_node11* node11 = BEHAVIAC_NEW Condition_bt_ships_2_basic_probability_node11;
								node11->SetClassNameString("Condition");
								node11->SetId(11);
#if !defined(BEHAVIAC_RELEASE)
								node11->SetAgentType("framework::Ship");
#endif
								node10->AddChild(node11);
								node10->SetHasEvents(node10->HasEvents() | node11->HasEvents());
							}
							{
								Action_bt_ships_2_basic_probability_node12* node12 = BEHAVIAC_NEW Action_bt_ships_2_basic_probability_node12;
								node12->SetClassNameString("Action");
								node12->SetId(12);
#if !defined(BEHAVIAC_RELEASE)
								node12->SetAgentType("framework::Ship");
#endif
								node10->AddChild(node12);
								node10->SetHasEvents(node10->HasEvents() | node12->HasEvents());
							}
							{
								Wait_bt_ships_2_basic_probability_node8* node8 = BEHAVIAC_NEW Wait_bt_ships_2_basic_probability_node8;
								node8->SetClassNameString("Wait");
								node8->SetId(8);
#if !defined(BEHAVIAC_RELEASE)
								node8->SetAgentType("framework::Ship");
#endif
								node10->AddChild(node8);
								node10->SetHasEvents(node10->HasEvents() | node8->HasEvents());
							}
							node9->SetHasEvents(node9->HasEvents() | node10->HasEvents());
						}
						node3->SetHasEvents(node3->HasEvents() | node9->HasEvents());
					}
					{
						DecoratorWeight_bt_ships_2_basic_probability_node14* node14 = BEHAVIAC_NEW DecoratorWeight_bt_ships_2_basic_probability_node14;
						node14->SetClassNameString("DecoratorWeight");
						node14->SetId(14);
#if !defined(BEHAVIAC_RELEASE)
						node14->SetAgentType("framework::Ship");
#endif
						node3->AddChild(node14);
						{
							Sequence* node15 = BEHAVIAC_NEW Sequence;
							node15->SetClassNameString("Sequence");
							node15->SetId(15);
#if !defined(BEHAVIAC_RELEASE)
							node15->SetAgentType("framework::Ship");
#endif
							node14->AddChild(node15);
							{
								Condition_bt_ships_2_basic_probability_node16* node16 = BEHAVIAC_NEW Condition_bt_ships_2_basic_probability_node16;
								node16->SetClassNameString("Condition");
								node16->SetId(16);
#if !defined(BEHAVIAC_RELEASE)
								node16->SetAgentType("framework::Ship");
#endif
								node15->AddChild(node16);
								node15->SetHasEvents(node15->HasEvents() | node16->HasEvents());
							}
							{
								Action_bt_ships_2_basic_probability_node17* node17 = BEHAVIAC_NEW Action_bt_ships_2_basic_probability_node17;
								node17->SetClassNameString("Action");
								node17->SetId(17);
#if !defined(BEHAVIAC_RELEASE)
								node17->SetAgentType("framework::Ship");
#endif
								node15->AddChild(node17);
								node15->SetHasEvents(node15->HasEvents() | node17->HasEvents());
							}
							{
								Wait_bt_ships_2_basic_probability_node21* node21 = BEHAVIAC_NEW Wait_bt_ships_2_basic_probability_node21;
								node21->SetClassNameString("Wait");
								node21->SetId(21);
#if !defined(BEHAVIAC_RELEASE)
								node21->SetAgentType("framework::Ship");
#endif
								node15->AddChild(node21);
								node15->SetHasEvents(node15->HasEvents() | node21->HasEvents());
							}
							node14->SetHasEvents(node14->HasEvents() | node15->HasEvents());
						}
						node3->SetHasEvents(node3->HasEvents() | node14->HasEvents());
					}
					node2->SetHasEvents(node2->HasEvents() | node3->HasEvents());
				}
				node1->SetHasEvents(node1->HasEvents() | node2->HasEvents());
			}
			{
				DecoratorLoop_bt_ships_2_basic_probability_node19* node19 = BEHAVIAC_NEW DecoratorLoop_bt_ships_2_basic_probability_node19;
				node19->SetClassNameString("DecoratorLoop");
				node19->SetId(19);
#if !defined(BEHAVIAC_RELEASE)
				node19->SetAgentType("framework::Ship");
#endif
				node1->AddChild(node19);
				{
					Action_bt_ships_2_basic_probability_node20* node20 = BEHAVIAC_NEW Action_bt_ships_2_basic_probability_node20;
					node20->SetClassNameString("Action");
					node20->SetId(20);
#if !defined(BEHAVIAC_RELEASE)
					node20->SetAgentType("framework::Ship");
#endif
					node19->AddChild(node20);
					node19->SetHasEvents(node19->HasEvents() | node20->HasEvents());
				}
				node1->SetHasEvents(node1->HasEvents() | node19->HasEvents());
			}
			pBT->SetHasEvents(pBT->HasEvents() | node1->HasEvents());
		}
		return true;
	}

	// Source file: ships/3_basic_move

	class BEHAVIAC_API Parallel_bt_ships_3_basic_move_node1 : public Parallel
	{
	public:
		BEHAVIAC_DECLARE_DYNAMIC_TYPE(Parallel_bt_ships_3_basic_move_node1, Parallel);
		Parallel_bt_ships_3_basic_move_node1()
		{
			m_failPolicy = FAIL_ON_ONE;
			m_succeedPolicy = SUCCEED_ON_ALL;
			m_exitPolicy = EXIT_NONE;
			m_childFinishPolicy = CHILDFINISH_LOOP;
		}
	protected:
	};

	class BEHAVIAC_API DecoratorLoop_bt_ships_3_basic_move_node2 : public DecoratorLoop
	{
	public:
		BEHAVIAC_DECLARE_DYNAMIC_TYPE(DecoratorLoop_bt_ships_3_basic_move_node2, DecoratorLoop);
		DecoratorLoop_bt_ships_3_basic_move_node2()
		{
			m_bDecorateWhenChildEnds = true;
		}
	protected:
		virtual int GetCount(Agent* pAgent) const
		{
			BEHAVIAC_UNUSED_VAR(pAgent);
			return -1;
		}
	};

	class BEHAVIAC_API Condition_bt_ships_3_basic_move_node4 : public Condition
	{
	public:
		BEHAVIAC_DECLARE_DYNAMIC_TYPE(Condition_bt_ships_3_basic_move_node4, Condition);
		Condition_bt_ships_3_basic_move_node4()
		{
		}
	protected:
		virtual EBTStatus update_impl(Agent* pAgent, EBTStatus childStatus)
		{
			BEHAVIAC_UNUSED_VAR(pAgent);
			BEHAVIAC_UNUSED_VAR(childStatus);
			bool opl = ((framework::GameObject*)pAgent)->_Execute_Method_<framework::METHOD_TYPE_framework_GameObject_alignedWithPlayer, bool >();
			bool opr = true;
			bool op = Details::Equal(opl, opr);
			return op ? BT_SUCCESS : BT_FAILURE;
		}
	};

	class BEHAVIAC_API Action_bt_ships_3_basic_move_node5 : public Action
	{
	public:
		BEHAVIAC_DECLARE_DYNAMIC_TYPE(Action_bt_ships_3_basic_move_node5, Action);
		Action_bt_ships_3_basic_move_node5()
		{
		}
	protected:
		virtual EBTStatus update_impl(Agent* pAgent, EBTStatus childStatus)
		{
			BEHAVIAC_UNUSED_VAR(pAgent);
			BEHAVIAC_UNUSED_VAR(childStatus);
			bool result = ((framework::Ship*)pAgent)->_Execute_Method_<framework::METHOD_TYPE_framework_Ship_Fire, bool >();
			return ((framework::Ship*)pAgent)->framework::Ship::checkresult(result);
		}
	};

	class BEHAVIAC_API Wait_bt_ships_3_basic_move_node0 : public Wait
	{
	public:
		BEHAVIAC_DECLARE_DYNAMIC_TYPE(Wait_bt_ships_3_basic_move_node0, Wait);
		Wait_bt_ships_3_basic_move_node0()
		{
			m_ignoreTimeScale = false;
		}
	protected:
		virtual float GetTime(Agent* pAgent) const
		{
			BEHAVIAC_UNUSED_VAR(pAgent);
			return 500;
		}
	};

	class BEHAVIAC_API Action_bt_ships_3_basic_move_node7 : public Action
	{
	public:
		BEHAVIAC_DECLARE_DYNAMIC_TYPE(Action_bt_ships_3_basic_move_node7, Action);
		Action_bt_ships_3_basic_move_node7()
		{
			method_p0 = 5;
		}
	protected:
		virtual EBTStatus update_impl(Agent* pAgent, EBTStatus childStatus)
		{
			BEHAVIAC_UNUSED_VAR(pAgent);
			BEHAVIAC_UNUSED_VAR(childStatus);
			((framework::GameObject*)pAgent)->_Execute_Method_<framework::METHOD_TYPE_framework_GameObject_TurnTowardsTarget, int, float >(method_p0);
			return BT_RUNNING;
		}
		float method_p0;
	};

	class BEHAVIAC_API DecoratorLoop_bt_ships_3_basic_move_node8 : public DecoratorLoop
	{
	public:
		BEHAVIAC_DECLARE_DYNAMIC_TYPE(DecoratorLoop_bt_ships_3_basic_move_node8, DecoratorLoop);
		DecoratorLoop_bt_ships_3_basic_move_node8()
		{
			m_bDecorateWhenChildEnds = true;
		}
	protected:
		virtual int GetCount(Agent* pAgent) const
		{
			BEHAVIAC_UNUSED_VAR(pAgent);
			return -1;
		}
	};

	class BEHAVIAC_API Condition_bt_ships_3_basic_move_node10 : public Condition
	{
	public:
		BEHAVIAC_DECLARE_DYNAMIC_TYPE(Condition_bt_ships_3_basic_move_node10, Condition);
		Condition_bt_ships_3_basic_move_node10()
		{
		}
	protected:
		virtual EBTStatus update_impl(Agent* pAgent, EBTStatus childStatus)
		{
			BEHAVIAC_UNUSED_VAR(pAgent);
			BEHAVIAC_UNUSED_VAR(childStatus);
			bool opl = ((framework::GameObject*)pAgent)->_Execute_Method_<framework::METHOD_TYPE_framework_GameObject_playerIsAligned, bool >();
			bool opr = true;
			bool op = Details::Equal(opl, opr);
			return op ? BT_SUCCESS : BT_FAILURE;
		}
	};

	class BEHAVIAC_API SelectorProbability_bt_ships_3_basic_move_node11 : public SelectorProbability
	{
	public:
		BEHAVIAC_DECLARE_DYNAMIC_TYPE(SelectorProbability_bt_ships_3_basic_move_node11, SelectorProbability);
		SelectorProbability_bt_ships_3_basic_move_node11()
		{
		}
	public:
		void Initialize(const char* method)
		{
			this->m_method = Action::LoadMethod(method);
		}
	};

	class BEHAVIAC_API DecoratorWeight_bt_ships_3_basic_move_node12 : public DecoratorWeight
	{
	public:
		BEHAVIAC_DECLARE_DYNAMIC_TYPE(DecoratorWeight_bt_ships_3_basic_move_node12, DecoratorWeight);
		DecoratorWeight_bt_ships_3_basic_move_node12()
		{
			m_bDecorateWhenChildEnds = false;
		}
	protected:
		virtual int GetWeight(Agent* pAgent) const
		{
			BEHAVIAC_UNUSED_VAR(pAgent);
			return 1;
		}
	};

	class BEHAVIAC_API Action_bt_ships_3_basic_move_node13 : public Action
	{
	public:
		BEHAVIAC_DECLARE_DYNAMIC_TYPE(Action_bt_ships_3_basic_move_node13, Action);
		Action_bt_ships_3_basic_move_node13()
		{
			method_p0 = -100;
			method_p1 = 0;
			method_p2 = 0.2f;
		}
	protected:
		virtual EBTStatus update_impl(Agent* pAgent, EBTStatus childStatus)
		{
			BEHAVIAC_UNUSED_VAR(pAgent);
			BEHAVIAC_UNUSED_VAR(childStatus);
			bool result = ((framework::Ship*)pAgent)->_Execute_Method_<framework::METHOD_TYPE_framework_Ship_GotoPointRelative, bool, float, float, float >(method_p0, method_p1, method_p2);
			return ((framework::Ship*)pAgent)->framework::Ship::checkresult(result);
		}
		float method_p0;
		float method_p1;
		float method_p2;
	};

	class BEHAVIAC_API DecoratorWeight_bt_ships_3_basic_move_node14 : public DecoratorWeight
	{
	public:
		BEHAVIAC_DECLARE_DYNAMIC_TYPE(DecoratorWeight_bt_ships_3_basic_move_node14, DecoratorWeight);
		DecoratorWeight_bt_ships_3_basic_move_node14()
		{
			m_bDecorateWhenChildEnds = false;
		}
	protected:
		virtual int GetWeight(Agent* pAgent) const
		{
			BEHAVIAC_UNUSED_VAR(pAgent);
			return 1;
		}
	};

	class BEHAVIAC_API Action_bt_ships_3_basic_move_node15 : public Action
	{
	public:
		BEHAVIAC_DECLARE_DYNAMIC_TYPE(Action_bt_ships_3_basic_move_node15, Action);
		Action_bt_ships_3_basic_move_node15()
		{
			method_p0 = 100;
			method_p1 = 0;
			method_p2 = 0.2f;
		}
	protected:
		virtual EBTStatus update_impl(Agent* pAgent, EBTStatus childStatus)
		{
			BEHAVIAC_UNUSED_VAR(pAgent);
			BEHAVIAC_UNUSED_VAR(childStatus);
			bool result = ((framework::Ship*)pAgent)->_Execute_Method_<framework::METHOD_TYPE_framework_Ship_GotoPointRelative, bool, float, float, float >(method_p0, method_p1, method_p2);
			return ((framework::Ship*)pAgent)->framework::Ship::checkresult(result);
		}
		float method_p0;
		float method_p1;
		float method_p2;
	};

	BEHAVIAC_API bool Create_bt_ships_3_basic_move(BehaviorTree* pBT)
	{
		pBT->SetClassNameString("BehaviorTree");
		pBT->SetId(-1);
		pBT->SetName("ships/3_basic_move");
		pBT->SetIsFSM(false);
#if !defined(BEHAVIAC_RELEASE)
		pBT->SetAgentType("framework::Ship");
#endif
		// pars
		pBT->AddPar("framework::Ship", "bool", "par_a", "false");
		pBT->AddPar("framework::Ship", "int", "par_b", "0");
		// children
		{
			Parallel_bt_ships_3_basic_move_node1* node1 = BEHAVIAC_NEW Parallel_bt_ships_3_basic_move_node1;
			node1->SetClassNameString("Parallel");
			node1->SetId(1);
#if !defined(BEHAVIAC_RELEASE)
			node1->SetAgentType("framework::Ship");
#endif
			pBT->AddChild(node1);
			{
				DecoratorLoop_bt_ships_3_basic_move_node2* node2 = BEHAVIAC_NEW DecoratorLoop_bt_ships_3_basic_move_node2;
				node2->SetClassNameString("DecoratorLoop");
				node2->SetId(2);
#if !defined(BEHAVIAC_RELEASE)
				node2->SetAgentType("framework::Ship");
#endif
				node1->AddChild(node2);
				{
					Sequence* node3 = BEHAVIAC_NEW Sequence;
					node3->SetClassNameString("Sequence");
					node3->SetId(3);
#if !defined(BEHAVIAC_RELEASE)
					node3->SetAgentType("framework::Ship");
#endif
					node2->AddChild(node3);
					{
						Condition_bt_ships_3_basic_move_node4* node4 = BEHAVIAC_NEW Condition_bt_ships_3_basic_move_node4;
						node4->SetClassNameString("Condition");
						node4->SetId(4);
#if !defined(BEHAVIAC_RELEASE)
						node4->SetAgentType("framework::Ship");
#endif
						node3->AddChild(node4);
						node3->SetHasEvents(node3->HasEvents() | node4->HasEvents());
					}
					{
						Action_bt_ships_3_basic_move_node5* node5 = BEHAVIAC_NEW Action_bt_ships_3_basic_move_node5;
						node5->SetClassNameString("Action");
						node5->SetId(5);
#if !defined(BEHAVIAC_RELEASE)
						node5->SetAgentType("framework::Ship");
#endif
						node3->AddChild(node5);
						node3->SetHasEvents(node3->HasEvents() | node5->HasEvents());
					}
					{
						Wait_bt_ships_3_basic_move_node0* node0 = BEHAVIAC_NEW Wait_bt_ships_3_basic_move_node0;
						node0->SetClassNameString("Wait");
						node0->SetId(0);
#if !defined(BEHAVIAC_RELEASE)
						node0->SetAgentType("framework::Ship");
#endif
						node3->AddChild(node0);
						node3->SetHasEvents(node3->HasEvents() | node0->HasEvents());
					}
					node2->SetHasEvents(node2->HasEvents() | node3->HasEvents());
				}
				node1->SetHasEvents(node1->HasEvents() | node2->HasEvents());
			}
			{
				Action_bt_ships_3_basic_move_node7* node7 = BEHAVIAC_NEW Action_bt_ships_3_basic_move_node7;
				node7->SetClassNameString("Action");
				node7->SetId(7);
#if !defined(BEHAVIAC_RELEASE)
				node7->SetAgentType("framework::Ship");
#endif
				node1->AddChild(node7);
				node1->SetHasEvents(node1->HasEvents() | node7->HasEvents());
			}
			{
				DecoratorLoop_bt_ships_3_basic_move_node8* node8 = BEHAVIAC_NEW DecoratorLoop_bt_ships_3_basic_move_node8;
				node8->SetClassNameString("DecoratorLoop");
				node8->SetId(8);
#if !defined(BEHAVIAC_RELEASE)
				node8->SetAgentType("framework::Ship");
#endif
				node1->AddChild(node8);
				{
					Sequence* node9 = BEHAVIAC_NEW Sequence;
					node9->SetClassNameString("Sequence");
					node9->SetId(9);
#if !defined(BEHAVIAC_RELEASE)
					node9->SetAgentType("framework::Ship");
#endif
					node8->AddChild(node9);
					{
						Condition_bt_ships_3_basic_move_node10* node10 = BEHAVIAC_NEW Condition_bt_ships_3_basic_move_node10;
						node10->SetClassNameString("Condition");
						node10->SetId(10);
#if !defined(BEHAVIAC_RELEASE)
						node10->SetAgentType("framework::Ship");
#endif
						node9->AddChild(node10);
						node9->SetHasEvents(node9->HasEvents() | node10->HasEvents());
					}
					{
						SelectorProbability_bt_ships_3_basic_move_node11* node11 = BEHAVIAC_NEW SelectorProbability_bt_ships_3_basic_move_node11;
						node11->SetClassNameString("SelectorProbability");
						node11->SetId(11);
#if !defined(BEHAVIAC_RELEASE)
						node11->SetAgentType("framework::Ship");
#endif
						node9->AddChild(node11);
						{
							DecoratorWeight_bt_ships_3_basic_move_node12* node12 = BEHAVIAC_NEW DecoratorWeight_bt_ships_3_basic_move_node12;
							node12->SetClassNameString("DecoratorWeight");
							node12->SetId(12);
#if !defined(BEHAVIAC_RELEASE)
							node12->SetAgentType("framework::Ship");
#endif
							node11->AddChild(node12);
							{
								Action_bt_ships_3_basic_move_node13* node13 = BEHAVIAC_NEW Action_bt_ships_3_basic_move_node13;
								node13->SetClassNameString("Action");
								node13->SetId(13);
#if !defined(BEHAVIAC_RELEASE)
								node13->SetAgentType("framework::Ship");
#endif
								node12->AddChild(node13);
								node12->SetHasEvents(node12->HasEvents() | node13->HasEvents());
							}
							node11->SetHasEvents(node11->HasEvents() | node12->HasEvents());
						}
						{
							DecoratorWeight_bt_ships_3_basic_move_node14* node14 = BEHAVIAC_NEW DecoratorWeight_bt_ships_3_basic_move_node14;
							node14->SetClassNameString("DecoratorWeight");
							node14->SetId(14);
#if !defined(BEHAVIAC_RELEASE)
							node14->SetAgentType("framework::Ship");
#endif
							node11->AddChild(node14);
							{
								Action_bt_ships_3_basic_move_node15* node15 = BEHAVIAC_NEW Action_bt_ships_3_basic_move_node15;
								node15->SetClassNameString("Action");
								node15->SetId(15);
#if !defined(BEHAVIAC_RELEASE)
								node15->SetAgentType("framework::Ship");
#endif
								node14->AddChild(node15);
								node14->SetHasEvents(node14->HasEvents() | node15->HasEvents());
							}
							node11->SetHasEvents(node11->HasEvents() | node14->HasEvents());
						}
						node9->SetHasEvents(node9->HasEvents() | node11->HasEvents());
					}
					node8->SetHasEvents(node8->HasEvents() | node9->HasEvents());
				}
				node1->SetHasEvents(node1->HasEvents() | node8->HasEvents());
			}
			pBT->SetHasEvents(pBT->HasEvents() | node1->HasEvents());
		}
		return true;
	}

	// Source file: ships/4_destroy_projectiles

	class BEHAVIAC_API DecoratorLoop_bt_ships_4_destroy_projectiles_node1 : public DecoratorLoop
	{
	public:
		BEHAVIAC_DECLARE_DYNAMIC_TYPE(DecoratorLoop_bt_ships_4_destroy_projectiles_node1, DecoratorLoop);
		DecoratorLoop_bt_ships_4_destroy_projectiles_node1()
		{
			m_bDecorateWhenChildEnds = true;
		}
	protected:
		virtual int GetCount(Agent* pAgent) const
		{
			BEHAVIAC_UNUSED_VAR(pAgent);
			return -1;
		}
	};

	class BEHAVIAC_API Condition_bt_ships_4_destroy_projectiles_node3 : public Condition
	{
	public:
		BEHAVIAC_DECLARE_DYNAMIC_TYPE(Condition_bt_ships_4_destroy_projectiles_node3, Condition);
		Condition_bt_ships_4_destroy_projectiles_node3()
		{
			opl_p0 = 200;
		}
	protected:
		virtual EBTStatus update_impl(Agent* pAgent, EBTStatus childStatus)
		{
			BEHAVIAC_UNUSED_VAR(pAgent);
			BEHAVIAC_UNUSED_VAR(childStatus);
			bool opl = ((framework::GameObject*)pAgent)->_Execute_Method_<framework::METHOD_TYPE_framework_GameObject_projectileNearby, bool, float >(opl_p0);
			bool opr = true;
			bool op = Details::Equal(opl, opr);
			return op ? BT_SUCCESS : BT_FAILURE;
		}
		float opl_p0;
	};

	class BEHAVIAC_API DecoratorCountLimit_bt_ships_4_destroy_projectiles_node4 : public DecoratorCountLimit
	{
	public:
		BEHAVIAC_DECLARE_DYNAMIC_TYPE(DecoratorCountLimit_bt_ships_4_destroy_projectiles_node4, DecoratorCountLimit);
		DecoratorCountLimit_bt_ships_4_destroy_projectiles_node4()
		{
			m_bDecorateWhenChildEnds = true;
		}
	protected:
		virtual int GetCount(Agent* pAgent) const
		{
			BEHAVIAC_UNUSED_VAR(pAgent);
			return 5;
		}
	};

	class BEHAVIAC_API Action_bt_ships_4_destroy_projectiles_node6 : public Action
	{
	public:
		BEHAVIAC_DECLARE_DYNAMIC_TYPE(Action_bt_ships_4_destroy_projectiles_node6, Action);
		Action_bt_ships_4_destroy_projectiles_node6()
		{
			method_p0 = 200;
		}
	protected:
		virtual EBTStatus update_impl(Agent* pAgent, EBTStatus childStatus)
		{
			BEHAVIAC_UNUSED_VAR(pAgent);
			BEHAVIAC_UNUSED_VAR(childStatus);
			((framework::Ship*)pAgent)->_Execute_Method_<framework::METHOD_TYPE_framework_Ship_DestroyAllNearbyProjectiles, void, float >(method_p0);
			return BT_SUCCESS;
		}
		float method_p0;
	};

	class BEHAVIAC_API Wait_bt_ships_4_destroy_projectiles_node0 : public Wait
	{
	public:
		BEHAVIAC_DECLARE_DYNAMIC_TYPE(Wait_bt_ships_4_destroy_projectiles_node0, Wait);
		Wait_bt_ships_4_destroy_projectiles_node0()
		{
			m_ignoreTimeScale = false;
		}
	protected:
		virtual float GetTime(Agent* pAgent) const
		{
			BEHAVIAC_UNUSED_VAR(pAgent);
			return 1000;
		}
	};

	BEHAVIAC_API bool Create_bt_ships_4_destroy_projectiles(BehaviorTree* pBT)
	{
		pBT->SetClassNameString("BehaviorTree");
		pBT->SetId(-1);
		pBT->SetName("ships/4_destroy_projectiles");
		pBT->SetIsFSM(false);
#if !defined(BEHAVIAC_RELEASE)
		pBT->SetAgentType("framework::Ship");
#endif
		// children
		{
			DecoratorLoop_bt_ships_4_destroy_projectiles_node1* node1 = BEHAVIAC_NEW DecoratorLoop_bt_ships_4_destroy_projectiles_node1;
			node1->SetClassNameString("DecoratorLoop");
			node1->SetId(1);
#if !defined(BEHAVIAC_RELEASE)
			node1->SetAgentType("framework::Ship");
#endif
			pBT->AddChild(node1);
			{
				Sequence* node2 = BEHAVIAC_NEW Sequence;
				node2->SetClassNameString("Sequence");
				node2->SetId(2);
#if !defined(BEHAVIAC_RELEASE)
				node2->SetAgentType("framework::Ship");
#endif
				node1->AddChild(node2);
				{
					Condition_bt_ships_4_destroy_projectiles_node3* node3 = BEHAVIAC_NEW Condition_bt_ships_4_destroy_projectiles_node3;
					node3->SetClassNameString("Condition");
					node3->SetId(3);
#if !defined(BEHAVIAC_RELEASE)
					node3->SetAgentType("framework::Ship");
#endif
					node2->AddChild(node3);
					node2->SetHasEvents(node2->HasEvents() | node3->HasEvents());
				}
				{
					DecoratorCountLimit_bt_ships_4_destroy_projectiles_node4* node4 = BEHAVIAC_NEW DecoratorCountLimit_bt_ships_4_destroy_projectiles_node4;
					node4->SetClassNameString("DecoratorCountLimit");
					node4->SetId(4);
#if !defined(BEHAVIAC_RELEASE)
					node4->SetAgentType("framework::Ship");
#endif
					node2->AddChild(node4);
					{
						Sequence* node5 = BEHAVIAC_NEW Sequence;
						node5->SetClassNameString("Sequence");
						node5->SetId(5);
#if !defined(BEHAVIAC_RELEASE)
						node5->SetAgentType("framework::Ship");
#endif
						node4->AddChild(node5);
						{
							Action_bt_ships_4_destroy_projectiles_node6* node6 = BEHAVIAC_NEW Action_bt_ships_4_destroy_projectiles_node6;
							node6->SetClassNameString("Action");
							node6->SetId(6);
#if !defined(BEHAVIAC_RELEASE)
							node6->SetAgentType("framework::Ship");
#endif
							node5->AddChild(node6);
							node5->SetHasEvents(node5->HasEvents() | node6->HasEvents());
						}
						{
							Wait_bt_ships_4_destroy_projectiles_node0* node0 = BEHAVIAC_NEW Wait_bt_ships_4_destroy_projectiles_node0;
							node0->SetClassNameString("Wait");
							node0->SetId(0);
#if !defined(BEHAVIAC_RELEASE)
							node0->SetAgentType("framework::Ship");
#endif
							node5->AddChild(node0);
							node5->SetHasEvents(node5->HasEvents() | node0->HasEvents());
						}
						node4->SetHasEvents(node4->HasEvents() | node5->HasEvents());
					}
					node2->SetHasEvents(node2->HasEvents() | node4->HasEvents());
				}
				node1->SetHasEvents(node1->HasEvents() | node2->HasEvents());
			}
			pBT->SetHasEvents(pBT->HasEvents() | node1->HasEvents());
		}
		return true;
	}

	// Source file: ships/5_group

	class BEHAVIAC_API Parallel_bt_ships_5_group_node1 : public Parallel
	{
	public:
		BEHAVIAC_DECLARE_DYNAMIC_TYPE(Parallel_bt_ships_5_group_node1, Parallel);
		Parallel_bt_ships_5_group_node1()
		{
			m_failPolicy = FAIL_ON_ONE;
			m_succeedPolicy = SUCCEED_ON_ALL;
			m_exitPolicy = EXIT_NONE;
			m_childFinishPolicy = CHILDFINISH_LOOP;
		}
	protected:
	};

	class BEHAVIAC_API DecoratorLoop_bt_ships_5_group_node2 : public DecoratorLoop
	{
	public:
		BEHAVIAC_DECLARE_DYNAMIC_TYPE(DecoratorLoop_bt_ships_5_group_node2, DecoratorLoop);
		DecoratorLoop_bt_ships_5_group_node2()
		{
			m_bDecorateWhenChildEnds = true;
		}
	protected:
		virtual int GetCount(Agent* pAgent) const
		{
			BEHAVIAC_UNUSED_VAR(pAgent);
			return -1;
		}
	};

	class BEHAVIAC_API Action_bt_ships_5_group_node4 : public Action
	{
	public:
		BEHAVIAC_DECLARE_DYNAMIC_TYPE(Action_bt_ships_5_group_node4, Action);
		Action_bt_ships_5_group_node4()
		{
			method_p0 = 10;
			method_p1 = 0;
			method_p2 = 0.5f;
		}
	protected:
		virtual EBTStatus update_impl(Agent* pAgent, EBTStatus childStatus)
		{
			BEHAVIAC_UNUSED_VAR(pAgent);
			BEHAVIAC_UNUSED_VAR(childStatus);
			bool result = ((framework::Ship*)pAgent)->_Execute_Method_<framework::METHOD_TYPE_framework_Ship_GotoPointRelative, bool, float, float, float >(method_p0, method_p1, method_p2);
			return ((framework::Ship*)pAgent)->framework::Ship::checkresult(result);
		}
		float method_p0;
		float method_p1;
		float method_p2;
	};

	class BEHAVIAC_API Action_bt_ships_5_group_node5 : public Action
	{
	public:
		BEHAVIAC_DECLARE_DYNAMIC_TYPE(Action_bt_ships_5_group_node5, Action);
		Action_bt_ships_5_group_node5()
		{
			method_p0 = 0;
			method_p1 = 10;
			method_p2 = 0.5f;
		}
	protected:
		virtual EBTStatus update_impl(Agent* pAgent, EBTStatus childStatus)
		{
			BEHAVIAC_UNUSED_VAR(pAgent);
			BEHAVIAC_UNUSED_VAR(childStatus);
			bool result = ((framework::Ship*)pAgent)->_Execute_Method_<framework::METHOD_TYPE_framework_Ship_GotoPointRelative, bool, float, float, float >(method_p0, method_p1, method_p2);
			return ((framework::Ship*)pAgent)->framework::Ship::checkresult(result);
		}
		float method_p0;
		float method_p1;
		float method_p2;
	};

	class BEHAVIAC_API Action_bt_ships_5_group_node6 : public Action
	{
	public:
		BEHAVIAC_DECLARE_DYNAMIC_TYPE(Action_bt_ships_5_group_node6, Action);
		Action_bt_ships_5_group_node6()
		{
			method_p0 = -10;
			method_p1 = 0;
			method_p2 = 0.5f;
		}
	protected:
		virtual EBTStatus update_impl(Agent* pAgent, EBTStatus childStatus)
		{
			BEHAVIAC_UNUSED_VAR(pAgent);
			BEHAVIAC_UNUSED_VAR(childStatus);
			bool result = ((framework::Ship*)pAgent)->_Execute_Method_<framework::METHOD_TYPE_framework_Ship_GotoPointRelative, bool, float, float, float >(method_p0, method_p1, method_p2);
			return ((framework::Ship*)pAgent)->framework::Ship::checkresult(result);
		}
		float method_p0;
		float method_p1;
		float method_p2;
	};

	class BEHAVIAC_API Action_bt_ships_5_group_node7 : public Action
	{
	public:
		BEHAVIAC_DECLARE_DYNAMIC_TYPE(Action_bt_ships_5_group_node7, Action);
		Action_bt_ships_5_group_node7()
		{
			method_p0 = 0;
			method_p1 = -10;
			method_p2 = 0.5f;
		}
	protected:
		virtual EBTStatus update_impl(Agent* pAgent, EBTStatus childStatus)
		{
			BEHAVIAC_UNUSED_VAR(pAgent);
			BEHAVIAC_UNUSED_VAR(childStatus);
			bool result = ((framework::Ship*)pAgent)->_Execute_Method_<framework::METHOD_TYPE_framework_Ship_GotoPointRelative, bool, float, float, float >(method_p0, method_p1, method_p2);
			return ((framework::Ship*)pAgent)->framework::Ship::checkresult(result);
		}
		float method_p0;
		float method_p1;
		float method_p2;
	};

	class BEHAVIAC_API DecoratorLoop_bt_ships_5_group_node8 : public DecoratorLoop
	{
	public:
		BEHAVIAC_DECLARE_DYNAMIC_TYPE(DecoratorLoop_bt_ships_5_group_node8, DecoratorLoop);
		DecoratorLoop_bt_ships_5_group_node8()
		{
			m_bDecorateWhenChildEnds = true;
		}
	protected:
		virtual int GetCount(Agent* pAgent) const
		{
			BEHAVIAC_UNUSED_VAR(pAgent);
			return -1;
		}
	};

	class BEHAVIAC_API SelectorProbability_bt_ships_5_group_node9 : public SelectorProbability
	{
	public:
		BEHAVIAC_DECLARE_DYNAMIC_TYPE(SelectorProbability_bt_ships_5_group_node9, SelectorProbability);
		SelectorProbability_bt_ships_5_group_node9()
		{
		}
	public:
		void Initialize(const char* method)
		{
			this->m_method = Action::LoadMethod(method);
		}
	};

	class BEHAVIAC_API DecoratorWeight_bt_ships_5_group_node10 : public DecoratorWeight
	{
	public:
		BEHAVIAC_DECLARE_DYNAMIC_TYPE(DecoratorWeight_bt_ships_5_group_node10, DecoratorWeight);
		DecoratorWeight_bt_ships_5_group_node10()
		{
			m_bDecorateWhenChildEnds = false;
		}
	protected:
		virtual int GetWeight(Agent* pAgent) const
		{
			BEHAVIAC_UNUSED_VAR(pAgent);
			return 1;
		}
	};

	class BEHAVIAC_API Action_bt_ships_5_group_node11 : public Action
	{
	public:
		BEHAVIAC_DECLARE_DYNAMIC_TYPE(Action_bt_ships_5_group_node11, Action);
		Action_bt_ships_5_group_node11()
		{
		}
	protected:
		virtual EBTStatus update_impl(Agent* pAgent, EBTStatus childStatus)
		{
			BEHAVIAC_UNUSED_VAR(pAgent);
			BEHAVIAC_UNUSED_VAR(childStatus);
			bool result = ((framework::Ship*)pAgent)->_Execute_Method_<framework::METHOD_TYPE_framework_Ship_Fire, bool >();
			return ((framework::Ship*)pAgent)->framework::Ship::checkresult(result);
		}
	};

	class BEHAVIAC_API DecoratorWeight_bt_ships_5_group_node12 : public DecoratorWeight
	{
	public:
		BEHAVIAC_DECLARE_DYNAMIC_TYPE(DecoratorWeight_bt_ships_5_group_node12, DecoratorWeight);
		DecoratorWeight_bt_ships_5_group_node12()
		{
			m_bDecorateWhenChildEnds = false;
		}
	protected:
		virtual int GetWeight(Agent* pAgent) const
		{
			BEHAVIAC_UNUSED_VAR(pAgent);
			return 10;
		}
	};

	class BEHAVIAC_API Wait_bt_ships_5_group_node0 : public Wait
	{
	public:
		BEHAVIAC_DECLARE_DYNAMIC_TYPE(Wait_bt_ships_5_group_node0, Wait);
		Wait_bt_ships_5_group_node0()
		{
			m_ignoreTimeScale = false;
		}
	protected:
		virtual float GetTime(Agent* pAgent) const
		{
			BEHAVIAC_UNUSED_VAR(pAgent);
			return 500;
		}
	};

	BEHAVIAC_API bool Create_bt_ships_5_group(BehaviorTree* pBT)
	{
		pBT->SetClassNameString("BehaviorTree");
		pBT->SetId(-1);
		pBT->SetName("ships/5_group");
		pBT->SetIsFSM(false);
#if !defined(BEHAVIAC_RELEASE)
		pBT->SetAgentType("framework::Ship");
#endif
		// pars
		pBT->AddPar("framework::Ship", "bool", "par_a", "false");
		pBT->AddPar("framework::Ship", "int", "par_b", "0");
		// children
		{
			Parallel_bt_ships_5_group_node1* node1 = BEHAVIAC_NEW Parallel_bt_ships_5_group_node1;
			node1->SetClassNameString("Parallel");
			node1->SetId(1);
#if !defined(BEHAVIAC_RELEASE)
			node1->SetAgentType("framework::Ship");
#endif
			pBT->AddChild(node1);
			{
				DecoratorLoop_bt_ships_5_group_node2* node2 = BEHAVIAC_NEW DecoratorLoop_bt_ships_5_group_node2;
				node2->SetClassNameString("DecoratorLoop");
				node2->SetId(2);
#if !defined(BEHAVIAC_RELEASE)
				node2->SetAgentType("framework::Ship");
#endif
				node1->AddChild(node2);
				{
					Sequence* node3 = BEHAVIAC_NEW Sequence;
					node3->SetClassNameString("Sequence");
					node3->SetId(3);
#if !defined(BEHAVIAC_RELEASE)
					node3->SetAgentType("framework::Ship");
#endif
					node2->AddChild(node3);
					{
						Action_bt_ships_5_group_node4* node4 = BEHAVIAC_NEW Action_bt_ships_5_group_node4;
						node4->SetClassNameString("Action");
						node4->SetId(4);
#if !defined(BEHAVIAC_RELEASE)
						node4->SetAgentType("framework::Ship");
#endif
						node3->AddChild(node4);
						node3->SetHasEvents(node3->HasEvents() | node4->HasEvents());
					}
					{
						Action_bt_ships_5_group_node5* node5 = BEHAVIAC_NEW Action_bt_ships_5_group_node5;
						node5->SetClassNameString("Action");
						node5->SetId(5);
#if !defined(BEHAVIAC_RELEASE)
						node5->SetAgentType("framework::Ship");
#endif
						node3->AddChild(node5);
						node3->SetHasEvents(node3->HasEvents() | node5->HasEvents());
					}
					{
						Action_bt_ships_5_group_node6* node6 = BEHAVIAC_NEW Action_bt_ships_5_group_node6;
						node6->SetClassNameString("Action");
						node6->SetId(6);
#if !defined(BEHAVIAC_RELEASE)
						node6->SetAgentType("framework::Ship");
#endif
						node3->AddChild(node6);
						node3->SetHasEvents(node3->HasEvents() | node6->HasEvents());
					}
					{
						Action_bt_ships_5_group_node7* node7 = BEHAVIAC_NEW Action_bt_ships_5_group_node7;
						node7->SetClassNameString("Action");
						node7->SetId(7);
#if !defined(BEHAVIAC_RELEASE)
						node7->SetAgentType("framework::Ship");
#endif
						node3->AddChild(node7);
						node3->SetHasEvents(node3->HasEvents() | node7->HasEvents());
					}
					node2->SetHasEvents(node2->HasEvents() | node3->HasEvents());
				}
				node1->SetHasEvents(node1->HasEvents() | node2->HasEvents());
			}
			{
				DecoratorLoop_bt_ships_5_group_node8* node8 = BEHAVIAC_NEW DecoratorLoop_bt_ships_5_group_node8;
				node8->SetClassNameString("DecoratorLoop");
				node8->SetId(8);
#if !defined(BEHAVIAC_RELEASE)
				node8->SetAgentType("framework::Ship");
#endif
				node1->AddChild(node8);
				{
					SelectorProbability_bt_ships_5_group_node9* node9 = BEHAVIAC_NEW SelectorProbability_bt_ships_5_group_node9;
					node9->SetClassNameString("SelectorProbability");
					node9->SetId(9);
#if !defined(BEHAVIAC_RELEASE)
					node9->SetAgentType("framework::Ship");
#endif
					node8->AddChild(node9);
					{
						DecoratorWeight_bt_ships_5_group_node10* node10 = BEHAVIAC_NEW DecoratorWeight_bt_ships_5_group_node10;
						node10->SetClassNameString("DecoratorWeight");
						node10->SetId(10);
#if !defined(BEHAVIAC_RELEASE)
						node10->SetAgentType("framework::Ship");
#endif
						node9->AddChild(node10);
						{
							Action_bt_ships_5_group_node11* node11 = BEHAVIAC_NEW Action_bt_ships_5_group_node11;
							node11->SetClassNameString("Action");
							node11->SetId(11);
#if !defined(BEHAVIAC_RELEASE)
							node11->SetAgentType("framework::Ship");
#endif
							node10->AddChild(node11);
							node10->SetHasEvents(node10->HasEvents() | node11->HasEvents());
						}
						node9->SetHasEvents(node9->HasEvents() | node10->HasEvents());
					}
					{
						DecoratorWeight_bt_ships_5_group_node12* node12 = BEHAVIAC_NEW DecoratorWeight_bt_ships_5_group_node12;
						node12->SetClassNameString("DecoratorWeight");
						node12->SetId(12);
#if !defined(BEHAVIAC_RELEASE)
						node12->SetAgentType("framework::Ship");
#endif
						node9->AddChild(node12);
						{
							Wait_bt_ships_5_group_node0* node0 = BEHAVIAC_NEW Wait_bt_ships_5_group_node0;
							node0->SetClassNameString("Wait");
							node0->SetId(0);
#if !defined(BEHAVIAC_RELEASE)
							node0->SetAgentType("framework::Ship");
#endif
							node12->AddChild(node0);
							node12->SetHasEvents(node12->HasEvents() | node0->HasEvents());
						}
						node9->SetHasEvents(node9->HasEvents() | node12->HasEvents());
					}
					node8->SetHasEvents(node8->HasEvents() | node9->HasEvents());
				}
				node1->SetHasEvents(node1->HasEvents() | node8->HasEvents());
			}
			pBT->SetHasEvents(pBT->HasEvents() | node1->HasEvents());
		}
		return true;
	}

	// Source file: ships/6_director

	class BEHAVIAC_API DecoratorLoop_bt_ships_6_director_node1 : public DecoratorLoop
	{
	public:
		BEHAVIAC_DECLARE_DYNAMIC_TYPE(DecoratorLoop_bt_ships_6_director_node1, DecoratorLoop);
		DecoratorLoop_bt_ships_6_director_node1()
		{
			m_bDecorateWhenChildEnds = true;
		}
	protected:
		virtual int GetCount(Agent* pAgent) const
		{
			BEHAVIAC_UNUSED_VAR(pAgent);
			return -1;
		}
	};

	class BEHAVIAC_API Action_bt_ships_6_director_node3 : public Action
	{
	public:
		BEHAVIAC_DECLARE_DYNAMIC_TYPE(Action_bt_ships_6_director_node3, Action);
		Action_bt_ships_6_director_node3()
		{
			method_p0 = 3;
		}
	protected:
		virtual EBTStatus update_impl(Agent* pAgent, EBTStatus childStatus)
		{
			BEHAVIAC_UNUSED_VAR(pAgent);
			BEHAVIAC_UNUSED_VAR(childStatus);
			((framework::WorldState*)pAgent)->_Execute_Method_<framework::METHOD_TYPE_framework_WorldState_SpawnShip, void, int >(method_p0);
			return BT_SUCCESS;
		}
		int method_p0;
	};

	class BEHAVIAC_API DecoratorLoopUntil_bt_ships_6_director_node4 : public DecoratorLoopUntil
	{
	public:
		BEHAVIAC_DECLARE_DYNAMIC_TYPE(DecoratorLoopUntil_bt_ships_6_director_node4, DecoratorLoopUntil);
		DecoratorLoopUntil_bt_ships_6_director_node4()
		{
			m_bDecorateWhenChildEnds = false;
			m_until = true;
		}
	protected:
		virtual int GetCount(Agent* pAgent) const
		{
			BEHAVIAC_UNUSED_VAR(pAgent);
			return -1;
		}
	};

	class BEHAVIAC_API Condition_bt_ships_6_director_node5 : public Condition
	{
	public:
		BEHAVIAC_DECLARE_DYNAMIC_TYPE(Condition_bt_ships_6_director_node5, Condition);
		Condition_bt_ships_6_director_node5()
		{
		}
	protected:
		virtual EBTStatus update_impl(Agent* pAgent, EBTStatus childStatus)
		{
			BEHAVIAC_UNUSED_VAR(pAgent);
			BEHAVIAC_UNUSED_VAR(childStatus);
			bool opl = ((framework::WorldState*)pAgent)->_Execute_Method_<framework::METHOD_TYPE_framework_WorldState_NextWave, bool >();
			bool opr = true;
			bool op = Details::Equal(opl, opr);
			return op ? BT_SUCCESS : BT_FAILURE;
		}
	};

	class BEHAVIAC_API Action_bt_ships_6_director_node6 : public Action
	{
	public:
		BEHAVIAC_DECLARE_DYNAMIC_TYPE(Action_bt_ships_6_director_node6, Action);
		Action_bt_ships_6_director_node6()
		{
			method_p0 = 4;
		}
	protected:
		virtual EBTStatus update_impl(Agent* pAgent, EBTStatus childStatus)
		{
			BEHAVIAC_UNUSED_VAR(pAgent);
			BEHAVIAC_UNUSED_VAR(childStatus);
			((framework::WorldState*)pAgent)->_Execute_Method_<framework::METHOD_TYPE_framework_WorldState_SpawnShip, void, int >(method_p0);
			return BT_SUCCESS;
		}
		int method_p0;
	};

	class BEHAVIAC_API DecoratorLoopUntil_bt_ships_6_director_node7 : public DecoratorLoopUntil
	{
	public:
		BEHAVIAC_DECLARE_DYNAMIC_TYPE(DecoratorLoopUntil_bt_ships_6_director_node7, DecoratorLoopUntil);
		DecoratorLoopUntil_bt_ships_6_director_node7()
		{
			m_bDecorateWhenChildEnds = false;
			m_until = true;
		}
	protected:
		virtual int GetCount(Agent* pAgent) const
		{
			BEHAVIAC_UNUSED_VAR(pAgent);
			return -1;
		}
	};

	class BEHAVIAC_API Condition_bt_ships_6_director_node8 : public Condition
	{
	public:
		BEHAVIAC_DECLARE_DYNAMIC_TYPE(Condition_bt_ships_6_director_node8, Condition);
		Condition_bt_ships_6_director_node8()
		{
		}
	protected:
		virtual EBTStatus update_impl(Agent* pAgent, EBTStatus childStatus)
		{
			BEHAVIAC_UNUSED_VAR(pAgent);
			BEHAVIAC_UNUSED_VAR(childStatus);
			bool opl = ((framework::WorldState*)pAgent)->_Execute_Method_<framework::METHOD_TYPE_framework_WorldState_NextWave, bool >();
			bool opr = true;
			bool op = Details::Equal(opl, opr);
			return op ? BT_SUCCESS : BT_FAILURE;
		}
	};

	class BEHAVIAC_API Action_bt_ships_6_director_node9 : public Action
	{
	public:
		BEHAVIAC_DECLARE_DYNAMIC_TYPE(Action_bt_ships_6_director_node9, Action);
		Action_bt_ships_6_director_node9()
		{
			method_p0 = 5;
		}
	protected:
		virtual EBTStatus update_impl(Agent* pAgent, EBTStatus childStatus)
		{
			BEHAVIAC_UNUSED_VAR(pAgent);
			BEHAVIAC_UNUSED_VAR(childStatus);
			((framework::WorldState*)pAgent)->_Execute_Method_<framework::METHOD_TYPE_framework_WorldState_SpawnShip, void, int >(method_p0);
			return BT_SUCCESS;
		}
		int method_p0;
	};

	class BEHAVIAC_API DecoratorLoopUntil_bt_ships_6_director_node10 : public DecoratorLoopUntil
	{
	public:
		BEHAVIAC_DECLARE_DYNAMIC_TYPE(DecoratorLoopUntil_bt_ships_6_director_node10, DecoratorLoopUntil);
		DecoratorLoopUntil_bt_ships_6_director_node10()
		{
			m_bDecorateWhenChildEnds = false;
			m_until = true;
		}
	protected:
		virtual int GetCount(Agent* pAgent) const
		{
			BEHAVIAC_UNUSED_VAR(pAgent);
			return -1;
		}
	};

	class BEHAVIAC_API Condition_bt_ships_6_director_node11 : public Condition
	{
	public:
		BEHAVIAC_DECLARE_DYNAMIC_TYPE(Condition_bt_ships_6_director_node11, Condition);
		Condition_bt_ships_6_director_node11()
		{
		}
	protected:
		virtual EBTStatus update_impl(Agent* pAgent, EBTStatus childStatus)
		{
			BEHAVIAC_UNUSED_VAR(pAgent);
			BEHAVIAC_UNUSED_VAR(childStatus);
			bool opl = ((framework::WorldState*)pAgent)->_Execute_Method_<framework::METHOD_TYPE_framework_WorldState_NextWave, bool >();
			bool opr = true;
			bool op = Details::Equal(opl, opr);
			return op ? BT_SUCCESS : BT_FAILURE;
		}
	};

	BEHAVIAC_API bool Create_bt_ships_6_director(BehaviorTree* pBT)
	{
		pBT->SetClassNameString("BehaviorTree");
		pBT->SetId(-1);
		pBT->SetName("ships/6_director");
		pBT->SetIsFSM(false);
#if !defined(BEHAVIAC_RELEASE)
		pBT->SetAgentType("framework::WorldState");
#endif
		// children
		{
			DecoratorLoop_bt_ships_6_director_node1* node1 = BEHAVIAC_NEW DecoratorLoop_bt_ships_6_director_node1;
			node1->SetClassNameString("DecoratorLoop");
			node1->SetId(1);
#if !defined(BEHAVIAC_RELEASE)
			node1->SetAgentType("framework::WorldState");
#endif
			pBT->AddChild(node1);
			{
				Sequence* node2 = BEHAVIAC_NEW Sequence;
				node2->SetClassNameString("Sequence");
				node2->SetId(2);
#if !defined(BEHAVIAC_RELEASE)
				node2->SetAgentType("framework::WorldState");
#endif
				node1->AddChild(node2);
				{
					Action_bt_ships_6_director_node3* node3 = BEHAVIAC_NEW Action_bt_ships_6_director_node3;
					node3->SetClassNameString("Action");
					node3->SetId(3);
#if !defined(BEHAVIAC_RELEASE)
					node3->SetAgentType("framework::WorldState");
#endif
					node2->AddChild(node3);
					node2->SetHasEvents(node2->HasEvents() | node3->HasEvents());
				}
				{
					DecoratorLoopUntil_bt_ships_6_director_node4* node4 = BEHAVIAC_NEW DecoratorLoopUntil_bt_ships_6_director_node4;
					node4->SetClassNameString("DecoratorLoopUntil");
					node4->SetId(4);
#if !defined(BEHAVIAC_RELEASE)
					node4->SetAgentType("framework::WorldState");
#endif
					node2->AddChild(node4);
					{
						Condition_bt_ships_6_director_node5* node5 = BEHAVIAC_NEW Condition_bt_ships_6_director_node5;
						node5->SetClassNameString("Condition");
						node5->SetId(5);
#if !defined(BEHAVIAC_RELEASE)
						node5->SetAgentType("framework::WorldState");
#endif
						node4->AddChild(node5);
						node4->SetHasEvents(node4->HasEvents() | node5->HasEvents());
					}
					node2->SetHasEvents(node2->HasEvents() | node4->HasEvents());
				}
				{
					Action_bt_ships_6_director_node6* node6 = BEHAVIAC_NEW Action_bt_ships_6_director_node6;
					node6->SetClassNameString("Action");
					node6->SetId(6);
#if !defined(BEHAVIAC_RELEASE)
					node6->SetAgentType("framework::WorldState");
#endif
					node2->AddChild(node6);
					node2->SetHasEvents(node2->HasEvents() | node6->HasEvents());
				}
				{
					DecoratorLoopUntil_bt_ships_6_director_node7* node7 = BEHAVIAC_NEW DecoratorLoopUntil_bt_ships_6_director_node7;
					node7->SetClassNameString("DecoratorLoopUntil");
					node7->SetId(7);
#if !defined(BEHAVIAC_RELEASE)
					node7->SetAgentType("framework::WorldState");
#endif
					node2->AddChild(node7);
					{
						Condition_bt_ships_6_director_node8* node8 = BEHAVIAC_NEW Condition_bt_ships_6_director_node8;
						node8->SetClassNameString("Condition");
						node8->SetId(8);
#if !defined(BEHAVIAC_RELEASE)
						node8->SetAgentType("framework::WorldState");
#endif
						node7->AddChild(node8);
						node7->SetHasEvents(node7->HasEvents() | node8->HasEvents());
					}
					node2->SetHasEvents(node2->HasEvents() | node7->HasEvents());
				}
				{
					Action_bt_ships_6_director_node9* node9 = BEHAVIAC_NEW Action_bt_ships_6_director_node9;
					node9->SetClassNameString("Action");
					node9->SetId(9);
#if !defined(BEHAVIAC_RELEASE)
					node9->SetAgentType("framework::WorldState");
#endif
					node2->AddChild(node9);
					node2->SetHasEvents(node2->HasEvents() | node9->HasEvents());
				}
				{
					DecoratorLoopUntil_bt_ships_6_director_node10* node10 = BEHAVIAC_NEW DecoratorLoopUntil_bt_ships_6_director_node10;
					node10->SetClassNameString("DecoratorLoopUntil");
					node10->SetId(10);
#if !defined(BEHAVIAC_RELEASE)
					node10->SetAgentType("framework::WorldState");
#endif
					node2->AddChild(node10);
					{
						Condition_bt_ships_6_director_node11* node11 = BEHAVIAC_NEW Condition_bt_ships_6_director_node11;
						node11->SetClassNameString("Condition");
						node11->SetId(11);
#if !defined(BEHAVIAC_RELEASE)
						node11->SetAgentType("framework::WorldState");
#endif
						node10->AddChild(node11);
						node10->SetHasEvents(node10->HasEvents() | node11->HasEvents());
					}
					node2->SetHasEvents(node2->HasEvents() | node10->HasEvents());
				}
				node1->SetHasEvents(node1->HasEvents() | node2->HasEvents());
			}
			pBT->SetHasEvents(pBT->HasEvents() | node1->HasEvents());
		}
		return true;
	}

	// Source file: ships/7_signal

	class BEHAVIAC_API DecoratorLoop_bt_ships_7_signal_node1 : public DecoratorLoop
	{
	public:
		BEHAVIAC_DECLARE_DYNAMIC_TYPE(DecoratorLoop_bt_ships_7_signal_node1, DecoratorLoop);
		DecoratorLoop_bt_ships_7_signal_node1()
		{
			m_bDecorateWhenChildEnds = true;
		}
	protected:
		virtual int GetCount(Agent* pAgent) const
		{
			BEHAVIAC_UNUSED_VAR(pAgent);
			return 1;
		}
	};

	class BEHAVIAC_API Parallel_bt_ships_7_signal_node3 : public Parallel
	{
	public:
		BEHAVIAC_DECLARE_DYNAMIC_TYPE(Parallel_bt_ships_7_signal_node3, Parallel);
		Parallel_bt_ships_7_signal_node3()
		{
			m_failPolicy = FAIL_ON_ONE;
			m_succeedPolicy = SUCCEED_ON_ALL;
			m_exitPolicy = EXIT_NONE;
			m_childFinishPolicy = CHILDFINISH_ONCE;
		}
	protected:
	};

	class BEHAVIAC_API DecoratorLoop_bt_ships_7_signal_node4 : public DecoratorLoop
	{
	public:
		BEHAVIAC_DECLARE_DYNAMIC_TYPE(DecoratorLoop_bt_ships_7_signal_node4, DecoratorLoop);
		DecoratorLoop_bt_ships_7_signal_node4()
		{
			m_bDecorateWhenChildEnds = true;
		}
	protected:
		virtual int GetCount(Agent* pAgent) const
		{
			BEHAVIAC_UNUSED_VAR(pAgent);
			return 3;
		}
	};

	class BEHAVIAC_API Action_bt_ships_7_signal_node6 : public Action
	{
	public:
		BEHAVIAC_DECLARE_DYNAMIC_TYPE(Action_bt_ships_7_signal_node6, Action);
		Action_bt_ships_7_signal_node6()
		{
		}
	protected:
		virtual EBTStatus update_impl(Agent* pAgent, EBTStatus childStatus)
		{
			BEHAVIAC_UNUSED_VAR(pAgent);
			BEHAVIAC_UNUSED_VAR(childStatus);
			bool result = ((framework::Ship*)pAgent)->_Execute_Method_<framework::METHOD_TYPE_framework_Ship_Fire, bool >();
			return ((framework::Ship*)pAgent)->framework::Ship::checkresult(result);
		}
	};

	class BEHAVIAC_API Wait_bt_ships_7_signal_node0 : public Wait
	{
	public:
		BEHAVIAC_DECLARE_DYNAMIC_TYPE(Wait_bt_ships_7_signal_node0, Wait);
		Wait_bt_ships_7_signal_node0()
		{
			m_ignoreTimeScale = false;
		}
	protected:
		virtual float GetTime(Agent* pAgent) const
		{
			BEHAVIAC_UNUSED_VAR(pAgent);
			return 1000;
		}
	};

	class BEHAVIAC_API Action_bt_ships_7_signal_node8 : public Action
	{
	public:
		BEHAVIAC_DECLARE_DYNAMIC_TYPE(Action_bt_ships_7_signal_node8, Action);
		Action_bt_ships_7_signal_node8()
		{
			method_p0 = 500;
			method_p1 = 50;
			method_p2 = 80;
		}
	protected:
		virtual EBTStatus update_impl(Agent* pAgent, EBTStatus childStatus)
		{
			BEHAVIAC_UNUSED_VAR(pAgent);
			BEHAVIAC_UNUSED_VAR(childStatus);
			bool result = ((framework::Ship*)pAgent)->_Execute_Method_<framework::METHOD_TYPE_framework_Ship_GotoPoint, bool, float, float, float >(method_p0, method_p1, method_p2);
			return ((framework::Ship*)pAgent)->framework::Ship::checkresult(result);
		}
		float method_p0;
		float method_p1;
		float method_p2;
	};

	class BEHAVIAC_API Assignment_bt_ships_7_signal_node9 : public Assignment
	{
	public:
		BEHAVIAC_DECLARE_DYNAMIC_TYPE(Assignment_bt_ships_7_signal_node9, Assignment);
		Assignment_bt_ships_7_signal_node9()
		{
		}
	protected:
		virtual EBTStatus update_impl(Agent* pAgent, EBTStatus childStatus)
		{
			BEHAVIAC_UNUSED_VAR(pAgent);
			BEHAVIAC_UNUSED_VAR(childStatus);
			EBTStatus result = BT_SUCCESS;
			bool opr = true;
			Agent* pAgent_opl = Agent::GetInstance("framework::WorldState", pAgent->GetContextId());
			BEHAVIAC_ASSERT(pAgent_opl);
			((framework::WorldState*)pAgent_opl)->_Get_Property_<framework::PROPERTY_TYPE_framework_WorldState_SyncSignal, bool >() = opr;
			return result;
		}
	};

	BEHAVIAC_API bool Create_bt_ships_7_signal(BehaviorTree* pBT)
	{
		pBT->SetClassNameString("BehaviorTree");
		pBT->SetId(-1);
		pBT->SetName("ships/7_signal");
		pBT->SetIsFSM(false);
#if !defined(BEHAVIAC_RELEASE)
		pBT->SetAgentType("framework::Ship");
#endif
		// pars
		pBT->AddPar("framework::Ship", "bool", "par_a", "false");
		pBT->AddPar("framework::Ship", "int", "par_b", "0");
		// children
		{
			DecoratorLoop_bt_ships_7_signal_node1* node1 = BEHAVIAC_NEW DecoratorLoop_bt_ships_7_signal_node1;
			node1->SetClassNameString("DecoratorLoop");
			node1->SetId(1);
#if !defined(BEHAVIAC_RELEASE)
			node1->SetAgentType("framework::Ship");
#endif
			pBT->AddChild(node1);
			{
				Sequence* node2 = BEHAVIAC_NEW Sequence;
				node2->SetClassNameString("Sequence");
				node2->SetId(2);
#if !defined(BEHAVIAC_RELEASE)
				node2->SetAgentType("framework::Ship");
#endif
				node1->AddChild(node2);
				{
					Parallel_bt_ships_7_signal_node3* node3 = BEHAVIAC_NEW Parallel_bt_ships_7_signal_node3;
					node3->SetClassNameString("Parallel");
					node3->SetId(3);
#if !defined(BEHAVIAC_RELEASE)
					node3->SetAgentType("framework::Ship");
#endif
					node2->AddChild(node3);
					{
						DecoratorLoop_bt_ships_7_signal_node4* node4 = BEHAVIAC_NEW DecoratorLoop_bt_ships_7_signal_node4;
						node4->SetClassNameString("DecoratorLoop");
						node4->SetId(4);
#if !defined(BEHAVIAC_RELEASE)
						node4->SetAgentType("framework::Ship");
#endif
						node3->AddChild(node4);
						{
							Sequence* node5 = BEHAVIAC_NEW Sequence;
							node5->SetClassNameString("Sequence");
							node5->SetId(5);
#if !defined(BEHAVIAC_RELEASE)
							node5->SetAgentType("framework::Ship");
#endif
							node4->AddChild(node5);
							{
								Action_bt_ships_7_signal_node6* node6 = BEHAVIAC_NEW Action_bt_ships_7_signal_node6;
								node6->SetClassNameString("Action");
								node6->SetId(6);
#if !defined(BEHAVIAC_RELEASE)
								node6->SetAgentType("framework::Ship");
#endif
								node5->AddChild(node6);
								node5->SetHasEvents(node5->HasEvents() | node6->HasEvents());
							}
							{
								Wait_bt_ships_7_signal_node0* node0 = BEHAVIAC_NEW Wait_bt_ships_7_signal_node0;
								node0->SetClassNameString("Wait");
								node0->SetId(0);
#if !defined(BEHAVIAC_RELEASE)
								node0->SetAgentType("framework::Ship");
#endif
								node5->AddChild(node0);
								node5->SetHasEvents(node5->HasEvents() | node0->HasEvents());
							}
							node4->SetHasEvents(node4->HasEvents() | node5->HasEvents());
						}
						node3->SetHasEvents(node3->HasEvents() | node4->HasEvents());
					}
					{
						Action_bt_ships_7_signal_node8* node8 = BEHAVIAC_NEW Action_bt_ships_7_signal_node8;
						node8->SetClassNameString("Action");
						node8->SetId(8);
#if !defined(BEHAVIAC_RELEASE)
						node8->SetAgentType("framework::Ship");
#endif
						node3->AddChild(node8);
						node3->SetHasEvents(node3->HasEvents() | node8->HasEvents());
					}
					node2->SetHasEvents(node2->HasEvents() | node3->HasEvents());
				}
				{
					Assignment_bt_ships_7_signal_node9* node9 = BEHAVIAC_NEW Assignment_bt_ships_7_signal_node9;
					node9->SetClassNameString("Assignment");
					node9->SetId(9);
#if !defined(BEHAVIAC_RELEASE)
					node9->SetAgentType("framework::Ship");
#endif
					node2->AddChild(node9);
					node2->SetHasEvents(node2->HasEvents() | node9->HasEvents());
				}
				{
					Noop* node11 = BEHAVIAC_NEW Noop;
					node11->SetClassNameString("Noop");
					node11->SetId(11);
#if !defined(BEHAVIAC_RELEASE)
					node11->SetAgentType("framework::Ship");
#endif
					node2->AddChild(node11);
					node2->SetHasEvents(node2->HasEvents() | node11->HasEvents());
				}
				node1->SetHasEvents(node1->HasEvents() | node2->HasEvents());
			}
			pBT->SetHasEvents(pBT->HasEvents() | node1->HasEvents());
		}
		return true;
	}

	class CppGenerationManager : GenerationManager
	{
	public:
		CppGenerationManager()
		{
			SetInstance(this);
		}

		virtual void RegisterBehaviorsImplement()
		{
			Workspace::GetInstance()->RegisterBehaviorTreeCreator("base/boring", Create_bt_base_boring);
			Workspace::GetInstance()->RegisterBehaviorTreeCreator("base/homing", Create_bt_base_homing);
			Workspace::GetInstance()->RegisterBehaviorTreeCreator("base/slowboring", Create_bt_base_slowboring);
			Workspace::GetInstance()->RegisterBehaviorTreeCreator("base/sync_homing", Create_bt_base_sync_homing);
			Workspace::GetInstance()->RegisterBehaviorTreeCreator("projectile/projectile_test", Create_bt_projectile_projectile_test);
			Workspace::GetInstance()->RegisterBehaviorTreeCreator("ships/0_basic", Create_bt_ships_0_basic);
			Workspace::GetInstance()->RegisterBehaviorTreeCreator("ships/1_1_suicide", Create_bt_ships_1_1_suicide);
			Workspace::GetInstance()->RegisterBehaviorTreeCreator("ships/1_2_suicide", Create_bt_ships_1_2_suicide);
			Workspace::GetInstance()->RegisterBehaviorTreeCreator("ships/1_3_suicide", Create_bt_ships_1_3_suicide);
			Workspace::GetInstance()->RegisterBehaviorTreeCreator("ships/2_basic_probability", Create_bt_ships_2_basic_probability);
			Workspace::GetInstance()->RegisterBehaviorTreeCreator("ships/3_basic_move", Create_bt_ships_3_basic_move);
			Workspace::GetInstance()->RegisterBehaviorTreeCreator("ships/4_destroy_projectiles", Create_bt_ships_4_destroy_projectiles);
			Workspace::GetInstance()->RegisterBehaviorTreeCreator("ships/5_group", Create_bt_ships_5_group);
			Workspace::GetInstance()->RegisterBehaviorTreeCreator("ships/6_director", Create_bt_ships_6_director);
			Workspace::GetInstance()->RegisterBehaviorTreeCreator("ships/7_signal", Create_bt_ships_7_signal);
		}
	};

	static CppGenerationManager cppGenerationManager;
}
