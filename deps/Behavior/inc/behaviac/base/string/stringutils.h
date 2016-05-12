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

#ifndef BEHAVIAC_CORE_STRINGUTILS_H
#define BEHAVIAC_CORE_STRINGUTILS_H

#include "behaviac/base/base.h"
#include "behaviac/base/core/container/string_t.h"
#include "behaviac/base/core/container/vector_t.h"
#include "behaviac/base/convertutf.h"

#include <string>
#include <sstream>

namespace behaviac
{
    namespace StringUtils
    {
#define LOCALE_CN_UTF8 "zh_CN.utf8"

        /// convert multibyte string to wide char string
        BEHAVIAC_API bool MBSToWCS(behaviac::wstring& resultString, const behaviac::string& str, const char* locale = LOCALE_CN_UTF8);

        /// convert multibyte string to wide char string
        BEHAVIAC_API behaviac::wstring MBSToWCS(const behaviac::string& str, const char* locale = LOCALE_CN_UTF8);

        /// convert wide char string to multibyte string
        BEHAVIAC_API bool WCSToMBS(behaviac::string& resultString, const behaviac::wstring& wstr, const char* locale = LOCALE_CN_UTF8);

        /// convert wide char string to multibyte string
        BEHAVIAC_API behaviac::string WCSToMBS(const behaviac::wstring& wstr, const char* locale = LOCALE_CN_UTF8);

        //************************************************************************************
        /**
        * Wide2Char
        *
        * @brief    Convert a wide behaviac::string to a char behaviac::string (multi byte), removing not convertible
        *           chars. This is slow !
        *
        */

        // Wide to UTF-8 conversion routine:
        inline void Wide2Char(behaviac::string& resultString, const behaviac::wstring& wstr)
        {
            //bool bResult = WCSToMBS(resultString, wstr);
            //BEHAVIAC_ASSERT(bResult);
            uint32_t widesize = (uint32_t)wstr.length();
            uint32_t utf8size = 3 * widesize + 1;
            char* buffer = (char*)BEHAVIAC_MALLOC_WITHTAG(utf8size, "Wide2Char");
            memset(buffer, 0, utf8size);

            if (sizeof(wchar_t) == 2)
            {
                const UTF16* sourcestart = reinterpret_cast<const UTF16*>(wstr.c_str());
                const UTF16* sourceend = sourcestart + widesize;
                UTF8* targetstart = reinterpret_cast<UTF8*>(buffer);
                UTF8* targetend = targetstart + utf8size;
                ConvertUTF16toUTF8(&sourcestart, sourceend, &targetstart, targetend, strictConversion);

            }
            else if (sizeof(wchar_t) == 4)
            {
                const UTF32* sourcestart = reinterpret_cast<const UTF32*>(wstr.c_str());
                const UTF32* sourceend = sourcestart + widesize;
                UTF8* targetstart = reinterpret_cast<UTF8*>(buffer);
                UTF8* targetend = targetstart + utf8size;
                ConvertUTF32toUTF8(&sourcestart, sourceend, &targetstart, targetend, strictConversion);

            }
            else
            {
                BEHAVIAC_ASSERT(false);
            }

            resultString = buffer;
            BEHAVIAC_FREE(buffer);
        }

        // Wide to UTF-8 conversion routine:
        inline behaviac::string Wide2Char(const behaviac::wstring& wstr)
        {
            behaviac::string resultString;
            Wide2Char(resultString, wstr);
            return resultString;
        }

