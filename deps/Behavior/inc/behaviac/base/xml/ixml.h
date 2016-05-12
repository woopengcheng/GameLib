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

#ifndef BEHAVIAC_CORE_IXML_H
#define BEHAVIAC_CORE_IXML_H

#include "behaviac/base/core/string/stringid.h"
#include "behaviac/base/core/string/formatstring.h"
#include "behaviac/base/core/thread/mutex.h"

#include "behaviac/base/base.h"
#include "behaviac/base/swapbyte.h"
#include "behaviac/base/string/stringutils.h"
#include "behaviac/base/string/pathid.h"
#include "behaviac/base/string/valuetostring.h"
#include "behaviac/base/string/valuefromstring.h"
#include "behaviac/base/xml/base64.h"

#include "behaviac/base/dynamictype.h"

#include <vector>
#include <map>

namespace behaviac
{
	// Hash for behaviac::string
	class StringHashCompare
	{
	public:
		enum
		{
			// parameters for hash table
			bucket_size = 4,	// 0 < bucket_size
			min_buckets = 8
		};	// min_buckets = 2 ^^ N, 0 < N

		StringHashCompare()
		{
		}

		size_t operator()(const behaviac::string& keyVal) const
		{
			// hash _Keyval to size_t value by pseudorandomizing transform
			::ldiv_t qrem = ::ldiv((long)stdext::hash_value(keyVal.c_str()), 127773);
			qrem.rem = 16807 * qrem.rem - 2836 * qrem.quot;

			if (qrem.rem < 0)
			{
				qrem.rem += 2147483647;
			}

			return ((size_t)qrem.rem);
		}

		bool operator()(const behaviac::string& keyVal1, const behaviac::string& keyVal2) const
		{
			// test if _Keyval1 ordered before _Keyval2
			return (comp(keyVal1, keyVal2));
		}

	protected:
		std::less<behaviac::string> comp;	// the comparator object
	};

	// Hash for array of char
	class CharPtrHashCompare
	{
	public:
		enum
		{
			// parameters for hash table
			bucket_size = 4,	// 0 < bucket_size
			min_buckets = 8
		};	// min_buckets = 2 ^^ N, 0 < N

		CharPtrHashCompare()
		{
		}

		size_t operator()(const char*  keyVal) const
		{
			// hash _Keyval to size_t value by pseudorandomizing transform
			ldiv_t qrem = ldiv((long)stdext::hash_value(keyVal), 127773);
			qrem.rem = 16807 * qrem.rem - 2836 * qrem.quot;

			if (qrem.rem < 0)
			{
				qrem.rem += 2147483647;
			}

			return ((size_t)qrem.rem);
		}

		bool operator()(const char* keyVal1, const char* keyVal2) const
		{
			return strcmp(keyVal1, keyVal2) < 0;
		}
	};

	class BEHAVIAC_API XmlString
	{
	public:
		XmlString() : m_str(0)
		{}

		XmlString(XmlString const& string);
		XmlString(char const* string);
		~XmlString();

		void operator=(XmlString const& string);

		char const* c_str() const
		{
			return m_str;
		}

	private:
		void Insert(char const* string);
		void Remove();

		typedef behaviac::map<char const*, int32_t, CharPtrHashCompare> TableType;
		static behaviac::Mutex ms_critSection;
		static TableType m_table;

		char const* m_str;
	};

	/**
	******************************************************************************
	* XmlAttribute class
	******************************************************************************
	*/

	class XmlAttribute
	{
	public:
		BEHAVIAC_DECLARE_MEMORY_OPERATORS(XmlAttribute);

		XmlAttribute() : m_wide(false) {}
		XmlAttribute(const char* k, const char* v) : m_key(k), m_value(v), m_wide(false) {}
		explicit XmlAttribute(const char* k) : m_key(k), m_wide(false) {}

		bool operator<(const XmlAttribute& attr) const
		{
			return string_icmp(m_key.c_str(), attr.m_key.c_str()) < 0;
		}

		bool operator==(const XmlAttribute& attr) const
		{
			return string_icmp(m_key.c_str(), attr.m_key.c_str()) == 0;
		}

