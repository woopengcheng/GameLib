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
using System.Windows.Forms.Design;
using Behaviac.Design.Data;
using Behaviac.Design.Properties;
using Behaviac.Design.Attributes;

namespace Behaviac.Design
{
    internal partial class MetaStoreDock : Form
    {
        public class MemberItem
        {
            public string DisplayName = "";

            public PropertyDef Property = null;
            public MethodDef Method = null;

            public MemberItem(string displayName, PropertyDef property) {
                DisplayName = displayName;
                Property = property;
            }

            public MemberItem(string displayName, MethodDef method) {
                DisplayName = displayName;
                Method = method;
            }
        }

        private const string Par_Str = "-  ";
        private const string Customized_Str = "*  ";
        private const string Member_Str = "   ";
        private const string Empty_Type_Str = "?  ";
        private const string Changeable_Type_Str = "$  ";

        private static MetaStoreDock _metaStoreDock = null;
        private static AgentType _lastAgent = null;
        private static string _lastSelectedType = "";
        private static int _lastMemberTypeIndex = (int)MemberType.Property;
        private static Nodes.BehaviorNode _isParDirtyBehavior = null;

        private int _previousSelectedTypeIndex = -1;
        private int _previousSelectedMemberIndex = -1;

        internal static void Inspect(BehaviorTreeViewDock dock) {
            if (_metaStoreDock == null) {
                _metaStoreDock = new MetaStoreDock();
                _metaStoreDock.Owner = MainWindow.Instance;
            }

            _metaStoreDock.initialize(dock);
            _metaStoreDock.Show();
        }

        internal static bool IsVisible() {
            return _metaStoreDock != null && _metaStoreDock.Visible;
        }

        internal static void CheckSave() {
            if (_metaStoreDock != null && Workspace.Current.IsBlackboardDirty &&
                DialogResult.Yes == MessageBox.Show(Resources.MetaSaveInfo, Resources.Warning, MessageBoxButtons.YesNo)) {
                _metaStoreDock.save();
            }
        }

        internal static void Reset()
        {
            if (_metaStoreDock != null)
            {
                _metaStoreDock.Clear();
            }
        }

        private BehaviorTreeViewDock _dock = null;

        public MetaStoreDock() {
            InitializeComponent();

            _metaStoreDock = this;

            _previousSelectedTypeIndex = -1;
            _previousSelectedMemberIndex = -1;

            Plugin.UpdateMetaStoreHandler += Plugin_UpdateMetaStoreHandler;
            Plugin.PostSetWorkspaceHandler += Plugin_PostSetWorkspaceHandler;
        }

        private void MetaStoreDock_FormClosing(object sender, FormClosingEventArgs e) {
            //this.Hide();

            //e.Cancel = true;

            _metaStoreDock = null;

            Plugin.UpdateMetaStoreHandler -= Plugin_UpdateMetaStoreHandler;
            Plugin.PostSetWorkspaceHandler -= Plugin_PostSetWorkspaceHandler;

            _lastSelectedType = "";
        }

        private void Plugin_UpdateMetaStoreHandler(object dock)
        {
            if (MetaStoreDock.IsVisible())
                MetaStoreDock.Inspect(dock as BehaviorTreeViewDock);
        }

        private void Plugin_PostSetWorkspaceHandler()
        {
            if (MetaStoreDock.IsVisible())
                MetaStoreDock.Reset();
        }

        private void Clear()
        {
            _lastSelectedType = "";
        }

        private void apply(bool query, int typeSelectedIndex, int memberSelectedIndex, bool resetSelectedIndex)
        {
            if (this.metaTypePanel.IsModified)
            {
                if (this.metaTypePanel.Verify())
                {
                    this.metaTypePanel.IsModified = false;
                }
                else
                {
                    if (query)
                        MessageBox.Show(Resources.TypeVerifyWarning, Resources.Warning, MessageBoxButtons.OK);

                    return;
                }

                editType(typeSelectedIndex);
            }

            if (memberSelectedIndex < 0)
                return;

            if (this._metaPropertyPanel != null && this._metaPropertyPanel.IsModified)
            {
                if (this._metaPropertyPanel.Verify())
                {
                    this._metaPropertyPanel.IsModified = false;
                }
                else
                {
                    if (query)
                        MessageBox.Show(Resources.PropertyVerifyWarning, Resources.Warning, MessageBoxButtons.OK);

                    return;
                }
            }
            else if (this._metaMethodPanel != null && this._metaMethodPanel.IsModified)
            {
                if (this._metaMethodPanel.Verify())
                {
                    this._metaMethodPanel.IsModified = false;
                }
                else
                {
                    if (query)
                        MessageBox.Show(Resources.MethodVerifyWarning, Resources.Warning, MessageBoxButtons.OK);

                    return;
                }
            }
            else if (this._editEnumMemberPanel != null && this._editEnumMemberPanel.IsModified)
            {
                if (this._editEnumMemberPanel.Verify())
                {
                    this._editEnumMemberPanel.IsModified = false;
                }
                else
                {
                    if (query)
                        MessageBox.Show(Resources.EnumMemberVerifyWarning, Resources.Warning, MessageBoxButtons.OK);

                    return;
                }
            }
            else
            {
                return;
            }

            editMember(memberSelectedIndex, resetSelectedIndex);
        }

        private void save(bool refreshWorkspace = false)
        {
            if (!refreshWorkspace)
            {
                apply(true, this.typeListBox.SelectedIndex, this.memberListBox.SelectedIndex, true);
            }

            bool isBlackboardDirty = Workspace.Current.IsBlackboardDirty;

            Workspace.SaveExtraMeta(Workspace.Current);

            Workspace.SaveCustomMeta(Workspace.Current);

            if (_isParDirtyBehavior != null) {
                Nodes.BehaviorNode root = getCurrentRootNode();

                if (root != null && _isParDirtyBehavior == root && root.IsModified) {
                    BehaviorTreeViewDock dock = getCurrentBehaviorTreeDock();

                    if (dock.BehaviorTreeView.Save(false)) {
                        _isParDirtyBehavior = null;
                    }
                }
            }

            if (refreshWorkspace && isBlackboardDirty)
            {
                // refresh the meta
                Debug.Check(Workspace.Current != null);
                MainWindow.Instance.SetWorkspace(Workspace.Current.FileName, false, true);
            }

            PropertiesDock.InspectObject(null, null);
        }

        private void applyButton_Click(object sender, EventArgs e)
        {
            save();
        }

        private void okButton_Click(object sender, EventArgs e)
        {
            save();

            this.Close();
        }

        private void closeButton_Click(object sender, EventArgs e)
        {
            CheckSave();

            this.Close();
        }

        private bool _initialized = false;

