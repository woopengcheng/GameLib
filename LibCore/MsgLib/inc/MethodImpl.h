#ifndef __msg_method_impl_h__
#define __msg_method_impl_h__
#include "MsgLib/inc/ObjectMsgCall.h"

namespace Msg 
{  
	 class ParaseMsgCall;

	enum EMethodType
	{
		METHOD_TYPE_ERROR = 0,
		METHOD_TYPE_STATIC ,    //5 静态函数调用类型
		METHOD_TYPE_OBJECT ,    //5 类对象调用类型
	};
	  
	//5 这里需要对函数进行封装.因为每个消息的具体函数不能通过统一的函数模板的方式实现.
	 
	class DLL_EXPORT MethodImpl
	{ 
	public:
		typedef ObjectMsgCall * (*pfnMethodImpl)(ParaseMsgCall * pParseMsgCall);

	public:
		 UINT8          m_cMethodType;								//5 处理的消息函数的类型
		 char           m_pMehtodName[MAX_MSG_METHOD_NAME_LENGTH];	//5 处理函数的名字
		 pfnMethodImpl  m_pMethodImpl;								//5 对消息函数的一个封装. 
		 BOOL			m_bEnable;									//5 是否启用当前RPC,在脚本配置.
	};
	 

	template< typename Func>
	class StaticMethodImpl : public MethodImpl
	{
	public:
		Func m_objFunc;    //5 这里采用模板的方式来保存每个消息的函数.
	};

	template< typename ClassObject , typename Func>
	class ObjectMethodImpl : public MethodImpl
	{
	public:
		ClassObject * m_pObj;       //5 处理消息的具体对象.
		Func          m_objFunc;    //5 这里采用模板的方式来保存每个消息的函数.
	};
}

#endif
