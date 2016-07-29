#include "CRobot.h"
#include "PB/net_common.pb.h"
#include "RobotCommand.h"

namespace Robot
{
	void ExportClassToLua(lua_State* L)
	{
		lua_tinker::class_add<CRobot>(L, "Robot" , true); 
		lua_tinker::class_def<CRobot>(L, "SendMsg", &CRobot::SendMsg);

		lua_tinker::namespace_add(L, "CUtil");
		lua_tinker::class_add<CUtil::CStream>(L, "CUtil::CStream", true);
		lua_tinker::class_def<CUtil::CStream>(L, "GetDataLen", &CUtil::CStream::GetDataLen);
		/*
		lua_tinker::class_def<CUtil::CStream>(L, "marshal", lua_tinker::args_type_overload_member_functor(
			lua_tinker::make_member_functor_ptr((CUtil::CStream &(CUtil::CStream::*)(char))(&CUtil::CStream::operator<<)),
			lua_tinker::make_member_functor_ptr((CUtil::CStream &(CUtil::CStream::*)(bool))(&CUtil::CStream::operator<<)),
			lua_tinker::make_member_functor_ptr((CUtil::CStream &(CUtil::CStream::*)(INT16))(&CUtil::CStream::operator<<)),
			lua_tinker::make_member_functor_ptr((CUtil::CStream &(CUtil::CStream::*)(UINT16))(&CUtil::CStream::operator<<)),
			lua_tinker::make_member_functor_ptr((CUtil::CStream &(CUtil::CStream::*)(INT32))(&CUtil::CStream::operator<<)),
			lua_tinker::make_member_functor_ptr((CUtil::CStream &(CUtil::CStream::*)(UINT32))(&CUtil::CStream::operator<<)),
			lua_tinker::make_member_functor_ptr((CUtil::CStream &(CUtil::CStream::*)(INT64))(&CUtil::CStream::operator<<)),
			lua_tinker::make_member_functor_ptr((CUtil::CStream &(CUtil::CStream::*)(UINT64))(&CUtil::CStream::operator<<)),
			lua_tinker::make_member_functor_ptr((CUtil::CStream &(CUtil::CStream::*)(float))(&CUtil::CStream::operator<<)),
			lua_tinker::make_member_functor_ptr((CUtil::CStream &(CUtil::CStream::*)(double))(&CUtil::CStream::operator<<)),
			lua_tinker::make_member_functor_ptr((CUtil::CStream &(CUtil::CStream::*)(const CUtil::Marshal &))(&CUtil::CStream::operator<<)),
			lua_tinker::make_member_functor_ptr((CUtil::CStream &(CUtil::CStream::*)(const CUtil::Chunk &))(&CUtil::CStream::operator<<)),
			lua_tinker::make_member_functor_ptr((CUtil::CStream &(CUtil::CStream::*)(const Msg::Object &))(&CUtil::CStream::operator<<)),
			lua_tinker::make_member_functor_ptr((CUtil::CStream &(CUtil::CStream::*)(const CUtil::CStream &))(&CUtil::CStream::operator<<)),
			lua_tinker::make_member_functor_ptr((CUtil::CStream &(CUtil::CStream::*)(const CUtil::Marshal &))(&CUtil::CStream::operator<<)),
			lua_tinker::make_member_functor_ptr((CUtil::CStream &(CUtil::CStream::*)(const std::string&))(&CUtil::CStream::operator<<))
			));
		lua_tinker::class_def<CUtil::CStream>(L, "unmarshal", lua_tinker::args_type_overload_member_functor(
			lua_tinker::make_member_functor_ptr((CUtil::CStream &(CUtil::CStream::*)(char &))(&CUtil::CStream::operator>>)),
			lua_tinker::make_member_functor_ptr((CUtil::CStream &(CUtil::CStream::*)(bool &))(&CUtil::CStream::operator>>)),
			lua_tinker::make_member_functor_ptr((CUtil::CStream &(CUtil::CStream::*)(INT16 &))(&CUtil::CStream::operator>>)),
			lua_tinker::make_member_functor_ptr((CUtil::CStream &(CUtil::CStream::*)(UINT16 &))(&CUtil::CStream::operator>>)),
			lua_tinker::make_member_functor_ptr((CUtil::CStream &(CUtil::CStream::*)(INT32 &))(&CUtil::CStream::operator>>)),
			lua_tinker::make_member_functor_ptr((CUtil::CStream &(CUtil::CStream::*)(UINT32 &))(&CUtil::CStream::operator>>)),
			lua_tinker::make_member_functor_ptr((CUtil::CStream &(CUtil::CStream::*)(INT64 &))(&CUtil::CStream::operator>>)),
			lua_tinker::make_member_functor_ptr((CUtil::CStream &(CUtil::CStream::*)(UINT64 &))(&CUtil::CStream::operator>>)),
			lua_tinker::make_member_functor_ptr((CUtil::CStream &(CUtil::CStream::*)(float &))(&CUtil::CStream::operator>>)),
			lua_tinker::make_member_functor_ptr((CUtil::CStream &(CUtil::CStream::*)(double &))(&CUtil::CStream::operator>>)),
			lua_tinker::make_member_functor_ptr((CUtil::CStream &(CUtil::CStream::*)(CUtil::Marshal &))(&CUtil::CStream::operator>>)),
			lua_tinker::make_member_functor_ptr((CUtil::CStream &(CUtil::CStream::*)(CUtil::Chunk &))(&CUtil::CStream::operator>>)),
			lua_tinker::make_member_functor_ptr((CUtil::CStream &(CUtil::CStream::*)(Msg::Object &))(&CUtil::CStream::operator>>)),
			lua_tinker::make_member_functor_ptr((CUtil::CStream &(CUtil::CStream::*)(CUtil::CStream &))(&CUtil::CStream::operator>>)),
			lua_tinker::make_member_functor_ptr((CUtil::CStream &(CUtil::CStream::*)(CUtil::Marshal &))(&CUtil::CStream::operator>>)),
			lua_tinker::make_member_functor_ptr((CUtil::CStream &(CUtil::CStream::*)(std::string&))(&CUtil::CStream::operator>>))
			));
			*/

	}

