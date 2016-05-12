////////////////////////////////////////////////////////////////////////////////////////////////////
// Copyright (c) 2009, Daniel Kollmann
// All rights reserved.
//
// Redistribution and use in source and binary forms, with or without modification, are permitted
// provided that the following conditions are met:
//
// - Redistributions of source code must retain the above copyright notice, this list of conditions
//   and the following disclaimer.
//
// - Redistributions in binary form must reproduce the above copyright notice, this list of
//   conditions and the following disclaimer in the documentation and/or other materials provided
//   with the distribution.
//
// - Neither the name of Daniel Kollmann nor the names of its contributors may be used to endorse
//   or promote products derived from this software without specific prior written permission.
//
// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR
// IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND
// FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR
// CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
// DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
// DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
// WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY
// WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
////////////////////////////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// The above software in this distribution may have been modified by THL A29 Limited ("Tencent Modifications").
//
// All Tencent Modifications are Copyright (C) 2015 THL A29 Limited.
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////

using System;
using System.Collections.Generic;
using System.Windows.Forms;
using System.IO;
using Behaviac.Design.Properties;

namespace Behaviac.Design
{
    /// <summary>
    /// This class is the window which appears when creating or editing a workspace.
    /// </summary>
    internal partial class EditWorkspaceDialog : Form
    {
        internal EditWorkspaceDialog() {
            InitializeComponent();

            checkButtons();
        }

        private string _filename = string.Empty;

        /// <summary>
        /// The workspace which is currently edited or created.
        /// </summary>
        private Workspace _workspace = null;

        /// <summary>
        /// The workspace created or edited by the dialogue.
        /// </summary>
        internal Workspace Workspace {
            get { return _workspace; }
        }

        /// <summary>
        /// Sets the workspace you want to edit.
        /// </summary>
        /// <param name="ws">The workspace which will be edited.</param>
        internal void EditWorkspace(Workspace ws) {
            _workspace = ws;

            // Fill the form.
            this.nameTextBox.Text = ws.Name;
            this.XMLTextBox.Text = ws.XMLFolder;
            this.workspaceTextBox.Text = Path.GetDirectoryName(ws.FileName);
            this.sourceTextBox.Text = ws.Folder;
            this.exportTextBox.Text = ws.DefaultExportFolder;

            this.workspaceTextBox.Enabled = false;
            this.workspaceButton.Enabled = false;

            this._filename = ws.FileName;

            this.Text = Resources.WorkspaceEditTiltle;

            // Add all xml files into the listbox.
            if (!string.IsNullOrEmpty(ws.XMLFolder)) {
                DirectoryInfo dirInfo = new DirectoryInfo(Directory.GetCurrentDirectory());
                string xmlFolder = ws.XMLFolder;

                if (!Path.IsPathRooted(ws.XMLFolder)) {
                    xmlFolder = Path.GetFullPath(xmlFolder);
                }

                if (!Directory.Exists(ws.XMLFolder)) {
                    Directory.CreateDirectory(ws.XMLFolder);
                }

                string[] files = Directory.GetFiles(xmlFolder, "*.xml", SearchOption.TopDirectoryOnly);
                foreach(string file in files) {
                    string filename = Path.GetFileName(file);
                    pluginListBox.Items.Add(filename, ws.XMLPlugins.Contains(filename));
                }
            }

            checkButtons();
        }

