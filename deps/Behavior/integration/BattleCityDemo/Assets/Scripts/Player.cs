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

[behaviac.TypeMetaInfo("Player", "Player -> GameActor")]
[AddComponentMenu("BattleCity/Player")]
public class Player : GameActor 
{
	public int id = -1;
	public Color forceColor = Color.green;
	public int fireInterval = 1500;
	public int twoBulletInterval = 300;
	public float aimSpeed = 180.0f;
	public float bulletLifeTime = 2.5f;
	
	[behaviac.MemberMetaInfo()]
	public int hp = 1;

	[behaviac.MemberMetaInfo()]
	public int damageLevel = 1;

	private float fireTimer 		= 0.0f;
	private bool enableFire 		= false;
	private bool isFiring			= false;

	private Transform anchorNode 	= null;
	private bool swithColorFlash 	= false;
	private GameObject shieldObject = null;
	private float colorTimeLerp 	= 0.0f;

	protected TurretController turretCtrl = null;
	protected Transform firePoint 	= null;
	protected int bulletCountPerShoot = 1;
	protected bool btloadResult 	= false;
	protected Hashtable meshTable 	= null;

	public float shieldInteval 		= 0.0f;
	public BubbleHud bubbleHud 		= null;
	public string behaviorTree 		= "";
	public int initLifeCount 		= 0;

	public int playerLifeCount
	{
		set
		{
 			if(value >= 0)
				GameLevelCommon.instance.actorLifeTable[id] = value;
		}

		get 
		{ 
			int life = 0;
			bool isFound = GameLevelCommon.instance.actorLifeTable.ContainsKey(id);
			if(isFound)
				life = (int)GameLevelCommon.instance.actorLifeTable[id];
			else
				GameLevelCommon.instance.actorLifeTable[id] = 0;

			return life;
		}
	}

	public float bulletSpeed
	{
		get{ return 15.0f + (damageLevel - 1)*5.0f;}
	}

	public bool init()
	{
		//< for bt debug
		base.Init();
		this.SetIdFlag(1);
		
		transform.localRotation = Quaternion.Euler(0, currentOrientation, 0);
		// find turret and barrel
		Transform turretTransform = null;
		for(int i = 0; i < transform.childCount; ++i)
		{
			Transform child = transform.GetChild(i);
			if(child.name == "Turret")
			{
				turretTransform = child;
				for(int k = 0; k < turretTransform.transform.childCount; ++k)
				{
					Transform childOfTurret = turretTransform.GetChild(k);
					if(childOfTurret.name == "FirePoint")
					{
						firePoint = childOfTurret;
						break;
					}
					//<
				}
			}
			else if(child.name == "Anchor")
			{
				anchorNode = child;
			}
		}
		
		if(turretTransform != null)
		{
			turretCtrl = new TurretController();
			turretCtrl.turret = turretTransform;
		}
		
		anchorNode.transform.DetachChildren();
		GameObject flagPrefab = null;
		if(force == eGameForce.RED_FORCE)
			flagPrefab = Resources.Load("Prefabs/ForceFlagRed") as GameObject;
		else
			flagPrefab = Resources.Load("Prefabs/ForceFlagBlue") as GameObject;           
        
		GameObject flagInstance = GameObject.Instantiate(flagPrefab) as GameObject;
		flagInstance.transform.parent = anchorNode;
		flagInstance.transform.localPosition = Vector3.zero;
		flagInstance.transform.localRotation = Quaternion.identity;

		meshTable = new Hashtable();
		gatherMeshes();
        
        if(behaviorTree.Length > 0)
        {
            btloadResult = btload(behaviorTree, true);
            if(btloadResult)
                btsetcurrent(behaviorTree);
            else
				Debug.LogError("Behavior tree data load failed! " + behaviorTree);
        }

		bubbleHud = BubbleHud.createBubbleHud(this);

		//< todo: trick
		int life = playerLifeCount;
		playerLifeCount = life;

		targetToMove = transform.localPosition;
		gameScore = calcGameScore();

        return true;
	}

	public override void updateBehaviacParameter()
	{
		base.updateBehaviacParameter();
		SetVariable("parIn_Self", this);
	}

