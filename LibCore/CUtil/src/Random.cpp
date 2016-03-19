#include "CUtil/inc/Random.h"


namespace CUtil
{
	Random g_objRandomGenerator;

#ifdef WIN32

	Random::Random()
	{
		m_hProv = NULL;
		if(!CryptAcquireContext(&m_hProv, NULL, NULL, PROV_RSA_FULL, 0)) 
		{
			MsgAssert( !CryptAcquireContext(&m_hProv, NULL, NULL, PROV_RSA_FULL, CRYPT_NEWKEYSET) , "CryptAcquireContext Failed. \n" ) 
		}
	}

	Random::~Random()
	{
		if(m_hProv != NULL)
			CryptReleaseContext(m_hProv,0);

	}

	INT64 Random::Generator()
	{
		if( m_hProv != NULL)
		{
			INT64 result = 0;
			if(CryptGenRandom( m_hProv, sizeof(result), (BYTE*)&result)) 
			{
				return result;
			}
		}
		return ((INT64)rand() << 32) + rand();
	}
	 
#else

	Random::Random()
	{
		m_pFile = fopen("/dev/urandom","r");
		assert(m_pFile && "can't open urandom");
	}

	Random::~Random()
	{
		fclose(m_pFile);
	}

	INT64 Random::Generator()
	{
		INT64 result = 0;
		fread(&result,sizeof(result),1,m_pFile);
		return result;
	}
#endif
}