        private void initialize(BehaviorTreeViewDock dock) {
            _initialized = false;
            _dock = dock;

            this.Text = Resources.MetaStore;

            if (getCurrentBehaviorTreeDock() != null) {
                this.Text += " - " + getCurrentBehaviorTreeDock().Text;
            }

            this.typeListBox.Items.Clear();
            this.instanceComboBox.Items.Clear();
            this.memberTypeComboBox.Items.Clear();

            string selectedType = _lastSelectedType;

            if (string.IsNullOrEmpty(_lastSelectedType))
            {
                AgentType agentType = getCurrentViewAgentType();

                if (agentType == null)
                {
                    agentType = _lastAgent;
                }

                if (agentType != null)
                {
                    selectedType = agentType.AgentTypeName;
                }
            }

            resetAllTypes(selectedType);

            // set all member types
            foreach(string mt in Enum.GetNames(typeof(MemberType))) {
                this.memberTypeComboBox.Items.Add(mt);
            }

            this.memberTypeComboBox.SelectedIndex = _lastMemberTypeIndex;

            _initialized = true;
        }

        private void resetAllTypes(string selectedType) {
            this.typeListBox.Items.Clear();

            int index = 0;

            for (int i = 0; i < Plugin.AgentTypes.Count; ++i) {
                AgentType agent = Plugin.AgentTypes[i];
                string agentName = (agent.IsCustomized ? Customized_Str : Member_Str) + agent.AgentTypeName;
                this.typeListBox.Items.Add(agentName);

                if (agent.AgentTypeName == selectedType) {
                    index = i;
                }
            }

            for (int i = 0; i < CustomizedTypeManager.Instance.Enums.Count; ++i) {
                CustomizedEnum customizedEnum = CustomizedTypeManager.Instance.Enums[i];
                this.typeListBox.Items.Add(Customized_Str + customizedEnum.Name);

                if (customizedEnum.Name == selectedType) {
                    index = i + Plugin.AgentTypes.Count;
                }
            }

            for (int i = 0; i < CustomizedTypeManager.Instance.Structs.Count; ++i) {
                CustomizedStruct customizedStruct = CustomizedTypeManager.Instance.Structs[i];
                this.typeListBox.Items.Add(Customized_Str + customizedStruct.Name);

                if (customizedStruct.Name == selectedType) {
                    index = i + Plugin.AgentTypes.Count + CustomizedTypeManager.Instance.Enums.Count;
                }
            }

            if (this.typeListBox.Items.Count > 0) {
                this.typeListBox.SelectedIndex = index;
            }
        }

        private BehaviorTreeViewDock getCurrentBehaviorTreeDock() {
            return (_initialized || _dock == null) ? BehaviorTreeViewDock.LastFocused : _dock;
        }

        private Nodes.BehaviorNode getCurrentRootNode() {
            BehaviorTreeViewDock dock = getCurrentBehaviorTreeDock();
            BehaviorTreeView focusedView = (dock != null) ? dock.BehaviorTreeView : null;
            return (focusedView != null) ? focusedView.RootNode : null;
        }

        private AgentType getCurrentViewAgentType() {
            BehaviorTreeViewDock dock = getCurrentBehaviorTreeDock();
            BehaviorTreeView focusedView = (dock != null) ? dock.BehaviorTreeView : null;
            return (focusedView != null && focusedView.RootNode != null) ? focusedView.RootNode.AgentType : null;
        }

        private string getPrefixString(PropertyDef p)
        {
            if (p != null)
            {
                if (p.IsChangeableType)
                    return (p.Type == typeof(object)) ? Empty_Type_Str : Changeable_Type_Str;

                if (p.IsPar)
                    return Par_Str;

                if (!p.IsMember)
                    return Customized_Str;
            }

            return Member_Str;
        }

        private string getPrefixString(MethodDef m)
        {
            if (m != null)
            {
                if (m.IsChangeableType)
                    return (m.ReturnType == typeof(object)) ? Empty_Type_Str : Changeable_Type_Str;

                if (m.IsCustomized)
                    return Customized_Str;
            }

            return Member_Str;
        }

        private void setMembers() {
            this.addMemberButton.Enabled = false;
            this.removeMemberButton.Enabled = false;
            this.upMemberButton.Enabled = false;
            this.downMemberButton.Enabled = false;

            this.memberListBox.Items.Clear();

            int index = this.typeListBox.SelectedIndex;

            if (index > -1 && this.memberTypeComboBox.SelectedIndex > -1) {
                string filter = !string.IsNullOrEmpty(memberFilterTextBox.Text) ? memberFilterTextBox.Text.ToLowerInvariant() : string.Empty;

                // agent
                if (index < Plugin.AgentTypes.Count) {
                    this.addMemberButton.Enabled = true;

                    AgentType agentType = Plugin.AgentTypes[index];

                    if (agentType != null) {
                        if (this.memberTypeComboBox.SelectedIndex == (int)MemberType.Property) {
                            Nodes.BehaviorNode root = this.getCurrentRootNode();
                            if (root == null || root.AgentType != agentType)
                            {
                                agentType.ClearPars();
                            }

                            IList<PropertyDef> properties = agentType.GetProperties();

                            foreach(PropertyDef p in properties) {
                                if (p.IsArrayElement)
                                    continue;

                                string propName = p.DisplayName.ToLowerInvariant();

                                if (memberFilterCheckBox.Checked && propName.StartsWith(filter) ||
                                    !memberFilterCheckBox.Checked && propName.Contains(filter))
                                {
                                    string disp = getPrefixString(p) + p.DisplayName;
                                    this.memberListBox.Items.Add(new MemberItem(disp, p));
                                }
                            }

                        } else if (this.memberTypeComboBox.SelectedIndex == (int)MemberType.Method ||
                                   this.memberTypeComboBox.SelectedIndex == (int)MemberType.Task) {
                            MethodType methodType = MethodType.Method;

                            if (this.memberTypeComboBox.SelectedIndex == (int)MemberType.Task) {
                                methodType = MethodType.Task;
                            }

                            IList<MethodDef> methods = agentType.GetMethods(methodType);
                            foreach(MethodDef m in methods) {
                                string methodName = m.DisplayName.ToLowerInvariant();

                                if (memberFilterCheckBox.Checked && methodName.StartsWith(filter) ||
                                    !memberFilterCheckBox.Checked && methodName.Contains(filter))
                                {
                                    string disp = getPrefixString(m) + m.DisplayName;
                                    this.memberListBox.Items.Add(new MemberItem(disp, m));
                                }
                            }
                        }
                    }

                } else {
                    if (this.memberTypeComboBox.SelectedIndex == (int)MemberType.Property) {
                        this.addMemberButton.Enabled = true;

                        // enum
                        if (index - Plugin.AgentTypes.Count < CustomizedTypeManager.Instance.Enums.Count) {
                            CustomizedEnum customizedEnum = CustomizedTypeManager.Instance.Enums[index - Plugin.AgentTypes.Count];

                            foreach(CustomizedEnum.CustomizedEnumMember member in customizedEnum.Members) {
                                string memberName = member.DisplayName.ToLowerInvariant();

                                if (memberFilterCheckBox.Checked && memberName.StartsWith(filter) ||
                                    !memberFilterCheckBox.Checked && memberName.Contains(filter)) {
                                    string disp = Customized_Str + member.DisplayName;
                                    this.memberListBox.Items.Add(disp);
                                }
                            }
                        }

                        // struct
                        else {
                            CustomizedStruct customizedStruct = CustomizedTypeManager.Instance.Structs[index - Plugin.AgentTypes.Count - CustomizedTypeManager.Instance.Enums.Count];

                            foreach(PropertyDef member in customizedStruct.Properties) {
                                string memberName = member.DisplayName.ToLowerInvariant();

                                if (memberFilterCheckBox.Checked && memberName.StartsWith(filter) ||
                                    !memberFilterCheckBox.Checked && memberName.Contains(filter)) {
                                    string disp = Customized_Str + member.DisplayName;
                                    this.memberListBox.Items.Add(disp);
                                }
                            }
                        }
                    }
                }
            }

            this.memberCountLabel.Text = this.memberListBox.Items.Count.ToString();
        }

