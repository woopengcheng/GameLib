#include "RoleCollection.h"

namespace Orm
{

	RoleCollection::RoleCollection()
	{
		m_pRole = NULL;
	}

	RoleCollection::~RoleCollection()
	{
		SAFE_DELETE(m_pRole);
	}

	Role * RoleCollection::GetRole()
	{
		if (!m_pRole)
		{
			m_pRole = new Role;
			m_pRole->SetMasterID(m_vMasterId);
		}

		return m_pRole;
	}

	RoleItems * RoleCollection::NewRoleItems(BOOL bAddToManager)
	{
		RoleItems * pRoleItems = new RoleItems;
		pRoleItems->SetMasterID(m_vMasterId);
		if (bAddToManager)
		{
			m_vecItems.push_back(pRoleItems);
		}

		return pRoleItems;
	}

	BOOL RoleCollection::DeleteRoleItems(RoleItems * pValue , bool bFree /*= false*/)
	{
		return m_vecItems.Remove(pValue , bFree);
	}

	void RoleCollection::LoadRoleItems(mongo::BSONObj & obj)
	{
		m_vecItems.Cleanup();
		mongo::BSONObjIterator iter(obj);
		while(iter.more())
		{
			mongo::BSONElement be = iter.next();
			Assert(be.isABSONObj());

			RoleItems * pRoleItems = NewRoleItems();
			pRoleItems->FromBson(be.Obj());
		}
	}

	void RoleCollection::SaveRoleItems(mongo::BSONArrayBuilder & bab)
	{
		for (size_t i = 0;i < m_vecItems.size();++i)
		{
			mongo::BSONObj obj;
			RoleItems * pRoleItems = m_vecItems[i];
			pRoleItems->ToBson(obj);
			bab.append(obj);
		}
	}

	void RoleCollection::ToBson(std::string & strBuf)
	{ 
		mongo::BSONObj __tmpobj;
		ToBson(__tmpobj);
		CUtil::Compress(__tmpobj.objdata(),__tmpobj.objsize(),strBuf);
	}

	void RoleCollection::ToBson(mongo::BSONObj & objBson)
	{
		mongo::BSONObjBuilder __builder;
		objBson = __builder.obj();
	}

	void RoleCollection::FromBson(const char * pData , INT32 nSize)
	{

	}

	void RoleCollection::FromBson(const mongo::BSONObj & objBson)
	{

	}

}