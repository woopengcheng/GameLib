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
using System.Collections.Generic;

public enum eGameStatus
{
	RUNNING, PAUSED, OVER
}

[behaviac.TypeMetaInfo("GameLevelCommon", "GameLevelCommon -> Agent")]
[AddComponentMenu("BattleCity/GameLevelCommon")]
public class GameLevelCommon : behaviac.Agent 
{
	public static GameLevelCommon instance = null;

	[behaviac.MemberMetaInfo()]
	public static int defaultWalkLevel = (1 << (int)eBrickType.TREE) | (1 << (int)eBrickType.ICE) | (1 << (int)eBrickType.EMPTY);
	[behaviac.MemberMetaInfo()]
	public static int actorWalkLevel = (1 << (int)eBrickType.TREE) | (1 << (int)eBrickType.ICE) | (1 << (int)eBrickType.EMPTY) | (1 << (int)eBrickType.ACTOR);
	[behaviac.MemberMetaInfo()]
	public static int bulletWalkLevel = actorWalkLevel | (1 << (int)eBrickType.OCEAN);
	[behaviac.MemberMetaInfo()]
	public static int allWalkLevel = 0x0000ffff & (~(1 << (int)eBrickType.OBSTACLE));
	[behaviac.MemberMetaInfo()]
	public static int soilWalkLevel = actorWalkLevel | (1 << (int)eBrickType.SOIL);
	[behaviac.MemberMetaInfo()]
	public static int soilSteelWalkLevel = soilWalkLevel | (1 << (int)eBrickType.STEEL);
	[behaviac.MemberMetaInfo()]
	public static int soilSteelOceanWalkLevel = soilSteelWalkLevel | (1 << (int)eBrickType.OCEAN);
	[behaviac.MemberMetaInfo()]
	public bool isPlayerAlive = false;
	public int mainCharID = -1;

	public UIManager uiManager = null;
	public CreditSystem creditSystem = null;

	public BehaviacSystem behaviacSystem = null;
	public Transform bulletAnchor = null;
	public Transform awardAnchor = null;
	public Transform headquater = null;
	public Transform enemyAnchor = null;
	public Transform dynamicEffectAnchor = null;
	public byte[,] navData = null;
	public System.Random randGenerator = null;
	public AStarPathFinding pathFindingSystem = null;

	private Player playerChar = null;

	public ArrayList pathResultGizmo = null;

	protected GameObject bulletPrefab = null;

	protected List<GameActor> enemyList = null;

	protected List<Award> awardList = null;

	protected List<Transform> enemySpawnPointList = null;
	protected List<Transform> playerSpawnPointList = null;

	public string behaviorTree = "";
	protected bool btloadResult = false;

	public eGameStatus gameStatus = eGameStatus.RUNNING;
	public bool showHUD = true;

	public Hashtable actorLifeTable = new Hashtable();

	public bool isMelee = false;

	public DangerEstimator dangerRecord = null;

	public Camera mainCamera = null;

	public bool isElite = false;

	public bool debugDisplayGridPoint = false;
	public bool debugDisplayPathPoint = true;
	public bool debugDisplayDangerBullet = true;

	static protected int objectID = 1000;
	static public int getNewObjectID()
	{
		return ++objectID;
	}

	void Awake()
	{
		//< for bt debug
		base.Init();
		this.SetIdFlag(1);

		instance = this;

		bool result = false;
		//behaviacSystem = GetComponent<BehaviacSystem>();
		behaviacSystem = BehaviacSystem.Instance;
		if(behaviacSystem == null)
		{
			Debug.LogError("do not contain behaviac system in the level!");
		}
		else
		{
			result = behaviacSystem.Init();
			if(!result)
				Debug.LogError("behaviac system init failed!");
		}

		behaviac.Agent.BindInstance (GameLevelCommon.instance, "GameLevel");

		navData = generateNavData();
		pathFindingSystem = new AStarPathFinding(25);

		dangerRecord = new DangerEstimator();
		dangerRecord.init();

		GameObject playerObject = GameObject.Find("@Player");
		if(playerObject != null)
		{
			playerChar = playerObject.GetComponent<Player>();
			if(playerChar.id == -1)
				playerChar.id = getNewObjectID();
			playerChar.SetName("@Player");
			setActorLifeCount(playerChar.id, playerChar.initLifeCount);
			playerChar.init();
			setMainCharID(playerChar.id);
			isPlayerAlive = true;
			playerChar.updateGridInfo();
		}

		awardList = new List<Award>();
		awardAnchor = GameObject.Find("@Awards").transform;
		if(awardAnchor != null)
		{
			for(int i = 0; i < awardAnchor.childCount; ++i)
            {
				Award newAward = awardAnchor.GetChild(i).GetComponent<Award>();
				Award awd = newAward.GetComponent<Award>();
				if(awd != null)
				{
					awd.gameObject.layer = LayerMask.NameToLayer("AwardLayer");
					awd.id = getNewObjectID();
					awd.name = "Award_" + awd.id.ToString();
	                awardList.Add(awd);
	            }
			}
		}

		GameObject headquaterObject = GameObject.Find("@Headquater");
		if(headquaterObject != null)
			headquater = headquaterObject.transform;

		bulletPrefab = Resources.Load("Prefabs/@Bullet") as GameObject;
		if(bulletPrefab == null)		
		{
			Debug.LogError("Bullet prefab load failed!");
		}

		dynamicEffectAnchor = GameObject.Find("@DynamicEffect").transform;

		GameObject mainCameraObject = GameObject.Find("@MainCamera");
		mainCamera = mainCameraObject.GetComponent<Camera>();
		InputManager.Instance.setMainCamera(mainCameraObject.transform);

		enemyList = new List<GameActor>();
		enemyAnchor = GameObject.Find("@Enemies").transform;
		for(int i = 0; i < enemyAnchor.childCount; ++i)
		{
			GameActor enemy = enemyAnchor.GetChild(i).GetComponent<GameActor>();
			if(enemy != null)
			{
				Player p_ = enemy as Player;

				if(p_.id == -1)
					p_.id = getNewObjectID();

				setActorLifeCount(p_.id, p_.initLifeCount);
				result = p_.init();
				if(result)
				{
					p_.SetName("Enemy_" + p_.id.ToString());
					p_.updateGridInfo();
					enemyList.Add(p_);
				}
				else
				{
					Debug.LogError(p_.name + " init failed!");
				}
			}
		}

		randGenerator = new System.Random(Guid.NewGuid().GetHashCode());

		if(behaviorTree.Length > 0)
		{
			btloadResult = btload(behaviorTree, true);
			if(btloadResult)
				btsetcurrent(behaviorTree);
			else
				Debug.LogError("Behavior tree data load failed! " + behaviorTree);
		}

		enemySpawnPointList = new List<Transform>();
		playerSpawnPointList = new List<Transform>();
		gatherSpawnPoints();

		TankUtilities.Shuffle(enemySpawnPointList);

		GameObject uiManagerObject = GameObject.Find("@UI");
		if(uiManagerObject != null)
			uiManager = uiManagerObject.GetComponent<UIManager>();

		creditSystem = new CreditSystem();
		creditSystem.init();

		Time.timeScale = 1.0f;
	}