        // UTF-8 to Wide conversion routine:
        inline void Char2Wide(behaviac::wstring& resultString, const behaviac::string& str)
        {
            BEHAVIAC_UNUSED_VAR(resultString);
            BEHAVIAC_UNUSED_VAR(str);

            uint32_t strsize = (uint32_t)str.length();
            uint32_t widesize = strsize + 1;
            wchar_t* buffer = (wchar_t*)BEHAVIAC_MALLOC_WITHTAG(widesize * sizeof(wchar_t), "Char2Wide");
            memset(buffer, 0, widesize * sizeof(wchar_t));

            if (sizeof(wchar_t) == 2)
            {
                BEHAVIAC_ASSERT(sizeof(UTF16) == 2);
                const UTF8* sourcestart = reinterpret_cast<const UTF8*>(str.c_str());
                const UTF8* sourceend = sourcestart + strsize;
                UTF16* targetstart = reinterpret_cast<UTF16*>(buffer);
                UTF16* targetend = targetstart + widesize;
				behaviac::ConvertUTF8toUTF16(&sourcestart, sourceend, &targetstart, targetend, strictConversion);
            }
            else if (sizeof(wchar_t) == 4)
            {
                BEHAVIAC_ASSERT(sizeof(UTF32) == 4);
                const UTF8* sourcestart = reinterpret_cast<const UTF8*>(str.c_str());
                const UTF8* sourceend = sourcestart + strsize;
                UTF32* targetstart = reinterpret_cast<UTF32*>(buffer);
                UTF32* targetend = targetstart + widesize;
				behaviac::ConvertUTF8toUTF32(&sourcestart, sourceend, &targetstart, targetend, strictConversion);
            }
            else
            {
                BEHAVIAC_ASSERT(false);
            }

            resultString = buffer;
            BEHAVIAC_FREE(buffer);
        }

        inline behaviac::wstring Char2Wide(const behaviac::string& str)
        {
            behaviac::wstring resultString;
            Char2Wide(resultString, str);
            return resultString;
        }

        // UTF-8 to Wide conversion routine:
        // Watch out this function will allocate memory for buffer, it must
        // be BEHAVIAC_FREEd by the caller
        inline void Char2Wide(const behaviac::string& str, wchar_t*& buffer)
        {
            BEHAVIAC_UNUSED_VAR(str);
            BEHAVIAC_UNUSED_VAR(buffer);

            //This function allocate memory, leaks ?
            BEHAVIAC_ASSERT(buffer == NULL);
            uint32_t strsize = (uint32_t)str.length();
            uint32_t widesize = strsize + 1;
            buffer = (wchar_t*)BEHAVIAC_MALLOC_WITHTAG(widesize * sizeof(wchar_t), "Char2Wide");
            memset(buffer, 0, widesize * sizeof(wchar_t));

            if (sizeof(wchar_t) == 2)
            {
                BEHAVIAC_ASSERT(sizeof(UTF16) == 2);
                const UTF8* sourcestart = reinterpret_cast<const UTF8*>(str.c_str());
                const UTF8* sourceend = sourcestart + strsize;
                UTF16* targetstart = reinterpret_cast<UTF16*>(buffer);
                UTF16* targetend = targetstart + widesize;
				behaviac::ConvertUTF8toUTF16(&sourcestart, sourceend, &targetstart, targetend, strictConversion);
            }
            else if (sizeof(wchar_t) == 4)
            {
                BEHAVIAC_ASSERT(sizeof(UTF32) == 4);
                const UTF8* sourcestart = reinterpret_cast<const UTF8*>(str.c_str());
                const UTF8* sourceend = sourcestart + strsize;
                UTF32* targetstart = reinterpret_cast<UTF32*>(buffer);
                UTF32* targetend = targetstart + widesize;
                behaviac::ConvertUTF8toUTF32(&sourcestart, sourceend, &targetstart, targetend, strictConversion);
            }
            else
            {
                BEHAVIAC_ASSERT(false);
            }
        }

        //////////////////////////////////////////////////
        // Some standard functions

        // converts all chars to lower case
        inline behaviac::string tolower(const behaviac::string& str)
        {
            behaviac::string strResult = make_lower(str);
            return strResult;
        }

        inline behaviac::string toupper(const behaviac::string& str)
        {
            behaviac::string strResult = make_upper(str);
            return strResult;
        }

        // works like strstr, but is case-insensitive
        inline const char* stristr(const char* szString, const char* szSubstring)
        {
            int nSuperstringLength = int(strlen(szString));
            int nSubstringLength = int(strlen(szSubstring));

            for (int nSubstringPos = 0; nSubstringPos <= nSuperstringLength - nSubstringLength; ++nSubstringPos)
            {
                if (string_nicmp(szString + nSubstringPos, szSubstring, nSubstringLength) == 0)
                {
                    return szString + nSubstringPos;
                }
            }

            return NULL;
        }

