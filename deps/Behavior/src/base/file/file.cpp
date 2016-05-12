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

#include "behaviac/base/core/config.h"
#include "behaviac/base/core/logging/log.h"

#include "behaviac/base/file/filemanager.h"
#include "behaviac/base/file/file.h"

#include <functional>
#include <algorithm>

namespace behaviac
{
	//////////////////////////////////////////////////////////////////////////
	// File System File
	//////////////////////////////////////////////////////////////////////////

	CPhysicalFile::CPhysicalFile(CFileSystem::Handle handle, bool removableDevice)
		: m_handle(handle)
		, m_isRemovableDevice(removableDevice)
	{
	}

	CPhysicalFile::~CPhysicalFile()
	{
		CFileSystem::closeFile(m_handle);
	}

	uint32_t CPhysicalFile::Read(void* pBuffer, uint32_t numberOfBytesToRead)
	{
		uint32_t numberOfBytesRead;

		if (!CFileSystem::readFile(m_handle, pBuffer, numberOfBytesToRead, &numberOfBytesRead))
		{
			{
				CFileSystem::ReadError(m_handle);
			}
		}

		return numberOfBytesRead;
	}

	uint32_t CPhysicalFile::Read(void* pBuffer, uint32_t offsetOfBytesToRead, uint32_t numberOfBytesToRead)
	{
		CFileSystem::SetFilePointer(m_handle, offsetOfBytesToRead, CFileSystem::ESeekMoveMode_Begin);
		uint32_t numberOfBytesRead;

		if (!CFileSystem::readFile(m_handle, pBuffer, numberOfBytesToRead, &numberOfBytesRead))
		{
			{
				CFileSystem::ReadError(m_handle);
			}
		}

		return numberOfBytesRead;
	}

	uint32_t CPhysicalFile::Write(const void* pBuffer, uint32_t nNumberOfBytesToWrite)
	{
		uint32_t numberOfBytesWritten;
		CFileSystem::writeFile(m_handle, pBuffer, nNumberOfBytesToWrite, &numberOfBytesWritten);
		return numberOfBytesWritten;
	}

	int64_t CPhysicalFile::Seek(int64_t distanceToMove, CFileSystem::ESeekMoveMode moveMethod)
	{
		return CFileSystem::SetFilePointer(m_handle, distanceToMove, moveMethod);
	}

	uint64_t CPhysicalFile::GetSize()
	{
		return CFileSystem::GetFileSize(m_handle);
	}

	bool CPhysicalFile::SetEndOfFile()
	{
		return CFileSystem::SetEndOfFile(m_handle);
	}

	void CPhysicalFile::Flush()
	{
		return CFileSystem::FlushFile(m_handle);
	}

	//////////////////////////////////////////////////////////////////////////
	// Memory Stream File
	//////////////////////////////////////////////////////////////////////////

	CMemoryStreamFile::CMemoryStreamFile() :
		m_offset(0)
	{
	}

	CMemoryStreamFile::~CMemoryStreamFile()
	{
	}

	uint32_t CMemoryStreamFile::Read(void* pBuffer, uint32_t numberOfBytesToRead)
	{
		uint32_t nbBytes = numberOfBytesToRead;

		if (m_offset + nbBytes > m_data.size())
		{
			nbBytes = uint32_t(m_data.size() - m_offset);
		}

		if (nbBytes)
		{
			memcpy((char*)pBuffer, &m_data[0] + m_offset, nbBytes);
			m_offset += nbBytes;
		}

		return nbBytes;
	}

	uint32_t CMemoryStreamFile::Write(const void* pBuffer, uint32_t nNumberOfBytesToWrite)
	{
		uint64_t totalOffset = m_offset + nNumberOfBytesToWrite;

		if (totalOffset > m_data.size())
		{
			// Resize will take care of increasing the capacity to optimize future write calls:
			m_data.resize(uint32_t(totalOffset));
		}

		if (nNumberOfBytesToWrite)
		{
			memcpy(&m_data[0] + m_offset, (const char*)pBuffer, nNumberOfBytesToWrite);
			m_offset = totalOffset;
		}

		return nNumberOfBytesToWrite;
	}

	int64_t CMemoryStreamFile::Seek(int64_t distanceToMove, CFileSystem::ESeekMoveMode moveMethod)
	{
		switch (moveMethod)
		{
		case CFileSystem::ESeekMoveMode_Cur:
		{
			m_offset += distanceToMove;
		}
		break;

		case CFileSystem::ESeekMoveMode_End:
		{
			m_offset = GetSize() + distanceToMove;
		}
		break;

		case CFileSystem::ESeekMoveMode_Begin:
		{
			m_offset = distanceToMove;
		}
		break;

		default:
			BEHAVIAC_ASSERT(false);
			break;
		}

		behaviac::ClampValue(m_offset, uint64_t(0), GetSize());
		return m_offset;
	}