        private void checkButtons() {
            string wksName = nameTextBox.Text.Trim();
            string wksLocation = workspaceTextBox.Text.Trim();
            string xmlfolder = XMLTextBox.Text.Trim();

            doneButton.Enabled = true;
            workspaceTextBox.Enabled = true;
            workspaceButton.Enabled = true;
            sourceTextBox.Enabled = true;
            sourceButton.Enabled = true;
            exportTextBox.Enabled = true;
            exportButton.Enabled = true;
            XMLTextBox.Enabled = true;
            XMLButton.Enabled = true;

            if (string.IsNullOrEmpty(wksName)) {
                doneButton.Enabled = false;
                workspaceTextBox.Enabled = false;
                workspaceButton.Enabled = false;
                sourceTextBox.Enabled = false;
                sourceButton.Enabled = false;
                exportTextBox.Enabled = false;
                exportButton.Enabled = false;
                XMLTextBox.Enabled = false;
                XMLButton.Enabled = false;

            } else if (string.IsNullOrEmpty(wksLocation)) {
                doneButton.Enabled = false;
                sourceTextBox.Enabled = false;
                sourceButton.Enabled = false;
                exportTextBox.Enabled = false;
                exportButton.Enabled = false;
                XMLTextBox.Enabled = false;
                XMLButton.Enabled = false;

            } else if (string.IsNullOrEmpty(xmlfolder)) {
                //doneButton.Enabled = false;
            }
        }

        /// <summary>
        /// Handles when the done button is pressed.
        /// </summary>
        private void doneButton_Click(object sender, EventArgs e) {
            string wksName = nameTextBox.Text.Trim();
            string wksLocation = workspaceTextBox.Text.Trim();
            string xmlfolder = XMLTextBox.Text.Trim();

            if (string.IsNullOrEmpty(wksName) ||
                string.IsNullOrEmpty(wksLocation) || !Directory.Exists(wksLocation) )
                //string.IsNullOrEmpty(xmlfolder) || !Directory.Exists(xmlfolder))
            {
                MessageBox.Show(Resources.WorkspaceSettingWarning, Resources.Warning, MessageBoxButtons.OK);
                return;
            }

            string driveStr0 = Path.GetPathRoot(wksLocation);

            // create the given behavior folder if it does not exist
            string behaviorFolder = this.sourceTextBox.Text;

            if (string.IsNullOrEmpty(behaviorFolder)) {
                behaviorFolder = Path.Combine(wksLocation, "behaviors");
            }

            string driveStr1 = Path.GetPathRoot(behaviorFolder);
            Debug.Check(driveStr1 == driveStr0);

            if (!Directory.Exists(behaviorFolder)) {
                Directory.CreateDirectory(behaviorFolder);
            }

            // create the given export folder if it does not exist
            string exportFolder = this.exportTextBox.Text;

            if (string.IsNullOrEmpty(exportFolder)) {
                exportFolder = Path.Combine(wksLocation, "exported");
            }

            string driveStr2 = Path.GetPathRoot(behaviorFolder);
            Debug.Check(driveStr2 == driveStr0);

            if (!Directory.Exists(exportFolder)) {
                Directory.CreateDirectory(exportFolder);
            }

            // create the updated or new workspace
            if (_workspace != null) {
                _workspace = new Workspace(_filename, wksName, xmlfolder, behaviorFolder, exportFolder, _workspace.ExportDatas);

            } else {
                _workspace = new Workspace(_filename, wksName, xmlfolder, behaviorFolder, exportFolder);
            }

            if (!Plugin.IsASCII(_workspace.RelativeDefaultExportFolder)) {
                string errorMsg = string.Format("The relative export path '{0}' can only be ASCII", _workspace.RelativeDefaultExportFolder);
                MessageBox.Show(errorMsg, Resources.FileError, MessageBoxButtons.OK);
                return;
            }

            // update plugins
            foreach(string plugin in pluginListBox.CheckedItems) {
                _workspace.AddXMLPlugin(plugin);
            }

            Close();
        }

        /// <summary>
        /// Handles when the cancel button is pressed.
        /// </summary>
        private void cancelButton_Click(object sender, EventArgs e) {
            // we did not edit or create a workspace
            _workspace = null;

            Close();
        }

        private void setFilename() {
            string name = this.nameTextBox.Text.Trim();
            string location = this.workspaceTextBox.Text.Trim();

            if (!string.IsNullOrEmpty(name) && !string.IsNullOrEmpty(location)) {
                _filename = Path.Combine(location, name);
                _filename = Path.ChangeExtension(_filename, "workspace.xml");
            }
        }

