#include "CUtil/inc/AutomicUInt.h"

#ifdef WIN32
#include "windows.h"
#endif

namespace CUtil
{

#if defined(_WIN32)
	void AtomicUInt::set(unsigned newX) {
		InterlockedExchange((volatile long *)&x, newX);
	}

	AtomicUInt AtomicUInt::operator++() {
		return InterlockedIncrement((volatile long*)&x);
	}
	AtomicUInt AtomicUInt::operator++(int) {
		return InterlockedIncrement((volatile long*)&x)-1;
	}
	AtomicUInt AtomicUInt::operator--() {
		return InterlockedDecrement((volatile long*)&x);
	}
	AtomicUInt AtomicUInt::operator--(int) {
		return InterlockedDecrement((volatile long*)&x)+1;
	}
# if defined(_WIN64)
	// don't see an InterlockedAdd for _WIN32...hmmm
	void AtomicUInt::signedAdd(int by) {
		InterlockedAdd((volatile long *)&x,by);
	}
# endif
#elif defined(__GCC_HAVE_SYNC_COMPARE_AND_SWAP_4)
	// this is in GCC >= 4.1
	void AtomicUInt::set(unsigned newX) { __sync_synchronize(); x = newX; }
	AtomicUInt AtomicUInt::operator++() {
		return __sync_add_and_fetch(&x, 1);
	}
	AtomicUInt AtomicUInt::operator++(int) {
		return __sync_fetch_and_add(&x, 1);
	}
	AtomicUInt AtomicUInt::operator--() {
		return __sync_add_and_fetch(&x, -1);
	}
	AtomicUInt AtomicUInt::operator--(int) {
		return __sync_fetch_and_add(&x, -1);
	}
	void AtomicUInt::signedAdd(int by) {
		__sync_fetch_and_add(&x, by);
	}
#elif defined(__GNUC__)  && (defined(__i386__) || defined(__x86_64__))
	void AtomicUInt::set(unsigned newX) {
		asm volatile("mfence" ::: "memory");
		x = newX;
	}

	// from boost 1.39 interprocess/detail/atomic.hpp
	unsigned atomic_int_helper(volatile unsigned *x, int val) {
		int r;
		asm volatile
			(
			"lock\n\t"
			"xadd %1, %0":
		"+m"( *x ), "=r"( r ): // outputs (%0, %1)
		"1"( val ): // inputs (%2 == %1)
		"memory", "cc" // clobbers
			);
		return r;
	}
	AtomicUInt AtomicUInt::operator++() {
		return atomic_int_helper(&x, 1)+1;
	}
	AtomicUInt AtomicUInt::operator++(int) {
		return atomic_int_helper(&x, 1);
	}
	AtomicUInt AtomicUInt::operator--() {
		return atomic_int_helper(&x, -1)-1;
	}
	AtomicUInt AtomicUInt::operator--(int) {
		return atomic_int_helper(&x, -1);
	}
	void AtomicUInt::signedAdd(int by) {
		atomic_int_helper(&x, by);
	}
#else
#  error "unsupported compiler or platform"
#endif
}