#ifndef __orm_role_collection_h__
#define __orm_role_collection_h__
#include "GameDB/inc/DBCommon.h" 
#include "GameDB/inc/OrmCollection.h" 
#include "GameDB/inc/OrmVector.h" 
#include "Role.h"
#include "RoleItems.h"

namespace Orm
{ 
	class RoleCollection : public GameDB::OrmCollection<INT64>
	{
	public:
		RoleCollection(); 
		~RoleCollection(); 

	public:
		virtual void		ToBson(std::string & strBuf);
		virtual void		ToBson(mongo::BSONObj  & objBson);
		virtual void		FromBson(const char * pData , INT32 nSize);
		virtual void		FromBson(const mongo::BSONObj  & objBson);

	public:
		Role * GetRole();

	public:
		GameDB::OrmVectorEx<RoleItems *> & GetRoleItems(){ return m_vecItems; }

	public:
		RoleItems * NewRoleItems(BOOL bAddToManager = TRUE);
		BOOL DeleteRoleItems(RoleItems * pValue , bool bFree = false);
		void LoadRoleItems(mongo::BSONObj & obj);
		void SaveRoleItems(mongo::BSONArrayBuilder & bab);

	protected:
		Role				* m_pRole;
		GameDB::OrmVectorEx<RoleItems*> m_vecItems;
	};

}

#endif