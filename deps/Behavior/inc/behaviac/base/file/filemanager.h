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

#ifndef BEHAVIAC_CORE_FILEMANAGER_H
#define BEHAVIAC_CORE_FILEMANAGER_H

#include "behaviac/base/base.h"
#include "behaviac/base/core/container/string_t.h"
#include "behaviac/base/core/thread/mutex.h"
#include "behaviac/base/file/filesystem.h"

namespace behaviac
{
	class IFile;
	class CPathID;

	/**
	CFileManager is used to access files.

	CFileManager is used internally as a singleton.
	you can override CFileManager to provide your file manager.
	*/
	class BEHAVIAC_API CFileManager
	{
	private:
		static CFileManager*	ms_pInstance;
		static bool				ms_bCreatedByMe;
		static void _SetInstance(CFileManager* pInstance);
		static CFileManager* _GetInstance();

	public:
		static CFileManager* GetInstance()
		{
			CFileManager* pRandomGenerator = CFileManager::_GetInstance();

			return pRandomGenerator;
		}

		static void Cleanup();
	public:
		virtual IFile* FileOpen(const char* fileName, CFileSystem::EOpenAccess iOpenAccess = CFileSystem::EOpenAccess_Read);

		virtual void FileClose(IFile* file);
		virtual bool FileExists(const char* fileName);
		virtual bool FileExists(behaviac::string filePath, behaviac::string ext);

		virtual uint64_t FileGetSize(const char* fileName);
		virtual behaviac::wstring GetCurrentWorkingDirectory();

		bool PathIsRelative(const behaviac::string& path);
		void FormatPath(const char* fileNameToConvert, char* fileNameOut) const;
		const char* MakeRelativePath(const char* szFullPath, const char* szRootPath);

#if BEHAVIAC_COMPILER_ANDROID && (BEHAVIAC_COMPILER_ANDROID_VER > 8)
		void SetAssetManager(AAssetManager* mgr)
		{
			this->m_mgr = mgr;
		}
		AAssetManager* GetAssetManager()
		{
			return this->m_mgr;
		}
#endif//#if BEHAVIAC_COMPILER_ANDROID && (BEHAVIAC_COMPILER_ANDROID_VER > 8)
	protected:
		BEHAVIAC_DECLARE_MEMORY_OPERATORS(CFileManager);

		CFileManager();
		virtual ~CFileManager();

#if BEHAVIAC_COMPILER_ANDROID && (BEHAVIAC_COMPILER_ANDROID_VER > 8)
		AAssetManager* m_mgr;
#endif//#if BEHAVIAC_COMPILER_ANDROID && (BEHAVIAC_COMPILER_ANDROID_VER > 8)
	};
}//namespace behaviac

#endif // #ifndef BEHAVIAC_CORE_FILEMANAGER_H
