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

#ifndef BEHAVIAC_CORE_SERIALIZABLENODE_H
#define BEHAVIAC_CORE_SERIALIZABLENODE_H

#include "behaviac/base/base.h"
#include "behaviac/base/xml/ixml.h"
#include "behaviac/base/swapbytebase.h"
#include "behaviac/base/swapbytehelpers.h"
#include "behaviac/base/smartptr.h"


template <typename T>
bool FromBinary(const uint8_t* binaryData, T& value)
{
    //BEHAVIAC_ASSERT(0, "please provide your specification!");
    value = *(T*)binaryData;

    return true;
}

template <typename T>
uint8_t* ToBinary(const T& value)
{
    //static T s_binary_temp;

    //BEHAVIAC_ASSERT(0, "please provide your specification!");
    //return (uint8_t*)&s_binary_temp;
    return (uint8_t*)&value;
}

namespace behaviac
{
	class ISerializableNode;

	class CSerializationID
	{
	public:
		CSerializationID() : m_idString(""), m_id()
		{
		}

		// The user must ensure tag is valid for the lifetime of this object
		explicit CSerializationID(const char* idString)
			: m_idString(idString),
			m_id(idString)
		{}

		// The user must ensure tag is valid for the lifetime of this object
		explicit CSerializationID(behaviac::CStringID::IDType crc, const char* idString)
			: m_idString(idString),
			m_id(crc, idString)
		{}

		bool operator==(const CSerializationID& other) const
		{
			return m_id == other.m_id;
		}

		bool operator<(const CSerializationID& other) const
		{
			return m_id < other.m_id;
		}

		bool operator==(const behaviac::CStringID& other) const
		{
			return m_id == other;
		}

		CSerializationID& operator=(const behaviac::CStringID& other)
		{
			m_idString = NULL;
			m_id = other;
			return *this;
		}

		const char* GetString() const
		{
			return m_idString;
		}
		const behaviac::CStringID& GetID() const
		{
			return m_id;
		}

	private:
		const char* m_idString;
		behaviac::CStringID	m_id;
	};

	////////////////////////////////////////////////////////////////////////////////
	// This ref should not be confused with XmlNodeRef in that it should never, EVER
	// be used to keep a reference to a child node in a hierarchy. Supporting that
	// behavior implies lots of allocations and is a major cause of memory
	// fragmentation. ISerializableNode's refcounting is designed to keep a refcount
	// on the top node of a hierarchy (it can be the clone of an other node's child).
	class SerializableNodeRef
	{
	public:
		SerializableNodeRef()
			: m_node(NULL)
		{}
		~SerializableNodeRef();

		SerializableNodeRef(const SerializableNodeRef& node);
		SerializableNodeRef& operator=(const SerializableNodeRef& node);

		template <class TNodeClass, class TParamType>
		TNodeClass* CreateNode(TParamType param)
		{
			TNodeClass* node = BEHAVIAC_NEW TNodeClass(param);
			*this = node;
			return node;
		}

		template <class TNodeClass>
		TNodeClass* CreateNode()
		{
			TNodeClass* node = BEHAVIAC_NEW TNodeClass();
			*this = node;
			return node;
		}

		template <class TNodeClass, class TParamType1, class TParamType2>
		TNodeClass* CreateNode(TParamType1 param1, TParamType2 param2)
		{
			TNodeClass* node = BEHAVIAC_NEW TNodeClass(param1, param2);
			*this = node;
			return node;
		}

		template <class TNodeClass, class TParamType1, class TParamType2, class TParamType3>
		TNodeClass* CreateNode(TParamType1 param1, TParamType2 param2, TParamType3 param3)
		{
			TNodeClass* node = BEHAVIAC_NEW TNodeClass(param1, param2, param3);
			*this = node;
			return node;
		}

		ISerializableNode* GetNode() const
		{
			return m_node;
		}

		ISerializableNode* operator->() const
		{
			return m_node;
		}

		void ReleaseNode()
		{
			*this = NULL;
		}

	private:
		// Only an ISerializableNode may construct a SerializableNodeRef (through
		// clone() for example)
		friend class ISerializableNode;
		SerializableNodeRef(ISerializableNode* node);
		SerializableNodeRef& operator=(ISerializableNode* node);

