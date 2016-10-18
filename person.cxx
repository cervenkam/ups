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
void Person::Used(Card* card,unsigned char player){
	if(card==nullptr){
		OUT(ms_algos[player]->m_player << " " << ENDS << endl);
	}else{
		OUT(ms_algos[player]->m_player << " " << USED << " " << *card << endl);
	}
}
void Person::Print(unsigned card,bool clear){
	(void)clear;
	OUT(MOVED(99) << MOVEC(MOVE_X) << MOVEA(2));
	Hand* hand = GetHand();
	unsigned char size = hand->Size();
	for(unsigned char b=0; b<size; b++){
		OUT(*hand->Get(b));
	}
	for(unsigned char b=size; b<Hand::ms_SIZE; b++){
		OUT("   ");
	}
	OUT(MOVEB(1) << MOVED(12));
	for(unsigned char b=0; b<size; b++){
		OUT(((b==card)?" @ ":"   "));
	}
	if(card==size){
		OUT("-->");
	}else{
		OUT("   ");
	}
}
/*
	Chooses the card which will be used
		=> force Forces player to play
		<= Card which will be used
*/

Card* Person::Play(bool force){
	Hand* hand = GetHand();
	while(hand->Size()>0){
		unsigned card = 0;
		while(1){
			Print(card,true);
			RAW_READ;
			char c = getchar();
			COOKED_READ;
			OUT(MOVED(5) << "     " << endl);
			if(c==91){
				RAW_READ;
				c = getchar();
				COOKED_READ;
				if(c==68){
					card+=hand->Size();
				}else if(c==67){
					card++;
				}
				card%=(hand->Size()+1);
			}else if(c==13){
				break;
			}else if(c==3){
				CLEAR;
				exit(0);
			}
		}
		if(card<hand->Size()){
			Card* crd = hand->Get(card);
			if(force || !Algorithm::FirstCard() || Algorithm::FirstCard()->IsPlayable(crd)){
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
