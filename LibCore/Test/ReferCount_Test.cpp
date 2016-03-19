// #include "CUtil/inc/ReferCount.h"
// #include "UnitTest++/UnitTestPP.h"
// 
// class TestChunk : public CUtil::HoderObject
// {
// public:
// 	boost::intrusive_ptr< CUtil::Holder<TestChunk> > _holder;
// 	TestChunk(char *& pD)
// 		: pData(pD)
// 	{
// 		CUtil::Holder<TestChunk> * p = (CUtil::Holder<TestChunk> *)pD;
// 		_holder = p;
// 	}
// 
// 	TestChunk(const TestChunk & test)
// 		: _holder(test._holder)
// 		, pData(test.pData)
// 	{
// 	}
// 
// 	TestChunk &operator=(const TestChunk &rRHS) 
// 	{
// 		if (this != &rRHS)
// 		{
// 			pData = rRHS.pData;
// 			_holder = rRHS._holder;
// 		}
// 		return *this;
// 	}
// 	virtual ~TestChunk()
// 	{
// 	}
// 	char * GetData()
// 	{
// 		return pData;
// 	}
// 
// public:
// 	virtual BOOL Release() override
// 	{
// 		delete pData;
// 		pData = NULL;
// 		delete _holder;
// 
// 		return TRUE;
// 	}
// 
// public:
// 	char *& pData;
// };
//
// TEST(ReferCount)
// {  
// 	char * pData = new char[1024];
// 	memset(pData , 0 , 1024);
// 	{
// 		TestChunk objChunk1(pData);
// 		{
// 			TestChunk objChunk2 = objChunk1;
// 			{
// 				TestChunk objChunk3(objChunk2);
// 				{
// 					TestChunk & objChunk4 = objChunk3;
// 					CHECK_EQUAL(objChunk4._holder->GetRefCount() , 3);
// 				}
// 				CHECK_EQUAL(objChunk3._holder->GetRefCount() , 3);
// 			}
// 			CHECK_EQUAL(objChunk2._holder->GetRefCount() , 2);
// 		}
// 		CHECK_EQUAL(objChunk1._holder->GetRefCount() , 1);
// 	}
// 	char * p = NULL;
// 	CHECK_EQUAL(pData , p);
// } 
 