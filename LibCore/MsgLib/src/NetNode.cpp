#include "Msglib/inc/NetNode.h"

namespace Msg
{
	NetNode::NetNode()
	{

	}

	NetNode::~NetNode()
	{
		m_mapMyselfNodes.clear();
	}

	NetNode & NetNode::GetInstance()
	{
		static NetNode sInstance;
		return sInstance;
	}

	void NetNode::InsertMyselfNodes(const std::string & strName , RpcInterface * pInterface)
	{
		if (!IsInMyselfNodes(strName))
		{
			m_mapMyselfNodes.insert(std::make_pair(strName , pInterface));
		}
	}

	BOOL NetNode::IsInMyselfNodes(const std::string & strName)
	{
		CollectionMyselfNodesT::iterator iter = m_mapMyselfNodes.find(strName);
		if (iter == m_mapMyselfNodes.end())
		{
			return FALSE;
		}

		return TRUE;
	}

	RpcInterface * NetNode::GetMyselfNode(const std::string & strName)
	{
		CollectionMyselfNodesT::iterator iter = m_mapMyselfNodes.find(strName);
		if (iter != m_mapMyselfNodes.end())
		{
			return iter->second;
		}

		return NULL; 
	}

}