        private MetaPropertyPanel _metaPropertyPanel = null;
        private MetaMethodPanel _metaMethodPanel = null;
        private EditEnumMemberPanel _editEnumMemberPanel = null;

        private void initMetaPanel()
        {
            if (this.memberTypeComboBox.SelectedIndex == (int)MemberType.Property)
            {
                if (this._metaMethodPanel != null)
                {
                    this.Controls.Remove(this._metaMethodPanel);
                    this._metaMethodPanel = null;
                }

                int index = this.typeListBox.SelectedIndex;

                if (index >= Plugin.AgentTypes.Count && index - Plugin.AgentTypes.Count < CustomizedTypeManager.Instance.Enums.Count) // enum
                {
                    if (this._metaPropertyPanel != null)
                    {
                        this.Controls.Remove(this._metaPropertyPanel);
                        this._metaPropertyPanel = null;
                    }

                    if (this._editEnumMemberPanel == null)
                    {
                        this._editEnumMemberPanel = new EditEnumMemberPanel();
                        this._editEnumMemberPanel.Hide();
                        this.Controls.Add(this._editEnumMemberPanel);

                        setMetaPanelLocation();
                    }
                }
                else // agent or struct
                {
                    if (this._editEnumMemberPanel != null)
                    {
                        this.Controls.Remove(this._editEnumMemberPanel);
                        this._editEnumMemberPanel = null;
                    }

                    if (this._metaPropertyPanel == null)
                    {
                        this._metaPropertyPanel = new MetaPropertyPanel();
                        this._metaPropertyPanel.Hide();
                        this.Controls.Add(this._metaPropertyPanel);

                        setMetaPanelLocation();
                    }
                }
            }
            else if (this.memberTypeComboBox.SelectedIndex == (int)MemberType.Method ||
                this.memberTypeComboBox.SelectedIndex == (int)MemberType.Task)
            {
                if (this._metaPropertyPanel != null)
                {
                    this.Controls.Remove(this._metaPropertyPanel);
                    this._metaPropertyPanel = null;
                }

                if (this._editEnumMemberPanel != null)
                {
                    this.Controls.Remove(this._editEnumMemberPanel);
                    this._editEnumMemberPanel = null;
                }

                if (this._metaMethodPanel == null)
                {
                    this._metaMethodPanel = new MetaMethodPanel();
                    this._metaMethodPanel.Hide();
                    this.Controls.Add(this._metaMethodPanel);

                    setMetaPanelLocation();
                }
            }
            else
            {
                if (this._metaPropertyPanel != null)
                {
                    this.Controls.Remove(this._metaPropertyPanel);
                    this._metaPropertyPanel = null;
                }

                if (this._metaMethodPanel != null)
                {
                    this.Controls.Remove(this._metaMethodPanel);
                    this._metaMethodPanel = null;
                }

                if (this._editEnumMemberPanel != null)
                {
                    this.Controls.Remove(this._editEnumMemberPanel);
                    this._editEnumMemberPanel = null;
                }
            }
        }

        private void setMetaPanelLocation()
        {
            if (this._metaPropertyPanel != null)
            {
                this._metaPropertyPanel.Left = this.memberListBox.Left;
                this._metaPropertyPanel.Width = this.downMemberButton.Right - this._metaPropertyPanel.Left;
                this._metaPropertyPanel.Top = this.memberListBox.Bottom + 10;
            }
            else if (this._metaMethodPanel != null)
            {
                this._metaMethodPanel.Left = this.memberListBox.Left;
                this._metaMethodPanel.Width = this.downMemberButton.Right - this._metaMethodPanel.Left;
                this._metaMethodPanel.Top = this.memberListBox.Bottom + 10;
            }
            else if (this._editEnumMemberPanel != null)
            {
                this._editEnumMemberPanel.Left = this.memberListBox.Left;
                this._editEnumMemberPanel.Width = this.downMemberButton.Right - this._editEnumMemberPanel.Left;
                this._editEnumMemberPanel.Top = this.memberListBox.Bottom + 10;
            }
        }

        private void typeListBox_SelectedIndexChanged(object sender, EventArgs e) {
            this.addMemberButton.Enabled = true;
            this.removeTypeButton.Enabled = false;

            int typeIndex = this.typeListBox.SelectedIndex;
            if (typeIndex >= 0)
            {
                _previousSelectedMemberIndex = -1;
                if (_previousSelectedTypeIndex != typeIndex)
                {
                    apply(false, _previousSelectedTypeIndex, -1, false);

                    _previousSelectedTypeIndex = typeIndex;
                }

                this.removeTypeButton.Enabled = true;

                this.instanceComboBox.Items.Clear();

                if (this.typeListBox.SelectedIndex < Plugin.AgentTypes.Count) {
                    _lastAgent = Plugin.AgentTypes[this.typeListBox.SelectedIndex];

                    AgentType currentViewAgentType = getCurrentViewAgentType();

                    // set all instance names
                    if (currentViewAgentType != null && currentViewAgentType == _lastAgent) {
                        this.instanceComboBox.Items.Add(VariableDef.kSelf);
                    }

                    foreach(Plugin.InstanceName_t instanceName in Plugin.InstanceNames) {
                        if (instanceName.agentType_ == _lastAgent && _lastAgent != currentViewAgentType) {
                            this.instanceComboBox.Items.Add(instanceName.displayName_);
                        }
                    }

                    if (this.instanceComboBox.Items.Count > 0) {
                        this.instanceComboBox.SelectedIndex = 0;
                    }

                    if (!Plugin.AgentTypes[this.typeListBox.SelectedIndex].IsCustomized) {
                        this.removeTypeButton.Enabled = false;
                    }
                }

                this.metaTypePanel.Initialize(this.getSelectedType());

                setMembers();

                if (this.typeListBox.SelectedIndex == 0) {
                    this.addMemberButton.Enabled = false;
                }
            }

            if (this._metaPropertyPanel != null)
            {
                this._metaPropertyPanel.Hide();
            }
            if (this._metaMethodPanel != null)
            {
                this._metaMethodPanel.Hide();
            }
            if (this._editEnumMemberPanel != null)
            {
                this._editEnumMemberPanel.Hide();
            }
            initMetaPanel();
        }

        private void memberTypeComboBox_SelectedIndexChanged(object sender, EventArgs e)
        {
            _lastMemberTypeIndex = this.memberTypeComboBox.SelectedIndex;

            setMembers();

            initMetaPanel();
        }

        private void memberFilterTextBox_TextChanged(object sender, EventArgs e)
        {
            setMembers();
        }

        private void memberFilterCheckBox_CheckedChanged(object sender, EventArgs e)
        {
            setMembers();
        }