		bool operator==(const char* sKey) const
		{
			return string_icmp(m_key.c_str(), sKey) == 0;
		}

		inline void SetKey(const char* key)
		{
			m_key = key;
		}
		inline void SetValue(const char* value)
		{
			m_value = value;
		}
		inline const char* GetKey() const
		{
			return m_key.c_str();
		}
		inline const char* GetValue() const
		{
			return m_value.c_str();
		}

		inline bool IsWide() const
		{
			return m_wide;
		}
		inline void SetValue(const wchar_t* value)
		{
			m_wide = true;
			m_value_w = value;
		}
		inline const wchar_t* GetValueWide() const
		{
			return m_value_w.c_str();
		}
	protected:
		XmlString m_key;
		XmlString m_value;

		bool		m_wide;
		behaviac::wstring m_value_w;
	};

	class IXmlNode;

	//! Xml node attributes class.
	typedef behaviac::vector<XmlAttribute>	XmlAttributes;

	/**
	******************************************************************************
	* XmlNodeRef, wrapper class implementing reference counting for IXmlNode.
	******************************************************************************
	*/
	class XmlNodeRef
	{
	private:
		IXmlNode* p;
	public:
		BEHAVIAC_DECLARE_MEMORY_OPERATORS(XmlNodeRef);

		XmlNodeRef() : p(NULL) {}
		XmlNodeRef(IXmlNode* p_);
		XmlNodeRef(const behaviac::XmlNodeRef& p_);
		~XmlNodeRef();

		operator IXmlNode* () const
		{
			return p;
		}
		IXmlNode& operator*() const
		{
			return *p;
		}
		IXmlNode* operator->(void) const
		{
			return p;
		}

		XmlNodeRef&  operator=(IXmlNode* newp);
		XmlNodeRef&  operator=(const behaviac::XmlNodeRef& newp);

		friend class XmlConstNodeRef;
	};

	class XmlConstNodeRef
	{
	private:
		const IXmlNode* p; // not const as we need to call addref/decref on it. But anyway this class is only returning const items
	public:
		BEHAVIAC_DECLARE_MEMORY_OPERATORS(XmlConstNodeRef);

		XmlConstNodeRef() : p(NULL) {}
		XmlConstNodeRef(const IXmlNode* p_);
		XmlConstNodeRef(const XmlConstNodeRef& p_);
		XmlConstNodeRef(const behaviac::XmlNodeRef& p_);
		~XmlConstNodeRef();

		operator const IXmlNode* () const
		{
			return p;
		}
		const IXmlNode& operator*() const
		{
			return *p;
		}
		const IXmlNode* operator->(void) const
		{
			return p;
		}

		XmlConstNodeRef&  operator=(const IXmlNode* newp);
		XmlConstNodeRef&  operator=(const XmlConstNodeRef& newp);
		XmlConstNodeRef&  operator=(const behaviac::XmlNodeRef& newp);
	};

	/**
	******************************************************************************
	* IXmlNode class
	* Never use IXmlNode directly instead use reference counted XmlNodeRef.
	******************************************************************************
	*/
	class IFile;
	class IXmlNode
	{
	public:
		BEHAVIAC_DECLARE_MEMORY_OPERATORS(IXmlNode);

		virtual ~IXmlNode() {};

		//////////////////////////////////////////////////////////////////////////
		//! Reference counting.
		virtual void AddRef() const = 0;
		//! When ref count reach zero XML node dies.
		virtual void Release() const = 0;

		//////////////////////////////////////////////////////////////////////////
		//! Get XML node tag.
		virtual const char* getTag() const = 0;
		virtual void setTag(const char* tag) = 0;

		//! Return true if given tag equal to node tag.
		virtual bool isTag(const char* tag) const = 0;

		//! Copy attributes to this node from givven node.
		virtual void copyAttributes(XmlConstNodeRef fromNode) = 0;

		//! Get XML Node attribute for specified key.
		virtual const char* getAttr(const char* key) const = 0;

		//! Get the number of attribute
		virtual int	getAttrCount() const = 0;

		//! Get an attribute value with his index
		virtual const char* getAttr(int index) const = 0;
		virtual const char* getAttrTag(int index) const = 0;

