#include "Msglib/inc/RpcInterface.h"
#include "MsgLib/inc/RpcManager.h"
#include "MsgLib/inc/MsgHelper.h"
#include "MsgLib/inc/RpcCheckParams.h"
#include "Timer/inc/TimerHelp.h"
// #include "UnitTest++/UnitTestPP.h"
#include "CUtil/inc/CUtil.h"
#include "CUtil/inc/Chunk.h"
#include "Test/MsgNameDefine.h"
#include "Test/GRpc.h"
#include "Test/RpcDatas.h"
#include "ThreadPool/inc/ThreadPoolInterface.h"
#include "ThreadPool/inc/ThreadTask.h"
#include "gtest/gtest.h"

static bool g_sClosed = false;
static bool g_sBCheck1 = false;
static bool g_sBCheck2 = false;
static INT32 g_sBCheck3 = false;
const INT32 FOO_HANDLER_ID = 100;
const INT32 BAR_HANDLER_ID = 101;
const INT32 PROXY_HANDLER_ID = 102;
const INT32 BAR_HANDLER_ID2 = 103;

static std::string s_parse = "{\
	\"foo\" : \
	{\
		\"server\" :\
		{\
			\"net_node_name\" : \"foo\",\
			\"listen_type\" : \"tcp\",\
			\"listen_address\" : \"127.0.0.1\",\
			\"listen_port\" : 50000 \
		},		\
		\
		\"net_thread\" :\
		{\
			\"priority\" : 10000\
		},\
		\
		\"clients\" : \
		[\
			{\
				\"type\" : \"tcp\",\
				\"address\" : \"127.0.0.1\",\
				\"port\" : 50001,	\
				\"reconnect\" : 1,\
				\"same_process\" : 1\
			},\
			{\
				\"type\" : \"tcp\",\
				\"address\" : \"127.0.0.1\",\
				\"port\" : 50002,	\
				\"reconnect\" : 1,\
				\"same_process\" : 1\
			}\
		]\
	}, \
	\
	\"bar\" : \
	{\
		\"server\" : \
		{\
			\"net_node_name\" : \"bar\",\
			\"listen_type\" : \"tcp\",\
			\"listen_address\" : \"127.0.0.1\",\
			\"listen_port\" : 50001\
		},\
		\
		\"thread\" : \
		[\
			{\
				\"priority\" : 20002,\
				\"count\" : 1\
			}\
		],\
		\
		\"net_thread\" :\
		{\
			\"priority\" : 10001\
		},\
		\"clients\" : \
		[\
			{\
				\"type\" : \"tcp\",\
				\"address\" : \"127.0.0.1\",\
				\"port\" : 50002,	\
				\"reconnect\" : 1,\
				\"same_process\" : 1\
			}\
		]\
	},\
	\
	\"proxy\" : \
	{\
		\"server\" : \
		{\
			\"net_node_name\" : \"proxy\",\
			\"listen_type\" : \"tcp\",\
			\"listen_address\" : \"127.0.0.1\",\
			\"listen_port\" : 50002\
		},\
		\
		\"thread\" : \
		[\
			{\
				\"priority\" : 20004,\
				\"count\" : 1\
			}\
		],\
		\
		\"net_thread\" :\
		{\
			\"priority\" : 10002\
		},\
		\
		\"enable_rpc\" :\
		[\
			{ \"name\" : \"testParamsAndRpcDatas\" , \"enable\" : false }\
		]\
	}\
	\
}";

class FooRpcInterface;
class FooListener : public Msg::IRpcListener
{
public:
	FooListener(FooRpcInterface * pMaster)
		: m_pParent(pMaster)
	{

	}

public:
	virtual CErrno OnConnected(Msg::RpcInterface * pRpcInterface, INT32 nSessionID, const std::string & strNetNodeName, bool bReconnect = false) override
	{
		if (strNetNodeName == "foo_To_bar")
		{
		}

		return CErrno::Success();
	}
	virtual CErrno OnDisconnected(Msg::RpcInterface * pRpcInterface, INT32 nSessionID, INT32 nPeerSessionID) override
	{

		return CErrno::Success();
	}

private:
	FooRpcInterface * m_pParent;
};

class FooRpcInterface : public Msg::RpcInterface
{
public:
	FooRpcInterface()
	{
		m_pRpcListener = new FooListener(this);
	}
	static FooRpcInterface	&	GetInstance() { static FooRpcInterface foo; return foo; }

public:
	virtual void				OnRegisterRpcs(void) override;

};

class FooHandler : public Msg::IRpcMsgCallableObject
{
	RPC_DEFINE_FooHandler;
public:
	FooHandler(INT32 nObjectID, FooRpcInterface * pDBServer, INT32 nSessionID)
		: Msg::IRpcMsgCallableObject(Msg::Object(nObjectID), pDBServer->GetRpcManager())
		, m_pDBServer(pDBServer)
		, m_nClientSessionID(nSessionID)
	{}

public:
	INT32						GetClientSessionID() const { return m_nClientSessionID; }
	FooRpcInterface			*	GetDBServer() { return m_pDBServer; }

private:
	FooRpcInterface			*	m_pDBServer;
	INT32						m_nClientSessionID;
};

Msg::ObjectMsgCall * FooHandler::testMulitServerNode_RpcServerProxy(INT32 nSessionID, Msg::Object objSrc , std_string & dbname/* = std::string()*/ , CUtilChunk & value/* = CUtil::Chunk()*/ , int value2/* = 0*/ , unsigned int value22/* = 0*/ , char valChar/* = char(0)*/)
{
	CUtilChunk res = CUtil::Chunk();


	if(-1 == ProxySendMsg("foo_To_proxy" , PROXY_HANDLER_ID , dbname , value , value2 , value22 , valChar))
	{
		ReturnDelay;
	}

	ReturnNULL;
}

Msg::ObjectMsgCall * FooHandler::testMulitServerNode_RpcClientProxy(INT32 nSessionID, Msg::Object objSrc  ,CUtilChunk & res/* = CUtil::Chunk()*/)
{


	Return(res);
}

Msg::ObjectMsgCall * FooHandler::testMulitServerNode_RpcTimeoutProxy(INT32 nSessionID, Msg::Object objSrc,std_string & dbname/* = std::string()*/ , CUtilChunk & value/* = CUtil::Chunk()*/ , int value2/* = 0*/ , unsigned int value22/* = 0*/ , char valChar/* = char(0)*/ )
{


 	std::cout << "testMulitServerNode_RpcTimeoutProxy" << std::endl;
	ReturnNULL;
}

