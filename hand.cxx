/*
	Hand (set) of cards
	Author: Martin Cervenka
	Version: 30.03.2016
*/
#include "hand.h"
#include <iostream>

/*
	Sets the card of specified position
	DEPRECATED - do not use this method, use Add instead
		=> position Position to set
		=> card Card which will be pushed on this position
*/
bool Hand::Set(unsigned char position,Card* card){
	if(position<ms_SIZE){
		m_cards[position]=card;
		m_count = position+1;
		return true;
	}
	return false;
}
/*
	Returns size of the hand (number of cards)
		<= Number of cards in hand
*/
unsigned char Hand::Size(){
	return m_count;
}
/*
	Returns the card on specified position
		<= index Specified position
		=> Card on position
*/
Card* Hand::Get(unsigned char index){
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
Card* Hand::Use(unsigned char index){
	if(index>=m_count){
		cerr << "Card does not exists" << endl;
		return nullptr;
	}
	Card* tmp = m_cards[index];
	m_cards[index]=m_cards[--m_count];
	return tmp;
}
/*
	Adds the card to hand
		=> card Card to be added
		<= Has been added?
*/
bool Hand::Add(Card* card){
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
	m_count = 0;
}
