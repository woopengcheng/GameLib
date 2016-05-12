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
#if UNITY_EDITOR || UNITY_STANDALONE_WIN
using System.Runtime.InteropServices;
#endif

[AddComponentMenu("BehaviacSystem")]
public class BehaviacSystem
{
    #region singleon
    private static BehaviacSystem _instance;
    public static BehaviacSystem Instance
    {
        get
        {
            if (_instance == null)
            {
                _instance = new BehaviacSystem();
            }

            return _instance;
        }
    }
    #endregion

	protected static BehaviacFileManager ms_fileSystem = null;

    private string WorkspaceExportPath
    {
        get
        {
            string relativePath = "/Resources/behaviac/exported";
            string path = "";
            if (Application.platform == RuntimePlatform.WindowsEditor)
            {
                path = Application.dataPath + relativePath;
            }
            else if (Application.platform == RuntimePlatform.WindowsPlayer)
            {
                path = Application.dataPath + relativePath;
            }
            else
            {
                path = "Assets" + relativePath;
            }

            return path;
        }
    }

	public bool Init ()
    {
        if (ms_fileSystem == null)
        {
            ms_fileSystem = new BehaviacFileManager();
        }

        {
            //< write log file
            behaviac.Config.IsLogging = true;
            //behaviac.Config.IsSocketing = false;

            behaviac.Workspace.Instance.FilePath = this.WorkspaceExportPath;
            //behaviac.Workspace.Instance.FileFormat = behaviac.Workspace.EFileFormat.EFF_cs;
            behaviac.Workspace.Instance.FileFormat = behaviac.Workspace.EFileFormat.EFF_xml;

            //register names
            behaviac.Agent.RegisterInstanceName<GameLevelCommon>("GameLevel");
            behaviac.Workspace.Instance.ExportMetas("behaviac/workspace/xmlmeta/BattleCityMeta.xml");

            behaviac.Debug.Log("Behaviac meta data export over.");
            behaviac.Agent.SetIdMask(0xffffffff);
        }

        return true;
    }

    public void Uninit()
	{
        behaviac.Workspace.Instance.Cleanup();
	}

}
