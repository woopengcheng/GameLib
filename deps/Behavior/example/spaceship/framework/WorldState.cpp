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

#include "SDL_image.h"

#include "WorldState.h"
#include "GameObject.h"
#include "Projectile.h"
#include "util.h"
#include "Ship.h"

namespace framework
{
	BEGIN_PROPERTIES_DESCRIPTION(WorldState)
		REGISTER_PROPERTY(time);
		REGISTER_PROPERTY(HealthHP);
		REGISTER_PROPERTY(SyncSignal);

		REGISTER_METHOD(NextWave);
		REGISTER_METHOD(PlayerHP);

		REGISTER_METHOD(SpawnShip).PARAM_DISPLAY_INFO(L"level");
	END_PROPERTIES_DESCRIPTION()

	WorldState::WorldState() : m_player(0), m_ai(0), SyncSignal(false)
	{
		description = "";
		CoordinateSizeX = kCOL_WIDTH;
		CoordinateSizeY = kCOL_HEIGHT;

		CellSizeX = (int)CoordinateSizeX / coarseGraining ;
		CellSizeY = (int)CoordinateSizeY / coarseGraining ;

		textureResources = BEHAVIAC_NEW TextureResourceMap();
		activeObjectList = BEHAVIAC_NEW GameObjectList();
		deleteList = BEHAVIAC_NEW GameObjectList();
		backgroundObjectList = BEHAVIAC_NEW GameObjectList();
		worldMatrix = (GameObjectList***)BEHAVIAC_MALLOC(sizeof(GameObjectList**) * CellSizeX);

		for (int i = 0; i < CellSizeX; ++i)
		{
			worldMatrix[i] = (GameObjectList**)BEHAVIAC_MALLOC(sizeof(GameObjectList*) * CellSizeY);
		}

		for (int i = 0; i < CellSizeX; i++)
		{
			for (int j = 0; j < CellSizeY; j++)
			{
				worldMatrix[i][j] = BEHAVIAC_NEW GameObjectList();
			}
		}

		HealthHP = 50;
		time = SDL_GetTicks();
		time = 0;
	}

	WorldState::~WorldState()
	{
		if (textureResources)
		{
			for (TextureResourceMap::iterator it = textureResources->begin(); it != textureResources->end(); ++it)
			{
				if (it->second && it->second->texture)
					SDL_DestroyTexture(it->second->texture);
			}
			textureResources->clear();

			BEHAVIAC_DELETE textureResources;
		}
	}

	TextureResource* WorldState::GetTextureResource(const char* filename)
	{
		TextureResource* textureRes = NULL;
		TextureResourceMap::iterator it = textureResources->find(filename);
		if (it != textureResources->end())
		{
			textureRes = it->second;
		}
		else
		{
			SDL_Texture* texture = IMG_LoadTexture(framework::renderer->sdlrenderer, filename);
			if (texture)
			{
				textureRes = BEHAVIAC_NEW TextureResource;
				textureRes->texture = texture;
				SDL_QueryTexture(texture, NULL, NULL, &textureRes->width, &textureRes->height);
				(*textureResources)[filename] = textureRes;
			}
		}

		return textureRes;
	}

	GameObject* WorldState::getPlayer() const
	{
		GameObjectIter itr;

		for (itr = activeObjectList->begin(); itr != activeObjectList->end(); itr++)
		{
			if ((*itr)->isPlayer)
			{
				return *itr;
			}
		}

		return NULL;
	}

	bool WorldState::insertObject(GameObject* gameObject, point p)
	{
		if (pointOutofBounds(p))
		{
			return false;
		}

		GameObjectList* currentList = getListFromPoint(p);
		currentList->push_front(gameObject);
		gameObject->location = p;
		activeObjectList->push_front(gameObject);

		return true;
	}

	bool WorldState::deleteObject(GameObject* gameObject)
	{
		registerForDeletion(gameObject);
		return true;
	}

	void WorldState::clearAIs()
	{
		this->deleteQueued();

		for (GameObjectIter itr = activeObjectList->begin(); itr != activeObjectList->end();)
		{
			GameObjectIter t = itr++;
			GameObject* p = *t;

			if (!p->isPlayer)
			{
				GameObjectList* currentList = getListFromPoint(p->location);
				currentList->remove(p);

				BEHAVIAC_DELETE(p);

				activeObjectList->erase(t);
			}
		}
	}

