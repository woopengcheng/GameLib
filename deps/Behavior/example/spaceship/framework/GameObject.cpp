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

#include "GameObject.h"
#include "WorldState.h"
#include "globals.h"
#include "util.h"
#include "Projectile.h"

namespace framework
{
	int GameObject::IDCount = 0;

	BEGIN_PROPERTIES_DESCRIPTION(GameObject)
		REGISTER_PROPERTY(HP);
		REGISTER_PROPERTY(age);

		REGISTER_METHOD(GoStraight).PARAM_DISPLAY_INFO(L"speed");
		REGISTER_METHOD(TurnTowardsTarget).PARAM_DISPLAY_INFO(L"turnSpeed");

		REGISTER_METHOD(alignedWithPlayer);
		REGISTER_METHOD(playerIsAligned);
		REGISTER_METHOD(projectileNearby).PARAM_DISPLAY_INFO(L"radius");
		REGISTER_METHOD(distanceToPlayer);
	END_PROPERTIES_DESCRIPTION()

	GameObject::GameObject()
	{
		HP = 100;
		displayName = "default";
		displayHeading = 0;
		faction = 0;
		ID = getID();
		movementHeading = 0;
		accelMagnitude = 0;
		accelHeading = 0;
		isPlayer = false;
		usesPhysics = true;
		age = 0;
		owner = NULL;
		textureResource = NULL;
	};

	GameObject::~GameObject()
	{
		printf("%s destructor\n", displayName.c_str());
	}

	bool GameObject::doActions(float dt)
	{
		return true;
	}

	bool GameObject::registerCollision(GameObject* collidedObject)
	{
		printf("%s Collided with %s", this->displayName.c_str(), collidedObject->displayName.c_str());

		return true;
	}

	void GameObject::setSprite(char* resource, bool bBG)
	{
		char filename[_MAX_PATH];
		string_sprintf(filename, "../example/spaceship/Resources/%s.png", resource);

		textureResource = framework::ws->GetTextureResource(filename);
	}

	bool GameObject::registerWallCollision()
	{
		//move a little away from the wall if too close
		accelMagnitude = 0;

		if (GameObject::location.x < 0)
		{
			GameObject::location.x = 0;
		}

		if (GameObject::location.y < 0)
		{
			GameObject::location.y = 0;
		}

		//for some reason it works fine without this code
		//if (GameObject::location.x > (WorldState.CoordinateSizeX - 1))
		//	GameObject::location.x = WorldState.CoordinateSizeX - 1;
		//if (GameObject::location.y > (WorldState.CoordinateSizeY - 1))
		//	GameObject::location.y = WorldState.CoordinateSizeY - 1;
		return true;
	}

	bool GameObject::collideWith(GameObject* other) const
	{
		if (other && this->textureResource)
		{
			SDL_Rect myRect;
			myRect.x = (int)(this->location.x + this->textureResource->width * 0.1f);
			myRect.y = (int)(this->location.y + this->textureResource->height * 0.1f);
			myRect.w = (int)(this->textureResource->width * 0.8f);
			myRect.h = (int)(this->textureResource->height * 0.8f);

			SDL_Rect otherRect;
			otherRect.x = (int)(other->location.x + other->textureResource->width * 0.1f);
			otherRect.y = (int)(other->location.y + other->textureResource->height * 0.1f);
			otherRect.w = (int)(other->textureResource->width * 0.8f);
			otherRect.h = (int)(other->textureResource->height * 0.8f);

			return SDL_TRUE == SDL_HasIntersection(&myRect, &otherRect);
		}

		return false;
	}

	void GameObject::applyForceRect(float x, float y)
	{
		float oldX = accelMagnitude * cos(accelHeading * PI / 180);
		float oldY = accelMagnitude * sin(accelHeading * PI / 180);
		//should probably be using vectors everywhere
		float newX = oldX + x;
		float newY = oldY + y;
		accelMagnitude = sqrt(newX * newX + newY * newY);
		accelHeading = (180.0f * atan2(newY, newX)) / PI;
	}

	void GameObject::applyForcePolar(float heading, float magnitude)
	{
		float X = magnitude * cos(getRadians(heading));
		float Y = magnitude * sin(getRadians(heading));
		applyForceRect(X, Y);
	}

	bool GameObject::alignedWithPlayer() const
	{
		bool result = abs(ws->angleBetween(this, ws->getPlayer())) < 10;
		return result;
	}

	int GameObject::getHealth() const
	{
		return HP;
	}

	void GameObject::setHealth(unsigned int hp)
	{
		HP = hp;
	}

	bool GameObject::playerIsAligned() const
	{
		bool result = abs(ws->angleBetween(ws->getPlayer(), this)) < 10;
		return result;
	}

	float GameObject::distanceToPlayer() const
	{
		float result = ws->distanceBetween(ws->getPlayer(), this);
		return result;
	}

	bool GameObject::projectileNearby(float radius) const
	{
		const GameObjectList* objects = ws->getAllGameObjects();
		ConstGameObjectIter iter;

		for (iter = objects->begin(); iter != objects->end(); iter++)
		{
			if ((*iter)->faction != PROJECTILE_FACTION)
			{
				continue;
			}

			float distance = ws->distanceBetween(this, *iter);

			if (distance < radius)
			{
				return true;
			}
		}

		return false;
	}

	void GameObject::GoStraight(int speed)
	{
		ws->moveObject(this, this->location.offsetPolar(this->displayHeading, (float)speed));
	}

	int GameObject::TurnTowardsTarget(float turnSpeed)
	{
		GameObject* target = ws->getPlayer();
		float angleBetween = ws->angleBetween(this, target);

		turnSpeed /= 20.0f;

		if (angleBetween < -(turnSpeed + 1))
		{
			ws->rotateObject(this, -turnSpeed); //clockwise
			return behaviac::BT_RUNNING;
		}
		else if (angleBetween > turnSpeed + 1)
		{
			ws->rotateObject(this, turnSpeed);
			return behaviac::BT_RUNNING;
		}

		return behaviac::BT_RUNNING;
	}

}//namespace framework