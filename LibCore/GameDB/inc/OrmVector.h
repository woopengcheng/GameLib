#ifndef __gamedb_orm_vector_h__
#define __gamedb_orm_vector_h__
#include "GameDB/inc/DBCommon.h"
#include "CUtil/inc/CUtil.h" 
#include "bson/bson.h"

namespace GameDB
{
	//5 ORM栈上的数据
	template<typename T>
	class OrmVector : public std::vector<T>
	{
	public:
		typedef std::vector<T> CollectionOrmsT;

	public:
		void Cleanup()
		{
			CollectionOrmsT::clear();
		}

	public:
		void Load(const char * pData , UINT32 unSize)
		{
			Cleanup();
			Load(mongo::BSONObj(pData , unSize));
		}

		void Load(mongo::BSONObj & objBson)
		{
			mongo::BSONObjIterator iter(objBson);
			while(iter.more())
			{
				T t;
				mongo::BSONElement be = iter.next();
				t.FromBson(be.Obj());
				push_back(t);
			}
		}

		void Save(mongo::BSONObj & obj)
		{
			mongo::BSONObjBuilder bb;
			for (size_t i = 0;i < CollectionOrmsT::size();++i)
			{
				T & t = CollectionOrmsT::at(i);
				mongo::BSONObj tmp;
				t.ToBson(tmp);
				bb.append(tmp);
			}
			obj = bb.obj();
		}

	public:
		INT64		HashMake(INT64 llSeed = DEFAULT_HASH_SEED)
		{
			size_t size = CollectionOrmsT::size();
			INT64  hash = CUtil::CityHash(&size , sizeof(size) , llSeed);
			for (size_t i = 0;I < size;++i)
			{
				hash = CUtil::CityHash(&i , sizeof(size_t) , hash);
				hash = CUtil::HashMake<T>(CollectionOrmsT::at(i) , hash);
			}

			return hash;
		}
	};

	//5 ORM堆上的数据,这里的T一定是Orm对象
	template<typename T>
	class OrmVectorEx : public std::vector<T>
	{
	public:
		typedef std::vector<T> CollectionOrmsT;

		template<typename Object>
		struct ObjectType
		{
			typedef Object Type;
		};

		template<typename Object>
		struct ObjectType<Object*>
		{
			typedef Object Type;
		};

	public:
		~OrmVectorEx()
		{
			Cleanup();
		}

	public:
		void Cleanup(BOOL bFree = TRUE)
		{
			if (bFree)
			{
				for (typename CollectionOrmsT::iterator iter = CollectionOrmsT::begin();iter != CollectionOrmsT::end();++iter)
				{
					delete(*iter);
				}
			}
			CollectionOrmsT::clear();
		}

		BOOL Remove(T value , BOOL bFree)
		{
			CollectionOrmsT::iterator iter = std::remove(CollectionOrmsT::begin() , CollectionOrmsT::end() , value);
			if (iter == CollectionOrmsT::end())
			{
				return false;
			}

			if (bFree)
			{
				delete value;
			}

			CollectionOrmsT::erase(iter , CollectionOrmsT::end());

			return TRUE;
		}
	public:
		void Load(const char * pData , UINT32 unSize)
		{
			Cleanup();
			Load(mongo::BSONObj(pData , unSize));
		}

		void Load(mongo::BSONObj & objBson)
		{
			mongo::BSONObjIterator iter(objBson);
			while(iter.more())
			{
				T t = new Object<T>::Type();
				mongo::BSONElement be = iter.next();
				t.FromBson(be.Obj());
				push_back(t);
			}
		}

		void Save(mongo::BSONObj & obj)
		{
			mongo::BSONObjBuilder bb;
			for (size_t i = 0;i < CollectionOrmsT::size();++i)
			{
				T & t = CollectionOrmsT::at(i);
				mongo::BSONObj tmp;
				t.ToBson(tmp);
				bb.append(tmp);
			}
			obj = bb.obj();
		}

	public:
		INT64		HashMake(INT64 llSeed = DEFAULT_HASH_SEED)
		{
			size_t size = CollectionOrmsT::size();
			INT64  hash = CUtil::CityHash(&size , sizeof(size) , llSeed);
			for (size_t i = 0;I < size;++i)
			{
				hash = CUtil::CityHash(&i , sizeof(size_t) , hash);
				if (T t = CollectionOrmsT::at(i))
				{
					hash = t.HashMake(hash);
				}
			}

			return hash;
		}
	};


}

#endif