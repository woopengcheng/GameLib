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

#ifndef BEHAVIAC_CORE_FILE_H
#define BEHAVIAC_CORE_FILE_H

#include "behaviac/base/dynamictype.h"
#include "behaviac/base/file/filesystem.h"

#include <vector>

namespace behaviac
{
	//////////////////////////////////////////////////////////////////////////
	// File interface
	//////////////////////////////////////////////////////////////////////////
	class BEHAVIAC_API IFile : public CDynamicType
	{
	public:
		BEHAVIAC_DECLARE_MEMORY_OPERATORS(IFile);
		BEHAVIAC_DECLARE_DYNAMIC_TYPE(IFile, CDynamicType)

			virtual ~IFile() {};

		virtual uint32_t Read(void* pBuffer, uint32_t numberOfBytesToRead) = 0;
		// virtual uint32_t Read(void* pBuffer, uint32_t OffsetOfBytesToRead, uint32_t numberOfBytesToRead) {
		//     BEHAVIAC_UNUSED_VAR(pBuffer);
		//     BEHAVIAC_UNUSED_VAR(OffsetOfBytesToRead);
		//     BEHAVIAC_UNUSED_VAR(numberOfBytesToRead);
		//     return 0;
		// }
		virtual uint32_t	Write(const void* pBuffer, uint32_t numberOfBytesToWrite) = 0;
		virtual int64_t		Seek(int64_t distanceToMove, CFileSystem::ESeekMoveMode moveMethod) = 0;
		virtual uint64_t	GetSize() = 0;
		virtual bool		SetEndOfFile() = 0;
		virtual void		Flush() { };

		template<class T>
		uint32_t Read(T& elem)
		{
			return Read(&elem, sizeof(T));
		}

		template<class T>
		uint32_t Write(const T& elem)
		{
			return Write(&elem, sizeof(T));
		}

		virtual int64_t GetCurrentPosition()
		{
			return Seek(0, CFileSystem::ESeekMoveMode_Cur);
		}
	};

	//////////////////////////////////////////////////////////////////////////
	// Physical File
	//////////////////////////////////////////////////////////////////////////
	class BEHAVIAC_API CPhysicalFile : public IFile
	{
	public:
		BEHAVIAC_DECLARE_MEMORY_OPERATORS(CPhysicalFile);
		BEHAVIAC_DECLARE_DYNAMIC_TYPE(CPhysicalFile, IFile)

	public:
		CPhysicalFile(CFileSystem::Handle handle, bool isRemovableDevice = false);
		virtual ~CPhysicalFile();

		virtual uint32_t	Read(void* pBuffer, uint32_t numberOfBytesToRead);
		virtual uint32_t	Read(void* pBuffer, uint32_t offsetOfBytesToRead, uint32_t numberOfBytesToRead);
		virtual uint32_t	Write(const void* pBuffer, uint32_t numberOfBytesToWrite);
		virtual int64_t		Seek(int64_t distanceToMove, CFileSystem::ESeekMoveMode moveMethod);
		virtual uint64_t	GetSize();
		virtual bool		SetEndOfFile();
		virtual void		Flush();

	protected:
		CFileSystem::Handle     m_handle;
		bool m_isRemovableDevice;
	};

	//////////////////////////////////////////////////////////////////////////
	//  CSequentialFile
	//////////////////////////////////////////////////////////////////////////
	class BEHAVIAC_API CSequentialWriter : public IFile
	{
	public:
		BEHAVIAC_DECLARE_MEMORY_OPERATORS(CSequentialWriter);
		BEHAVIAC_DECLARE_DYNAMIC_TYPE(CSequentialWriter, IFile);

		CSequentialWriter(IFile* file, uint32_t bufferSize = 4 * 1024 * 1024);
		virtual ~CSequentialWriter();

		virtual uint32_t Read(void* pBuffer, uint32_t numberOfBytesToRead);
		virtual uint32_t Write(const void* pBuffer, uint32_t numberOfBytesToWrite);
		virtual int64_t Seek(int64_t distanceToMove, CFileSystem::ESeekMoveMode moveMethod);
		virtual uint64_t GetSize();
		virtual bool SetEndOfFile();

	protected:
		void FlushBuffer();

	private:
		bool					m_fastWritingToEnd;
		IFile*					m_file;
		uint64_t				m_filePointer;

