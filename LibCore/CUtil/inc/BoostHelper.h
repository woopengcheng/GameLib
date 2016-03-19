#ifndef __net_boost_helper_h__
#define __net_boost_helper_h__   
#include <boost/shared_ptr.hpp>
#include <boost/weak_ptr.hpp> 
#include <boost/enable_shared_from_this.hpp>
#include "boost/core/noncopyable.hpp"
#include "CUtil/inc/intrusive_ptr.h"
//#include "boost/intrusive_ptr.hpp"

#ifndef __SPU__
#include <boost/mem_fn.hpp>
#endif 

#define DECLARE_BOOST_POINTERS(name) \
	typedef boost::shared_ptr<name> name##Ptr; \
	typedef boost::shared_ptr<const name> name##ConstPtr; \
	typedef boost::weak_ptr<name> name##WeakPtr; \
	typedef boost::weak_ptr<const name> name##ConstWeakPtr;

#define DECLARE_BOOST_TEMPLATE_POINTERS(nametype , name) \
	typedef boost::shared_ptr<nametype > name##Ptr; \
	typedef boost::shared_ptr<const nametype > name##ConstPtr; \
	typedef boost::weak_ptr<nametype > name##WeakPtr; \
	typedef boost::weak_ptr<const nametype > name##ConstWeakPtr;

template<bool x> struct StaticFail;
template<> struct StaticFail<true>{};
template<int x> struct StaticAssertTest{};

#define STATIC_ASSERT(x) typedef StaticAssertTest<sizeof(StaticFail<(bool)x>)> StaticAssert_##__LINE__
 

#endif