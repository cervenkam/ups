/*
	Abstract algorithm
	Author: Martin Cervenka
	Version: 30.03.2016
*/
#include "algorithm.h"
#include "card.h"
#include "hand.h"
#include <vector>
#include <iostream>
#include "stdmcr.h"

using namespace std;

/* Static list of all algorithms in the play */
vector<Algorithm*> Algorithm::algos;
/* Card which should another player react on */
Card* Algorithm::first = NULL;

/*
	Abstract algorithm constructor
		=> player Name of the player
		=> ch ID of this player
*/
Algorithm::Algorithm(const char* player,unsigned char myID){
	this->player = player;
	this->myID=myID;
	this->hand = new Hand();
	algos.push_back(this);
}
/*
	Lays down the card from player to table
		=> card Played card
*/
bool Algorithm::Send(Card* card){
	bool ok = (card==NULL);
	unsigned char count = hand->Size();
	for(unsigned char a=0; !ok && a<count; a++){
		if(hand->Get(a)==card){
			ok=true;
			break;
		}
	}
	if(!ok){
		return false;
	}
	count = algos.size();
	for(unsigned char a=0; a<count; a++){
		algos[a]->Used(card,myID);
	}
	return true;
}
/*
	Returns the card which should another player react on
		<= Card to be reacted on
*/
Card* Algorithm::FirstCard(){
	return first;
}
/*
	Returns the player's hand
		<= Player's card 
*/
Hand* Algorithm::GetHand(){
	return hand;
}
/*
	Sets the card which should another player react on
		=> card Card to be reacted on
*/
void Algorithm::SetFirstCard(Card* card){
	first=card;
}
/*
	Adds points to this player
		=> points Points to add
*/
void Algorithm::AddPoints(unsigned char points){
	this->points += points;
}
/*
	Gets points of this player
		<= Points of this player
*/
unsigned char Algorithm::GetPoints(){
	return points;
}
/*
	Clears points of this player
*/
void Algorithm::ClearPoints(){
	points = 0;
}
/*
	Identifies the algorithm
*/
void Algorithm::Identify(){
	OUT("I am " << name << endl);
}
