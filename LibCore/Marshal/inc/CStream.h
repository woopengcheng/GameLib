#ifndef __cutil_c_stream_h__
#define __cutil_c_stream_h__
#include "CUtil/inc/Common.h"
#include "CUtil/inc/Chunk.h"
#include "CUtil/inc/ByteOrder.h"
#include "Marshal/inc/Marshal.h"
#include "Marshal/inc/CommonMarshal.h"
#pragma warning(disable:4146)
namespace Msg
{
	class Object;
}

namespace CUtil
{
	template<typename T>
	inline T& remove_const(const T &t) { return const_cast<T&>(t); }

	class DLL_EXPORT CStream
	{
	public:
		class Exception
		{
		public: 
			std::string what;
			Exception (){}
			Exception(const char *t):what(t){}
		};

	public:
		CStream()
			: m_nCurPos(0)
			, m_nTransactionPos(0)
		{

		}

		CStream(const CStream & cs) 
		{
			m_nCurPos = cs.GetCurPos();
			m_nTransactionPos = cs.GetTransactionPos();
			m_objChunk = cs.GetData();
		}

		CStream(const void * pBuf , UINT32 unLength)
			: m_nCurPos(0)
			, m_nTransactionPos(0)
			, m_objChunk(pBuf , unLength)
		{

		}
		CStream(const CUtil::Chunk & objChunk)
			: m_nCurPos(0)
			, m_nTransactionPos(0)
			, m_objChunk(objChunk)
		{

		}
		~CStream(){}

		void Copy(const CStream & cs) 
		{
			m_nCurPos = cs.GetCurPos();
			m_nTransactionPos = cs.GetTransactionPos();
			m_objChunk = cs.GetData();
		}

	public:
		CStream & operator = (const CStream & cs)
		{
			m_nCurPos = cs.GetCurPos();
			m_nTransactionPos = cs.GetTransactionPos();
			m_objChunk = cs.GetData();

			return *this;
		}
		bool operator !=(const CStream & objStream) const
		{
			return m_objChunk!=objStream.GetData() || m_nCurPos != objStream.GetCurPos() || m_nTransactionPos != objStream.GetTransactionPos();
		}
		bool operator ==(const CStream & objStream) const
		{
			return m_objChunk==objStream.GetData() && m_nCurPos == objStream.GetCurPos() && m_nTransactionPos == objStream.GetTransactionPos();
		}
		friend	std::ostream&	operator<<(std::ostream&os,const CStream & objStream)
		{

			return os << "CStream:cur_pos=" << objStream.GetCurPos() << ":datalen=" << objStream.GetDataLen() << ":data=" << objStream.GetData().Begin();
		}

	public:
		template<typename T>
		CStream & Pushback(T & t)
		{
			m_objChunk.Pushback((void*)&t , sizeof(t));

			return * this;
		}
		 
