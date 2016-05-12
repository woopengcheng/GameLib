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

public class Bullet : MonoBehaviour 
{
	public Vector3 initDirection = Vector3.zero;
	public float speed = 35.0f;
	public float lifeTime = 2.0f;
	public int damage = 1;
	public eGameForce force = eGameForce.RED_FORCE;

	public int locomotionKey = int.MinValue;

	private bool alive = false;
	
	// Use this for initialization
	void Start ()
	{
		MeshRenderer mr = GetComponent<MeshRenderer>();
		if(damage > 1)
			mr.material.color = Color.red;
		else
			mr.material.color = Color.white;

		alive = true;
	}

	void OnCollisionEnter(Collision collision)
	{
		//GameObject.Destroy(this.gameObject);
	}

	void destroyBullet()
	{
		alive = false;
		GameLevelCommon.instance.dangerRecord.removeBullet(this);
		Transform dynamicEffectAnchor = GameLevelCommon.instance.dynamicEffectAnchor;
		GameLevelCommon.instance.spawnEffect("Effects/Explosions/Explosion_0", dynamicEffectAnchor, transform.position, 0.5f);
		GameObject.Destroy(this.gameObject);
	}

	void OnTriggerEnter(Collider collider)
	{
		if(!alive)
			return;

		if(collider.name.Contains("Brick"))
		{
			Brick br = collider.transform.parent.GetComponent<Brick>();
			if(br != null && br.type == eBrickType.OCEAN)
				return;
		}
		else
		{
			string layerName = LayerMask.LayerToName(collider.gameObject.layer);
			if(GameLevelCommon.instance.isMelee)
			{
				if(layerName == "AwardLayer")
                	return;
			}
			else
			{
				if((layerName == "NpcLayer" && force == eGameForce.BLUE_FORCE) || (layerName == "AwardLayer"))
					return;
			}
		}

		if(force == eGameForce.RED_FORCE)
		{
			SoundEffectPlayer.Instance.playSound("Sounds/Shooted", 1, 0.0f, 1.0f, false);
		}

		Transform tran_ = collider.transform;
		Player p_ = tran_.GetComponent<Player>();
		if(p_ != null)
			p_.flashWhite();

		if(p_ != null && p_.shieldInteval == 0.0f)
		{
			if((--p_.hp) == 0)
			{
				if(force == eGameForce.RED_FORCE)
				{
					GameLevelCommon.instance.creditSystem.addTankKillCredit(p_);
				}

				Vector3 tr = p_.transform.position + new Vector3(0, 2.0f, 0);
				Transform dynamicEffectAnchor = GameLevelCommon.instance.dynamicEffectAnchor;
				GameLevelCommon.instance.spawnEffect("Effects/Explosions/Explosion_0", dynamicEffectAnchor, tr, 2.0f);
				GameLevelCommon.instance.destroyGameActor(p_);
			}
		}

		destroyBullet();
	}
	
	// Update is called once per frame
	void Update ()
	{
		if(lifeTime > 0.0f)
		{
			transform.position += speed*Time.deltaTime*initDirection;
			lifeTime -= Time.deltaTime;
		}
		else
		{
			destroyBullet();
		}
	}
}
