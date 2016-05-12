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

public enum eGameForce {RED_FORCE, BLUE_FORCE, UNKNOWN_FORCE}

[behaviac.TypeMetaInfo("GameActor", "GameActor -> Agent")]
public class GameActor : behaviac.Agent
{
	public float moveSpeed = 4.0f;

	public float rotateSpeed = 180.0f;

	[behaviac.MemberMetaInfo()]
	public float currentOrientation = 0.0f;

	public bool isMoving = false;

	public bool isRotating = false;

	public bool aiEnabled = true;

	[behaviac.MemberMetaInfo()]
	public eGameForce force = eGameForce.RED_FORCE;

	[behaviac.MemberMetaInfo()]
	public int gameScore = 0;

	public ArrayList pathResult = null;
	public int gridX = -1;
	public int gridY = -1;	
	protected float moveStepDistance = 4.0f;
	private Vector3 siteStartToMove = Vector3.zero;
	protected Vector3 targetToMove = Vector3.zero;
	private float moveLerpFactor = 0.0f;
	private float targetToRotate = 0.0f;

	public bool dodgeFromDanager = true;

    protected behaviac.EBTStatus moveAGrid(Vector3 targetPos)
	{
		Vector3 deltaVector = targetPos - transform.position;
		if(!isMoving)
		{
			if(deltaVector.sqrMagnitude < 0.01f)
				return behaviac.EBTStatus.BT_SUCCESS;
		}
	
		if(!isMoving && !isRotating)
		{
			siteStartToMove = transform.localPosition;
			
			int _2dx = -1; int _2dy = -1;
			GameLevelCommon._3dTo2d(targetPos.x, targetPos.z, out _2dx, out _2dy);
			if(_2dx < 0 || _2dx > 24 || _2dy < 0 || _2dy > 24)
			{
				Debug.LogError("Out of Map Range: " + _2dx.ToString() + " - " + _2dy.ToString());
				Debug.LogError(targetToMove);
			}
			bool wakable = GameLevelCommon.instance.isPointWakable(_2dx, _2dy, GameLevelCommon.defaultWalkLevel);
			if(!wakable)
				return behaviac.EBTStatus.BT_FAILURE;
            
			targetToMove = targetPos;
			updateGridInfo();

            isMoving = true;
            moveLerpFactor = 0.0f;
        }
        
        return behaviac.EBTStatus.BT_RUNNING;
	}

	private bool startNewStep = true;
	private Vector3 newMoveTarget = Vector3.zero;
	[behaviac.MethodMetaInfo()]
	public behaviac.EBTStatus moveForward()
	{
		if(startNewStep)
		{
			newMoveTarget = transform.localPosition + moveStepDistance*transform.forward;
			startNewStep = false;
		}

		behaviac.EBTStatus btStatus = moveAGrid(newMoveTarget);
		if(btStatus != behaviac.EBTStatus.BT_RUNNING)
			startNewStep = true;

		return btStatus;
	}

	[behaviac.MethodMetaInfo()]
	public eMapDirection getBestMoveDirection(eMapDirection tendencyDirection)
	{
		List<eMapDirection> safeDirections = null;

		if(dodgeFromDanager)
		{
			eGameForce dodgeForce = eGameForce.UNKNOWN_FORCE;
			if(!GameLevelCommon.instance.isMelee)
			{
				if(force == eGameForce.RED_FORCE)
					dodgeForce = eGameForce.BLUE_FORCE;
				else if(force == eGameForce.BLUE_FORCE)
                    dodgeForce = eGameForce.RED_FORCE;
            }
			safeDirections = GameLevelCommon.instance.dangerRecord.getSafeMoveDirections(transform.localPosition, dodgeForce);
        }
        else
		{
			safeDirections = new List<eMapDirection>();
			safeDirections.Add(eMapDirection.UNKNOWN);
			safeDirections.Add(eMapDirection.LEFT);
			safeDirections.Add(eMapDirection.RIGHT);
			safeDirections.Add(eMapDirection.TOP);
			safeDirections.Add(eMapDirection.BOTTOM);
		}
		List<eMapDirection> targetDirections = new List<eMapDirection>();
		bool isTendencyDirectionAvailable = false;
		for(int i = 0; i < safeDirections.Count; ++i)
		{
			bool isDirectionWakable = false;
			eMapDirection direction = safeDirections[i];
			if(direction == eMapDirection.UNKNOWN)
			{
				isDirectionWakable = true;
			}
			else
			{
				eBrickType brickType = GameLevelCommon.instance.getBrickType(this, direction, 1);
				if(GameLevelCommon.instance.isBrickTypeWakable(brickType, GameLevelCommon.defaultWalkLevel))
					isDirectionWakable = true;
            }

			if(isDirectionWakable)
			{
				targetDirections.Add(direction);
				if(tendencyDirection == direction)
					isTendencyDirectionAvailable = true;
			}            
		}

		if(targetDirections.Count == 0)
			return eMapDirection.UNKNOWN;
       
		if(isTendencyDirectionAvailable)
			return tendencyDirection;
		else
		{
			int index = GameLevelCommon.instance.randomNumberInt(0, targetDirections.Count);
			eMapDirection target = targetDirections[index];
			return target;
		}
	}

