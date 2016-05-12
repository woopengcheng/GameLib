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

#ifndef BEHAVIAC_CORE_EXTENSIONCONFIG_H
#define BEHAVIAC_CORE_EXTENSIONCONFIG_H

#include "behaviac/base/core/singleton.h"
#include "behaviac/base/core/string/stringid.h"
#include "behaviac/base/core/rapidxml/rapidxml.hpp"

#include "behaviac/base/base.h"
#include "behaviac/base/xml/ixml.h"

#include <vector>
#include <string>

namespace behaviac
{
	class BEHAVIAC_API CExtensionConfig
	{
	public:
		BEHAVIAC_DECLARE_MEMORY_OPERATORS(CExtensionConfig);

	public:
		struct SConfigInfo
		{
			behaviac::CStringID					m_classID;
			behaviac::string					m_className; // used when creating .dep files, for example
			behaviac::vector<behaviac::string>	m_sourceExt;
			behaviac::string					m_targetExt;
			behaviac::vector<behaviac::string>	m_compileDependencies;

			void LoadFromXML(behaviac::XmlNodeRef&);
		};

		// will be const will return const* in the future
		const SConfigInfo* GetInfoFromSource(const char* target);
		const SConfigInfo* GetInfoFromTarget(const char* source);
		const SConfigInfo* GetInfoFromDependency(const char* file);
		const SConfigInfo* GetInfoFromClassID(behaviac::CStringID classID);

	protected:
		CExtensionConfig();
		~CExtensionConfig();

		void LoadFromXML(const char*);
		void LoadFromXML(behaviac::XmlNodeRef&);

		behaviac::vector<SConfigInfo> m_resInfos;

		BEHAVIAC_DELCARE_SINGLETON(CExtensionConfig);
	};
}//namespace behaviac

#endif // #ifndef BEHAVIAC_CORE_EXTENSIONCONFIG_H