	uint64_t CMemoryStreamFile::GetSize()
	{
		return m_data.size();
	}

	const char* CMemoryStreamFile::GetData()
	{
		return &m_data[0];
	}

	bool CMemoryStreamFile::SetEndOfFile()
	{
		return false;
	}

	//////////////////////////////////////////////////////////////////////////
	// Streamed sequential file System File NO SEEK
	//////////////////////////////////////////////////////////////////////////

	CSequentialWriter::CSequentialWriter(IFile* file, uint32_t bufferSize /*= 4*1024*1024*/) : m_file(file)
	{
		m_filePointer = m_file->Seek(0, CFileSystem::ESeekMoveMode_End);
		m_bufferSize = bufferSize;
		m_buffer = BEHAVIAC_MALLOC_WITHTAG(m_bufferSize, "CSequentialWriter");
		m_bufferUsed = 0;
		m_fastWritingToEnd = true;
	}

	CSequentialWriter::~CSequentialWriter()
	{
		FlushBuffer();
		BEHAVIAC_FREE(m_buffer);
		BEHAVIAC_DELETE(m_file);
	}

	uint32_t CSequentialWriter::Read(void* pBuffer, uint32_t numberOfBytesToRead)
	{
		BEHAVIAC_UNUSED_VAR(pBuffer);
		BEHAVIAC_UNUSED_VAR(numberOfBytesToRead);
		BEHAVIAC_ASSERT(!"no read supported\n");
		return 0;
	}

	void CSequentialWriter::FlushBuffer()
	{
		uint32_t written = m_file->Write(m_buffer, m_bufferUsed);
		BEHAVIAC_UNUSED_VAR(written);
		BEHAVIAC_ASSERT(written == m_bufferUsed);
		m_bufferUsed = 0;
	}

	uint32_t CSequentialWriter::Write(const void* pBuffer, uint32_t nNumberOfBytesToWrite)
	{
		m_filePointer += nNumberOfBytesToWrite;

		if ((m_bufferUsed + nNumberOfBytesToWrite) <= m_bufferSize)
		{
			memcpy(((char*)m_buffer) + m_bufferUsed, pBuffer, nNumberOfBytesToWrite);
			m_bufferUsed += nNumberOfBytesToWrite;
			return nNumberOfBytesToWrite;

		}
		else
		{
			FlushBuffer();

			if (nNumberOfBytesToWrite > m_bufferSize)
			{
				uint32_t written = m_file->Write(pBuffer, nNumberOfBytesToWrite);
				BEHAVIAC_UNUSED_VAR(written);
				return nNumberOfBytesToWrite;

			}
			else
			{
				memcpy(((char*)m_buffer) + m_bufferUsed, pBuffer, nNumberOfBytesToWrite);
				m_bufferUsed += nNumberOfBytesToWrite;
				return nNumberOfBytesToWrite;
			}
		}
	}

	int64_t CSequentialWriter::Seek(int64_t distanceToMove, CFileSystem::ESeekMoveMode moveMethod)
	{
		if ((moveMethod == CFileSystem::ESeekMoveMode_End) && (distanceToMove == 0))
		{
			// optim for archive usage.
			if (!m_fastWritingToEnd)
			{
				FlushBuffer();
				m_filePointer = m_file->Seek(0, CFileSystem::ESeekMoveMode_End);
			}

			m_fastWritingToEnd = true;
			return m_filePointer;
		}

		FlushBuffer();
		m_filePointer = m_file->Seek(distanceToMove, moveMethod);
		return m_filePointer;
	}

	uint64_t CSequentialWriter::GetSize()
	{
		return m_filePointer;
	}

	bool CSequentialWriter::SetEndOfFile()
	{
		FlushBuffer();
		return m_file->SetEndOfFile();
	}

	//////////////////////////////////////////////////////////////////////////
	// Streamed sequential file
	//////////////////////////////////////////////////////////////////////////

	CSequentialReader::CSequentialReader(IFile* file, uint32_t bufferSize)
		: m_file(file),
		m_fileSize(file->GetSize()),
		m_buffer(BEHAVIAC_MALLOC_WITHTAG(bufferSize, "CSequentialReader")),
		m_bufferSize(bufferSize),
		m_bufferPointer(0),
		m_bufferPointerEnd(0)
	{
	}

	CSequentialReader::~CSequentialReader()
	{
		BEHAVIAC_FREE(m_buffer);
		BEHAVIAC_DELETE(m_file);
	}

