#ifndef __msg_net_node_h__
#define __msg_net_node_h__
#include "MsgLib/inc/MsgCommon.h"

namespace Msg
{
	class RpcInterface;

	class DLL_EXPORT NetNode
	{
	public:
		typedef tbb_unordered_map<std::string , RpcInterface *> CollectionMyselfNodesT; //5 普通的估计也是安全的.初始化是按照顺序来的.

	public:
		NetNode();
		~NetNode();

	public:
		static NetNode & GetInstance();

	public:
		void						InsertMyselfNodes(const std::string & strName , RpcInterface * pInterface);
		BOOL						IsInMyselfNodes(const std::string & strName);
		RpcInterface			*	GetMyselfNode(const std::string & strName);

	private:
		CollectionMyselfNodesT		m_mapMyselfNodes;
	};
}

#endif