Msg::ObjectMsgCall * FooHandler::testParamsAndRpcDatas_RpcServer(INT32 nSessionID, Msg::Object objSrc, TestRpcData & rpcData/* = TestRpcData()*/, TestRpcData2 & rpcData2/* = TestRpcData2()*/, std::vector<INT32> & Vec/* = std::vector<INT32>(1)*/)
{
	INT32 res = 100;

	TestRpcData test1;
	test1.p1 = 11;
	test1.p2 = 12;
	test1.p3 = 4;
	test1.p4 = 5;
	test1.p6 = 7;
	test1.p7 = 8;
	test1.p8 = 9;
	test1.p9 = 10;
	test1.p10 = 2;
	test1.p11 = 5;
	test1.p12 = 12;
	test1.p13 = CUtilChunk("14", sizeof("14"));
	test1.p5.push_back(1);
	test1.p5.push_back(2);
	TestRpcData2 test2;

	EXPECT_EQ(rpcData.p1, test1.p1);
	EXPECT_EQ(rpcData.p2, test1.p2);
	EXPECT_EQ(rpcData.p3, test1.p3);
	EXPECT_EQ(rpcData.p4, test1.p4);
	EXPECT_EQ(rpcData.p5[0], test1.p5[0]);
	EXPECT_EQ(rpcData.p5[1], test1.p5[1]);
	EXPECT_EQ(rpcData.p5.size() , 2);
	EXPECT_EQ(rpcData.p6, test1.p6);
	EXPECT_EQ(rpcData.p7, test1.p7);
	EXPECT_EQ(rpcData.p8, test1.p8);
	EXPECT_EQ(rpcData.p9, test1.p9);
	EXPECT_EQ(rpcData.p10, test1.p10);
	EXPECT_EQ(rpcData.p11, test1.p11);
	EXPECT_EQ(rpcData.p12, test1.p12);
	EXPECT_EQ(rpcData.p13, test1.p13);

	EXPECT_EQ(rpcData2.p1, test2.p1);
	EXPECT_EQ(rpcData2.p2, test2.p2);
	EXPECT_EQ(rpcData2.p3, test2.p3);
	EXPECT_EQ(rpcData2.p4, test2.p4);
	EXPECT_EQ(rpcData2.p6, test2.p6);
	EXPECT_EQ(rpcData2.p7, test2.p7);
	EXPECT_EQ(rpcData2.p8, test2.p8);
	EXPECT_EQ(rpcData2.p9, test2.p9);

	EXPECT_EQ(Vec.size(), 1);

	static INT32 nCount = 100;
	while (nCount-- != 2)
	{
//		gDebugStream("testParamsAndRpcDatas_RpcServer:count=" << nCount);
		ReturnDelay;
	}

	Return(nCount);
}

class BarRpcInterface;

static INT32  rpc_testMulitServerNode(const char * pSessionName, Msg::VecObjects & vecTargets, Msg::Object objSrc, std_string & dbname = std::string(), CUtilChunk & value = CUtil::Chunk(), int value2 = 0, unsigned int value22 = 0, char valChar = char(0), Msg::RpcCallback * pCallback = NULL, UINT16 usPriority = 0, Msg::EMSG_SYNC_TYPE objSyncType = Msg::SYNC_TYPE_NONSYNC);
static INT32  rpc_testMulitServerNode(const char * pSessionName, Msg::Object objTarget, Msg::Object objSrc, std_string & dbname = std::string(), CUtilChunk & value = CUtil::Chunk(), int value2 = 0, unsigned int value22 = 0, char valChar = char(0), Msg::RpcCallback * pCallback = NULL, UINT16 usPriority = 0, Msg::EMSG_SYNC_TYPE objSyncType = Msg::SYNC_TYPE_NONSYNC);
static INT32  rpc_testMulitServerNode(const char * pSessionName, Msg::Object objSrc, std_string & dbname = std::string(), CUtilChunk & value = CUtil::Chunk(), int value2 = 0, unsigned int value22 = 0, char valChar = char(0), Msg::RpcCallback * pCallback = NULL, UINT16 usPriority = 0, Msg::EMSG_SYNC_TYPE objSyncType = Msg::SYNC_TYPE_NONSYNC);
static INT32  rpc_testMulitServerNode(const std::string & pSessionName, Msg::VecObjects & vecTargets, Msg::Object objSrc, std_string & dbname = std::string(), CUtilChunk & value = CUtil::Chunk(), int value2 = 0, unsigned int value22 = 0, char valChar = char(0), Msg::RpcCallback * pCallback = NULL, UINT16 usPriority = 0, Msg::EMSG_SYNC_TYPE objSyncType = Msg::SYNC_TYPE_NONSYNC);
static INT32  rpc_testMulitServerNode(const std::string & pSessionName, Msg::Object objTarget, Msg::Object objSrc, std_string & dbname = std::string(), CUtilChunk & value = CUtil::Chunk(), int value2 = 0, unsigned int value22 = 0, char valChar = char(0), Msg::RpcCallback * pCallback = NULL, UINT16 usPriority = 0, Msg::EMSG_SYNC_TYPE objSyncType = Msg::SYNC_TYPE_NONSYNC);
static INT32  rpc_testMulitServerNode(const std::string & pSessionName, Msg::Object objSrc, std_string & dbname = std::string(), CUtilChunk & value = CUtil::Chunk(), int value2 = 0, unsigned int value22 = 0, char valChar = char(0), Msg::RpcCallback * pCallback = NULL, UINT16 usPriority = 0, Msg::EMSG_SYNC_TYPE objSyncType = Msg::SYNC_TYPE_NONSYNC);
static INT32  rpc_testMulitServerNode(INT32 nSessionID, Msg::VecObjects & vecTargets, Msg::Object objSrc, std_string & dbname = std::string(), CUtilChunk & value = CUtil::Chunk(), int value2 = 0, unsigned int value22 = 0, char valChar = char(0), Msg::RpcCallback * pCallback = NULL, UINT16 usPriority = 0, Msg::EMSG_SYNC_TYPE objSyncType = Msg::SYNC_TYPE_NONSYNC);
static INT32  rpc_testMulitServerNode(INT32 nSessionID, Msg::Object objTarget, Msg::Object objSrc, std_string & dbname = std::string(), CUtilChunk & value = CUtil::Chunk(), int value2 = 0, unsigned int value22 = 0, char valChar = char(0), Msg::RpcCallback * pCallback = NULL, UINT16 usPriority = 0, Msg::EMSG_SYNC_TYPE objSyncType = Msg::SYNC_TYPE_NONSYNC);
static INT32  rpc_testMulitServerNode(INT32 nSessionID, Msg::Object objSrc, std_string & dbname = std::string(), CUtilChunk & value = CUtil::Chunk(), int value2 = 0, unsigned int value22 = 0, char valChar = char(0), Msg::RpcCallback * pCallback = NULL, UINT16 usPriority = 0, Msg::EMSG_SYNC_TYPE objSyncType = Msg::SYNC_TYPE_NONSYNC);

class BarHandler : public Msg::IRpcMsgCallableObject
{
	RPC_DEFINE_BarHandler;
public:
	BarHandler(Msg::Object objID, Msg::RpcInterface * pDBSlave)
		: Msg::IRpcMsgCallableObject(objID, pDBSlave->GetRpcManager())
	{
	}

	~BarHandler()
	{
	}
public:
	virtual CErrno Update() override { return CErrno::Success(); }
};

