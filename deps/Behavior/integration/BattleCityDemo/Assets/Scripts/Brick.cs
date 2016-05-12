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
using System;

public enum eBrickType{EMPTY, TREE, AWARD, ICE, SOIL, ACTOR, HEADQUATER, STEEL, OCEAN, OBSTACLE, PATHRESULT, TOTAL};

[ExecuteInEditMode]
public class Brick : MonoBehaviour 
{
	public eBrickType type = eBrickType.SOIL;
	public Transform ltPart;
	public Transform rtPart;
	public Transform rbPart;
	public Transform lbPart;

	public bool partFirst = true;
	public bool partSecond = true;
	public bool partThird = true;
	public bool partFourth = true;

	public int getGameScore()
	{
		if(type == eBrickType.SOIL)
			return 20;
		else if(type == eBrickType.STEEL)
			return 40;
		else
			return 0;
	}

	void Awake()
	{
	}
	
	// Use this for initialization
	void Start () 
	{

	}
	
	// Update is called once per frame
	void Update () 
	{
		ltPart.gameObject.SetActive(partFirst);
		rtPart.gameObject.SetActive(partSecond);
		lbPart.gameObject.SetActive(partThird);
		rbPart.gameObject.SetActive(partFourth);
	}

	public void shootByBullet(string name, int damage)
	{
		if((damage < 2 && type == eBrickType.STEEL) || type == eBrickType.OCEAN ||
		   type == eBrickType.TREE || type == eBrickType.ICE)
			return;

		int idx = name.IndexOf("_");
		string strNum = name.Substring(idx + 1);
		int subBrickIndex = Convert.ToInt32(strNum);
		switch(subBrickIndex)
		{
		case 0:
			partFirst = false;
			break;
		case 1:
			partSecond = false;
			break;
		case 2:
			partThird = false;
			break;
		case 3:
			partFourth = false;
			break;
		}

		if(!partFirst && !partSecond && !partThird && !partFourth)
		{
			int _2dx = -1; int _2dy = -1;
			GameLevelCommon._3dTo2d(this.transform.position.x, this.transform.position.z, out _2dx, out _2dy);
			GameLevelCommon.instance.navData[_2dy, _2dx] = (byte)eBrickType.EMPTY;
			GameLevelCommon.instance.creditSystem.addBrickDestroyCredit(this);
			GameObject.Destroy(this.gameObject);
		}
	}
}
