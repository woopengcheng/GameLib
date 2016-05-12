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
using System.Text;
using Behaviac.Design;
using Behaviac.Design.Nodes;
using PluginBehaviac.Properties;
using Behaviac.Design.Attributes;

namespace PluginBehaviac.Nodes
{
    [NodeDesc("Decorators", "repeat_icon")]
    public class DecoratorRepeat : DecoratorCount
	{
		public DecoratorRepeat() : base(Resources.DecoratorRepeat, Resources.DecoratorRepeatDesc)
		{
            this._count = new VariableDef((int)1);
		}

        public override string DocLink
        {
            get { return "http://www.behaviac.com/docs/zh/references/decorator/#repeat"; }
        }

        public override string ExportClass
        {
            get { return "DecoratorRepeat"; }
        }

        public override void CheckForErrors(BehaviorNode rootBehavior, List<ErrorCheck> result)
        {
            Type valueType = this._count.ValueType;

            string typeName = Plugin.GetNativeTypeName(valueType.FullName);

            if (Plugin.IsIntergerNumberType(typeName))
            {
                if (this._count.ValueClass == VariableDef.kConst)
                {
                    string valueString = this._count.Value.ToString();
                    if (valueType == typeof(long) || valueType == typeof(int) || valueType == typeof(short) || valueType == typeof(sbyte))
                    {
                        long count = long.Parse(valueString);
                        if (count <= 0)
                        {
                            result.Add(new Node.ErrorCheck(this, ErrorCheckLevel.Error, "Count should be larger than -1!"));
                        }
                    }
                    else if (valueType == typeof(ulong) || valueType == typeof(uint) || valueType == typeof(ushort) || valueType == typeof(byte))
                    {
                        ulong count = ulong.Parse(valueString);
                        if (count >= 100000000)
                        {
                            result.Add(new Node.ErrorCheck(this, ErrorCheckLevel.Warning, "Count is a huge number it could be wrong!"));
                        }
                    }
                    else
                    {
                        string errMsg = string.Format("Count is a '{0}', it is not a number!", valueType.Name);
                        result.Add(new Node.ErrorCheck(this, ErrorCheckLevel.Error, errMsg));
                    }
                }
            }
            else
            {
                result.Add(new Node.ErrorCheck(this, ErrorCheckLevel.Error, "Count should be an integer number type!"));
            }

            base.CheckForErrors(rootBehavior, result);
        }
	}
}