        private PropertyDef getPropertyByIndex(int index) {
            if (index > -1 && index < this.memberListBox.Items.Count) {
                MemberItem item = this.memberListBox.Items[index] as MemberItem;
                if (item != null)
                    return item.Property;
            }

            return null;
        }

        private MethodDef getMethodByIndex(int index) {
            if (index > -1 && index < this.memberListBox.Items.Count) {
                MemberItem item = this.memberListBox.Items[index] as MemberItem;
                if (item != null)
                    return item.Method;
            }

            return null;
        }

        private void setDescription() {
            this.removeMemberButton.Enabled = false;
            this.upMemberButton.Enabled = false;
            this.downMemberButton.Enabled = false;

            int index = this.typeListBox.SelectedIndex;
            int memberIndex = this.memberListBox.SelectedIndex;

            if (index > -1 && memberIndex > -1) {
                // agent
                if (index < Plugin.AgentTypes.Count) {
                    AgentType agent = Plugin.AgentTypes[index];

                    if (this.memberTypeComboBox.SelectedIndex == (int)MemberType.Property) {
                        PropertyDef prop = getPropertyByIndex(memberIndex);

                        //don't allow to remove automatically added "_$local_task_param_$_0"
                        if (prop != null && !prop.IsAddedAutomatically && (prop.IsCustomized || prop.IsPar)) {
                            this.removeMemberButton.Enabled = true;
                            this.upMemberButton.Enabled = true;
                            this.downMemberButton.Enabled = true;

                        } else {
                            this.removeMemberButton.Enabled = false;
                            this.upMemberButton.Enabled = false;
                            this.downMemberButton.Enabled = false;
                        }

                        if (prop != null) {
                            bool canBeEdit = prop.IsCustomized | prop.IsPar;

                            if (prop.IsPar)
                            {
                                //don't allow to edit automatically added "_$local_task_param_$_0"
                                if (prop.IsAddedAutomatically)
                                {
                                    canBeEdit = false;
                                }
                            }

                            Nodes.Behavior root = getCurrentRootNode() as Nodes.Behavior;

                            if (this._metaPropertyPanel != null)
                            {
                                this._metaPropertyPanel.Initialize(canBeEdit, agent, null, prop, root != null);
                                this._metaPropertyPanel.Show();
                            }
                        }

                    } else if (this.memberTypeComboBox.SelectedIndex == (int)MemberType.Method ||
                               this.memberTypeComboBox.SelectedIndex == (int)MemberType.Task) {
                        MethodDef method = getMethodByIndex(memberIndex);
                        bool canBeEdit = true;

                        if (method != null && method.IsCustomized) {
                            this.removeMemberButton.Enabled = true;
                            this.upMemberButton.Enabled = true;
                            this.downMemberButton.Enabled = true;

                        } else {
                            canBeEdit = false;
                            this.removeMemberButton.Enabled = false;
                            this.upMemberButton.Enabled = false;
                            this.downMemberButton.Enabled = false;
                        }

                        if (method != null && this._metaMethodPanel != null)
                        {
                            this._metaMethodPanel.Initialize(canBeEdit, agent, method, (MemberType)this.memberTypeComboBox.SelectedIndex);
                            this._metaMethodPanel.Show();
                        }
                    }

                } else {
                    if (this.memberTypeComboBox.SelectedIndex == (int)MemberType.Property) {
                        this.removeMemberButton.Enabled = true;
                        this.upMemberButton.Enabled = true;
                        this.downMemberButton.Enabled = true;

                        // enum
                        if (index - Plugin.AgentTypes.Count < CustomizedTypeManager.Instance.Enums.Count) {
                            CustomizedEnum customizedEnum = CustomizedTypeManager.Instance.Enums[index - Plugin.AgentTypes.Count];
                            CustomizedEnum.CustomizedEnumMember enumMember = customizedEnum.Members[memberIndex];

                            if (this._editEnumMemberPanel != null)
                            {
                                this._editEnumMemberPanel.Initialize(customizedEnum, enumMember);
                                this._editEnumMemberPanel.Show();
                            }
                        }
                        // struct
                        else
                        {
                            CustomizedStruct customizedStruct = CustomizedTypeManager.Instance.Structs[index - Plugin.AgentTypes.Count - CustomizedTypeManager.Instance.Enums.Count];
                            PropertyDef structProp = customizedStruct.Properties[memberIndex];

                            if (this._metaPropertyPanel != null)
                            {
                                this._metaPropertyPanel.Initialize(true, null, customizedStruct, structProp, false);
                                this._metaPropertyPanel.Show();
                            }
                        }
                    }
                }
            }
        }

        private bool isMemberModified()
        {
            if (this._metaPropertyPanel != null)
            {
                return this._metaPropertyPanel.IsModified;
            }
            else if (this._metaMethodPanel != null)
            {
                return this._metaMethodPanel.IsModified;
            }
            else if (this._editEnumMemberPanel != null)
            {
                return this._editEnumMemberPanel.IsModified;
            }

            return false;
        }

        private delegate void preSelectMemberDelegate();

        private void preSelectMember()
        {
            if (_previousSelectedMemberIndex >= 0 && this.isMemberModified())
            {
                apply(false, -1, _previousSelectedMemberIndex, false);
            }
        }

        private void postSelectMember()
        {
            int currentMemberIndex = this.memberListBox.SelectedIndex;

            setDescription();

            _previousSelectedMemberIndex = currentMemberIndex;
            if (this.memberListBox.SelectedIndex != currentMemberIndex)
            {
                this.memberListBox.SelectedIndex = currentMemberIndex;
            }
        }

        private void memberListBox_MouseDown(object sender, MouseEventArgs e)
        {
            preSelectMember();
            
            postSelectMember();

            if (this.memberListBox.SelectedItem != null) {
                string instanceName = this.instanceComboBox.Text;

                if (!string.IsNullOrEmpty(instanceName)) {
                    instanceName = instanceName.Replace(Par_Str, "");
                    instanceName = instanceName.Replace(Customized_Str, "");
                    instanceName = instanceName.Replace(Member_Str, "");
                    instanceName = instanceName.Replace(Empty_Type_Str, "");
                    instanceName = instanceName.Replace(Changeable_Type_Str, "");
                    instanceName = instanceName.Trim();

                    MemberItem item = this.memberListBox.SelectedItem as MemberItem;
                    Debug.Check(item.Property != null || item.Method != null);
                    string memberName = (item.Property != null) ? item.Property.Name : item.Method.Name;

                    memberName = instanceName + "." + memberName;

                    // method
                    if (this.memberTypeComboBox.SelectedIndex != (int)MemberType.Property) {
                        memberName += "()";
                    }

                    memberListBox.DoDragDrop(memberName, DragDropEffects.Move);
                }
            }
        }

        private void memberListBox_KeyDown(object sender, KeyEventArgs e)
        {
            if (e.KeyCode == Keys.Delete)
            {
                removeSelectedMember();
            }
            else
            {
                preSelectMember();
            }
        }

        private void memberListBox_SelectedIndexChanged(object sender, EventArgs e)
        {
            postSelectMember();
        }

        private object getSelectedType() {
            int index = this.typeListBox.SelectedIndex;

            if (index < 0) {
                return null;
            }

