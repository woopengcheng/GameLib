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
using System.Collections;
using System.ComponentModel;
using System.IO;
using System.Text;
using System.Windows.Forms;
using Microsoft.Win32;

namespace Behaviac.Design
{
    public class RecentMenu
    {
        public class RecentMenuItem : MenuItem
        {
            protected String filename = string.Empty;
            public String Filename {
                get { return filename; }
                set { filename = value; }
            }

            public RecentMenuItem(String _filename, String entryname, EventHandler eventHandler)
                : base(entryname, eventHandler) {
                filename = _filename;
            }
        }

        protected ToolStripMenuItem recentFileMenuItem;
        protected ClickedHandler clickedHandler;
        protected String registryKeyName;
        protected int numEntries = 0;
        protected int maxEntries = 5;
        protected int maxShortenPathLength = 48;

        public RecentMenu(ToolStripMenuItem _recentFileMenuItem, ClickedHandler _clickedHandler, String _registryKeyName) {
            recentFileMenuItem = _recentFileMenuItem;
            recentFileMenuItem.Checked = false;
            recentFileMenuItem.Enabled = false;

            clickedHandler = _clickedHandler;

            if (_registryKeyName != null) {
                RegistryKeyName = _registryKeyName;
                LoadFromRegistry();
            }
        }

        public delegate void ClickedHandler(String filename);

        protected void OnClick(object sender, System.EventArgs e) {
            ToolStripMenuItem menuItem = (ToolStripMenuItem)sender;
            clickedHandler(menuItem.Text);
        }

        public virtual ToolStripItemCollection MenuItems {
            get { return recentFileMenuItem.DropDownItems; }
        }

        public virtual int StartIndex {
            get { return 0; }
        }

        public virtual int EndIndex {
            get { return numEntries; }
        }

        public int NumEntries {
            get { return numEntries; }
        }

        public int MaxShortenPathLength {
            get { return maxShortenPathLength; }
            set { maxShortenPathLength = value < 16 ? 16 : value; }
        }

        protected virtual void Enable() {
            recentFileMenuItem.Enabled = true;
        }

        protected virtual void Disable() {
            recentFileMenuItem.Enabled = false;
            recentFileMenuItem.DropDownItems.RemoveAt(0);
        }

        protected virtual void SetFirstFile(ToolStripItem menuItem) {
        }

        public void SetFirstFile(int number) {
            if (number > 0 && numEntries > 1 && number < numEntries) {
                ToolStripItem menuItem = MenuItems[StartIndex + number];
                MenuItems.Remove(menuItem);
                MenuItems.Insert(0, menuItem);
                SetFirstFile(menuItem);
            }
        }

        public static String FixupEntryname(int number, String entryname) {
            if (number < 9) {
                return "&" + (number + 1) + "  " + entryname;
            }

            else if (number == 9) {
                return "1&0" + "  " + entryname;
            }

            else {
                return (number + 1) + "  " + entryname;
            }
        }

        public int FindFilenameNumber(String filename) {
            if (filename == null) {
                throw new ArgumentNullException("filename");
            }

            if (filename.Length == 0) {
                throw new ArgumentException("filename");
            }

            if (numEntries > 0) {
                int number = 0;

                for (int i = StartIndex; i < EndIndex; i++, number++) {
                    if (i >= 0 && i < MenuItems.Count &&
                        String.Compare(((ToolStripMenuItem)MenuItems[i]).Text, filename, true) == 0) {
                        return number;
                    }
                }
            }

            return -1;
        }

        public int FindFilenameMenuIndex(String filename) {
            int number = FindFilenameNumber(filename);
            return number < 0 ? -1 : StartIndex + number;
        }

        public String GetFileAt(int number) {
            if (number < 0 || number >= numEntries) {
                return string.Empty;
            }

            return ((ToolStripMenuItem)MenuItems[StartIndex + number]).Text;
        }