class BarListener : public Msg::IRpcListener
{
public:
	BarListener(BarRpcInterface * pMaster)
		: m_pParent(pMaster)
	{

	}

public:
	virtual CErrno OnConnected(Msg::RpcInterface * pRpcInterface, INT32 nSessionID, const std::string & strNetNodeName, bool bReconnect = false) override
	{
		if (strNetNodeName == "bar_To_foo")
		{
			BarHandler barHandler(BAR_HANDLER_ID, pRpcInterface);
 			rpc_testMulitServerNode(nSessionID, FOO_HANDLER_ID , BAR_HANDLER_ID);
		}

		return CErrno::Success();
	}
	virtual CErrno OnDisconnected(Msg::RpcInterface * pRpcInterface, INT32 nSessionID, INT32 nPeerSessionID) override
	{

		return CErrno::Success();
	}

private:
	BarRpcInterface * m_pParent;
};

class BarRpcInterface : public Msg::RpcInterface, public ThreadPool::ThreadSustainTask
{
public:
	BarRpcInterface()
		: ThreadPool::ThreadSustainTask(103, "ProxyRpcInterface")
	{
		m_pRpcListener = new BarListener(this);
	}
	static BarRpcInterface	&	GetInstance() { static BarRpcInterface foo; return foo; }

	virtual CErrno Init(Json::Value & conf)
	{
		Json::Value thread = conf.get("thread", Json::Value());
		std::map<UINT32, UINT32> mapThreads;
		INT32 nCount = thread.size();
		for (INT32 i = 0; i < nCount; ++i)
		{
			Json::Value objThread = thread[i];
			UINT32 priority = objThread.get("priority", 0).asUInt();
			SetThreadPriority(priority);
			UINT32 count = objThread.get("count", 1).asUInt();

			mapThreads[priority] = count;

			ThreadPool::ThreadPoolInterface::GetInstance().CreateThread(priority, count);
			ThreadPool::ThreadPoolInterface::GetInstance().AddTask(this);
		}

		return Msg::RpcInterface::Init(conf);
	}

	CErrno Update(void)
	{
		return Msg::RpcInterface::Update();
	}

public:
	virtual void				OnRegisterRpcs(void) override;
};

INT32  rpc_testMulitServerNode(const char * pSessionName, Msg::VecObjects & vecTargets, Msg::Object objSrc, std_string & dbname , CUtilChunk & value , int value2 , unsigned int value22 , char valChar , Msg::RpcCallback * pCallback , UINT16 usPriority , Msg::EMSG_SYNC_TYPE objSyncType)
{
	GEN_RPC_CALL_5((&(BarRpcInterface::GetInstance())), pSessionName, Msg::g_sztestMulitServerNode_RpcCall, dbname, value, value2, value22, valChar, vecTargets, objSrc, pCallback, usPriority, objSyncType, 10);
}

INT32  rpc_testMulitServerNode(const char * pSessionName, Msg::Object objTarget, Msg::Object objSrc, std_string & dbname , CUtilChunk & value , int value2 , unsigned int value22 , char valChar , Msg::RpcCallback * pCallback , UINT16 usPriority , Msg::EMSG_SYNC_TYPE objSyncType)
{
	std::vector<Msg::Object> vecTargets;
	vecTargets.push_back(objTarget);
	return rpc_testMulitServerNode(pSessionName, vecTargets, objSrc, dbname, value, value2, value22, valChar, pCallback, usPriority, objSyncType);
}

INT32  rpc_testMulitServerNode(const char * pSessionName, Msg::Object objSrc, std_string & dbname , CUtilChunk & value , int value2 , unsigned int value22 , char valChar , Msg::RpcCallback * pCallback , UINT16 usPriority , Msg::EMSG_SYNC_TYPE objSyncType)
{
	return rpc_testMulitServerNode(pSessionName, 0, objSrc, dbname, value, value2, value22, valChar, pCallback, usPriority, objSyncType);
}

INT32  rpc_testMulitServerNode(const std::string & pSessionName, Msg::VecObjects & vecTargets, Msg::Object objSrc, std_string & dbname , CUtilChunk & value , int value2 , unsigned int value22 , char valChar , Msg::RpcCallback * pCallback , UINT16 usPriority , Msg::EMSG_SYNC_TYPE objSyncType)
{
	GEN_RPC_CALL_5((&(BarRpcInterface::GetInstance())), pSessionName, Msg::g_sztestMulitServerNode_RpcCall, dbname, value, value2, value22, valChar, vecTargets, objSrc, pCallback, usPriority, objSyncType, 10);
}

INT32  rpc_testMulitServerNode(const std::string & pSessionName, Msg::Object objTarget, Msg::Object objSrc, std_string & dbname , CUtilChunk & value , int value2 , unsigned int value22 , char valChar , Msg::RpcCallback * pCallback , UINT16 usPriority , Msg::EMSG_SYNC_TYPE objSyncType)
{
	std::vector<Msg::Object> vecTargets;
	vecTargets.push_back(objTarget);
	return rpc_testMulitServerNode(pSessionName, vecTargets, objSrc, dbname, value, value2, value22, valChar, pCallback, usPriority, objSyncType);
}

INT32  rpc_testMulitServerNode(const std::string & pSessionName, Msg::Object objSrc, std_string & dbname , CUtilChunk & value , int value2 , unsigned int value22 , char valChar , Msg::RpcCallback * pCallback , UINT16 usPriority , Msg::EMSG_SYNC_TYPE objSyncType)
{
	return rpc_testMulitServerNode(pSessionName, 0, objSrc, dbname, value, value2, value22, valChar, pCallback, usPriority, objSyncType);
}

INT32  rpc_testMulitServerNode(INT32 nSessionID, Msg::VecObjects & vecTargets, Msg::Object objSrc, std_string & dbname , CUtilChunk & value , int value2 , unsigned int value22 , char valChar , Msg::RpcCallback * pCallback , UINT16 usPriority , Msg::EMSG_SYNC_TYPE objSyncType)
{
	GEN_RPC_CALL_5((&(BarRpcInterface::GetInstance())), nSessionID, Msg::g_sztestMulitServerNode_RpcCall, dbname, value, value2, value22, valChar, vecTargets, objSrc, pCallback, usPriority, objSyncType, 10);
}

INT32  rpc_testMulitServerNode(INT32 nSessionID, Msg::Object objTarget, Msg::Object objSrc, std_string & dbname , CUtilChunk & value , int value2 , unsigned int value22 , char valChar , Msg::RpcCallback * pCallback , UINT16 usPriority , Msg::EMSG_SYNC_TYPE objSyncType)
{
	std::vector<Msg::Object> vecTargets;
	vecTargets.push_back(objTarget);
	return rpc_testMulitServerNode(nSessionID, vecTargets, objSrc, dbname, value, value2, value22, valChar, pCallback, usPriority, objSyncType);
}

INT32  rpc_testMulitServerNode(INT32 nSessionID, Msg::Object objSrc, std_string & dbname , CUtilChunk & value , int value2 , unsigned int value22 , char valChar , Msg::RpcCallback * pCallback , UINT16 usPriority , Msg::EMSG_SYNC_TYPE objSyncType)
{
	return rpc_testMulitServerNode(nSessionID, 0, objSrc, dbname, value, value2, value22, valChar, pCallback, usPriority, objSyncType);
}