        private void nameTextBox_TextChanged(object sender, EventArgs e) {
            setFilename();

            checkButtons();
        }

        private void workspaceButton_Click(object sender, EventArgs e) {
            folderBrowserDialog.Description = Resources.SelectWorkspaceFolder;
            folderBrowserDialog.ShowNewFolderButton = true;

            // assign the user path entered by the user to the browse dialogue
            if (!string.IsNullOrEmpty(this.workspaceTextBox.Text)) {
                folderBrowserDialog.SelectedPath = Path.GetFullPath(this.workspaceTextBox.Text);
            }

            // assign the path selected by the user to the textbox
            if (folderBrowserDialog.ShowDialog() == DialogResult.OK) {
                this.workspaceTextBox.Text = folderBrowserDialog.SelectedPath;

                if (string.IsNullOrEmpty(this.sourceTextBox.Text)) {
                    this.sourceTextBox.Text = Path.Combine(workspaceTextBox.Text, "behaviors");
                }

                if (string.IsNullOrEmpty(this.exportTextBox.Text)) {
                    this.exportTextBox.Text = Path.Combine(workspaceTextBox.Text, "exported");
                }

                //if (string.IsNullOrEmpty(this.xmlFolderTextBox.Text))
                //{
                //    this.xmlFolderTextBox.Text = Path.Combine(locationTextBox.Text, "xmlmeta");
                //}

                setFilename();
            }

            checkButtons();
        }

        /// <summary>
        /// Handles when the browse button for the default xml folder is clicked.
        /// </summary>
        private void importXMLButton_Click(object sender, EventArgs e) {
            if (string.IsNullOrEmpty(_filename)) {
                MessageBox.Show(Resources.WorkspaceNameWarning, Resources.Warning, MessageBoxButtons.OK);
                return;
            }

            folderBrowserDialog.Description = Resources.SelectXMLPluginsFolder;
            folderBrowserDialog.ShowNewFolderButton = false;

            if (!string.IsNullOrEmpty(XMLTextBox.Text)) {
                folderBrowserDialog.SelectedPath = Path.GetFullPath(XMLTextBox.Text);
            }

            if (folderBrowserDialog.ShowDialog() == DialogResult.OK) {
                string wsFolder = Path.GetDirectoryName(_filename);
                string folder = folderBrowserDialog.SelectedPath;

                string driveStr0 = Path.GetPathRoot(this.workspaceTextBox.Text);
                string driveStr1 = Path.GetPathRoot(folder);

                if (driveStr1 != driveStr0) {
                    MessageBox.Show(Resources.WorkspaceXmlMetaRootWarning, Resources.Warning, MessageBoxButtons.OK);
                    return;
                }

                XMLTextBox.Text = folder;

                // Collect the previous XML plugin items.
                List<string> removedPlugins = new List<string>();
                foreach(string plugin in pluginListBox.Items)

                if (Path.GetExtension(plugin) == ".xml") { // xml
                    removedPlugins.Add(plugin);
                }

                // Remove the previous XML plugin items.
                foreach(string plugin in removedPlugins)
                pluginListBox.Items.Remove(plugin);

                // Add all XML plugins into the listbox from the current XML folder.
                string[] filenames = Directory.GetFiles(folder, "*.xml", SearchOption.TopDirectoryOnly);
                foreach(string filename in filenames)
                pluginListBox.Items.Add(Path.GetFileName(filename), true);
            }

            checkButtons();
        }

        private void buttonSource_Click(object sender, EventArgs e) {
            folderBrowserDialog.Description = Resources.SelectWorkspaceFolder;
            folderBrowserDialog.ShowNewFolderButton = true;

            // assign the user path entered by the user to the browse dialogue
            if (!string.IsNullOrEmpty(this.sourceTextBox.Text)) {
                folderBrowserDialog.SelectedPath = Path.GetFullPath(this.sourceTextBox.Text);
            }

            // assign the path selected by the user to the textbox
            if (folderBrowserDialog.ShowDialog() == DialogResult.OK) {
                string driveStr0 = Path.GetPathRoot(this.workspaceTextBox.Text);
                string driveStr1 = Path.GetPathRoot(folderBrowserDialog.SelectedPath);

                if (driveStr1 != driveStr0) {
                    MessageBox.Show(Resources.WorkspaceSourceRootWarning, Resources.Warning, MessageBoxButtons.OK);
                    return;
                }

                this.sourceTextBox.Text = folderBrowserDialog.SelectedPath;
            }

            checkButtons();
        }

