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

public class UIScoreNumber : MonoBehaviour 
{
	public int number = 0;
	public Color textColor = new Color(0.6f, 0.6f, 0.0f);

	private int lastNumber = -1;
	// Use this for initialization
	void Start () 
	{
		MeshRenderer mr = gameObject.AddComponent<MeshRenderer>();
		mr.castShadows = false;
		mr.receiveShadows = false;
		gameObject.AddComponent<MeshFilter>();
		gameObject.AddComponent<BubbleHud>();
		Material mat = Resources.Load("Materials/ScoreNumber") as Material;
		mr.material = mat;
	}
	
	// Update is called once per frame
	void Update ()
	{
		if(lastNumber != number)
		{
			setNumber(number, textColor);
			lastNumber = number;
		}
	}

	void setNumber(int number, Color clr)
	{
		string str = number.ToString();	
			
		MeshFilter	f	=	GetComponent<MeshFilter>();
		f.mesh.Clear();
		
		char[] carray	=	str.ToCharArray();		
		uint	vertexCount	=	(uint)carray.Length*4;
		uint	indexCount	=	(uint)carray.Length*6;
		
		//float fBeginX	=	-carray.Length*0.5f;
		float fBeginX	=	0;
		
		Vector3[] 	pos	=	new Vector3[vertexCount];
		Vector2[]	uv		=	new Vector2[vertexCount];
		for(int i=0;i<carray.Length;i++)
		{						
			pos[i*4]	= new Vector3(fBeginX+i-0.6f,1,0);
			pos[i*4+1]	= new Vector3(fBeginX+i+0.6f,1,0);
			pos[i*4+2]	= new Vector3(fBeginX+i-0.6f,-1,0);
			pos[i*4+3]	= new Vector3(fBeginX+i+0.6f,-1,0);
			
			uint idx = (uint)carray[i] - '0';
			float	y	=	1.0f -	(idx&0xFC)/16.0f;
			float	x	=	(idx&0x3)/4.0f;

			float shrink = 0.4f*0.125f;
			uv[i*4]	= new Vector2(x + shrink,y);
			uv[i*4+1] = new Vector2(x + 0.25f - shrink,y);
			uv[i*4+2] = new Vector2(x + shrink,y-0.25f);
			uv[i*4+3] = new Vector2(x + 0.25f - shrink,y-0.25f);
		}
		f.mesh.vertices	=	pos;
		f.mesh.uv		=	uv;
		
		int[] index	=	new int[indexCount];
		for(int i=0;i<carray.Length;i++)
		{
			index[i*6]	=	i*4;
			index[i*6+1]	=	i*4+1;
			index[i*6+2]	=	i*4+2;
			index[i*6+3]	=	i*4+2;
			index[i*6+4]	=	i*4+1;
			index[i*6+5]	=	i*4+3;
			
		}
		f.mesh.triangles	=	index;
		f.mesh.bounds	=	new Bounds(new Vector3(-fBeginX*2,-1,-1),new Vector3(fBeginX*2,1,1));
		
		MeshRenderer r = GetComponent<MeshRenderer>();
		r.material.SetColor("_colorBias", clr);
	}
}
