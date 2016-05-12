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

#ifndef WORLDSTATE_H_
#define WORLDSTATE_H_

#include <list>
#include <queue>
#include <set>
#include "behaviac/agent/agent.h"
#include "behaviac/agent/registermacros.h"

#include "GameObject.h"
#include "point.h"

const static int kCOL_WIDTH = 640;
const static int kCOL_HEIGHT = 480;

const static int kWIDTH = kCOL_WIDTH + 80;
const static int kHEIGHT = kCOL_HEIGHT + 100;

namespace framework
{
	struct TextureResource;
    class GameObject;
	class Ship;

	typedef std::map<std::string, TextureResource*> TextureResourceMap;

    /// Convenience typedef for a standard list of GameObjects
    typedef std::list<GameObject*> GameObjectList;
    /// Convenience typedef for a GameObjectList iterator
    typedef GameObjectList::iterator GameObjectIter;
    /// Convenience typedef for a const GameObjectList iterator
    typedef GameObjectList::const_iterator ConstGameObjectIter;
    /// Determines how finely the world is broken down into cells which hold GameObjects
    /// A larger value means larger cells, and thus lower memory usage
#define coarseGraining 10

    /// Data structure that maps a collection of objects into a 2d space
	class WorldState : public behaviac::Agent
    {
    public:

		DECLARE_BEHAVIAC_OBJECT(framework::WorldState, behaviac::Agent);

        /// Width of a cell. Computed from the coarseGraining value
        int CellSizeX;
        /// Height of a cell. Computed from the coarseGraining value
        int CellSizeY;
        /// Width of the world
        float CoordinateSizeX;
        /// Height of the world
        float CoordinateSizeY;
        /// Current time in milliseconds. The absolute value doesn't mean anything, only differences
        unsigned int time;
        /// Stores the amount of real time between frames for e.g. physics updates
        unsigned int timeElapsed;
        /// Description of the level
		std::string description;
        /// Name of the level
		std::string name;
        /// ID of the level
        std::string id;

		unsigned int HealthHP;

        /// Standard Constructor
        WorldState();
		~WorldState();

        /// Insert the gameObject into the world at the given point
        /** \return true if the object was actually inserted, false if it could not be */
        bool insertObject(GameObject* gameObject, point p);
        /// Searches the world for an object with "isPlayer" set, and returns that
        GameObject* getPlayer() const;
        /// Marks the object for deletion. The memory is not actually freed until a convenient time, and until then the object will continue to interact with things (right now)
        bool deleteObject(GameObject* gameObject);
        /// Move an object that's already in the world to the specified point (checking for collisions)
        /**	\return true if the object was actually moved, false if it collided, went out of bounds, etc */
        bool moveObject(GameObject* gameObject, point p);
        /// Rotate an object in the world about its center (checking for collisions)
        /** \return true if the object was actually rotated, false if it collided, went out of bounds, etc */
        bool rotateObject(GameObject* gameObject, float angle);
        /// Get a list of all the objects in the region of a point
        GameObjectList* getAtCell(point p);
        /// Get a list of all the active gameObjects in the world
        const GameObjectList* getAllGameObjects() const;
        /// Free the memory of objects that have been marked for deletion
        void deleteQueued();

		void clearAIs();

        /// Get a list of all the inert gameObjects in the world
        const GameObjectList* getBackgroundObjects();
        /// Add an inert background object to the world. The object's location should already be set
        void addBackgroundObject(GameObject* background);
        /// Calculate the distance between obj1 and obj2. The distance is calculated based on their point locations, not on their full outlines.
        float distanceBetween(const GameObject* obj1, const GameObject* obj2) const;
        /// Calculate the angle from obj1 to obj2. The angles are in the range -180 to 180.
        float angleBetween(const GameObject* obj1, const GameObject* obj2) const;

		bool NextWave() const;
		void SpawnShip(int level);

		void SetPlayer(Ship* ship)
		{
			m_player = ship;
		}

		Ship* GetPlayer() const
		{
			return m_player;
		}

		void SetAI(Ship* ship)
		{
			m_ai = ship;
		}

		Ship* GetAI() const
		{
			return m_ai;
		}

		int PlayerHP() const;

		void ResetSyncSignal()
		{
			this->SyncSignal = false;
		}

		TextureResource* GetTextureResource(const char* filename);

    private:
		TextureResourceMap* textureResources;
        GameObjectList*** worldMatrix;
        GameObjectList* activeObjectList;
        GameObjectList* backgroundObjectList;
        bool pointOutofBounds(point p);
        GameObjectList* getListFromPoint(point p);
        GameObjectList* deleteList;
        void registerForDeletion(GameObject* obj);

		bool  SyncSignal;
		Ship* m_player;
		Ship* m_ai;
    };
}

#endif