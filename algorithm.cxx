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
vector<Algorithm*> Algorithm::ms_algos;
/* Card which should another player react on */
Card* Algorithm::m_first = nullptr;

/*
	Abstract algorithm constructor
		=> player Name of the player
		=> ch ID of this player
*/
Algorithm::Algorithm(const char* player,unsigned char myID){
	m_player = player;
	m_myID=myID;
	m_hand = new Hand(); //deleted in destructor
	ms_algos.push_back(this);
	m_semaphore = new Semaphore(0); //deleted in destructor
}
/*
	Copy constructor
*/
Algorithm::Algorithm(const Algorithm& algorithm){
	m_player = algorithm.m_player;
	m_myID = algorithm.m_myID;
	m_hand = algorithm.m_hand;
	m_semaphore = algorithm.m_semaphore;
	m_copy = true;
}
/*
	Lays down the card from player to table
		=> card Played card
*/
bool Algorithm::Send(Card* card){
	bool ok = (card==nullptr);
	unsigned char count = m_hand->Size();
	for(unsigned char a=0; !ok && a<count; a++){
		if(m_hand->Get(a)==card){
			ok=true;
			break;
		}
	}
	if(!ok){
		return false;
	}
	count = ms_algos.size();
	for(unsigned char a=0; a<count; a++){
		ms_algos[a]->Used(card,m_myID);
	}
	return true;
}
/*
	Returns the card which should another player react on
		<= Card to be reacted on
*/
Card* Algorithm::FirstCard(){
	return m_first;
}
/*
	Gets semapohore for this object
*/
Semaphore* Algorithm::GetSemaphore(){
	return m_semaphore;
}

/*
	Returns the player's hand
		<= Player's card 
*/
Hand* Algorithm::GetHand(){
	return m_hand;
}
/*
	Sets the card which should another player react on
		=> card Card to be reacted on
*/
void Algorithm::SetFirstCard(Card* card){
	m_first=card;
}
/*
	Adds points to this player
		=> points Points to add
*/
void Algorithm::AddPoints(unsigned char points){
	m_points += points;
}
/*
	Gets points of this player
		<= Points of this player
*/
unsigned char Algorithm::GetPoints(){
	return m_points;
}
/*
	Clears points of this player
*/
void Algorithm::ClearPoints(){
	m_points = 0;
}
/*
	Identifies the algorithm
*/
void Algorithm::Identify(){
	OUT(I_AM << m_name << endl);
}
/*
	Set this player ready
*/
void Algorithm::SetReady(bool ready){
	m_ready = ready;
}
/*
	Set this player ready
*/
void Algorithm::SetReady(){
	m_ready = true;
}
/*
	Is this player ready to start
*/
bool Algorithm::IsReady(){
	return m_ready;
}
/*
	Destructor
*/
Algorithm::~Algorithm(){
	if(!m_copy){
		delete m_hand; //created in constructor
		delete m_semaphore; //created in constructor
		delete[] m_player; //created in Configuration::Load
		unsigned len = ms_algos.size();
		for(unsigned a=0; a<len; a++){
			if(this == ms_algos[a]){
				ms_algos.erase(ms_algos.begin()+a);
				break;
			}
		}
	}
}
