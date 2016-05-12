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
using System.Drawing;
using System.Data;
using System.Text;
using System.Windows.Forms;
using Behaviac.Design.Attributes;
using Behaviac.Design.Properties;

namespace Behaviac.Design
{
    internal partial class MetaPropertyPanel : UserControl
    {
        private bool _initialized = false;
        private bool _isNew = true;
        private bool _isArray = false;

        private AgentType _agent = null;
        CustomizedStruct _customizedStruct = null;
        private PropertyDef _originalProperty = null;
        private PropertyDef _property = null;

        DesignerPropertyEditor _valueEditor;

        public MetaPropertyPanel()
        {
            InitializeComponent();
        }

        public void Initialize(bool canBeEdit, AgentType agent, CustomizedStruct customizedStruct, PropertyDef prop, bool canBePar)
        {
            Debug.Check(agent != null || customizedStruct != null);

            _initialized = false;
            _isModified = false;
            _isNew = (prop == null);
            _agent = agent;
            _customizedStruct = customizedStruct;
            _originalProperty = prop;

            setTypes();

            if (_isNew)
            {
                this.Text = canBeEdit ? Resources.AddProperty : Resources.ViewProperty;

                if (customizedStruct == null)
                    _property = new PropertyDef(agent, null, agent.AgentTypeName, "", "", "");
                else
                    _property = new PropertyDef(null, null, customizedStruct.Name, "", "", "");
            }
            else
            {
                this.Text = canBeEdit ? Resources.EditProperty : Resources.ViewProperty;

                resetProperty(prop, prop.IsPar);
            }

            this.isStaticCheckBox.Visible = (agent != null);
            this.isPublicCheckBox.Visible = !canBeEdit && (agent != null);
            this.isConstcheckBox.Visible = (agent != null);
            this.customizedCheckBox.Visible = !canBeEdit && !_property.IsInherited;
            this.isLocalCheckBox.Checked = customizedStruct == null && _property.IsPar;
            this.isLocalCheckBox.Visible = canBePar && customizedStruct == null && !_property.IsMember;
            this.nameTextBox.Enabled = canBeEdit;
            this.arrayCheckBox.Enabled = canBeEdit || _property.IsChangeableType;
            this.typeComboBox.Enabled = canBeEdit || _property.IsChangeableType;
            this.isStaticCheckBox.Enabled = canBeEdit;
            this.isConstcheckBox.Enabled = canBeEdit;
            this.dispTextBox.Enabled = canBeEdit;
            this.descTextBox.Enabled = canBeEdit;

            _initialized = true;
        }

        public PropertyDef GetProperty()
        {
            Debug.Check(_property != null && !string.IsNullOrEmpty(_property.BasicName));

            if (string.IsNullOrEmpty(_property.DisplayName))
            {
                _property.DisplayName = _property.BasicName;
            }

            if (string.IsNullOrEmpty(_property.BasicDescription))
            {
                _property.BasicDescription = _property.BasicName;
            }

            return _property;
        }

        private bool _isModified = false;
        public bool IsModified
        {
            get { return _isModified; }
            set { _isModified = true; }
        }

        private void resetProperty(PropertyDef prop, bool isPar)
        {
            if (isPar)
                _property = new ParInfo(prop);
            else
                _property = new PropertyDef(prop);
            _property.OldName = prop.Name;

            _isArray = Plugin.IsArrayType(_property.Type);
            Type type = _isArray ? _property.Type.GetGenericArguments()[0] : _property.Type;

            this.nameTextBox.Text = _property.BasicName;
            this.arrayCheckBox.Checked = _isArray;
            this.typeComboBox.Text = Plugin.GetMemberValueTypeName(type);
            this.isStaticCheckBox.Checked = _property.IsStatic;
            this.isPublicCheckBox.Checked = _property.IsPublic;
            this.isConstcheckBox.Checked = _property.IsReadonly;
            this.customizedCheckBox.Checked = _property.IsExportedButAlsoCustomized;
            this.dispTextBox.Text = _property.DisplayName;
            this.descTextBox.Text = _property.BasicDescription;

            resetType(type, false);
        }

        private void createValueEditor(Type type)
        {
            Type editorType = Plugin.InvokeEditorType(type);
            Debug.Check(editorType != null);

            ParInfo par = this._property as ParInfo;

            if (_property.Variable == null || _property.Variable.ValueType != type)
            {
                _property.Variable = new VariableDef(Plugin.DefaultValue(type));
            }

            _valueEditor = (DesignerPropertyEditor)editorType.InvokeMember(string.Empty, System.Reflection.BindingFlags.CreateInstance, null, null, new object[0]);
            _valueEditor.Margin = new System.Windows.Forms.Padding(0);
            _valueEditor.Anchor = AnchorStyles.Top | AnchorStyles.Left | AnchorStyles.Right;
            _valueEditor.Width = this.dispTextBox.Width;
            _valueEditor.Location = this.dispTextBox.Location;
            _valueEditor.Location = new Point(_valueEditor.Location.X, _valueEditor.Location.Y - _valueEditor.Height - 6);
            _valueEditor.ValueWasChanged += new DesignerPropertyEditor.ValueChanged(editor_ValueWasChanged);

            if (par != null)
            {
                _valueEditor.SetPar(par, null);
            }
            else
            {
                _valueEditor.SetVariable(this._property.Variable, null);
            }

            _valueEditor.ValueWasAssigned();

            this.Controls.Add(_valueEditor);
            _valueEditor.BringToFront();
        }