//////////////////////////////////////////////////////////////////////////

Msg::ObjectMsgCall * BarHandler::testMulitServerNode_RpcServer(INT32 nSessionID, Msg::Object objSrc, std_string & dbname/* = std::string()*/, CUtilChunk & value/* = CUtil::Chunk()*/, int value2/* = 0*/, unsigned int value22/* = 0*/, char valChar/* = char(0)*/)
{
	CUtilChunk res = CUtil::Chunk();
	
	g_sBCheck1 = true;
	Return(res);
}

static INT32  rpc_testParamsAndRpcDatas(const char * pSessionName, Msg::VecObjects & vecTargets, Msg::Object objSrc, TestRpcData & rpcData = TestRpcData(), TestRpcData2 & rpcData2 = TestRpcData2(), std::vector<INT32> & Vec = std::vector<INT32>(1), Msg::RpcCallback * pCallback = NULL, UINT16 usPriority = 0, Msg::EMSG_SYNC_TYPE objSyncType = Msg::SYNC_TYPE_NONSYNC);
static INT32  rpc_testParamsAndRpcDatas(const char * pSessionName, Msg::Object objTarget, Msg::Object objSrc, TestRpcData & rpcData = TestRpcData(), TestRpcData2 & rpcData2 = TestRpcData2(), std::vector<INT32> & Vec = std::vector<INT32>(1), Msg::RpcCallback * pCallback = NULL, UINT16 usPriority = 0, Msg::EMSG_SYNC_TYPE objSyncType = Msg::SYNC_TYPE_NONSYNC);
static INT32  rpc_testParamsAndRpcDatas(const char * pSessionName, Msg::Object objSrc, TestRpcData & rpcData = TestRpcData(), TestRpcData2 & rpcData2 = TestRpcData2(), std::vector<INT32> & Vec = std::vector<INT32>(1), Msg::RpcCallback * pCallback = NULL, UINT16 usPriority = 0, Msg::EMSG_SYNC_TYPE objSyncType = Msg::SYNC_TYPE_NONSYNC);
static INT32  rpc_testParamsAndRpcDatas(const std::string & pSessionName, Msg::VecObjects & vecTargets, Msg::Object objSrc, TestRpcData & rpcData = TestRpcData(), TestRpcData2 & rpcData2 = TestRpcData2(), std::vector<INT32> & Vec = std::vector<INT32>(1), Msg::RpcCallback * pCallback = NULL, UINT16 usPriority = 0, Msg::EMSG_SYNC_TYPE objSyncType = Msg::SYNC_TYPE_NONSYNC);
static INT32  rpc_testParamsAndRpcDatas(const std::string & pSessionName, Msg::Object objTarget, Msg::Object objSrc, TestRpcData & rpcData = TestRpcData(), TestRpcData2 & rpcData2 = TestRpcData2(), std::vector<INT32> & Vec = std::vector<INT32>(1), Msg::RpcCallback * pCallback = NULL, UINT16 usPriority = 0, Msg::EMSG_SYNC_TYPE objSyncType = Msg::SYNC_TYPE_NONSYNC);
static INT32  rpc_testParamsAndRpcDatas(const std::string & pSessionName, Msg::Object objSrc, TestRpcData & rpcData = TestRpcData(), TestRpcData2 & rpcData2 = TestRpcData2(), std::vector<INT32> & Vec = std::vector<INT32>(1), Msg::RpcCallback * pCallback = NULL, UINT16 usPriority = 0, Msg::EMSG_SYNC_TYPE objSyncType = Msg::SYNC_TYPE_NONSYNC);
static INT32  rpc_testParamsAndRpcDatas(INT32 nSessionID, Msg::VecObjects & vecTargets, Msg::Object objSrc, TestRpcData & rpcData = TestRpcData(), TestRpcData2 & rpcData2 = TestRpcData2(), std::vector<INT32> & Vec = std::vector<INT32>(1), Msg::RpcCallback * pCallback = NULL, UINT16 usPriority = 0, Msg::EMSG_SYNC_TYPE objSyncType = Msg::SYNC_TYPE_NONSYNC);
static INT32  rpc_testParamsAndRpcDatas(INT32 nSessionID, Msg::Object objTarget, Msg::Object objSrc, TestRpcData & rpcData = TestRpcData(), TestRpcData2 & rpcData2 = TestRpcData2(), std::vector<INT32> & Vec = std::vector<INT32>(1), Msg::RpcCallback * pCallback = NULL, UINT16 usPriority = 0, Msg::EMSG_SYNC_TYPE objSyncType = Msg::SYNC_TYPE_NONSYNC);
static INT32  rpc_testParamsAndRpcDatas(INT32 nSessionID, Msg::Object objSrc, TestRpcData & rpcData = TestRpcData(), TestRpcData2 & rpcData2 = TestRpcData2(), std::vector<INT32> & Vec = std::vector<INT32>(1), Msg::RpcCallback * pCallback = NULL, UINT16 usPriority = 0, Msg::EMSG_SYNC_TYPE objSyncType = Msg::SYNC_TYPE_NONSYNC);

class ProxyRpcInterface;
class ProxyHandler : public Msg::IRpcMsgCallableObject
{
	RPC_DEFINE_ProxyHandler;
public:
	ProxyHandler(INT32 nObjectID, Msg::RpcInterface * pDBServer, INT32 nSessionID)
		: Msg::IRpcMsgCallableObject(Msg::Object(nObjectID), pDBServer->GetRpcManager())
		, m_pDBServer(pDBServer)
		, m_nClientSessionID(nSessionID)
	{}

public:
	INT32						GetClientSessionID() const { return m_nClientSessionID; }
	Msg::RpcInterface		*	GetDBServer() { return m_pDBServer; }

private:
	Msg::RpcInterface		*	m_pDBServer;
	INT32						m_nClientSessionID;
};

class ProxyCallback : public Msg::RpcCallback
{
public:
	ProxyCallback()
		: m_nTest(111)
	{}
	~ProxyCallback()
	{

	}
	virtual INT32 OnCall(void * pContext) override
	{
		EXPECT_EQ(m_nTest, 333);

		return -1;
	}
	INT32	m_nTest;
};
DECLARE_BOOST_POINTERS(ProxyCallback);
Msg::RpcCallbackPtr	g_rpcCallbackTest;

