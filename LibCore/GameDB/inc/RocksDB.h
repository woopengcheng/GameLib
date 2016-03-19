#ifndef __game_db_rocksdb_h__ 
#define __game_db_rocksdb_h__
#include "CUtil/inc/Common.h"  
#include "Marshal/inc/CStream.h"
#include "Marshal/inc/Marshal.h"
#include "port/win/port_win.h"
#include "db/filename.h"
#include "rocksdb/db.h"
#include "rocksdb/env.h"
#include "rocksdb/iterator.h"
#include "rocksdb/comparator.h"
#include "rocksdb/write_batch.h"
#include "rocksdb/comparator.h"
#include "rocksdb/cache.h"
#include "rocksdb/options.h"

namespace GameDB
{
	typedef rocksdb::Slice					Slice;
	typedef rocksdb::Status					Status;
	typedef rocksdb::Options				Options;
	typedef rocksdb::WriteBatch				WriteBatch;
	typedef rocksdb::EnvWrapper				EnvWrapper;
	typedef rocksdb::port::Mutex			DBMutex;
	typedef rocksdb::WriteBatch::Handler	Handler;
	typedef rocksdb::DB						DB;
	typedef	rocksdb::Cache					Cache;
	typedef rocksdb::WritableFile			WritableFile;
	typedef rocksdb::SequentialFile			SequentialFile;
	typedef rocksdb::ReadOptions			ReadOptions;
	typedef rocksdb::WriteOptions			WriteOptions;
	typedef rocksdb::EnvOptions				EnvOptions;
	typedef rocksdb::Env					Env;
	typedef rocksdb::Iterator				Iterator;
	typedef rocksdb::FileType				FileType;
	typedef rocksdb::CompressionType		CompressionType;
	typedef rocksdb::FileType				FileType;

#define DBComparator						rocksdb::Comparator
#define kLogFile							rocksdb::kLogFile
#define kDBLockFile							rocksdb::kDBLockFile
#define kTableFile							rocksdb::kTableFile
#define kDescriptorFile						rocksdb::kDescriptorFile
#define kCurrentFile						rocksdb::kCurrentFile
#define kTempFile							rocksdb::kTempFile
#define kInfoLogFile						rocksdb::kInfoLogFile
#define	kNoCompression						rocksdb::kNoCompression
#define	kSnappyCompression					rocksdb::kSnappyCompression
#define	kLZ4Compression						rocksdb::kLZ4Compression

	extern const rocksdb::Comparator * BytewiseComparator();
	extern std::shared_ptr<Cache> NewLRUCache(size_t capacity);
	extern Status RepairDB(const std::string& dbname, const Options& options);
	extern Status DestroyDB(const std::string& name, const Options& options);
	extern bool ParseFileName(const std::string& filename,uint64_t* number,	FileType* type);

//	extern DLL_EXPORT CUtil::CStream & operator << (CUtil::CStream& cs, const Slice& c);

	typedef CUtil::Chunk  Chunk;

	typedef std::vector<Slice> CollectionSlicesT;
	typedef std::map<std::string, Slice> CollectionKeyValsT;
	typedef std::vector<std::string>	CollectionBuffersT;
}

#endif