	// Use this for initialization
	void Start()
	{
		SoundEffectPlayer.Instance.playSound("Sounds/Game_Start", 0, 0.0f, 1.0f, false);
	}

	void gatherSpawnPoints()
	{
		GameObject spawnPointAnchor = GameObject.Find("@ActorSpawnPoint");
		if(spawnPointAnchor != null)
		{
			for(int i = 0; i < spawnPointAnchor.transform.childCount; ++i)
			{
				Transform child = spawnPointAnchor.transform.GetChild(i);
				if(child.name == "@ESP")
				{
					enemySpawnPointList.Add(child);
				}
				else if(child.name == "@PSP")
				{
					playerSpawnPointList.Add(child);
				}
			}
		}
	}

	public Bullet spawnBullet(int index, Vector3 pos, Vector3 dir, float speed, int damage, float life, eGameForce force)
	{
		Bullet bu = null;
		if(bulletPrefab != null)
		{
			GameObject bulletObject = GameObject.Instantiate(bulletPrefab) as GameObject;
			bu = bulletObject.GetComponent<Bullet>();
			bu.transform.parent = bulletAnchor;
			bu.transform.localPosition = pos;
			bu.speed = speed;
			bu.force = force;

			int damageLeft = damage - index*2;
			if(damageLeft < 2)
				bu.damage = 1;
			else
				bu.damage = 2;			
			
			bu.lifeTime = life;
			bu.initDirection = dir;
			bu.initDirection.y = 0.0f;
			bu.locomotionKey = DangerEstimator.getLocomotionKey(bu.transform.localPosition, bu.initDirection);
			if(bu.locomotionKey == int.MinValue)
			{
				Debug.LogWarning("Wrong bullet index" + " : " + index.ToString());
			}
			GameLevelCommon.instance.dangerRecord.addBullet(bu);
		}
		return bu;
	}

	class SortObjectByDistanceToPlayer<T> : IComparer<T> where T : MonoBehaviour
	{
		public int Compare(T a0, T a1)
		{
			if(GameLevelCommon.instance == null || GameLevelCommon.instance.playerChar == null)
				return 0;

			Vector3 playerPos = GameLevelCommon.instance.playerChar.transform.position;
			Vector3 p0 = a0.transform.position;
			Vector3 p1 = a1.transform.position;
			float d0 = (p0 - playerPos).sqrMagnitude;
			float d1 = (p1 - playerPos).sqrMagnitude;
			if(d0 > d1)
				return 1;
			else if(d0 == d1)
				return 0;
			else
				return -1;
		}
	}

	bool pickScene(out Vector3 point)
	{
		point = Vector3.zero;
		Ray ray = mainCamera.ScreenPointToRay(Input.mousePosition);
		RaycastHit hitInfo;
		bool hit = Physics.Raycast(ray, out hitInfo);
		if(hit)
		{
			point = hitInfo.point;
		}

		return hit;
	}

    // Update is called once per frame
	private SortObjectByDistanceToPlayer<GameActor> enemySorter = new SortObjectByDistanceToPlayer<GameActor>();
	private SortObjectByDistanceToPlayer<Award> awardSorter = new SortObjectByDistanceToPlayer<Award>();
	private float sortTick = 0.0f;
	private bool pauseSwitch = true;

	// for test todo:
	public List<kBulletStatus> dangerousBullets = null;

	void Update()
	{
		behaviac.Workspace.Instance.Update ();

		if(Input.GetKeyDown(KeyCode.Q))
		{
			uiManager.showCredit();
		}

		if(Input.GetKeyDown(KeyCode.B))
		{
			playerChar.takeShield(5.0f);
		}

		if(Input.GetKeyDown(KeyCode.G))
		{
			if(playerChar != null)
			{
				int i = playerChar.playerLifeCount;
				playerChar.playerLifeCount = i + 1;
			}
		}

		if(Input.GetKeyDown(KeyCode.H))
		{
			if(playerChar != null)
			{
				int i = playerChar.playerLifeCount;
				playerChar.playerLifeCount = i - 1;
			}
        }

		if(Input.GetKeyDown(KeyCode.P))
		{
			if(pauseSwitch)
				Time.timeScale = 0.0f;
			else
				Time.timeScale = 1.0f;
			pauseSwitch = !pauseSwitch;
		}

		InputManager.Instance.tick();

		//< sort
		if(sortTick > 2.0f)
		{
			if(enemyList.Count > 1 && playerChar != null)
				enemyList.Sort(enemySorter);

			if(awardList.Count > 1 && playerChar != null)
				awardList.Sort(awardSorter);

			sortTick = 0.0f;
		}
		sortTick += Time.deltaTime;

		//< update game level data

		if(btloadResult)
			btexec();

		//< update datas in agent's blackboard
        updateAgentAIData();

		//< tick player
		if(playerChar != null)
			playerChar.tick();

		if(playerChar != null && !playerChar.aiEnabled)
			keyBoardController(playerChar);

		//< tick enemies
		for(int i = 0; i < enemyList.Count; ++i)
		{
			Player p_ = enemyList[i] as Player;
			if(p_ != null)
			{
				p_.tick();
			}
		}
	}