		//! Check if attributes with specified key exist.
		virtual bool haveAttr(const char* key) const = 0;

		//! Adds new child node.
		virtual void addChild(XmlNodeRef node) = 0;

		//! Creates new xml node and add it to childs list.
		virtual XmlNodeRef newChild(const char* tagName) = 0;

		//! Remove child node.
		virtual void removeChild(XmlNodeRef node) = 0;

		//! Remove all child nodes.
		virtual void removeAllChilds() = 0;

		//! Swap child nodes
		virtual void swapChilds(int child1, int child2) = 0;

		//! Get number of child XML nodes.
		virtual int	getChildCount() const = 0;
		virtual int	getChildCount(const char* tag) const = 0;

		//! Get XML Node child nodes.
		virtual XmlNodeRef getChild(int i) = 0;
		virtual XmlConstNodeRef getChild(int i) const = 0;

		//! Find node with specified tag.
		virtual XmlNodeRef findChild(const char* tag) = 0;
		virtual XmlConstNodeRef findChild(const char* tag) const = 0;

		//! Find node with specified tag.
		// The non-const version create it if not found, and the const one return
		// the constant invalid node
		virtual XmlNodeRef findChildSafe(const char* tag) = 0;
		virtual XmlConstNodeRef findChildSafe(const char* tag) const = 0;

		//! Returns content of this node.
		virtual const char* getContent() const = 0;

		//! Replaces content of this node
		virtual void setContent(const char* str) = 0;

		//! Replaces content of this node, with the optional ability to simply transfer ownership of the buffer to avoid any allocation/copy (the goal is to improve parsing performance)
		virtual void transferContent(behaviac::string& newContent) = 0;

		//! Deep clone of this and all child xml nodes.
		virtual XmlNodeRef clone() const = 0;

#ifdef _DEBUG
		//! Returns line number for XML tag.
		virtual int getLine() const = 0;
		//! Set line number in xml.
		virtual void setLine(int line) = 0;
#endif

		//! Returns XML of this node and sub nodes.
		virtual void getXML(behaviac::string& xml, int level = 0) const = 0;
		virtual void getXML(behaviac::wstring& xml, int level = 0) const = 0;

		virtual bool saveToFile(const char* fileName) const = 0;
		virtual bool saveToFile(IFile* file) const = 0;

		virtual void ReserveAttr(int nCount) = 0;

		//! Set XML attribute methods
		virtual void setAttrText(const char* key, const char* text) = 0;
		virtual void setAttrText(const char* key, const wchar_t* text) = 0;
		void setAttr(const char* key, const char* value)
		{
			setAttrText(key, value);
		}
		void setAttr(const char* key, char* value)
		{
			setAttrText(key, value);
		}
		void setAttr(const char* key, const behaviac::wstring& value)
		{
			setAttrText(key, value.c_str());
		}
		template <typename T> void setAttr(const char* key, const T& value)
		{
			behaviac::string str = behaviac::StringUtils::ToString(value);
			setAttrText(key, str.c_str());
		}
		template <class T> void setAttrStringIDKind(const char* key, const T& value)
		{
			// We never need the crc if we have the behaviac::string
			// We only save the crc when the behaviac::string is not available (and use a behaviac::string signature to know if it's a behaviac::string or a crc)
			// We always use a single attribute with the same name, to avoid xml merging conflict
			// Xml is not intended to be use at runtime in the game, so we focus here on simplicity and not on performance
#if STRINGID_USESTRINGCONTENT
			setAttrText(key, value.c_str());
#else
			char crcValue[11];
			string_sprintf(crcValue, "%%$%08X", value.GetUniqueID());
			setAttr(key, crcValue);
#endif // #if STRINGID_USESTRINGCONTENT
		}

		//! Value processed as hexadecimal
		inline void setAttr_Hex(const char* key, int32_t& value)
		{
			char str[11];
			string_sprintf(str, "0x%08x", value);
			setAttrText(key, str);
		}
#if !BEHAVIAC_COMPILER_64BITS
		inline void setAttr_Hex(const char* key, int64_t& value)
		{
			char str[19];
			string_sprintf(str, "0x%16llx", value);
			setAttrText(key, str);
		}

