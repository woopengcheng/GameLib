#ifndef __gamedb_comparator_h__ 
#define __gamedb_comparator_h__
#include "GameDB/inc/DBCommon.h"

namespace GameDB
{
	//5 用于levelDB排序用的.
	class DLL_EXPORT Comparator :public DBComparator
	{
	public:
		Comparator(){}
		virtual ~Comparator(){}
		
	public:
		// Three-way comparison.  Returns value:
		//   < 0 iff "a" < "b",
		//   == 0 iff "a" == "b",
		//   > 0 iff "a" > "b"
		virtual INT32		Compare(const Slice& a, const Slice& b) const override; 
		virtual void		FindShortestSeparator(std::string* start,const Slice& limit) const override;
		virtual void		FindShortSuccessor(std::string* key) const override;
		virtual const char* Name() const override { return "GameDBComparator"; }
	};
}

#endif