            if (index < Plugin.AgentTypes.Count) {
                return Plugin.AgentTypes[index];
            }

            index -= Plugin.AgentTypes.Count;

            if (index < CustomizedTypeManager.Instance.Enums.Count) {
                return CustomizedTypeManager.Instance.Enums[index];
            }

            index -= CustomizedTypeManager.Instance.Enums.Count;

            return CustomizedTypeManager.Instance.Structs[index];
        }

        private void addTypeButton_Click(object sender, EventArgs e)
        {
            MetaTypeDialog typeDialog = new MetaTypeDialog(null);

            if (DialogResult.OK == typeDialog.ShowDialog())
            {
                Workspace.Current.IsBlackboardDirty = true;

                MetaTypePanel.MetaTypes metaType = typeDialog.GetMetaType();

                if (metaType == MetaTypePanel.MetaTypes.Agent)
                {
                    AgentType agentType = typeDialog.GetCustomizedAgent();
                    Debug.Check(agentType != null);

                    Plugin.AgentTypes.Add(agentType);

                    resetAllTypes(agentType.AgentTypeName);

                    _lastSelectedType = agentType.AgentTypeName;
                }
                else if (metaType == MetaTypePanel.MetaTypes.Enum)
                {
                    CustomizedEnum customizedEnum = typeDialog.GetCustomizedEnum();
                    Debug.Check(customizedEnum != null);

                    CustomizedTypeManager.Instance.Enums.Add(customizedEnum);

                    resetAllTypes(customizedEnum.Name);

                    _lastSelectedType = customizedEnum.Name;
                }
                else if (metaType == MetaTypePanel.MetaTypes.Struct)
                {
                    CustomizedStruct customizedStruct = typeDialog.GetCustomizedStruct();
                    Debug.Check(customizedStruct != null);

                    CustomizedTypeManager.Instance.Structs.Add(customizedStruct);

                    resetAllTypes(customizedStruct.Name);

                    _lastSelectedType = customizedStruct.Name;
                }

                // refresh the workspace to load the type               
                save(true);
            }
        }

        private void editType(int typeSelectedIndex)
        {
            if (typeSelectedIndex > -1)
            {
                Workspace.Current.IsBlackboardDirty = true;

                MetaTypePanel.MetaTypes metaType = this.metaTypePanel.GetMetaType();

                if (metaType == MetaTypePanel.MetaTypes.Agent)
                {
                    AgentType agentType = this.metaTypePanel.GetCustomizedAgent();
                    Debug.Check(agentType != null);

                    string disp = Customized_Str + agentType.AgentTypeName;
                    this.typeListBox.Items[typeSelectedIndex] = disp;

                    _lastSelectedType = agentType.AgentTypeName;
                }
                else if (metaType == MetaTypePanel.MetaTypes.Enum)
                {
                    CustomizedEnum customizedEnum = this.metaTypePanel.GetCustomizedEnum();
                    Debug.Check(customizedEnum != null);

                    string disp = Customized_Str + customizedEnum.Name;
                    this.typeListBox.Items[typeSelectedIndex] = disp;

                    _lastSelectedType = customizedEnum.Name;
                }
                else if (metaType == MetaTypePanel.MetaTypes.Struct)
                {
                    CustomizedStruct customizedStruct = this.metaTypePanel.GetCustomizedStruct();
                    Debug.Check(customizedStruct != null);

                    string disp = Customized_Str + customizedStruct.Name;
                    this.typeListBox.Items[typeSelectedIndex] = disp;

                    _lastSelectedType = customizedStruct.Name;
                }

                // refresh the workspace to load the type
                save(true);
            }
        }

        private void removeTypeButton_Click(object sender, EventArgs e) {
            int index = this.typeListBox.SelectedIndex;

            if (index > -1 &&
                DialogResult.OK == MessageBox.Show(Resources.TypeRemoveWarning, Resources.Warning, MessageBoxButtons.OKCancel))
            {
                if (index < Plugin.AgentTypes.Count) {
                    AgentType agent = Plugin.AgentTypes[index];

                    if (agent.IsCustomized) {
                        Workspace.Current.IsBlackboardDirty = true;

                        Plugin.AgentTypes.RemoveAt(index);

                        this.typeListBox.Items.RemoveAt(index);
                    }

                } else {
                    if (index - Plugin.AgentTypes.Count < CustomizedTypeManager.Instance.Enums.Count) {
                        CustomizedTypeManager.Instance.Enums.RemoveAt(index - Plugin.AgentTypes.Count);

                        this.typeListBox.Items.RemoveAt(index);

                    } else {
                        CustomizedTypeManager.Instance.Structs.RemoveAt(index - Plugin.AgentTypes.Count - CustomizedTypeManager.Instance.Enums.Count);

                        this.typeListBox.Items.RemoveAt(index);
                    }
                }

                if (index >= this.typeListBox.Items.Count) {
                    index = this.typeListBox.Items.Count - 1;
                }

                this.typeListBox.SelectedIndex = index;

                _lastSelectedType = "";

                // refresh the workspace to load the type
                save(true);
            }
        }

        private int getLastParIndex() {
            int index = 0;

            for (int i = 0; i < this.memberListBox.Items.Count; ++i) {
                MemberItem item = this.memberListBox.Items[i] as MemberItem;

                if (item.Property != null && item.Property.IsPar) {
                    index++;

                } else {
                    break;
                }
            }

            return index;
        }