	void keyBoardController(Player p_)
	{
		if(Input.GetKey(KeyCode.UpArrow))
		{
			if(p_.rotateToAngle(0) == behaviac.EBTStatus.BT_SUCCESS)
			{
				p_.moveForward();
			}
			p_.aimToAngle(0);
		}

		if(Input.GetKey(KeyCode.DownArrow))
		{
			if(p_.rotateToAngle(180) == behaviac.EBTStatus.BT_SUCCESS)
			{
				p_.moveForward();
			}
			p_.aimToAngle(180);
        }

		if(Input.GetKey(KeyCode.LeftArrow))
		{
			if(p_.rotateToAngle(270) == behaviac.EBTStatus.BT_SUCCESS)
			{
				p_.moveForward();
			}
			p_.aimToAngle(270);
        }

		if(Input.GetKey(KeyCode.RightArrow))
		{
			if(p_.rotateToAngle(90) == behaviac.EBTStatus.BT_SUCCESS)
			{
				p_.moveForward();
			}
			p_.aimToAngle(90);
        }

		if(Input.GetKey(KeyCode.Space))
		{
			playerChar.fire();
        }
    }
    
    void updateAgentAIData()
	{
		//<
		if(playerChar != null)
			playerChar.updateBehaviacParameter();

		for(int i = 0; i < enemyList.Count; ++i)
		{
			Player p_ = enemyList[i] as Player;
			if(p_ != null)
			{
				p_.updateBehaviacParameter();
			}
		}
	}

	[behaviac.MethodMetaInfo()]
	public void setActorLifeCount(int id, int life)
	{
		actorLifeTable[id] = life;
	}

	[behaviac.MethodMetaInfo()]
	public void setMainCharID(int id)
	{
		mainCharID = id;
	}

	[behaviac.MethodMetaInfo()]
	public int getActorLifeCount(int id)
	{
		int count = (int)actorLifeTable[id];
		return count;
	}

	[behaviac.MethodMetaInfo()]
	public behaviac.EBTStatus isAllEnemyDeadForever()
	{
		int totalLifeCount = 0;
		foreach(DictionaryEntry entry in actorLifeTable)
		{
			int id = (int)entry.Key;
			int lifeCount = (int)entry.Value;
			if(id != mainCharID)
				totalLifeCount += lifeCount;
		}

		int enemyAlive = getEnemyCount();
		totalLifeCount += enemyAlive;
		if(totalLifeCount > 0)
			return behaviac.EBTStatus.BT_FAILURE;
		else
			return behaviac.EBTStatus.BT_SUCCESS;
	}

	[behaviac.MethodMetaInfo()]
	public behaviac.EBTStatus isPlayerDeadForever()
    {
		int playerLifeCount = getActorLifeCount(mainCharID);
		if(playerLifeCount > 0 || isPlayerAlive)
			return behaviac.EBTStatus.BT_FAILURE;
		else
			return behaviac.EBTStatus.BT_SUCCESS;
	}

	const float brickSize = 4.0f;
	const int navGridSize = 25;
	[behaviac.MethodMetaInfo()]
	public eBrickType getBrickType(GameActor p_, eMapDirection dir_, int step)
	{
		int _2dx = -1; int _2dy = -1;
		Vector3 biasVector = Vector3.zero;
		switch(dir_)
		{
		case eMapDirection.LEFT:
			biasVector = -Vector3.right*step*brickSize;
			break;
		case eMapDirection.RIGHT:
			biasVector = Vector3.right*step*brickSize;
			break;
		case eMapDirection.TOP:
			biasVector = Vector3.forward*step*brickSize;
			break;
		case eMapDirection.BOTTOM:
			biasVector = -Vector3.forward*step*brickSize;
			break;
		}
		Vector3 queryPosition = p_.transform.position + biasVector;
		_3dTo2d(queryPosition.x, queryPosition.z, out _2dx, out _2dy);
		eBrickType t = (eBrickType)navData[_2dy, _2dx];	
		return t;
	}

	[behaviac.MethodMetaInfo()]
	public float getAvailabeMoveDirection(GameActor p_, eMapDirection defaultDirection)
	{
		float angleToRotate = -1.0f;
		int walkLevel = GameLevelCommon.defaultWalkLevel;

		if(defaultDirection != eMapDirection.UNKNOWN)
		{
			eBrickType t_ = getBrickType(p_, defaultDirection, 1);
			if(isBrickTypeWakable(t_, walkLevel))
			{
				angleToRotate = getAngleFromDirection(defaultDirection);
				return angleToRotate;
			}
		}

		//< random
		ArrayList _dirList = new ArrayList();
		_dirList.Add(eMapDirection.LEFT);
		_dirList.Add(eMapDirection.RIGHT);
		_dirList.Add(eMapDirection.TOP);
		_dirList.Add(eMapDirection.BOTTOM);

		while(_dirList.Count > 0)
		{
			int index = randGenerator.Next(0, _dirList.Count);
			eMapDirection d_ = (eMapDirection)_dirList[index];
			eBrickType t_ = getBrickType(p_, d_, 1);
			if(isBrickTypeWakable(t_, walkLevel))
			{
				angleToRotate = getAngleFromDirection(d_);
				if(angleToRotate != p_.currentOrientation)
					break;
			}
			_dirList.RemoveAt(index);
		}

		return angleToRotate;
	}

	[behaviac.MethodMetaInfo()]
	public bool isPointWakable(int x, int y, int walkLevel)
	{
		eBrickType t = (eBrickType)navData[y, x];
		return isBrickTypeWakable(t, walkLevel);
	}

