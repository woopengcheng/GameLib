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

#include "./propertynode.h"

namespace behaviac
{
	CPropertyNode::~CPropertyNode()
	{
	}

	SerializableNodeRef CPropertyNode::clone() const
	{
		return GetNodeRef(BEHAVIAC_NEW CPropertyNode(this->m_pAgent, this->m_tag.c_str()));
	}

	int32_t CPropertyNode::getChildCount() const
	{
		return (int32_t)m_children.size();
	}

	ISerializableNode* CPropertyNode::getChild(int32_t childIndex)
	{
		BEHAVIAC_ASSERT(childIndex < getChildCount());
		ChildrenContainer::iterator iter = m_children.begin();
		std::advance(iter, childIndex);
		return &*iter;
	}

	const ISerializableNode* CPropertyNode::getChild(int32_t childIndex) const
	{
		BEHAVIAC_ASSERT(childIndex < getChildCount());
		ChildrenContainer::const_iterator iter = m_children.begin();
		std::advance(iter, childIndex);
		return &*iter;
	}

	ISerializableNode* CPropertyNode::findChild(const CSerializationID& childID)
	{
		{
			ChildrenContainer::iterator iter, end = m_children.end();

			for (iter = m_children.begin(); iter != end; ++iter)
			{
				ISerializableNode* currentChild = &*iter;

				if (currentChild->isTag(childID))
				{
					return currentChild;
				}
			}
		}

		return NULL;
	}

	const ISerializableNode* CPropertyNode::findChild(const CSerializationID& childID) const
	{
		{
			ChildrenContainer::const_iterator iter, end = m_children.end();

			for (iter = m_children.begin(); iter != end; ++iter)
			{
				const ISerializableNode* currentChild = &*iter;

				if (currentChild->isTag(childID))
				{
					return currentChild;
				}
			}
		}

		return NULL;
	}

	CPropertyNode* CPropertyNode::newChild(const CSerializationID& childID)
	{
		behaviac::string tag(this->m_tag);
		tag += "::";
		tag += childID.GetString();

		CPropertyNode newXmlChild(this->m_pAgent, tag.c_str());

		m_children.push_back(newXmlChild);
		return &m_children.back();
	}

	void CPropertyNode::removeChild(ISerializableNode* child)
	{
		{
			ChildrenContainer::iterator iter, end = m_children.end();

			for (iter = m_children.begin(); iter != end; ++iter)
			{
				if (&*iter == child)
				{
					m_children.erase(iter);
					return;
				}
			}
		}
	}

	int32_t CPropertyNode::getAttributesCount() const
	{
		return 0;
	}

	void CPropertyNode::RebuildChildrenList()
	{
	}

	////////////////////////////////////////////////////////////////////////////////
	// set/getAttr type specializations
	////////////////////////////////////////////////////////////////////////////////
	void CPropertyNode::addChild(const CSerializationID& keyID, const ISerializableNode* child)
	{
		BEHAVIAC_UNUSED_VAR(keyID);
		BEHAVIAC_UNUSED_VAR(child);
	}

	void CPropertyNode::addChild(XmlNodeRef xmlChild)
	{
		BEHAVIAC_UNUSED_VAR(xmlChild);
	}

	bool CPropertyNode::LoadFromFile(const char* fileName)
	{
		BEHAVIAC_UNUSED_VAR(fileName);
		return (false);
	}

	bool CPropertyNode::SaveToFile(const char* fileName) const
	{
		BEHAVIAC_UNUSED_VAR(fileName);
		return false;
	}

	bool CPropertyNode::LoadFromFile(IFile* file)
	{
		BEHAVIAC_UNUSED_VAR(file);
		return (false);
	}

	bool CPropertyNode::SaveToFile(IFile* file) const
	{
		BEHAVIAC_UNUSED_VAR(file);
		return false;
	}

	// This is a very unprecise approximation...
	int32_t CPropertyNode::GetMemUsage() const
	{
		int32_t memUsage = sizeof(CPropertyNode);

		{
			ChildrenContainer::const_iterator iter, end = m_children.end();

			for (iter = m_children.begin(); iter != end; ++iter)
			{
				memUsage += iter->GetMemUsage();
			}
		}

		return memUsage;
	}

	const char* CPropertyNode::getAttrRaw(const CSerializationID& keyID, int typeId, int length) const
	{
		BEHAVIAC_UNUSED_VAR(length);

		const CMemberBase* m = this->m_pAgent->FindMember(keyID.GetID());

		if (m)
		{
			CMemberBase* pM = const_cast<CMemberBase*>(m);

			//void* p = pM->Get(this->m_pAgent, typeId);
			void* p = pM->GetVariable(this->m_pAgent, typeId);

			return (const char*)p;
		}

		return 0;
	}

	void CPropertyNode::setAttrRaw(const CSerializationID& keyID, const char* valueData, int typeId, int length)
	{
		BEHAVIAC_UNUSED_VAR(length);

		const CMemberBase* m = this->m_pAgent->FindMember(keyID.GetString());

		if (m)
		{
			CMemberBase* pM = const_cast<CMemberBase*>(m);

			//pM->Set(this->m_pAgent, valueData, typeId);
			pM->SetVariable(this->m_pAgent, valueData, typeId);
		}
	}
}//namespace behaviac