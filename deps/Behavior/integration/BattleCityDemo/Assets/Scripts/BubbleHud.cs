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

public class BubbleHud : MonoBehaviour 
{
	public string num =	"0";
	public Color color = new Color(1,1,1,1);
	public bool isInit = false;
	// Use this for initialization
	void Start() 
	{
	
	}
	
	// Update is called once per frame
	void Update() 
	{
	
	}

	public static BubbleHud createBubbleHud(Player p_)
	{
		GameObject go = new GameObject();
		MeshRenderer mr = go.AddComponent<MeshRenderer>();
		mr.castShadows = false;
		mr.receiveShadows = false;
		go.AddComponent<MeshFilter>();
		BubbleHud bh = go.AddComponent<BubbleHud>();
		Material mat = Resources.Load("Materials/BubbleNumber") as Material;
		mr.material = mat;

		go.name = "HUD";
		go.transform.parent = p_.transform;
		go.transform.localScale = new Vector3(2.0f, 2.5f, 2.5f);

		return bh;
	}

	public void tick()
	{
		transform.rotation = Quaternion.Euler(90, 0, 0);
		transform.position = transform.parent.transform.position + new Vector3(0.7f, 4, 2.5f);
	}

	public void setNumber(int hp, Color clr, int life)
	{
		string number = "";
		while(hp > 0)
		{
			number += ":";
			--hp;
		}
		number += life.ToString();

		if(num != number || !isInit)
		{
			if(!isInit)
				isInit	=	true;

			MeshFilter	f	=	GetComponent<MeshFilter>();
			f.mesh.Clear();
			
			num	=	number;
			char[] carray	=	num.ToCharArray();
			
			uint	vertexCount	=	(uint)carray.Length*4;
			uint	indexCount	=	(uint)carray.Length*6;
			
			float fBeginX	=	-carray.Length*0.5f;

			Vector3[] 	pos	=	new Vector3[vertexCount];
			Vector2[]	uv		=	new Vector2[vertexCount];
			for(int i=0;i<carray.Length;i++)
			{
				
				
				pos[i*4]	=new Vector3(fBeginX+i-0.6f,1,0);
				pos[i*4+1]	=new Vector3(fBeginX+i+0.6f,1,0);
				pos[i*4+2]	=new Vector3(fBeginX+i-0.6f,-1,0);
				pos[i*4+3]	=new Vector3(fBeginX+i+0.6f,-1,0);
				
				uint idx = (uint)carray[i] - '0';
				float	y	=	1.0f -	(idx&0xFC)/16.0f;
				float	x	=	(idx&0x3)/4.0f;
				
				uv[i*4]	=new Vector2(x+0.4f*0.125f,y);
				uv[i*4+1]=new Vector2(x+0.25f-0.4f*0.125f,y);
				uv[i*4+2]=new Vector2(x+0.4f*0.125f,y-0.25f);
				uv[i*4+3]=new Vector2(x+0.25f-0.4f*0.125f,y-0.25f);
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
			r.material.SetColor("_colorBias",clr);
			color	=	clr;
		}
	}
}