        private void editor_ValueWasChanged(object sender, DesignerPropertyInfo property)
        {
            if (_initialized)
            {
                DesignerPropertyEditor editor = sender as DesignerPropertyEditor;

                if (_property.IsCustomized || _property.IsPar)
                {
                    _property.Variable = editor.GetVariable();

                    this.IsModified = true;
                }
            }
        }

        public bool Verify()
        {
            Debug.Check(_property != null);

            string propertyName = this.nameTextBox.Text;

            bool isValid = !string.IsNullOrEmpty(propertyName) && propertyName.Length >= 1 &&
                char.IsLetter(propertyName[0]) && (_property.Type != null);

            if (isValid && _agent != null)
            {
                PropertyDef prop = _agent.GetPropertyByName(propertyName);

                if (_isNew)
                {
                    isValid = (prop == null);
                }
                else
                {
                    isValid = (prop == null || prop == _originalProperty);
                }
            }

            return isValid;
        }

        private void setTypes()
        {
            this.typeComboBox.Items.Clear();
            foreach (string typeName in Plugin.GetAllMemberValueTypeNames(false, true))
            {
                this.typeComboBox.Items.Add(typeName);
            }
        }

        private void resetType(Type type, bool reset)
        {
            Debug.Check(_property != null);

            if (type == null)
            {
                if (this.typeComboBox.SelectedItem != null)
                {
                    type = Plugin.GetMemberValueType(this.typeComboBox.SelectedItem.ToString());
                    Debug.Check(type != null);

                    if (_isArray)
                    {
                        type = typeof(List<>).MakeGenericType(type);
                    }
                }
            }
            else
            {
                if (_isArray)
                {
                    type = typeof(List<>).MakeGenericType(type);
                }
            }

            if (type != null)
            {
                if (reset)
                {
                    _property.Type = type;
                    _property.NativeType = Plugin.GetMemberValueTypeName(type);
                }

                if (_valueEditor != null)
                {
                    this.Controls.Remove(_valueEditor);
                    _valueEditor = null;
                }

                if ((_agent != null) && ((_property.IsPar && _property is ParInfo) || _property.IsCustomized))
                {
                    createValueEditor(type);

                    this.valueLabel.Visible = true;
                }
                else
                {
                    this.valueLabel.Visible = false;
                }
            }
        }

        private void nameTextBox_TextChanged(object sender, EventArgs e)
        {
            if (_initialized)
            {
                Debug.Check(_property != null);

                if (_property.Name != this.nameTextBox.Text)
                {
                    _property.Name = this.nameTextBox.Text;
                    _property.DisplayName = _property.Name;
                    this.dispTextBox.Text = _property.DisplayName;

                    this.IsModified = true;
                }
            }
        }

        private void arrayCheckBox_CheckedChanged(object sender, EventArgs e)
        {
            if (_initialized)
            {
                _isArray = this.arrayCheckBox.Checked;

                resetType(null, true);

                this.IsModified = true;
            }
        }

        private void typeComboBox_SelectedIndexChanged(object sender, EventArgs e)
        {
            if (_initialized)
            {
                Debug.Check(_property != null);

                resetType(null, true);

                this.IsModified = true;
            }
        }

        private void isStaticCheckBox_CheckedChanged(object sender, EventArgs e)
        {
            if (_initialized)
            {
                Debug.Check(_property != null);
                _property.IsStatic = this.isStaticCheckBox.Checked;

                this.IsModified = true;
            }
        }

        private void isPublicCheckBox_CheckedChanged(object sender, EventArgs e)
        {
            if (_initialized)
            {
                Debug.Check(_property != null);
                _property.IsPublic = this.isPublicCheckBox.Checked;

                this.IsModified = true;
            }
        }

        private void dispTextBox_TextChanged(object sender, EventArgs e)
        {
            if (_initialized && _property.DisplayName != this.dispTextBox.Text)
            {
                Debug.Check(_property != null);
                _property.DisplayName = this.dispTextBox.Text;

                this.IsModified = true;
            }
        }

        private void descTextBox_TextChanged(object sender, EventArgs e)
        {
            if (_initialized && _property.BasicDescription != this.descTextBox.Text)
            {
                Debug.Check(_property != null);
                _property.BasicDescription = this.descTextBox.Text;

                this.IsModified = true;
            }
        }

        private void isConstcheckBox_CheckedChanged(object sender, EventArgs e)
        {
            if (_initialized)
            {
                Debug.Check(_property != null);
                _property.IsReadonly = this.isConstcheckBox.Checked;

                this.IsModified = true;
            }
        }

        private void customizedCheckBox_CheckedChanged(object sender, EventArgs e)
        {
            if (_initialized)
            {
                Debug.Check(_property != null);

                if (!_property.IsCustomized)
                {
                    _property.IsExportedButAlsoCustomized = this.customizedCheckBox.Checked;

                    this.IsModified = true;
                }
            }
        }

        private void isLocalCheckBox_CheckedChanged(object sender, EventArgs e)
        {
            if (_initialized)
            {
                resetProperty(_property, this.isLocalCheckBox.Checked);

                this.IsModified = true;
            }
        }
    }
}
