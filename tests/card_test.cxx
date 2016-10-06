#include "testing.h"
#include "../card.h"
#include <iostream>

NAMESPACE(0)
FIRST_TEST("R7 Card color check")
	Card card(0);
	ASSERT((unsigned)card.GetColor(),0);
TEST(0,1,"R7 Card rank check")
	Card card(0);
	ASSERT((unsigned)card.GetRank(),0);
TEST(1,2,"R8 on R7 Card playable check")
	Card c1(0b000);
	Card c2(0b100);
	DEBUG(c1 << " on " << c2 << endl)
	ASSERT(c2.IsPlayable(&c1),true);
TEST(2,3,"R7 on R8 Card playable check")
	Card c1(0b100);
	Card c2(0b000);
	DEBUG(c1 << " on " << c2 << endl)
	ASSERT(c2.IsPlayable(&c1),false);
TEST(3,4,"NULL on R7 Card playable check")
	Card c1(0b000);
	DEBUG("NULL on " << c1 << endl)
	ASSERT(c1.IsPlayable(NULL),true); 
TEST(4,5,"G9 on B9 Card playable check")
	Card c1(0b1011);
	Card c2(0b1001);
	DEBUG(c2 << " on " << c1 << endl)
	ASSERT(c1.IsPlayable(&c2),true); 
TEST(5,6,"B9 on BX Card playable check")
	Card c1(0b1111);
	Card c2(0b1011);
	DEBUG(c2 << " on " << c1 << endl)
	ASSERT(c1.IsPlayable(&c2),false); 
TEST(6,7,"Valuable card X test")
	Card c1(0b1111);
	DEBUG("Valuable card " << c1 << endl)
	ASSERT(c1.IsValuable(),true)
TEST(7,8,"Valuable card E test")
	Card c1(0b11101);
	DEBUG("Valuable card " << c1 << endl)
	ASSERT(c1.IsValuable(),true)
TEST(8,9,"Non-Valuable card 9 test")
	Card c1(0b1001);
	DEBUG("Valuable card " << c1 << endl)
	ASSERT(c1.IsValuable(),false)
	
END
FIRST_START(9)
END
