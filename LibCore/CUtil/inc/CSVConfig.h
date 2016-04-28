#ifndef __cutil_csvconfig_h__
#define __cutil_csvconfig_h__
#include "CUtil/inc/Common.h"

namespace CUtil
{
	class DLL_EXPORT CSVConfig
	{
	public:
		CSVConfig()
			: m_bLoaded(false)
		{}
		virtual ~CSVConfig() {}

	public:
		bool			IsLoaded() { return m_bLoaded; }
		virtual BOOL	OnLoad() { return FALSE; }

	protected:
		bool			m_bLoaded;
	};


}

#endif