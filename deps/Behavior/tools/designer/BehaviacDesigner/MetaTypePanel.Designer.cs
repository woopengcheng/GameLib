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
    partial class MetaTypePanel
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

        #region Component Designer generated code

        /// <summary>
        /// Required method for Designer support - do not modify
        /// the contents of this method with the code editor.
        /// </summary>
        private void InitializeComponent() {
            System.ComponentModel.ComponentResourceManager resources = new System.ComponentModel.ComponentResourceManager(typeof(MetaTypePanel));
            this.typeComboBox = new System.Windows.Forms.ComboBox();
            this.typeLabel = new System.Windows.Forms.Label();
            this.descGroupBox = new System.Windows.Forms.GroupBox();
            this.baseComboBox = new System.Windows.Forms.ComboBox();
            this.baseLabel = new System.Windows.Forms.Label();
            this.descTextBox = new System.Windows.Forms.TextBox();
            this.descLabel = new System.Windows.Forms.Label();
            this.dispTextBox = new System.Windows.Forms.TextBox();
            this.dispLabel = new System.Windows.Forms.Label();
            this.nameTextBox = new System.Windows.Forms.TextBox();
            this.agentLabel = new System.Windows.Forms.Label();
            this.descGroupBox.SuspendLayout();
            this.SuspendLayout();
            // 
            // typeComboBox
            // 
            resources.ApplyResources(this.typeComboBox, "typeComboBox");
            this.typeComboBox.BackColor = System.Drawing.Color.FromArgb(((int)(((byte)(65)))), ((int)(((byte)(65)))), ((int)(((byte)(65)))));
            this.typeComboBox.DropDownStyle = System.Windows.Forms.ComboBoxStyle.DropDownList;
            this.typeComboBox.ForeColor = System.Drawing.Color.LightGray;
            this.typeComboBox.FormattingEnabled = true;
            this.typeComboBox.Name = "typeComboBox";
            this.typeComboBox.SelectedIndexChanged += new System.EventHandler(this.typeComboBox_SelectedIndexChanged);
            // 
            // typeLabel
            // 
            resources.ApplyResources(this.typeLabel, "typeLabel");
            this.typeLabel.Name = "typeLabel";
            // 
            // descGroupBox
            // 
            resources.ApplyResources(this.descGroupBox, "descGroupBox");
            this.descGroupBox.BackColor = System.Drawing.Color.FromArgb(((int)(((byte)(56)))), ((int)(((byte)(56)))), ((int)(((byte)(56)))));
            this.descGroupBox.Controls.Add(this.typeComboBox);
            this.descGroupBox.Controls.Add(this.typeLabel);
            this.descGroupBox.Controls.Add(this.baseComboBox);
            this.descGroupBox.Controls.Add(this.baseLabel);
            this.descGroupBox.Controls.Add(this.descTextBox);
            this.descGroupBox.Controls.Add(this.descLabel);
            this.descGroupBox.Controls.Add(this.dispTextBox);
            this.descGroupBox.Controls.Add(this.dispLabel);
            this.descGroupBox.Controls.Add(this.nameTextBox);
            this.descGroupBox.Controls.Add(this.agentLabel);
            this.descGroupBox.ForeColor = System.Drawing.Color.LightGray;
            this.descGroupBox.Name = "descGroupBox";
            this.descGroupBox.TabStop = false;
            // 
            // baseComboBox
            // 
            resources.ApplyResources(this.baseComboBox, "baseComboBox");
            this.baseComboBox.BackColor = System.Drawing.Color.FromArgb(((int)(((byte)(65)))), ((int)(((byte)(65)))), ((int)(((byte)(65)))));
            this.baseComboBox.DropDownStyle = System.Windows.Forms.ComboBoxStyle.DropDownList;
            this.baseComboBox.ForeColor = System.Drawing.Color.LightGray;
            this.baseComboBox.FormattingEnabled = true;
            this.baseComboBox.Name = "baseComboBox";
            this.baseComboBox.SelectedIndexChanged += new System.EventHandler(this.baseComboBox_SelectedIndexChanged);
            // 
            // baseLabel
            // 
            resources.ApplyResources(this.baseLabel, "baseLabel");
            this.baseLabel.Name = "baseLabel";
            // 
            // descTextBox
            // 
            resources.ApplyResources(this.descTextBox, "descTextBox");
            this.descTextBox.BackColor = System.Drawing.Color.FromArgb(((int)(((byte)(65)))), ((int)(((byte)(65)))), ((int)(((byte)(65)))));
            this.descTextBox.BorderStyle = System.Windows.Forms.BorderStyle.FixedSingle;
            this.descTextBox.ForeColor = System.Drawing.Color.LightGray;
            this.descTextBox.Name = "descTextBox";
            this.descTextBox.TextChanged += new System.EventHandler(this.descTextBox_TextChanged);
            // 
            // descLabel
            // 
            resources.ApplyResources(this.descLabel, "descLabel");
            this.descLabel.Name = "descLabel";
            // 
            // dispTextBox
            // 
            resources.ApplyResources(this.dispTextBox, "dispTextBox");
            this.dispTextBox.BackColor = System.Drawing.Color.FromArgb(((int)(((byte)(65)))), ((int)(((byte)(65)))), ((int)(((byte)(65)))));
            this.dispTextBox.BorderStyle = System.Windows.Forms.BorderStyle.FixedSingle;
            this.dispTextBox.ForeColor = System.Drawing.Color.LightGray;
            this.dispTextBox.Name = "dispTextBox";
            this.dispTextBox.TextChanged += new System.EventHandler(this.dispTextBox_TextChanged);
            // 
            // dispLabel
            // 
            resources.ApplyResources(this.dispLabel, "dispLabel");
            this.dispLabel.Name = "dispLabel";
            // 
            // nameTextBox
            // 
            resources.ApplyResources(this.nameTextBox, "nameTextBox");
            this.nameTextBox.BackColor = System.Drawing.Color.FromArgb(((int)(((byte)(65)))), ((int)(((byte)(65)))), ((int)(((byte)(65)))));
            this.nameTextBox.BorderStyle = System.Windows.Forms.BorderStyle.FixedSingle;
            this.nameTextBox.ForeColor = System.Drawing.Color.LightGray;
            this.nameTextBox.Name = "nameTextBox";
            this.nameTextBox.TextChanged += new System.EventHandler(this.nameTextBox_TextChanged);
            // 
            // agentLabel
            // 
            resources.ApplyResources(this.agentLabel, "agentLabel");
            this.agentLabel.Name = "agentLabel";
            // 
            // MetaTypePanel
            // 
            resources.ApplyResources(this, "$this");
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.BackColor = System.Drawing.Color.FromArgb(((int)(((byte)(56)))), ((int)(((byte)(56)))), ((int)(((byte)(56)))));
            this.Controls.Add(this.descGroupBox);
            this.ForeColor = System.Drawing.Color.LightGray;
            this.Name = "MetaTypePanel";
            this.descGroupBox.ResumeLayout(false);
            this.descGroupBox.PerformLayout();
            this.ResumeLayout(false);

        }

        #endregion

        private System.Windows.Forms.ComboBox typeComboBox;
        private System.Windows.Forms.Label typeLabel;
        private System.Windows.Forms.GroupBox descGroupBox;
        private System.Windows.Forms.ComboBox baseComboBox;
        private System.Windows.Forms.Label baseLabel;
        private System.Windows.Forms.TextBox descTextBox;
        private System.Windows.Forms.Label descLabel;
        private System.Windows.Forms.TextBox dispTextBox;
        private System.Windows.Forms.Label dispLabel;
        private System.Windows.Forms.TextBox nameTextBox;
        private System.Windows.Forms.Label agentLabel;


    }
}
