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

public class FlyingCamera : MonoBehaviour
{
//    private float lookSpeed = 200.0f;
//    private float moveSpeed = 10.0f;
//    private float updownSpeed = 10.0f;
//
//    private float rotationX = 180.0f;
//    private float rotationY = -35.0f;
//
//	private float rotationXCur = 180.0f;
//	private float rotationYCur = -35.0f;
	//
	private float initWidth = 0.0f;
	private Vector3 initPosition = Vector3.zero;
	private Quaternion initRotate = Quaternion.identity;
	Camera cameraComponent = null;

	public void reset()
	{
		if(cameraComponent != null)
		{
			cameraComponent.orthographic = true;
			cameraComponent.orthographicSize = initWidth;
			cameraComponent.transform.localPosition = initPosition;
			cameraComponent.transform.localRotation = initRotate;
		}
	}

	void Start()
	{
//		rotationX 		= transform.eulerAngles.y;
//		rotationXCur 	= transform.eulerAngles.y;
//		rotationY 		= transform.eulerAngles.x;
//		rotationYCur	= transform.eulerAngles.x;

		cameraComponent = GetComponent<Camera>();
		if(cameraComponent != null)
		{
			initWidth = cameraComponent.orthographicSize;
			initPosition = cameraComponent.transform.localPosition;
			initRotate = cameraComponent.transform.localRotation;
		}
	}

    // Update is called once per frame
    void Update()
    {
		return;
//        if(Input.GetMouseButton(1))
//        {
//			rotationX += Input.GetAxis("Mouse X") * lookSpeed * Time.deltaTime;
//			rotationY -= Input.GetAxis("Mouse Y") * lookSpeed * Time.deltaTime;
//            rotationY = Mathf.Clamp(rotationY, -90, 90);
//        }
//
//        //transform.localRotation = Quaternion.AngleAxis(rotationX, Vector3.up);
//        //transform.localRotation *= Quaternion.AngleAxis(rotationY, Vector3.left);
//		// make rotateToAngle smooth
//		rotationXCur = Mathf.LerpAngle(rotationXCur, rotationX, Time.deltaTime*8);
//		rotationYCur = Mathf.Lerp(rotationYCur, rotationY, Time.deltaTime*8);
//		transform.rotation = Quaternion.Euler(rotationYCur, rotationXCur, 0);
//
//		float v = Input.GetAxis("Vertical");
//		float h = Input.GetAxis("Horizontal");
//        float deltaMove = Time.deltaTime * this.moveSpeed;
//		transform.position += transform.forward * v * deltaMove;
//		transform.position += transform.right * h * deltaMove;
//
//		float scrollDelta = Input.GetAxis("Mouse ScrollWheel");
//		transform.position += transform.forward * scrollDelta * deltaMove * 40.0f;
//
//        float deltaUpdown = Time.deltaTime * this.updownSpeed;
//        if (Input.GetKey(KeyCode.R))
//            transform.position += Vector3.up * deltaUpdown;
//        if (Input.GetKey(KeyCode.F))
//            transform.position += Vector3.up * -deltaUpdown;
    }
}