	public void tick()
	{
		tickSteering();
		if(turretCtrl != null)
			turretCtrl.tick(aimSpeed);

		tickFire();

		tickflashColor();

		tickSheild();

		//< tick behavior tree
		if(btloadResult && aiEnabled)
			btexec();

		if(bubbleHud != null)
		{
			bubbleHud.setNumber(hp, forceColor, playerLifeCount);
			bubbleHud.tick();
			MeshRenderer mr = bubbleHud.GetComponent<MeshRenderer>();
			if(mr != null)
				mr.enabled = GameLevelCommon.instance.showHUD;
        }
	}

	void tickSheild()
	{
		if(shieldInteval > 0.0f)
		{
			shieldInteval -= Time.deltaTime;
			if(shieldInteval <= 0.0f && shieldObject != null)
			{
				GameObject.Destroy(shieldObject);
				shieldObject = null;
				shieldInteval = 0.0f;
				dodgeFromDanager = true;
			}
		}
	}

	void tickFire()
	{
		if(enableFire)
		{
			if(damageLevel > 2)
				bulletCountPerShoot = 2;
			StartCoroutine("coFireBullet");
			enableFire = false;
			fireTimer = (float)fireInterval/1000.0f;
		}

		if(fireTimer > 0.0f)
		{
			fireTimer -= Time.deltaTime;
			if(fireTimer < 0.0f)
			{
				isFiring = false;
				fireTimer = 0.0f;
			}
		}
	}

	IEnumerator coFireBullet()
	{
		for(int i = 0; i < bulletCountPerShoot; ++i)
		{
			float angle = barrelTargetAzimuth;
			Vector3 fireDirection = Quaternion.Euler(0, angle, 0)*Vector3.forward;
			GameLevelCommon.instance.spawnBullet(i, firePoint.transform.position, fireDirection, 
			                                     bulletSpeed, damageLevel, bulletLifeTime, force);	
			if(force == eGameForce.RED_FORCE)
				SoundEffectPlayer.Instance.playSound("Sounds/Tank_Shoot", 4, 0.0f, 1.0f, false);

			yield return new WaitForSeconds((float)twoBulletInterval/1000.0f);
		}
	}
	
	public void takeShield(float interval)
	{
		if(shieldObject == null)
		{
			GameObject prefabs = Resources.Load("Prefabs/ShieldEffect") as GameObject;
			GameObject inst = GameObject.Instantiate(prefabs) as GameObject;
			inst.transform.parent = this.transform;
			inst.transform.localPosition = Vector3.zero;
			inst.transform.localRotation = Quaternion.identity;
			shieldObject = inst;
		}
		shieldInteval = interval;
		dodgeFromDanager = false;
	}

	[behaviac.MethodMetaInfo()]
	public float detectNearestEnemyInView()
	{
		float angle = -1.0f;
		int minDistance = int.MaxValue;
		for(int i = 0; i < 4; ++i)
		{
			eMapDirection d = (eMapDirection)i;
			int distance = detectEnemy(d);
			if(distance > 0)
			{
				if(minDistance > distance)
				{
					minDistance = distance;
					angle = 90.0f*(float)i;
				}
			}
		}

		return angle;
	}

	private Player nearestEnemy = null;
	[behaviac.MethodMetaInfo()]
	public behaviac.EBTStatus findNearestEnemy()
	{
		Player p = GameLevelCommon.instance.getNearestEnemy(force) as Player;
		if(p == null)
			return behaviac.EBTStatus.BT_FAILURE;
		else
		{
			nearestEnemy = p;
			return behaviac.EBTStatus.BT_SUCCESS;
		}
	}

	[behaviac.MethodMetaInfo()]
	public behaviac.EBTStatus canDestroyBrick(int step)
	{
		eMapDirection direction = (eMapDirection)Mathf.RoundToInt(currentOrientation / 90.0f);
		eBrickType brickType = GameLevelCommon.instance.getBrickType(this, direction, step);
		if((int)brickType < (int)eBrickType.STEEL)
		{
			return behaviac.EBTStatus.BT_SUCCESS;
		}
		else if((int)brickType < (int)eBrickType.OCEAN && damageLevel > 1)
		{
			return behaviac.EBTStatus.BT_SUCCESS;
		}
		else
		{
			return behaviac.EBTStatus.BT_FAILURE;
		}
	}

