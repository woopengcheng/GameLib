#ifndef __cutil_crtti_h__
#define __cutil_crtti_h__
#include "CUtil/inc/Common.h"

namespace CUtil
{
	class CObject;
}

#define MAX_RTTI_PARENT_COUNT 4

#define BASE(ParanetClassName) (ParanetClassName##::GetClass())

#define RTTI(ClassName ,nClassGUID , ...) public:\
	static CUtil::CObject	*	CreateObject(){ return new ClassName; }\
	static CUtil::CRTTI		*	GetClass(){ static CUtil::CRTTI	sClass##ClassName(#ClassName , nClassGUID , &ClassName##::CreateObject , sizeof(ClassName) , __VA_ARGS__);return &sClass##ClassName; }\
	virtual CUtil::CRTTI	*	GetRTTI(){return ClassName::GetClass(); }\

typedef CUtil::CObject * (*pCreateFunc)();

namespace CUtil
{
	class DLL_EXPORT CRTTI
	{
	public:
		typedef std_unordered_map<INT32 , CRTTI *> CollectionRTTIByGUIDT;
		typedef std_unordered_map<std::string , CRTTI *> CollectionRTTIByNameT;

	public:
		CRTTI(const char * pName , INT32 nGUID , pCreateFunc pFunc , INT32 nClassSize , CRTTI * pP0 = NULL , CRTTI * pP1 = NULL , CRTTI * pP2 = NULL , CRTTI * pP3 = NULL);
		~CRTTI();

	public:
		inline	CRTTI	**	GetParents() { return m_pParent; }
		inline	BOOL		IsDerivedFrom( CRTTI * pBase);
		inline	BOOL		IsDerivedFrom( std::string  strName);
		inline	BOOL		IsDerivedFrom( INT32 nGUID);
		inline	INT32		GetClassSize() const { return m_nClassSize; }
		inline	INT32		GetClassGUID() const { return m_nClassGUID; }
		inline	std::string GetClassName() const { return m_strClassName; }
		CUtil::CObject	*	CreateObject(){	return m_pCreateFunc();	}

	public:
		static	BOOL		InsertRuntimeClass(INT32 nClassGUID , const std::string & strName , CRTTI * pClass);
		static	CRTTI	*	GetRuntimeClass(const std::string & strName);
		static	CRTTI	*	GetRuntimeClass(INT32 nClassGUID);
		static	CollectionRTTIByGUIDT & GetRTTIByGUIDMap();
		static	CollectionRTTIByNameT & GetRTTIByNameMap();

	private:
		CRTTI		*	m_pParent[MAX_RTTI_PARENT_COUNT];
		INT32			m_nClassGUID;
		INT32			m_nClassSize;
		std::string		m_strClassName;
		pCreateFunc		m_pCreateFunc;
	};

}


#endif