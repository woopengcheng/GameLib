#ifndef __cutil_marshal_h__
#define __cutil_marshal_h__
#include "CUtil/inc/Common.h"
#include "LogLib/inc/Log.h" 


namespace CUtil
{
	class CStream;

	class DLL_EXPORT Marshal
	{
	public:
		enum Transaction
		{
			Begin = 0 , 
			Rollback ,
			End , 
		};
	public:
		virtual CStream & marshal(CStream & cs) const = 0;
		virtual CStream & unMarshal(CStream & cs) = 0;
	};
}

#endif