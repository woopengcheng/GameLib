#ifndef __cutil_csvreader_h__
#define __cutil_csvreader_h__ 
#include "CUtil/inc/Common.h" 

namespace CUtil
{
	class DLL_EXPORT CSVReader
	{
	public:
		typedef std::vector<std::string>	VecValuesT;
		typedef std::vector<VecValuesT>		VecRowsT;

	public:
		INT32			Load(const char * pContent);
		INT32			Load(const std::string& file);

	public:
		std::string		Get(size_t row, size_t col);
		std::string		GetString(size_t row, size_t col);
		void			GetString(size_t row, size_t col, const char * split, std::vector<std::string> & v);
		INT32			GetInt32(size_t row, size_t col);
		INT64			GetInt64(size_t row, size_t col);
		double			GetDouble(size_t row, size_t col);
		INT32			GetInt32ByIndex(size_t row, size_t col, const char** names, size_t count);
		bool			GetBool(size_t row, size_t col);
	public:
		std::string		GetColName(size_t col, size_t row = 0);

	public:
		size_t			GetIndex(const char* name);
		size_t			GetIndex(const char* name, size_t row);
		size_t			Count();
		size_t			FieldCount();

	private:
		VecRowsT		m_vecRows;
		VecValuesT		m_vecNames;
	};

}
#endif
