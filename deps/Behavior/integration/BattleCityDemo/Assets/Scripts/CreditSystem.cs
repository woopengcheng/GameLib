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
using System.Collections.Generic;

public class CreditSystem
{
	private int tankCreditScore = 0;
	private int brickCreditScore = 0;
	private int bulletCreditScore = 0;
	private int awardCreditScore = 0;

	public CreditSystem()
	{
	}

	public bool init()
	{
		return true;
	}

	public void addTankKillCredit(Player tank)
	{
		tankCreditScore += tank.gameScore;
		GameLevelCommon.instance.uiManager.setTankCredit(tankCreditScore);
	}

	public void addDodgeFromDangerCredit(int dangerBulletCount)
	{
		bulletCreditScore += dangerBulletCount*30;
		GameLevelCommon.instance.uiManager.setBulletCredit(bulletCreditScore);
	}

	public void addBrickDestroyCredit(Brick gameBrick)
	{
		brickCreditScore += gameBrick.getGameScore();
		GameLevelCommon.instance.uiManager.setBrickCredit(brickCreditScore);
	}

	public void addAwardTakeCredit(Award gameAward)
	{
		awardCreditScore += gameAward.gameScore;
		GameLevelCommon.instance.uiManager.setAwardCredit(awardCreditScore);
	}
}