	CErrno RobotMsgProcess::Process(Net::ISession * pSession, UINT32 unMsgID, const char* pBuffer, UINT32 unLength)
	{
		if (m_pRobot)
		{
			return m_pRobot->Process(pSession, unMsgID, pBuffer, unLength);
		}

		return CErrno::Failure();
	}

	CRobot::CRobot(Msg::Object id, Msg::RpcManager * pRpcManager , Net::INetReactor * pReactor)
		: Msg::IRpcMsgCallableObject(id, pRpcManager) 
		, m_nRobotIndex(0)
		, m_pNetHandlerClient(NULL)
		, m_pReactor(pReactor)
		, m_pMsgProcess(NULL)
		, m_L(NULL)
	{

	}


	CRobot::~CRobot()
	{
	}

	CErrno CRobot::Init(void)
	{
		if (!m_pMsgProcess)
		{
			m_pMsgProcess = new RobotMsgProcess(this);
		}

		if (!m_pNetHandlerClient)
		{
			Net::ClientSession * pSession = new Net::ClientSession("127.0.0.1", 5555, "", "", -1, 0, -1, 0, 1);
			Net::NetHandlerClient * pClient = new Net::NetHandlerClient(m_pReactor, pSession, m_pMsgProcess);
			m_pNetHandlerClient = Net::NetHandlerClientPtr(pClient);
			m_pNetHandlerClient->Init("127.0.0.1", 5555);
		}

		if (!m_L)
		{
			m_L = luaL_newstate();
			luaL_openlibs(m_L);
			lua_tinker::init(m_L);
			RobotCommands::ExportClassToLua(m_L);
			Robot::ExportClassToLua(m_L);
			lua_tinker::dofile(m_L, "lua/RobotCommand.lua");
			lua_tinker::dofile(m_L, "lua/RobotCommandFunc.lua");
		}
		return CErrno::Success();
	}

	CErrno CRobot::Cleanup(void)
	{
		SAFE_DELETE(m_pMsgProcess);
		return CErrno::Success();
	}

	CErrno CRobot::Update(void)
	{
		m_pNetHandlerClient->Update();
		return CErrno::Success();
	}

	CErrno CRobot::Process(Net::ISession * pSession, UINT32 unMsgID, const char* pBuffer, UINT32 unLength)
	{
		lua_tinker::call<void>(m_L, "ProcessMsg", this, unMsgID, pBuffer, unLength);
		return CErrno::Success();
	}

	INT32 CRobot::SendMsg(const char * pBuf, UINT32 unSize)
	{
		if (m_pNetHandlerClient)
		{
			return m_pNetHandlerClient->SendMsg(pBuf, unSize);
		}

		return -1;
	}

	INT32 CRobot::HandleRobotCommand(INT32 nCommandType,  CUtil::CStream & cs)
	{
		MapRobotCommands::iterator iter = RobotCommands::sRobotCommands.find(nCommandType);
		if (iter != RobotCommands::sRobotCommands.end())
		{
			RobotCommand & command = iter->second;
			MapRobotParamsT & params = command.GetParams();

			MapRobotParamsT::iterator iter = params.begin();
			for (;iter != params.end();++iter)
			{
				INT32 nIndex = iter->second.type;
				std::string str = iter->second.default;
				switch (nIndex)
				{
				case ROBOT_TYPE_DOUBLE:
				{
					double value = 0;
					cs >> value;
					char buf[128];
					CUtil::ftoa(buf, value);
					iter->second.value = buf;
				}break;
				case ROBOT_TYPE_STRING:
				{
					cs >> str;
					iter->second.value = str;
				}break;
				default:
					break;
				}
			}
			if (nCommandType == 0)
			{
				PB::test_data msg;

				char pBuf[1024];
				std::string str;
				msg.set_param2(12);
				msg.mutable_params()->Add(2);
				msg.SerializePartialToString(&str);
				UINT64 ullSize = msg.ByteSize();
				((Net::MsgHeader*)pBuf)->unMsgID = 0;
				((Net::MsgHeader*)pBuf)->unMsgLength = ullSize + sizeof(Net::MsgHeader);
				memcpy(pBuf + sizeof(Net::MsgHeader), str.c_str(), ullSize);
				SendMsg(pBuf, ullSize + sizeof(Net::MsgHeader));
			}

			lua_tinker::call<void>(m_L, "HandleRobotCommand" , this, nCommandType , params);
		}
		else
		{
			gErrorStream("HandleRobotCommand error. type=" << nCommandType << ":robotindex=" << m_nRobotIndex);
		}

		return 0;
		switch (nCommandType)
		{
		case 0:
		{
			const char * kSendMsg = "client sendmsg.\n";
			INT64 unLen = strlen(kSendMsg) + 1;
			char pBuf[1024];
			((Net::MsgHeader*)pBuf)->unMsgID = 0;
			((Net::MsgHeader*)pBuf)->unMsgLength = unLen + sizeof(Net::MsgHeader);
			memcpy(pBuf + sizeof(Net::MsgHeader), kSendMsg, unLen);

			SendMsg(pBuf, sizeof(Net::MsgHeader) + unLen);
		}

		default:
			break;
		}


		return 0;
	}

}