        private void addMember() {
            int index = this.typeListBox.SelectedIndex;

            if (index > -1) {
                // agent
                if (index < Plugin.AgentTypes.Count) {
                    AgentType agent = Plugin.AgentTypes[index];

                    if (this.memberTypeComboBox.SelectedIndex == (int)MemberType.Property)
                    {
                        Nodes.Behavior root = getCurrentRootNode() as Nodes.Behavior;
                        MetaPropertyDialog propertyDialog = new MetaPropertyDialog(true, agent, null, null, root != null);

                        if (DialogResult.OK == propertyDialog.ShowDialog())
                        {
                            PropertyDef prop = propertyDialog.GetProperty();
                            Debug.Check(prop != null);

                            if (agent.AddProperty(prop))
                            {
                                Workspace.Current.IsBlackboardDirty = true;

                                if (prop.IsPar)
                                {
                                    if (root != null)
                                    {
                                        root.LocalVars.Add(prop as ParInfo);

                                        UndoManager.Save(root);

                                        _isParDirtyBehavior = root;
                                    }

                                    int lastParIndex = getLastParIndex();
                                    this.memberListBox.Items.Insert(lastParIndex, new MemberItem(Par_Str + prop.DisplayName, prop));
                                    this.memberListBox.SelectedIndex = lastParIndex;
                                }
                                else
                                {
                                    this.memberListBox.Items.Add(new MemberItem(Customized_Str + prop.DisplayName, prop));
                                    this.memberListBox.SelectedIndex = this.memberListBox.Items.Count - 1;
                                }

                                _lastSelectedType = "";
                            }
                        }

                    } else if (this.memberTypeComboBox.SelectedIndex == (int)MemberType.Method ||
                               this.memberTypeComboBox.SelectedIndex == (int)MemberType.Task) {
                        MetaMethodDialog methodDialog = new MetaMethodDialog(agent, null, (MemberType)this.memberTypeComboBox.SelectedIndex);

                        if (DialogResult.OK == methodDialog.ShowDialog()) {
                            MethodDef method = methodDialog.GetMethod();
                            Debug.Check(method != null);

                            if (agent.AddMethod(method)) {
                                Workspace.Current.IsBlackboardDirty = true;

                                this.memberListBox.Items.Add(new MemberItem(Customized_Str + method.DisplayName, method));
                                this.memberListBox.SelectedIndex = this.memberListBox.Items.Count - 1;

                                _lastSelectedType = "";
                            }
                        }
                    }

                } else {
                    if (this.memberTypeComboBox.SelectedIndex == (int)MemberType.Property) {
                        // enum
                        if (index - Plugin.AgentTypes.Count < CustomizedTypeManager.Instance.Enums.Count) {
                            CustomizedEnum customizedEnum = CustomizedTypeManager.Instance.Enums[index - Plugin.AgentTypes.Count];
                            EditEnumMemberDialog enumMemberDialog = new EditEnumMemberDialog(customizedEnum, null);

                            if (DialogResult.OK == enumMemberDialog.ShowDialog()) {
                                Workspace.Current.IsBlackboardDirty = true;

                                CustomizedEnum.CustomizedEnumMember enumMember = enumMemberDialog.GetEnumMember();
                                Debug.Check(enumMember != null);

                                customizedEnum.Members.Add(enumMember);

                                this.memberListBox.Items.Add(Customized_Str + enumMember.DisplayName);
                                this.memberListBox.SelectedIndex = this.memberListBox.Items.Count - 1;

                                _lastSelectedType = customizedEnum.Name;

                                // refresh the workspace to load the type
                                save(true);
                            }
                        }

                        // struct
                        else {
                            CustomizedStruct customizedStruct = CustomizedTypeManager.Instance.Structs[index - Plugin.AgentTypes.Count - CustomizedTypeManager.Instance.Enums.Count];
                            MetaPropertyDialog propertyDialog = new MetaPropertyDialog(true, null, customizedStruct, null, false);

                            if (DialogResult.OK == propertyDialog.ShowDialog()) {
                                Workspace.Current.IsBlackboardDirty = true;

                                PropertyDef prop = propertyDialog.GetProperty();
                                Debug.Check(prop != null);

                                customizedStruct.Properties.Add(prop);

                                this.memberListBox.Items.Add(Customized_Str + prop.DisplayName);
                                this.memberListBox.SelectedIndex = this.memberListBox.Items.Count - 1;

                                _lastSelectedType = customizedStruct.Name;

                                // refresh the workspace to load the type
                                save(true);
                            }
                        }
                    }
                }
            }
        }

        private void addMemberButton_Click(object sender, EventArgs e) {
            addMember();
        }

        IList<Nodes.BehaviorNode> getAllBehaviors(PropertyDef property) {
            List<Nodes.BehaviorNode> allBehaviors = new List<Nodes.BehaviorNode>();

            if (property != null && property.IsPar) {
                allBehaviors.Add(getCurrentRootNode());

            } else {
                BehaviorTreeList behaviorTreeList = BehaviorManager.Instance as BehaviorTreeList;

                if (behaviorTreeList != null) {
                    foreach(Nodes.BehaviorNode behavior in behaviorTreeList.GetAllOpenedBehaviors()) {
                        if (!allBehaviors.Contains(behavior)) {
                            allBehaviors.Add(behavior);
                        }
                    }

                    foreach(Nodes.BehaviorNode behavior in behaviorTreeList.GetAllBehaviors()) {
                        if (!allBehaviors.Contains(behavior)) {
                            allBehaviors.Add(behavior);
                        }
                    }
                }
            }

            return allBehaviors;
        }

        private bool checkMembersInWorkspace(AgentType agentType, bool clear, MethodDef method, PropertyDef property) {
            foreach(Nodes.BehaviorNode behavior in getAllBehaviors(property)) {
                if (behavior != null && behavior is Nodes.Node) {
                    bool bReset = ((Nodes.Node)behavior).ResetMembers(true, agentType, clear, method, property);
                    if (bReset)
                        return DialogResult.Yes == MessageBox.Show(Resources.ModifyMemberWarning, Resources.Warning, MessageBoxButtons.YesNo);
                }
            }

            return true;
        }

        private void resetMembersInWorkspace(AgentType agentType, bool clear, MethodDef method, PropertyDef property) {
            foreach(Nodes.BehaviorNode behavior in getAllBehaviors(property)) {
                if (behavior != null && behavior is Nodes.Node) {
                    bool bReset = ((Nodes.Node)behavior).ResetMembers(false, agentType, clear, method, property);
                    if (bReset)
                        UndoManager.Save(behavior);
                }
            }
        }

