/*
	Human player "algorithm"
	Author: Martin Cervenka
	Version: 30.03.2016
*/
#include "lang.h"
#include "stdmcr.h"
#include "person.h"
#include "card.h"
#include "hand.h"
#include <iostream>
#include <fstream>
#define MOVE_X 70

/*
	Creates person player
		=> player Name of the player
		=> ch ID of this player
*/
Person::Person(const char* player,unsigned char ch): Algorithm(player,ch){
	m_name="person";
	SetReady();
}

/*
	Proceeds the used card from any player
		=> card Played card
		=> player Player which used card
*/
void Person::Used(const Card* card,unsigned char player){
	if(card==nullptr){
		OUT(m_game->GetAlgorithm(player)->m_player << " " << ENDS << endl);
	}else{
		OUT(m_game->GetAlgorithm(player)->m_player << " " << USED << " " << *card << endl);
	}
}
void Person::Print(unsigned card,bool) const{
	OUT(MOVED(99) << MOVEC(MOVE_X) << MOVEA(2));
	unsigned char size = GetCardCount();
	for(unsigned char b=0; b<size; b++){
		OUT(*GetCard(b));
	}
	for(unsigned char b=size; b<Hand::ms_SIZE; b++){
		OUT("   ");
	}
	OUT(MOVEB(1) << MOVED(12));
	for(unsigned char b=0; b<size; b++){
		OUT(((b==card)?" @ ":"   "));
	}
	OUT(((card==size)?"-->":"   "));
}
/*
	Chooses the card which will be used
		=> force Forces player to play
		<= Card which will be used
*/

const Card* Person::Play(bool force) const{
	while(GetCardCount()>0){
		unsigned card = 0;
		while(OnePlay(card)) {;}
		if(card<GetCardCount()){
			const Card* crd = GetCard(card);
			if(force || !m_game->FirstCard() || m_game->FirstCard()->IsPlayable(crd)){
				return crd;
			}
		}else{
			if(force){
				continue;
			}
			return nullptr;
		}
	}
	return nullptr;
}

void Person::NewHand() const{

}

bool Person::OnePlay(unsigned& card) const{
	Print(card,true);
	READ(char c = getchar());
	OUT(MOVED(5) << "     " << endl);
	switch(c){
		case 91:
			READ(c = getchar());
			switch(c){
				case 68: card+=GetCardCount(); break;
				case 67: card++; break;
			}
			card%=(GetCardCount()+1);
			break;
		case 13: return false;
		case 3: CLEAR; exit(0);
	}
	return true;
}
/*
	Selects if this player wants to start new game
		<= 1 want to, -1 dont want to, 0 don't care
*/
char Person::Vote() const{
	char end;
	OUT(CONTINUE);
	cin >> end;
	switch(end){
		case NO[0]: return -1; break;
		case YES[0]: return 1; break;
		default: return 0; break;
	}
	return end!=NO[0]; //"don't care" vote
}
