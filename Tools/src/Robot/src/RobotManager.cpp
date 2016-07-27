#include "CRobot.h"
#include "RobotManager.h"
#include "RobotGroup.h"
#include "MsgLib/inc/RpcManager.h"
#include "RPCCallFuncs.h"
#include "NetLib/inc/NetHandlerClient.h"


const INT32 cnRobotManagerID = 0xFFFFFFF;

namespace Robot
{
	RobotManager::RobotManager( Msg::RpcManager * pRpcManager)
		: m_pRpcManager(pRpcManager)
		, m_nStartPos(0)
		, m_nEndPos(0)
		, m_pNetReactor(NULL)
	{
	}


	RobotManager::~RobotManager()
	{
	}

	RobotManager & RobotManager::GetInstance()
	{
		static RobotManager instance(RobotGroup::GetInstance().GetRpcManager());
		return instance; 
	}

	CErrno RobotManager::Init(void)
	{
		if (!m_pNetReactor)
		{
 			m_pNetReactor = new Net::NetReactorDefault;
			//m_pNetReactor = new Net::NetReactorWES;
			m_pNetReactor->Init();
		}

		return CErrno::Success();
	}

	CErrno RobotManager::Cleanup(void)
	{
		MapRobots::iterator iter = m_mapRobots.begin();
		for (;iter != m_mapRobots.end();++iter)
		{
			iter->second->Cleanup();
			delete iter->second;
		}
		m_mapRobots.clear();

		SAFE_DELETE(m_pNetReactor);

		return CErrno::Success();
	}

	CErrno RobotManager::Update(void)
	{
		m_pNetReactor->Update();

		return CErrno::Success();
	}

	INT32 RobotManager::PreCreateRobots(INT32 nStartPos, INT32 nEndPos)
	{
		if (nStartPos <= 0)
		{
			nStartPos = 1;
		}
		INT32 nRes = 0;
		if (nStartPos > nEndPos || nEndPos < m_nEndPos || nStartPos < 0 || nEndPos < 0)
		{
			return -1;
		}
		else
		{
			INT32 nRealStartPos = 0, nRealEndPos = 0;
			if (nStartPos < m_nStartPos)		//5 这种情况是比创建的机器人的最小值还要小,那么则把最小值到上次最小值创建完成.
			{
				nRes = CreateRobots(nStartPos, m_nStartPos);
				m_nStartPos = nStartPos;
				nRealStartPos = m_nEndPos;
			}
			else
			{
				if (nStartPos > m_nEndPos)
				{
					nRealStartPos = nStartPos;
				}
				else
				{
					nRealStartPos = m_nEndPos;
				}
			}

			nRealEndPos = nEndPos;
			nRes += CreateRobots(nRealStartPos, nRealEndPos);
			m_nEndPos = nEndPos;
		}
		return nRes;
	}

	INT32 RobotManager::CreateRobots(INT32 nStartPos, INT32 nEndPos)
	{
		for (INT32 i = nStartPos;i < nEndPos;++i)
		{
			std::string strName = CUtil::itoa(i);
			CRobot * pRobot = new CRobot(i, m_pRpcManager , m_pNetReactor);
			pRobot->Init();
			m_pNetReactor->AddNetHandler(pRobot->GetNetHandler());
			m_mapRobots.insert(std::make_pair(i, pRobot));

			rpc_CreateRobot(RobotGroup::GetInstance(),
				RobotGroup::GetInstance().GetRobotSessionID(),
				RobotGroup::GetInstance().GetServerID(), i,
				strName, i % 5, i % 100, CUtil::random() % 4, strName);
		}
		return nEndPos - nStartPos;
	}


}