		ISerializableNode* m_node;
	};

	class BEHAVIAC_API ISerializableNode : private CRefCounted // refcount interface must not be public
	{
	public:
		BEHAVIAC_DECLARE_MEMORY_OPERATORS(ISerializableNode);

		ISerializableNode(bool bText, bool bSwap = true) : m_bText(bText), m_bSwap(bSwap)
		{}

		bool IsText() const
		{
			return this->m_bText;
		}

		virtual ~ISerializableNode() {};

		virtual SerializableNodeRef clone() const = 0;

		virtual int32_t getChildCount() const = 0;
		virtual ISerializableNode* getChild(int32_t childIndex) = 0;
		virtual const ISerializableNode* getChild(int32_t childIndex) const = 0;

		virtual ISerializableNode* findChild(const CSerializationID& childID) = 0;
		virtual const ISerializableNode* findChild(const CSerializationID& childID) const = 0;

		virtual ISerializableNode* newChild(const CSerializationID& childID) = 0;

		virtual void removeChild(ISerializableNode* child) = 0;

		virtual bool isTag(const CSerializationID& tagID) const = 0;
		virtual CSerializationID getTag() const = 0;

		virtual int32_t getAttributesCount() const = 0;

		virtual bool SaveToFile(const char* fileName) const = 0;
		virtual bool LoadFromFile(const char* fileName) = 0;

		virtual bool SaveToFile(IFile* file) const = 0;
		virtual bool LoadFromFile(IFile* file) = 0;

		virtual int32_t GetMemUsage() const = 0;

		/**
		to get the raw value for the specified key to 'valueData', whose type id is typeId, and length is 'length'

		@return the raw value behaviac::string
		*/
		virtual const char* getAttrRaw(const CSerializationID& keyID, int typeId = 0, int length = 0) const
		{
			BEHAVIAC_UNUSED_VAR(keyID);
			BEHAVIAC_UNUSED_VAR(typeId);
			BEHAVIAC_UNUSED_VAR(length);

			BEHAVIAC_ASSERT(0);

			return 0;
		}

		/**
		to set the raw value for the specified key to 'valueData', whose type id is typeId, and length is 'length'
		*/
		virtual void setAttrRaw(const CSerializationID& keyID, const char* valueStr, int typeId = 0, int length = 0)
		{
			BEHAVIAC_UNUSED_VAR(keyID);
			BEHAVIAC_UNUSED_VAR(valueStr);
			BEHAVIAC_UNUSED_VAR(typeId);
			BEHAVIAC_UNUSED_VAR(length);

			BEHAVIAC_ASSERT(0);
		}

		template <class T>
		void setAttr(const CSerializationID& keyID, const T& value)
		{
			int typeId = ::GetClassTypeNumberId<T>();

			if (this->m_bText)
			{
				behaviac::string str = behaviac::StringUtils::ToString(value);
				this->setAttrRaw(keyID, str.c_str(), typeId, sizeof(T));
			}
			else
			{
				uint8_t* binaryData = ToBinary(value);

				if (binaryData)
				{
					if (this->m_bSwap)
					{
						SwapByte(*(T*)binaryData);
					}

					this->setAttrRaw(keyID, (const char*)binaryData, typeId, sizeof(T));
				}
			}
		}

		template <class T>
		bool getAttr(const CSerializationID& keyID, T& value) const
		{
			int typeId = ::GetClassTypeNumberId<T>();

			if (this->m_bText)
			{
				const char* valueStr = this->getAttrRaw(keyID, typeId, sizeof(T));

				if (valueStr)
				{
					if (behaviac::StringUtils::FromString(valueStr, value))
					{
						return true;

					}
					else
					{
						BEHAVIAC_LOGWARNING("Fail to read a (%s) from the value (%s) in the xml attribute (%s) in xml node (%s)\n",
							GetClassTypeName((T*)0),
							(strlen(valueStr) < 64 ? valueStr : behaviac::StringUtils::printf("__too_long_[%u]_to_display__", strlen(valueStr)).c_str()),
							keyID.GetString(), getTag().GetString());
						return false;
					}
				}
			}
			else
			{
				const char* p = this->getAttrRaw(keyID, typeId, sizeof(T));

				if (p)
				{
					bool bOk = FromBinary((uint8_t*)p, value);

					if (bOk && this->m_bSwap)
					{
						SwapByte(value);
					}
				}
			}

			return false;
		}

