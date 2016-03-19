#include "GameDB/inc/UserAuth.h"

namespace GameDB
{


	UserAuth::UserAuth(const User & objUserInfos)
		: m_objUserInfo(objUserInfos)
	{

	}

	UserAuth::~UserAuth()
	{

	}

	bool UserAuth::CheckSysPermission()
	{
		return m_objUserInfo.get_sysuser();
	}

}