	[behaviac.MethodMetaInfo()]
	public bool isBrickTypeWakable(eBrickType t, int walkLevel)
	{
		int k = 1 << (int)t;
		if((k & walkLevel) != 0)
			return true;
		else
			return false;
	}

	[behaviac.MethodMetaInfo()]
	public behaviac.Agent getPlayer()
	{
		return playerChar;
	}

	[behaviac.MethodMetaInfo()]
	public int randomNumberInt(int min, int max)
	{
		return randGenerator.Next(min, max);
	}

	[behaviac.MethodMetaInfo()]
	public float randomNumberFloat(float min, float max)
	{
		int i = (int)(min*1000.0f);
		int j = (int)(max*1000.0f);
		int k = randGenerator.Next(i, j);
		return (float)k/1000.0f;
	}

	[behaviac.MethodMetaInfo()]
	public int getEnemyCount()
	{
		return enemyList.Count;
	}

	[behaviac.MethodMetaInfo()]
	public behaviac.Agent getNearestEnemy(eGameForce force)
	{
		if(force == eGameForce.RED_FORCE)
		{
			Player result = null;
			for(int i = 0; i < enemyList.Count; ++i)
			{
				Player enemy = enemyList[i] as Player;
				if(enemy.shieldInteval == 0.0f)
				{
					result = enemy;
					break;
				}
			}
			return result;
		}
		else
		{
			return playerChar;
		}
    }

	[behaviac.MethodMetaInfo()]
	public int getNearestAwardID()
	{
		if(awardList.Count > 0)
			return awardList[0].id;
		else
			return -1;
	}

	[behaviac.MethodMetaInfo()]
	public bool isAwardExist(int id)
	{
		Award result = awardList.Find(delegate(Award element){return element.id == id;});
		if(result != null)
			return true;
		else
			return false;
	}

	[behaviac.MethodMetaInfo()]
	public Vector3 getAwardPosition(int id)
	{
		Award result = awardList.Find(delegate(Award element){return element.id == id;});
		if(result != null)
			return result.transform.position;
		else
		{
			Debug.LogWarning("Can not find award : " + id.ToString());
			return Vector3.zero;
		}
    }

	[behaviac.MethodMetaInfo()]
	public Vector3 getEnemyPosition(behaviac.Agent agent)
    {
		if(agent == null)
		{
			return Vector3.zero;
		//	Debug.LogError("getEnemyPosition error!");
		}
		return agent.transform.position;
	}

	public bool findAWakablePosition(out int x, out int y)
	{
		x = randGenerator.Next(1, 24);
		y = randGenerator.Next(1, 24);
		int count = 0;
		while(!isPointWakable(x, y, GameLevelCommon.defaultWalkLevel) && count < 1000)
		{
			x = randGenerator.Next(1, 24);
			y = randGenerator.Next(1, 24);
			count++;
		}
		
		if(count == 1000)
		{
			Debug.LogError("Can not find a avalid position!");
			return false;
		}
		else
		{
			return true;
		}
	}

	public void destroyGameActor(Player p_)
	{
		if(p_ != null)
		{
			bool result = false;
			if(p_.force == eGameForce.RED_FORCE)
			{
				playerChar = null;
				isPlayerAlive = false;
				SoundEffectPlayer.Instance.playSound("Sounds/Player_Dead", 2, 0.0f, 1.0f, false);
			}
			else
			{
				result = enemyList.Remove(p_);
				SoundEffectPlayer.Instance.playSound("Sounds/Npc_Dead", 3, 0.0f, 1.0f, false);

				if(!result)
				{
					Debug.LogError("Remove enemy failed!");
				}
			}

			GameObject.Destroy(p_.gameObject);
			GameLevelCommon.instance.navData[p_.gridY, p_.gridX] = (byte)eBrickType.EMPTY;
		}
	}

	[behaviac.MethodMetaInfo()]
	public void continueGame()
	{
		if(uiManager != null)
		{
			uiManager.hideUI();
			gameStatus = eGameStatus.RUNNING;
			Time.timeScale = 1.0f;
		}
	}


	[behaviac.MethodMetaInfo()]
	public behaviac.EBTStatus showCredit()
	{
		uiManager.showCredit();
		return behaviac.EBTStatus.BT_SUCCESS;
	}

	[behaviac.MethodMetaInfo()]
	public void stopGame(bool doesWin)
	{
		if(uiManager != null)
		{
			string soundEffect = "Sounds/Game_Over";
			if(doesWin)
				soundEffect = "Sounds/GameTheme";

			gameStatus = eGameStatus.OVER;
			Time.timeScale = 0.0f;
			SoundEffectPlayer.Instance.playSound(soundEffect, 0, 0.0f, 1.0f, false);
			uiManager.showUI(doesWin);
		}
	}

	[behaviac.MethodMetaInfo()]
	public int spawnRandomActor(eGameForce force)
	{
		float moveSpeed = isElite ? 16.0f : randomNumberFloat(4.0f, 16.0f);
		int fireInterval =  isElite ? 4000 : randomNumberInt(500, 6000);
		int twoBulletInterval = isElite ? 2000 : randomNumberInt(200, fireInterval);
		int hp = randomNumberInt(1, 4);
		float aimSpeed = isElite ? 720.0f : randomNumberFloat(90.0f, 720.0f);
		float rotateSpeed = isElite ? 720.0f : randomNumberFloat(90.0f, 720.0f);
		int damageLevel = randomNumberInt(1, 5);
		string behaviorTree = "Tank_SafeWander_RandomFire";

		int playerID = spawnGameActor(force, moveSpeed, behaviorTree, fireInterval, 
		                              twoBulletInterval, hp, damageLevel, aimSpeed, rotateSpeed, -1, Color.red, -1);
		return playerID;
	}

