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

#ifndef RENDERER_H_
#define RENDERER_H_

#include <map>
#include <stdlib.h>
#include "SDL.h"

struct SDL_Texture;

namespace framework
{
    class WorldState;
    class GameObject;

    /// Render sprites from the worldState to the graphics context
    class Renderer
    {
    public:
        /// Constructor. Initialize resources and get the camera from the worldState
        Renderer();

        /// Render to the global graphics context
        bool Render();

		/// Set which object to center the display on
        bool Renderer::setCamera(GameObject* obj);

        /// The width of the screen, in pixels
        int screenWidth;
        /// The height of the screen, in pixels
        int screenHeight;

		SDL_Renderer* sdlrenderer;

    private:
        GameObject* camera;
        double screenStartX;
        double screenStartY;
    };
}

#endif