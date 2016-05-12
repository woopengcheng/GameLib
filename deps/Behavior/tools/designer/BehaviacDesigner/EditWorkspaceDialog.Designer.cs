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

namespace Behaviac.Design
{
    partial class EditWorkspaceDialog
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
            System.ComponentModel.ComponentResourceManager resources = new System.ComponentModel.ComponentResourceManager(typeof(EditWorkspaceDialog));
            this.doneButton = new System.Windows.Forms.Button();
            this.cancelButton = new System.Windows.Forms.Button();
            this.nameLabel = new System.Windows.Forms.Label();
            this.nameTextBox = new System.Windows.Forms.TextBox();
            this.pluginListBox = new System.Windows.Forms.CheckedListBox();
            this.XMLButton = new System.Windows.Forms.Button();
            this.XMLTextBox = new System.Windows.Forms.TextBox();
            this.metaLabel = new System.Windows.Forms.Label();
            this.workspaceButton = new System.Windows.Forms.Button();
            this.workspaceTextBox = new System.Windows.Forms.TextBox();
            this.locationLabel = new System.Windows.Forms.Label();
            this.sourceTextBox = new System.Windows.Forms.TextBox();
            this.label1 = new System.Windows.Forms.Label();
            this.sourceButton = new System.Windows.Forms.Button();
            this.exportTextBox = new System.Windows.Forms.TextBox();
            this.label2 = new System.Windows.Forms.Label();
            this.exportButton = new System.Windows.Forms.Button();
            this.folderBrowserDialog = new Ookii.Dialogs.VistaFolderBrowserDialog();
            this.SuspendLayout();
            // 
            // doneButton
            // 
            resources.ApplyResources(this.doneButton, "doneButton");
            this.doneButton.BackColor = System.Drawing.Color.FromArgb(((int)(((byte)(65)))), ((int)(((byte)(65)))), ((int)(((byte)(65)))));
            this.doneButton.FlatAppearance.MouseDownBackColor = System.Drawing.Color.DarkGray;
            this.doneButton.FlatAppearance.MouseOverBackColor = System.Drawing.Color.DarkGray;
            this.doneButton.Name = "doneButton";
            this.doneButton.UseVisualStyleBackColor = false;
            this.doneButton.Click += new System.EventHandler(this.doneButton_Click);
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
            this.cancelButton.Click += new System.EventHandler(this.cancelButton_Click);
            // 
            // nameLabel
            // 
            resources.ApplyResources(this.nameLabel, "nameLabel");
            this.nameLabel.Name = "nameLabel";
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
            // pluginListBox
            // 
            resources.ApplyResources(this.pluginListBox, "pluginListBox");
            this.pluginListBox.BackColor = System.Drawing.Color.FromArgb(((int)(((byte)(65)))), ((int)(((byte)(65)))), ((int)(((byte)(65)))));
            this.pluginListBox.BorderStyle = System.Windows.Forms.BorderStyle.FixedSingle;
            this.pluginListBox.CheckOnClick = true;
            this.pluginListBox.ForeColor = System.Drawing.Color.LightGray;
            this.pluginListBox.FormattingEnabled = true;
            this.pluginListBox.Name = "pluginListBox";
            // 
            // XMLButton
            // 
            resources.ApplyResources(this.XMLButton, "XMLButton");
            this.XMLButton.BackColor = System.Drawing.Color.FromArgb(((int)(((byte)(65)))), ((int)(((byte)(65)))), ((int)(((byte)(65)))));
            this.XMLButton.FlatAppearance.MouseDownBackColor = System.Drawing.Color.DarkGray;
            this.XMLButton.FlatAppearance.MouseOverBackColor = System.Drawing.Color.DarkGray;
            this.XMLButton.Name = "XMLButton";
            this.XMLButton.UseVisualStyleBackColor = false;
            this.XMLButton.Click += new System.EventHandler(this.importXMLButton_Click);
            // 
            // XMLTextBox
            // 
            resources.ApplyResources(this.XMLTextBox, "XMLTextBox");
            this.XMLTextBox.BackColor = System.Drawing.Color.FromArgb(((int)(((byte)(65)))), ((int)(((byte)(65)))), ((int)(((byte)(65)))));
            this.XMLTextBox.BorderStyle = System.Windows.Forms.BorderStyle.FixedSingle;
            this.XMLTextBox.ForeColor = System.Drawing.Color.LightGray;
            this.XMLTextBox.Name = "XMLTextBox";
            this.XMLTextBox.KeyDown += new System.Windows.Forms.KeyEventHandler(this.xmlFolderTextBox_KeyDown);
            this.XMLTextBox.Leave += new System.EventHandler(this.xmlFolderTextBox_Leave);
            // 
            // metaLabel
            // 
            resources.ApplyResources(this.metaLabel, "metaLabel");
            this.metaLabel.Name = "metaLabel";
            // 
            // workspaceButton
            // 
            resources.ApplyResources(this.workspaceButton, "workspaceButton");
            this.workspaceButton.BackColor = System.Drawing.Color.FromArgb(((int)(((byte)(65)))), ((int)(((byte)(65)))), ((int)(((byte)(65)))));
            this.workspaceButton.FlatAppearance.MouseDownBackColor = System.Drawing.Color.DarkGray;
            this.workspaceButton.FlatAppearance.MouseOverBackColor = System.Drawing.Color.DarkGray;
            this.workspaceButton.Name = "workspaceButton";
            this.workspaceButton.UseVisualStyleBackColor = false;
            this.workspaceButton.Click += new System.EventHandler(this.workspaceButton_Click);
            // 
            // workspaceTextBox
            // 
            resources.ApplyResources(this.workspaceTextBox, "workspaceTextBox");
            this.workspaceTextBox.BackColor = System.Drawing.Color.FromArgb(((int)(((byte)(65)))), ((int)(((byte)(65)))), ((int)(((byte)(65)))));
            this.workspaceTextBox.BorderStyle = System.Windows.Forms.BorderStyle.FixedSingle;
            this.workspaceTextBox.ForeColor = System.Drawing.Color.LightGray;
            this.workspaceTextBox.Name = "workspaceTextBox";
            this.workspaceTextBox.KeyDown += new System.Windows.Forms.KeyEventHandler(this.locationTextBox_KeyDown);
            this.workspaceTextBox.Leave += new System.EventHandler(this.locationTextBox_Leave);
            // 
            // locationLabel
            // 
            resources.ApplyResources(this.locationLabel, "locationLabel");
            this.locationLabel.Name = "locationLabel";
            // 
            // sourceTextBox
            // 
            resources.ApplyResources(this.sourceTextBox, "sourceTextBox");
            this.sourceTextBox.BackColor = System.Drawing.Color.FromArgb(((int)(((byte)(65)))), ((int)(((byte)(65)))), ((int)(((byte)(65)))));
            this.sourceTextBox.BorderStyle = System.Windows.Forms.BorderStyle.FixedSingle;
            this.sourceTextBox.ForeColor = System.Drawing.Color.LightGray;
            this.sourceTextBox.Name = "sourceTextBox";
            this.sourceTextBox.KeyDown += new System.Windows.Forms.KeyEventHandler(this.sourceTextBox_KeyDown);
            this.sourceTextBox.Leave += new System.EventHandler(this.sourceTextBox_Leave);
            // 
            // label1
            // 
            resources.ApplyResources(this.label1, "label1");
            this.label1.Name = "label1";
            // 
            // sourceButton
            // 
            resources.ApplyResources(this.sourceButton, "sourceButton");
            this.sourceButton.BackColor = System.Drawing.Color.FromArgb(((int)(((byte)(65)))), ((int)(((byte)(65)))), ((int)(((byte)(65)))));
            this.sourceButton.FlatAppearance.MouseDownBackColor = System.Drawing.Color.DarkGray;
            this.sourceButton.FlatAppearance.MouseOverBackColor = System.Drawing.Color.DarkGray;
            this.sourceButton.Name = "sourceButton";
            this.sourceButton.UseVisualStyleBackColor = false;
            this.sourceButton.Click += new System.EventHandler(this.buttonSource_Click);
            // 
            // exportTextBox
            // 
            resources.ApplyResources(this.exportTextBox, "exportTextBox");
            this.exportTextBox.BackColor = System.Drawing.Color.FromArgb(((int)(((byte)(65)))), ((int)(((byte)(65)))), ((int)(((byte)(65)))));
            this.exportTextBox.BorderStyle = System.Windows.Forms.BorderStyle.FixedSingle;
            this.exportTextBox.ForeColor = System.Drawing.Color.LightGray;
            this.exportTextBox.Name = "exportTextBox";
            this.exportTextBox.KeyDown += new System.Windows.Forms.KeyEventHandler(this.exportTextBox_KeyDown);
            this.exportTextBox.Leave += new System.EventHandler(this.exportTextBox_Leave);
            // 
            // label2
            // 
            resources.ApplyResources(this.label2, "label2");
            this.label2.Name = "label2";
            // 
            // exportButton
            // 
            resources.ApplyResources(this.exportButton, "exportButton");
            this.exportButton.BackColor = System.Drawing.Color.FromArgb(((int)(((byte)(65)))), ((int)(((byte)(65)))), ((int)(((byte)(65)))));
            this.exportButton.FlatAppearance.MouseDownBackColor = System.Drawing.Color.DarkGray;
            this.exportButton.FlatAppearance.MouseOverBackColor = System.Drawing.Color.DarkGray;
            this.exportButton.Name = "exportButton";
            this.exportButton.UseVisualStyleBackColor = false;
            this.exportButton.Click += new System.EventHandler(this.buttonExport_Click);
            // 
            // folderBrowserDialog
            // 
            resources.ApplyResources(this.folderBrowserDialog, "folderBrowserDialog");
            // 
            // EditWorkspaceDialog
            // 
            this.AcceptButton = this.doneButton;
            resources.ApplyResources(this, "$this");
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.BackColor = System.Drawing.Color.FromArgb(((int)(((byte)(56)))), ((int)(((byte)(56)))), ((int)(((byte)(56)))));
            this.CancelButton = this.cancelButton;
            this.Controls.Add(this.exportTextBox);
            this.Controls.Add(this.label2);
            this.Controls.Add(this.exportButton);
            this.Controls.Add(this.sourceTextBox);
            this.Controls.Add(this.label1);
            this.Controls.Add(this.sourceButton);
            this.Controls.Add(this.workspaceTextBox);
            this.Controls.Add(this.locationLabel);
            this.Controls.Add(this.workspaceButton);
            this.Controls.Add(this.metaLabel);
            this.Controls.Add(this.XMLTextBox);
            this.Controls.Add(this.XMLButton);
            this.Controls.Add(this.pluginListBox);
            this.Controls.Add(this.nameTextBox);
            this.Controls.Add(this.nameLabel);
            this.Controls.Add(this.cancelButton);
            this.Controls.Add(this.doneButton);
            this.ForeColor = System.Drawing.Color.LightGray;
            this.MaximizeBox = false;
            this.MinimizeBox = false;
            this.Name = "EditWorkspaceDialog";
            this.ShowInTaskbar = false;
            this.ResumeLayout(false);
            this.PerformLayout();

        }

        #endregion

        private System.Windows.Forms.Button doneButton;
        private System.Windows.Forms.Button cancelButton;
        private System.Windows.Forms.Label nameLabel;
        private System.Windows.Forms.TextBox nameTextBox;
        private System.Windows.Forms.CheckedListBox pluginListBox;
        private System.Windows.Forms.Button XMLButton;
        private System.Windows.Forms.TextBox XMLTextBox;
        private System.Windows.Forms.Label metaLabel;
        private System.Windows.Forms.Button workspaceButton;
        private System.Windows.Forms.TextBox workspaceTextBox;
        private System.Windows.Forms.Label locationLabel;
        private System.Windows.Forms.TextBox sourceTextBox;
        private System.Windows.Forms.Label label1;
        private System.Windows.Forms.Button sourceButton;
        private System.Windows.Forms.TextBox exportTextBox;
        private System.Windows.Forms.Label label2;
        private System.Windows.Forms.Button exportButton;
        private Ookii.Dialogs.VistaFolderBrowserDialog folderBrowserDialog;
    }
}