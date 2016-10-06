#include "testing.h"
#include "../algorithm.h"
#include "../algono.h"

NAMESPACE(3)
	AlgoNo algo("TESTING_ONLY",0); //using algono because of abstract class
FIRST_TEST("Init test")
	ASSERT((unsigned)algo.GetHand()->Size(),0);
TEST(0,1,"First card get NULL")
	ASSERT(Algorithm::FirstCard(),NULL)
TEST(1,2,"First card set")
	Card card(0b101);
	Algorithm::SetFirstCard(&card);
	ASSERT(Algorithm::FirstCard(),&card);
TEST(2,3,"Send NULL test")
	ASSERT(algo.Send(NULL),true)
TEST(3,4,"Send bad card")
	Card* card = new Card(0b101);
	ASSERT(algo.Send(card),false)
	delete card;
TEST(4,5,"Send good card")
	Card* card = new Card(0b101);
	algo.GetHand()->Add(card);
	ASSERT(algo.Send(card),true)
	delete card;
END
START(2,3,5)
END
