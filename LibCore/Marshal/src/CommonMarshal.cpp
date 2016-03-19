#include "Marshal/inc/CommonMarshal.h"
#include "Marshal/inc/CStream.h"

namespace CUtil
{
	CStream & CharPtr::marshal( CStream & cs )const
	{    
		cs << m_unLen;  
		cs.Pushback(m_pBuf , m_unLen);

		return cs;
	}

	CStream & CharPtr::unMarshal(CStream & cs )
	{ 
		cs >> m_unLen;

		MsgAssert_Re(!(m_unLen > cs.GetDataLen() - cs.GetCurPos()) , cs , "unMarshal invalid length.");
		
		void * pBuf = NULL;
		cs.Pop(pBuf , m_unLen);
		m_pBuf = (char *)pBuf;
		return cs;
	}



}