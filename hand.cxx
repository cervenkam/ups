/*
	Hand (set) of cards
	Author: Martin Cervenka
	Version: 30.03.2016
*/
#include "hand.h"
#include <iostream>

/*
	Returns size of the hand (number of cards)
		<= Number of cards in hand
*/
unsigned char Hand::Size() const{
	return m_count;
}
/*
	Returns the card on specified position
		<= index Specified position
		=> Card on position
*/
const Card* Hand::Get(unsigned char index) const{
	if(index>=m_count){
		return nullptr;
	}else{
		return m_cards[index];
	}
}
/*
	Uses the card
		=> index Position of the card in hand
		<= Card to be used
*/
const Card* Hand::Use(unsigned char index){
	if(index>=m_count){
		cerr << "Card does not exists" << endl;
		return nullptr;
	}
	const Card* tmp = m_cards[index];
	m_cards[index]=m_cards[--m_count];
	return tmp;
}
/*
	Adds the card to hand
		=> card Card to be added
		<= Has been added?
*/
bool Hand::Add(const Card* card){
	if(m_count==ms_SIZE){
		return false;
	}
	m_cards[m_count++]=card;
	return true;
}
/*
	Clears the hand
*/
void Hand::Clear(){
	for(unsigned a=0; a<m_count; a++){
		delete m_cards[a]; //created in Deck::Fill ((32-x)/32)
	}
	m_count = 0;
}

/*
	Destruct the hand
*/
Hand::~Hand(){
	Clear();
}