	[behaviac.MethodMetaInfo()]
	public int detectEnemy(eMapDirection dir)
	{
		int stepDistance = 0;
		int xStep = 0; int yStep = 0;
		switch(dir)
		{
		case eMapDirection.LEFT:
			xStep = -1; yStep = 0;
			break;
		case eMapDirection.RIGHT:
			xStep = 1; yStep = 0;
			break;
		case eMapDirection.TOP:
			xStep = 0; yStep = -1;
			break;
		case eMapDirection.BOTTOM:
			xStep = 0; yStep = 1;
			break;
		}

		int _2dx = -1; int _2dy = -1;
		GameLevelCommon._3dTo2d(targetToMove.x, targetToMove.z, out _2dx, out _2dy);
		eBrickType gridType = eBrickType.EMPTY;
		while(true)
		{
			_2dx = _2dx + xStep;
			_2dy = _2dy + yStep;
			gridType = (eBrickType)GameLevelCommon.instance.navData[_2dy, _2dx];
			if(gridType == eBrickType.EMPTY)
			{
				++stepDistance;
			}
			else
			{
				if(gridType == eBrickType.ACTOR)
				{
					++stepDistance;
					return stepDistance;
				}
				else if(gridType == eBrickType.OCEAN || gridType == eBrickType.AWARD ||
				        gridType == eBrickType.SOIL || gridType == eBrickType.STEEL)
				{
					++stepDistance;
				}
				else
				{
					return -1;
				}
			}
		}
	}

	[behaviac.MethodMetaInfo()]
	public behaviac.EBTStatus aimToAngle(float angle)
	{
		if(angle == turretCtrl.azimuth)
			return behaviac.EBTStatus.BT_SUCCESS;
		else
		{
			barrelTargetAzimuth = angle;
			turretCtrl.isRotating = true;
			return behaviac.EBTStatus.BT_RUNNING;
		}
	}

	[behaviac.MethodMetaInfo()]
	public behaviac.EBTStatus aimToTarget(Vector3 position)
	{
		Vector3 deltaVector = position - transform.position;
		deltaVector.y = 0.0f;
		float angle = angleAroundAxis(Vector3.forward, deltaVector, Vector3.up);
		if(angle < 0.0f)
			angle += 360.0f;

		return aimToAngle(angle);
	}

	private bool startNewFire = true;
	[behaviac.MethodMetaInfo()]
	public behaviac.EBTStatus fire()
	{
		if(startNewFire)
		{
			enableFire = true;
			isFiring = true;
			startNewFire = false;
		}

		if(!isFiring)
		{
			startNewFire = true;
			return behaviac.EBTStatus.BT_SUCCESS;
		}
		else
		{
			return behaviac.EBTStatus.BT_RUNNING;
		}
	}

	[behaviac.MethodMetaInfo()]
	public Vector3 predicateFirePoint(behaviac.Agent target)
	{
		return Vector3.zero;
	}

	[behaviac.MethodMetaInfo()]
	public eMapDirection locationTendencyToTarget(Vector3 position)
	{

		Vector3 deltaVector = position - transform.position;
		deltaVector.y = 0.0f;
		float angle = angleAroundAxis(Vector3.forward, deltaVector, Vector3.up);
		if(angle < 0.0f)
			angle += 360.0f;

		if((angle >= 0.0f && angle < 45.0f) || (angle >= 315.0f && angle <= 360.0f))
			return eMapDirection.TOP;
		else if((angle >= 45.0f && angle < 135.0f))
			return eMapDirection.RIGHT;
		else if((angle >= 135.0f && angle < 225.0f))
			return eMapDirection.BOTTOM;
		else 
			return eMapDirection.LEFT;
	}

    [behaviac.MemberMetaInfo()]
    public int Index = 0;

	[behaviac.MethodMetaInfo()]
    public void getTarget(ref Vector3 target)
    {
        if (this.Index == 0)
        {
            target = new Vector3(-4, 0, 8);
        }
        else if (this.Index == 1)
        {
            target = new Vector3(8, 0, -8);
        }
        else if (this.Index == 2)
        {
            target = new Vector3(0, 0, -8);
        }
    }


