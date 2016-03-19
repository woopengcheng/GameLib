#ifndef __cutil_unit_test_h__
#define __cutil_unit_test_h__ 
#include "CUtil/inc/Common.h" 

namespace CUtil
{
	extern void DLL_EXPORT UnitTestStart(void);
	extern INT32 DLL_EXPORT GoogleTestStart(int argc, char **argv);
}

#endif