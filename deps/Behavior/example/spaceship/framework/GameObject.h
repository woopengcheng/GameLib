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

#ifndef GAMEOBJECT_H_
#define GAMEOBJECT_H_
#include <list>
#include <string>
#include "point.h"
#include <set>
#include "SDL.h"
#include "globals.h"

#include "behaviac/agent/agent.h"
#include "behaviac/agent/registermacros.h"

class CL_Sprite;
class CL_CollisionOutline;

const unsigned int kIdMask_World =		0x0001;
const unsigned int kIdMask_Player =		0x0002;
const unsigned int kIdMask_Opponent =	0x0004;
const unsigned int kIdMask_Projectile =	0x0008;

namespace framework
{
    class WorldState;
    class BehaviorTreeNode;
    class GameObject;

	struct TextureResource
	{
		SDL_Texture* texture;
		int width;
		int height;

		TextureResource() : texture(NULL), width(-1), height(-1) { }
	};

    /// Base class for any item in the game
    class GameObject : public behaviac::Agent
    {
    public:
		DECLARE_BEHAVIAC_OBJECT(framework::GameObject, behaviac::Agent);

        /// Called every frame to update the object
        virtual bool doActions(float dt);
        /// Notify the object of a collision with another object. Only called if usesPhysics is true
        virtual bool registerCollision(GameObject* collidedObject);
        /// Notify the object it has hit the edge of the level. Responding improperly to this can cause out of bounds errors
        virtual bool registerWallCollision();
        /// Set the GameObject's sprite, and load the collision outline if it exists. If it doesn't, regenerate and save it.
        //* The resource is a relative filename to an image (without the extension). e.g. "Ammo\\test"
        void setSprite(char* resource, bool bBG = false);

        /// Default constructor
        GameObject();
        /// Cleans up resources in use by the object when it is deleted
        ~GameObject();

        /// If Newtonian physics is enabled for the world, this imparts a force on the object
        void applyForceRect(float x, float y);
        /// If Newtonian physics is enabled for the world, this imparts a force on the object
        void applyForcePolar(float heading, float magnitude);

        float getXPosition() const
        {
            return location.x;
        }
        float getYPosition() const
        {
            return location.y;
        }

		void setBrain(const char* bt)
		{
			this->btsetcurrent(bt);
		}

        int getHealth() const;
		void setHealth(unsigned int hp);

		bool collideWith(GameObject* other) const;

        /// The graphical representation of the object
		framework::TextureResource* textureResource;

        /// True if this object is the player. Undefined behavior if more than one object has this flag set to true.
        bool isPlayer;
        /// Determines whether the object collides with others
        bool usesPhysics;

        /// Number of milliseconds the object has been in existence. Updated every tick based on the timeElapsed
        //long age;
		long age;

        /// The way the object's sprite is "turned"
        float displayHeading;
        /// Absolute coordinates of the object in the world.
        point location;

        GameObject* owner; //Generally the object that created this object, if any

        /// The scalar component of the object's acceleration
        float accelMagnitude;

        /// The human friendly name of the object
		std::string displayName;
		/// Used to divide objects into different categories to customize interactions between them
        int faction;

	protected:
        /// Unique for each object instance
        int ID;
        /// Generate a unique ID for an object
        static int getID()
        {
            IDCount++;
            return IDCount;
        }

        /// The number of "hit points" the object has.
        unsigned int HP;
        
		/// The directional component of the object's velocity
        float movementHeading;
        
		/// The directional component of the object's acceleration
        float accelHeading;

        /// Am I facing the player
        bool alignedWithPlayer() const;
        /// Is the player facing me
        bool playerIsAligned() const;
        bool projectileNearby(float radius) const;
        float distanceToPlayer() const;

		void GoStraight(int speed);
		int TurnTowardsTarget(float turnSpeed);

    private:
        static int IDCount;
    };
}

#endif
