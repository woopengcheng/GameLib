#ifndef __cutil_cobject_h__
#define __cutil_cobject_h__
#include "CUtil/inc/Common.h"
#include "CUtil/inc/CRTTI.h"

namespace CUtil
{
	class CObject
	{
	public:
		static CUtil::CRTTI	*	GetClass(){ static CUtil::CRTTI	sClassCObject("CObject", 0 , NULL , sizeof(CObject));return &sClassCObject; }
		virtual CUtil::CRTTI*	GetRTTI(){return CObject::GetClass(); }

	public:
		virtual ~CObject() = 0{;}

	};

}


#endif