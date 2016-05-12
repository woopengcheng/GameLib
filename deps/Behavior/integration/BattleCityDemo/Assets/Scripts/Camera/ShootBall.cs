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
using System.Linq;
using System.Collections;
using System.Collections.Generic;

public class ShootBall : MonoBehaviour 
{	
	public Camera targetCamera;
	public GameObject ballPrefab;
	public float magnitude;
    public float lifetime = 1.0f;

    private Vector3 lastPosition = Vector3.zero;
    private List<KeyValuePair<GameObject, float>> cleanup;

	void Start () 
    {
        this.cleanup = new List<KeyValuePair<GameObject, float>>();
	}
	
	void Update()
    {
		if (Input.GetKeyDown(KeyCode.F) == true)
        {
            Ray cursorRay = targetCamera.ScreenPointToRay(Input.mousePosition);
            GameObject newBall = 
                Instantiate(
                    ballPrefab, 
                    targetCamera.transform.position, 
                    targetCamera.transform.rotation) 
                as GameObject;
            Vector3 currentPosition = this.targetCamera.transform.position;
            Vector3 velocity = 
                (currentPosition - this.lastPosition) / Time.deltaTime;
			newBall.rigidbody.velocity = 
                velocity + (cursorRay.direction * magnitude);
            this.cleanup.Add(
                new KeyValuePair<GameObject, float>(
                    newBall, 
                    Time.time + this.lifetime));
		}

        foreach (KeyValuePair<GameObject, float> ball in this.cleanup)
            if (ball.Value < Time.time)
                GameObject.Destroy(ball.Key);
        this.cleanup.RemoveAll(n => n.Value < Time.time);
        this.lastPosition = this.targetCamera.transform.position;
	}
}
