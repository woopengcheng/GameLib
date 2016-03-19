#include "CUtil/inc/Chunk.h"
//#include "UnitTest++/UnitTestPP.h"
#include "gtest/gtest.h"

TEST(Chunk , chunk)
{
	{
		CUtil::Chunk objChunk;
		EXPECT_EQ(objChunk.GetChunkData()->GetDataLen() , 0);
		{
			CUtil::Chunk objChunk2 = objChunk;
			EXPECT_EQ(objChunk.GetChunkData()->GetRefCount() , -1);
		}
		EXPECT_EQ(objChunk.GetChunkData()->GetRefCount() , -1);
	}
	{
		CUtil::Chunk objChunk("test" , sizeof("test"));
		CUtil::Chunk objChunk2("test2" , sizeof("test2"));
		CUtil::Chunk objChunk3("test" , sizeof("test"));
		CUtil::Chunk objChunk4;
		CUtil::Chunk objChunk5;
		EXPECT_EQ(objChunk != objChunk2 , true);
		EXPECT_EQ(objChunk , objChunk3);
		EXPECT_EQ(objChunk4 , objChunk5);
	}
	{
		CUtil::Chunk objChunk("test" , sizeof("test"));
		CUtil::Chunk objChunk2;
		CUtil::Chunk objChunk3;
		objChunk2.Insert(objChunk2.Begin() , "test" , sizeof("test"));
		objChunk3.Pushback("test" , sizeof("test"));
		EXPECT_EQ(objChunk , objChunk2);
		EXPECT_EQ(objChunk2.GetDataLen() , sizeof("test"));
		EXPECT_EQ(objChunk , objChunk3);
		EXPECT_EQ(objChunk3.GetDataLen() , sizeof("test"));
	}
	{
		CUtil::Chunk objChunk2;
		CUtil::Chunk objChunk3;
		objChunk2.Reverse(11);
		EXPECT_EQ(objChunk2.GetSize() , 16); 
		objChunk2.Reverse(17);
		EXPECT_EQ(objChunk2.GetSize() , 32);

		EXPECT_EQ(objChunk2 , objChunk3);
	}
	{
		CUtil::Chunk objChunk("test" , sizeof("test"));
		objChunk.Clear();
		EXPECT_EQ(objChunk.GetDataLen() , 0);
	}
	{
		CUtil::Chunk objChunk("test" , sizeof("test"));
		CUtil::Chunk objChunk1("est" , sizeof("est"));
		CUtil::Chunk objChunk2("st" , sizeof("st"));
		objChunk.Erase(objChunk.Begin() , (char *)objChunk.Begin() + 1);
		EXPECT_EQ(objChunk.GetDataLen() , objChunk1.GetDataLen());
		EXPECT_EQ(objChunk , objChunk1);
		objChunk.Erase(objChunk.Begin() , (char *)objChunk.Begin() + 1);
		EXPECT_EQ(objChunk.GetDataLen() , objChunk2.GetDataLen());
		EXPECT_EQ(objChunk , objChunk2);
	}
	{
		CUtil::Chunk objChunk("test" , sizeof("test"));
		CUtil::Chunk objChunk1("est" , sizeof("est"));
		CUtil::Chunk objChunk2("st" , sizeof("st"));
		CUtil::Chunk objChunk3;
		objChunk.Erase(objChunk.Begin() , (char *)objChunk.Begin() + 1);
		EXPECT_EQ(objChunk.GetDataLen() , objChunk1.GetDataLen());
		EXPECT_EQ(objChunk , objChunk1);
		objChunk.Erase(objChunk.Begin() , (char *)objChunk.Begin() + 1);
		EXPECT_EQ(objChunk.GetDataLen() , objChunk2.GetDataLen());
		EXPECT_EQ(objChunk , objChunk2);
		objChunk.Erase(objChunk.Begin() , (char *)objChunk.End());
		EXPECT_EQ(objChunk.GetDataLen() , objChunk3.GetDataLen());
		EXPECT_EQ(objChunk , objChunk3);
	}
	{
		CUtil::Chunk objChunk("test" , sizeof("test"));
		CUtil::Chunk objChunk1("te" , sizeof("te")-1);
		CUtil::Chunk objChunk2("st" , sizeof("st"));
		CUtil::Chunk objChunk3("tt" , sizeof("tt"));
		EXPECT_EQ((UINT32)((char*)objChunk.End() - (char*)objChunk.Begin()) , objChunk.GetDataLen());

		objChunk1.Insert(objChunk1.End() , "st" , sizeof("st"));
		EXPECT_EQ(objChunk , objChunk1);

		objChunk2.Insert(objChunk2.Begin() , "te" , sizeof("te") - 1);
		EXPECT_EQ(objChunk , objChunk2);

		objChunk3.Insert((char*)(objChunk3.Begin()) + 1 , "es" , sizeof("es") - 1);
		EXPECT_EQ(objChunk , objChunk3); 
	}
	{
		char szBuf[5 + sizeof(CUtil::ChunkData)] = {0};
		CUtil::Chunk objChunk(szBuf , sizeof(szBuf), false);
		CUtil::Chunk objChunk1("te" , sizeof("te") - 1);
		CUtil::Chunk objChunk2("test" , sizeof("test"));
		CUtil::Chunk objChunk3("test\0es" , sizeof("test\0es"));

		EXPECT_EQ(objChunk.GetDataLen() , 0);
		EXPECT_EQ(objChunk.GetSize() , sizeof(szBuf) - sizeof(CUtil::ChunkData));
		EXPECT_EQ(objChunk.GetChunkData()->GetRefCount() , -1);

		objChunk.Insert(objChunk.Begin() , "te" , sizeof("te")-1);
		EXPECT_EQ(objChunk.GetDataLen() , sizeof("te")-1);
		EXPECT_EQ(objChunk.GetSize() , sizeof(szBuf) - sizeof(CUtil::ChunkData));
		EXPECT_EQ(objChunk , objChunk1);
		EXPECT_EQ(objChunk.GetChunkData()->GetRefCount() , -1);

		objChunk.Pushback("st" , sizeof("st"));
		EXPECT_EQ(objChunk.GetDataLen() , sizeof("test"));
		EXPECT_EQ(objChunk , objChunk2);
		EXPECT_EQ(objChunk.GetSize() , sizeof(szBuf) - sizeof(CUtil::ChunkData));
		EXPECT_EQ(objChunk.GetChunkData()->GetRefCount() , -1);

		objChunk.Insert(objChunk.End() , "es" , sizeof("es"));
		EXPECT_EQ(objChunk.GetDataLen() , sizeof("test") + sizeof("es"));
		EXPECT_EQ(objChunk , objChunk3);
		EXPECT_EQ(objChunk.GetSize() , 8);
		EXPECT_EQ(objChunk.GetChunkData()->GetRefCount() , 1);

		CUtil::Chunk objChunk4 = objChunk;
		EXPECT_EQ(objChunk.GetChunkData()->GetRefCount() , 2);
		EXPECT_EQ(objChunk.GetDataLen() , objChunk3.GetDataLen()); 

		objChunk4.Insert(objChunk4.End() , "!" , sizeof("!"));
		EXPECT_EQ(objChunk.GetDataLen() , objChunk3.GetDataLen()); 
		EXPECT_EQ(objChunk.GetSize() , 8);
		EXPECT_EQ(objChunk.GetChunkData()->GetRefCount() , 1);
		EXPECT_EQ(objChunk4.GetDataLen() , objChunk3.GetDataLen() + sizeof("!")); 
		EXPECT_EQ(objChunk4.GetSize() , 16);
		EXPECT_EQ(objChunk4.GetChunkData()->GetRefCount() , 1);
	}
}
