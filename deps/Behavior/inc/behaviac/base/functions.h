#ifndef BEHAVIAC_BASE_FUNCTIONS_H_
#define BEHAVIAC_BASE_FUNCTIONS_H_

namespace behaviac
{
    template<typename VariableType>
    inline uint64_t ConvertToInteger(const VariableType& v)
    {
        BEHAVIAC_UNUSED_VAR(v);
        return 0;
    }

    template<>
    inline uint64_t ConvertToInteger(const long& v)
    {
        return (uint64_t)v;
    }

    template<>
    inline uint64_t ConvertToInteger(const int& v)
    {
        return (uint64_t)v;
    }

    template<>
    inline uint64_t ConvertToInteger(const short& v)
    {
        return (uint64_t)v;
    }

    //template<>
    //inline uint64_t ConvertToInteger(const __int8& v)
    //{
    //	return (uint64_t)v;
    //}

    template<>
    inline uint64_t ConvertToInteger(const char& v)
    {
        return (uint64_t)v;
    }

    template<>
    inline uint64_t ConvertToInteger(const unsigned long& v)
    {
        return (uint64_t)v;
    }

    template<>
    inline uint64_t ConvertToInteger(const unsigned int& v)
    {
        return (uint64_t)v;
    }

    template<>
    inline uint64_t ConvertToInteger(const unsigned short& v)
    {
        return (uint64_t)v;
    }

    //template<>
    //inline uint64_t ConvertToInteger(const unsigned __int8& v)
    //{
    //	return (uint64_t)v;
    //}

    template<>
    inline uint64_t ConvertToInteger(const unsigned char& v)
    {
        return (uint64_t)v;
    }
    ////////////////////////////////////////////////////////////////////////
    ////////////////////////////////////////////////////////////////////////
    template<typename VariableType>
    inline void ConvertFromInteger(int v, VariableType& ret)
    {
        BEHAVIAC_UNUSED_VAR(v);
        ret = VariableType();
    }

    template<>
    inline void ConvertFromInteger(int v, long& ret)
    {
        ret = (long)v;
    }

    template<>
    inline void ConvertFromInteger(int v, int& ret)
    {
        ret = (int)v;
    }

    template<>
    inline void ConvertFromInteger(int v, short& ret)
    {
        ret = (short)v;
    }

    //template<>
    //inline void ConvertFromInteger(int v, __int8& ret)
    //{
    //	ret = (__int8)v;
    //}

    template<>
    inline void ConvertFromInteger(int v, char& ret)
    {
        ret = (char)v;
    }

    template<>
    inline void ConvertFromInteger(int v, unsigned long& ret)
    {
        ret = (unsigned long)v;
    }

    template<>
    inline void ConvertFromInteger(int v, unsigned int& ret)
    {
        ret = (unsigned int)v;
    }

    template<>
    inline void ConvertFromInteger(int v, unsigned short& ret)
    {
        ret = (unsigned short)v;
    }

    //template<>
    //inline unsigned __int8 ConvertFromInteger(int v)
    //{
    //	ret = (unsigned __int8)v;
    //}

    template<>
    inline void ConvertFromInteger(int v, unsigned char& ret)
    {
        ret = (unsigned char)v;
    }
    ////////////////////////////////////////////////////////////////////////
	template<typename VariableType>
	inline double ConvertToDouble(const VariableType& v)
	{
		BEHAVIAC_UNUSED_VAR(v);
		return 0.0;
	}

	template<>
	inline double ConvertToDouble(const float& v)
	{
		return (double)v;
	}

	template<>
	inline double ConvertToDouble(const double& v)
	{
		return (double)v;
	}
}

#endif//BEHAVIAC_BASE_FUNCTIONS_H_
