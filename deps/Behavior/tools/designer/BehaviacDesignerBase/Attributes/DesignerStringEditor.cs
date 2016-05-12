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
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Text;
using System.Windows.Forms;

namespace Behaviac.Design.Attributes
{
    public partial class DesignerStringEditor : Behaviac.Design.Attributes.DesignerPropertyEditor
    {
        public DesignerStringEditor() {
            InitializeComponent();
        }

        public override void ReadOnly() {
            base.ReadOnly();

            textBox.ReadOnly = true;
        }

        public override void SetProperty(DesignerPropertyInfo property, object obj) {
            base.SetProperty(property, obj);

            object v = property.Property.GetValue(obj, null);

            if (v != null) {
                if (Plugin.IsCharType(v.GetType())) {
                    textBox.MaxLength = 1;
                }

                textBox.Text = trimQuotes(v.ToString());

            } else {
                Debug.Check(false);
            }
        }

        private string trimQuotes(string text) {
            char[] toTrim = {'"'};
            return text.Trim(toTrim);
        }

        public override void SetArrayProperty(DesignerArrayPropertyInfo arrayProperty, object obj) {
            base.SetArrayProperty(arrayProperty, obj);

            textBox.Text = (arrayProperty.Value != null) ? trimQuotes(arrayProperty.Value.ToString()) : string.Empty;

            if (Plugin.IsCharType(arrayProperty.Value.GetType())) {
                textBox.MaxLength = 1;
            }
        }

        public override void SetParameter(MethodDef.Param param, object obj, bool bReadonly) {
            base.SetParameter(param, obj, bReadonly);

            if (Plugin.IsCharType(param.Value.GetType())) {
                textBox.MaxLength = 1;
            }

            textBox.Text = trimQuotes(param.Value.ToString());
        }

        public override void SetVariable(VariableDef variable, object obj) {
            base.SetVariable(variable, obj);

            if (variable != null) {
                string str = trimQuotes(variable.Value.ToString());

                if (textBox.Text != str) {
                    textBox.Text = str;

                    _modified = true;
                    valueChanged();
                }

                if (Plugin.IsCharType(variable.ValueType))
                {
                    textBox.MaxLength = 1;
                }
            }
        }

        private void valueChanged() {
            if (!_valueWasAssigned)
            { return; }

            if (_property.Property != null) {
                if (Plugin.IsCharType(_property.Property.PropertyType)) {
                    char c = GetChar(textBox.Text);

                    _property.Property.SetValue(_object, c, null);

                } else {
                    _property.Property.SetValue(_object, textBox.Text, null);
                }

            } else if (_arrayProperty != null) {
                if (Plugin.IsCharType(_arrayProperty.ItemType)) {
                    char c = GetChar(textBox.Text);

                    _arrayProperty.Value = c;

                } else {
                    _arrayProperty.Value = textBox.Text;
                }

            } else if (_param != null) {
                Debug.Check(_param.Attribute is DesignerString);

                if (Plugin.IsCharType(_param.Value.GetType())) {
                    char c = GetChar(textBox.Text);

                    _param.Value = c;

                } else {
                    _param.Value = textBox.Text;
                }

            } else if (_variable != null) {
                if (Plugin.IsCharType(_variable.ValueType))
                {
                    char c = GetChar(textBox.Text);

                    _variable.Value = c;

                } else {
                    _variable.Value = textBox.Text;
                }

            } else {
                Debug.Check(false);
            }

            if (_modified) {
                OnValueChanged(_property);

                _modified = false;
            }
        }

        private char GetChar(string t) {
            char c = 'A';

            if (t.Length >= 1) {
                c = textBox.Text[0];
            }

            return c;
        }

        private void textBox_LostFocus(object sender, EventArgs e) {
            valueChanged();
        }

        private void textBox_KeyDown(object sender, KeyEventArgs e) {
            if (e.KeyCode == Keys.Enter) {
                valueChanged();
            }
        }

        private bool _modified = false;
        private void textBox_TextChanged(object sender, EventArgs e) {
            //valueChanged();
            if (!_valueWasAssigned)
            { return; }

            _modified = true;
        }

        private void textBox_MouseEnter(object sender, EventArgs e) {
            this.OnMouseEnter(e);
        }
    }
}
