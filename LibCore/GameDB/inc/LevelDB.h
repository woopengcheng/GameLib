#ifndef __game_db_leveldb_h__ 
#define __game_db_leveldb_h__
#include "CUtil/inc/Common.h"  
#include "Marshal/inc/CStream.h"
#include "Marshal/inc/Marshal.h"
#include "port/port.h"
#include "db/filename.h"
#include "leveldb/db.h"
#include "leveldb/env.h"
#include "leveldb/iterator.h"
#include "leveldb/comparator.h"
#include "leveldb/write_batch.h"
#include "leveldb/comparator.h"
#include "leveldb/cache.h"
#include "leveldb/options.h"

namespace GameDB
{
	typedef leveldb::Slice					Slice;
	typedef leveldb::Status					Status;
	typedef leveldb::Options				Options;
	typedef leveldb::WriteBatch				WriteBatch;
	typedef leveldb::EnvWrapper				EnvWrapper;
	typedef leveldb::port::Mutex			DBMutex;
	typedef leveldb::WriteBatch::Handler	Handler;
	typedef leveldb::DB						DB;
	typedef	leveldb::Cache					Cache;
	typedef leveldb::WritableFile			WritableFile;
	typedef leveldb::SequentialFile			SequentialFile;
	typedef leveldb::ReadOptions			ReadOptions;
	typedef leveldb::WriteOptions			WriteOptions;
	typedef leveldb::Env					Env;
	typedef leveldb::Iterator				Iterator;
	typedef leveldb::FileType				FileType;
	typedef leveldb::CompressionType		CompressionType;
	typedef leveldb::FileType				FileType;

#define DBComparator						leveldb::Comparator
#define kLogFile							leveldb::kLogFile
#define kDBLockFile							leveldb::kDBLockFile
#define kTableFile							leveldb::kTableFile
#define kDescriptorFile						leveldb::kDescriptorFile
#define kCurrentFile						leveldb::kCurrentFile
#define kTempFile							leveldb::kTempFile
#define kInfoLogFile						leveldb::kInfoLogFile
#define	kNoCompression						leveldb::kNoCompression
#define	kSnappyCompression					leveldb::kSnappyCompression

	extern const leveldb::Comparator * BytewiseComparator();
	extern Cache* NewLRUCache(size_t capacity);
	extern Status RepairDB(const std::string& dbname, const Options& options);
	extern Status DestroyDB(const std::string& name, const Options& options);
	extern bool ParseFileName(const std::string& filename,uint64_t* number,	FileType* type);

	extern DLL_EXPORT CUtil::CStream & operator << (CUtil::CStream& cs, const Slice& c);

	typedef CUtil::Chunk  Chunk;

	typedef std::vector<Slice> CollectionSlicesT;
	typedef std::map<std::string, Slice> CollectionKeyValsT;
	typedef std::vector<std::string>	CollectionBuffersT;
}

#endif