        private void editMember(int selectedMemberIndex, bool resetMemberSelectedIndex)
        {
            int index = this.typeListBox.SelectedIndex;
            int memberIndex = selectedMemberIndex;

            if (index > -1 && memberIndex > -1) {
                // agent
                if (index < Plugin.AgentTypes.Count) {
                    AgentType agent = Plugin.AgentTypes[index];
                    MemberItem item = this.memberListBox.Items[memberIndex] as MemberItem;
                    bool bEdit = false;

                    if (this.memberTypeComboBox.SelectedIndex == (int)MemberType.Property) {
                        PropertyDef prop = getPropertyByIndex(memberIndex);

                        if (prop != null && this._metaPropertyPanel != null)
                        {
                            bool canBeEdit = prop.IsCustomized | prop.IsPar;

                            if (prop.IsPar)
                            {
                                //don't allow to edit automatically added "_$local_task_param_$_0"
                                if (prop.IsAddedAutomatically) {
                                    canBeEdit = false;
                                }
                            }

                            Nodes.Behavior root = getCurrentRootNode() as Nodes.Behavior;

                            PropertyDef curProp = this._metaPropertyPanel.GetProperty();
                            Debug.Check(curProp != null);

                            if (prop.IsMember && !prop.IsChangeableType)
                            {
                                prop.IsExportedButAlsoCustomized = curProp.IsExportedButAlsoCustomized;
                                bEdit = true;
                            }
                            else if ((prop.IsChangeableType || canBeEdit) && (curProp.Name == prop.Name || checkMembersInWorkspace(agent, false, null, curProp)))
                            {
                                if (curProp.IsPar || curProp.IsPar != prop.IsPar)
                                {
                                    _isParDirtyBehavior = root;
                                }

                                if (curProp.Type != prop.Type)
                                {
                                    resetMembersInWorkspace(agent, true, null, curProp);
                                }
                                else if (curProp.Name != prop.Name)
                                {
                                    resetMembersInWorkspace(agent, false, null, curProp);
                                }

                                if (Plugin.IsArrayType(prop.Type))
                                {
                                    PropertyDef elementProperty = agent.GetPropertyByName(prop.BasicName + "[]");

                                    if (elementProperty != null && elementProperty.IsArrayElement)
                                    {
                                        elementProperty.SetArrayElement(curProp);
                                    }
                                }

                                prop.CopyFrom(curProp);

                                item.DisplayName = getPrefixString(prop) + prop.DisplayName;
                                bEdit = true;

                                if (curProp.IsPar != prop.IsPar)
                                {
                                    bEdit = false;
                                    Workspace.Current.IsBlackboardDirty = true;

                                    this.memberListBox.Items.RemoveAt(memberIndex);

                                    if (curProp.IsPar)
                                    {
                                        if (agent.RemoveProperty(prop))
                                        {
                                            if (root != null)
                                            {
                                                root.LocalVars.Add(curProp as ParInfo);

                                                UndoManager.Save(root);
                                            }

                                            int lastParIndex = getLastParIndex();
                                            this.memberListBox.Items.Insert(lastParIndex, new MemberItem(Par_Str + curProp.DisplayName, curProp));
                                            if (resetMemberSelectedIndex)
                                                this.memberListBox.SelectedIndex = lastParIndex;
                                        }
                                    }
                                    else
                                    {
                                        if (root != null && root.LocalVars.Remove(prop as ParInfo))
                                        {
                                            UndoManager.Save(root);
                                        }

                                        agent.RemoveProperty(prop);
                                        agent.AddProperty(curProp);

                                        this.memberListBox.Items.Add(new MemberItem(Customized_Str + curProp.DisplayName, curProp));
                                        if (resetMemberSelectedIndex)
                                            this.memberListBox.SelectedIndex = this.memberListBox.Items.Count - 1;
                                    }

                                    BehaviorTreeViewDock.RefreshAll();
                                    PropertiesDock.UpdatePropertyGrids();
                                }
                                else
                                {
                                    if (curProp.IsPar)
                                    {
                                        if (root != null)
                                        {
                                            UndoManager.Save(root);
                                        }
                                    }
                                }
                            }
                        }
                    } else if (this.memberTypeComboBox.SelectedIndex == (int)MemberType.Method ||
                               this.memberTypeComboBox.SelectedIndex == (int)MemberType.Task) {
                        MethodDef method = getMethodByIndex(memberIndex);

                        if (method != null && this._metaMethodPanel != null)
                        {
                            MethodDef curMethod = this._metaMethodPanel.GetMethod();
                            Debug.Check(curMethod != null);

                            if ((method.IsChangeableType || method.IsCustomized) && checkMembersInWorkspace(agent, false, curMethod, null))
                            {
                                resetMembersInWorkspace(agent, false, curMethod, null);

                                method.CopyFrom(curMethod);

                                item.DisplayName = getPrefixString(method) + method.DisplayName;
                                bEdit = true;
                            }
                        }
                    }

                    if (bEdit)
                    {
                        _lastSelectedType = "";

                        Workspace.Current.IsBlackboardDirty = true;

                        // update the memberListBox
                        this.memberListBox.Items.RemoveAt(memberIndex);
                        this.memberListBox.Items.Insert(memberIndex, item);
                        if (resetMemberSelectedIndex)
                            this.memberListBox.SelectedIndex = memberIndex;

                        BehaviorTreeViewDock.RefreshAll();
                        PropertiesDock.UpdatePropertyGrids();
                    }

                } else {
                    bool bEdit = false;

                    if (this.memberTypeComboBox.SelectedIndex == (int)MemberType.Property)
                    {
                        // enum
                        if (index - Plugin.AgentTypes.Count < CustomizedTypeManager.Instance.Enums.Count)
                        {
                            if (this._editEnumMemberPanel != null)
                            {
                                CustomizedEnum.CustomizedEnumMember enumMember = this._editEnumMemberPanel.GetEnumMember();
                                Debug.Check(enumMember != null);

                                CustomizedEnum customizedEnum = CustomizedTypeManager.Instance.Enums[index - Plugin.AgentTypes.Count];
                                customizedEnum.Members[memberIndex] = enumMember;
                                this.memberListBox.Items[memberIndex] = Customized_Str + enumMember.DisplayName;

                                bEdit = true;

                                _lastSelectedType = customizedEnum.Name;

                                // refresh the workspace to load the type
                                save(true);
                            }
                        }
                        // struct
                        else
                        {
                            if (this._metaPropertyPanel != null)
                            {
                                Workspace.Current.IsBlackboardDirty = true;

                                PropertyDef prop = this._metaPropertyPanel.GetProperty();
                                Debug.Check(prop != null);

                                CustomizedStruct customizedStruct = CustomizedTypeManager.Instance.Structs[index - Plugin.AgentTypes.Count - CustomizedTypeManager.Instance.Enums.Count];
                                customizedStruct.Properties[memberIndex] = prop;
                                this.memberListBox.Items[memberIndex] = Customized_Str + prop.DisplayName;

                                bEdit = true;

                                _lastSelectedType = customizedStruct.Name;

                                // refresh the workspace to load the type
                                save(true);
                            }
                        }
                    }

                    if (bEdit)
                    {
                        Workspace.Current.IsBlackboardDirty = true;

                        BehaviorTreeViewDock.RefreshAll();
                        PropertiesDock.UpdatePropertyGrids();
                    }
                }
            }
        }

        private void removeSelectedMember()
        {
            int index = this.typeListBox.SelectedIndex;
            int memberIndex = this.memberListBox.SelectedIndex;

            if (index > -1 && memberIndex > -1) {
                // agent
                if (index < Plugin.AgentTypes.Count) {
                    AgentType agent = Plugin.AgentTypes[index];
                    bool bRemoved = false;

                    if (this.memberTypeComboBox.SelectedIndex == (int)MemberType.Property) {
                        PropertyDef prop = getPropertyByIndex(memberIndex);

                        if (prop != null) {
                            prop.OldName = prop.Name;
                        }

                        if (prop != null && (prop.IsCustomized || prop.IsPar) && !prop.IsAddedAutomatically &&
                            checkMembersInWorkspace(agent, true, null, prop) &&
                            agent.RemoveProperty(prop)) {
                            bRemoved = true;

                            this.memberListBox.Items.RemoveAt(memberIndex);

                            if (prop.IsPar) {
                                Nodes.BehaviorNode root = getCurrentRootNode();

                                if (root != null && ((Nodes.Node)root).LocalVars.Remove(prop as ParInfo)) {
                                    UndoManager.Save(root);

                                    _isParDirtyBehavior = root;
                                }
                            }

                            if (memberIndex >= this.memberListBox.Items.Count) {
                                memberIndex = this.memberListBox.Items.Count - 1;
                            }

                            this.memberListBox.SelectedIndex = memberIndex;

                            resetMembersInWorkspace(agent, true, null, prop);
                        }

                    } else if (this.memberTypeComboBox.SelectedIndex == (int)MemberType.Method ||
                               this.memberTypeComboBox.SelectedIndex == (int)MemberType.Task) {
                        MethodDef method = getMethodByIndex(memberIndex);

                        if (method != null) {
                            method.OldName = method.Name;
                        }

                        if (method != null && method.IsCustomized &&
                            checkMembersInWorkspace(agent, true, method, null) &&
                            agent.RemoveMethod(method)) {
                            bRemoved = true;

                            this.memberListBox.Items.RemoveAt(memberIndex);

                            if (memberIndex >= this.memberListBox.Items.Count) {
                                memberIndex = this.memberListBox.Items.Count - 1;
                            }

                            this.memberListBox.SelectedIndex = memberIndex;

                            resetMembersInWorkspace(agent, true, method, null);
                        }
                    }

                    if (bRemoved) {
                        setDescription();

                        _lastSelectedType = "";

                        Workspace.Current.IsBlackboardDirty = true;

                        BehaviorTreeViewDock.RefreshAll();
                        PropertiesDock.UpdatePropertyGrids();
                    }

                } else {
                    if (this.memberTypeComboBox.SelectedIndex == (int)MemberType.Property) {
                        // enum
                        if (index - Plugin.AgentTypes.Count < CustomizedTypeManager.Instance.Enums.Count) {
                            CustomizedEnum customizedEnum = CustomizedTypeManager.Instance.Enums[index - Plugin.AgentTypes.Count];

                            customizedEnum.Members.RemoveAt(memberIndex);

                            _lastSelectedType = customizedEnum.Name;
                        }

                        // struct
                        else {
                            CustomizedStruct customizedStruct = CustomizedTypeManager.Instance.Structs[index - Plugin.AgentTypes.Count - CustomizedTypeManager.Instance.Enums.Count];

                            customizedStruct.Properties.RemoveAt(memberIndex);

                            _lastSelectedType = customizedStruct.Name;
                        }

                        this.memberListBox.Items.RemoveAt(memberIndex);

                        if (memberIndex >= this.memberListBox.Items.Count) {
                            memberIndex = this.memberListBox.Items.Count - 1;
                        }

                        this.memberListBox.SelectedIndex = memberIndex;

                        setDescription();

                        Workspace.Current.IsBlackboardDirty = true;

                        BehaviorTreeViewDock.RefreshAll();
                        PropertiesDock.UpdatePropertyGrids();

                        // refresh the workspace to load the type
                        save(true);
                    }
                }
            }
        }

