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

public class kBulletStatus
{
	public bool toward = false;
	public float distance = 0.0f;
	public bool negativeSide = true;
	public Bullet bullet = null;
}

public class DangerEstimator
{
	private Hashtable bulletLocomotion = new Hashtable();

	public DangerEstimator()
	{
	}

	public bool init()
	{
		return true;
	}

	class SortBulletByAlign : IComparer<Bullet>
	{
		public bool isHorizonal = true;
		public SortBulletByAlign()
		{
		}

		public int Compare(Bullet b0, Bullet b1)
		{
			float v0 = 0.0f; float v1 = 0.0f;
			if(isHorizonal)
			{
				v0 = b0.transform.localPosition.x; 
				v1 = b1.transform.localPosition.x;
			}
			else
			{
				v0 = b0.transform.localPosition.z; 
				v1 = b1.transform.localPosition.z;
			}
			if(v0 > v1)
				return 1;
			else if(v0 == v1)
				return 0;
			else
				return -1;
		}
	}

	private SortBulletByAlign bulletSorter = new SortBulletByAlign();

	public List<eMapDirection> getSafeMoveDirections(Vector3 currentPosition, eGameForce force)
	{
		List<eMapDirection> safeDirections = new List<eMapDirection>();

		List<kBulletStatus> list = estimate(currentPosition, force);
		int bulletCount = list != null ? list.Count : 0;
		if(bulletCount == 0)
			safeDirections.Add(eMapDirection.UNKNOWN);

		int[] result = new int[(int)eMapDirection.TOTAL];
		for(int i = 0; i < (int)eMapDirection.TOTAL; ++i)
		{
			Vector3 newPosition = currentPosition;
			switch((eMapDirection)i)
			{
			case eMapDirection.TOP:
				newPosition += Vector3.forward*4.0f;
				break;
			case eMapDirection.RIGHT:
				newPosition += Vector3.right*4.0f;
				break;
			case eMapDirection.BOTTOM:
				newPosition -= Vector3.forward*4.0f;
				break;
			case eMapDirection.LEFT:
				newPosition -= Vector3.right*4.0f;
				break;
			}
			list = estimate(newPosition, force);
			result[i] = list != null ? list.Count : 0;
			if(result[i] == 0)
				safeDirections.Add((eMapDirection)i);
		}

		return safeDirections;
	}

	public List<kBulletStatus> estimate(Vector3 targetPosition, eGameForce force)
	{
		int keyH = getLocomotionKey(targetPosition, Vector3.right);
		int keyV = getLocomotionKey(targetPosition, Vector3.forward);
		if(keyH == int.MinValue || keyV == int.MinValue)
		{
			Debug.LogWarning("TargetPosition is illegel!");
			return null;
		}

		bool hasKeyH = false;
		bool hasKeyV = false;
		hasKeyH = bulletLocomotion.ContainsKey(Mathf.Abs(keyH));
		hasKeyV = bulletLocomotion.ContainsKey(Mathf.Abs(keyV));
		if(!hasKeyH && !hasKeyV)
			return null;

		List<kBulletStatus> dangerousBullets = new List<kBulletStatus>();
		if(hasKeyH)
		{
			List<kBulletStatus> statusHorizonal = gatherBulletStatus(Mathf.Abs(keyH), targetPosition);
			if(statusHorizonal != null)
			{
				checkBulletStatus(statusHorizonal);
				estimateDangerousBullets(statusHorizonal, ref dangerousBullets);
			}
		}

		if(hasKeyV)
		{
			List<kBulletStatus> statusVertical = gatherBulletStatus(Mathf.Abs(keyV), targetPosition);
			if(statusVertical != null)
			{
				checkBulletStatus(statusVertical);
				estimateDangerousBullets(statusVertical, ref dangerousBullets);
			}
		}

		if(force != eGameForce.UNKNOWN_FORCE)
		{
			for(int i = 0; i < dangerousBullets.Count; ++i)
			{
				kBulletStatus status = dangerousBullets[i];
				//if(status.bullet.force != force || Mathf.Abs(status.distance)/status.bullet.speed > 1.0f)
				if(status.bullet.force != force)
				{
					dangerousBullets.RemoveAt(i);
					i = 0;
				}
			}
		}

		return dangerousBullets;
	}