        // does the same as strstr, but the szString is allowed to be no more than the specified size
        inline const char* strnstr(const char* szString, const char* szSubstring, int nSuperstringLength)
        {
            int nSubstringLength = int(strlen(szSubstring));

            if (!nSubstringLength)
            {
                return szString;
            }

            for (int nSubstringPos = 0; szString[nSubstringPos] && nSubstringPos < nSuperstringLength - nSubstringLength; ++nSubstringPos)
            {
                if (strncmp(szString + nSubstringPos, szSubstring, nSubstringLength) == 0)
                {
                    return szString + nSubstringPos;
                }
            }

            return NULL;
        }

        // calculates the number of characters in the given behaviac::string, limited by the end pointer
        inline int strnlen(const char* szString, const char* szStringEnd)
        {
            const char* p;

            for (p = szString; p < szStringEnd && *p; ++p)
            {
                continue;
            }

            return int(p - szString);
        }

        inline int strlen(const char* szString)
        {
            int n = (int)::strlen(szString);

            return n;
        }

        inline const behaviac::string printf(const char* fmt, ...)
        {
            char tempStr[4096];
            va_list argPtr;
            va_start(argPtr, fmt);
            string_vnprintf(tempStr, 4096, fmt, argPtr);
            va_end(argPtr);
            return behaviac::string(tempStr);
        }
        inline const behaviac::wstring printf(const wchar_t* fmt, ...)
        {
            wchar_t tempStr[4096];
            va_list argPtr;
            va_start(argPtr, fmt);
            string_vnwprintf(tempStr, 4096, fmt, argPtr);
            va_end(argPtr);
            return behaviac::wstring(tempStr);
        }

        /////////////////////////////////////////////
        // General behaviac::string manipulations

        inline bool IsDigit(const char* p, bool allowNegatives = false)
        {
            if (allowNegatives)
            {
                if (p && *p == '-')
                {
                    p++;
                }
            }

            while (p && *p != 0 && *p >= '0' && *p <= '9')
            {
                p++;
            }

            return (p && *p == 0);
        }

        inline void RemoveTrailingSpaces(behaviac::string& str)
        {
            if (!str.empty())
            {
                behaviac::string::size_type ind = str.find_last_not_of(" \t");

                if (ind == behaviac::string::npos)
                {
                    str.clear();

                }
                else
                {
                    str.resize(ind + 1);
                }
            }
        }

        inline void ReplaceWide(wchar_t* stringInOut, const wchar_t* stringToLookFor, wchar_t charToReplace)
        {
            wchar_t* foundSomething = NULL;
            wchar_t* curPos = stringInOut;

            //Loop until the end of the behaviac::string
            while ((foundSomething = wcsstr(curPos, stringToLookFor)) != NULL)
            {
                //Let's replace it
                if (foundSomething)
                {
                    size_t lookForLength = wcslen(stringToLookFor);
                    // Add one for the \0 that we also need to move
                    size_t remainingLength = wcslen(foundSomething) + 1;
                    (*foundSomething) = charToReplace;
                    ++foundSomething;
                    //move everything left one pos
                    memmove(foundSomething, foundSomething + lookForLength - 1, (remainingLength - lookForLength) * sizeof(wchar_t));
                    curPos = foundSomething;
                }
            }
        }

        // Will replace tag contained in sourceStr with value
        // Returns whether tag was found
        template <typename T>
        bool ReplaceTag(behaviac::string& sourceStr, const behaviac::string& tag, T value)
        {
            behaviac::string::size_type i = sourceStr.find(tag.c_str());

            if (i != behaviac::string::npos)
            {
                std::stringstream ss;
                ss << value;
                sourceStr.replace(i, tag.size(), ss.str().c_str());
                return true;
            }

            return false;
        }

        template <typename T>
        bool ReplaceTag(behaviac::wstring& sourceStr, const behaviac::wstring& tag, T value)
        {
            behaviac::wstring::size_type i = sourceStr.find(tag.c_str());

            if (i != behaviac::wstring::npos)
            {
                std::wstringstream ss;
                ss << value;
                sourceStr.replace(i, tag.size(), ss.str().c_str());
                return true;
            }

            return false;
        }

