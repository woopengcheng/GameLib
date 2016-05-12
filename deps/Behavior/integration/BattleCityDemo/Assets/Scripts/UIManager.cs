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

public class UIManager : MonoBehaviour 
{
	public Transform gameOver;
	public Transform gameWin;
	public Transform uiCameraObject;

	public Transform awardCreditObject;
	public Transform brickCreditObject;
	public Transform tankCreditObject;
	public Transform bulletCreditObject;
	public Transform totalCreditObject;

	public Transform creditUIObject;

	public Transform gameUI;

	public Camera uiCamera = null;

	public bool showCreditUI = true;

	private int awardScore = 0;
	private int bulletScore = 0;
	private int tankScore = 0;
	private int brickScore = 0;
	// Use this for initialization
	void Start () 
	{
		init();
	}
	
	// Update is called once per frame
	void Update () 
	{
	
	}

	void updateTotalCredit()
	{
		UIScoreNumber sn = totalCreditObject.GetComponent<UIScoreNumber>();
		sn.textColor = Color.red;
		sn.number = awardScore + bulletScore + tankScore + brickScore;
	}

	public void setTankCredit(int score)
	{
		UIScoreNumber sn = tankCreditObject.GetComponent<UIScoreNumber>();
		sn.number = score;
		tankScore = score;
		updateTotalCredit();
	}

	public void setAwardCredit(int score)
	{
		UIScoreNumber sn = awardCreditObject.GetComponent<UIScoreNumber>();
		sn.number = score;
		awardScore = score;
		updateTotalCredit();
	}

	public void setBrickCredit(int score)
	{
		UIScoreNumber sn = brickCreditObject.GetComponent<UIScoreNumber>();
		sn.number = score;
		brickScore = score;
		updateTotalCredit();
	}

	public void setBulletCredit(int score)
	{
		UIScoreNumber sn = bulletCreditObject.GetComponent<UIScoreNumber>();
		sn.number = score;
		bulletScore = score;
		updateTotalCredit();
	}

	public bool init()
	{
		gameWin.gameObject.SetActive(false);
		gameOver.gameObject.SetActive(false);

		gameUI.gameObject.SetActive(showCreditUI);

		uiCamera = uiCameraObject.GetComponent<Camera>();
		uiCamera.clearFlags = CameraClearFlags.Nothing;
		uiCamera.enabled = true;
		uiCamera.backgroundColor = Color.black;

		return false;
	}

	public void showCredit()
	{
		gameUI.gameObject.SetActive(true);		
		uiCamera.clearFlags = CameraClearFlags.SolidColor;
		Animator ani = creditUIObject.GetComponent<Animator>();
		ani.CrossFade("creditExhibition", 0.1f);
	}

	public void showUI(bool doesWin)
	{
		gameUI.gameObject.SetActive(false);

		uiCamera.clearFlags = CameraClearFlags.SolidColor;
		if(doesWin)
		{
			gameWin.gameObject.SetActive(true);
			gameOver.gameObject.SetActive(false);
		}
		else
		{
			gameWin.gameObject.SetActive(false);
			gameOver.gameObject.SetActive(true);
		}

		uiCamera.enabled = true;
	}

	public void hideUI()
	{
		uiCamera.enabled = false;
	}
}
