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

#ifndef SHIP_H_
#define SHIP_H_

#include <list>
#include <vector>
#include <string>
#include <set>
#include "point.h"
#include "GameObject.h"
#include "WorldState.h"
#include "Projectile.h"

namespace framework
{
    class WorldState;

    class Ship: public GameObject
    {
    public:
		DECLARE_BEHAVIAC_OBJECT(framework::Ship, GameObject);

        bool doActions(float dt);
        bool registerCollision(GameObject* collidedObject);
        bool registerWallCollision();

        Ship();

        void fire(Projectile* p);
		bool Fire();

		bool GotoPoint(float x, float y, float speed);
		bool GotoPointRelative(float x, float y, float time);

		float GetConstFloatValue();
		double GetConstDoubleValue();

		void DestroyAllNearbyProjectiles(float radius);

		behaviac::EBTStatus checkresult(bool bFired);

		const char* (*projectileBrain)(Projectile* s);

	private:
		bool	m_bStarted;
		float	m_time;
		float	m_speed;
		point	m_target;
    };
}

#endif