class ProxyListener : public Msg::IRpcListener
{
public:
	ProxyListener(ProxyRpcInterface * pMaster)
		: m_pParent(pMaster)
	{

	}

public:
	virtual CErrno OnConnected(Msg::RpcInterface * pRpcInterface, INT32 nSessionID, const std::string & strNetNodeName, bool bReconnect = false) override
	{
		if (strNetNodeName == "proxy_To_foo")
		{
			static ProxyHandler fooHandler(PROXY_HANDLER_ID, pRpcInterface, nSessionID);

			static bool bFirst = true;
			if (bFirst)
			{
				TestRpcData test1;
				test1.p1 = 11;
				test1.p2 = 12;
				test1.p3 = 4;
				test1.p4 = 5;
				test1.p6 = 7;
				test1.p7 = 8;
				test1.p8 = 9;
				test1.p9 = 10;
				test1.p10 = 2;
				test1.p11 = 5;
				test1.p12 = 12;
				test1.p13 = CUtilChunk("14", sizeof("14"));
				test1.p5.push_back(1);
				test1.p5.push_back(2);
				TestRpcData2 test2;

				ProxyCallback * pCallback = new ProxyCallback;
				pCallback->m_nTest = 333;
				INT32 nResult = rpc_testParamsAndRpcDatas(nSessionID, FOO_HANDLER_ID, fooHandler.GetObjectID(), test1, test2 ,std::vector<INT32>(1) ,pCallback);
				EXPECT_EQ(nResult, -1);  //5 测试rpc_enable是否是关闭的.

				Msg::MethodImpl * pMethod = pRpcInterface->GetRpcManager()->GetMethodImpl(Msg::g_sztestParamsAndRpcDatas_RpcClient);
				if (pMethod)
				{
					pMethod->m_bEnable = TRUE;
				}

				pCallback = new ProxyCallback;  //5 这里是因为发送错误后,释放掉了.
				pCallback->m_nTest = 333;
				nResult = rpc_testParamsAndRpcDatas(nSessionID, FOO_HANDLER_ID, fooHandler.GetObjectID(), test1, test2, std::vector<INT32>(1), pCallback);
				if (nResult == -1)
				{
					EXPECT_EQ(nResult , 0);//5 测试rpc_enable是否是开启的.
				}

				bFirst = false;
			}
		}

		return CErrno::Success();
	}
	virtual CErrno OnDisconnected(Msg::RpcInterface * pRpcInterface, INT32 nSessionID, INT32 nPeerSessionID) override
	{

		return CErrno::Success();
	}

private:
	ProxyRpcInterface * m_pParent;
};

class ProxyRpcInterface : public Msg::RpcInterface , public ThreadPool::ThreadSustainTask
{
public:
	ProxyRpcInterface()
		: ThreadPool::ThreadSustainTask(103, "ProxyRpcInterface")
	{
		m_pRpcListener = new ProxyListener(this);
	}
	static ProxyRpcInterface	&	GetInstance() { static ProxyRpcInterface foo; return foo; }

	virtual CErrno Init(Json::Value & conf)
	{
		Json::Value thread = conf.get("thread", Json::Value());
		std::map<UINT32, UINT32> mapThreads;
		INT32 nCount = thread.size();
		for (INT32 i = 0; i < nCount; ++i)
		{
			Json::Value objThread = thread[i];
			UINT32 priority = objThread.get("priority", 0).asUInt();
			SetThreadPriority(priority);
			UINT32 count = objThread.get("count", 1).asUInt();

			mapThreads[priority] = count;

 			ThreadPool::ThreadPoolInterface::GetInstance().CreateThread(priority, count);
 			ThreadPool::ThreadPoolInterface::GetInstance().AddTask(this);
		}
		
		return Msg::RpcInterface::Init(conf);
	}

	CErrno Update(void)
	{
		return Msg::RpcInterface::Update();
	}

	virtual CErrno			Cleanup(void)
	{
		return Msg::RpcInterface::Cleanup();
	}
public:
	virtual void				OnRegisterRpcs(void) override;

};

INT32  rpc_testParamsAndRpcDatas(const char * pSessionName, Msg::VecObjects & vecTargets, Msg::Object objSrc, TestRpcData & rpcData , TestRpcData2 & rpcData2 , std::vector<INT32> & Vec , Msg::RpcCallback * pCallback , UINT16 usPriority, Msg::EMSG_SYNC_TYPE objSyncType)
{
	GEN_RPC_CALL_3((&(ProxyRpcInterface::GetInstance())), pSessionName, Msg::g_sztestParamsAndRpcDatas_RpcCall, rpcData, rpcData2, Vec, vecTargets, objSrc, pCallback, usPriority, objSyncType, 10);
}

INT32  rpc_testParamsAndRpcDatas(const char * pSessionName, Msg::Object objTarget, Msg::Object objSrc, TestRpcData & rpcData , TestRpcData2 & rpcData2 , std::vector<INT32> & Vec , Msg::RpcCallback * pCallback , UINT16 usPriority, Msg::EMSG_SYNC_TYPE objSyncType)
{
	std::vector<Msg::Object> vecTargets;
	vecTargets.push_back(objTarget);
	return rpc_testParamsAndRpcDatas(pSessionName, vecTargets, objSrc, rpcData, rpcData2, Vec, pCallback, usPriority, objSyncType);
}

INT32  rpc_testParamsAndRpcDatas(const char * pSessionName, Msg::Object objSrc, TestRpcData & rpcData , TestRpcData2 & rpcData2 , std::vector<INT32> & Vec , Msg::RpcCallback * pCallback , UINT16 usPriority, Msg::EMSG_SYNC_TYPE objSyncType)
{
	return rpc_testParamsAndRpcDatas(pSessionName, 0, objSrc, rpcData, rpcData2, Vec, pCallback, usPriority, objSyncType);
}

INT32  rpc_testParamsAndRpcDatas(const std::string & pSessionName, Msg::VecObjects & vecTargets, Msg::Object objSrc, TestRpcData & rpcData , TestRpcData2 & rpcData2 , std::vector<INT32> & Vec , Msg::RpcCallback * pCallback , UINT16 usPriority, Msg::EMSG_SYNC_TYPE objSyncType)
{
	GEN_RPC_CALL_3((&(ProxyRpcInterface::GetInstance())), pSessionName, Msg::g_sztestParamsAndRpcDatas_RpcCall, rpcData, rpcData2, Vec, vecTargets, objSrc, pCallback, usPriority, objSyncType, 10);
}

INT32  rpc_testParamsAndRpcDatas(const std::string & pSessionName, Msg::Object objTarget, Msg::Object objSrc, TestRpcData & rpcData , TestRpcData2 & rpcData2 , std::vector<INT32> & Vec , Msg::RpcCallback * pCallback , UINT16 usPriority, Msg::EMSG_SYNC_TYPE objSyncType)
{
	std::vector<Msg::Object> vecTargets;
	vecTargets.push_back(objTarget);
	return rpc_testParamsAndRpcDatas(pSessionName, vecTargets, objSrc, rpcData, rpcData2, Vec, pCallback, usPriority, objSyncType);
}

INT32  rpc_testParamsAndRpcDatas(const std::string & pSessionName, Msg::Object objSrc, TestRpcData & rpcData , TestRpcData2 & rpcData2 , std::vector<INT32> & Vec , Msg::RpcCallback * pCallback , UINT16 usPriority, Msg::EMSG_SYNC_TYPE objSyncType)
{
	return rpc_testParamsAndRpcDatas(pSessionName, 0, objSrc, rpcData, rpcData2, Vec, pCallback, usPriority, objSyncType);
}

