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

#include "Renderer.h"
#include "globals.h"

namespace framework
{
	Renderer::Renderer()
	{
		screenStartX = 0.0;
		screenStartY = 0.0;
		screenWidth = kWIDTH;
		screenHeight = kHEIGHT;
		camera = ws->getPlayer();
	}

	bool Renderer::setCamera(GameObject* obj)
	{
		camera = obj;
		return true;
	}

	bool Renderer::Render()
	{
		const GameObjectList* backgrounds = ws->getBackgroundObjects();
		const GameObjectList* objects = ws->getAllGameObjects();

		ConstGameObjectIter itr;

		for (itr = backgrounds->begin(); itr != backgrounds->end(); itr++)
		{
			GameObject* obj = *itr;
			if (obj->textureResource && obj->textureResource->texture)
			{
				SDL_RenderCopy(this->sdlrenderer, obj->textureResource->texture, NULL, NULL);
			}
		}

		for (itr = objects->begin(); itr != objects->end(); itr++)
		{
			GameObject* obj = *itr;
			if (obj->textureResource && obj->textureResource->texture)
			{
				SDL_Rect destination;
				destination.x = (int)obj->location.x;
				destination.y = (int)obj->location.y;
				destination.w = obj->textureResource->width;
				destination.h = obj->textureResource->height;
				SDL_RenderCopyEx(this->sdlrenderer, obj->textureResource->texture, NULL, &destination, obj->displayHeading, NULL, SDL_FLIP_NONE);

				//if (obj->usesPhysics)
				//{
				//	SDL_SetRenderDrawColor(this->sdlrenderer, 255, 255, 255, 0);
				//	SDL_RenderDrawRect(this->sdlrenderer, &destination);
				//}
			}
		}

		return true;
	}
}//namespace framework