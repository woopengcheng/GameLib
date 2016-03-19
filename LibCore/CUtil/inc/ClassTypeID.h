#ifndef __class_type_id_h__
#define __class_type_id_h__ 

namespace  
{ 
#ifdef WIN32 
	typedef unsigned __int64   class_type_id;
#else 
	typedef unsigned long long class_type_id;
#endif

	const class_type_id CTID_MsgTimerNode        = 0x00000001;
	const class_type_id CTID_Node		         = 0x00000002;
	const class_type_id CTID_TimerNode	         = 0x00000004;

	const class_type_id CTID_MsgCall	         = 0x00000008;
	const class_type_id CTID_ObjectMsgCall       = 0x00000010;
	const class_type_id CTID_TimingWheelTimerNode= 0x00000020;
}
#endif