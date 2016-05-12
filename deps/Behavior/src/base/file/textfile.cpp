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

#include "behaviac/base/file/textfile.h"
#include "behaviac/base/file/file.h"
#include "behaviac/base/file/filemanager.h"

namespace behaviac
{
	char* LoadTextFileAsBuffer(const char* fileName)
	{
		char* returnedBuffer = NULL;
		IFile* fileHandle = behaviac::CFileManager::GetInstance()->FileOpen(fileName, CFileSystem::EOpenAccess_Read);

		if (fileHandle)
		{
			uint32_t fileSize = (uint32_t)fileHandle->GetSize();
			char* fileBuffer = (char*)BEHAVIAC_MALLOC_WITHTAG(fileSize + 1, "TextFileBuffer");

			if (fileHandle->Read(fileBuffer, fileSize) == fileSize)
			{
				fileBuffer[fileSize] = '\0';
				returnedBuffer = fileBuffer;

			}
			else
			{
				BEHAVIAC_ASSERT(0, "Fail to read the text file : %s\n", fileName);
			}

			behaviac::CFileManager::GetInstance()->FileClose(fileHandle);

		}
		else
		{
			BEHAVIAC_ASSERT(0, "Fail to open the text file : %s\n", fileName);
		}

		return returnedBuffer;
	}

	bool LoadTextFileAsStringArray(const char* fileName, behaviac::vector<behaviac::string>& stringArray)
	{
		char* buffer = LoadTextFileAsBuffer(fileName);

		if (buffer)
		{
			ConvertTextBufferAsStringArray(buffer, stringArray);
			BEHAVIAC_FREE(buffer);
			return true;

		}
		else
		{
			return false;
		}
	}

	void ConvertTextBufferAsStringArray(const char* buffer, behaviac::vector<behaviac::string>& stringArray)
	{
		BEHAVIAC_ASSERT(buffer);
		const char* lineBuffer = buffer;

		while (*lineBuffer != '\0')
		{
			const char* currentPos = lineBuffer;

			while (*currentPos != '\n' && *currentPos != '\r' && *currentPos != '\0')
			{
				currentPos++;
			}

			behaviac::string line;
			line.assign(lineBuffer, currentPos - lineBuffer);
			stringArray.push_back(line);

			while (*currentPos == '\n' || *currentPos == '\r')
			{
				currentPos++;
			}

			lineBuffer = currentPos;
		}
	}
}//namespace behaviac