		CStream & Pushback(void * pBuf , UINT32 unSize)
		{
			m_objChunk.Pushback(pBuf , unSize); 

			return * this;
		}

//		unsigned char PopByte8()
//		{
//			unsigned char c;
//			Pop(c);
//			return c;
//		}
//		unsigned short PopByte16()
//		{
//			unsigned short s;
//			Pop(s);
//			return byteorder_16(s);
//		}
//		unsigned int PopByte32()
//		{
//			unsigned int l;
//			Pop(l);
//			return byteorder_32(l);
//		}
//		UINT64 pop_byte_64()
//		{
//			UINT64 ll;
//			Pop(ll);
//			return byteorder_64(ll);
//		}
// 		CStream & Pushback16(INT16 x)
// 		{ 
// 
// 			return * this;
// 		}
// 		CStream & Pushback16(UINT16 x)
// 		{ 
// 
// 			return * this;
// 		}
// 		CStream & Pushback32(UINT32 x)//change unsigned int to size_t to forbid convert from size_t to 
// 		{
// 			if (x>UINT_MAX) 
// 				throw CStream::Exception("size of x is bigger than UINT_MAX in Marshal::compact_uint32\n");
// 			if (x < 0x80) return Pushback((unsigned char)x);
// 			else if (x < 0x4000) return Pushback(byteorder_16(x|0x8000));
// 			else if (x < 0x20000000) return Pushback(byteorder_32(x|0xc0000000));
// 			Pushback((unsigned char)0xe0);
// 			return Pushback(byteorder_32(x)); 
// 		}
// 		CStream & Pushback32(INT32 x)//change unsigned int to size_t to forbid convert from size_t to 
// 		{
// 			if (x >= 0)
// 			{
// 				if (x < 0x40) return Pushback((unsigned char)x);
// 				else if (x < 0x2000) return Pushback(byteorder_16(x|0x8000));
// 				else if (x < 0x10000000) return Pushback(byteorder_32(x|0xc0000000));
// 				Pushback((unsigned char)0xe0);
// 				return Pushback(byteorder_32(x));
// 			}
// 			if (-x > 0)
// 			{
// 				x = -x;
// 				if (x < 0x40) return Pushback((unsigned char)x|0x40);
// 				else if (x < 0x2000) return Pushback(byteorder_16(x|0xa000));
// 				else if (x < 0x10000000) return Pushback(byteorder_32(x|0xd0000000));
// 				Pushback((unsigned char)0xf0);
// 				return Pushback(byteorder_32(x));
// 			}
// 			Pushback((unsigned char)0xf0);
// 			return Pushback(byteorder_32(x));
// 		}
// 		CStream & Pushback64(void * pBuf , UINT32 unSize)
// 		{
// 			m_objChunk.Pushback(pBuf , unSize); 
// 
// 			return * this;
// 		}
// 
// 		CStream & Pop32(UINT32 & x)
// 		{
// 			MsgAssert_Re (m_nCurPos + sizeof(x) <= m_objChunk.GetDataLen() , *this , "CStream pop error.");
// 
// 			switch ( *( (char *)m_objChunk.Begin() + m_nCurPos) & 0xe0)
// 			{
// 			case 0xe0:
// 				PopByte8();
// 				x = PopByte32();
// 				return *this;
// 			case 0xc0:
// 				x = PopByte32() & ~0xc0000000;
// 				return *this;
// 			case 0xa0:
// 			case 0x80:
// 				x = PopByte16() & ~0x8000;
// 				return *this;
// 			}
// 			x = PopByte8();
// 			return *this;

// 		}
// 		CStream & Pop32(INT32 & x)
// 		{
// 			MsgAssert_Re (m_nCurPos + sizeof(x) <= m_objChunk.GetDataLen() , *this , "CStream pop error.");
// 			 
// 			switch ( *( (char *)m_objChunk.Begin() + m_nCurPos) & 0xf0)
// 			{
// 			case 0xf0:
// 				PopByte8();
// 				x = -(PopByte32());
// 				return *this;
// 			case 0xe0:
// 				PopByte8();
// 				x = PopByte32();
// 				return *this;
// 			case 0xd0:
// 				x = -(PopByte32() & ~0xd0000000);
// 				return *this;
// 			case 0xc0:
// 				x = PopByte32() & ~0xc0000000;
// 				return *this;
// 			case 0xb0:
// 			case 0xa0:
// 				x = -(PopByte16() & ~0xa000);
// 				return *this;
// 			case 0x90:
// 			case 0x80:
// 				x = PopByte16() & ~0x8000;
// 				return *this;
// 			case 0x70:
// 			case 0x60:
// 			case 0x50:
// 			case 0x40:
// 				x = -(PopByte8() & ~0x40);
// 				return *this;
// 			}
// 			x = PopByte8();
// 			return *this;
// 		}
 
		template<typename T>
		CStream & Pop(T & t)
		{
			MsgAssert_Re (m_nCurPos + sizeof(t) <= m_objChunk.GetDataLen() , *this , "CStream pop error.");

			t = *(T*)( (char *)m_objChunk.Begin() + m_nCurPos);
			m_nCurPos += sizeof(t);

			return * this;
		}

		CStream & Pop(void *& pBuf , UINT32 unSize)
		{
			MsgAssert_Re (m_nCurPos + unSize <= m_objChunk.GetDataLen() , *this , "CStream pop error.");
			pBuf = ( (char *)m_objChunk.Begin() + m_nCurPos);
			m_nCurPos += unSize;

			return * this;
		}
	public:
		CStream & operator << (char t)				{ return Pushback(t);}  
		CStream & operator << (UINT8 t)				{ return Pushback(t);} 
		CStream & operator << (bool t)				{ return Pushback(t);} 
		CStream & operator << (INT16 t)				{ return Pushback(t);} 
		CStream & operator << (UINT16 t)			{ return Pushback(t);} 
		CStream & operator << (INT32 t)				{ return Pushback(t);} 
		CStream & operator << (UINT32 t)			{ return Pushback(t);} 
		CStream & operator << (INT64 t)				{ return Pushback(t);} 
		CStream & operator << (UINT64 t)			{ return Pushback(t);} 
		CStream & operator << (float t)				{ return Pushback(t);} 
		CStream & operator << (double t)			{ return Pushback(t);} 
		CStream & operator << (const Marshal & t)			{ return t.marshal(*this); } 
		CStream & operator << (const Chunk & t);	 
		CStream & operator << (const Msg::Object & t);	 
		CStream & operator << (const CStream & t);	 

		template<typename T> CStream & operator << (const std::basic_string<T> & t)
		{
			STATIC_ASSERT(sizeof(T) == 1);  //5 测试是否是单字节.utf16和utf32.单独处理
			UINT32 unBytes = (UINT32)(t.size() )* sizeof(T);
			Pushback(unBytes);
//			m_objChunk.Insert(m_objChunk.End() , (void*)t.c_str() , (UINT32)unBytes);
			m_objChunk.Pushback((void*)t.c_str() , (UINT32)unBytes);

			return *this;
		}
// 		template<typename T> CStream & operator << (const std::basic_string<T> & t)
// 		{
// 			return operator << (*const_cast<std::basic_string<T>*>(&t)); 
// 		}

		template<typename T1 , typename T2> CStream & operator << (const std::pair<T1 , T2> & t)
		{
			return *this << t.first << t.second;
		} 

