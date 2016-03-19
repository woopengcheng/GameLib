#include "CUtil/inc/CRTTI.h"
#include "CUtil/inc/CObject.h"
/*#include "UnitTest++/UnitTestPP.h"*/
#include "gtest/gtest.h"


TEST(CRTTI , RTTI)
{
	const INT32 nA = 1;
	const INT32 nB = 2;
	const INT32 nC = 3;
	const INT32 nD = 4;
	class A : virtual public CUtil::CObject
	{
		RTTI(A , 1 , BASE(CObject));
	}; 
	class B : virtual public A
	{
		RTTI(B , 2 , BASE(A));
	}; 
	class C : virtual public A
	{
		RTTI(C , 3 , BASE(A));
	}; 
	class D : virtual public C , virtual public B
	{
		RTTI(D , 4 , BASE(C) , BASE(B));
	}; 
	class E : virtual public D
	{
		RTTI(E , 5 , BASE(D));
	}; 
	class F : virtual public CUtil::CObject
	{
		RTTI(F , 6 , BASE(CUtil::CObject));
	}; 
	class G : virtual public E , virtual public F
	{
		RTTI(G , 7 , BASE(E) , BASE(F));
	}; 
	class H : public C , public B , public F 
	{
		RTTI(G , 8 , BASE(C) , BASE(B) , BASE(F));
	}; 

	A a; 
	EXPECT_EQ(a.GetRTTI()->GetClassGUID() , 1);
	EXPECT_EQ(a.GetRTTI()->GetClassName() , "A");
	EXPECT_EQ(a.GetRTTI()->GetClassSize() , sizeof(A));

	EXPECT_EQ(a.GetRTTI()->IsDerivedFrom(BASE(CUtil::CObject)) , TRUE);
	EXPECT_EQ(B::GetClass()->IsDerivedFrom(BASE(A)) , TRUE);
	EXPECT_EQ(C::GetClass()->IsDerivedFrom(BASE(A)) , TRUE);
	EXPECT_EQ(D::GetClass()->IsDerivedFrom(BASE(CUtil::CObject)) , TRUE);
	EXPECT_EQ(D::GetClass()->IsDerivedFrom(BASE(A)) , TRUE);
	EXPECT_EQ(D::GetClass()->IsDerivedFrom(BASE(B)) , TRUE);
	EXPECT_EQ(D::GetClass()->IsDerivedFrom(BASE(C)) , TRUE);
	EXPECT_EQ(E::GetClass()->IsDerivedFrom(BASE(CUtil::CObject)) , TRUE);
	EXPECT_EQ(E::GetClass()->IsDerivedFrom(BASE(A)) , TRUE);
	EXPECT_EQ(E::GetClass()->IsDerivedFrom(BASE(B)) , TRUE);
	EXPECT_EQ(E::GetClass()->IsDerivedFrom(BASE(C)) , TRUE);
	EXPECT_EQ(E::GetClass()->IsDerivedFrom(BASE(D)) , TRUE);
	EXPECT_EQ(G::GetClass()->IsDerivedFrom(BASE(CUtil::CObject)) , TRUE); 
	EXPECT_EQ(G::GetClass()->IsDerivedFrom(BASE(A)) , TRUE);
	EXPECT_EQ(G::GetClass()->IsDerivedFrom(BASE(B)) , TRUE);
	EXPECT_EQ(G::GetClass()->IsDerivedFrom(BASE(C)) , TRUE);
	EXPECT_EQ(G::GetClass()->IsDerivedFrom(BASE(D)) , TRUE);
	EXPECT_EQ(G::GetClass()->IsDerivedFrom(BASE(E)) , TRUE);
	EXPECT_EQ(G::GetClass()->IsDerivedFrom(BASE(F)) , TRUE);
	EXPECT_EQ(H::GetClass()->IsDerivedFrom(BASE(CUtil::CObject)) , TRUE); 
	EXPECT_EQ(H::GetClass()->IsDerivedFrom(BASE(A)) , TRUE);
	EXPECT_EQ(H::GetClass()->IsDerivedFrom(BASE(B)) , TRUE);
	EXPECT_EQ(H::GetClass()->IsDerivedFrom(BASE(C)) , TRUE); 
	EXPECT_EQ(H::GetClass()->IsDerivedFrom(BASE(F)) , TRUE);

	CUtil::CObject * pCObject = H::CreateObject();
	EXPECT_EQ(pCObject->GetRTTI()->IsDerivedFrom(BASE(CUtil::CObject)) , TRUE); 
	EXPECT_EQ(pCObject->GetRTTI()->IsDerivedFrom(BASE(A)) , TRUE);
	EXPECT_EQ(pCObject->GetRTTI()->IsDerivedFrom(BASE(B)) , TRUE);
	EXPECT_EQ(pCObject->GetRTTI()->IsDerivedFrom(BASE(C)) , TRUE); 
	EXPECT_EQ(pCObject->GetRTTI()->IsDerivedFrom(BASE(F)) , TRUE);

//	SAFE_DELETE(pCObject);
	delete pCObject;
}