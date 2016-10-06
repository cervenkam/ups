#include "testing.h"
#include "../hand.h"

NAMESPACE(1)
Hand hand;
Card card(0b100);
FIRST_TEST("Size 0 of the free hand")
	ASSERT((unsigned)hand.Size(),0)
TEST(0,1,"Add card test")
	hand.Add(&card);
	ASSERT(hand.Get(0),&card)
TEST(1,2,"Get bad element")
	ASSERT(hand.Get(1),NULL)
TEST(2,3,"Set bad element")
	bool res = hand.Set(10,&card);
	ASSERT(res,false);
TEST(3,4,"Set good element")
	bool res = hand.Set(3,&card);
	ASSERT(res,true);
	ASSERT(hand.Get(3),&card);
TEST(4,5,"Use test")
	Hand hand;
	hand.Add(&card);
	ASSERT((unsigned)hand.Size(),1)
	hand.Use(0);
	ASSERT((unsigned)hand.Size(),0)
END
START(0,1,5)
END
