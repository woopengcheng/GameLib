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

public class InputManager : Singleton<InputManager>
{
	private Transform mainCamera = null;
	private int fingerCountPrevious = 0;

	private Vector3 cameraStartPoint = Vector3.zero;
	private Vector3 firstTouchPoint_0 = Vector3.zero;
	private Vector3 firstTouchPoint_1 = Vector3.zero;

	public InputManager()
	{
	}

	public void setMainCamera(Transform camera)
	{
		mainCamera = camera;
	}

	public void tick()
	{
		bool usingUnityRemote = (Application.platform == RuntimePlatform.WindowsEditor || Application.platform == RuntimePlatform.WindowsPlayer)
								&& Input.touchCount != 0;
		if(Application.platform == RuntimePlatform.Android || 
		   Application.platform == RuntimePlatform.IPhonePlayer || usingUnityRemote)
		{
			mobilePlatformInputs();
		}
		else
		{
			windowsPlatformInputs();
		}

		fingerCountPrevious = Input.touchCount;
	}

	void windowsPlatformInputs()
	{
		if(Input.GetMouseButton(0)) 
		{
			OrbitCamera oCamera = mainCamera.GetComponent<OrbitCamera>();
			if(oCamera.enabled)
			{
				oCamera.horizonalAngle += Input.GetAxis("Mouse X")*5;
				oCamera.verticalAngle -= Input.GetAxis("Mouse Y")*5;
			}
		}
	}

	void mobilePlatformInputs()
	{
		Camera mainCameraComponent = mainCamera.GetComponent<Camera>();
		Touch[] fingers = Input.touches;
		if(mainCameraComponent.isOrthoGraphic)
		{
			if(Input.touchCount == 1)
			{
				Touch touch_0 = fingers[0];
				Vector3 currentFingerPosition = getViewPositionFromTouch(mainCameraComponent, touch_0.position);
				if(Input.touchCount != fingerCountPrevious)
				{
					cameraStartPoint = mainCamera.transform.localPosition;
					firstTouchPoint_0 = currentFingerPosition;
				}
				
				Vector3 delta = firstTouchPoint_0 - currentFingerPosition;
				mainCamera.transform.localPosition = cameraStartPoint + delta;
			}
			else if(Input.touchCount == 2)
			{
				Touch touch_0 = fingers[0];
				Touch touch_1 = fingers[1];
				
				Vector3 vp_0 = getViewPositionFromTouch(mainCameraComponent, touch_0.position);
				Vector3 vp_1 = getViewPositionFromTouch(mainCameraComponent, touch_1.position);
				if(Input.touchCount != fingerCountPrevious)
				{
					firstTouchPoint_0 = vp_0;
					firstTouchPoint_1 = vp_1;
				}
				
				float firstDeltaDistance = Mathf.Abs((firstTouchPoint_0 - firstTouchPoint_1).magnitude);
				float currentDeltaDistance = Mathf.Abs((vp_0 - vp_1).magnitude);
				mainCameraComponent.orthographicSize = mainCameraComponent.orthographicSize*(firstDeltaDistance/currentDeltaDistance); 
			}
		}
		else
		{
			if(Input.touchCount == 1)
			{
				Touch touch_0 = fingers[0];
				OrbitCamera oCamera = mainCamera.GetComponent<OrbitCamera>();
				if(oCamera.enabled)
				{
					oCamera.horizonalAngle += touch_0.deltaPosition.x;
					oCamera.verticalAngle -= touch_0.deltaPosition.y;
				}
			}
		}
	}

	Vector3 getViewPositionFromTouch(Camera cameraComponent, Vector3 position)
	{
		Vector3 touchPosition = new Vector3(position.x, position.y, 0);
		Vector3 viewPortRatioPoint = cameraComponent.ScreenToViewportPoint(touchPosition);
		viewPortRatioPoint.z = 0.0f;
		viewPortRatioPoint = (viewPortRatioPoint - new Vector3(0.5f, 0.5f, 0))*2.0f;
		float verticalCoord = viewPortRatioPoint.y*cameraComponent.orthographicSize;
		float horizonalCoord = viewPortRatioPoint.x*cameraComponent.orthographicSize*cameraComponent.aspect;
		Vector3 viewPortPosition = new Vector3(horizonalCoord, 0, verticalCoord);
		return viewPortPosition;
	}
}