	[behaviac.MethodMetaInfo()]
	public int spawnGameActor(eGameForce force, float moveSpeed, string behaviorTree, int fireInterval, 
	                                     int twoBulletInterval, int hp, int damageLevel, float aimSpeed, 
	                                     float rotateSpeed, int spawnIndex, Color clr, int id)
	{
		if(playerChar != null && force == eGameForce.RED_FORCE)
			return -1;
	
		string prefabsPath = "Prefabs/@Player";
		GameObject pref = Resources.Load(prefabsPath) as GameObject;
		bool hasFound = false;

		List<Transform> spawnPointList = null;
		if(force == eGameForce.RED_FORCE)
			spawnPointList = playerSpawnPointList;
		else
			spawnPointList = enemySpawnPointList;

		Player newPlayer = null;
		if(pref != null)
		{
			Quaternion rot = Quaternion.identity;
			Vector3 pos = Vector3.zero;
			if(spawnPointList.Count == 0)
			{
				int x = 0, y = 0;
				hasFound = findAWakablePosition(out x, out y);
				if(hasFound)
				{
					float _3dx = 0.0f; float _3dy = 0.0f;
					_2dTo3d(x, y, out _3dx, out _3dy);
					pos = new Vector3(_3dx, 0, _3dy);
				}
			}
			else
			{
				int index = randGenerator.Next(0, spawnPointList.Count);
				if(spawnIndex >= 0 && spawnIndex < spawnPointList.Count)
					index = spawnIndex;

				index = Mathf.Clamp(index, 0, spawnPointList.Count - 1);
				Transform spawnPoint = spawnPointList[index];

				int _2dx = -1; int _2dy = -1;
				_3dTo2d(spawnPoint.localPosition.x, spawnPoint.localPosition.z, out _2dx, out _2dy);
				bool wakable = isPointWakable(_2dx, _2dy, GameLevelCommon.defaultWalkLevel);
				if(!wakable)
					Debug.LogWarning("Spawn enemy overlapped!");
				rot = spawnPoint.rotation;
				pos = spawnPoint.position;
				hasFound = true;
			}

			if(hasFound)
			{
				GameObject newPlayerObject = GameObject.Instantiate(pref) as GameObject;
				newPlayer = newPlayerObject.GetComponent<Player>();
				if(newPlayer != null)
				{
					if(id == -1)
						newPlayer.id = getNewObjectID();
					else
						newPlayer.id = id;

					newPlayer.force = force;
					newPlayer.moveSpeed = moveSpeed;
					newPlayer.rotateSpeed = rotateSpeed;
					newPlayer.behaviorTree = behaviorTree;
					newPlayer.fireInterval = fireInterval;
					newPlayer.twoBulletInterval = twoBulletInterval;
					newPlayer.hp = hp;
					newPlayer.damageLevel = damageLevel;
					//newPlayer.damageLevel = 3;
					newPlayer.aimSpeed = aimSpeed;
					newPlayer.forceColor = clr;

					if(force == eGameForce.BLUE_FORCE)
						newPlayer.transform.parent = enemyAnchor;

					newPlayer.transform.localPosition = pos;
					newPlayer.transform.localRotation = rot;

					bool playerInit = newPlayer.init();
					if(playerInit)
					{
						int layerIndex = LayerMask.NameToLayer("Default");
						if(force == eGameForce.BLUE_FORCE)
						{
							layerIndex = LayerMask.NameToLayer("NpcLayer");
							newPlayer.gameObject.layer = layerIndex;
							foreach(Transform t in newPlayer.transform)
								t.gameObject.layer = layerIndex;

							newPlayer.SetName("Enemy_" + newPlayer.id.ToString());
							enemyList.Add(newPlayer);
						}
						else
						{
							layerIndex = LayerMask.NameToLayer("PlayerLayer");
							newPlayer.SetName("@Player");
							playerChar = newPlayer;
							isPlayerAlive = true;
							setMainCharID(playerChar.id);
						}

						newPlayer.gameObject.layer = layerIndex;
						foreach(Transform t in newPlayer.transform)
							t.gameObject.layer = layerIndex;

						newPlayer.updateGridInfo();
						newPlayer.takeShield(2.0f);
					}
					else
					{
						Debug.LogError("spawnPlayer Player::init() failed!");
					}
				}
				else
				{
					Debug.LogError("spawnPlayer can not load Prefabs/@Player.prefabs!");
				}
			}
			else
			{
				Debug.LogError("spawnPlayer can not find a valid position to spawn player!");
			}
		}

		return newPlayer.id;
	}

	[behaviac.MethodMetaInfo()]
	public behaviac.EBTStatus spawnAward(int intType, int x, int y, float lifeTime)
	{
		eGameAward type = (eGameAward)intType;
		string prefabsPath = "Prefabs/StarAward";
		switch(type)
		{
		case eGameAward.POWER:
			break;
		case eGameAward.DEFENSE:
			prefabsPath = "Prefabs/DefenseAward";
			break;
		case eGameAward.ACCELERATE:
			prefabsPath = "Prefabs/AcelerateAward";
			break;
		case eGameAward.LIFE:
			prefabsPath = "Prefabs/TankAward";
			break;
		}
		GameObject awardPrefabs = Resources.Load(prefabsPath) as GameObject;
		if(awardPrefabs != null)
		{
			GameObject newAward = GameObject.Instantiate(awardPrefabs) as GameObject;
			newAward.transform.parent = awardAnchor;

			if(x < 0 || y < 0)
			{
				x = randGenerator.Next(1, 24);
				y = randGenerator.Next(1, 24);
			}

			float _3dx = 0.0f; float _3dy = 0.0f;
			_2dTo3d(x, y, out _3dx, out _3dy);
			newAward.transform.localPosition = new Vector3(_3dx, 0, _3dy);
			Award awd = newAward.GetComponent<Award>();
			if(awd != null)
			{
				awd.gameObject.layer = LayerMask.NameToLayer("AwardLayer");
				awd.id = getNewObjectID();
				awd.name = "Award_" + awd.id.ToString();
				awd.lifeTime = lifeTime;
				awardList.Add(awd);
			}
			else
			{
				Debug.LogError(prefabsPath + " is incorrect!");
			}
		}
		return behaviac.EBTStatus.BT_SUCCESS;
	}