	[behaviac.MethodMetaInfo()]
	public behaviac.EBTStatus moveToDirection(eMapDirection direction)
	{
		if(isMoving || isRotating)
			return behaviac.EBTStatus.BT_RUNNING;

		if(direction == eMapDirection.UNKNOWN)
			return behaviac.EBTStatus.BT_SUCCESS;

		float angle = (float)direction*90.0f;
		behaviac.EBTStatus btResult = rotateToAngle(angle);
		if(btResult == behaviac.EBTStatus.BT_SUCCESS)
		{
			btResult = moveForward();
		}
		return btResult;
	}

	[behaviac.MethodMetaInfo()]
	public behaviac.EBTStatus rotateToAngle(float angle)
	{				
		float p0_ = angle/360.0f;
		float p1 = (p0_ - Mathf.Floor(p0_))*360.0f; // remap angle between 0 ~ 360 degree
		if(p1 == currentOrientation)
		{
			return behaviac.EBTStatus.BT_SUCCESS;
		}
		
		if(!isMoving && !isRotating)
		{
			targetToRotate = p1;
			isRotating = true;
		}
		
		return behaviac.EBTStatus.BT_RUNNING;
	}

	[behaviac.MethodMetaInfo()]
	public behaviac.EBTStatus navigate()
	{
		GameLevelCommon.instance.dangerousBullets = null;
		if(pathResult == null || pathResult.Count == 0)
			return behaviac.EBTStatus.BT_FAILURE;
		
		if(isMoving || isRotating)
			return behaviac.EBTStatus.BT_RUNNING;
		else
		{
			int currect2DX = -1; int currect2DY = -1;
			GameLevelCommon._3dTo2d(transform.localPosition.x, transform.localPosition.z, out currect2DX, out currect2DY);
			AStarPathFinding._2dPosition targetNode = pathResult[0] as AStarPathFinding._2dPosition;
		
			float target3DX = 0.0f; float target3DZ = 0.0f;
			if(currect2DX == targetNode.x && currect2DY == targetNode.y)
			{
				pathResult.RemoveAt(0);
				if(pathResult.Count == 0)
				{
					return behaviac.EBTStatus.BT_SUCCESS;
				}
				else
				{
					// todo:
					targetNode = pathResult[0] as AStarPathFinding._2dPosition;
					GameLevelCommon._2dTo3d(targetNode.x, targetNode.y, out target3DX, out target3DZ);
					eMapDirection direction = GameLevelCommon.getRelativeDirection(currect2DX, currect2DY, targetNode.x, targetNode.y);

					if(dodgeFromDanager)
					{
						eGameForce dodgeForce = eGameForce.UNKNOWN_FORCE;
						if(!GameLevelCommon.instance.isMelee)
						{
							if(force == eGameForce.RED_FORCE)
								dodgeForce = eGameForce.BLUE_FORCE;
							else if(force == eGameForce.BLUE_FORCE)
								dodgeForce = eGameForce.RED_FORCE;
						}
						List<kBulletStatus> dangerousBullet = GameLevelCommon.instance.dangerRecord.estimate(new Vector3(target3DX, 0, target3DZ), dodgeForce);
						if(dangerousBullet != null && dangerousBullet.Count > 0)
						{
							GameLevelCommon.instance.dangerousBullets = dangerousBullet;
							GameLevelCommon.instance.creditSystem.addDodgeFromDangerCredit(dangerousBullet.Count);
							return behaviac.EBTStatus.BT_FAILURE;
						}
					}
					return moveToDirection(direction);
				}
			}
			else
			{
				// todo: 
				targetNode = pathResult[0] as AStarPathFinding._2dPosition;
				GameLevelCommon._2dTo3d(targetNode.x, targetNode.y, out target3DX, out target3DZ);
				eMapDirection direction = GameLevelCommon.getRelativeDirection(currect2DX, currect2DY, targetNode.x, targetNode.y);

				if(dodgeFromDanager)
				{
					eGameForce dodgeForce = eGameForce.UNKNOWN_FORCE;
					if(!GameLevelCommon.instance.isMelee)
					{
						if(force == eGameForce.RED_FORCE)
							dodgeForce = eGameForce.BLUE_FORCE;
						else if(force == eGameForce.BLUE_FORCE)
							dodgeForce = eGameForce.RED_FORCE;
					}
					List<kBulletStatus> dangerousBullet = GameLevelCommon.instance.dangerRecord.estimate(new Vector3(target3DX, 0, target3DZ), dodgeForce);
                    if(dangerousBullet != null && dangerousBullet.Count > 0)
					{
						GameLevelCommon.instance.dangerousBullets = dangerousBullet;
						GameLevelCommon.instance.creditSystem.addDodgeFromDangerCredit(dangerousBullet.Count);
						return behaviac.EBTStatus.BT_FAILURE;
					}
				}				
				return moveToDirection(direction);
			}
		}
	}

