#ifndef __net_byte_order_h__
#define __net_byte_order_h__

#include "NetLib/inc/NetCommon.h" 

#ifdef WIN32
#include <Windows.h>
#endif

namespace Net
{
	extern UINT64 ntohll(UINT64 value);
	extern UINT64 htonll(UINT64 value);

	template<class T> struct Convert
	{
		static void ToHostOrder(T& val)
		{
			assert(false && "no impl");
		}

		static void ToNetworkOrder(T& val)
		{
			assert(false && "no impl");
		}
	};

	template<> struct Convert<UINT16>
	{
		static void ToHostOrder(UINT16 & val)
		{
			val = htons(val);
		}

		static void ToNetworkOrder(UINT16& val)
		{
			val = ntohs(val);
		}
	};

	template<> struct Convert<UINT32>
	{
		static void ToHostOrder(UINT32& val)
		{
			val = htonl(val);
		}

		static void ToNetworkOrder(UINT32& val)
		{
			val = ntohl(val);
		}
	};

	template<> struct Convert<UINT64>
	{
		static void ToHostOrder(UINT64& val)
		{
			val = htonll(val);
		}

		static void ToNetworkOrder(UINT64& val)
		{
			val = ntohll(val);
		}
	};

	template<> struct Convert<INT32>
	{
		static void ToHostOrder(INT32& val)
		{
			val = htonl(val);
		}

		static void ToNetworkOrder(INT32& val)
		{
			val = ntohl(val);
		}
	};

	UINT64 ntohll( UINT64 value ) 
	{
		enum { TYP_INIT, TYP_SMLE, TYP_BIGE };

		union
		{
			UINT64 ull;
			UINT8  c[8];
		} x;

		// Test if on Big Endian system.
		static int typ = TYP_INIT;

		if (typ == TYP_INIT)
		{
			x.ull = 0x01;
			typ = (x.c[7] == 0x01) ? TYP_BIGE : TYP_SMLE;
		}

		// System is Big Endian; return value as is.
		if (typ == TYP_BIGE)
		{
			return value;
		}

		// else convert value to Big Endian
		x.ull = value;

		SINT8 c = 0;
		c = x.c[0];
		x.c[0] = x.c[7];
		x.c[7] = c;
		c = x.c[1];
		x.c[1] = x.c[6];
		x.c[6] = c;
		c = x.c[2];
		x.c[2] = x.c[5];
		x.c[5] = c;
		c = x.c[3];
		x.c[3] = x.c[4];
		x.c[4] = c;

		return x.ull;
	}

	UINT64 htonll(UINT64 value)
	{
		return ntohll(value);
	}


}

#endif 
