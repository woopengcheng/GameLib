#ifndef __cutil_byte_order_h__
#define __cutil_byte_order_h__
#include "CUtil/inc/Common.h" 

#define CUTIL_BIG_ENDIAN (0x44434241)
#define CUTIL_LITTLE_ENDIAN (0x41424344)
#define CUTIL_PDP_ENDIAN (0x42414443)
#define CUR_ENDIAN ('ABCD')

#if CUR_ENDIAN == CUTIL_BIG_ENDIAN
#define USE_CUTIL_BIG_ENDIAN
#elif CUTIL_LITTLE_ENDIAN==CUR_ENDIAN
#define USE_CUTIL_LITTLE_ENDIAN
#elif CUTIL_PDP_ENDIAN == CUR_ENDIAN
#define USE_CUTIL_PDP_ENDIAN
#endif

inline char IsLittleEndian()
{
	static char c = -1;
	if (c == -1)
	{
		static int i = 0x12345678;
		char * p = (char *)(&i);
		if (*p == 0x78)
		{
			c = 1;
		}
		else
		{
			c = 0;
		}
	}

	return c;
}
 
#if defined USE_CUTIL_LITTLE_ENDIAN
	#define byteorder_16(x)	(x)
	#define byteorder_32(x)	(x)
	#define byteorder_64(x)	(x)
#elif defined __GNUC__
	/*
	#define byteorder_16 htons
	#define byteorder_32 htonl
	*/
	inline unsigned short byteorder_16(unsigned short x)
	{
		register unsigned short v;
		__asm__ __volatile__ ("xchgb %b0, %h0" : "=a"(v) : "0"(x));
		return v;
	}
	inline unsigned int byteorder_32(unsigned int x)
	{
		register unsigned int v;
		__asm__ ("bswapl %0" : "=r"(v) : "0"(x));
		return v;
	}
	inline unsigned long byteorder_64(unsigned long x)
	{
		register unsigned long v;
		__asm__("bswapq %0":"=r"(v):"0"(x));
		return v;
	}

#elif defined WIN32 
	inline unsigned __int16 byteorder_16(unsigned __int16 x)
	{  
		__asm ror x, 8
		return x;
	}
	inline unsigned __int32 byteorder_32(unsigned __int32 x)
	{
		__asm mov eax, x
		__asm bswap eax
		__asm mov x, eax
		return x;
	}
	inline unsigned __int64 byteorder_64(unsigned __int64 x)
	{
		__asm mov rax, x
		__asm bswap rax
		__asm mov x,rax
		return x;
	}
#endif

#endif