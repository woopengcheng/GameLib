#ifndef __game_db_db_port_h__
#define __game_db_db_port_h__

#define USE_ROCKSDB
//#define USE_LEVELDB
#ifdef USE_ROCKSDB
#include "GameDB/inc/RocksDB.h"
#elif defined USE_LEVELDB
#include "GameDB/inc/LevelDB.h"
#endif


#endif // __game_db_db_port_h__