		inline void setAttr_Hex(const char* key, uint64_t& value)
		{
			char str[19];
			string_sprintf(str, "0x%16llx", value);
			setAttrText(key, str);
		}
#else
		inline void setAttr_Hex(const char* key, long long& value)
		{
			char str[19];
			string_sprintf(str, "0x%16llx", value);
			setAttrText(key, str);
		}
#endif
		//! Value processed as int32_t to avoid precision loss
		inline void setAttrPrecise(const char* key, const float& value)
		{
			union
			{
				float f;
				int32_t p;
			} temp;
			temp.f = value;
			setAttr(key, temp.p);
		}

		//! Encode the memory representation of these types in Base64
		template <class T> void setAttrEncode64(const char* key, const behaviac::vector<T>& value);
		template <class T> void setAttrEncode64(const char* key, const T& value);

		//!	Generic version (for enums).
		template <class T>
		void setAttrGeneric(const char* key, T& value)
		{
			setAttr(key, (int)value);
		}
		//////////////////////////////////////////////////////////////////////////

		//! Delete attribute.
		virtual void delAttr(const char* key) = 0;
		//! Remove all node attributes.
		virtual void removeAllAttributes() = 0;

		//! Get XML attribute methods
		template <typename T> bool getAttr(const char* key, T& value) const
		{
			const char* str = getAttr(key);

			if (str)
			{
				if (behaviac::StringUtils::FromString(str, value))
				{
					return true;

				}
				else
				{
					BEHAVIAC_LOGWARNING("Fail to read a (%s) from the value (%s) in the xml attribute (%s) in xml node (%s)\n",
						GetClassTypeName((T*)0),
						strlen(str) < 64 ? str : behaviac::StringUtils::printf("__too_long_[%u]_to_display__", strlen(str)).c_str(),
						key, getTag());
					return false;
				}
			}

			return false;
		}

		template <bool> bool getAttr(const char* key, bool& value) const
		{
			const char* str = getAttr(key);

			if (str)
			{
				if (behaviac::StringUtils::FromString(str, value))
				{
					return true;

				}
				else
				{
					BEHAVIAC_LOGWARNING("Fail to read a (%s) from the value (%s) in the xml attribute (%s) in xml node (%s)\n",
						GetClassTypeName((bool*)0),
						strlen(str) < 64 ? str : behaviac::StringUtils::printf("__too_long_[%u]_to_display__", strlen(str)).c_str(),
						key, getTag());
					return false;
				}
			}

			return false;
		}

		template <class T> bool getAttrStringIDKind(const char* key, T& value) const
		{
			// See "setAttrStringIDKind" for format details
			const char* textVal = getAttr(key);

			if (textVal)
			{
				if (textVal[0] == '%' && textVal[1] == '$')
				{
					int32_t crcValue;

					if (sscanf(textVal + 2, "%08X", &crcValue) == 1)
					{
						value = T(crcValue);

					}
					else
					{
						return false;
					}
				}
				else
				{
					value.SetContent(textVal);
				}

				return true;
			}

			return false;
		}

		//!	Generic version (for enums).
		template <class T>
		bool getAttrGeneric(const char* key, T& value) const
		{
			int	val;

			if (!getAttr(key, val))
			{
				return	false;
			}

			value = (T)val;
			return	true;
		}

		//! Value processed as hexadecimal
		inline bool getAttr_Hex(const char* key, int32_t& value) const
		{
			const char* str = getAttr(key);

			if (str && *str)
			{
				if (strncmp(str, "0x", 2) == 0 && sscanf(str, "0x%08x", &value) == 1)
				{
					return true;

				}
				else
				{
					BEHAVIAC_LOGWARNING("Fail to read a (hexa U32) from the value (%s) in the xml attribute (%s) in xml node (%s)\n", str, key, getTag());
				}
			}

			return false;
		}
#if !BEHAVIAC_COMPILER_64BITS
		inline bool getAttr_Hex(const char* key, int64_t& value) const
		{
			const char* str = getAttr(key);

			if (str && *str)
			{
				if (strncmp(str, "0x", 2) == 0 && sscanf(str, "0x%16llx", &value) == 1)
				{
					return true;

				}
				else
				{
					BEHAVIAC_LOGWARNING("Fail to read a (hexa U64) from the value (%s) in the xml attribute (%s) in xml node (%s)\n", str, key, getTag());
				}
			}

			return false;
		}

