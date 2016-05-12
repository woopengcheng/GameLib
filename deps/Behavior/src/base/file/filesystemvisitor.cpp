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

#include "behaviac/base/file/filesystemvisitor.h"


namespace behaviac
{
	CVectorFileSystemVisitor::CVectorFileSystemVisitor(behaviac::vector< behaviac::string >& vec, bool storeFullPath, uint32_t maximumSize, bool keepCase)
		: m_vec(vec)
		, m_storeFullPath(storeFullPath)
		, m_maximumSize(maximumSize)
		, m_keepCase(keepCase)
	{
	}

	bool CVectorFileSystemVisitor::EnterDirectory(const char* fullDirPath, const char* dirName)
	{
		BEHAVIAC_UNUSED_VAR(dirName);
		BEHAVIAC_UNUSED_VAR(fullDirPath);
		return true;
	}

	bool CVectorFileSystemVisitor::VisitFile(const char* fullFilePath, const char* fileName)
	{
		m_vec.push_back(behaviac::string());
		behaviac::string& str = m_vec.back();
		str = m_storeFullPath ? fullFilePath : fileName;

		if (!m_keepCase)
		{
			str = make_lower(str);
		}

		return m_maximumSize == 0 || m_vec.size() < m_maximumSize;
	}

	bool CVectorFileSystemVisitor::VisitDirectory(const char* fullDirPath, const char* dirName)
	{
		return VisitFile(fullDirPath, dirName);
	}

	void CVectorFileSystemVisitor::ExitDirectory()
	{
	}

	CCounterFileSystemVisitor::CCounterFileSystemVisitor(uint32_t maximumSize)
		: m_maximumSize(maximumSize)
		, m_count(0)
	{
	}

	bool CCounterFileSystemVisitor::EnterDirectory(const char* fullDirPath, const char* dirName)
	{
		BEHAVIAC_UNUSED_VAR(fullDirPath);
		BEHAVIAC_UNUSED_VAR(dirName);

		return true;
	}

	bool CCounterFileSystemVisitor::VisitFile(const char* fullFilePath, const char* fileName)
	{
		BEHAVIAC_UNUSED_VAR(fullFilePath);
		BEHAVIAC_UNUSED_VAR(fileName);

		++m_count;
		return m_maximumSize == 0 || m_count < m_maximumSize;
	}

	bool CCounterFileSystemVisitor::VisitDirectory(const char* fullDirPath, const char* dirName)
	{
		return VisitFile(fullDirPath, dirName);
	}

	void CCounterFileSystemVisitor::ExitDirectory()
	{
	}
}//namespace behaviac