INT32  rpc_testParamsAndRpcDatas(INT32 nSessionID, Msg::VecObjects & vecTargets, Msg::Object objSrc, TestRpcData & rpcData , TestRpcData2 & rpcData2 , std::vector<INT32> & Vec , Msg::RpcCallback * pCallback , UINT16 usPriority, Msg::EMSG_SYNC_TYPE objSyncType)
{
	GEN_RPC_CALL_3((&(ProxyRpcInterface::GetInstance())), nSessionID, Msg::g_sztestParamsAndRpcDatas_RpcCall, rpcData, rpcData2, Vec, vecTargets, objSrc, pCallback, usPriority, objSyncType, 10);
}

INT32  rpc_testParamsAndRpcDatas(INT32 nSessionID, Msg::Object objTarget, Msg::Object objSrc, TestRpcData & rpcData , TestRpcData2 & rpcData2 , std::vector<INT32> & Vec , Msg::RpcCallback * pCallback , UINT16 usPriority, Msg::EMSG_SYNC_TYPE objSyncType)
{
	std::vector<Msg::Object> vecTargets;
	vecTargets.push_back(objTarget);
	return rpc_testParamsAndRpcDatas(nSessionID, vecTargets, objSrc, rpcData, rpcData2, Vec, pCallback, usPriority, objSyncType);
}

INT32  rpc_testParamsAndRpcDatas(INT32 nSessionID, Msg::Object objSrc, TestRpcData & rpcData , TestRpcData2 & rpcData2 , std::vector<INT32> & Vec , Msg::RpcCallback * pCallback , UINT16 usPriority, Msg::EMSG_SYNC_TYPE objSyncType)
{
	return rpc_testParamsAndRpcDatas(nSessionID, 0, objSrc, rpcData, rpcData2, Vec, pCallback, usPriority, objSyncType);
}


Msg::ObjectMsgCall * ProxyHandler::testParamsAndRpcDatas_RpcClient(INT32 nSessionID, Msg::Object objSrc, INT32 res/* = 0*/)
{
	CUtil::Parameters * pParams = GetInParams();
	EXPECT_EQ(!pParams, false);
	if (pParams)
	{
		TestRpcData test1;
		test1.p1 = 11;
		test1.p2 = 12;
		test1.p3 = 4;
		test1.p4 = 5;
		test1.p6 = 7;
		test1.p7 = 8;
		test1.p8 = 9;
		test1.p9 = 10;
		test1.p10 = 2;
		test1.p11 = 5;
		test1.p12 = 12;
		test1.p13 = CUtilChunk("14", sizeof("14"));
		test1.p5.push_back(1);
		test1.p5.push_back(2);
		TestRpcData2 test2;

		TestRpcData   rpcData = pParams->GetValue<TestRpcData>(0);
		TestRpcData2  rpcData2 = pParams->GetValue<TestRpcData2>(1);

		EXPECT_EQ(rpcData.p1, test1.p1);
		EXPECT_EQ(rpcData.p2, test1.p2);
		EXPECT_EQ(rpcData.p3, test1.p3);
		EXPECT_EQ(rpcData.p4, test1.p4);
		EXPECT_EQ(rpcData.p5[0], test1.p5[0]);
		EXPECT_EQ(rpcData.p5[1], test1.p5[1]);
		EXPECT_EQ(rpcData.p5.size(), 2);
		EXPECT_EQ(rpcData.p6, test1.p6);
		EXPECT_EQ(rpcData.p7, test1.p7);
		EXPECT_EQ(rpcData.p8, test1.p8);
		EXPECT_EQ(rpcData.p9, test1.p9);
		EXPECT_EQ(rpcData.p10, test1.p10);
		EXPECT_EQ(rpcData.p11, test1.p11);
		EXPECT_EQ(rpcData.p12, test1.p12);
		EXPECT_EQ(rpcData.p13, test1.p13);

		EXPECT_EQ(rpcData2.p1, test2.p1);
		EXPECT_EQ(rpcData2.p2, test2.p2);
		EXPECT_EQ(rpcData2.p3, test2.p3);
		EXPECT_EQ(rpcData2.p4, test2.p4);
		EXPECT_EQ(rpcData2.p6, test2.p6);
		EXPECT_EQ(rpcData2.p7, test2.p7);
		EXPECT_EQ(rpcData2.p8, test2.p8);
		EXPECT_EQ(rpcData2.p9, test2.p9);

		std::vector<INT32> Vec2;
		std::vector<INT32> Vec3;
		std::vector<INT32> &  Vec = pParams->GetValue<std::vector<INT32>>(2);
		pParams->GetValue(2, Vec2);
//		Vec3 = static_cast<std::vector<INT32>>((*pParams)[2]);
		EXPECT_EQ(Vec.size(), 1);
		EXPECT_EQ(Vec2.size(), 1);
		EXPECT_EQ(Vec3.size(), 0);
		
		TestRpcData   rpcData3;
		TestRpcData2  rpcData4;
		TestRpcData2  rpcData5;
		std::vector<INT32> Vec4;
		pParams->GetValueEx(rpcData3, rpcData4, Vec4);
		rpcData5 = (*pParams)[1];
		EXPECT_EQ(Vec4.size(), 1);

		EXPECT_EQ(rpcData3.p1, test1.p1);
		EXPECT_EQ(rpcData3.p2, test1.p2);
		EXPECT_EQ(rpcData3.p3, test1.p3);
		EXPECT_EQ(rpcData3.p4, test1.p4);
		EXPECT_EQ(rpcData3.p5[0], test1.p5[0]);
		EXPECT_EQ(rpcData3.p5[1], test1.p5[1]);
		EXPECT_EQ(rpcData3.p5.size(), 2);
		EXPECT_EQ(rpcData3.p6, test1.p6);
		EXPECT_EQ(rpcData3.p7, test1.p7);
		EXPECT_EQ(rpcData3.p8, test1.p8);
		EXPECT_EQ(rpcData3.p9, test1.p9);
		EXPECT_EQ(rpcData3.p10, test1.p10);
		EXPECT_EQ(rpcData3.p11, test1.p11);
		EXPECT_EQ(rpcData3.p12, test1.p12);
		EXPECT_EQ(rpcData3.p13, test1.p13);

		EXPECT_EQ(rpcData4.p1, test2.p1);
		EXPECT_EQ(rpcData4.p2, test2.p2);
		EXPECT_EQ(rpcData4.p3, test2.p3);
		EXPECT_EQ(rpcData4.p4, test2.p4);
		EXPECT_EQ(rpcData4.p6, test2.p6);
		EXPECT_EQ(rpcData4.p7, test2.p7);
		EXPECT_EQ(rpcData4.p8, test2.p8);
		EXPECT_EQ(rpcData4.p9, test2.p9);

		EXPECT_EQ(rpcData5.p1, test2.p1);
		EXPECT_EQ(rpcData5.p2, test2.p2);
		EXPECT_EQ(rpcData5.p3, test2.p3);
		EXPECT_EQ(rpcData5.p4, test2.p4);
		EXPECT_EQ(rpcData5.p6, test2.p6);
		EXPECT_EQ(rpcData5.p7, test2.p7);
		EXPECT_EQ(rpcData5.p8, test2.p8);
		EXPECT_EQ(rpcData5.p9, test2.p9);
	}

	EXPECT_EQ(res , 1);

	g_rpcCallbackTest = GetCallback();
	if (g_rpcCallbackTest)
	{
		INT32 * t = NULL;
		EXPECT_EQ(g_rpcCallbackTest->OnCall(t), -1);
	}
	EXPECT_EQ(!g_rpcCallbackTest, false);
	ReturnNULL;
}

