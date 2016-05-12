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

using System;
using System.IO;
using System.Windows.Forms;
using Behaviac.Design.Properties;

namespace Behaviac.Design
{
    public partial class ControlsDialog : Form
    {
        public ControlsDialog(bool bShowMeTip) {
            InitializeComponent();

            this.checkBoxNext.Visible = bShowMeTip;

            if (bShowMeTip) {
                this.checkBoxNext.Checked = !Settings.Default.ShowControlsOnStartUp;
            }
        }

        private void ControlsDialog_Load(object sender, EventArgs e) {
            string appDir = Path.GetDirectoryName(Application.ExecutablePath);
            string controlFile = (Settings.Default.Language == (int)Language.English || System.Threading.Thread.CurrentThread.CurrentUICulture.Name != "zh-CN")
                                 ? "..\\doc\\ControlHelp.html" : "..\\doc\\ControlHelp.zh-CN.html";
            controlFile = Path.Combine(appDir, controlFile);
            controlFile = Path.GetFullPath(controlFile);

            if (File.Exists(controlFile)) {
                webBrowser.Url = new Uri(controlFile);
            }
        }

        private void webBrowser_PreviewKeyDown(object sender, PreviewKeyDownEventArgs e) {
            if (e.KeyCode == Keys.Escape) {
                this.Close();
            }
        }

        private void ControlsDialog_KeyDown(object sender, KeyEventArgs e) {
            if (e.KeyCode == Keys.Escape) {
                this.Close();
            }
        }

        private void buttonClose_Click(object sender, EventArgs e) {
            this.Close();
        }

        private void ControlsDialog_FormClosed(object sender, FormClosedEventArgs e) {
            if (this.checkBoxNext.Visible) {
                //don't show me next time
                Settings.Default.ShowControlsOnStartUp = !this.checkBoxNext.Checked;
            }
        }

        private void loadWorkspace(string wksFile)
        {
            try
            {
                wksFile = Path.Combine(Application.StartupPath, wksFile);
                wksFile = Path.GetFullPath(wksFile);
                MainWindow.Instance.BehaviorTreeList.OpenWorkspace(wksFile);
            }
            catch
            {
            }
        }

        private void openGameDemo(string demoFile)
        {
            try
            {
                demoFile = Path.Combine(Application.StartupPath, demoFile);
                demoFile = Path.GetFullPath(demoFile);
                System.Diagnostics.Process.Start(demoFile);
            }
            catch
            {
            }
        }

        private void workspace1Button_Click(object sender, EventArgs e)
        {
            loadWorkspace("../../../integration/unity/Assets/behaviac/workspace/behaviacunittest.workspace.xml");
        }

        private void workspace2Button_Click(object sender, EventArgs e)
        {
            loadWorkspace("../../../integration/BattleCityDemo/Assets/behaviac/workspace/BattleCity.workspace.xml");

            openGameDemo("../../../integration/BuildExe/BattleCityDemo.exe");
        }

        private void workspace3Button_Click(object sender, EventArgs e)
        {
            loadWorkspace("../../../test/btunittest/BehaviacData/BehaviacUnitTestCpp.workspace.xml");
        }

        private void workspace4Button_Click(object sender, EventArgs e)
        {
            loadWorkspace("../../../example/spaceship/data/ships.workspace.xml");

            openGameDemo("../../../bin/spaceship_msvc_debug.exe");
        }

        private void referLinkTextBox_LinkClicked(object sender, LinkClickedEventArgs e)
        {
            try
            {
                System.Diagnostics.Process.Start(this.referLinkTextBox.Text);
            }
            catch
            {
            }
        }
    }
}