	[behaviac.MethodMetaInfo()]
	public Transform spawnEffect(string fileName, Transform parent, Vector3 tr, float sc)
	{
		GameObject obj = Resources.Load(fileName) as GameObject;
		GameObject inst = GameObject.Instantiate(obj) as GameObject;
		inst.transform.parent = parent;
		inst.transform.localPosition = tr;
		inst.transform.localScale = new Vector3(sc, sc, sc);
		return null;
	}

	// todo:
	[behaviac.MethodMetaInfo()]
	public behaviac.EBTStatus isActorAlive(int id)
	{
		bool isAlive = false;
		if(playerChar != null && playerChar.id == id)
		{
			isAlive = true;
		}

		for(int i = 0; i < enemyList.Count; i++)
		{
			Player p = enemyList[i] as Player;
			if(p != null && p.id == id)
			{
				isAlive = true;
				break;
			}
		}

		if(isAlive)
			return behaviac.EBTStatus.BT_SUCCESS;
		else
			return behaviac.EBTStatus.BT_FAILURE;
	}

	[behaviac.MethodMetaInfo()]
	static public int getIntArrayElement(List<int> data, int index)
	{
 		if(index < 0 || index >= data.Count)
		{
			Debug.LogError("Out of index range!");
			return 0;
		}
		else
		{
			return data[index];
		}
	}

	[behaviac.MethodMetaInfo()]
	static public string getStringArrayElement(List<string> data, int index)
	{
		if(index < 0 || index >= data.Count)
		{
			Debug.LogError("Out of index range!");
			return "";
		}
		else
		{
			//return "Tank_Wander_RandomFire";
			string path = data[index];
			return path;
		}
	}

	[behaviac.MethodMetaInfo()]
	static public UnityEngine.Color getColorArrayElement(List<UnityEngine.Color> data, int index)
	{
		if(index < 0 || index >= data.Count)
		{
			Debug.LogError("Out of index range!");
			return Color.cyan;
		}
        else
        {
            return data[index];
        }
    }

	[behaviac.MethodMetaInfo()]
	static public eGameForce getGameForceArrayElement(List<eGameForce> data, int index)
	{
		if(index < 0 || index >= data.Count)
		{
			Debug.LogError("Out of index range!");
			return eGameForce.BLUE_FORCE;
		}
		else
		{
			return data[index];
        }
    }    
    
    public void destroyAward(Award awd)
	{
		Award result = awardList.Find(delegate(Award element){return element == awd;});
		if(result == null)
			Debug.LogError("Can not find the Award in game level award list!");

		awardList.Remove(awd);
		GameObject.Destroy(awd.gameObject);
	}
	
	static public float getAngleFromDirection(eMapDirection dir)
	{
		float angle = 0.0f;
		switch(dir)
		{
			case eMapDirection.LEFT:
				angle = 270.0f;
				break;
			case eMapDirection.RIGHT:
				angle = 90.0f;
				break;
			case eMapDirection.TOP:
				angle = 0.0f;
				break;
			case eMapDirection.BOTTOM:
				angle = 180.0f;
				break;
		}
		return angle;
	}

	void OnDrawGizmos()
	{
		if(navData != null && debugDisplayGridPoint)
		{
			for(int row = 0; row < navGridSize; row++)
			{
				for(int col = 0; col < navGridSize; col++)
				{
					float xcoord = (col - 12)*brickSize; // todo: 12
					float zcoord = (12 - row)*brickSize;
					Vector3 tr = new Vector3(xcoord, 0.5f, zcoord);

					eBrickType t = (eBrickType)navData[row, col];
					switch(t)
					{
					case eBrickType.OBSTACLE:
						Gizmos.color = Color.cyan;
						break;
					case eBrickType.EMPTY:
					case eBrickType.TREE:
						Gizmos.color = Color.white;
						break;
					case eBrickType.ACTOR:
						Gizmos.color = Color.cyan;
						break;
					default:
						Gizmos.color = Color.yellow;
						break;
					}
					Gizmos.DrawSphere(tr, 0.3f);
				}
			}
		}

		if(dangerousBullets != null && dangerousBullets.Count > 0 && debugDisplayDangerBullet)
		{
			for(int i = 0; i < dangerousBullets.Count; ++i)
			{
				kBulletStatus status = dangerousBullets[i];
				if(status.bullet != null)
				{
					Vector3 tr = status.bullet.transform.localPosition;
					Gizmos.color = Color.red;
					Gizmos.DrawSphere(tr, 1.0f);
				}
			}
		}

		if(playerChar != null)
			pathResultGizmo = playerChar.pathResult;
		if(pathResultGizmo != null && debugDisplayPathPoint)
		{
			for(int i = 0; i < pathResultGizmo.Count; ++i)
			{
				AStarPathFinding._2dPosition _p = pathResultGizmo[i] as AStarPathFinding._2dPosition;
				float _3dx = 0.0f; float _3dy = 0.0f;
				_2dTo3d(_p.x, _p.y, out _3dx, out _3dy);
				Gizmos.color = Color.green;
				Vector3 tr = new Vector3(_3dx, 1.0f, _3dy);
				Gizmos.DrawSphere(tr, 0.5f);
			}
		}
		//<
	}

	public static eMapDirection getRelativeDirection(int x0, int y0, int x1, int y1)
	{
		int dx = x1 - x0;
		int dy = y1 - y0;
		if(dx == 0 && dy == 1)
			return eMapDirection.BOTTOM;
		else if(dx == 0 && dy == -1)
			return eMapDirection.TOP;
		else if(dx == 1 && dy == 0)
			return eMapDirection.RIGHT;
		else if(dx == -1 && dy == 0)
			return eMapDirection.LEFT;
        else
		{
			Debug.LogError("grids are not close!");
			return eMapDirection.UNKNOWN;
		}
    }

