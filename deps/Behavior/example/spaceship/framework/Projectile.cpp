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

#include "Projectile.h"
#include "globals.h"

namespace framework
{
	BEGIN_PROPERTIES_DESCRIPTION(Projectile)
		//
	END_PROPERTIES_DESCRIPTION()

	Projectile::Projectile() : GameObject()
	{
		displayName = "projectile";
		faction = PROJECTILE_FACTION;
	}

	bool Projectile::doActions(float dt)
	{
		super::doActions(dt);
		return true;
	}

	bool Projectile::registerWallCollision()
	{
		printf("projectile wall collision\n");
		ws->deleteObject(this);
		return true;
	}

	bool Projectile::registerCollision(GameObject* collidedObject)
	{
		printf("projectile collision\n");
		ws->deleteObject(this);
		return true;
	}

}//namespace framework