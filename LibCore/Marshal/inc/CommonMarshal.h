#ifndef __cutil_common_marshal_h__
#define __cutil_common_marshal_h__
#include "Marshal/inc/Marshal.h"


namespace CUtil
{
	class CStream;

	class DLL_EXPORT CharPtr : public CUtil::Marshal
	{ 
	public:
		CharPtr( void ): m_pBuf(NULL) , m_unLen(0) {}
		CharPtr(char * pBuf , UINT32 unLen) : m_pBuf(pBuf) , m_unLen(unLen) {}

	public:
		virtual CStream & marshal(CStream & cs) const override;
		virtual CStream & unMarshal(CStream & cs) override;

	protected:
		char * m_pBuf;
		UINT32 m_unLen;
	};

	template<typename Container>
	class  STLContainer : public CUtil::Marshal
	{ 
	public: 
		STLContainer( Container & pContainer ): m_pContainer(&pContainer){}  //5 默认隐式转换.不加explicit
		STLContainer(STLContainer<Container> & stl)
		{
			std::copy(stl.GetContainer()->begin(), stl.GetContainer()->end(), std::back_inserter(*m_pContainer));  
		}
	public:
		virtual CStream & marshal( CStream & cs )const override
		{
			cs << m_pContainer->size();

			Container::iterator iter = m_pContainer->begin();
			for (;iter != m_pContainer->end(); ++iter)
			{
				cs << *iter;
			}

			return cs;
		}
		virtual CStream & unMarshal (CStream & cs) override
		{
			m_pContainer->clear();
			size_t unCount = 0;
			cs >> unCount;
			for (;unCount > 0; --unCount)
			{
				typename Container::value_type val;
				cs >> val;

				m_pContainer->insert(m_pContainer->end() , val);
			}

			return cs;
		}

		Container * GetContainer(){ return  m_pContainer; }

	protected:
		Container * m_pContainer;
	};

	template<typename Container>
	class  STLContainer<typename Container &> : public CUtil::Marshal
	{ 
	public: 
		STLContainer( Container & pContainer ): m_pContainer(&pContainer){}  //5 默认隐式转换.不加explicit
		STLContainer(STLContainer<Container> & stl)
		{
			std::copy(stl.GetContainer()->begin(), stl.GetContainer()->end(), std::back_inserter(*m_pContainer));  
		}
	public:
		virtual CStream & marshal( CStream & cs ) const override
		{
			cs << m_pContainer->size();

			Container::iterator iter = m_pContainer->begin();
			for (;iter != m_pContainer->end(); ++iter)
			{
				cs << *iter;
			}

			return cs;
		}
		virtual CStream & unMarshal (CStream & cs) override
		{
			m_pContainer->clear();
			UINT32 unCount = 0;
			cs >> unCount;
			for (;unCount > 0; --unCount)
			{
				typename Container::value_type val;
				cs >> val;

				m_pContainer->insert(m_pContainer->end() , val);
			}

			return cs;
		}

		Container * GetContainer(){ return  m_pContainer; }

		std::ostream&	operator<<(const Container & c)
		{
			os << '[';
			typename _CType::const_iterator i = c.begin(), e = c.end();
			if ( i != e ) { os << *i; for ( ++i; i != e; ++i) os << ',' << *i; }
			os << ']';
			return os;
		}

	protected:
		Container * m_pContainer;
	};
	 
	
	template <class _T1, class _T2>
	inline std::ostream & operator<<(std::ostream & os, const std::pair<_T1, _T2>& __x)
	{
		return os << __x.first << '=' << __x.second;
	}

	template < typename _CType >
	std::ostream & TraceContainer(std::ostream & os, const _CType & c)
	{
		os << '[';
		typename _CType::const_iterator i = c.begin(), e = c.end();
		if ( i != e ) { os << *i; for ( ++i; i != e; ++i) os << ',' << *i; }
		os << ']';
		return os;
	}

	template < typename T >
	std::ostream & operator << (std::ostream & os, const std::vector<T> &x)
	{
		return TraceContainer(os, x);
	}

	template < typename T >
	std::ostream & operator << (std::ostream & os, const std::list<T> &x)
	{
		return TraceContainer(os, x);
	}

	template < typename T >
	std::ostream & operator << (std::ostream & os, const std::deque<T> &x)
	{
		return TraceContainer(os, x);
	}

	template < typename T1, typename T2>
	std::ostream & operator << (std::ostream & os, const std::map<T1, T2> &x)
	{
		return TraceContainer(os, x);
	}

}

#endif