        private void buttonExport_Click(object sender, EventArgs e) {
            folderBrowserDialog.Description = Resources.SelectWorkspaceFolder;
            folderBrowserDialog.ShowNewFolderButton = true;

            // assign the user path entered by the user to the browse dialogue
            if (!string.IsNullOrEmpty(this.exportTextBox.Text)) {
                folderBrowserDialog.SelectedPath = Path.GetFullPath(this.exportTextBox.Text);
            }

            // assign the path selected by the user to the textbox
            if (folderBrowserDialog.ShowDialog() == DialogResult.OK) {
                string driveStr0 = Path.GetPathRoot(this.workspaceTextBox.Text);
                string driveStr1 = Path.GetPathRoot(folderBrowserDialog.SelectedPath);

                if (driveStr1 != driveStr0) {
                    MessageBox.Show(Resources.WorkspaceExportRootWarning, Resources.Warning, MessageBoxButtons.OK);
                    return;
                }

                this.exportTextBox.Text = folderBrowserDialog.SelectedPath;
            }

            checkButtons();
        }

        private void locationTextBox_Leave(object sender, EventArgs e) {
            if (_workspace != null && !Directory.Exists(this.workspaceTextBox.Text)) {
                this.workspaceTextBox.Text = Path.GetDirectoryName(_workspace.FileName);
                checkButtons();
            }
        }

        private void locationTextBox_KeyDown(object sender, KeyEventArgs e) {
            if (e.KeyCode == Keys.Enter) {
                if (_workspace != null && !Directory.Exists(this.workspaceTextBox.Text)) {
                    this.workspaceTextBox.Text = Path.GetDirectoryName(_workspace.FileName);
                    checkButtons();
                }
            }
        }

        private void sourceTextBox_Leave(object sender, EventArgs e) {
            if (_workspace != null && !Directory.Exists(this.sourceTextBox.Text)) {
                this.sourceTextBox.Text = _workspace.Folder;
                checkButtons();
            }
        }

        private void sourceTextBox_KeyDown(object sender, KeyEventArgs e) {
            if (e.KeyCode == Keys.Enter) {
                if (_workspace != null && !Directory.Exists(this.sourceTextBox.Text)) {
                    this.sourceTextBox.Text = _workspace.Folder;
                    checkButtons();
                }
            }
        }

        private void exportTextBox_Leave(object sender, EventArgs e) {
            if (_workspace != null && !Directory.Exists(this.exportTextBox.Text)) {
                this.exportTextBox.Text = _workspace.DefaultExportFolder;
                checkButtons();
            }
        }

        private void exportTextBox_KeyDown(object sender, KeyEventArgs e) {
            if (e.KeyCode == Keys.Enter) {
                if (_workspace != null && !Directory.Exists(this.exportTextBox.Text)) {
                    this.exportTextBox.Text = _workspace.DefaultExportFolder;
                    checkButtons();
                }
            }
        }

        private void xmlFolderTextBox_Leave(object sender, EventArgs e) {
            if (_workspace != null && !Directory.Exists(this.XMLTextBox.Text)) {
                this.XMLTextBox.Text = _workspace.XMLFolder;
                checkButtons();
            }
        }

        private void xmlFolderTextBox_KeyDown(object sender, KeyEventArgs e) {
            if (e.KeyCode == Keys.Enter) {
                if (_workspace != null && !Directory.Exists(this.XMLTextBox.Text)) {
                    this.XMLTextBox.Text = _workspace.XMLFolder;
                    checkButtons();
                }
            }
        }
    }
}