	[behaviac.MethodMetaInfo()]
    private behaviac.EBTStatus moveto(Vector3 targetPos)
    {
        behaviac.EBTStatus s = base.moveAGrid(targetPos);

        return s;
    }

    [behaviac.MethodMetaInfo()]
    public void getInfo(ref float w, ref int index)
    {
        index = this.Index;

        if (index == 0)
        {
            w = 0.0f;
        }
        else if (index == 1)
        {
            w = 1.0f;
        }
        else if (index == 2)
        {
            w = 2.0f;
        }
    }

    //public static List<string> Msgs = new List<string>();

    [behaviac.MethodMetaInfo()]
    public void printf(float w, int index)
    {
        //string m = string.Format("W {0} Index {1} {2}", w, index, this.name);

        //Msgs.Add(m);
    }

	static float angleAroundAxis (Vector3 dirA, Vector3 dirB, Vector3 axis)
	{
		dirA = dirA - Vector3.Project (dirA, axis);
		dirB = dirB - Vector3.Project (dirB, axis);
		float angle = Vector3.Angle (dirA, dirB);
		return angle*(Vector3.Dot(axis, Vector3.Cross(dirA, dirB)) < 0 ? -1 : 1);
	}

	public float barrelTargetAzimuth
	{
		get
		{
			float val = 0.0f;
			if(turretCtrl != null)
				val = turretCtrl.targetAzimuth;
			return val;
		}
		set
		{
			float val = Mathf.Clamp(value, 0.0f, 360.0f);
			if(turretCtrl != null)
				turretCtrl.targetAzimuth = val;
		}
	}

	//< flash white
	protected void tickflashColor()
	{
		if(colorTimeLerp >= 0.0f && swithColorFlash)
		{			
			colorTimeLerp -= (Time.deltaTime)*6.0f;
			if(colorTimeLerp < 0.0f)
			{
				swithColorFlash = false;
				colorTimeLerp = 0.0f;
			}

			foreach(DictionaryEntry entry in meshTable)
			{
				Color clr = new Color(1.0f, 0.0f, 0.0f, 0.0f);
				Renderer meshInfo = (entry.Value as GameObject).GetComponent<Renderer>();
				foreach(Material mat in meshInfo.materials)
				{
					mat.SetColor("_AddColor", clr*colorTimeLerp);
				}
			}
		}
	}
	
	public void flashWhite()
	{
		swithColorFlash = true;
		colorTimeLerp = 1.0f;
	}

	protected int gatherMeshes()
	{		
		Transform[] childrenNode = transform.GetComponentsInChildren<Transform>();
		int i = 0;
		foreach(Transform child in childrenNode)
		{
			Renderer meshInfo = child.GetComponent<Renderer>();
			if(meshInfo != null)
			{
				string shaderName = meshInfo.material.shader.name;
				if(shaderName == "BattleCityShader/PlayerDiffuse")
				{
					meshTable[child.name] = child.gameObject;
					++i;
				}
			}
		}
		//renderNode
		return i;
	}

	protected int calcGameScore()
	{
		int maxFireInterval = 5000;
		float maxRotateSpeed = 720.0f;
		float maxAimSpeed = 720.0f;
		float maxMoveSpeed = 16.0f;

		float gameMoveSpeed = Mathf.Clamp(moveSpeed, 0.0f, maxMoveSpeed);
		float gameRotateSpeed = Mathf.Clamp(rotateSpeed, 0.0f, maxRotateSpeed);
		float gameAimSpeed = Mathf.Clamp(aimSpeed, 0.0f, maxAimSpeed);

		int fire = Mathf.Clamp(fireInterval, 0, maxFireInterval);
		int score = hp*10 + bulletCountPerShoot*10 + (maxFireInterval - fire)/100 + (int)(maxMoveSpeed - gameMoveSpeed)*10 + 
			(int)(maxRotateSpeed - gameRotateSpeed)/10 + (int)(maxAimSpeed - gameAimSpeed)/10;
		return score;
	}
}