        // returns true if the behaviac::string matches the wildcard
        inline bool MatchWildcard(const char* szString, const char* szWildcard)
        {
            const char* pString = szString, *pWildcard = szWildcard;

            // skip the obviously the same starting substring
            while (*pWildcard && *pWildcard != '*' && *pWildcard != '?')
                if (*pString != *pWildcard)
                {
                    return false;    // must be exact match unless there's a wildcard character in the wildcard behaviac::string

                }
                else
                {
                    ++pString, ++pWildcard;
                }

            if (!*pString)
            {
                // this will only match if there are no non-wild characters in the wildcard
                for (; *pWildcard; ++pWildcard)
                    if (*pWildcard != '*' && *pWildcard != '?')
                    {
                        return false;
                    }

                return true;
            }

            switch (*pWildcard)
            {
                case '\0':
                    return false; // the only way to match them after the leading non-wildcard characters is !*pString, which was already checked

                    // we have a wildcard with wild character at the start.
                case '*':
                {
                    // merge consecutive ? and *, since they are equivalent to a single *
                    while (*pWildcard == '*' || *pWildcard == '?')
                    {
                        ++pWildcard;
                    }

                    if (!*pWildcard)
                    {
                        return true;    // the rest of the behaviac::string doesn't matter: the wildcard ends with *
                    }

                    for (; *pString; ++pString)
                        if (MatchWildcard(pString, pWildcard))
                        {
                            return true;
                        }

                    return false;
                }

                case '?':
                    return MatchWildcard(pString + 1, pWildcard + 1) || MatchWildcard(pString, pWildcard + 1);

                default:
                    BEHAVIAC_ASSERT(0);
                    return false;
            }
        }

        // TContainer is usually behaviac::vector<behaviac::string> but it could be anything
        // where PushBack(behaviac::string) is implemented.
        template <class TContainer>
        inline void SplitIntoArray(const behaviac::string& src, const behaviac::string& delim, TContainer& result)
        {
            behaviac::string tsrc = src;
            behaviac::string::size_type pos = tsrc.find(delim.c_str());
            behaviac::string::size_type length = delim.length();

            while (pos != behaviac::string::npos)
            {
                result.push_back(tsrc.substr(0, pos));
                tsrc = tsrc.substr(pos + length);
                pos = tsrc.find(delim.c_str());
            }

            // filter ending delim without behaviac::string
            if (tsrc.size())
            {
                result.push_back(tsrc);
            }
        }

        inline bool StartsWith(const char* str, const char* token)
        {
            const char* p = strstr(str, token);
            return (p == str);
        }

        /**
        get the behaviac::string before 'sep' in behaviac::string 'params' and store it into 'token'

        @return the pointer after 'token', pointing to 'sep'
        */
        inline const char* FirstToken(const char* params, char sep, behaviac::string& token)
        {
            //const int 5
            const char* end = strchr(params, sep);

            if (end)
            {
                int length = (int)(end - params);
                token = behaviac::string(params, length);
                return end;
            }

            return 0;
        }

        inline const char* SecondeToken(const char* params, char sep, behaviac::string& token)
        {
            //const int 5
            const char* end = strchr(params, sep);

            if (end)
            {
                //skip 'sep'
                end++;

                const char* end2 = strchr(end, sep);

                if (end2)
                {
					int length = (int)(end2 - end);
                    token = behaviac::string(end, length);
                    return end2;

                }
                else
                {
                    //int Agent::Property
                    token = end;
                }
            }

            return 0;
        }

        inline const char* ThirdToken(const char* params, char sep, behaviac::string& token)
        {
            //const int 5
            const char* end = strchr(params, sep);

            if (end)
            {
                //skip 'sep'
                end++;

                const char* end2 = strchr(end, sep);

                if (end2)
                {
                    end2++;
                    token = end2;
                }
            }

            return 0;
        }
        // test the string is valid string
        // if the string is valid then return true
        // else return false
        inline bool IsValidString(behaviac::string str)
        {
            if (str.length() == 0 || (str[0] == '\"' && str[1] == '\"'))
            {
                return false;
            }

            return true;
        }

