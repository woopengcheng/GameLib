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

public enum eMapDirection{TOP, RIGHT, BOTTOM, LEFT, TOTAL, UNKNOWN}

public class AStarPathFinding
{
	private int navSize = 0;
	
	private ArrayList closeArray = new ArrayList();
	private ArrayList openArray = new ArrayList();

	public int walkLevel = GameLevelCommon.defaultWalkLevel;

	public class _2dPosition
	{
		public _2dPosition(int x_, int y_, _2dPosition parent_)
		{
			x = x_; y = y_; parent = parent_;
		}
		public int x = 0;
		public int y = 0;
		public int G = 0;
		public int H = 0;
		public int F = 0; // F = G + H;
		public _2dPosition parent = null;
	}

	public AStarPathFinding(int size_)
	{
		navSize = size_;
	}

	public ArrayList pathFinding(int fromX, int fromY, int toX, int toY)
	{
		closeArray.Clear();
		openArray.Clear();

		_2dPosition from = new _2dPosition(fromX, fromY, null);
		_2dPosition to = new _2dPosition(toX, toY, null);

		// first, at from to close
		openArray.Add(from);

		_2dPosition destPoint = null;
		do
		{
			int lowerestIndex = -1;
			_2dPosition currentPoint = getLowestFFromOpen(out lowerestIndex);
			closeArray.Add(currentPoint);
			openArray.RemoveAt(lowerestIndex);// exception if lowerestIndex out of arraylist bound
			if(currentPoint.x == to.x && currentPoint.y == to.y)
			{
				destPoint = currentPoint;
				break; // find the answer
			}

			for(int j = 0; j < (int)eMapDirection.TOTAL; ++j)
			{
				_2dPosition newPoint = getClosetInfo(currentPoint, to, (eMapDirection)j);
				if(newPoint != null)
				{
					_2dPosition oldPoint_ = findFromList(closeArray, newPoint);
					if(oldPoint_ != null)
						continue;

					oldPoint_ = findFromList(openArray, newPoint);
					if(oldPoint_ == null)
						openArray.Add(newPoint); // important, the newest point must append to the end of array
					else
					{
						// if already exsit, 
						if(oldPoint_.G > newPoint.G)
						{
							oldPoint_.G = newPoint.G;
							oldPoint_.F = oldPoint_.G + oldPoint_.H;
							oldPoint_.parent = currentPoint;
						}
					}
				}
			}

		} while(openArray.Count > 0);

		if(destPoint != null)
		{
			ArrayList result_ = new ArrayList();
			_2dPosition node_ = destPoint;
			while(node_ != null)
			{
				result_.Insert(0, node_);
				node_ = node_.parent;
			}
			//behaviac.Debug.Log("Path point count : " + result_.Count.ToString());
			return result_;
		}

		return null;
	}

	private static _2dPosition findFromList(ArrayList list_, _2dPosition t_)
	{
		_2dPosition kp = null;
		for(int i = 0; i < list_.Count; ++i)
		{
			_2dPosition node_ = list_[i] as _2dPosition;
			if(node_.x == t_.x && node_.y == t_.y)
			{
				kp = node_;
				break;
			}
		}
		return kp;
	}

	private _2dPosition getLowestFFromOpen(out int index)
	{
		//todo: optimize
		_2dPosition lowest = null;
		int minF = int.MaxValue;
		index = -1;
		for(int i = 0; i < openArray.Count; ++i) // find lowest point of newest
		{
			_2dPosition t = openArray[i] as _2dPosition;
			if(t.F <= minF)
			{
				index = i;
				lowest = t;
				minF = t.F;
			}
			//else
			//{
			//	break;//need sorted list
			//}
		}
		return lowest;
	}
	
	private _2dPosition getClosetInfo(_2dPosition _from, _2dPosition to, eMapDirection _d)
	{
		if(_from.x <= 0 || _from.x >= navSize - 1 || to.x <= 0 || to.x >= navSize - 1)
		{
			behaviac.Debug.LogError("parameters of getClosetInfo() are illegal!");
			return null;
		}

		int newX = 0; int newY = 0;
		switch(_d)
		{
		case eMapDirection.LEFT:
			newX = _from.x - 1;
			newY = _from.y;
			break;
		case eMapDirection.RIGHT:
			newX = _from.x + 1;
			newY = _from.y;
			break;
		case eMapDirection.TOP:
			newX = _from.x;
			newY = _from.y - 1;
			break;
		case eMapDirection.BOTTOM:
			newX = _from.x;
			newY = _from.y + 1;
			break;
		}

		bool isWakable = GameLevelCommon.instance.isPointWakable(newX, newY, walkLevel);
		if(isWakable)
		{
			_2dPosition n = new _2dPosition(newX, newY, _from);
			n.G = _from.G + 1;
			n.H = Mathf.Abs(to.x - n.x) + Mathf.Abs(to.y - n.y);
			// substract overlap distance
			if(to.x == n.x || to.y == n.y)
				n.H -= 1;

			n.F = n.H + n.G;
			return n;
		}
		else
		{
			return null;
		}
	}
}