Msg::ObjectMsgCall * ProxyHandler::testParamsAndRpcDatas_RpcTimeout(INT32 nSessionID, Msg::Object objSrc, TestRpcData & rpcData/* = TestRpcData()*/, TestRpcData2 & rpcData2/* = TestRpcData2()*/, std::vector<INT32> & Vec/* = std::vector<INT32>(1)*/)
{


	std::cout << "testParamsAndRpcDatas_RpcTimeout" << std::endl;
	ReturnNULL;
}

Msg::ObjectMsgCall * ProxyHandler::testMulitServerNode_RpcServerProxy(INT32 nSessionID, Msg::Object objSrc, std_string & dbname/* = std::string()*/, CUtilChunk & value/* = CUtil::Chunk()*/, int value2/* = 0*/, unsigned int value22/* = 0*/, char valChar/* = char(0)*/)
{
	CUtilChunk res = CUtil::Chunk();


	if (-1 == ProxySendMsg("proxy_To_bar", BAR_HANDLER_ID, dbname, value, value2, value22, valChar))
	{
		ReturnDelay;
	}

	ReturnNULL;
}

Msg::ObjectMsgCall * ProxyHandler::testMulitServerNode_RpcClientProxy(INT32 nSessionID, Msg::Object objSrc, CUtilChunk & res/* = CUtil::Chunk()*/)
{

	Return(res);
}

Msg::ObjectMsgCall * ProxyHandler::testMulitServerNode_RpcTimeoutProxy(INT32 nSessionID, Msg::Object objSrc, std_string & dbname/* = std::string()*/, CUtilChunk & value/* = CUtil::Chunk()*/, int value2/* = 0*/, unsigned int value22/* = 0*/, char valChar/* = char(0)*/)
{


	std::cout << "testMulitServerNode_RpcTimeoutProxy" << std::endl;
	ReturnNULL;
}
#include "GRpc.h"

Msg::ObjectMsgCall * GRpc::testMulitServerNode_RpcClient(INT32 nSessionID, Msg::Object objSrc, CUtilChunk & res/* = CUtil::Chunk()*/)
{

	++g_sBCheck3;

	ReturnNULL;
}

Msg::ObjectMsgCall * GRpc::testMulitServerNode_RpcTimeout(INT32 nSessionID, Msg::Object objSrc, std_string & dbname/* = std::string()*/, CUtilChunk & value/* = CUtil::Chunk()*/, int value2/* = 0*/, unsigned int value22/* = 0*/, char valChar/* = char(0)*/)
{


	std::cout << "testMulitServerNode_RpcTimeout" << std::endl;
	ReturnNULL;
}


//tool defaultParams define here.
static int g_rpcDefaultParam_int = 0;
static short g_rpcDefaultParam_short = 0;
static long g_rpcDefaultParam_long = 0;
static char g_rpcDefaultParam_char = char(0);
static unsigned int g_rpcDefaultParam_unsigned_int = 0;
static unsigned short g_rpcDefaultParam_unsigned_short = 0;
static unsigned char g_rpcDefaultParam_unsigned_char = 0;
static std::string g_rpcDefaultParam_std__string = std::string();
static time_t g_rpcDefaultParam_time_t = 0;
static UINT8 g_rpcDefaultParam_UINT8 = 0;
static SINT8 g_rpcDefaultParam_SINT8 = 0;
static UINT16 g_rpcDefaultParam_UINT16 = 0;
static INT16 g_rpcDefaultParam_INT16 = 0;
static UINT32 g_rpcDefaultParam_UINT32 = 0;
static INT32 g_rpcDefaultParam_INT32 = 0;
static UINT64 g_rpcDefaultParam_UINT64 = 0;
static INT64 g_rpcDefaultParam_INT64 = 0;
static double g_rpcDefaultParam_double = 0.0f;
static float g_rpcDefaultParam_float = 0.0f;
static std_string g_rpcDefaultParam_std_string = std::string();
static CUtilChunk g_rpcDefaultParam_CUtilChunk = CUtil::Chunk();
static TestRpcData g_rpcDefaultParam_TestRpcData = TestRpcData();
static TestRpcData2 g_rpcDefaultParam_TestRpcData2 = TestRpcData2();
static std::vector<INT32> g_rpcDefaultParam_std__vector_INT32_;

void FooRpcInterface::OnRegisterRpcs(void)
{
	Assert(m_pRpcManager && Msg::RpcCheckParams::GetInstance());
	static GRpc g_pGRpc(Msg::DEFAULT_RPC_CALLABLE_ID, m_pRpcManager);

	//tool testMulitServerNode generate default deliver and return check param here
	{
		CUtil::Parameters objDeliverParams, objReturnParams;
		CUtil::GenParamHelper::GenParams(objDeliverParams, g_rpcDefaultParam_std_string, g_rpcDefaultParam_CUtilChunk, g_rpcDefaultParam_int, g_rpcDefaultParam_unsigned_int, g_rpcDefaultParam_char);
		CUtil::GenParamHelper::GenParams(objReturnParams, g_rpcDefaultParam_CUtilChunk);
		Msg::RpcCheckParams::GetInstance()->InsertDeliverParams("testMulitServerNode", objDeliverParams);
		Msg::RpcCheckParams::GetInstance()->InsertReturnParams("testMulitServerNode", objReturnParams);

		m_pRpcManager->RegisterFunc<FooHandler >(Msg::g_sztestMulitServerNode_RpcServerProxy, &FooHandler::testMulitServerNode_RpcServerProxy);
		m_pRpcManager->RegisterFunc<FooHandler >(Msg::g_sztestMulitServerNode_RpcClientProxy, &FooHandler::testMulitServerNode_RpcClientProxy);
		m_pRpcManager->RegisterFunc<FooHandler >(Msg::g_sztestMulitServerNode_RpcTimeoutProxy, &FooHandler::testMulitServerNode_RpcTimeoutProxy);
	}

	//tool testParamsAndRpcDatas generate default deliver and return check param here
	{
		CUtil::Parameters objDeliverParams, objReturnParams;
		CUtil::GenParamHelper::GenParams(objDeliverParams, g_rpcDefaultParam_TestRpcData, g_rpcDefaultParam_TestRpcData2, g_rpcDefaultParam_std__vector_INT32_);
		CUtil::GenParamHelper::GenParams(objReturnParams, g_rpcDefaultParam_INT32);
		Msg::RpcCheckParams::GetInstance()->InsertDeliverParams("testParamsAndRpcDatas", objDeliverParams);
		Msg::RpcCheckParams::GetInstance()->InsertReturnParams("testParamsAndRpcDatas", objReturnParams);
		m_pRpcManager->RegisterFunc<FooHandler >(Msg::g_sztestParamsAndRpcDatas_RpcServer, &FooHandler::testParamsAndRpcDatas_RpcServer);
	}

	FooHandler::InitObjectFuncs();
}