		inline bool getAttr_Hex(const char* key, uint64_t& value) const
		{
			const char* str = getAttr(key);

			if (str && *str)
			{
				if (strncmp(str, "0x", 2) == 0 && sscanf(str, "0x%16llx", &value) == 1)
				{
					return true;

				}
				else
				{
					BEHAVIAC_LOGWARNING("Fail to read a (hexa U64) from the value (%s) in the xml attribute (%s) in xml node (%s)\n", str, key, getTag());
				}
			}

			return false;
		}
#else
		inline bool getAttr_Hex(const char* key, long long& value) const
		{
			const char* str = getAttr(key);

			if (str && *str)
			{
				if (strncmp(str, "0x", 2) == 0 && sscanf(str, "0x%16llx", &value) == 1)
				{
					return true;

				}
				else
				{
					BEHAVIAC_LOGWARNING("Fail to read a (hexa U64) from the value (%s) in the xml attribute (%s) in xml node (%s)\n", str, key, getTag());
				}
			}

			return false;
		}

		inline bool getAttr_Hex(const char* key, unsigned long long& value) const
		{
			const char* str = getAttr(key);

			if (str && *str)
			{
				if (strncmp(str, "0x", 2) == 0 && sscanf(str, "0x%16llx", &value) == 1)
				{
					return true;

				}
				else
				{
					BEHAVIAC_LOGWARNING("Fail to read a (hexa U64) from the value (%s) in the xml attribute (%s) in xml node (%s)\n", str, key, getTag());
				}
			}

			return false;
		}

#endif
		//! Assign a default value if the param is missing
		template<typename T>
		bool getAttr(const char* key, T& value, const T& defaultValue) const
		{
			if (!getAttr(key, value))
			{
				value = defaultValue;
				return false;
			}

			return true;
		}

		//! Value processed as int32_t to avoid precision loss
		inline bool getAttrPrecise(const char* key, float& value) const
		{
			union
			{
				float f;
				int32_t p;
			} temp;

			if (getAttr(key, temp.p))
			{
				value = temp.f;
				return true;
			}

			return false;
		}

		//! Decode the Base64 memory dump and put it back in these types
		template <class T> bool getAttrEncode64(const char* key, behaviac::vector<T>& value) const;
		template <class T> bool getAttrEncode64(const char* key, T& value) const;

		//! Lets be friendly to him.
		friend class XmlNodeRef;
		friend class XmlConstNodeRef;
	};

	template< class T >
	void IXmlNode::setAttrEncode64(const char* key, const behaviac::vector<T>& value)
	{
		int nbInt = static_cast<int>(value.size());
		int sizeInBytes = nbInt * sizeof(typename behaviac::vector<T>::value_type);
		char buf[32] = { 0 };
		string_sprintf(buf, "Count(%d) ", nbInt);
		behaviac::string str;
		//+15 is approx for "Count(%d) "
		int32_t buffSize = XmlBase64::GetEncodedBuffSize(sizeInBytes + 15);
		str.reserve(buffSize);
		str = buf;
		int8_t* outBuff;
		XmlBase64::EncodeBuff((const int8_t*)value.data(), outBuff, sizeInBytes);
		str += (const char*)outBuff;
		BEHAVIAC_FREE(outBuff);
		setAttr(key, str.c_str());
	}

	template< class T >
	bool IXmlNode::getAttrEncode64(const char* key, behaviac::vector<T>& value) const
	{
		const char* str = getAttr(key);

		if (str && *str)
		{
			value.clear();
			int count = 0;

			if (strncmp(str, "Count", 5) == 0)
			{
				sscanf(str, "Count(%d)", &count);
				value.resize(count);
				str = strchr(str, ')');
				str += 2;
			}

			int8_t* outBuff;
			XmlBase64::DecodeBuff((const int8_t*)str, outBuff);
			SwapByteArray((typename behaviac::vector<T>::value_type*)outBuff, count);
			memcpy(&value[0], outBuff, count * sizeof(typename behaviac::vector<T>::value_type));
			BEHAVIAC_FREE(outBuff);
			return true;
		}

		return false;
	}