	uint32_t CSequentialReader::Read(void* pBuffer, uint32_t numberOfBytesToRead)
	{
		uint32_t dataRead = 0;
		uint32_t dataAvailable = (uint32_t)(m_bufferPointerEnd - m_bufferPointer);

		if (dataAvailable > 0)
		{
			// Take what's left in the buffer
			uint32_t dataNeeded = behaviac::Min(dataAvailable, numberOfBytesToRead);
			memcpy(pBuffer, (void*)m_bufferPointer, dataNeeded);
			m_bufferPointer += dataNeeded;
			dataRead += dataNeeded;
			pBuffer = (void*)((char*)pBuffer + dataNeeded);
		}

		if (dataRead < numberOfBytesToRead)
		{
			uint32_t dataNeeded = numberOfBytesToRead - dataRead;

			if (dataNeeded > m_bufferSize)
			{
				// Read directly
				dataRead += m_file->Read(pBuffer, dataNeeded);
			}
			else
			{
				// Fill the buffer and take what we need
				FillBuffer();
				// file read may have failed so check what is available
				uint32_t dataAvailable1 = (uint32_t)(m_bufferPointerEnd - m_bufferPointer);
				dataNeeded = behaviac::Min(dataAvailable1, dataNeeded);
				memcpy(pBuffer, (void*)m_bufferPointer, dataNeeded);
				m_bufferPointer += dataNeeded;
				dataRead += dataNeeded;
			}
		}

		return dataRead;
	}

	void CSequentialReader::FillBuffer()
	{
		uint32_t dataRead = m_file->Read(m_buffer, m_bufferSize);
		m_bufferPointer = (char*)m_buffer;
		m_bufferPointerEnd = m_bufferPointer + dataRead;
	}

	uint32_t CSequentialReader::Write(const void* pBuffer, uint32_t nNumberOfBytesToWrite)
	{
		BEHAVIAC_UNUSED_VAR(pBuffer);
		BEHAVIAC_UNUSED_VAR(nNumberOfBytesToWrite);
		BEHAVIAC_ASSERT(false, "no write supported\n");
		return 0;
	}

	int64_t CSequentialReader::Seek(int64_t distanceToMove, CFileSystem::ESeekMoveMode moveMethod)
	{
		int64_t realOffset = m_file->Seek(0, CFileSystem::ESeekMoveMode_Cur);
		int64_t deltaOffset = -int64_t(m_bufferPointerEnd - m_bufferPointer);

		if (moveMethod == CFileSystem::ESeekMoveMode_Cur)
		{
			if (distanceToMove == 0)
			{
				return realOffset + deltaOffset;
			}

			distanceToMove += deltaOffset;

		}
		else if (moveMethod == CFileSystem::ESeekMoveMode_Begin)
		{
			if (distanceToMove == (realOffset + deltaOffset))
			{
				return distanceToMove;
			}
		}

		// TODO: This could be optimized by recuperating what's still valid in the buffer and only reading what's necessary.
		int64_t result = m_file->Seek(distanceToMove, moveMethod);
		FillBuffer();
		return result;
	}

	uint64_t CSequentialReader::GetSize()
	{
		return m_fileSize;
	}

	bool CSequentialReader::SetEndOfFile()
	{
		return m_file->SetEndOfFile();
	}

	//////////////////////////////////////////////////////////////////////////
	// Memory File
	//////////////////////////////////////////////////////////////////////////
	CMemoryFile::CMemoryFile(IFile* file, CFileSystem::EOpenAccess access) :
		m_file(file),
		m_access(access)
	{
		BEHAVIAC_ASSERT(file);

		if (m_access & (CFileSystem::EOpenAccess_Read | CFileSystem::EOpenAccess_WriteAppend))
		{
			// Copy in memory the entire file:
			uint32_t fileSize = uint32_t(m_file->GetSize());

			if (fileSize > 0)
			{
				m_data.resize(fileSize);
				m_file->Read(&m_data[0], fileSize);
			}
		}

		if (!(m_access & (CFileSystem::EOpenAccess_Write | CFileSystem::EOpenAccess_WriteAppend)))
		{
			// File not needed anymore. Close it now:
			behaviac::CFileManager::GetInstance()->FileClose(m_file);
			m_file = NULL;
		}
	}

	CMemoryFile::~CMemoryFile()
	{
		if (m_file)
		{
			//We assume it is a writable file here
			BEHAVIAC_ASSERT(m_access & (CFileSystem::EOpenAccess_Write | CFileSystem::EOpenAccess_WriteAppend));
			m_file->Seek(0, CFileSystem::ESeekMoveMode_Begin);
			m_file->Write(&m_data[0], uint32_t(GetSize()));

			// If the buffer size is less than the file size, this means that a call to "SetEndOfFile" was made... so we must call it also on the file!
			if (m_data.size() < uint32_t(m_file->GetSize()))
			{
				m_file->SetEndOfFile();
			}

			behaviac::CFileManager::GetInstance()->FileClose(m_file);
			m_file = NULL;
		}
	}