        private void removeMemberButton_Click(object sender, EventArgs e)
        {
            removeSelectedMember();
        }

        private void swapTwoProperties(int index1, int index2) {
            int index = this.typeListBox.SelectedIndex;

            if (index > -1 && index1 > -1 && index1 < this.memberListBox.Items.Count &&
                index2 > -1 && index2 < this.memberListBox.Items.Count) {
                AgentType agent = Plugin.AgentTypes[index];
                bool swapSucceeded = false;

                if (this.memberTypeComboBox.SelectedIndex == (int)MemberType.Property) {
                    PropertyDef prop1 = getPropertyByIndex(index1);
                    PropertyDef prop2 = getPropertyByIndex(index2);

                    if (prop1 != null && prop2 != null) {
                        swapSucceeded = agent.SwapTwoProperties(prop1, prop2);

                        if (swapSucceeded && prop1.IsPar) {
                            if (prop1.IsPar && prop2.IsPar) {
                                Nodes.BehaviorNode root = getCurrentRootNode();

                                if (root != null) {
                                    Nodes.Node node = (Nodes.Node)root;

                                    ParInfo prePar = node.LocalVars[index2];
                                    node.LocalVars[index2] = node.LocalVars[index1];
                                    node.LocalVars[index1] = prePar;

                                    UndoManager.Save(root);

                                    _isParDirtyBehavior = root;
                                }
                            }
                        }
                    }

                } else if (this.memberTypeComboBox.SelectedIndex == (int)MemberType.Method ||
                           this.memberTypeComboBox.SelectedIndex == (int)MemberType.Task) {
                    MethodDef method1 = getMethodByIndex(index1);
                    MethodDef method2 = getMethodByIndex(index2);

                    if (method1 != null && method2 != null) {
                        swapSucceeded = agent.SwapTwoMethods(method1, method2);
                    }
                }

                if (swapSucceeded) {
                    object preItem = this.memberListBox.Items[index2];
                    this.memberListBox.Items[index2] = this.memberListBox.Items[index1];
                    this.memberListBox.Items[index1] = preItem;

                    this.memberListBox.SelectedIndex = index2;

                    Workspace.Current.IsBlackboardDirty = true;
                }
            }
        }

        private void moveCustomizedType(CustomizedEnum customizedEnum, CustomizedStruct customizedStruct, int sourceIndex, int targetIndex) {
            if (sourceIndex >= 0 && sourceIndex < this.memberListBox.Items.Count &&
                targetIndex >= 0 && targetIndex < this.memberListBox.Items.Count) {
                if (customizedEnum != null) {
                    CustomizedEnum.CustomizedEnumMember enumMember = customizedEnum.Members[sourceIndex];
                    customizedEnum.Members.RemoveAt(sourceIndex);
                    customizedEnum.Members.Insert(targetIndex, enumMember);

                    Workspace.Current.IsBlackboardDirty = true;

                } else if (customizedStruct != null) {
                    PropertyDef structMember = customizedStruct.Properties[sourceIndex];
                    customizedStruct.Properties.RemoveAt(sourceIndex);
                    customizedStruct.Properties.Insert(targetIndex, structMember);

                    Workspace.Current.IsBlackboardDirty = true;
                }

                object item = this.memberListBox.Items[sourceIndex];
                this.memberListBox.Items.RemoveAt(sourceIndex);
                this.memberListBox.Items.Insert(targetIndex, item);

                this.memberListBox.SelectedIndex = targetIndex;
            }
        }

        private void moveMember(int sourceIndex, int targetIndex) {
            int index = this.typeListBox.SelectedIndex;
            int memberIndex = this.memberListBox.SelectedIndex;

            if (index > -1 && memberIndex > -1) {
                // agent
                if (index < Plugin.AgentTypes.Count) {
                    swapTwoProperties(sourceIndex, targetIndex);

                } else {
                    if (this.memberTypeComboBox.SelectedIndex == (int)MemberType.Property) {
                        // enum
                        if (index - Plugin.AgentTypes.Count < CustomizedTypeManager.Instance.Enums.Count) {
                            CustomizedEnum customizedEnum = CustomizedTypeManager.Instance.Enums[index - Plugin.AgentTypes.Count];

                            moveCustomizedType(customizedEnum, null, sourceIndex, targetIndex);
                        }

                        // struct
                        else {
                            CustomizedStruct customizedStruct = CustomizedTypeManager.Instance.Structs[index - Plugin.AgentTypes.Count - CustomizedTypeManager.Instance.Enums.Count];

                            moveCustomizedType(null, customizedStruct, sourceIndex, targetIndex);
                        }
                    }
                }
            }
        }

        private void upMemberButton_Click(object sender, EventArgs e) {
            int memberIndex = this.memberListBox.SelectedIndex;

            moveMember(memberIndex, memberIndex - 1);
        }

        private void downMemberButton_Click(object sender, EventArgs e) {
            int memberIndex = this.memberListBox.SelectedIndex;

            moveMember(memberIndex, memberIndex + 1);
        }

        private void memberListBox_Format(object sender, ListControlConvertEventArgs e) {
            if (e.ListItem is MemberItem) {
                MemberItem item = e.ListItem as MemberItem;
                e.Value = item.DisplayName;
            }
        }

        private void MetaStoreDock_Resize(object sender, EventArgs e)
        {
            setMetaPanelLocation();
        }
    }
}
