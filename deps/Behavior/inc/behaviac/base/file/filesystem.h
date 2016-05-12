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

#ifndef BEHAVIAC_CORE_FILESYSTEM_NATIVE_H
#define BEHAVIAC_CORE_FILESYSTEM_NATIVE_H

#include "behaviac/base/base.h"
#include "behaviac/base/string/stringutils.h"

#include <vector>
#include <string>

#if BEHAVIAC_COMPILER_MSVC
#define FILE_SYSTEM_INVALID_HANDLE (INVALID_HANDLE_VALUE)
#elif BEHAVIAC_COMPILER_ANDROID && (BEHAVIAC_COMPILER_ANDROID_VER > 8)
#include <android/asset_manager.h>

#define FILE_SYSTEM_INVALID_HANDLE (0)
#else
#define FILE_SYSTEM_INVALID_HANDLE (0)
#endif//#if BEHAVIAC_COMPILER_MSVC

typedef uint64_t FileTime;

// :NOTE: TEMPORARY for Beta: This is used only as a wrapper over behaviac::string <-> wstring conversion functions. In retail/master, we must support unicode,
// while for the release/debug, to avoid problems on the production side (ie. editor), we just support multibyte.
#ifndef _DEBUG
#   define BEHAVIAC_FILE_SYSTEM_UTF8
#endif // _DEBUG

#ifdef BEHAVIAC_FILE_SYSTEM_UTF8
#   define STRING2WSTRING(x) behaviac::StringUtils::Char2Wide(x)
#   define WSTRING2STRING(x) behaviac::StringUtils::Wide2Char(x)
#else
#   define STRING2WSTRING(x) behaviac::StringUtils::Char2Wide(x)
#   define WSTRING2STRING(x) behaviac::StringUtils::Wide2Char(x)
#endif // BEHAVIAC_FILE_SYSTEM_UTF8

namespace behaviac
{
	class IFileSystemVisitor;
	class BEHAVIAC_API CFileSystem
	{
	public:

		static const uint32_t kMAX_PATH = 260;
		// global constants
		static const uint32_t MAX_FILE_PATH_SIZE = (kMAX_PATH);
		static const uint32_t MAX_FILE_SHORT_PATH_SIZE = (13); // 8.3

		typedef void* Handle;

		struct SFileInfo
		{
			FileTime    creationTime;
			FileTime    lastAccessTime;
			FileTime    lastWriteTime;
			uint64_t    fileSize;

			static const uint32_t ATTRIB_DIRECTORY = 0;
			static const uint32_t ATTRIB_FILE = 1;

			uint32_t   fileAttributes; // one of the two above const
			char     fileName[MAX_FILE_PATH_SIZE + 1]; // +1 for 0 terminating
			char     alternFileName[MAX_FILE_SHORT_PATH_SIZE + 1]; // +1 for 0 terminating
		};

		/// File open mode.
		enum EOpenAccess
		{
			EOpenAccess_Invalid = 0,
			EOpenAccess_Read = 1,
			EOpenAccess_Write = 2,
			EOpenAccess_ReadWrite = 4,
			EOpenAccess_WriteAppend = 8
		};

		enum ESeekMoveMode
		{
			ESeekMoveMode_Cur = 0,
			ESeekMoveMode_End = 1,
			ESeekMoveMode_Begin = 2,
			ESeekMoveMode_Set = 3, // for SEEK_SET synonym
			ESeekMoveMode_Force32Bits = 0xFFFFFFFF
		};

		static bool GetFileInfo(const char* inFileName, SFileInfo& fileInfo);

		static bool GetFileInfo(Handle hFile, SFileInfo& fileInfo);

		static Handle OpenCreateFile(const char* szFullPath, EOpenAccess openAccess);

		static void closeFile(Handle file);

		static bool readFile(Handle file,
			void* pBuffer,
			uint32_t nNumberOfBytesToRead,
			uint32_t* pNumberOfBytesRead);

		static bool writeFile(Handle hFile,
			const void* pBuffer,
			uint32_t nNumberOfBytesToWrite,
			uint32_t* pNumberOfBytesWritten);

		static bool copyFile(const char* existingFileName,
			const char* newFileName,
			bool failIfExists);

		static int64_t SetFilePointer(Handle file,
			int64_t distanceToMove,
			ESeekMoveMode moveMethod);

		static bool SetEndOfFile(Handle file); // truncate the file to the current pointer

		static void FlushFile(Handle file);

		static bool FileExist(const char* szFullPath);

		static uint64_t GetFileSize(Handle hFile);

		static bool Delete(const char* szPath, bool bRecursive);

		static bool Move(const char* srcFullPath, const char* destFullPath);

		static bool removeDirectory(const char* directoryPath);   // remove a directory and everything under it. Path must not include a final '\'
		static void MakeSureDirectoryExist(const char* filename); // make dirs in order to be able to save the file

		static void findFiles(const char* searchString,
			behaviac::vector<behaviac::string>& fileList,
			bool giveFiles,
			bool giveDirectories,
			bool recursive = false,
			int maximumSize = 0,
			bool keepCase = false);

		static void Visit
			(
			IFileSystemVisitor&     visitor,
			const char*             pathWithFilter,
			bool                    visitFiles,
			bool                    visitDirectories,
			bool                    recursive
			);

		static void HandleDamageDisk(const char* szFilename);

		// convert the "\" path separator to a platform specific one, and be sure to be lowercase
		static void ConvertPath(const char* szFilePathToConvert, char* szFilePathOut);

		static bool getFileAttributes(const char* szFilename, uint32_t& fileAttributes);
		static bool setFileAttributes(const char* szFilename, uint32_t fileAttributes);

		static bool isFullPath(const char* szFilename);

		static bool IsFileSystemInUTF8()
		{
#ifdef BEHAVIAC_FILE_SYSTEM_UTF8
			return true;
#else
			return false;
#endif
		}

		static void ReadError(Handle);

		static bool StartMonitoringDirectory(const wchar_t* dir);
		static void StopMonitoringDirectory();
		static void GetModifiedFiles(behaviac::vector<behaviac::string>& modifiedFiles);
	};
}

#endif // #ifndef BEHAVIAC_CORE_FILESYSTEM_NATIVE_H