	uint32_t CMemoryFile::Read(void* pBuffer, uint32_t numberOfBytesToRead)
	{
		BEHAVIAC_ASSERT(m_access & CFileSystem::EOpenAccess_Read);
		uint32_t nbBytes = numberOfBytesToRead;

		if (m_offset + nbBytes > GetSize())
		{
			nbBytes = uint32_t(GetSize() - m_offset);
		}

		if (nbBytes)
		{
			memcpy((char*)pBuffer, &m_data[0] + m_offset, nbBytes);
			m_offset += nbBytes;
		}

		return nbBytes;
	}

	uint32_t CMemoryFile::Write(const void* pBuffer, uint32_t nNumberOfBytesToWrite)
	{
		BEHAVIAC_ASSERT(m_access & (CFileSystem::EOpenAccess_Write | CFileSystem::EOpenAccess_WriteAppend));

		if (m_access & CFileSystem::EOpenAccess_WriteAppend)
		{
			// Always put the offset at the end:
			m_offset = GetSize();
		}

		uint64_t totalOffset = m_offset + nNumberOfBytesToWrite;

		if (totalOffset > GetSize())
		{
			// Resize will take care of increasing the capacity to optimize future write calls:
			m_data.resize(uint32_t(totalOffset));
		}

		if (nNumberOfBytesToWrite)
		{
			memcpy(&m_data[0] + m_offset, (const char*)pBuffer, nNumberOfBytesToWrite);
			m_offset = totalOffset;
		}

		return nNumberOfBytesToWrite;
	}

	bool CMemoryFile::SetEndOfFile()
	{
		if ((m_access & CFileSystem::EOpenAccess_Write) && !(m_access & CFileSystem::EOpenAccess_WriteAppend))
		{
			m_data.resize(uint32_t(m_offset));
			return true;

		}
		else
		{
			return false;
		}
	}

	//////////////////////////////////////////////////////////////////////////
	// Memory Buffer File (read-only)
	//////////////////////////////////////////////////////////////////////////
	CMemoryBufferFile::CMemoryBufferFile(const void* buffer, uint64_t bufferSize)
		: m_buffer(buffer),
		m_bufferSize(bufferSize),
		m_offset(0)
	{
	}

	CMemoryBufferFile::~CMemoryBufferFile()
	{
	}

	uint32_t CMemoryBufferFile::Read(void* pBuffer, uint32_t numberOfBytesToRead)
	{
		uint32_t nbBytes = numberOfBytesToRead;

		if (m_offset + nbBytes > GetSize())
		{
			nbBytes = uint32_t(GetSize() - m_offset);
		}

		if (nbBytes)
		{
			memcpy((char*)pBuffer, (char*)m_buffer + m_offset, nbBytes);
			m_offset += nbBytes;
		}

		return nbBytes;
	}

	uint32_t CMemoryBufferFile::Write(const void* pBuffer, uint32_t nNumberOfBytesToWrite)
	{
		BEHAVIAC_UNUSED_VAR(pBuffer);
		BEHAVIAC_UNUSED_VAR(nNumberOfBytesToWrite);
		BEHAVIAC_ASSERT(false, "CMemoryBufferFile is read-only");
		return 0;
	}

	int64_t CMemoryBufferFile::Seek(int64_t distanceToMove, CFileSystem::ESeekMoveMode moveMethod)
	{
		switch (moveMethod)
		{
		case CFileSystem::ESeekMoveMode_Cur:
		{
			m_offset += distanceToMove;
		}
		break;

		case CFileSystem::ESeekMoveMode_End:
		{
			m_offset = GetSize() + distanceToMove;
		}
		break;

		case CFileSystem::ESeekMoveMode_Begin:
		{
			m_offset = distanceToMove;
		}
		break;

		default:
			BEHAVIAC_ASSERT(false);
			break;
		}

		behaviac::ClampValue(m_offset, uint64_t(0), GetSize());
		return m_offset;
	}

	uint64_t CMemoryBufferFile::GetSize()
	{
		return m_bufferSize;
	}

	bool CMemoryBufferFile::SetEndOfFile()
	{
		return false;
	}

	const char* CMemoryBufferFile::GetData()
	{
		return (const char*)m_buffer;
	}

	//****************************************************************************
	inline bool GetLine(IFile* file, behaviac::string& line)
	{
		char byte;
		uint32_t bytesRead = 0;
		bool retVal = true;
		line.resize(0);

		while (1)
		{
			uint32_t byteRead = file->Read(&byte, sizeof(byte));

			if (!byteRead)
			{
				if (bytesRead == 0)
				{
					retVal = false;
				}

				break;

			}
			else
			{
				bytesRead += byteRead;

				if (byte == '\n')
				{
					break;
				}

				line += byte;
			}
		}

		return retVal;
	}
}//namespace behaviac
