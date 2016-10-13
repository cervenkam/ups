/*
	Simple bot algorithm
	Author: Martin Cervenka
	Version: 30.03.2016
*/
#include "algono.h"
#include "card.h"
#include "hand.h"
#include <iostream>

/*
	Simple bot constructor
		=> ch ID of this player
		=> player Name of the player
*/
AlgoNo::AlgoNo(const char* player,unsigned char ch): Algorithm(player,ch){
	name="algono";
	SetReady();
}

/*
	Is called when a card is used (with any player)
		=> card Player card
		=> player ID of the player which played card
*/
void AlgoNo::Used(Card* card,unsigned char player){ (void)card; (void)player; }
/*
	Defines the card which this player will use
		=> force Force the player to play? (he is not lay down the first card)
		<= Card which will player use
*/
Card* AlgoNo::Play(bool force){
	if(!force){
		return NULL;
	}
	Hand* hand = GetHand();
	Card* first = FirstCard();
	unsigned char size = hand->Size();
	for(unsigned char a=0; a<size; a++){
		if(first==NULL || hand->Get(a)->IsPlayable(first)){
			return hand->Get(a);
		}
	}
	return hand->Get(0); //HACK dangerous
}