	List<kBulletStatus> gatherBulletStatus(int key, Vector3 targetPosition)
	{
		if(key < 0)
		{
			Debug.LogError("key must be a positive vaule!");
			return null;
		}

		List<Bullet> bullectList = bulletLocomotion[key] as List<Bullet>;
		if(bullectList.Count == 0)
			return null;

		List<kBulletStatus> bulletStatus = new List<kBulletStatus>();

		bool isHorizonal = key < 100;
		// sort from negative side to positive side
		bulletSorter.isHorizonal = isHorizonal;
		bullectList.Sort(bulletSorter);

		for(int i = 0; i < bullectList.Count; ++i)
		{
			Bullet singleBullet = bullectList[i];
			if(Mathf.Abs(singleBullet.locomotionKey) != key)
			{
				Debug.LogError("Target bullet has a wrong locomotionKey!");
				return null;
			}
			
			Vector3 bulletPosition = singleBullet.transform.localPosition;
			float dotValue = Vector3.Dot(singleBullet.initDirection, targetPosition - bulletPosition);
			float distance = 0.0f;
			if(isHorizonal)
				distance = bulletPosition.x - targetPosition.x;
			else
				distance = bulletPosition.z - targetPosition.z;
			
			kBulletStatus newStatus = new kBulletStatus();
			newStatus.toward = dotValue >= 0.0f;
			newStatus.distance = distance;
			newStatus.negativeSide = distance < 0.0f;
			newStatus.bullet = singleBullet;			
			bulletStatus.Add(newStatus);
		}

		return bulletStatus;
	}

	void estimateDangerousBullets(List<kBulletStatus> status, ref List<kBulletStatus> result)
	{
		int dangerNegative = 0;
		int dangerPositive = 0;
		for(int i = 0; i < status.Count; ++i)
		{
			kBulletStatus bs = status[i];
			if(bs.negativeSide)
			{
				if(bs.toward)
				{
					result.Add(bs);
					++dangerNegative;
				}
				else
				{
					if(dangerNegative > 0)
					{
						result.RemoveAt(result.Count - 1);
						--dangerNegative;
					}
				}
			}
			else
			{
				if(bs.toward)
				{
					++dangerPositive;
					if(dangerPositive > 0)
						result.Add(bs);
				}
				else
				{
					--dangerPositive;			
				}
			}
		}
	}

	void checkBulletStatus(List<kBulletStatus> status)
	{
		bool firstSide = false;
		float previousDistance = float.MinValue;
		for(int i = 0; i < status.Count; ++i)
		{
			kBulletStatus bs = status[i];
			if(i == 0)
				firstSide = bs.negativeSide;
			else
			{
				if(!firstSide && bs.negativeSide)
					Debug.LogError("bullet status order error!");
			}

			if(bs.distance < previousDistance)
				Debug.LogError("bullet distance order error!");

			previousDistance = bs.distance;
		}
	}

	public static int getLocomotionKey(Vector3 position, Vector3 direction)
	{
		int key = int.MinValue;
		int stepX = Mathf.RoundToInt(direction.x);
		int stepY = -Mathf.RoundToInt(direction.z);
		if(Mathf.Abs(stepX) + Mathf.Abs(stepY) > 1)
		{
			Debug.LogWarning("Bullet direction error! -> " + stepX.ToString() + " : " + stepY.ToString());
			return key;
		}

		int _2dx = -1; int _2dy = -1;
		GameLevelCommon._3dTo2d(position.x, position.z, out _2dx, out _2dy);
		if(_2dx == -1 || _2dy == -1)
			return key;

		if(Mathf.Abs(stepX) == 0 && Mathf.Abs(stepY) == 1)
		{
			// vertical
			key = (_2dx + 101)*stepY;
		}
		else if(Mathf.Abs(stepX) == 1 && Mathf.Abs(stepY) == 0)
		{
			// horizonal
			key = (_2dy + 1)*stepX;
		}
		else
		{
			Debug.LogError("Bullet direction error!");
		}

		return key;
	}

	public void addBullet(Bullet bu)
	{
		if(bu.locomotionKey == int.MinValue)
			return;

		int key = Mathf.Abs(bu.locomotionKey);

		if(bulletLocomotion.ContainsKey(key))
		{
			List<Bullet> bullectList = bulletLocomotion[key] as List<Bullet>;
			bool exist = bullectList.Contains(bu);
			if(!exist)
			{
				bullectList.Add(bu);
			}
			else
			{
				Debug.LogError("Found two identical bullet from bulletLocomotion table!");
			}
		}
		else
		{
			List<Bullet> bullectList = new List<Bullet>();
			bullectList.Add(bu);
			bulletLocomotion[key] = bullectList;
		}
		//Debug.Log("+ " + bu.locomotionKey.ToString());
	}

	public void removeBullet(Bullet bu)
	{	
		if(bu.locomotionKey == int.MinValue)
			return;

		// remove record from table
		int key = Mathf.Abs(bu.locomotionKey);
		//Debug.Log("- " + bu.locomotionKey.ToString());
		if(bulletLocomotion.ContainsKey(key))
		{
			List<Bullet> bullectList = bulletLocomotion[key] as List<Bullet>;
			bool exist = bullectList.Contains(bu);
            if(!exist)
			{
				Debug.LogError("Can not find bullet from bulletLocomotion table!");
			}
			else
			{
				bullectList.Remove(bu);
			}
        }
		else
		{
			Debug.LogError("Can not find bullet key from bulletLocomotion table!");
		}
    }    
}
