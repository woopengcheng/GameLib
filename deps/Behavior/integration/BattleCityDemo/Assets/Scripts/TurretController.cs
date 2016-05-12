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

using UnityEngine;
using System.Collections;

public class TurretController
{
	public Transform	turret = null;
	public float		azimuth = 0.0f;
	public float		targetAzimuth = 0.0f;
	public bool			isRotating = false;

	public static float calcSpinFactor(float targetAngle, float currentOrientation)
	{
		float deltaAzimuth = targetAngle - currentOrientation;
		float sign = Mathf.Sign(deltaAzimuth);

		float spinDirection = 1.0f;
		if(deltaAzimuth > 180.0f || deltaAzimuth < -180.0f)
			spinDirection = -1.0f;
		else if(deltaAzimuth == 0.0f)
			spinDirection = 0.0f;

		float factor = sign*spinDirection;
		return factor;
	}

	public static float cuteRotate(float currentOrientation, float targetAngle, float rotateSpeed, out float rotateFactor)
	{
		float spinFactor = calcSpinFactor(targetAngle, currentOrientation);		
		rotateFactor = spinFactor;
		
		float deltaRotate = Time.deltaTime*rotateSpeed*spinFactor;
		currentOrientation += deltaRotate;
		float newFactor = calcSpinFactor(targetAngle, currentOrientation);
		if(newFactor*spinFactor < 0.0f)
			currentOrientation = targetAngle;
		
		if(currentOrientation < 0.0f)
			currentOrientation += 360.0f;
		if(currentOrientation > 360.0f)
			currentOrientation -= 360.0f;
		
		return currentOrientation;
	}

	public void tick(float turretRotateSpeed)
	{
		float spinFactor = 0.0f;
		azimuth = cuteRotate(azimuth, targetAzimuth, turretRotateSpeed, out spinFactor);
		if(spinFactor == 0.0f)
			isRotating = false;
		else
			isRotating = true;

		turret.transform.rotation = Quaternion.Euler(270, azimuth, 0);
	}
}
