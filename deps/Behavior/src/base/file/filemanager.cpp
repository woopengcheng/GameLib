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

#include "behaviac/base/file/filemanager.h"
#include "behaviac/base/file/file.h"
#include "behaviac/base/file/filesystem.h"

#include "behaviac/base/string/stringutils.h"
#include "behaviac/base/core/string/formatstring.h"

#include "behaviac/base/core/assert_t.h"

#if BEHAVIAC_COMPILER_MSVC
#include <windows.h>
#endif//#if BEHAVIAC_COMPILER_MSVC

namespace behaviac
{
	bool CFileManager::ms_bCreatedByMe = false;
	CFileManager* CFileManager::ms_pInstance;

	void CFileManager::_SetInstance(CFileManager* pInstance)
	{
		BEHAVIAC_ASSERT(ms_pInstance == 0);
		//if created by me, can't _SetInstance(0)
		BEHAVIAC_ASSERT(!ms_bCreatedByMe);
		CFileManager::ms_pInstance = pInstance;
	}

	CFileManager* CFileManager::_GetInstance()
	{
		CFileManager* pInstance = CFileManager::ms_pInstance;

		if (!pInstance)
		{
			pInstance = BEHAVIAC_NEW CFileManager;
			ms_bCreatedByMe = true;
		}

		return pInstance;
	}

	CFileManager::CFileManager()
	{
#if BEHAVIAC_COMPILER_ANDROID && (BEHAVIAC_COMPILER_ANDROID_VER > 8)
		m_mgr = 0;
#endif//#if BEHAVIAC_COMPILER_ANDROID && (BEHAVIAC_COMPILER_ANDROID_VER > 8)

		CFileManager::_SetInstance(this);
	}

	CFileManager::~CFileManager()
	{
		BEHAVIAC_ASSERT(ms_pInstance == this);
		ms_pInstance = 0;
		ms_bCreatedByMe = false;
	}

	void CFileManager::Cleanup()
	{
		if (ms_pInstance && ms_bCreatedByMe)
		{
			BEHAVIAC_DELETE(ms_pInstance);
		}

		//BEHAVIAC_ASSERT(ms_pInstance == 0);
	}

	IFile* CFileManager::FileOpen(const char* fileName, CFileSystem::EOpenAccess iOpenAccess)
	{
		bool onRemovableDevice = false;

		CFileSystem::Handle hFile = CFileSystem::OpenCreateFile(fileName, iOpenAccess);

		if (hFile != FILE_SYSTEM_INVALID_HANDLE)
		{
			return BEHAVIAC_NEW CPhysicalFile(hFile, onRemovableDevice);
		}
		else
		{
			return NULL;
		}
	}

	void CFileManager::FileClose(IFile* file)
	{
		BEHAVIAC_DELETE(file);
	}
	bool CFileManager::FileExists(behaviac::string filePath, behaviac::string ext)
	{
		behaviac::string strFilename = filePath + ext;
		return this->FileExists(strFilename.c_str());
	}
	bool CFileManager::FileExists(const char* fileName)
	{
		if (CFileSystem::FileExist(fileName))
		{
			return true;
		}

		return false;
	}

	behaviac::wstring CFileManager::GetCurrentWorkingDirectory()
	{
#if BEHAVIAC_COMPILER_MSVC
		wchar_t dir[BEHAVIAC_CFG_FILENAME_MAXLENGTH];

		if (::GetCurrentDirectoryW(BEHAVIAC_CFG_FILENAME_MAXLENGTH, dir))
		{
			return dir;
		}

#endif

		return L"";
	}

	uint64_t CFileManager::FileGetSize(const char* fileName)
	{
		uint64_t fileSize = 0;
		CFileSystem::SFileInfo fileInfo;

		if (CFileSystem::GetFileInfo(fileName, fileInfo))
		{
			fileSize = fileInfo.fileSize;
		}

		return fileSize;
	}

	//
	bool CFileManager::PathIsRelative(const behaviac::string& path)
	{
		//return path.length() > 2 && path[0] == '.' && path[1] == '.' && (path[2] == '/' || path[2] == '\\');
		return path.length() > 2 && path[1] != ':';
	}

	void CFileManager::FormatPath(const char* fileNameToConvert, char* fileNameOut) const
	{
		const char* initialValue = fileNameToConvert;
		BEHAVIAC_UNUSED_VAR(initialValue);

		if (*fileNameToConvert == '\\')
		{
			// If we have a network path, keep the first of the two separators at the beginning instead of skipping it.
			if (*(fileNameToConvert + 1) == '\\')
			{
				*(fileNameOut++) = *(fileNameToConvert++);
				*(fileNameOut++) = *(fileNameToConvert++);

			}
			else
			{
				BEHAVIAC_ASSERT(0, "Absolute path that begins with \"\\\" are not supported.\n%s", initialValue);
				++fileNameToConvert;
			}
		}

		// make the path lower-letters and with forward-slashes
		for (; *fileNameToConvert; ++fileNameToConvert, ++fileNameOut)
		{
			// Skip multiple separator
			while ((*fileNameToConvert == '\\' || *fileNameToConvert == '/') && (*(fileNameToConvert + 1) == '\\' || *(fileNameToConvert + 1) == '/'))
			{
				fileNameToConvert++;
			}

			if (*fileNameToConvert == '/')
			{
				*fileNameOut = '\\';

			}
			else
			{
				*fileNameOut = (char)tolower(*fileNameToConvert);
			}
		}

		*fileNameOut = '\0';
	}

	//This function only substract the root path from the relative path in an non case sentive way
	//The function return NULL if the full path is not in the root path
	const char* CFileManager::MakeRelativePath(const char* szFullPath, const char* szRootPath)
	{
		if (szRootPath)
		{
			while (*szRootPath != '\0')
			{
				if (tolower(*szFullPath) == tolower(*szRootPath))
				{
					szFullPath++;
					szRootPath++;

				}
				else
				{
					return NULL;
				}
			}
		}

		return szFullPath;
	}
}//namespace behaviac