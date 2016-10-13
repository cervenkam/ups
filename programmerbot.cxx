/*
	Bot programmed as I would play
	Author: Martin Cervenka
	Version: 10.04.2016
*/
#include "programmerbot.h"
#include "card.h"
#include "hand.h"
#include "limits.h"
#include <iostream>
#define HAZARD 2
#define FIND_DO(x,y) \
		for(unsigned a=0; a<size; a++){ \
			Card* card = hand->Get(a); \
			if(x){ \
				y; \
			} \
		} 
#define FIND(x) FIND_DO(x,return card)
#define FIND_DO_P(p,x,y) \
	if((double)rand()/RAND_MAX < p){ \
		FIND_DO(x,y)\
	}
#define FIND_P(p,x) FIND_DO_P(p,x,return card)

/*
	Programmer bot constructor
		=> player Name of the player
		=> ch ID of this player
*/
ProgrammerBot::ProgrammerBot(const char* player,unsigned char ch): Algorithm(player,ch){
	name="programmerBot";
	SetReady();
}

/*
	Is called when a card is used (with any player)
		=> card Player card
		=> player ID of the player which played card
*/
void ProgrammerBot::Used(Card* card,unsigned char player){
	if(card==NULL){
		return;
	}
	switch(card->GetRank()){
		case CARD_7: counter7++; break;
		case CARD_X: counterX++; break;
		case CARD_E: counterE++; break;
	}
	(void)player;
}
/*
	Defines the card which this player will use
		=> force Force the player to play? (he is not lay down the first card)
		<= Card which will player use
*/
Card* ProgrammerBot::Play(bool force){
	Hand* hand = GetHand();
	unsigned char size = hand->Size();
	if(force){
		//I should put first card
		if(FirstCard()==NULL){
			//find strong rank
			unsigned count = 0;
			unsigned index = 0;
			for(unsigned x=0; x<size; x++){
				Card* c1 = hand->Get(x);
				//I dont want to use 7 as first card
				if(c1->GetRank()==CARD_7){
					continue;
				}
				unsigned duplicates = 0;
				FIND_DO_P(0.95,c1->GetRank()==card->GetRank(),duplicates++)
				if(duplicates>count){
					count = duplicates;
					index = x;
				}
			}
			return hand->Get(index);
		}
		//is there a fight about valuable cards
		if(FirstCard()->IsValuable()){
			int count = 0;
			//I'll have "count" number of cards to fight against
			FIND_DO(card->GetRank()==FirstCard()->GetRank() || card->GetRank()==CARD_7,count++)
			if(count>0){
				unsigned alreadyPlayed = counter7;
				if(FirstCard()->GetRank()==CARD_X){
					alreadyPlayed += counterX;
				}else{
					alreadyPlayed += counterE;
				}
				int rest = 8 - alreadyPlayed - count;
				if(rest-HAZARD<count){
					//ok, lets fight - find valuable card
					FIND(card->GetRank()==FirstCard()->GetRank())
					//else use 7
					FIND(card->GetRank()==CARD_7)
				}
			}
		}
		//Find same rank card if it is non-valuable
		if(!FirstCard()->IsValuable()){
			FIND(card->GetRank()==FirstCard()->GetRank())
		}
		//Find some not special card
		FIND_P(0.9,!card->IsSpecial())
		//I have to give special card - so find 7 ;)
		FIND_P(0.7,card->GetRank()==CARD_7)
		//ok then, I have to play X or E, so give first card
		return hand->Get(0); // TODO dangerous
	}else{
		//I can collect values - do it
		if(FirstCard()->IsValuable()){
			return NULL;
		}else if(!FirstCard()->IsSpecial()){
			//force next loop with non-valuable card
			FIND(card->GetRank()==FirstCard()->GetRank())
		}
		//do not continue
		return NULL;
	}
}