        inline bool IsValidString(const char* str)
        {
            if ((!str || strlen(str) == 0) || (str[0] == '\"' && str[1] == '\"'))
            {
                return false;
            }

            return true;
        }

        // finds the behaviac::string in the array of strings
        // returns its 0-based index or -1 if not found
        // comparison is case-sensitive
		inline int FindString(const char* szString, const char* arrStringList[], unsigned int arrStrCount)
		{
			if (szString)
			{
				for (unsigned int i = 0; i < arrStrCount; ++i)
				{
					if (0 == strcmp(arrStringList[i], szString))
					{
						return i;
					}
				}
			}

			return -1; // behaviac::string was not found
		}

        /////////////////////////////////////////////
        // File related functions

        // removes the extension from the file path
        inline void StripFileExtension(behaviac::string& strFileName)
        {
            for (const char* p = strFileName.c_str() + strFileName.size() - 1;
                 p >= strFileName.c_str();
                 --p)
            {
                switch (*p)
                {
                    case ':':
                    case '/':
                    case '\\':
                        // we've reached a path separator - it means there's no extension in this name
                        return;

                    case '.':
                        // there's an extension in this file name
                        strFileName.resize(p - strFileName.c_str());
                        return;
                }
            };

            // it seems the file name is a pure name, without path or extension
        }

        // Removes the full extension of a file (ie.: ".meta.inc.xml").
        inline void StripFullFileExtension(behaviac::string& strFileName)
        {
            int dotPos = -1;

            while (true)
            {
                dotPos = (int)strFileName.find('.', dotPos + 1);

                if (dotPos < 0)
                {
                    break;
                }

                if (dotPos != -1 &&
                    dotPos + 1 < (int)strFileName.size() &&
                    strFileName[dotPos + 1] != '/' &&
                    strFileName[dotPos + 1] != '\\')
                {
                    strFileName.resize(dotPos);
                    break;
                }
            }
        }

        // Removes the full extension of a file (ie.: ".meta.inc.xml").
        inline void StripFullFileExtension(const char* in, char* out)
        {
            char c;

            while (*in)
            {
                if (*in == '.')
                {
                    c = in[1];

                    if (c != '.' && c != '/' && c != '\\')
                    {
                        break;
                    }
                }

                *out++ = *in++;
            }

            *out = 0;
        }

        // searches and returns the pointer to the extension of the given file
        inline const char* FindExtension(const char* szFileName)
        {
            const char* szEnd = szFileName + strlen(szFileName);

            for (const char* p = szEnd - 1; p >= szFileName; --p)
            {
                if (*p == '.')
                {
                    return p + 1;

                }
                else if (*p == '/' || *p == '\\')
                {
                    return 0;
                }
            }

            return 0;
        }

        inline const wchar_t* FindExtension(const wchar_t* szFileName)
        {
            const wchar_t* szEnd = szFileName + wcslen(szFileName);

            for (const wchar_t* p = szEnd - 1; p >= szFileName; --p)
            {
                if (*p == '.')
                {
                    return p + 1;

                }
                else if (*p == '/' || *p == '\\')
                {
                    return 0;
                }
            }

            return 0;
        }

        // const version
        inline const char* FindFullExtension(const char* szFileName)
        {
            if (szFileName)
            {
				int slen = (int)strlen(szFileName);
                const char* end = szFileName + slen;
                const char* ptr = end - 1;
                const char* dot = NULL;

                while (ptr != szFileName &&     // Reached beginning of behaviac::string
                       *ptr != '/' &&              // Folder symbol found
                       *ptr != '\\')
                {
                    if (*ptr == '.')
                    {
                        // Remember last dot position found
                        dot = ptr;
                    }

                    --ptr;
                }

                // return ptr after dot or \0 if no ext found (as in original FindExtension funcs above)
                return dot ? dot + 1 : end;
            }

            return szFileName;
        }

