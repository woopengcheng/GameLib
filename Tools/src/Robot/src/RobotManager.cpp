#include "CRobot.h"
#include "RobotManager.h"
#include "RobotGroup.h"
#include "MsgLib/inc/RpcManager.h"
#include "RPCCallFuncs.h"


const INT32 cnRobotManagerID = 0xFFFFFFF;

namespace Robot
{
	RobotManager::RobotManager( Msg::RpcManager * pRpcManager)
		: m_pRpcManager(pRpcManager)
		, m_nStartPos(0)
		, m_nEndPos(0)
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
		return CErrno::Success();
	}

	CErrno RobotManager::Cleanup(void)
	{
		return CErrno::Success();
	}

	CErrno RobotManager::Update(void)
	{
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
			if (nStartPos < m_nStartPos)		//5 ��������Ǳȴ����Ļ����˵���Сֵ��ҪС,��ô�����Сֵ���ϴ���Сֵ�������.
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
			CRobot * pRobot = new CRobot(i, m_pRpcManager);
			m_mapRobots.insert(std::make_pair(i, pRobot));

			rpc_CreateRobot(RobotGroup::GetInstance(),
				RobotGroup::GetInstance().GetRobotSessionID(),
				RobotGroup::GetInstance().GetServerID(), i,
				strName, i % 5, i % 100, CUtil::random() % 4, strName);
		}
		return nEndPos - nStartPos;
	}

}