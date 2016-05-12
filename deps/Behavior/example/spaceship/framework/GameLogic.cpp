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

#include "GameLogic.h"
#include "GameObject.h"
#include "Ship.h"
#include "Definitions.h"
#include "globals.h"
#include "Projectile.h"

namespace framework
{
	GameLogic::GameLogic()
	{
		behaviac::Workspace::GetInstance()->SetTimeSinceStartup(0);
	}

	bool GameLogic::step()
	{
		ws->deleteQueued(); // Delete all the things that were destroyed last frame now, since it won't interfere with iterators, etc.
		unsigned int currentTime = SDL_GetTicks(); //Since the framerate might not be steady, need to see how much actual time has gone by
		ws->timeElapsed = currentTime - ws->time;
		ws->time = currentTime;

		const GameObjectList* objects = ws->getAllGameObjects();

		float dt = ws->timeElapsed / 1000.0f;

		behaviac::Workspace::GetInstance()->SetTimeSinceStartup(behaviac::Workspace::GetInstance()->GetTimeSinceStartup() + dt);

		behaviac::Workspace::GetInstance()->Update();

		for (ConstGameObjectIter itr = objects->begin(); itr != objects->end(); ++itr)
		{
			GameObject* g = *itr;
			g->age += ws->timeElapsed;
			g->doActions(dt);
		}

		return true;
	}
}//namespace framework