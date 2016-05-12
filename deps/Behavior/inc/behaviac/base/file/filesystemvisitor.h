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

#ifndef BEHAVIAC_CORE_FILESYSTEMVISITOR_H
#define BEHAVIAC_CORE_FILESYSTEMVISITOR_H

#include "behaviac/base/core/config.h"
#include "behaviac/base/core/assert_t.h"
#include "behaviac/base/core/container/string_t.h"
#include "behaviac/base/core/container/vector_t.h"
#include "behaviac/base/core/container/list_t.h"
#include "behaviac/base/core/container/map_t.h"
#include "behaviac/base/core/container/set_t.h"

namespace behaviac
{
	class IFileSystemVisitor
	{
	protected:
		virtual ~IFileSystemVisitor()
		{
		}

	public:
		// full path DOESN'T end with separator
		virtual bool EnterDirectory(const char* fullDirPath, const char* dirName) = 0;
		virtual bool VisitFile(const char* fullFilePath, const char* fileName) = 0;
		// full path DOESN'T end with separator
		virtual bool VisitDirectory(const char* fullDirPath, const char* dirName) = 0;
		virtual void ExitDirectory() = 0;
	};

	class CVectorFileSystemVisitor : public IFileSystemVisitor
	{
	public:
		CVectorFileSystemVisitor(behaviac::vector< behaviac::string >& vec, bool storeFullPath, uint32_t maximumSize, bool keepCase);
		virtual ~CVectorFileSystemVisitor() {}
	public:
		virtual bool EnterDirectory(const char* fullDirPath, const char* dirName);
		virtual bool VisitFile(const char* fullFilePath, const char* fileName);
		virtual bool VisitDirectory(const char* fullDirPath, const char* dirName);
		virtual void ExitDirectory();

	private:
		CVectorFileSystemVisitor(const CVectorFileSystemVisitor& copy);
		CVectorFileSystemVisitor& operator=(const CVectorFileSystemVisitor& copy);

		behaviac::vector< behaviac::string >&   m_vec;
		bool                    m_storeFullPath;
		uint32_t                  m_maximumSize;
		bool                    m_keepCase;
	};

	class CCounterFileSystemVisitor : public IFileSystemVisitor
	{
	public:
		CCounterFileSystemVisitor(uint32_t maximumSize);
		virtual ~CCounterFileSystemVisitor() {}
	public:
		uint32_t GetCount() const
		{
			return m_count;
		}

		virtual bool EnterDirectory(const char* fullDirPath, const char* dirName);
		virtual bool VisitFile(const char* fullFilePath, const char* fileName);
		virtual bool VisitDirectory(const char* fullDirPath, const char* dirName);
		virtual void ExitDirectory();

	private:
		uint32_t m_maximumSize;
		uint32_t m_count;
	};
}

#endif // BEHAVIAC_CORE_FILESYSTEMVISITOR_H