		// Node
		virtual void addChild(const CSerializationID& keyID, const ISerializableNode* child)
		{
			BEHAVIAC_UNUSED_VAR(keyID);
			BEHAVIAC_UNUSED_VAR(child);
		}

	protected:
		// Only SerializableNodeRef can use the refcounting
		friend class SerializableNodeRef;

		// Only this class can create a SerializableNodeRef so provide the
		// functionality to daughter classes.
		static SerializableNodeRef GetNodeRef(ISerializableNode* node)
		{
			return SerializableNodeRef(node);
		}

		bool	m_bText;
		bool	m_bSwap;
	};

	class SerializableNodeIt
	{
		int32_t m_index;
		ISerializableNode* m_node;

	public:
		BEHAVIAC_DECLARE_MEMORY_OPERATORS(SerializableNodeIt);

		SerializableNodeIt(ISerializableNode* node)
			: m_index(0),
			m_node(node)
		{}
		void reset(ISerializableNode* node)
		{
			m_index = 0;
			m_node = node;
		}
		ISerializableNode* first()
		{
			m_index = 0;
			return next();
		}
		ISerializableNode* first(const CSerializationID& tag)
		{
			m_index = 0;
			return next(tag);
		}

		ISerializableNode* next()
		{
			if (m_node)
			{
				if (m_index < m_node->getChildCount())
				{
					return m_node->getChild(m_index++);
				}
			}

			return NULL;
		}
		ISerializableNode* next(const CSerializationID& tag)
		{
			if (m_node)
			{
				while (m_index < m_node->getChildCount())
				{
					ISerializableNode* node = m_node->getChild(m_index++);

					if (node->isTag(tag))
					{
						return node;
					}
				}
			}

			return NULL;
		}
	};

	class ConstSerializableNodeIt
	{
		int32_t m_index;
		const ISerializableNode* m_node;

	public:
		BEHAVIAC_DECLARE_MEMORY_OPERATORS(ConstSerializableNodeIt);

		ConstSerializableNodeIt(const ISerializableNode* node)
			: m_index(0),
			m_node(node)
		{}
		void reset(const ISerializableNode* node)
		{
			m_index = 0;
			m_node = node;
		}
		const ISerializableNode* first()
		{
			m_index = 0;
			return next();
		}
		const ISerializableNode* first(const CSerializationID& tag)
		{
			m_index = 0;
			return next(tag);
		}

		const ISerializableNode* next()
		{
			if (m_node)
			{
				if (m_index < m_node->getChildCount())
				{
					return m_node->getChild(m_index++);
				}
			}

			return NULL;
		}
		const ISerializableNode* next(const CSerializationID& tag)
		{
			if (m_node)
			{
				while (m_index < m_node->getChildCount())
				{
					const ISerializableNode* node = m_node->getChild(m_index++);

					if (node->isTag(tag))
					{
						return node;
					}
				}
			}

			return NULL;
		}
	};

	inline SerializableNodeRef::~SerializableNodeRef()
	{
		if (m_node)
		{
			m_node->Release();
		}
	}

	inline SerializableNodeRef::SerializableNodeRef(const SerializableNodeRef& node)
		: m_node(node.m_node)
	{
		if (m_node)
		{
			m_node->AddRef();
		}
	}

	inline SerializableNodeRef& SerializableNodeRef::operator=(const SerializableNodeRef& node)
	{
		return *this = node.GetNode();
	}

	inline SerializableNodeRef::SerializableNodeRef(ISerializableNode* node)
		: m_node(node)
	{
		if (m_node)
		{
			m_node->AddRef();
		}
	}

	inline SerializableNodeRef& SerializableNodeRef::operator=(ISerializableNode* node)
	{
		if (node)
		{
			node->AddRef();
		}

		if (m_node)
		{
			m_node->Release();
		}

		m_node = node;
		return *this;
	}
}//namespace behaviac
#endif //BEHAVIAC_CORE_SERIALIZABLENODE_H