		template<typename T1> CStream & operator << (const std::vector<T1> & t)
		{
			return *this << STLContainer<std::vector<T1>>(remove_const(t));
		} 

		template<typename T1> CStream & operator << (const std::list<T1> & t)
		{
			return *this << STLContainer<std::list<T1>>(remove_const(t));
		} 
		
		template<typename T1> CStream & operator << (const std::deque<T1> & t)
		{
			return *this << STLContainer<std::deque<T1>>(remove_const(t));
		} 

		template<typename T1> CStream & operator << (const std::set<T1> & t)
		{
			return *this << STLContainer<std::set<T1>>(remove_const(t));
		} 

		template<typename T1 , typename T2> CStream & operator << (const std::map<T1 , T2> & t)
		{
			return *this << STLContainer<std::map<T1 , T2> >(remove_const(t));
		} 

	public:
		CStream & operator >> (char & t)			{ return Pop(t);}  
		CStream & operator >> (UINT8 & t)			{ return Pop(t);} 
		CStream & operator >> (bool & t)			{ return Pop(t);} 
		CStream & operator >> (INT16 & t)			{ return Pop(t);} 
		CStream & operator >> (UINT16 & t)			{ return Pop(t);} 
		CStream & operator >> (long & t)			{ return Pop(t);} 
		CStream & operator >> (INT32 & t)			{ return Pop(t);} 
		CStream & operator >> (UINT32 & t)			{ return Pop(t);} 
		CStream & operator >> (INT64 & t)			{ return Pop(t);} 
		CStream & operator >> (UINT64 & t)			{ return Pop(t);} 
		CStream & operator >> (float & t)			{ return Pop(t);} 
		CStream & operator >> (double & t)			{ return Pop(t);} 
		CStream & operator >> (Marshal & t)			{ return t.unMarshal(*this); } 
		CStream & operator >> (Chunk & t);
		CStream & operator >> (Msg::Object & t);
		CStream & operator >> (CStream & t);	 

		template<typename T> CStream & operator >> (std::basic_string<T> & t)
		{
			STATIC_ASSERT(sizeof(t) == 1);  //5 测试是否是单字节.utf16和utf32.单独处理
			
			UINT32 unBytes = 0;
			Pop(unBytes);

			MsgAssert_Re(!(unBytes % sizeof(T)) , *this , "unMarshal invalid length.");
			MsgAssert_Re(!(unBytes > m_objChunk.GetDataLen() - m_nCurPos) , *this , "unMarshal invalid length.");
			
			t.assign((T*)((char *)m_objChunk.Begin() + m_nCurPos) , unBytes / sizeof(T));
			m_nCurPos += unBytes;
			return *this;
		}
		 
		template<typename T1> CStream & operator >> (std::vector<T1> & t)
		{
			return *this >> STLContainer<std::vector<T1>>(t);
		} 

		template<typename T1> CStream & operator >> (std::list<T1> & t)
		{
			return *this >> STLContainer<std::list<T1>>(t);
		} 
		
		template<typename T1> CStream & operator >> (std::deque<T1> & t)
		{
			return *this >> STLContainer<std::deque<T1>>(t);
		} 

		template<typename T1> CStream & operator >> (std::set<T1> & t)
		{
			return *this >> STLContainer<std::set<T1>>(t);
		} 

		template<typename T1 , typename T2> CStream & operator >> (std::map<T1 , T2> & t)
		{
			return *this >> STLContainer<std::map<T1 , T2> >(t);
		} 

		template<typename T1 , typename T2> CStream & operator >> (std::pair<T1 , T2> & t)
		{
			return *this >> remove_const(t.first) >> remove_const(t.second);
		} 

		CStream & operator >> (Marshal::Transaction  t) 
		{
			switch(t)
			{
			case Marshal::Begin:
				{
					m_nTransactionPos = m_nCurPos;
				}break;
			case Marshal::Rollback:
				{
					m_nCurPos = m_nTransactionPos;
				}break;
			case Marshal::End:
				{

				}break;
			default:
				{
					MsgAssert_Re(0 , *this , "Marshal::Transaction type error.");
				}
			}
			return *this;
		}

		public: 
			UINT32  GetDataLen( void ) const { return m_objChunk.GetDataLen(); }
			INT32   GetCurPos( void )const { return m_nCurPos; }
			INT32   GetTransactionPos( void ) const { return m_nTransactionPos;	}
			Chunk & GetData( void ) { return m_objChunk;	}
			const Chunk & GetData( void ) const { return m_objChunk;	}
			void  * Begin() const { return m_objChunk.Begin(); }
			void  * End() const { return m_objChunk.End(); }
			void    Insert(void * pPos , void * pBegin , UINT32 unLen){ m_objChunk.Insert(pPos , pBegin , unLen); } 
			void    Clear(){ m_objChunk.Clear(); m_nCurPos = 0; m_nTransactionPos = 0; } 

	protected:
		INT32   m_nCurPos;			 //5 记录当前流的位置.
		INT32   m_nTransactionPos;   //5 将流作为事务处理.
		Chunk   m_objChunk;
	};
}


#endif