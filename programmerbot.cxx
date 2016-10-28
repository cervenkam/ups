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
#define FIND_DO(x,y) { \
		Hand* hnd = GetHand(); \
		unsigned char sze = hnd->Size(); \
		for(unsigned a=0; a<sze; a++){ \
			Card* card = hnd->Get(a); \
			if(x){ \
				y; \
			} \
		}\
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
	m_name="programmerBot";
	SetReady();
}

ProgrammerBot::ProgrammerBot(const Algorithm& algo): Algorithm(algo){
	m_name="programmerBot";
	SetReady();
}
/*
	Is called when a card is used (with any player)
		=> card Player card
		=> player ID of the player which played card
*/
void ProgrammerBot::Used(Card* card,unsigned char player){
	if(card==nullptr){
		return;
	}
	switch(card->GetRank()){
		case CARD_7: m_counter7++; break;
		case CARD_X: m_counterX++; break;
		case CARD_E: m_counterE++; break;
	}
	(void)player;
}
/*
	Defines the card which this player will use
		=> force Force the player to play? (he is not lay down the first card)
		<= Card which will player use
*/
Card* ProgrammerBot::Play(bool force){
	if(force){
		Card* crd = nullptr;
		if((crd = TryPutFirstCard())){
			return crd;
		}
		if((crd = TryValuableCard())){
			return crd;
		}
		return GetAnythingElse();
	}else{
		if(m_game->FirstCard()->IsValuable()){
			return nullptr;
		}else if(!m_game->FirstCard()->IsSpecial()){
			FIND(card->GetRank()==m_game->FirstCard()->GetRank())
		}
		return nullptr;
	}
}
Card* ProgrammerBot::GetAnythingElse(){
	if(!m_game->FirstCard()->IsValuable()){
		FIND(card->GetRank()==m_game->FirstCard()->GetRank())
	}
	FIND_P(0.9,!card->IsSpecial())
	FIND_P(0.7,card->GetRank()==CARD_7)
	return GetHand()->Get(0); // TODO dangerous
}
Card* ProgrammerBot::TryPutFirstCard(){
	if(!m_game->FirstCard()){
		return GetRandomCard();
	}else{
		return nullptr;
	}
}
Card* ProgrammerBot::GetRandomCard(){
	Hand* hand = GetHand();
	unsigned char size = hand->Size();
	unsigned count = 0;
	unsigned index = 0;
	for(unsigned x=0; x<size; x++){
		Card* c1 = hand->Get(x);
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
Card* ProgrammerBot::TryValuableCard(){
	if(m_game->FirstCard()->IsValuable()){
		int count = 0;
		FIND_DO(card->GetRank()==m_game->FirstCard()->GetRank() || card->GetRank()==CARD_7,count++)
		if(count>0){
			return TryByCardCounts(count);
		}
	}
	return nullptr;
}
Card* ProgrammerBot::TryByCardCounts(unsigned count){
	unsigned alreadyPlayed = m_counter7;
	if(m_game->FirstCard()->GetRank()==CARD_X){
		alreadyPlayed += m_counterX;
	}else{
		alreadyPlayed += m_counterE;
	}
	unsigned rest = 8 - alreadyPlayed - count;
	if(rest-HAZARD<count){
		FIND(card->GetRank()==m_game->FirstCard()->GetRank())
		FIND(card->GetRank()==CARD_7)
	}
	return nullptr;
}
/*
	Selects if this player wants to start new game
		<= 1 want to, -1 dont want to, 0 don't care
*/
char ProgrammerBot::Vote(){
	return 0; //"don't care" vote
}
/*
	Destructor
*/
ProgrammerBot::~ProgrammerBot(){

}