		uint32_t				m_bufferUsed;
		uint32_t				m_bufferSize;
		void*					m_buffer;
	};

	//////////////////////////////////////////////////////////////////////////
	//  CSequentialFile
	//////////////////////////////////////////////////////////////////////////
	class BEHAVIAC_API CSequentialReader : public IFile
	{
	public:
		BEHAVIAC_DECLARE_MEMORY_OPERATORS(CSequentialReader);
		BEHAVIAC_DECLARE_DYNAMIC_TYPE(CSequentialReader, IFile);

		CSequentialReader(IFile* file, uint32_t bufferSize);
		virtual ~CSequentialReader();

		virtual uint32_t Read(void* pBuffer, uint32_t numberOfBytesToRead);
		virtual uint32_t Write(const void* pBuffer, uint32_t numberOfBytesToWrite);
		virtual int64_t Seek(int64_t distanceToMove, CFileSystem::ESeekMoveMode moveMethod);
		virtual uint64_t GetSize();
		virtual bool SetEndOfFile();

	protected:
		void FillBuffer();

	private:
		IFile*					m_file;
		uint64_t				m_fileSize;

		void*					m_buffer;
		uint32_t				m_bufferSize;
		char*   				m_bufferPointer;
		char*                   m_bufferPointerEnd;
	};

	//////////////////////////////////////////////////////////////////////////
	// Memory Stream File
	//////////////////////////////////////////////////////////////////////////
	class BEHAVIAC_API CMemoryStreamFile : public IFile
	{
	public:
		BEHAVIAC_DECLARE_MEMORY_OPERATORS(CMemoryStreamFile);
		BEHAVIAC_DECLARE_DYNAMIC_TYPE(CMemoryStreamFile, IFile)

	public:
		CMemoryStreamFile();
		virtual ~CMemoryStreamFile();

		virtual uint32_t Read(void* pBuffer, uint32_t numberOfBytesToRead);
		virtual uint32_t Write(const void* pBuffer, uint32_t numberOfBytesToWrite);
		virtual int64_t Seek(int64_t distanceToMove, CFileSystem::ESeekMoveMode moveMethod);
		virtual uint64_t GetSize();
		virtual bool SetEndOfFile();

		const char* GetData();

	protected:
		behaviac::vector<char>			m_data;
		uint64_t					m_offset;
	};

	//////////////////////////////////////////////////////////////////////////
	// Memory File
	//////////////////////////////////////////////////////////////////////////
	class BEHAVIAC_API CMemoryFile : public CMemoryStreamFile
	{
	public:
		BEHAVIAC_DECLARE_MEMORY_OPERATORS(CMemoryFile);
		BEHAVIAC_DECLARE_DYNAMIC_TYPE(CMemoryFile, IFile)

	public:
		CMemoryFile(IFile* file, CFileSystem::EOpenAccess access);
		virtual ~CMemoryFile();

		virtual uint32_t Read(void* pBuffer, uint32_t numberOfBytesToRead);
		virtual uint32_t Write(const void* pBuffer, uint32_t numberOfBytesToWrite);
		virtual bool SetEndOfFile();

	private:
		IFile*						m_file;
		CFileSystem::EOpenAccess	m_access;
	};

	//////////////////////////////////////////////////////////////////////////
	// Memory Buffer File (read-only)
	//////////////////////////////////////////////////////////////////////////
	class BEHAVIAC_API CMemoryBufferFile : public IFile
	{
	public:
		BEHAVIAC_DECLARE_MEMORY_OPERATORS(CMemoryBufferFile);
		BEHAVIAC_DECLARE_DYNAMIC_TYPE(CMemoryBufferFile, IFile)

			CMemoryBufferFile(const void* buffer, uint64_t bufferSize);
		virtual ~CMemoryBufferFile();

		virtual uint32_t Read(void* pBuffer, uint32_t numberOfBytesToRead);
		virtual uint32_t Write(const void* pBuffer, uint32_t numberOfBytesToWrite);
		virtual int64_t Seek(int64_t distanceToMove, CFileSystem::ESeekMoveMode moveMethod);
		virtual uint64_t GetSize();
		virtual bool SetEndOfFile();

		const char* GetData();

	private:
		const void*                 m_buffer;
		uint64_t					m_bufferSize;
		uint64_t					m_offset;
	};
}//namespace behaviac

#endif // BEHAVIAC_CORE_FILE_H
