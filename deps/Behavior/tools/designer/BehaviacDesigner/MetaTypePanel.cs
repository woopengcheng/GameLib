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
using System.Drawing;
using System.Data;
using System.Text;
using System.Windows.Forms;
using System.Reflection;
using Behaviac.Design.Attributes;
using Behaviac.Design.Data;
using Behaviac.Design.Network;
using Behaviac.Design.Nodes;
using Behaviac.Design.Properties;

namespace Behaviac.Design
{
    internal partial class MetaTypePanel : UserControl
    {
        public enum MetaTypes
        {
            Agent,
            Enum,
            Struct
        }

        private bool _initialized = false;

        public MetaTypePanel()
        {
            InitializeComponent();
        }

        public MetaTypes GetMetaType()
        {
            return (MetaTypes)this.typeComboBox.SelectedIndex;
        }

        private bool _isModified = false;
        public bool IsModified
        {
            get { return _isModified; }
            set { _isModified = true; }
        }

        private AgentType _customizedAgent = null;
        public AgentType GetCustomizedAgent()
        {
            if (this.GetMetaType() == MetaTypes.Agent)
            {
                if (this.baseComboBox.SelectedIndex > -1)
                {
                    Debug.Check(_customizedAgent != null);

                    AgentType baseAgent = Plugin.AgentTypes[this.baseComboBox.SelectedIndex];
                    _customizedAgent.Reset(this.nameTextBox.Text, baseAgent, this.dispTextBox.Text, this.descTextBox.Text);
                }
            }

            return _customizedAgent;
        }

        private CustomizedEnum _customizedEnum = null;
        public CustomizedEnum GetCustomizedEnum()
        {
            if (this.GetMetaType() == MetaTypes.Enum)
            {
                Debug.Check(_customizedEnum != null);

                _customizedEnum.Reset(this.nameTextBox.Text, this.dispTextBox.Text, this.descTextBox.Text);

            }

            return _customizedEnum;
        }

        private CustomizedStruct _customizedStruct = null;
        public CustomizedStruct GetCustomizedStruct()
        {
            if (this.GetMetaType() == MetaTypes.Struct)
            {
                Debug.Check(_customizedStruct != null);

                _customizedStruct.Reset(this.nameTextBox.Text, this.dispTextBox.Text, this.descTextBox.Text);
            }

            return _customizedStruct;
        }

        private bool _isNew = false;

        public void Initialize(object typeObject)
        {
            _initialized = false;
            _isModified = false;
            _isNew = (typeObject == null);
            this.Text = _isNew ? Resources.AddType : Resources.EditType;

            MetaTypes metaType = MetaTypes.Agent;

            if (typeObject != null)
            {
                if (typeObject is AgentType)
                {
                    metaType = MetaTypes.Agent;
                    _customizedAgent = typeObject as AgentType;

                }
                else if (typeObject is CustomizedEnum)
                {
                    metaType = MetaTypes.Enum;
                    _customizedEnum = typeObject as CustomizedEnum;

                }
                else if (typeObject is CustomizedStruct)
                {
                    metaType = MetaTypes.Struct;
                    _customizedStruct = typeObject as CustomizedStruct;
                }
            }

            this.typeComboBox.Items.Clear();
            this.typeComboBox.Enabled = _isNew;
            foreach (string type in Enum.GetNames(typeof(MetaTypes)))
            {
                this.typeComboBox.Items.Add(type);
            }
            this.typeComboBox.SelectedIndex = (int)metaType;

            if (this.GetMetaType() == MetaTypes.Agent)
            {
                Debug.Check(_customizedAgent != null);

                if (_customizedAgent.Base != null)
                {
                    this.baseComboBox.SelectedText = _customizedAgent.Base.AgentTypeName;
                }

                this.nameTextBox.Text = _customizedAgent.AgentTypeName;
                this.dispTextBox.Text = _customizedAgent.DisplayName;
                this.descTextBox.Text = _customizedAgent.Description;

                this.typeComboBox.Enabled = _customizedAgent.IsCustomized;
                this.nameTextBox.Enabled = _customizedAgent.IsCustomized;
                this.baseComboBox.Enabled = _customizedAgent.IsCustomized;
                this.dispTextBox.Enabled = _customizedAgent.IsCustomized;
                this.descTextBox.Enabled = _customizedAgent.IsCustomized;
            }
            else
            {
                if (this.GetMetaType() == MetaTypes.Enum)
                {
                    Debug.Check(_customizedEnum != null);

                    this.nameTextBox.Text = _customizedEnum.Name;
                    this.dispTextBox.Text = _customizedEnum.DisplayName;
                    this.descTextBox.Text = _customizedEnum.Description;
                }
                else if (this.GetMetaType() == MetaTypes.Struct)
                {
                    Debug.Check(_customizedStruct != null);

                    this.nameTextBox.Text = _customizedStruct.Name;
                    this.dispTextBox.Text = _customizedStruct.DisplayName;
                    this.descTextBox.Text = _customizedStruct.Description;
                }

                this.typeComboBox.Enabled = true;
                this.nameTextBox.Enabled = true;
                this.baseComboBox.Enabled = true;
                this.dispTextBox.Enabled = true;
                this.descTextBox.Enabled = true;
            }

            this.nameTextBox.Focus();
            this.nameTextBox.SelectionStart = this.nameTextBox.TextLength;

            _initialized = true;
        }

