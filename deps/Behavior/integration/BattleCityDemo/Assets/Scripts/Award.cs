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

public enum eGameAward{POWER, ACCELERATE, LIFE, DEFENSE, TOTAL, UNKNOWN}

public class Award : MonoBehaviour 
{
	public eGameAward type = eGameAward.UNKNOWN;
	public float lifeTime = 5.0f;
	public int id = -1;

	public int gameScore = 0;

	public virtual void takeAward(Player p)
	{
	}

	void OnTriggerEnter(Collider other)
	{
		Player p_ = other.transform.GetComponent<Player>();
		if(p_ != null)
		{
			GameLevelCommon.instance.creditSystem.addAwardTakeCredit(this);
			takeAward(p_);
			GameLevelCommon.instance.destroyAward(this);

			string soundName = "";
			switch(type)
			{
			case eGameAward.ACCELERATE:
				soundName = "Sounds/AccelarateAward";
				break;
			case eGameAward.POWER:
				soundName = "Sounds/PowerAward";
				break;
			case eGameAward.DEFENSE:
				soundName = "Sounds/DefenseAward";
				break;
			case eGameAward.LIFE:
				soundName = "Sounds/LifeAward";
				break;
			}

			SoundEffectPlayer.Instance.playSound(soundName, 5, 0.0f, 1.0f, false);
		}
	}

	protected void tick()
	{
		lifeTime -= Time.deltaTime;
		if(lifeTime < 0.0f)
		{
			if (GameLevelCommon.instance != null)
			{
				GameLevelCommon.instance.destroyAward(this);
			}
			lifeTime = 0.0f;
		}
		transform.localRotation = Quaternion.Euler(0, Time.time*120.0f, 0);

		Vector3 position = transform.localPosition;
		float factor = Mathf.Abs(Mathf.Sin(Time.time))*1.0f;
		position.y = factor;
		transform.localPosition = position;

		float scale = 0.8f - Mathf.Abs(Mathf.Sin(Time.time))*0.3f;
		transform.localScale = new Vector3(scale, 1, scale);
	}
}