	template< class T >
	void IXmlNode::setAttrEncode64(const char* key, const T& value)
	{
		int sizeInBytes = sizeof(T);
		behaviac::string str;
		int32_t buffSize = XmlBase64::GetEncodedBuffSize(sizeInBytes);
		str.reserve(buffSize);
		int8_t* outBuff;
		XmlBase64::EncodeBuff((const int8_t*)&value, outBuff, sizeInBytes);
		str += (const char*)outBuff;
		BEHAVIAC_FREE(outBuff);
		setAttr(key, str.c_str());
	}

	template< class T >
	bool IXmlNode::getAttrEncode64(const char* key, T& value) const
	{
		const char* str = getAttr(key);

		if (str && *str)
		{
			int8_t* outBuff;
			XmlBase64::DecodeBuff((const int8_t*)str, outBuff);
			SwapByte(*(T*)outBuff);
			memcpy(&value, outBuff, sizeof(T));
			BEHAVIAC_FREE(outBuff);
			return true;
		}

		return false;
	}

	/************************************************************************/
	/* CXmlNodeIt class, iterate through children							*/
	/************************************************************************/
	class XmlNodeIt
	{
		int m_Idx;
		XmlNodeRef m_node;

	public:
		BEHAVIAC_DECLARE_MEMORY_OPERATORS(XmlNodeIt);

		XmlNodeIt(const behaviac::XmlNodeRef& node)
		{
			reset(node);
		}
		void reset(const behaviac::XmlNodeRef& node)
		{
			m_Idx = 0;
			m_node = node;
		}
		XmlNodeRef first()
		{
			m_Idx = 0;
			return next();
		}
		XmlNodeRef first(const char* tag)
		{
			m_Idx = 0;
			return next(tag);
		}

		XmlNodeRef next()
		{
			if (!m_node)
			{
				return XmlNodeRef(0);
			}

			if (m_Idx < m_node->getChildCount())
			{
				return m_node->getChild(m_Idx++);
			}

			return XmlNodeRef(0);
		}
		XmlNodeRef next(const char* tag)
		{
			if (!m_node)
			{
				return XmlNodeRef(0);
			}

			while (m_Idx < m_node->getChildCount())
			{
				XmlNodeRef node = m_node->getChild(m_Idx++);

				if (node->isTag(tag))
				{
					return node;
				}
			}

			return XmlNodeRef(0);
		}
	};

	class XmlConstNodeIt
	{
		int m_Idx;
		XmlConstNodeRef m_node;

	public:
		BEHAVIAC_DECLARE_MEMORY_OPERATORS(XmlConstNodeIt);

		XmlConstNodeIt(const XmlConstNodeRef& node)
		{
			reset(node);
		}
		void reset(const XmlConstNodeRef& node)
		{
			m_Idx = 0;
			m_node = node;
		}
		XmlConstNodeRef first()
		{
			m_Idx = 0;
			return next();
		}
		XmlConstNodeRef first(const char* tag)
		{
			m_Idx = 0;
			return next(tag);
		}

		XmlConstNodeRef next()
		{
			if (!m_node)
			{
				return XmlConstNodeRef(0);
			}

			if (m_Idx < m_node->getChildCount())
			{
				return m_node->getChild(m_Idx++);
			}

			return XmlConstNodeRef(0);
		}
		XmlConstNodeRef next(const char* tag)
		{
			if (!m_node)
			{
				return XmlConstNodeRef(0);
			}

			while (m_Idx < m_node->getChildCount())
			{
				XmlConstNodeRef node = m_node->getChild(m_Idx++);

				if (node->isTag(tag))
				{
					return node;
				}
			}

			return XmlConstNodeRef(0);
		}
	};

	//////////////////////////////////////////////////////////////////////////

	inline XmlNodeRef::XmlNodeRef(IXmlNode* p_) : p(p_)
	{
		if (p)
		{
			p->AddRef();
		}
	}

	inline XmlNodeRef::XmlNodeRef(const behaviac::XmlNodeRef& p_) : p(p_.p)
	{
		if (p)
		{
			p->AddRef();
		}
	}

