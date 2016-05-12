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
using System.IO;
using System.Xml;
using Behaviac.Design.Nodes;
using Behaviac.Design.Attributes;
using Behaviac.Design.Properties;

namespace Behaviac.Design.Exporters
{
    /// <summary>
    /// This exporter generates .cs files which generate a static variable which holds the behaviour tree.
    /// </summary>
    public class ExporterJson : ExporterXml
    {
        public ExporterJson(BehaviorNode node, string outputFolder, string filename, List<string> includedFilenames = null)
            : base(node, outputFolder, "", includedFilenames)
        {
            this._filename = filename + ".json";
        }

        public static string GetUTF8String(byte[] buffer) {
            if (buffer == null)
            { return null; }

            if (buffer.Length <= 3) {
                return Encoding.UTF8.GetString(buffer);
            }

            byte[] bomBuffer = new byte[] { 0xef, 0xbb, 0xbf };

            if (buffer[0] == bomBuffer[0]
                && buffer[1] == bomBuffer[1]
                && buffer[2] == bomBuffer[2]) {
                return new UTF8Encoding(false).GetString(buffer, 3, buffer.Length - 3);
            }

            return Encoding.UTF8.GetString(buffer);
        }

        public override FileManagers.SaveResult Export() {
            string filename = Path.Combine(_outputFolder, _filename);
            FileManagers.SaveResult result = FileManagers.FileManager.MakeWritable(filename, Resources.ExportFileWarning);

            if (FileManagers.SaveResult.Succeeded != result)
            { return result; }

            // get the abolute folder of the file we want toexport
            string folder = Path.GetDirectoryName(filename);

            if (!Directory.Exists(folder))
            { Directory.CreateDirectory(folder); }

            // export to the file
            using(MemoryStream ms = new MemoryStream()) {
                XmlWriterSettings ws = new XmlWriterSettings();
                ws.Indent = true;
                //ws.OmitXmlDeclaration = true;

                using(XmlWriter xmlWrtier = XmlWriter.Create(ms, ws)) {
                    xmlWrtier.WriteStartDocument();
                    ExportBehavior(xmlWrtier, _node);
                    xmlWrtier.WriteEndDocument();
                }

                //using (XmlReader xmlReader = XmlReader.Create(ms))
                {
                    XmlDocument xmlDoc = new XmlDocument();
                    byte[] data = ms.ToArray();
                    string xmlString = GetUTF8String(data);
                    xmlDoc.LoadXml(xmlString);
                    //xmlDoc.Load(xmlReader);

                    string json = XmlToJson.XmlToJSON(xmlDoc);

                    // export to the file
                    using(StreamWriter file = new StreamWriter(filename)) {
                        file.Write(json);
                        file.Close();
                    }
                }
            }

            return FileManagers.SaveResult.Succeeded;
        }

    }
}