	public static float floorToStep(float val, float step)
	{
		float f0 = val/step;
		float p0 = Mathf.Floor(f0);
		float p1 = f0 - p0;
		float p1s = Mathf.Floor(p1 + 0.5f)*step;
		return p0*step + p1s;
    }

	public static bool _3dTo2d(float _3x, float _3y, out int _2x, out int _2y)
	{
		_2x = -1; _2y = -1;
		if(_3x < -50.0f || _3x > 50.0f || _3y < -50.0f || _3y > 50.0f)
			return false;

		float step = 4.0f;
		float stepedX = GameLevelCommon.floorToStep(_3x, step);
		float stepedY = GameLevelCommon.floorToStep(_3y, step);
		int col = (int)((stepedX + 48.0f)/step);
		int row = (int)((48.0f - stepedY)/step);
		_2x = col; _2y = row;
		return true;
	}

	public static bool _2dTo3d(int _2x, int _2y, out float _3x, out float _3y)
	{
		_3x = 0.0f; _3y = 0.0f;
		if(_2x < 0 || _2x > 24.0f || _2y < 0 || _2y > 24)
			return false;

		float step = 4.0f;
		_3x = -48.0f + _2x*step;
		_3y = 48.0f - _2y*step;
		return true;
	}

	[ContextMenu("- Check Scene For Legal -")]
	bool checkScene()
	{
		GameObject playerObject = GameObject.Find("@Player");
		if(playerObject != null)
        {
			Vector3 tr = playerObject.transform.localPosition;
			tr.y = 0.0f;
			tr.x = floorToStep(tr.x, brickSize);
			tr.z = floorToStep(tr.z, brickSize);
			playerObject.transform.localPosition = tr;
		}

		Transform enemies = GameObject.Find("@Enemies").transform;
		for(int i = 0; i < enemies.childCount; ++i)
		{
			Transform enemy = enemies.GetChild(i);
			if(enemy != null)
			{
				Vector3 tr = enemy.localPosition;
				tr.y = 0.0f;
				tr.x = floorToStep(tr.x, brickSize);
				tr.z = floorToStep(tr.z, brickSize);
				enemy.localPosition = tr;
				if(enemy.name.Contains("@"))
					Debug.LogError("Name of enemy can not contains a '@'");
			}
		}

		Transform spawnPointAnchor = GameObject.Find("@ActorSpawnPoint").transform;
		for(int i = 0; i < spawnPointAnchor.childCount; ++i)
		{
			Transform point = spawnPointAnchor.GetChild(i);
			if(point != null)
			{
				Vector3 tr = point.localPosition;
				tr.y = 0.0f;
				tr.x = floorToStep(tr.x, brickSize);
				tr.z = floorToStep(tr.z, brickSize);
				point.localPosition = tr;
				if(point.name != "@ESP" && point.name != "@PSP")
					Debug.LogError("Name of spawn point is illegal!");
			}
		}

		GameObject bricks = GameObject.Find("@Bricks");
		for(int i = 0; i < bricks.transform.childCount; ++i)
		{
			Transform child = bricks.transform.GetChild(i);
			Brick br = child.GetComponent<Brick>();
			HeadQuater hq = child.GetComponent<HeadQuater>();
			if(br == null && hq == null)
			{
				Debug.LogError("Not a brick or headquater object below @Bricks group!");
				Debug.LogError("Name : " + child.name + " IID : " + child.GetInstanceID());
			}
			Vector3 tr = child.transform.localPosition;
			tr.y = 0.0f;
			tr.x = floorToStep(tr.x, brickSize);
			tr.z = floorToStep(tr.z, brickSize);
			child.transform.localPosition = tr;
		}

		GameObject gameAwards = GameObject.Find("@Awards");
		for(int i = 0; i < gameAwards.transform.childCount; ++i)
        {
			Transform child = gameAwards.transform.GetChild(i);
			//Award aw = child.GetComponent<Award>();
			Vector3 tr = child.transform.localPosition;
			tr.y = 0.0f;
			tr.x = floorToStep(tr.x, brickSize);
			tr.z = floorToStep(tr.z, brickSize);
			child.transform.localPosition = tr;
		}

		GameObject ground = GameObject.Find("@Ground");
		ground.transform.localPosition = new Vector3(0, -0.5f, 0);
		ground.transform.localScale = new Vector3(100, 1, 100);

		GameObject sceneObject = GameObject.Find("@Scene");
		sceneObject.transform.localPosition = new Vector3(0, 0, 0);
		sceneObject.transform.localScale = new Vector3(1, 1, 1);

		return true;
	}

	[ContextMenu("- Generate Navigation Data -")]
	byte[,] generateNavData()
	{
		byte[,] navGrid = new byte[navGridSize, navGridSize];
		int brickLayerIndex = LayerMask.NameToLayer("BrickLayer");
		for(int row = 0; row < navGridSize; row++)
		{
			for(int col = 0; col < navGridSize; col++)
			{
				if(row == 0 || col == 0 || row == navGridSize - 1 || col == navGridSize - 1)
				{
					navGrid[row, col] = (byte)eBrickType.OBSTACLE;
					continue;
				}

				float bias = brickSize/4.0f;
				float xcoord = -48.0f + col*brickSize;// todo: 48.0f 
				float zcoord =  48.0f - row*brickSize;// todo: 48.0f 
				Vector3 origin = new Vector3(xcoord, 25.0f, zcoord);
				Vector3[] offsets = new Vector3[]{  new Vector3(-bias, 0, bias), new Vector3(bias, 0, bias), 
													new Vector3(-bias, 0, -bias), new Vector3(bias, 0, -bias)};
				RaycastHit hitInfo;
				for(int i = 0; i < 4; i++)
				{
					bool doesCollide = Physics.Raycast(origin + offsets[i], -Vector3.up, out hitInfo, 
					                                   Mathf.Infinity, 1 << brickLayerIndex);
					if(doesCollide)
					{
						SubBrick sb = hitInfo.transform.GetComponent<SubBrick>();
						HeadQuater hq = hitInfo.transform.GetComponent<HeadQuater>();
						if(sb != null)
						{
							Brick brk = sb.brickParent.GetComponent<Brick>();
							byte t = (byte)brk.type;
							navGrid[row, col] = t;
						}
						else if(hq != null)
						{
							navGrid[row, col] = (byte)eBrickType.HEADQUATER;
						}
						break;
					}
					//<
				}
			}
		}
		//<
		return navGrid;
	}

