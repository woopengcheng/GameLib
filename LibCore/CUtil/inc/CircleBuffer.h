#ifndef __circle_buffer_h__
#define __circle_buffer_h__
#include "CUtil/inc/Common.h"

namespace CUtil
{  
	class DLL_EXPORT CircleBuffer
	{
	public:
		CircleBuffer( void );
		virtual ~CircleBuffer( void ); 

	public:
		virtual   INT32    Init(UINT32 unDataSize);
		virtual   INT32    Cleanup();

	public:
		INT32   GetBuffer(char * pBuf , UINT32 unSize);
		INT32   PushBuffer(const char * pBuf , UINT32 unSize);
		INT32   SkipBytes(UINT32 unSize);
		INT32   TryGetBuffer(char * pBuf , UINT32 unSize);
		BOOL    IsVaild(){ return m_pDataHead != NULL; }
		UINT32  GetSpace(){ return m_unTotalSize - m_unDataLength; }
		UINT32  GetDataLength(){ return m_unDataLength; }

	private:
		void    ReCalcPtrPos(char *& p);

	private:
		char  * m_pDataHead;
		char  * m_pDataStartPos;
		UINT32  m_unTotalSize;
		UINT32  m_unDataLength;
	};
}

#endif 
