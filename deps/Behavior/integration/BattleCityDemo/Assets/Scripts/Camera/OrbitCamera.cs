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

public class OrbitCamera : MonoBehaviour
{	
	protected Transform targetTransform = null;

	public float distance = 50.0f;
	public float verticalAngle = 45.0f;
	public float horizonalAngle = 0.0f;

	void Start()
	{
		reset();
	}
	
	public void reset()
	{
		GameObject targetObject = GameObject.Find("@Player");
		if(targetObject == null)
			return;
		targetTransform = targetObject.transform;

		Vector3 eulerAngles = targetTransform.localRotation.eulerAngles;
		horizonalAngle = eulerAngles.y;

		Quaternion quat = Quaternion.Euler(verticalAngle, horizonalAngle, 0.0f);
		Vector3 vec = quat*Vector3.forward;

		transform.localPosition = targetTransform.localPosition - distance*vec;
		transform.localRotation = quat;

		Camera cameraComponent = GetComponent<Camera>();
		cameraComponent.orthographic = false;
		cameraComponent.fieldOfView = 45.0f;
	}
	
	void Update () 
	{
		if(targetTransform != null)
		{
			Quaternion quat = Quaternion.Euler(verticalAngle, horizonalAngle, 0.0f);
			Vector3 vec = quat*Vector3.forward;
			transform.localPosition = targetTransform.localPosition - distance*vec;
			transform.localRotation = quat;
			
			distance *= (1-1*Input.GetAxis("Mouse ScrollWheel"));
		}
		else
		{
			reset();
		}
	}
}
