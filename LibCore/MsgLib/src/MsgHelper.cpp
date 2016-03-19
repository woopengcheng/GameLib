#include "MsgLib/inc/MsgHelper.h"

namespace Msg
{
	BOOL MsgHelper::GenMsgCall( ObjectMsgCall & objMsgCall , const char * pMsgCallName , std::vector<Object> & vtTargets , Object objSrc  , UINT16 usPriority )
	{
		objMsgCall.m_objSource = objSrc;
		objMsgCall.m_usPriority = usPriority;
		memcpy(objMsgCall.m_szMsgMethod , pMsgCallName , MAX_MSG_METHOD_NAME_LENGTH); 

		if (vtTargets.size() == 0)
		{
			return FALSE;
		} 

		objMsgCall.SetTargetsCount((UINT32)vtTargets.size());
		std::vector<Object>::iterator iter = vtTargets.begin();
		for (UINT32 i = 0 ;iter != vtTargets.end();++iter , ++i)
		{
			objMsgCall.m_aTargets[i] = *iter; 
		}

		return TRUE;
	} 


	BOOL MsgHelper::GenMsgCall( ObjectMsgCall & objMsgCall, const char * pMsgCallName , UINT16 usPriority)
	{
		objMsgCall.m_usPriority = usPriority;

		memcpy(objMsgCall.m_szMsgMethod , pMsgCallName , MAX_MSG_METHOD_NAME_LENGTH); 

		return TRUE;
	} 


	BOOL MsgHelper::GenMsgCall( RPCMsgCall & objMsgCall , BOOL bRecvRequest/* = FALSE*/ , UINT64 ullTimeout/* = 10*/ , UINT64 ullMsgID/* = -1*/ , EMSG_SYNC_TYPE objSyncType /*= SYNC_TYPE_NONSYNC*/)
	{
		objMsgCall.SetSyncType(objSyncType);
		objMsgCall.m_bClientRequest = bRecvRequest;
		objMsgCall.m_ullTimeout = ullTimeout; 

		if (ullMsgID == 0)
		{
			objMsgCall.m_ullMsgID = (UINT64)((CUtil::GenerateUUIDByTime())); 
		} 
		else
		{
			objMsgCall.m_ullMsgID = ullMsgID;
		}

		return TRUE;
	} 

	std::string MsgHelper::GenerateSuffixNodeName(const std::string & strNodeName)
	{
		std::string strName = "_To_" + strNodeName;
		return strName;
	}

	BOOL MsgHelper::IsSimilarWithNodeName(const std::string & strNodeName , const std::string & strSuffixNodeName)
	{
		std::string strName = MsgHelper::GenerateSuffixNodeName(strSuffixNodeName);
		if (strNodeName.find(strName))
		{
			return TRUE;
		}

		return FALSE;
	}

	std::string MsgHelper::GetSuffixNodeName(const std::string & strNodeName)
	{
		size_t nPos = strNodeName.find("_To_")+sizeof("_To_")-1;
		if (std::string::npos == nPos)
		{
			return std::string();
		}
		size_t nLength = strNodeName.length();
		std::string strName = strNodeName.substr(nPos , nLength - nPos);

		return strName;
	}

	std::string MsgHelper::ExchangeNodeName(const std::string & strNodeName)
	{
		INT32 nExtraSize = sizeof("_To_") - 1;
		size_t nPos = strNodeName.find("_To_");
		if (std::string::npos == nPos)
		{
			return std::string();
		}
		size_t nLength = strNodeName.length();
		std::string strName = strNodeName.substr(nPos + nExtraSize, nLength);
		strName += "_To_";
		strName += strNodeName.substr(0, nPos);

		return strName;

	}

	std::string MsgHelper::GeneratePeerInfoKey(const std::string & strCurNodeName, const std::string & strRemoteNodeName)
	{
		std::string strName = strCurNodeName;
		strName += "_To_";
		strName += strRemoteNodeName;

		return strName;
	}

}