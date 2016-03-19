#include "CUtil/inc/CircleBuffer.h"

namespace CUtil
{
	CircleBuffer::CircleBuffer( void )
		: m_pDataHead(NULL)
		, m_pDataStartPos(NULL)
		, m_unDataLength(0)
		, m_unTotalSize(0)
	{
	}

	CircleBuffer::~CircleBuffer( void )
	{

	}

	INT32 CircleBuffer::Init( UINT32 unDataSize )
	{
		if (unDataSize <= 0)
		{
			return FALSE;
		}

		Cleanup();   //5 防止多次调用Init.

		m_pDataHead   = new char[unDataSize];
		m_pDataStartPos    = m_pDataHead;
		m_unTotalSize = unDataSize;

		return TRUE;
	}

	INT32 CircleBuffer::Cleanup()
	{
		if (m_pDataHead != NULL && m_unTotalSize > 0)
		{
			SAFE_DELETE_ARRAY(m_pDataHead);
		}

		m_unTotalSize = 0;
		m_unDataLength = 0;

		return TRUE;
	}

	INT32 CircleBuffer::TryGetBuffer( char * pBuf , UINT32 unSize )
	{
		INT32   nResult = 0;
		char  * pData = m_pDataStartPos;
		UINT32  unPeeked = 0;

		while(unSize > 0)
		{
			if(m_unDataLength - unPeeked <= 0)
				break;

			ReCalcPtrPos(pData);

			UINT32 unMinSize = __min(__min(m_unDataLength - unPeeked , unSize),static_cast<UINT32>(m_pDataHead + m_unTotalSize - pData));
			if (unMinSize <= 0)
			{
				return 0;
			} 

			memcpy(pBuf , pData , unMinSize);
			pData += unMinSize;
			ReCalcPtrPos(pData);
			pBuf += unMinSize;
			unSize -= unMinSize;
			unPeeked += unMinSize;
			nResult += unMinSize;
		}

		return nResult;
	}

	INT32 CircleBuffer::GetBuffer( char * pBuf , UINT32 unSize )
	{
		INT32 nResult = 0;
		while(unSize > 0)
		{
			if(m_unDataLength <= 0)
				break;

			ReCalcPtrPos(m_pDataStartPos);

			UINT32 unMinSize = __min(__min(m_unDataLength , unSize),static_cast<UINT32>(m_pDataHead + m_unTotalSize - m_pDataStartPos));
			if (unMinSize <= 0)
			{
				return 0;
			} 

			memcpy(pBuf , m_pDataStartPos , unMinSize);
			m_pDataStartPos += unMinSize;
			ReCalcPtrPos(m_pDataStartPos);
			pBuf += unMinSize;
			unSize -= unMinSize;
			m_unDataLength -= unMinSize;
			nResult += unMinSize;
		}

		return nResult;
	}

	INT32 CircleBuffer::PushBuffer( const char * pBuf , UINT32 unSize )
	{
		INT32 nResult = 0;

		if (unSize > GetSpace() || GetSpace() <= 0)
		{
			MsgAssert_Re0(0 , "Buffer full."); 
		}

		while(unSize > 0)
		{
			if(GetSpace() <= 0)
				break;

			char * pWriter = m_pDataStartPos + m_unDataLength;
			ReCalcPtrPos(pWriter);

			UINT32 unMinSize = __min(__min(m_unTotalSize - m_unDataLength , unSize),static_cast<UINT32>(m_pDataHead + m_unTotalSize - pWriter));
			if (unMinSize < 0)
			{
				return 0;
			} 

			memcpy(pWriter , pBuf , unMinSize);
			ReCalcPtrPos(m_pDataStartPos);
			pBuf += unMinSize;
			unSize -= unMinSize;
			m_unDataLength += unMinSize;
			nResult += unMinSize;
		}

		return nResult;
	}

	INT32 CircleBuffer::SkipBytes( UINT32 unSize )
	{ 
		INT32 nResult = 0; 

		while(unSize > 0)
		{
			if(unSize > m_unDataLength)  //5 当数据不够跳过的长度时不允许跳过
				break;
			   
			UINT32 unMinSize = __min(__min(m_unDataLength , unSize),static_cast<UINT32>(m_pDataHead + m_unTotalSize - m_pDataStartPos));
			if (unMinSize <= 0)
			{
				return 0;
			} 
			 
			m_pDataStartPos += unMinSize;
			ReCalcPtrPos(m_pDataStartPos); 
			unSize -= unMinSize;
			m_unDataLength -= unMinSize;
			nResult += unMinSize;
		}
		return nResult;
	}

	void CircleBuffer::ReCalcPtrPos( char *& p )
	{
		if(p >= (m_pDataHead + m_unTotalSize))
		{
			p = m_pDataHead + (p - m_pDataHead) % m_unTotalSize;
		}
	}

}
