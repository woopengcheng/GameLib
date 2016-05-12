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

#include "Ship.h"
#include "Definitions.h"
#include "Projectile.h"

namespace framework
{
	// Instantiate the globals that were declared as "extern" in globals.h
	WorldState* framework::ws;
	Renderer* framework::renderer;
	GameLogic* framework::gl;

	BEGIN_PROPERTIES_DESCRIPTION(Ship)
	{
		REGISTER_METHOD(checkresult);
		REGISTER_METHOD(Fire);
		REGISTER_METHOD(GotoPoint).PARAM_DISPLAY_INFO(L"x").PARAM_DISPLAY_INFO(L"y").PARAM_DISPLAY_INFO(L"speed");
		REGISTER_METHOD(GotoPointRelative).PARAM_DISPLAY_INFO(L"x").PARAM_DISPLAY_INFO(L"y").PARAM_DISPLAY_INFO(L"time");
		REGISTER_METHOD(DestroyAllNearbyProjectiles).PARAM_DISPLAY_INFO(L"radius");

		REGISTER_METHOD(getXPosition);
		REGISTER_METHOD(getYPosition);
		REGISTER_METHOD(GetConstFloatValue);
		REGISTER_METHOD(GetConstDoubleValue);
	}
	END_PROPERTIES_DESCRIPTION()

	Ship::Ship() : GameObject(), projectileBrain(0), m_bStarted(false), m_time(0.0f), m_speed(0.0f)
	{
		GameObject::usesPhysics = true;
	}

	bool Ship::doActions(float dt)
	{
		super::doActions(dt);

		return true;
	}

	///Sets the projectile heading and inserts it into the world
	void Ship::fire(Projectile* p)
	{
		p->displayHeading = displayHeading;

		//TODO: insert the projectile at a more suitable point
		int w = textureResource ? textureResource->width : 0;
		int h = textureResource ? textureResource->height : 0;
		ws->insertObject(p, point(location.x + w * 0.5f, location.y + h * 0.5f));
	}

	bool Ship::registerWallCollision()
	{
		//cancel accelerations
		accelMagnitude = 0;

		return true;
	}

	bool Ship::registerCollision(GameObject* collidedObject)
	{
		printf("ship collid with : %s\n", collidedObject->displayName.c_str());

		HP -= 10;

		if (HP <= 0 && !isPlayer)
		{
			ws->deleteObject(this);
			framework::ws->SetAI(NULL);
		}

		return true;
	}

	behaviac::EBTStatus Ship::checkresult(bool bOk)
	{
		if (bOk)
		{
			return behaviac::BT_SUCCESS;
		}

		return behaviac::BT_RUNNING;
	}

	bool Ship::Fire()
	{
		printf("Fire!\n");

		Projectile* p = behaviac::Agent::Create<Projectile>();
		p->SetIdFlag(kIdMask_Projectile);

		p->setSprite("Ammo/test");
		p->setBrain(this->projectileBrain(p));
		p->owner = this;
		
		this->fire(p);

		return true;
	}

	bool Ship::GotoPoint(float x, float y, float speed)
	{
		assert(speed > 0.001f);

		point target(x, y);

		m_target = target;

		printf("going to point (%f %f)\n", m_target.x, m_target.y);

		int timeElapsed = ws->timeElapsed;

		point vectorToTarget = m_target - this->location;
		if (vectorToTarget.vectorSize() < 2)
		{
			return true;
		}

		vectorToTarget.normalize();
		vectorToTarget *= (float)(timeElapsed * speed /1000.0f);

		point finalPosition = point(vectorToTarget.x + this->getXPosition(),vectorToTarget.y + this->getYPosition());
		ws->moveObject(this, finalPosition);

		return false;
	}
	
	bool Ship::GotoPointRelative(float xRelative, float yRelative, float time)
	{
		if (!m_bStarted)
		{
			point vectorToTarget(xRelative, yRelative);

			m_target = this->location + vectorToTarget;

			assert(time >= 0.0001f);
			
			m_time = time;
			m_speed = vectorToTarget.vectorSize() / time;

			m_bStarted = true;
		}

		printf("going to point (%f %f)\n", m_target.x, m_target.y);

		float timeElapsed = ws->timeElapsed / 1000.0f;

		point vectorToTarget = m_target - this->location;
		m_time -= timeElapsed;
		if (m_time < 0.0f)
		{
			m_bStarted = false;
			return true;
		}

		vectorToTarget.normalize();
		vectorToTarget *= (timeElapsed * m_speed);

		point finalPosition = point(vectorToTarget.x + this->getXPosition(),vectorToTarget.y + this->getYPosition());
		ws->moveObject(this, finalPosition);

		return false;
	}

	float Ship::GetConstFloatValue()
	{
		return 1000.0f;
	}

	double Ship::GetConstDoubleValue()
	{
		return 1000.0;
	}

	void Ship::DestroyAllNearbyProjectiles(float radius)
	{
		printf("destroying nearby projectiles\n");

		const GameObjectList* objects = ws->getAllGameObjects();

		for (ConstGameObjectIter iter = objects->begin(); iter!=objects->end(); iter++)
		{
			GameObject* g = *iter;

			if (g->faction != PROJECTILE_FACTION)
				continue;

			float distance = ws->distanceBetween(this, g);
			if (distance < radius)
			{
				ws->deleteObject(g);
			}
		}
	}

}//namespace framework