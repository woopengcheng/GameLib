#ifndef __cutil_class_type_interface_h__
#define __cutil_class_type_interface_h__
#include "CUtil/inc/Common.h"

#define CLASS_TYPE_ID(CurClass , ParentClass , CurClassTypeID) CLASS_TYPE_ID_DECLARE(ParentClass::_CLASS_ALL_TYPE_ID , CurClassTypeID)
#define CLASS_TYPE_ID_DECLARE(ParentClassAllTypeID , CurClassTypeID)\
	public:\
	enum{_CLASS_ALL_TYPE_ID = (ParentClassAllTypeID | CurClassTypeID) , _CUR_CLASS_TYPE_ID = CurClassTypeID};\
	virtual BOOL IsKindOf(class_type_id llClassID){ return (llClassID & _CLASS_ALL_TYPE_ID) > 0; }\
	virtual class_type_id GetClassTypeID(){ return _CUR_CLASS_TYPE_ID;}\

namespace CUtil
{
	const class_type_id  CID_ClassTypeInterface = 0x00000000;

	/**
	 * @class : ClassTypeInterface
	 * @author: 5
	 * @date  : 2014年8月1日
	 * @file  : ClassType.h
	 * @brief : 用来对基类的指针类型进行转换.
	 */
	class DLL_EXPORT ClassTypeInterface
	{
		CLASS_TYPE_ID_DECLARE(0 , CID_ClassTypeInterface)
	public:
		template<typename T>
		T * GetClass()
		{
			if (IsKindOf(T::_CUR_CLASS_TYPE_ID))
			{
				return dynamic_cast<T*>(this);
			}
			else
				return NULL;
		}
	};
}

#endif