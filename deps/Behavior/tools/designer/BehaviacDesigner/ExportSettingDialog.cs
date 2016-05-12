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
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Text;
using System.Windows.Forms;
using System.IO;
using Behaviac.Design.Nodes;
using Behaviac.Design.Properties;

namespace Behaviac.Design
{
    internal partial class ExportSettingDialog : Form
    {
        private ExporterInfo _exporterInfo = null;

        public ExportSettingDialog(ExporterInfo exporterInfo) {
            InitializeComponent();

            Debug.Check(exporterInfo != null);
            _exporterInfo = exporterInfo;

            bool enableIncludedFiles = (_exporterInfo.ID == "cpp");
            this.includedFilesGridView.Visible = enableIncludedFiles;
            this.addFilenameButton.Visible = enableIncludedFiles;
            this.removeFilenameButton.Visible = enableIncludedFiles;

            string[] tokens = _exporterInfo.Description.Split(' ');
            this.Text = tokens[0] + " " + this.Text;
        }

        protected override void OnShown(EventArgs e) {
            loadSettings();
        }

        private void loadSettings() {
            if (Workspace.Current != null && _exporterInfo != null) {
                string exportFullPath = Workspace.Current.GetExportAbsoluteFolder(_exporterInfo.ID);
                this.exportFolderTextBox.Text = exportFullPath;

                this.unifiedCheckBox.Checked = Workspace.Current.ExportedUnifiedFile(_exporterInfo.ID);
                this.generateCustomizedTypesCheckBox.Checked = Workspace.Current.GenerateCustomizedTypes(_exporterInfo.ID);

                List<string> exportIncludedFilenames = Workspace.Current.GetExportIncludedFilenames(_exporterInfo.ID);
                foreach(string filename in exportIncludedFilenames) {
                    if (!string.IsNullOrEmpty(filename)) {
                        string includedFilename = FileManagers.FileManager.MakeAbsolute(exportFullPath, filename);
                        addFilename(includedFilename);
                    }
                }
            }
        }

        private void saveSettings() {
            if (Workspace.Current != null && _exporterInfo != null) {
                string wsFilename = Workspace.Current.FileName;
                wsFilename = wsFilename.Replace('/', '\\');

                bool exportUnifiedFile = this.unifiedCheckBox.Checked;
                bool generateCustomizedTypes = this.generateCustomizedTypesCheckBox.Checked;

                string exportFullPath = this.exportFolderTextBox.Text.Replace('/', '\\');
                string exportFolder = exportFullPath;
                exportFolder = Workspace.MakeRelative(exportFolder, wsFilename, true, true);
                exportFolder = exportFolder.Replace('\\', '/');

                List<string> exportIncludedFilenames = new List<string>();
                foreach(DataGridViewRow row in this.includedFilesGridView.Rows) {
                    string filename = (string)row.Cells["filenameColumn"].Value;
                    filename = filename.Replace('/', '\\');
                    filename = Workspace.MakeRelative(filename, exportFullPath, true, true);
                    filename = filename.Replace('\\', '/');
                    exportIncludedFilenames.Add(filename);
                }

                Workspace.Current.SetExportInfo(_exporterInfo.ID, Workspace.Current.ShouldBeExported(_exporterInfo.ID), exportUnifiedFile, generateCustomizedTypes, exportFolder, exportIncludedFilenames);

                Workspace.SaveWorkspaceFile(Workspace.Current);
            }
        }

        private int getFilenameRowIndex(string filename) {
            for (int index = 0; index < this.includedFilesGridView.Rows.Count; index++) {
                DataGridViewRow row = this.includedFilesGridView.Rows[index];

                if (filename == (string)row.Cells["filenameColumn"].Value) {
                    return index;
                }
            }

            return -1;
        }

        private string getBrowseFilename(string pdbLoc) {
            using(OpenFileDialog dlg = new OpenFileDialog()) {
                dlg.Title = "Included File";
                dlg.Filter = "C++ Header Files(*.h)|*.h";
                dlg.Multiselect = false;

                if (DialogResult.OK == dlg.ShowDialog()) {
                    return dlg.FileName;
                }
            }

            return string.Empty;
        }

        private void addFilename(string filename) {
            int index = this.getFilenameRowIndex(filename);

            if (index < 0) {
                index = this.includedFilesGridView.Rows.Add();
            }

            DataGridViewRow row = this.includedFilesGridView.Rows[index];
            row.Cells["filenameColumn"].Value = filename;
            row.Cells["browseColumn"].Value = "...";
            row.Selected = true;
        }

        private void removeSelectedRows(DataGridView dataGridView) {
            if (dataGridView.SelectedRows.Count == 0) {
                return;
            }

            List<DataGridViewRow> removingRows = new List<DataGridViewRow>();
            foreach(DataGridViewRow row in dataGridView.SelectedRows) {
                removingRows.Add(row);
            }

            foreach(DataGridViewRow row in removingRows) {
                dataGridView.Rows.Remove(row);
            }
        }

        private void browseButton_Click(object sender, EventArgs e) {
            using(Ookii.Dialogs.VistaFolderBrowserDialog dialog = new Ookii.Dialogs.VistaFolderBrowserDialog()) {
                dialog.Description = "Export Location";
                dialog.SelectedPath = this.exportFolderTextBox.Text;

                if (dialog.ShowDialog() == DialogResult.OK) {
                    string driveStr0 = Path.GetPathRoot(Workspace.Current.FileName);
                    string driveStr1 = Path.GetPathRoot(dialog.SelectedPath);

                    if (driveStr1 != driveStr0)
                    {
                        MessageBox.Show(Resources.WorkspaceExportRootWarning, Resources.Warning, MessageBoxButtons.OK);
                        return;
                    }

                    this.exportFolderTextBox.Text = dialog.SelectedPath;
                }
            }
        }

        private void addPdbSearchButton_Click(object sender, EventArgs e) {
            this.addFilename("");
        }

        private void removePdbSearchButton_Click(object sender, EventArgs e) {
            removeSelectedRows(this.includedFilesGridView);
        }

        private void okButton_Click(object sender, EventArgs e) {
            saveSettings();
        }

        private void includedFilesGridView_CellContentClick(object sender, DataGridViewCellEventArgs e) {
            if (e.RowIndex < 0 || e.ColumnIndex < 0) {
                return;
            }

            // Browse
            if (e.ColumnIndex == 1) {
                DataGridViewRow row = this.includedFilesGridView.Rows[e.RowIndex];
                string filename = getBrowseFilename((string)row.Cells["filenameColumn"].Value);

                if (!string.IsNullOrEmpty(filename)) {
                    row.Cells["filenameColumn"].Value = filename;
                }
            }
        }
    }
}