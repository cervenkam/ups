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
	m_name="algono";
	SetReady();
}

/*
	Is called when a card is used (with any player)
		=> card Player card
		=> player ID of the player which played card
*/
void AlgoNo::Used(const Card*,unsigned char){}
/*
	Defines the card which this player will use
		=> force Force the player to play? (he is not lay down the first card)
		<= Card which will player use
*/
const Card* AlgoNo::Play(bool force) const{
	if(!force){
		return nullptr;
	}
	const Card* first = m_game->FirstCard();
	unsigned char size = GetCardCount();
	for(unsigned char a=0; a<size; a++){
		if(first==nullptr || GetCard(a)->IsPlayable(first)){
			return GetCard(a);
		}
	}
	return GetCard(0); //HACK dangerous
}
/*
	Selects if this player wants to start new game
		<= 1 want to, -1 dont want to, 0 don't care
*/
char AlgoNo::Vote() const{
	return 0; //"don't care" vote
}
void AlgoNo::NewHand() const{

}
/*
	Destructor
*/
AlgoNo::~AlgoNo(){

}
