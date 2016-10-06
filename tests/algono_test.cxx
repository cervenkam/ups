#include "testing.h"
#include "../algono.h"

NAMESPACE(4)
	AlgoNo algo("FOR_TESTING_ONLY",0);
	FIRST_TEST("Play test force no card")
		ASSERT(algo.Play(true),NULL)
	TEST(0,1,"Play test force 1 card")
		Algorithm::SetFirstCard(NULL);
		Card* card = new Card(0b01010);
		algo.GetHand()->Add(card);
		DEBUG("Play " << *card <<endl)
		ASSERT(algo.Play(true),card);
	TEST(1,2,"Card not removed")
		ASSERT((unsigned)algo.GetHand()->Size(),1)
	TEST(2,3,"Fill hand")
		Hand* h = algo.GetHand();
		Card* c1 = new Card(0b10001);
		h->Add(c1);
		Card* c2 = new Card(0b11011);
		h->Add(c2);
		Card* c3 = new Card(0b1101);
		h->Add(c3);
		DEBUG("Play " << *h->Get(0) << *h->Get(1) << *h->Get(2) << *h->Get(3) << endl)
		ASSERT((unsigned)algo.GetHand()->Size(),4)
	TEST(3,4,"Fitting card on index 1")
		Card c(0b10001);
		DEBUG("First card " << c << endl)
		Algorithm::SetFirstCard(&c);
		ASSERT(algo.Play(true),algo.GetHand()->Get(1))
	TEST(4,5,"No fitting card")
		Card c(0b111);
		DEBUG("First card " << c << endl)
		Algorithm::SetFirstCard(&c);
		ASSERT(algo.Play(true),algo.GetHand()->Get(0))
	END
	START(3,4,5)
END
