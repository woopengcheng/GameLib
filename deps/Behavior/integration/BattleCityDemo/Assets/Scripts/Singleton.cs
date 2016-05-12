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

public class Singleton<T> : MonoBehaviour where T : MonoBehaviour
{
	private static T _instance;     
	private static object _lock = new object();
	public static T Instance
	{       
		get       
		{         
			if (applicationIsQuitting) 
			{          
				behaviac.Debug.LogWarning("[Singleton] Instance "+ typeof(T) +   
					" already destroyed on application quit." +          
					"Won't create again - returning null.");          
				return null;         
			}
			lock(_lock)
			{ 
				if (_instance == null)
				{
					_instance = (T) FindObjectOfType(typeof(T));
					if (_instance == null)
					{
						GameObject singleton = new GameObject();
						_instance = singleton.AddComponent<T>();
						singleton.name = "(singleton) "+ typeof(T).ToString();
						DontDestroyOnLoad(singleton);
						behaviac.Debug.Log("[Singleton] An instance of " + typeof(T) + 
							" is needed in the scene, so '" + singleton +
							"' was created with DontDestroyOnLoad.");
					} else {
						behaviac.Debug.Log("[Singleton] Using instance already created: " +
							_instance.gameObject.name);
					}
				}
				return _instance;
			}
		}
	}
	private static bool applicationIsQuitting = false;
/// <summary>
/// When unity quits, it destroys objects in a random order.
/// In principle, a Singleton is only destroyed when application quits.    
/// If any script calls Instance after it have been destroyed,     
///   it will create a buggy ghost object that will stay on the Editor scene    
///   even after stopping playing the Application. Really bad!    
/// So, this was made to be sure we're not creating that buggy ghost object.    
/// </summary>
    public void OnDestroy () {
		applicationIsQuitting = true;
	}
}