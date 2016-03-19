#ifndef __msg_assert_h__
#define __msg_assert_h__

#include <assert.h>
#include <iostream>
 
extern inline int * ASSERT_FAIL(){return NULL; }

#ifdef  _DEBUG
#define ASSERT (*ASSERT_FAIL() = 0)
#else
#define ASSERT 1

#endif
 
#define Assert(_Expression){ if(!(_Expression)){ std::cout << "Assert " << __FILE__ << ":" << __LINE__ << ", " << #_Expression; ASSERT; return ;} }
#define Assert_Re0(_Expression){ if(!(_Expression)){ std::cout << "Assert " << __FILE__ << ":" << __LINE__ << ", " << #_Expression; ASSERT; return 0;} } 
#define Assert_ReF(_Expression){ if(!(_Expression)){ std::cout << "Assert " << __FILE__ << ":" << __LINE__ << ", " << #_Expression; ASSERT; return CErrno::ERR_FAILURE;} }
#define Assert_ReF1(_Expression){ if(!(_Expression)){ std::cout << "Assert " << __FILE__ << ":" << __LINE__ << ", " << #_Expression; ASSERT; return -1;} } 
#define Assert_Re(_Expression , _ReturnValue){ if(!(_Expression)){ std::cout << "Assert " << __FILE__ << ":" << __LINE__ << ", " << #_Expression; ASSERT; return _ReturnValue;} }

#define MsgAssert(_Expression , Error){ if(!(_Expression)){ std::cout << "Assert " << __FILE__ << ":" << __LINE__ << ", " << #_Expression << " : " << Error; ASSERT; return ;} }
#define MsgAssert_Re0(_Expression , Error){ if(!(_Expression)){ std::cout << "Assert " << __FILE__ << ":" << __LINE__ << ", " << #_Expression << " : " << Error; ASSERT; return 0;} } 
#define MsgAssert_ReF(_Expression , Error){ if(!(_Expression)){ std::cout << "Assert " << __FILE__ << ":" << __LINE__ << ", " << #_Expression << " : " << Error; ASSERT; return CErrno::ERR_FAILURE;} } 
#define MsgAssert_ReF1(_Expression , Error){ if(!(_Expression)){ std::cout << "Assert " << __FILE__ << ":" << __LINE__ << ", " << #_Expression << " : " << Error; ASSERT; return -1;} } 
#define MsgAssert_Re(_Expression , _ReturnValue , Error){ if(!(_Expression)){ std::cout << "Assert " << __FILE__ << ":" << __LINE__ << ", " << #_Expression << " : " << Error; ASSERT; return _ReturnValue;} }

#endif