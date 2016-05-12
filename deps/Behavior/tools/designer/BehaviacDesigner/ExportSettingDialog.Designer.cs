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

namespace Behaviac.Design
{
    partial class ExportSettingDialog
    {
        /// <summary>
        /// Required designer variable.
        /// </summary>
        private System.ComponentModel.IContainer components = null;

        /// <summary>
        /// Clean up any resources being used.
        /// </summary>
        /// <param name="disposing">true if managed resources should be disposed; otherwise, false.</param>
        protected override void Dispose(bool disposing) {
            if (disposing && (components != null)) {
                components.Dispose();
            }

            base.Dispose(disposing);
        }

        #region Windows Form Designer generated code

        /// <summary>
        /// Required method for Designer support - do not modify
        /// the contents of this method with the code editor.
        /// </summary>
        private void InitializeComponent() {
            this.components = new System.ComponentModel.Container();
            System.ComponentModel.ComponentResourceManager resources = new System.ComponentModel.ComponentResourceManager(typeof(ExportSettingDialog));
            System.Windows.Forms.DataGridViewCellStyle dataGridViewCellStyle1 = new System.Windows.Forms.DataGridViewCellStyle();
            System.Windows.Forms.DataGridViewCellStyle dataGridViewCellStyle2 = new System.Windows.Forms.DataGridViewCellStyle();
            System.Windows.Forms.DataGridViewCellStyle dataGridViewCellStyle3 = new System.Windows.Forms.DataGridViewCellStyle();
            System.Windows.Forms.DataGridViewCellStyle dataGridViewCellStyle4 = new System.Windows.Forms.DataGridViewCellStyle();
            this.okButton = new System.Windows.Forms.Button();
            this.imageList = new System.Windows.Forms.ImageList(this.components);
            this.includedFilesGridView = new System.Windows.Forms.DataGridView();
            this.filenameColumn = new System.Windows.Forms.DataGridViewTextBoxColumn();
            this.browseColumn = new System.Windows.Forms.DataGridViewButtonColumn();
            this.removeFilenameButton = new System.Windows.Forms.Button();
            this.addFilenameButton = new System.Windows.Forms.Button();
            this.cancelButton = new System.Windows.Forms.Button();
            this.exportFolderTextBox = new System.Windows.Forms.TextBox();
            this.browseButton = new System.Windows.Forms.Button();
            this.exportFolderLabel = new System.Windows.Forms.Label();
            this.unifiedCheckBox = new System.Windows.Forms.CheckBox();
            this.generateCustomizedTypesCheckBox = new System.Windows.Forms.CheckBox();
            ((System.ComponentModel.ISupportInitialize)(this.includedFilesGridView)).BeginInit();
            this.SuspendLayout();
            // 
            // okButton
            // 
            resources.ApplyResources(this.okButton, "okButton");
            this.okButton.BackColor = System.Drawing.Color.FromArgb(((int)(((byte)(65)))), ((int)(((byte)(65)))), ((int)(((byte)(65)))));
            this.okButton.DialogResult = System.Windows.Forms.DialogResult.OK;
            this.okButton.FlatAppearance.MouseDownBackColor = System.Drawing.Color.DarkGray;
            this.okButton.FlatAppearance.MouseOverBackColor = System.Drawing.Color.DarkGray;
            this.okButton.Name = "okButton";
            this.okButton.UseVisualStyleBackColor = false;
            this.okButton.Click += new System.EventHandler(this.okButton_Click);
            // 
            // imageList
            // 
            this.imageList.ImageStream = ((System.Windows.Forms.ImageListStreamer)(resources.GetObject("imageList.ImageStream")));
            this.imageList.TransparentColor = System.Drawing.Color.Magenta;
            this.imageList.Images.SetKeyName(0, "VSFolder_closed.bmp");
            this.imageList.Images.SetKeyName(1, "DocumentHS.png");
            // 
            // includedFilesGridView
            // 
            resources.ApplyResources(this.includedFilesGridView, "includedFilesGridView");
            this.includedFilesGridView.AllowUserToAddRows = false;
            this.includedFilesGridView.AllowUserToDeleteRows = false;
            this.includedFilesGridView.AllowUserToResizeRows = false;
            this.includedFilesGridView.BackgroundColor = System.Drawing.SystemColors.WindowFrame;
            this.includedFilesGridView.ColumnHeadersBorderStyle = System.Windows.Forms.DataGridViewHeaderBorderStyle.Single;
            dataGridViewCellStyle1.Alignment = System.Windows.Forms.DataGridViewContentAlignment.MiddleLeft;
            dataGridViewCellStyle1.BackColor = System.Drawing.Color.FromArgb(((int)(((byte)(56)))), ((int)(((byte)(56)))), ((int)(((byte)(56)))));
            dataGridViewCellStyle1.Font = new System.Drawing.Font("SimSun", 9F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(134)));
            dataGridViewCellStyle1.ForeColor = System.Drawing.Color.LightGray;
            dataGridViewCellStyle1.SelectionBackColor = System.Drawing.SystemColors.Highlight;
            dataGridViewCellStyle1.SelectionForeColor = System.Drawing.SystemColors.HighlightText;
            dataGridViewCellStyle1.WrapMode = System.Windows.Forms.DataGridViewTriState.True;
            this.includedFilesGridView.ColumnHeadersDefaultCellStyle = dataGridViewCellStyle1;
            this.includedFilesGridView.ColumnHeadersHeightSizeMode = System.Windows.Forms.DataGridViewColumnHeadersHeightSizeMode.AutoSize;
            this.includedFilesGridView.Columns.AddRange(new System.Windows.Forms.DataGridViewColumn[] {
            this.filenameColumn,
            this.browseColumn});
            dataGridViewCellStyle2.Alignment = System.Windows.Forms.DataGridViewContentAlignment.MiddleLeft;
            dataGridViewCellStyle2.BackColor = System.Drawing.Color.FromArgb(((int)(((byte)(56)))), ((int)(((byte)(56)))), ((int)(((byte)(56)))));
            dataGridViewCellStyle2.Font = new System.Drawing.Font("SimSun", 9F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(134)));
            dataGridViewCellStyle2.ForeColor = System.Drawing.Color.LightGray;
            dataGridViewCellStyle2.SelectionBackColor = System.Drawing.SystemColors.Highlight;
            dataGridViewCellStyle2.SelectionForeColor = System.Drawing.SystemColors.HighlightText;
            dataGridViewCellStyle2.WrapMode = System.Windows.Forms.DataGridViewTriState.False;
            this.includedFilesGridView.DefaultCellStyle = dataGridViewCellStyle2;
            this.includedFilesGridView.MultiSelect = false;
            this.includedFilesGridView.Name = "includedFilesGridView";
            this.includedFilesGridView.RowHeadersBorderStyle = System.Windows.Forms.DataGridViewHeaderBorderStyle.Single;
            dataGridViewCellStyle3.Alignment = System.Windows.Forms.DataGridViewContentAlignment.MiddleLeft;
            dataGridViewCellStyle3.BackColor = System.Drawing.Color.FromArgb(((int)(((byte)(56)))), ((int)(((byte)(56)))), ((int)(((byte)(56)))));
            dataGridViewCellStyle3.Font = new System.Drawing.Font("SimSun", 9F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(134)));
            dataGridViewCellStyle3.ForeColor = System.Drawing.Color.LightGray;
            dataGridViewCellStyle3.SelectionBackColor = System.Drawing.SystemColors.Highlight;
            dataGridViewCellStyle3.SelectionForeColor = System.Drawing.SystemColors.HighlightText;
            dataGridViewCellStyle3.WrapMode = System.Windows.Forms.DataGridViewTriState.True;
            this.includedFilesGridView.RowHeadersDefaultCellStyle = dataGridViewCellStyle3;
            this.includedFilesGridView.RowHeadersVisible = false;
            dataGridViewCellStyle4.BackColor = System.Drawing.Color.FromArgb(((int)(((byte)(56)))), ((int)(((byte)(56)))), ((int)(((byte)(56)))));
            this.includedFilesGridView.RowsDefaultCellStyle = dataGridViewCellStyle4;
            this.includedFilesGridView.RowTemplate.Height = 23;
            this.includedFilesGridView.SelectionMode = System.Windows.Forms.DataGridViewSelectionMode.FullRowSelect;
            this.includedFilesGridView.CellContentClick += new System.Windows.Forms.DataGridViewCellEventHandler(this.includedFilesGridView_CellContentClick);
            // 
            // filenameColumn
            // 
            this.filenameColumn.AutoSizeMode = System.Windows.Forms.DataGridViewAutoSizeColumnMode.Fill;
            resources.ApplyResources(this.filenameColumn, "filenameColumn");
            this.filenameColumn.Name = "filenameColumn";
            // 
            // browseColumn
            // 
            resources.ApplyResources(this.browseColumn, "browseColumn");
            this.browseColumn.Name = "browseColumn";
            this.browseColumn.Resizable = System.Windows.Forms.DataGridViewTriState.True;
            this.browseColumn.SortMode = System.Windows.Forms.DataGridViewColumnSortMode.Automatic;
            // 
            // removeFilenameButton
            // 
            resources.ApplyResources(this.removeFilenameButton, "removeFilenameButton");
            this.removeFilenameButton.BackColor = System.Drawing.Color.FromArgb(((int)(((byte)(65)))), ((int)(((byte)(65)))), ((int)(((byte)(65)))));
            this.removeFilenameButton.FlatAppearance.MouseDownBackColor = System.Drawing.Color.DarkGray;
            this.removeFilenameButton.FlatAppearance.MouseOverBackColor = System.Drawing.Color.DarkGray;
            this.removeFilenameButton.Name = "removeFilenameButton";
            this.removeFilenameButton.UseVisualStyleBackColor = false;
            this.removeFilenameButton.Click += new System.EventHandler(this.removePdbSearchButton_Click);
            // 
            // addFilenameButton
            // 
            resources.ApplyResources(this.addFilenameButton, "addFilenameButton");
            this.addFilenameButton.BackColor = System.Drawing.Color.FromArgb(((int)(((byte)(65)))), ((int)(((byte)(65)))), ((int)(((byte)(65)))));
            this.addFilenameButton.FlatAppearance.MouseDownBackColor = System.Drawing.Color.DarkGray;
            this.addFilenameButton.FlatAppearance.MouseOverBackColor = System.Drawing.Color.DarkGray;
            this.addFilenameButton.Name = "addFilenameButton";
            this.addFilenameButton.UseVisualStyleBackColor = false;
            this.addFilenameButton.Click += new System.EventHandler(this.addPdbSearchButton_Click);
            // 
            // cancelButton
            // 
            resources.ApplyResources(this.cancelButton, "cancelButton");
            this.cancelButton.BackColor = System.Drawing.Color.FromArgb(((int)(((byte)(65)))), ((int)(((byte)(65)))), ((int)(((byte)(65)))));
            this.cancelButton.DialogResult = System.Windows.Forms.DialogResult.Cancel;
            this.cancelButton.FlatAppearance.MouseDownBackColor = System.Drawing.Color.DarkGray;
            this.cancelButton.FlatAppearance.MouseOverBackColor = System.Drawing.Color.DarkGray;
            this.cancelButton.Name = "cancelButton";
            this.cancelButton.UseVisualStyleBackColor = false;
            // 
            // exportFolderTextBox
            // 
            resources.ApplyResources(this.exportFolderTextBox, "exportFolderTextBox");
            this.exportFolderTextBox.BackColor = System.Drawing.Color.FromArgb(((int)(((byte)(65)))), ((int)(((byte)(65)))), ((int)(((byte)(65)))));
            this.exportFolderTextBox.BorderStyle = System.Windows.Forms.BorderStyle.FixedSingle;
            this.exportFolderTextBox.ForeColor = System.Drawing.Color.LightGray;
            this.exportFolderTextBox.Name = "exportFolderTextBox";
            this.exportFolderTextBox.ReadOnly = true;
            // 
            // browseButton
            // 
            resources.ApplyResources(this.browseButton, "browseButton");
            this.browseButton.BackColor = System.Drawing.Color.FromArgb(((int)(((byte)(65)))), ((int)(((byte)(65)))), ((int)(((byte)(65)))));
            this.browseButton.FlatAppearance.MouseDownBackColor = System.Drawing.Color.DarkGray;
            this.browseButton.FlatAppearance.MouseOverBackColor = System.Drawing.Color.DarkGray;
            this.browseButton.Name = "browseButton";
            this.browseButton.UseVisualStyleBackColor = false;
            this.browseButton.Click += new System.EventHandler(this.browseButton_Click);
            // 
            // exportFolderLabel
            // 
            resources.ApplyResources(this.exportFolderLabel, "exportFolderLabel");
            this.exportFolderLabel.Name = "exportFolderLabel";
            // 
            // unifiedCheckBox
            // 
            resources.ApplyResources(this.unifiedCheckBox, "unifiedCheckBox");
            this.unifiedCheckBox.BackColor = System.Drawing.Color.FromArgb(((int)(((byte)(65)))), ((int)(((byte)(65)))), ((int)(((byte)(65)))));
            this.unifiedCheckBox.Checked = true;
            this.unifiedCheckBox.CheckState = System.Windows.Forms.CheckState.Checked;
            this.unifiedCheckBox.ForeColor = System.Drawing.Color.LightGray;
            this.unifiedCheckBox.Name = "unifiedCheckBox";
            this.unifiedCheckBox.UseVisualStyleBackColor = false;
            // 
            // generateCustomizedTypesCheckBox
            // 
            resources.ApplyResources(this.generateCustomizedTypesCheckBox, "generateCustomizedTypesCheckBox");
            this.generateCustomizedTypesCheckBox.BackColor = System.Drawing.Color.FromArgb(((int)(((byte)(65)))), ((int)(((byte)(65)))), ((int)(((byte)(65)))));
            this.generateCustomizedTypesCheckBox.Checked = true;
            this.generateCustomizedTypesCheckBox.CheckState = System.Windows.Forms.CheckState.Checked;
            this.generateCustomizedTypesCheckBox.ForeColor = System.Drawing.Color.LightGray;
            this.generateCustomizedTypesCheckBox.Name = "generateCustomizedTypesCheckBox";
            this.generateCustomizedTypesCheckBox.UseVisualStyleBackColor = false;
            // 
            // ExportSettingDialog
            // 
            this.AcceptButton = this.okButton;
            resources.ApplyResources(this, "$this");
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.BackColor = System.Drawing.Color.FromArgb(((int)(((byte)(56)))), ((int)(((byte)(56)))), ((int)(((byte)(56)))));
            this.CancelButton = this.cancelButton;
            this.Controls.Add(this.generateCustomizedTypesCheckBox);
            this.Controls.Add(this.unifiedCheckBox);
            this.Controls.Add(this.exportFolderLabel);
            this.Controls.Add(this.browseButton);
            this.Controls.Add(this.exportFolderTextBox);
            this.Controls.Add(this.cancelButton);
            this.Controls.Add(this.removeFilenameButton);
            this.Controls.Add(this.addFilenameButton);
            this.Controls.Add(this.includedFilesGridView);
            this.Controls.Add(this.okButton);
            this.ForeColor = System.Drawing.Color.LightGray;
            this.MaximizeBox = false;
            this.MinimizeBox = false;
            this.Name = "ExportSettingDialog";
            this.ShowInTaskbar = false;
            ((System.ComponentModel.ISupportInitialize)(this.includedFilesGridView)).EndInit();
            this.ResumeLayout(false);
            this.PerformLayout();

        }

        #endregion

        private System.Windows.Forms.Button okButton;
        private System.Windows.Forms.ImageList imageList;
        private System.Windows.Forms.DataGridView includedFilesGridView;
        private System.Windows.Forms.Button removeFilenameButton;
        private System.Windows.Forms.Button addFilenameButton;
        private System.Windows.Forms.DataGridViewTextBoxColumn filenameColumn;
        private System.Windows.Forms.DataGridViewButtonColumn browseColumn;
        private System.Windows.Forms.Button cancelButton;
        private System.Windows.Forms.TextBox exportFolderTextBox;
        private System.Windows.Forms.Button browseButton;
        private System.Windows.Forms.Label exportFolderLabel;
        private System.Windows.Forms.CheckBox unifiedCheckBox;
        private System.Windows.Forms.CheckBox generateCustomizedTypesCheckBox;
    }
}