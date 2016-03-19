#ifndef __cutil_automic_unsigned_int_h__
#define __cutil_automic_unsigned_int_h__ 
#include "CUtil/inc/Common.h" 


namespace CUtil {

    /**
     * An unsigned integer supporting atomic read-modify-write operations.
     *
     * Many operations on these types depend on natural alignment (4 byte alignment for 4-byte
     * words, i.e.).
     */
    struct AtomicUInt {
        AtomicUInt() : x(0) {}
        AtomicUInt(unsigned z) : x(z) { }

        operator unsigned() const { return x; }
        unsigned get() const { return x; }
        void set(unsigned newX);

        AtomicUInt operator++(); // ++prefix
        AtomicUInt operator++(int);// postfix++
        AtomicUInt operator--(); // --prefix
        AtomicUInt operator--(int); // postfix--
        void signedAdd(int by);
        void zero() { set(0); }
        volatile unsigned x;
    };

} // namespace CUtil

#endif
