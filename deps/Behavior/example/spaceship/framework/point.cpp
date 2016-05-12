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

#include "point.h"
#include "util.h"
#include "behaviac/base/core/assert_t.h"
#include "behaviac/base/core/compiler.h"

namespace framework
{
	point::point(float i, float j)
	{
		x = i;
		y = j;
	}

	point::point()
	{
		x = 0;
		y = 0;
	}

	point point::offsetRect(float ox, float oy)
	{
		return point(x + ox, y + oy);
	}

	point point::offsetPolar(float heading, float distance)
	{
		float r = getRadians(heading);
		return point(x - distance * sin(-r), y - distance * cos(-r));
	}

	float point::angleTo(point p)
	{
		return atan2(p.y, p.x) - atan2(y, x);
	}

	void point::normalize()
	{
		float length = vectorSize();
		x /= length;
		y /= length;
	}

	std::string point::toString()
	{
		char buffer[128];
		string_sprintf(buffer, "%f,%f", x, y);
		std::string ret(buffer);
		return ret;
	}

	float framework::point::vectorSize()
	{
		return sqrtf(x * x + y * y);
	}

	point point::operator+ (const point& right) const
	{
		return point(x + right.x, y + right.y);
	}

	point point::operator- (const point& right) const
	{
		return point(x - right.x, y - right.y);
	}

	point point::operator*= (float k)
	{
		x *= k;
		y *= k;
		return *this;
	}

	point point::operator/= (float k)
	{
		x /= k;
		y /= k;
		return *this;
	}

}//namespace framework