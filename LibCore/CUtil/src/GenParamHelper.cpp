#include "CUtil/inc/GenParamHelper.h"
#include "CUtil/inc/Parameters.h"

namespace CUtil
{
	UINT32 GenParamHelper::GenParams(CUtil::Parameters & objParams)
	{
		objParams.m_unParamCount = 0; 
		return 0;
	}

	BOOL GenParamHelper::CheckParams(CUtil::Parameters & objParams)
	{
		bool bResult = true;
		if(objParams.m_unParamCount != 0)
		{
			bResult = false;
		}    

		return bResult;
	}
}