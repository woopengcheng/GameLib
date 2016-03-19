#include "CUtil/inc/CRTTI.h"

namespace CUtil
{
	CRTTI::CRTTI(const char * pName , INT32 nGUID ,  pCreateFunc pFunc , INT32 nClassSize , CRTTI * pP0 /*= NULL */, CRTTI * pP1 /*= NULL */, CRTTI * pP2 /*= NULL */, CRTTI * pP3 /*= NULL*/)
		: m_strClassName(pName)
		, m_pCreateFunc(pFunc)
		, m_nClassGUID(nGUID)
		, m_nClassSize(nClassSize)
	{
		memset(m_pParent , 0 , sizeof(m_pParent));

		m_pParent[0] = pP0;
		m_pParent[1] = pP1;
		m_pParent[2] = pP2;
		m_pParent[3] = pP3;

		InsertRuntimeClass(nGUID , pName , this);
	}

	CRTTI::~CRTTI()
	{

	}

	CRTTI	* CRTTI::GetRuntimeClass(const std::string & strName)
	{
		CollectionRTTIByNameT::iterator iter = GetRTTIByNameMap().find(strName);
		if (iter != GetRTTIByNameMap().end())
		{
			return iter->second;
		}

		return NULL;
	}

	CRTTI	* CRTTI::GetRuntimeClass(INT32 nClassGUID)
	{
		CollectionRTTIByGUIDT::iterator iter = GetRTTIByGUIDMap().find(nClassGUID);
		if (iter != GetRTTIByGUIDMap().end())
		{
			return iter->second;
		}

		return NULL;
	}

	BOOL CRTTI::InsertRuntimeClass(INT32 nClassGUID , const std::string & strName , CRTTI * pClass)
	{
		MsgAssert_Re0(!GetRuntimeClass(nClassGUID) || !GetRuntimeClass(strName) , "the same name class insert in RTTI define.");
		GetRTTIByGUIDMap().insert(std::make_pair(nClassGUID , pClass));
		GetRTTIByNameMap().insert(std::make_pair(strName , pClass));

		return TRUE;
	}

	CRTTI::CollectionRTTIByGUIDT & CRTTI::GetRTTIByGUIDMap()
	{ 
		static CollectionRTTIByGUIDT sRTTIByGUID;

		return sRTTIByGUID;
	}

	CRTTI::CollectionRTTIByNameT & CRTTI::GetRTTIByNameMap()
	{
		static CollectionRTTIByNameT sRTTIByName;
		return sRTTIByName;
	}

	BOOL CRTTI::IsDerivedFrom(std::string  strName)
	{
		CRTTI * pRTTI = CRTTI::GetRuntimeClass(strName);
		if (pRTTI)
		{
			return IsDerivedFrom(pRTTI);
		}

		return FALSE;
	}

	BOOL CRTTI::IsDerivedFrom(INT32 nGUID)
	{
		CRTTI * pRTTI = CRTTI::GetRuntimeClass(nGUID);
		if (pRTTI)
		{
			return IsDerivedFrom(pRTTI);
		}

		return FALSE;
	}

	BOOL CRTTI::IsDerivedFrom(CRTTI * pBase)
	{
		for (INT32 i = 0;i < MAX_RTTI_PARENT_COUNT;++i)
		{
			if (m_pParent[i])
			{
				if ( m_pParent[i] == pBase)
				{
					return TRUE;
				}
				else
				{
					if(m_pParent[i]->IsDerivedFrom(pBase))
					{
						return TRUE;
					}
				}
			}
			else
			{
				break;
			}
		}

		return FALSE;
	}

}

