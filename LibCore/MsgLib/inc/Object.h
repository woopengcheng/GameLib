#ifndef __msg_object_h__
#define __msg_object_h__ 
#include "MsgLib/inc/MsgCommon.h" 
#include "Marshal/inc/Marshal.h"
namespace Msg
{ 
	class DLL_EXPORT  Object/* : public CUtil::Marshal*/
	{ 
	public:
		Object()
			: m_llObjID(0)
		{
		}

		Object(INT64 llObjectID)
			: m_llObjID(llObjectID)
		{
		}


		bool operator != (const Object & obj) const
		{
			if (m_llObjID == obj.m_llObjID)
			{
				return false;
			}

			return true;
		}

		bool operator == (const Object & obj) const
		{
			if (m_llObjID != obj.m_llObjID)
			{
				return false;
			}

			return true;
		}

		bool operator < (const Object & obj) const
		{
			if (m_llObjID >= obj.m_llObjID)
			{
				return false;
			}

			return true;
		}

		bool operator > (const Object & obj) const
		{
			if (m_llObjID <= obj.m_llObjID)
			{
				return false;
			}

			return true;
		}

// 	public: 
// 		virtual CUtil::CStream & marshal(CUtil::CStream & cs);
// 		virtual CUtil::CStream & unMarshal(CUtil::CStream & cs);

	public:
		INT64   m_llObjID;
	};

	//map的比较函数
	struct DLL_EXPORT  ObjectCmpFunc
	{
		bool operator()(const Object &k1, const Object &k2)const
		{
			return k1 < k2;
		}
	};

	//map的Hash函数
	struct DLL_EXPORT  ObjectHashFunc
	{
		size_t operator()(const Object & _Keyval) const
		{ 
			return ((size_t)_Keyval.m_llObjID);
		}
	};

	typedef std::vector<Msg::Object> VecObjects;
}
#endif