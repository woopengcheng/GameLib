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

#ifndef point_h_
#define point_h_
#include <iostream>
#include <math.h>
#include <cmath>

namespace framework
{
    struct point
    {
    public:
        point(float x, float y);
        point();

        /// Create a new point at the given offset from this point
        point point::offsetRect(float ox, float oy);
        /// Create a new point at the given offset from this point
        point point::offsetPolar(float heading, float distance);
        /// Calculate the angle (in radians) to another point
        float angleTo(point p);

        /// Generate a string from the point, for easy display
		std::string point::toString();
        /// Retain the direction but make the magnitude 1
        void normalize();
        /// The distance from (0,0) to this point
        float vectorSize();

        point operator/= (float k);
        point operator*= (float k);
        point operator- (const point& right) const;
        point operator+ (const point& right) const;

        float x;
        float y;
    };
}

#endif