        // replaces the full file extension (from the first dot) of the given file name with the new one
        // NOTE: the new extension must NOT be with the dot
        inline void ReplaceFullExtension(behaviac::string& strFileName, const char* szNewExtension)
        {
            //It is assume that the new extension doesn't contain the dot
            BEHAVIAC_ASSERT(szNewExtension);
            BEHAVIAC_ASSERT(strlen(szNewExtension) > 0);
            BEHAVIAC_ASSERT(szNewExtension[0] != '.');
            size_t dot = strFileName.find('.');

            //We have an extension
            if (dot != behaviac::string::npos)
            {
                strFileName.replace(dot + 1, strFileName.size(), szNewExtension);

            }
            else
            {
                strFileName.append(".");
                strFileName.append(szNewExtension);
            }
        }

        // replaces the last file extension (last dot) of the given file name with the new one
        // NOTE: the new extension must NOT be with the dot
        inline void ReplaceExtension(behaviac::string& strFileName, const char* szNewExtension)
        {
            //It is assume that the new extension doesn't contain the dot
            BEHAVIAC_ASSERT(szNewExtension);
            BEHAVIAC_ASSERT(!strchr(szNewExtension, '.'));
            BEHAVIAC_ASSERT(strlen(szNewExtension) > 0);
            size_t dot = strFileName.rfind('.');

            //We have an extension
            if (dot != behaviac::string::npos)
            {
                strFileName.replace(dot + 1, strFileName.size(), szNewExtension);

            }
            else
            {
                strFileName.append(".");
                strFileName.append(szNewExtension);
            }
        }

        inline void ReplaceExtension(char* szFileName, const char* szNewExtension)
        {
            //It is assume that the new extension doesn't contain the dot
            BEHAVIAC_ASSERT(szFileName);
            BEHAVIAC_ASSERT(szNewExtension);
            BEHAVIAC_ASSERT(!strchr(szNewExtension, '.'));
            BEHAVIAC_ASSERT(strlen(szNewExtension) > 0);
            char* dot = strrchr(szFileName, '.');

            //We have an extension
            if (dot)
            {
                string_cpy(dot + 1, szNewExtension);
            }
            else
            {
                strcat((char*)szFileName, ".");
                strcat((char*)szFileName, szNewExtension);
            }
        }

        // returns the parent directory of the given file or directory.
        // the returned path is WITHOUT the trailing slash
        // if the input path has a trailing slash, it's ignored
        // nGeneration - is the number of parents to scan up
        template <class String>
        String GetParentDirectory(const String& strFilePath, int nGeneration = 1)
        {
            BEHAVIAC_ASSERT(strFilePath.size() > 1); // If this blows, find a better thing to do than - 2

            for (const char* p = strFilePath.c_str() + strFilePath.size() - 2; // -2 is for the possible trailing slash: there always must be some trailing symbol which is the file/directory name for which we should get the parent
                 p >= strFilePath.c_str();
                 --p)
            {
                switch (*p)
                {
                    case ':':
                        return String(strFilePath.c_str(), p);
                        break;

                    case '/':
                    case '\\':

                        // we've reached a path separator - return everything before it.
                        if (!--nGeneration)
                        {
                            return String(strFilePath.c_str(), p);
                        }

                        break;
                }
            };

            // it seems the file name is a pure name, without path or extension
            return String();
        }

        // searches and returns the pointer to the file name in the given file path
        inline const char* FindFileNameInPath(const char* szFilePath)
        {
            for (const char* p = szFilePath + strlen(szFilePath) - 1; p >= szFilePath; --p)
                if (*p == '\\' || *p == '/')
                {
                    return p + 1;
                }

            return szFilePath;
        }

        inline behaviac::string CombineDir(const char* path, const char* relative)
        {
            behaviac::string strFullPath;

            //if path hava / or \ in the end
			int len = strlen(path);
			if (path[len - 1] == '/' || path[len - 1] == '\\')
            {
                strFullPath = path;
            }
            else
            {
                strFullPath = path;
                strFullPath += '/';
            }

            //then process the relative path
            if (relative[0] == '/' || relative[0] == '\\')
            {
                const char* _relative = relative + 1;
                strFullPath += _relative;
            }
            else
            {
                strFullPath += relative;
            }

            return strFullPath;
        }