	//public string m_msgs = "";
	public string nextLevelName = "";
	private bool showLevels = false;
	public bool orthoCameraMode = true;
	void OnGUI()
	{
		string btName = "invalid";
		if (playerChar != null) 
		{
			btName = playerChar.behaviorTree;
		}

		if(Application.platform == RuntimePlatform.WindowsPlayer || Application.platform == RuntimePlatform.WindowsEditor)
		{
			if(GUI.Button(new Rect(10, Screen.height - 60,150, 50), "Launch Designer"))
			{
                string workspacePath = System.IO.Path.Combine(Application.dataPath, "behaviac/workspace/BattleCity.workspace.xml");
                string workspacePath2 = workspacePath.Replace("/", "\\");
                string stdWorkspacePath = string.Format("\"{0}\" /bt={1}", workspacePath2, btName);
				
				Debug.Log(stdWorkspacePath);

                //string[] outputs = new string[2];

                {
                    bool bExist = false;
                    string behaviacDesignerPath = System.IO.Path.Combine(Application.dataPath, "..\\..\\..\\tools\\designer\\out\\BehaviacDesigner.exe");

                    //outputs[0] = behaviacDesignerPath;

                    if (System.IO.File.Exists(behaviacDesignerPath))
                    {
                        bExist = true;
                    }
                    else
                    {
                        behaviacDesignerPath = System.IO.Path.Combine(Application.dataPath, "..\\..\\..\\tools\\designer\\out\\BehaviacDesigner_d.exe");
                        if (System.IO.File.Exists(behaviacDesignerPath))
                        {
                            bExist = true;
                        }
                        else
                        {
                            string behaviacDesignerPathRoot = System.Environment.GetEnvironmentVariable("BEHAVIAC_ROOT");
                            if (!string.IsNullOrEmpty(behaviacDesignerPathRoot))
                            {
                                behaviacDesignerPath = System.IO.Path.Combine(behaviacDesignerPathRoot, "tools\\designer\\out\\BehaviacDesigner.exe");
                                //outputs[1] = behaviacDesignerPath;
                                if (System.IO.File.Exists(behaviacDesignerPath))
                                {
                                    bExist = true;
                                }
                            }
                        }
                    }
#if !UNITY_WEBPLAYER
                    if (bExist)
                    {
                        System.Diagnostics.Process.Start(behaviacDesignerPath, stdWorkspacePath);
                    }
#endif
                }

                //System.IO.File.WriteAllLines("D:\\Temp\\tank_behaviac.log", outputs);
			}
		}

		if(playerChar != null)
		{
			string ctrlMode = "AI Controlled";
			if(playerChar.aiEnabled)
				ctrlMode = "Manual";
			if(GUI.Button(new Rect(10, Screen.height - 120,150, 50), ctrlMode))
			{
				playerChar.aiEnabled = !playerChar.aiEnabled;
            }
        }
        
        string cameraMode = "Perspective";
		if(!orthoCameraMode)
			cameraMode = "Orthographic";
		if(GUI.Button(new Rect(10, Screen.height - 180,150, 50), cameraMode))
		{
			if(mainCamera != null)
			{
				OrbitCamera orbit = mainCamera.gameObject.GetComponent<OrbitCamera>();
				FlyingCamera flying = mainCamera.gameObject.GetComponent<FlyingCamera>();
				if(orthoCameraMode)
				{
					orbit.enabled = true;
					flying.enabled = false;
					orbit.reset();
				}
				else
				{
                    orbit.enabled = false;
                    flying.enabled = true;
                    flying.reset();
                }
            }
            
            orthoCameraMode = !orthoCameraMode;
        }
        
        //Debug.Log (Screen.width);
		int left = Screen.width - 180;
		int width = 150;
        string[] levelNames = new string[]{"Tutorial_1_0", "Tutorial_6_0", "Tutorial_1_1", "Tutorial_1_2",
			"Tutorial_1_3", "Tutorial_2_0", "Tutorial_2_1", "Tutorial_3_0", "Tutorial_3_1", "Tutorial_4", "DeathMatch"};

		string[] displayNames = new string[]{"FireRandom", "MoveFire_FSM", "MoveForwardTurn", "MoveRandom",
			"MoveAndFireRandom", "FindEnemyAndAttack", "DodgeFromBullet", "DestroyAndAttack", "TakeAwardAndAttack", "Survival", "DeathMatch"};

		int loadLevelIndex = 0;
		for(int k = 0; k < levelNames.Length; ++k)
		{
			if(levelNames[k] == Application.loadedLevelName)
			{
				loadLevelIndex = k;
				break;
			}
		}

		GUI.Box(new Rect (left, 10, width + 20, showLevels ? levelNames.Length*40 : 80), displayNames[loadLevelIndex] + "\n" + btName);

		int top = 50;
		if(GUI.Button(new Rect(left + 10, top, width, 20), "Show Levels"))
		{
			showLevels = !showLevels;
		}

		if(showLevels)
		{			
			for(int i = 0; i < levelNames.Length; ++i)
			{
				top += 30;
				if(GUI.Button(new Rect(left + 10, top, width, 20), displayNames[i]))
				{				
					Application.LoadLevel(levelNames[i]);
                }				
			}
			//<
        }

		//GUI.Box(new Rect (10, 200, 200, 200), m_msgs);
    }
    
}