//namespace GameDB
//{ 
//
//	class DLL_EXPORT Slice
//	{
//	public:
//		// Create an empty slice.
//		Slice(){ }
//
//		// Create a slice that refers to d[0,n-1].
//		Slice(const char* d, size_t n) 
//			: m_slice(d , n) { }
//
//		// Create a slice that refers to the contents of "s"
//		Slice(const std::string& s) : m_slice(s){ }
//
//		// Create a slice that refers to s[0,strlen(s)-1]
//		Slice(const char* s) : m_slice(s) { }
//
//		// Return a pointer to the beginning of the referenced data
//		const char* data() const { return m_slice.data(); }
//
//		// Return the length (in bytes) of the referenced data
//		size_t size() const { return m_slice.size(); }
//
//		// Return true iff the length of the referenced data is zero
//		bool empty() const { return m_slice.size() == 0; }
//
//		// Return the ith byte in the referenced data.
//		// REQUIRES: n < size()
//		char operator[](size_t n) const { return m_slice[n];	}
//
//		// Change this slice to refer to an empty array
//		void clear() { m_slice.clear(); }
//
//		// Drop the first "n" bytes from this slice.
//		void remove_prefix(size_t n) { m_slice.remove_prefix(n); }
//
//		// Return a string that contains the copy of the referenced data.
//		std::string ToString() const { return m_slice.ToString(); }
//
//		// Three-way comparison.  Returns value:
//		//   <  0 iff "*this" <  "b",
//		//   == 0 iff "*this" == "b",
//		//   >  0 iff "*this" >  "b"
//		int compare(const Slice& b) const { return m_slice.compare(b.m_slice); }
//
//		// Return true iff "x" is a prefix of "*this"
//		bool starts_with(const Slice& x) const { return m_slice.starts_with(x.m_slice);	}		 
//
//	private:
//		Slice m_slice;
//	};	
//// 	extern DLL_EXPORT CUtil::CStream & operator << (CUtil::CStream& cs, const Slice& c);
//
//
//// DB contents are stored in a set of blocks, each of which holds a
//// sequence of key,value pairs.  Each block may be compressed before
//// being stored in a file.  The following enum describes which
//// compression method (if any) is used to compress a block.
//	enum CompressionType {
//		// NOTE: do not change the values of existing entries, as these are
//		// part of the persistent format on disk.
//		kNoCompression = 0x0,
//		kSnappyCompression = 0x1
//	};
//
//	// Options to control the behavior of a database (passed to DB::Open)
//	struct DLL_EXPORT Options {
//		// -------------------
//		// Parameters that affect behavior
//
//		// Comparator used to define the order of keys in the table.
//		// Default: a comparator that uses lexicographic byte-wise ordering
//		//
//		// REQUIRES: The client must ensure that the comparator supplied
//		// here has the same name and orders keys *exactly* the same as the
//		// comparator provided to previous open calls on the same DB.
//		const Comparator* comparator;
//
//		// If true, the database will be created if it is missing.
//		// Default: false
//		bool create_if_missing;
//
//		// If true, an error is raised if the database already exists.
//		// Default: false
//		bool error_if_exists;
//
//		// If true, the implementation will do aggressive checking of the
//		// data it is processing and will stop early if it detects any
//		// errors.  This may have unforeseen ramifications: for example, a
//		// corruption of one DB entry may cause a large number of entries to
//		// become unreadable or for the entire DB to become unopenable.
//		// Default: false
//		bool paranoid_checks;
//
//		// Use the specified object to interact with the environment,
//		// e.g. to read/write files, schedule background work, etc.
//		// Default: Env::Default()
//		Env* env;
//
//		// Any internal progress/error information generated by the db will
//		// be written to info_log if it is non-NULL, or to a file stored
//		// in the same directory as the DB contents if info_log is NULL.
//		// Default: NULL
//		Logger* info_log;
//
//		// -------------------
//		// Parameters that affect performance
//
//		// Amount of data to build up in memory (backed by an unsorted log
//		// on disk) before converting to a sorted on-disk file.
//		//
//		// Larger values increase performance, especially during bulk loads.
//		// Up to two write buffers may be held in memory at the same time,
//		// so you may wish to adjust this parameter to control memory usage.
//		// Also, a larger write buffer will result in a longer recovery time
//		// the next time the database is opened.
//		//
//		// Default: 4MB
//		size_t write_buffer_size;
//
//		// Number of open files that can be used by the DB.  You may need to
//		// increase this if your database has a large working set (budget
//		// one open file per 2MB of working set).
//		//
//		// Default: 1000
//		int max_open_files;
//
//		// Control over blocks (user data is stored in a set of blocks, and
//		// a block is the unit of reading from disk).
//
//		// If non-NULL, use the specified cache for blocks.
//		// If NULL, leveldb will automatically create and use an 8MB internal cache.
//		// Default: NULL
//		Cache* block_cache;
//
//		// Approximate size of user data packed per block.  Note that the
//		// block size specified here corresponds to uncompressed data.  The
//		// actual size of the unit read from disk may be smaller if
//		// compression is enabled.  This parameter can be changed dynamically.
//		//
//		// Default: 4K
//		size_t block_size;
//
//		// Number of keys between restart points for delta encoding of keys.
//		// This parameter can be changed dynamically.  Most clients should
//		// leave this parameter alone.
//		//
//		// Default: 16
//		int block_restart_interval;
//
//		// Compress blocks using the specified compression algorithm.  This
//		// parameter can be changed dynamically.
//		//
//		// Default: kSnappyCompression, which gives lightweight but fast
//		// compression.
//		//
//		// Typical speeds of kSnappyCompression on an Intel(R) Core(TM)2 2.4GHz:
//		//    ~200-500MB/s compression
//		//    ~400-800MB/s decompression
//		// Note that these speeds are significantly faster than most
//		// persistent storage speeds, and therefore it is typically never
//		// worth switching to kNoCompression.  Even if the input data is
//		// incompressible, the kSnappyCompression implementation will
//		// efficiently detect that and will switch to uncompressed mode.
//		CompressionType compression;
//
//		// Create an Options object with default values for all fields.
//		Options();
//	};
//
//	// Options that control read operations
//	struct DLL_EXPORT ReadOptions {
//		// If true, all data read from underlying storage will be
//		// verified against corresponding checksums.
//		// Default: false
//		bool verify_checksums;
//
//		// Should the data read for this iteration be cached in memory?
//		// Callers may wish to set this field to false for bulk scans.
//		// Default: true
//		bool fill_cache;
//
//		// If "snapshot" is non-NULL, read as of the supplied snapshot
//		// (which must belong to the DB that is being read and which must
//		// not have been released).  If "snapshot" is NULL, use an impliicit
//		// snapshot of the state at the beginning of this read operation.
//		// Default: NULL
//		const Snapshot* snapshot;
//
//		ReadOptions()
//			: verify_checksums(false),
//			fill_cache(true),
//			snapshot(NULL) {
//		}
//	};
//
//	// Options that control write operations
//	struct DLL_EXPORT WriteOptions {
//		// If true, the write will be flushed from the operating system
//		// buffer cache (by calling WritableFile::Sync()) before the write
//		// is considered complete.  If this flag is true, writes will be
//		// slower.
//		//
//		// If this flag is false, and the machine crashes, some recent
//		// writes may be lost.  Note that if it is just the process that
//		// crashes (i.e., the machine does not reboot), no writes will be
//		// lost even if sync==false.
//		//
//		// In other words, a DB write with sync==false has similar
//		// crash semantics as the "write()" system call.  A DB write
//		// with sync==true has similar crash semantics to a "write()"
//		// system call followed by "fsync()".
//		//
//		// Default: false
//		bool sync;
//
//		WriteOptions()
//			: sync(false) {
//		}
//	};
//
//
//	class DLL_EXPORT WriteBatch
//	{
//	public:
//		WriteBatch();
//		~WriteBatch();
//
//		// Store the mapping "key->value" in the database.
//		void Put(const Slice& key, const Slice& value) { return m_writeBatch.put(key, value); }
//
//		// If the database contains a mapping for "key", erase it.  Else do nothing.
//		void Delete(const Slice& key) { return m_writeBatch.Delete(key); }
//
//		// Clear all updates buffered in this batch.
//		void Clear() { return m_writeBatch.Clear(); }
//
//		// Support for iterating over the contents of a batch.
//		class DLL_EXPORT Handler {
//		public:
//			virtual ~Handler();
//			virtual void Put(const Slice& key, const Slice& value) = 0;
//			virtual void Delete(const Slice& key) = 0;
//	private:
//		WriteBatch  m_writeBatch;
//	};
//
//	class DLL_EXPORT Status
//	{
//	public:
//		Status(){}
//		Status(const Status& s)
//			: m_status(s.m_status)
//		{}
//		void operator=(const Status& s)
//		{
//			m_status = s.m_status;
//		}
//		static Status OK() 
//		{ 
//			return Status(); 
//		}
//
//		// Return error status of an appropriate type.
//		static Status NotFound(const Slice& msg, const Slice& msg2 = Slice()) 
//		{
//			return Status(kNotFound, msg, msg2);
//		}
//		static Status Corruption(const Slice& msg, const Slice& msg2 = Slice()) 
//		{
//			return Status(kCorruption, msg, msg2);
//		}
//		static Status NotSupported(const Slice& msg, const Slice& msg2 = Slice()) 
//		{
//			return Status(kNotSupported, msg, msg2);
//		}
//		static Status InvalidArgument(const Slice& msg, const Slice& msg2 = Slice()) 
//		{
//			return Status(kInvalidArgument, msg, msg2);
//		}
//		static Status IOError(const Slice& msg, const Slice& msg2 = Slice()) 
//		{
//			return Status(kIOError, msg, msg2);
//		}
//
//		bool ok() const 
//		{ 
//			return m_status.ok();
//		}
//
//		bool IsNotFound() const
//		{
//			return m_status.IsNotFound();
//		}
//		bool IsIOError() const
//		{
//			return m_status.IsIOError();
//		}
//		bool IsCorruption() const 
//		{ 
//			return m_status.IsCorruption();
//		}
//
//		std::string ToString() const
//		{
//			return m_status.ToString();
//		}
//
//	private:
//		Status m_status;
//	};
//
////	typedef Iterator		 Iterator;
//
//	
//
//	class Database;
//	class CustomDefinedCacheHandler : public WriteBatch::Handler
//	{
//	public:
//		CustomDefinedCacheHandler(Database * pDatabase)
//			: m_pDB(pDatabase)
//		{
//
//		}
//
//	public:
//		virtual void Put(const Slice& objKey, const Slice& objValue)
//		{
//
//		}
//		virtual void Delete(const Slice& objKey)
//		{
//
//		}
//
//	private:
//		Database * m_pDB;
//	};
//	 
//	static void fnCustomCacheDeleter(const Slice & objKey,void* pValue) 
//	{
//		std::string * p = (std::string*)pValue;
//		delete p;
//	}
//}
//
//
//
//#endif