#include "GameDB/inc/Comparator.h"

namespace GameDB
{


	INT32 Comparator::Compare(const Slice& a, const Slice& b) const
	{
		const DBComparator * pBaseComparator = BytewiseComparator();

		const char* abuf = a.data();
		const char* bbuf = b.data();

		// ZSCORE 解析出score并比较,相等则不继续向下
		if(abuf[0] == PREFIX_ZSET_SCORE && bbuf[0] == PREFIX_ZSET_SCORE && abuf[1] == bbuf[1])
		{
			const char* asp = strchr(abuf,SEPARATOR_SECOND);
			const char* bsp = strchr(bbuf,SEPARATOR_SECOND);

			assert(asp != NULL && bsp != NULL);

			size_t apos = asp - abuf;
			size_t bpos = bsp - bbuf;

			if(apos == bpos && memcmp(abuf,bbuf,apos) == 0)
			{				
				INT64 aval = *reinterpret_cast<const INT64*>(&asp[1]);
				INT64 bval = *reinterpret_cast<const INT64*>(&bsp[1]);
				const char* akey = &asp[1] + sizeof(INT64) + 1;
				const char* bkey = &bsp[1] + sizeof(INT64) + 1;

				INT32 result = 0;

				if(aval == bval)
				{
					Slice akeybuf(akey,abuf + a.size() - akey);
					Slice bkeybuf(bkey,bbuf + b.size() - bkey);
					result = pBaseComparator->Compare(akeybuf,bkeybuf);
				}
				else
				{
					result = aval > bval ? 1 : -1;
				}

				if(abuf[1] == SEPARATOR_FIRST_R)
					return result * -1;
				return result;
			}
		} 

		//5 调用系统默认的比较函数.
		return pBaseComparator->Compare(a , b);
	}

	void Comparator::FindShortestSeparator(std::string* start,const Slice& limit) const
	{
		const DBComparator * pBaseComparator = BytewiseComparator();

		pBaseComparator->FindShortestSeparator(start , limit );
	}

	void Comparator::FindShortSuccessor(std::string* key) const
	{ 
		const DBComparator * pBaseComparator = BytewiseComparator();

		pBaseComparator->FindShortSuccessor(key);
	}

}