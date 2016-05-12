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

#ifndef GAMELOGIC_H_
#define GAMELOGIC_H_

namespace framework
{
    class WorldState;
    class GameObject;
    class Ship;
    class Renderer;

    /// Coordinates the player and AI input to drive forward the worldState
    class GameLogic
    {
    public:
        /// Standard Constructor. Automatically gets keyboard, mouse, and player ship
        GameLogic();

        /// Step forward in time, get player input, and get AI actions
        bool step();
    };
}

#endif