        private void resetBaseTypes()
        {
            this.baseComboBox.Items.Clear();
            this.baseComboBox.Visible = false;
            this.baseLabel.Visible = false;

            if (this.typeComboBox.SelectedIndex == (int)MetaTypes.Agent)
            {
                this.baseComboBox.Visible = true;
                this.baseLabel.Visible = true;

                int baseIndex = 0;

                for (int i = 0; i < Plugin.AgentTypes.Count; ++i)
                {
                    AgentType agentType = Plugin.AgentTypes[i];
                    this.baseComboBox.Items.Add(agentType.AgentTypeName);

                    if (this._customizedAgent != null && this._customizedAgent.Base != null && this._customizedAgent.Base.AgentTypeName == agentType.AgentTypeName)
                    {
                        baseIndex = i;
                    }
                }

                this.baseComboBox.SelectedIndex = baseIndex;
            }
        }

        public bool Verify()
        {
            if (string.IsNullOrEmpty(this.nameTextBox.Text) || this.nameTextBox.Text.Length < 1 || !char.IsLetter(this.nameTextBox.Text[0]))
                return false;

            foreach (AgentType agent in Plugin.AgentTypes)
            {
                if (agent.AgentTypeName == this.nameTextBox.Text)
                    return false;
            }

            foreach (CustomizedEnum customizedEnum in CustomizedTypeManager.Instance.Enums)
            {
                if (customizedEnum.Name == this.nameTextBox.Text)
                    return false;
            }

            foreach (CustomizedStruct customizedStruct in CustomizedTypeManager.Instance.Structs)
            {
                if (customizedStruct.Name == this.nameTextBox.Text)
                    return false;
            }

            return this.GetMetaType() != MetaTypes.Agent || this.baseComboBox.SelectedIndex > -1;
        }

        private void typeComboBox_SelectedIndexChanged(object sender, EventArgs e)
        {
            if (this.GetMetaType() == MetaTypes.Agent)
            {
                if (_customizedAgent == null)
                {
                    _customizedAgent = new AgentType(this.nameTextBox.Text, null, this.dispTextBox.Text, this.descTextBox.Text);
                }
            }
            else if (this.GetMetaType() == MetaTypes.Enum)
            {
                if (_customizedEnum == null)
                {
                    _customizedEnum = new CustomizedEnum(this.nameTextBox.Text, this.dispTextBox.Text, this.descTextBox.Text);
                }
            }
            else if (this.GetMetaType() == MetaTypes.Struct)
            {
                if (_customizedStruct == null)
                {
                    _customizedStruct = new CustomizedStruct(this.nameTextBox.Text, this.dispTextBox.Text, this.descTextBox.Text);
                }
            }

            resetBaseTypes();

            if (_initialized)
                this.IsModified = true;
        }

        private void nameTextBox_TextChanged(object sender, EventArgs e)
        {
            if (_initialized)
                this.IsModified = true;
        }

        private void baseComboBox_SelectedIndexChanged(object sender, EventArgs e)
        {
            if (_initialized)
                this.IsModified = true;
        }

        private void dispTextBox_TextChanged(object sender, EventArgs e)
        {
            if (_initialized)
                this.IsModified = true;
        }

        private void descTextBox_TextChanged(object sender, EventArgs e)
        {
            if (_initialized)
                this.IsModified = true;
        }
    }
}