	[behaviac.MethodMetaInfo()]
	public behaviac.EBTStatus findPath(Vector3 dest, int walkLevel)
	{
		if(isMoving)
			return behaviac.EBTStatus.BT_RUNNING;

		GameLevelCommon.instance.pathFindingSystem.walkLevel = walkLevel;
		int fromX = -1; int fromY = -1; int toX = -1; int toY = -1;
		GameLevelCommon._3dTo2d(transform.position.x, transform.position.z, out fromX, out fromY);
		GameLevelCommon._3dTo2d(dest.x, dest.z, out toX, out toY);
		pathResult = GameLevelCommon.instance.pathFindingSystem.pathFinding(fromX, fromY, toX, toY);
		if(pathResult == null || pathResult.Count == 0)
			return behaviac.EBTStatus.BT_FAILURE;
        else
			return  behaviac.EBTStatus.BT_SUCCESS;
    }

	[behaviac.MethodMetaInfo()]
	public int getPathDistance()
	{
		if(pathResult == null || pathResult.Count == 0)
			return 0;
		else
			return pathResult.Count;
	}
    
	public void updateGridInfo()
	{
		int _2dx = -1; int _2dy = -1;
		GameLevelCommon._3dTo2d(targetToMove.x, targetToMove.z, out _2dx, out _2dy);
		GameLevelCommon.instance.navData[_2dy, _2dx] = (byte)eBrickType.ACTOR;
		if((gridX > 0 && gridX < 24 && gridY > 0 && gridY < 24) && 
		   (gridX != _2dx || gridY != _2dy))
		{
			eBrickType originType = (eBrickType)GameLevelCommon.instance.navData[gridY, gridX];
			if(originType != eBrickType.ACTOR)
			{
				Debug.LogWarning(this.GetName());
				Debug.LogWarning(originType);
				Debug.LogWarning("Grid info of actor update failed!");
			}
			GameLevelCommon.instance.navData[gridY, gridX] = (byte)eBrickType.EMPTY;
		}

		gridX = _2dx; gridY = _2dy; 
	}
	
	protected void tickSteering() 
	{
		if(isMoving)
		{
			float t = moveSpeed / moveStepDistance;
			moveLerpFactor += (Time.deltaTime*t);
			if(moveLerpFactor > 1.0f)
				moveLerpFactor = 1.0f;
			transform.localPosition = Vector3.Lerp(siteStartToMove, targetToMove, moveLerpFactor);
			if(moveLerpFactor == 1.0f)
				isMoving = false;
		}

		if(isRotating)
		{
			float spinFactor = 0.0f;
			currentOrientation = TurretController.cuteRotate(currentOrientation, targetToRotate, rotateSpeed, out spinFactor);
			if(spinFactor == 0.0f)
				isRotating = false;
			else
				isRotating = true;
			
			transform.localRotation = Quaternion.Euler(0, currentOrientation, 0);
		}
	}

	public virtual void updateBehaviacParameter()
	{
	}
}
