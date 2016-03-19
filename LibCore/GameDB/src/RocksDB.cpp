#ifdef USE_ROCKSDB

#include "GameDB/inc/RocksDB.h"

namespace GameDB
{
	const DBComparator* BytewiseComparator()
	{
		return rocksdb::BytewiseComparator();
	}

	std::shared_ptr<Cache> NewLRUCache(size_t capacity)
	{
		return rocksdb::NewLRUCache(capacity);
	}

	Status RepairDB(const std::string& dbname, const Options& options)
	{
		return rocksdb::RepairDB(dbname, options);
	}

	Status DestroyDB(const std::string& name, const Options& options)
	{
		return rocksdb::DestroyDB(name, options);
	}

	bool ParseFileName(const std::string& filename, uint64_t* number, FileType* type)
	{
		return rocksdb::ParseFileName(filename, number, type);
	}

}
#endif // !USE_ROCKSDB