        static public String ShortenPathname(String pathname, int maxLength) {
            if (pathname.Length <= maxLength) {
                return pathname;
            }

            String root = Path.GetPathRoot(pathname);

            if (root.Length > 3) {
                root += Path.DirectorySeparatorChar;
            }

            String[] elements = pathname.Substring(root.Length).Split(Path.DirectorySeparatorChar, Path.AltDirectorySeparatorChar);

            int filenameIndex = elements.GetLength(0) - 1;

            if (elements.GetLength(0) == 1) { // pathname is just a root and filename
                if (elements[0].Length > 5) { // long enough to shorten
                    // if path is a UNC path, root may be rather long
                    if (root.Length + 6 >= maxLength) {
                        return root + elements[0].Substring(0, 3) + "...";

                    } else {
                        return pathname.Substring(0, maxLength - 3) + "...";
                    }
                }

            } else if ((root.Length + 4 + elements[filenameIndex].Length) > maxLength) { // pathname is just a root and filename
                root += "...\\";

                int len = elements[filenameIndex].Length;

                if (len < 6) {
                    return root + elements[filenameIndex];
                }

                if ((root.Length + 6) >= maxLength) {
                    len = 3;

                } else {
                    len = maxLength - root.Length - 3;
                }

                return root + elements[filenameIndex].Substring(0, len) + "...";

            } else if (elements.GetLength(0) == 2) {
                return root + "...\\" + elements[1];

            } else {
                int len = 0;
                int begin = 0;

                for (int i = 0; i < filenameIndex; i++) {
                    if (elements[i].Length > len) {
                        begin = i;
                        len = elements[i].Length;
                    }
                }

                int totalLength = pathname.Length - len + 3;
                int end = begin + 1;

                while (totalLength > maxLength) {
                    if (begin > 0) {
                        totalLength -= elements[--begin].Length - 1;
                    }

                    if (totalLength <= maxLength) {
                        break;
                    }

                    if (end < filenameIndex) {
                        totalLength -= elements[++end].Length - 1;
                    }

                    if (begin == 0 && end == filenameIndex) {
                        break;
                    }
                }

                for (int i = 0; i < begin; i++) {
                    root += elements[i] + '\\';
                }

                root += "...\\";

                for (int i = end; i < filenameIndex; i++) {
                    root += elements[i] + '\\';
                }

                return root + elements[filenameIndex];
            }

            return pathname;
        }

        public void AddFile(String filename, bool saveFullname = true) {
            String pathname = saveFullname ? Path.GetFullPath(filename) : filename;
            AddFile(pathname, ShortenPathname(pathname, MaxShortenPathLength));
        }

        public void AddFile(String filename, String entryname) {
            if (filename == null) {
                throw new ArgumentNullException("filename");
            }

            if (filename.Length == 0) {
                throw new ArgumentException("filename");
            }

            if (numEntries > 0) {
                int index = FindFilenameMenuIndex(filename);

                if (index >= 0) {
                    SetFirstFile(index - StartIndex);
                    return;
                }
            }

            if (numEntries < maxEntries) {
                ToolStripMenuItem menuItem = new ToolStripMenuItem(filename, null, new System.EventHandler(OnClick));
                MenuItems.Insert(StartIndex, menuItem);
                SetFirstFile(menuItem);

                if (numEntries++ == 0) {
                    Enable();
                }

            } else if (numEntries > 1) {
                int index = StartIndex + numEntries - 1;

                if (index < MenuItems.Count) {
                    ToolStripMenuItem menuItem = (ToolStripMenuItem)MenuItems[index];
                    //menuItem.Text = FixupEntryname(0, entryname);
                    menuItem.Text = filename;
                    SetFirstFile(menuItem);
                }
            }
        }

        public void RemoveFile(int number) {
            if (number >= 0 && number < numEntries) {
                if (--numEntries == 0) {
                    Disable();

                } else {
                    int startIndex = StartIndex;

                    if (number == 0) {
                        SetFirstFile(MenuItems[startIndex + 1]);
                    }

                    MenuItems.RemoveAt(startIndex + number);
                }
            }
        }

        public void RemoveFile(String filename) {
            if (numEntries > 0) {
                RemoveFile(FindFilenameNumber(filename));
            }
        }

        public void RemoveAll() {
            if (numEntries > 0) {
                for (int index = EndIndex - 1; index > StartIndex; index--) {
                    MenuItems.RemoveAt(index);
                }

                Disable();
                numEntries = 0;
            }
        }

        public String RegistryKeyName {
            get { return registryKeyName; }
            set
            {
                registryKeyName = value.Trim();

                if (registryKeyName.Length == 0) {
                    registryKeyName = null;
                }
            }
        }

        public void LoadFromRegistry(String keyName) {
            RegistryKeyName = keyName;
            LoadFromRegistry();
        }

        public void LoadFromRegistry() {
            if (registryKeyName != null) {
                RemoveAll();

                RegistryKey regKey = Registry.CurrentUser.OpenSubKey(registryKeyName);

                if (regKey != null) {
                    maxEntries = (int)regKey.GetValue("max", maxEntries);

                    for (int number = maxEntries; number > 0; number--) {
                        String filename = (String)regKey.GetValue("File" + number.ToString());

                        if (filename != null && filename != "") {
                            AddFile(filename, false);
                        }
                    }

                    regKey.Close();
                }
            }
        }

        public void SaveToRegistry(String keyName) {
            RegistryKeyName = keyName;
            SaveToRegistry();
        }

        public void SaveToRegistry() {
            if (registryKeyName != null) {
                RegistryKey regKey = Registry.CurrentUser.CreateSubKey(registryKeyName);

                if (regKey != null) {
                    regKey.SetValue("max", maxEntries);

                    int number = 1;
                    int i = StartIndex;

                    for (; i < EndIndex; i++, number++) {
                        regKey.SetValue("File" + number.ToString(), ((ToolStripMenuItem)MenuItems[i]).Text);
                    }

                    for (; number <= 16; number++) {
                        regKey.DeleteValue("File" + number.ToString(), false);
                    }

                    regKey.Close();
                }
            }
        }
    }
}