        inline behaviac::wstring CombineDir(const wchar_t* path, const wchar_t* relative)
        {
            behaviac::wstring strFullPath;

            //if path hava / or \ in the end
            if (path[wcslen(path) - 1] == '/' || path[wcslen(path) - 1] == '\\')
            {
                strFullPath = path;

            }
            else
            {
                strFullPath = path;
                strFullPath += '/';
            }

            //then process the relative path
            if (relative[0] == '/' || relative[0] == '\\')
            {
                const wchar_t* _relative = relative + 1;
                strFullPath += _relative;

            }
            else
            {
                strFullPath += relative;
            }

            return strFullPath;
        }

        inline bool Compare(const char* str1, const char* str2, bool bIgnoreCase = true)
        {
            if (bIgnoreCase)
            {
                return string_icmp(str1, str2) == 0;
            }

            return strcmp(str1, str2) == 0;
        }

        inline bool UnifySeparator(behaviac::string& str)
        {
            const char* p = str.c_str();

            char* t = (char*)p;

            while (*t)
            {
                if (*t == '\\')
                {
                    *t = '/';
                }

                ++t;
            }

            return true;
        }
        inline behaviac::string ReadToken(const char* str, int pB, int end)
        {
            behaviac::string strT("");
            int p = pB;

            while (p < end)
            {
                strT += str[p++];
            }

            return strT;
        }
        inline behaviac::vector<behaviac::string> SplitTokens(const char* str)
        {
            behaviac::vector<behaviac::string> ret;
            //"int Self.AgentArrayAccessTest::ListInts[int Self.AgentArrayAccessTest::l_index]"
            int pB = 0;
            int i = 0;

            bool bBeginIndex = false;

            int strLen = StringUtils::strlen(str);

            while (i < strLen)
            {
                bool bFound = false;
                char c = str[i];

                if (c == ' ' && !bBeginIndex)
                {
                    bFound = true;

                }
                else  if (c == '[')
                {
                    bBeginIndex = true;
                    bFound = true;

                }
                else if (c == ']')
                {
                    bBeginIndex = false;
                    bFound = true;
                }

                if (bFound)
                {
                    string strT = ReadToken(str, pB, i);
                    //Debug.Check(strT.length() > 0);
                    BEHAVIAC_ASSERT(strT.length() > 0);
                    ret.push_back(strT);

                    pB = i + 1;
                }

                i++;
            }

            string t = ReadToken(str, pB, i);

            if (t.length() > 0)
            {
                ret.push_back(t);
            }

            return ret;
        }
        inline bool IsNullOrEmpty(const char* str)
        {
            if (str == NULL || str[0] == '\0')
            {
                return true;
            }

            return false;
        }

        inline bool EndsWith(const char* str, const char* suffix)
        {
            if (str == NULL || suffix == NULL)
            {
                return 0;
            }

            size_t str_len = strlen(str);
            size_t suffix_len = strlen(suffix);

            if (suffix_len > str_len)
            {
                return 0;
            }

            return 0 == strncmp(str + str_len - suffix_len, suffix, suffix_len);
        }

		inline const char* StrFind(const char* str1, const char sep)
		{
			return strchr(str1, sep);
		}


        inline bool StrEqual(const char* str1, const char* str2)
        {
            return strcmp(str1, str2) == 0;
        }

        inline bool StrEqualNoCase(const char* str1, const char* str2)
        {
            return string_icmp(str1, str2) == 0;
        }

		inline std::string& StringFormat(std::string& buff, const char* fmt_str, ...)
		{
			size_t n = 256;

			if (buff.size() < n)
			{
				buff.resize(n);
			}
			else
			{
				n = buff.size();
			}

			while (1)
			{
				va_list ap;
				va_start(ap, fmt_str);
				const int final_n = string_vnprintf(&buff[0], n, fmt_str, ap);
				va_end(ap);

				if (final_n < 0) // encoding error
				{
					//n += size_t(-final_n);
					buff = "encoding error";
					break;
				}

				if (static_cast<size_t>(final_n) >= n)
				{
					n += static_cast<size_t>(final_n)-n + 1;
					if (n > 4096) // 
					{
						buff = "string too long, larger then 4096...";
						break;
					}

					buff.resize(n);
				}
				else
				{
					buff[final_n] = '\0';
					buff.resize(final_n);
					break;
				}
			}

			return buff;
		}


    }
}
#endif // #ifndef BEHAVIAC_CORE_STRINGUTILS_H
