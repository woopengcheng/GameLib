#include "SlaveHandler.h"
#include "GameDB/inc/OperateRecord.h"
#include "GameDB/inc/DBCommon.h"

Msg::ObjectMsgCall * Server::SlaveHandler::SyncDataToSlave_RpcServer(INT32 nSessionID , Msg::Object objSrc ,std_string & dbname/* = std::string()*/ , CUtilChunk & value/* = CUtil::Chunk()*/, INT32 last_pos /*= 0*/)
{
	INT32 res = 0;
	CUtil::CStream cs;
	cs.Pushback(value.Begin() , value.GetDataLen());

	INT32 nCount = 0;
	cs >> nCount;

	MsgAssert_Re0(nCount < 1000000 && nCount >= 0 , "wrong count.");
	MsgAssert_Re0(dbname == m_pDatabase->GetName() , "gamedatabase is not same.");

	INT32 nType = -1;
	while (nCount --)
	{
		cs >> nType;
		switch(nType)
		{
		case GameDB::OperateRecord::OPERATE_RECODE_INSERT:
			{
				std::string dbKey , value;
				cs >> dbKey >> value;

				GameDB::Status status = m_pDatabase->QuickWrite(GameDB::Slice(dbKey),GameDB::Slice(value));
				gDebugStream("insert dbkey:" << dbKey << " dbVal: " << value);
				if (!status.ok())
				{
					gErrorStream("slave insert error.");
				}
			}
			break;
		case GameDB::OperateRecord::OPERATE_RECODE_DELETE:
			{
				std::string dbKey;
				cs >> dbKey;

				GameDB::Status status = m_pDatabase->QuickDel(GameDB::Slice(dbKey));
				gDebugStream("delete dbkey:" << dbKey << " dbVal: " << value);
				if (!status.ok())
				{
					gErrorStream("slave delete error.");
				}
			}
			break;
		default:
			{
				res = -1;
				gErrorStream("unknown operator type.");
				break;
			}
		}
	}

	m_nLastPos = last_pos;
	std::cout << "SyncDataToSlave_RpcServer m_nLastPos=" << m_nLastPos  << std::endl;
	Return(res , m_nLastPos);
}