void BarRpcInterface::OnRegisterRpcs(void)
{
	Assert(m_pRpcManager && Msg::RpcCheckParams::GetInstance());
	static GRpc g_pGRpc(Msg::DEFAULT_RPC_CALLABLE_ID, m_pRpcManager);

	//tool testMulitServerNode generate default deliver and return check param here
	{
		CUtil::Parameters objDeliverParams, objReturnParams;
		CUtil::GenParamHelper::GenParams(objDeliverParams, g_rpcDefaultParam_std_string, g_rpcDefaultParam_CUtilChunk, g_rpcDefaultParam_int, g_rpcDefaultParam_unsigned_int, g_rpcDefaultParam_char);
		CUtil::GenParamHelper::GenParams(objReturnParams, g_rpcDefaultParam_CUtilChunk);
		Msg::RpcCheckParams::GetInstance()->InsertDeliverParams("testMulitServerNode", objDeliverParams);
		Msg::RpcCheckParams::GetInstance()->InsertReturnParams("testMulitServerNode", objReturnParams);

		m_pRpcManager->RegisterFunc<GRpc >(Msg::g_sztestMulitServerNode_RpcClient, &GRpc::testMulitServerNode_RpcClient);
		m_pRpcManager->RegisterFunc<GRpc >(Msg::g_sztestMulitServerNode_RpcTimeout, &GRpc::testMulitServerNode_RpcTimeout);
		m_pRpcManager->RegisterFunc<BarHandler >(Msg::g_sztestMulitServerNode_RpcServer, &BarHandler::testMulitServerNode_RpcServer);
	}

	BarHandler::InitObjectFuncs();
}

void ProxyRpcInterface::OnRegisterRpcs(void)
{
	Assert(m_pRpcManager && Msg::RpcCheckParams::GetInstance());
	static GRpc g_pGRpc(Msg::DEFAULT_RPC_CALLABLE_ID, m_pRpcManager);

	//tool testMulitServerNode generate default deliver and return check param here
	{
		CUtil::Parameters objDeliverParams, objReturnParams;
		CUtil::GenParamHelper::GenParams(objDeliverParams, g_rpcDefaultParam_std_string, g_rpcDefaultParam_CUtilChunk, g_rpcDefaultParam_int, g_rpcDefaultParam_unsigned_int, g_rpcDefaultParam_char);
		CUtil::GenParamHelper::GenParams(objReturnParams, g_rpcDefaultParam_CUtilChunk);
		Msg::RpcCheckParams::GetInstance()->InsertDeliverParams("testMulitServerNode", objDeliverParams);
		Msg::RpcCheckParams::GetInstance()->InsertReturnParams("testMulitServerNode", objReturnParams);

		m_pRpcManager->RegisterFunc<ProxyHandler >(Msg::g_sztestMulitServerNode_RpcServerProxy, &ProxyHandler::testMulitServerNode_RpcServerProxy);
		m_pRpcManager->RegisterFunc<ProxyHandler >(Msg::g_sztestMulitServerNode_RpcClientProxy, &ProxyHandler::testMulitServerNode_RpcClientProxy);
		m_pRpcManager->RegisterFunc<ProxyHandler >(Msg::g_sztestMulitServerNode_RpcTimeoutProxy, &ProxyHandler::testMulitServerNode_RpcTimeoutProxy);
	}

	//tool testParamsAndRpcDatas generate default deliver and return check param here
	{
		CUtil::Parameters objDeliverParams, objReturnParams;
		CUtil::GenParamHelper::GenParams(objDeliverParams, g_rpcDefaultParam_TestRpcData, g_rpcDefaultParam_TestRpcData2, g_rpcDefaultParam_std__vector_INT32_);
		CUtil::GenParamHelper::GenParams(objReturnParams, g_rpcDefaultParam_INT32);
		Msg::RpcCheckParams::GetInstance()->InsertDeliverParams("testParamsAndRpcDatas", objDeliverParams);
		Msg::RpcCheckParams::GetInstance()->InsertReturnParams("testParamsAndRpcDatas", objReturnParams);

		m_pRpcManager->RegisterFunc<ProxyHandler >(Msg::g_sztestParamsAndRpcDatas_RpcClient, &ProxyHandler::testParamsAndRpcDatas_RpcClient);
		m_pRpcManager->RegisterFunc<ProxyHandler >(Msg::g_sztestParamsAndRpcDatas_RpcTimeout, &ProxyHandler::testParamsAndRpcDatas_RpcTimeout);
	}

	ProxyHandler::InitObjectFuncs();
}

CollectionObjectFuncsT GRpc::s_setFuncs;
CollectionObjectFuncsT FooHandler::s_setFuncs;
CollectionObjectFuncsT ProxyHandler::s_setFuncs;
CollectionObjectFuncsT BarHandler::s_setFuncs;


TEST(RPC , rpcTest)
{
// 	ThreadPool::ThreadPoolInterface::GetInstance().Init(std::map<UINT32, UINT32>());
// 	ThreadPool::ThreadPoolInterface::GetInstance().Startup();
// 	Json::Value root;
// 	Json::Reader reader;
// 	if (!reader.parse(s_parse, root))
// 	{
// 		std::cerr << "Parse Configure File Failed:" << reader.getFormatedErrorMessages() << std::endl;
// 	}
// 
// 	Json::Value proxy = root["proxy"];
// 	ProxyRpcInterface::GetInstance().Init(proxy);
// 
// 	Json::Value bar = root["bar"];
// 	BarRpcInterface::GetInstance().Init(bar);
// 
// 	Timer::sleep(100);
// 
// 	Json::Value foo = root["foo"];
// 	FooRpcInterface::GetInstance().Init(foo);
// 	static FooHandler fooHandler(FOO_HANDLER_ID, &(FooRpcInterface::GetInstance()), -1);
// 	static BarHandler barHandler(BAR_HANDLER_ID, &(BarRpcInterface::GetInstance()));
// 	static ProxyHandler proxyHandler(PROXY_HANDLER_ID, &(ProxyRpcInterface::GetInstance()) , -1);
// 	
// 	while (1)
// 	{
// 		if (g_sBCheck3)
// 		{
// 			break;
// 		}
// 		FooRpcInterface::GetInstance().Update();
// 	}
// 
// 	EXPECT_EQ(g_rpcCallbackTest.use_count(), 1);
// 	BarRpcInterface::GetInstance().Cleanup();
// 	ProxyRpcInterface::GetInstance().Cleanup();
// 	FooRpcInterface::GetInstance().Cleanup();
// 
// 	EXPECT_EQ(g_sBCheck1, true);
// 	EXPECT_EQ(g_sBCheck3, 1);
}
