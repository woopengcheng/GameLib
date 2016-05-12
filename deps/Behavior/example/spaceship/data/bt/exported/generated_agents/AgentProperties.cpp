// ---------------------------------------------------------------------
// This file is auto-generated, so please don't modify it by yourself!
// ---------------------------------------------------------------------

#include "behaviac/agent/taskmethod.h"

namespace behaviac
{
	class CppBehaviorLoaderImplement : CppBehaviorLoader
	{
	public:
		CppBehaviorLoaderImplement()
		{
			AgentProperties::SetInstance(this);
		}

		virtual bool load()
		{
			// ---------------------------------------------------------------------
			// properties
			// ---------------------------------------------------------------------

			AgentProperties* bb = NULL;

			// framework::GameObject
			bb = BEHAVIAC_NEW AgentProperties("framework::GameObject");
			AgentProperties::SetAgentTypeBlackboards("framework::GameObject", bb);
			bb->AddProperty("long", false, "age", "0", "framework::GameObject");
			bb->AddProperty("uint", false, "HP", "0", "framework::GameObject");

			// framework::Projectile
			bb = BEHAVIAC_NEW AgentProperties("framework::Projectile");
			AgentProperties::SetAgentTypeBlackboards("framework::Projectile", bb);
			bb->AddProperty("long", false, "age", "0", "framework::Projectile");
			bb->AddProperty("uint", false, "HP", "0", "framework::Projectile");

			// framework::Ship
			bb = BEHAVIAC_NEW AgentProperties("framework::Ship");
			AgentProperties::SetAgentTypeBlackboards("framework::Ship", bb);
			bb->AddProperty("long", false, "age", "0", "framework::Ship");
			bb->AddProperty("uint", false, "HP", "0", "framework::Ship");

			// framework::WorldState
			bb = BEHAVIAC_NEW AgentProperties("framework::WorldState");
			AgentProperties::SetAgentTypeBlackboards("framework::WorldState", bb);
			bb->AddProperty("uint", false, "HealthHP", "0", "framework::WorldState");
			bb->AddProperty("bool", false, "SyncSignal", "false", "framework::WorldState");
			bb->AddProperty("uint", false, "time", "0", "framework::WorldState");

			// ---------------------------------------------------------------------
			// tasks
			// ---------------------------------------------------------------------

			CTagObjectDescriptor* objectDesc = NULL;
			CCustomMethod* customeMethod = NULL;

			return true;
		}
	};

	static CppBehaviorLoaderImplement cppBehaviorLoaderImplement;
}
