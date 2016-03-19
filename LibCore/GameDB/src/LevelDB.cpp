#include "GameDB/inc/db_port.h"

#ifdef USE_LEVELDB
#include "GameDB/inc/LevelDB.h"

namespace GameDB
{
	const DBComparator* BytewiseComparator()
	{
		return leveldb::BytewiseComparator();
	}

	Cache* NewLRUCache(size_t capacity)
	{
		return leveldb::NewLRUCache(capacity);
	}

	Status RepairDB(const std::string& dbname, const Options& options)
	{
		return leveldb::RepairDB(dbname, options);
	}

	Status DestroyDB(const std::string& name, const Options& options)
	{
		return leveldb::DestroyDB(name, options);
	}

	bool ParseFileName(const std::string& filename, uint64_t* number, FileType* type)
	{
		return leveldb::ParseFileName(filename, number, type);
	}

}
#endif // USE_LEVELDB

