#include "testing.h"
#include "../deck.h"

NAMESPACE(2)
Deck deck;
FIRST_TEST("Creation size test")
	ASSERT((unsigned)deck.Size(),0)
TEST(0,1,"Fill test")
	deck.Fill();
	ASSERT((unsigned)deck.Size(),32)
TEST(1,2,"Last card peek test")
	ASSERT((unsigned)deck.Peek()->GetRank(),7)
	ASSERT((unsigned)deck.Peek()->GetColor(),3)
TEST(2,3,"Push fail test")
	Card card(0b000);
	deck.Push(&card);
	ASSERT((unsigned)deck.Size(),32)
TEST(3,4,"Pop test")
	deck.Pop();
	ASSERT((unsigned)deck.Size(),31)
TEST(4,5,"Penultimate pop test")
	ASSERT((unsigned)deck.Peek()->GetRank(),7)
	ASSERT((unsigned)deck.Peek()->GetColor(),2)
TEST(5,6,"Push test")
	Card card(0b101);
	deck.Push(&card);
	ASSERT((unsigned)deck.Size(),32)
//SHUFFLE NOT TESTED !!!
TEST(6,7,"Throw away test")
	deck.ThrowAway(4);
	ASSERT((unsigned)deck.Size(),28)
	ASSERT((unsigned)deck.Peek()->GetRank(),6)
	ASSERT((unsigned)deck.Peek()->GetColor(),3)
END
START(1,2,7)
END
