#include "CUtil\inc\CUtil.h" 
#include "CUtil\inc\CSVReader.h"
#include <fstream>

namespace CUtil
{
	void trim_right(char* text)
	{
		INT32 n = (INT32)strlen(text);
		while (n >= 0)
		{
			if (text[n] == '\r' || text[n] == '\n' || text[n] == ' ')
			{
				text[n] = '\0';
				n--;
			}
			else
			{
				break;
			}
		}
	}

	INT32 CSVReader::Load(const std::string& file)
	{
		std::fstream stream;
		stream.open(file.c_str(), std::ios_base::in);
		if (!stream.good())
			return -1;

		m_vecRows.clear();
		m_vecNames.clear();

		while (!stream.eof())
		{
			char buf[10240] = "";
			stream.getline(buf, sizeof(buf));
			if (buf[0] == '\0')
				continue;

			trim_right(buf);

			VecValuesT values;

			CUtil::tokenize(buf, values, "\t", "", "\"");

			if (values.size() == 0)
				continue;

			if (m_vecNames.size() == 0)
				m_vecNames = values;
			else
				m_vecRows.push_back(values);
		}
		return 0;
	}

	INT32 CSVReader::Load(const char * pContent)
	{
		if (!pContent)
			return -1;

		m_vecRows.clear();
		m_vecNames.clear();
		
		std::stringstream stream;
		stream << pContent;
		while (!stream.eof())
		{
			char buf[10240] = "";
			stream.getline(buf, sizeof(buf));
			if (buf[0] == '\0')
				continue;

			trim_right(buf);

			VecValuesT values;

			CUtil::tokenize(buf, values, "\t", "", "\"");

			if (values.size() == 0)
				continue;

			if (m_vecNames.size() == 0)
				m_vecNames = values;
			else
				m_vecRows.push_back(values);
		}
		return 0;
	}

	std::string CSVReader::Get(size_t row, size_t col)
	{
		MsgAssert_Re(row >= 0 && row < m_vecRows.size(), std::string() , "error row.row=" << row );

		VecValuesT& values = m_vecRows[row];

		MsgAssert_Re(col >= 0 && col < values.size(), std::string(), "error col:col=" << col);

		return values[col].c_str();
	}

	std::string  CSVReader::GetString(size_t row, size_t col)
	{
		return Get(row, col);
	}

	void CSVReader::GetString(size_t row, size_t col, const char * split, std::vector<std::string> & v)
	{
		MsgAssert(row >= 0 && row < m_vecRows.size(), "error row.row=" << row);

		VecValuesT& values = m_vecRows[row];

		MsgAssert(col >= 0 && col < values.size(), "error col:col=" << col);

		CUtil::tokenize(values[col].c_str(), v, split);
	}

	INT32 CSVReader::GetInt32(size_t row, size_t col)
	{
		std::string value = this->Get(row, col);

		return (INT32)CUtil::atoi(value);
	}

	INT64 CSVReader::GetInt64(size_t row, size_t col)
	{
		std::string value = this->Get(row, col);

		return CUtil::atoi(value);
	}

	double CSVReader::GetDouble(size_t row, size_t col)
	{
		std::string value = this->Get(row, col);

		return CUtil::atof(value);
	}

	INT32 CSVReader::GetInt32ByIndex(size_t row, size_t col, const char** names, size_t count)
	{
		MsgAssert_Re(names && count > 0 , -1 , "get_i32_byindex error.row=" << row << ":col=" << col);

		std::string name = this->Get(row, col);

		for (size_t i = 0; i < count; ++i)
		{
			if (stricmp(name.c_str() , names[i]) == 0)
				return (INT32)i;
		}
		MsgAssert_Re(false && "wrong configure", -1, "get_i32_byindex error.row=" << row << ":col=" << col);
		return -1;
	}

	bool CSVReader::GetBool(size_t row, size_t col)
	{
		std::string name = this->Get(row, col);

		return CUtil::strtobool(name.c_str()) >= 1;
	}

	size_t CSVReader::GetIndex(const char* name)
	{
		for (size_t i = 0; i < m_vecNames.size(); ++i)
		{
			const char* c = m_vecNames[i].c_str();
			if (CUtil::stricmp(c, name) == 0)
				return i;
		}
		return std::string::npos;
	}

	size_t CSVReader::GetIndex(const char* name, size_t row)
	{
		if (row == 0)
			return GetIndex(name);

		VecValuesT& values = m_vecRows[row - 1];

		for (size_t i = 0; i < values.size(); ++i)
		{
			const char* c = values[i].c_str();
			if (stricmp(c, name) == 0)
				return i;
		}
		return std::string::npos;
	}

	size_t CSVReader::Count()
	{
		return m_vecRows.size();
	}

	size_t CSVReader::FieldCount()
	{
		return m_vecNames.size();
	}

	std::string CSVReader::GetColName(size_t col, size_t row)
	{
		MsgAssert_Re(col >= 0 && col < m_vecNames.size() , std::string() , "GetColName error,row=" << row << "col" << col);
		
		if (row == 0)
		{
			return m_vecNames[col].c_str();
		}
		return m_vecRows[row - 1][col];
	}

}