	inline XmlNodeRef::~XmlNodeRef()
	{
		if (p)
		{
			p->Release();
		}
	}

	inline behaviac::XmlNodeRef&  XmlNodeRef::operator=(IXmlNode* newp)
	{
		if (newp)
		{
			newp->AddRef();
		}

		if (p)
		{
			p->Release();
		}

		p = newp;
		return *this;
	}

	inline behaviac::XmlNodeRef&  XmlNodeRef::operator=(const behaviac::XmlNodeRef& newp)
	{
		if (newp.p)
		{
			newp.p->AddRef();
		}

		if (p)
		{
			p->Release();
		}

		p = newp.p;
		return *this;
	}

	// const Node Ref
	inline XmlConstNodeRef::XmlConstNodeRef(const IXmlNode* p_) : p(p_)
	{
		if (p)
		{
			p->AddRef();
		}
	}

	inline XmlConstNodeRef::XmlConstNodeRef(const XmlConstNodeRef& p_) : p(p_.p)
	{
		if (p)
		{
			p->AddRef();
		}
	}

	inline XmlConstNodeRef::XmlConstNodeRef(const behaviac::XmlNodeRef& p_) : p(p_.p)
	{
		if (p)
		{
			p->AddRef();
		}
	}

	inline XmlConstNodeRef::~XmlConstNodeRef()
	{
		if (p)
		{
			p->Release();
		}
	}

	inline XmlConstNodeRef&  XmlConstNodeRef::operator=(const IXmlNode* newp)
	{
		if (newp)
		{
			newp->AddRef();
		}

		if (p)
		{
			p->Release();
		}

		p = newp;
		return *this;
	}

	inline XmlConstNodeRef&  XmlConstNodeRef::operator=(const XmlConstNodeRef& newp)
	{
		if (newp.p)
		{
			newp.p->AddRef();
		}

		if (p)
		{
			p->Release();
		}

		p = newp.p;
		return *this;
	}

	inline XmlConstNodeRef&  XmlConstNodeRef::operator=(const behaviac::XmlNodeRef& newp)
	{
		if (newp.p)
		{
			newp.p->AddRef();
		}

		if (p)
		{
			p->Release();
		}

		p = newp.p;
		return *this;
	}

	BEHAVIAC_API IXmlNode* CreateXmlNode(const char* tag);

	template <> inline void IXmlNode::setAttr(const char* key, const behaviac::string& value)
	{
		setAttrText(key, value.c_str());
	}
	template <> inline void IXmlNode::setAttr(const char* key, const behaviac::CStringID& value)
	{
		setAttrStringIDKind(key, value);
	}
	template <> inline void IXmlNode::setAttr(const char* key, const behaviac::CNoCaseStringID& value)
	{
		setAttrStringIDKind(key, value);
	}
	template <> inline void IXmlNode::setAttr(const char* key, const behaviac::CPathID& value)
	{
		setAttrStringIDKind(key, value);
	}

	template <> inline bool IXmlNode::getAttr(const char* key, const char*& value) const
	{
		const char* attrValue = getAttr(key);

		if (attrValue)
		{
			value = attrValue;
		}

		return attrValue != NULL;
	}
	template <> inline bool IXmlNode::getAttr(const char* key, behaviac::string& value) const
	{
		const char* attrValue = getAttr(key);

		if (attrValue)
		{
			value = attrValue;
		}

		return attrValue != NULL;
	}
	template <> inline bool IXmlNode::getAttr(const char* key, behaviac::CStringID& value) const
	{
		return getAttrStringIDKind(key, value);
	}
	template <> inline bool IXmlNode::getAttr(const char* key, behaviac::CNoCaseStringID& value) const
	{
		return getAttrStringIDKind(key, value);
	}
	template <> inline bool IXmlNode::getAttr(const char* key, behaviac::CPathID& value) const
	{
		return getAttrStringIDKind(key, value);
	}
}//namespace behaviac

BEHAVIAC_OVERRIDE_TYPE_NAME(behaviac::XmlNodeRef);
BEHAVIAC_OVERRIDE_TYPE_NAME(behaviac::XmlConstNodeRef);

#endif // #ifndef BEHAVIAC_CORE_IXML_H