	bool WorldState::moveObject(GameObject* gameObject, point p)
	{
		if (pointOutofBounds(p))
		{
			gameObject->registerWallCollision();
			return false;
		}

		//test for and report potential collisions
		//currently tests against all objects (does not scale well)
		//update with more sophisticated filtering in the future

		GameObjectIter itr;

		if (gameObject->usesPhysics)
		{
			for (itr = activeObjectList->begin(); itr != activeObjectList->end(); itr++)
			{
				if ((*itr) == gameObject)
					continue;

				if ((*itr)->faction == PROJECTILE_FACTION && (*itr)->owner != ws->getPlayer() && (gameObject->isPlayer == false))
					continue;

				if (gameObject->faction == PROJECTILE_FACTION && gameObject->owner != ws->getPlayer() && ((*itr)->isPlayer == false))
					continue;

				if (((*itr)->owner == gameObject)|| (gameObject->owner == *itr))
					continue;

				if ((*itr)->usesPhysics && (*itr)->collideWith(gameObject))
				{
					(*itr)->registerCollision(gameObject);
					gameObject->registerCollision(*itr);

					return false; //TODO support colliding with multiple entities
				}
			}
		}

		GameObjectList* currentList = getListFromPoint(gameObject->location);
		GameObjectList* newList = getListFromPoint(p);
		gameObject->location = p;

		if (currentList != newList)
		{
			newList->push_front(gameObject);
			currentList->remove(gameObject);
		}

		return true;
	}

	bool WorldState::rotateObject(GameObject* gameObject, float angle)
	{
		if (gameObject->usesPhysics)
		{
			GameObjectIter itr;

			for (itr = activeObjectList->begin(); itr != activeObjectList->end(); itr++)
			{
				if ((*itr) == gameObject)
					continue;

				if ((*itr)->faction == PROJECTILE_FACTION && (gameObject->faction == PROJECTILE_FACTION))
					continue;

				if (((*itr)->owner == gameObject) || (gameObject->owner == *itr))
					continue;

				if ((*itr)->usesPhysics && (*itr)->collideWith(gameObject))
				{
					//(*itr)->registerCollision(gameObject);
					//gameObject->registerCollision(*itr);

					return false; //TODO support colliding with multiple entities
				}
			}
		}

		gameObject->displayHeading += angle;
		gameObject->displayHeading = mod(gameObject->displayHeading, 360.0f);
		return true;
	}

	void WorldState::registerForDeletion(GameObject* obj)
	{
		deleteList->push_front(obj);
	}

	void WorldState::deleteQueued()
	{
		ConstGameObjectIter itr;
		deleteList->sort();
		deleteList->unique();

		for (itr = deleteList->begin(); itr != deleteList->end(); ++itr)
		{
			GameObject* p = *itr;
			GameObjectList* currentList = getListFromPoint(p->location);
			activeObjectList->remove(p);
			currentList->remove(p);
			BEHAVIAC_DELETE(p);
		}

		deleteList->clear();
	}

	void WorldState::addBackgroundObject(GameObject* background)
	{
		backgroundObjectList->push_back(background);
	}

	const GameObjectList* WorldState::getBackgroundObjects()
	{
		return backgroundObjectList;
	}

	GameObjectList* WorldState::getAtCell(point p)
	{
		if (pointOutofBounds(p))
		{
			return NULL;
		}

		return getListFromPoint(p);
	}

	const GameObjectList* WorldState::getAllGameObjects() const
	{
		return  activeObjectList;
	}

	bool WorldState::pointOutofBounds(point p)
	{
		if (p.x >= CoordinateSizeX || p.y >= CoordinateSizeY || p.x < 0 || p.y < 0)
		{
			return true;
		}

		return false;
	}

	GameObjectList* WorldState::getListFromPoint(point p)
	{
		return worldMatrix[(int)p.x / coarseGraining][(int)p.y / coarseGraining];
	}

	float WorldState::distanceBetween(const GameObject* obj1, const GameObject* obj2) const
	{
		point resultantVector(obj1->location.x - obj2->location.x, obj1->location.y - obj2->location.y);
		return resultantVector.vectorSize();
	}

	float WorldState::angleBetween(const GameObject* obj1, const GameObject* obj2) const
	{
		point resultantVector(obj1->location.x - obj2->location.x, obj1->location.y - obj2->location.y);
		resultantVector.normalize();

		float angleBetween = getAngle(atan2(resultantVector.y, resultantVector.x));
		angleBetween -= obj1->displayHeading + 90.f;
		angleBetween = mod(angleBetween, 360.0f);
		if (angleBetween > 180.0f)
		{
			angleBetween = 180.0f - angleBetween;
		}

		return angleBetween;
	}

	bool WorldState::NextWave() const
	{
		if (this->getAllGameObjects()->size() == 1) //the player and the spawner
		{
			/////BEHAVIAC_APPLOG("", "NextWave", 0);
			/////BEHAVIAC_APPLOG("RED", "NextWave", 0);
			/////BEHAVIAC_APPLOG("GREEN", "NextWave", 0);

			return true;
		}

		return false;
	}

	int WorldState::PlayerHP() const
	{
		assert(m_player);
		return m_player->getHealth();
	}

}//namespace framework
