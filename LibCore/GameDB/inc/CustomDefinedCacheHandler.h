#ifndef __database_custom_defined_cache_handler_h__
#define __database_custom_defined_cache_handler_h__ 
#include "GameDB/inc/DBCommon.h"
 
namespace GameDB
{
	class Database;

	class CustomDefinedCacheHandler : public Handler
	{
	public:
		CustomDefinedCacheHandler(Database * pDatabase)
			: m_pDB(pDatabase)
		{

		}

	public:
		virtual void	Put(const Slice& objKey, const Slice& objValue);
		virtual void	Delete(const Slice& objKey);

	private:
		Database *		m_pDB;
	};
}

#endif