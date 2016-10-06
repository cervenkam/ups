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
	if(position<SIZE){
		cards[position]=card;
		count = position+1;
		return true;
	}
	return false;
}
/*
	Returns size of the hand (number of cards)
		<= Number of cards in hand
*/
unsigned char Hand::Size(){
	return count;
}
/*
	Returns the card on specified position
		<= index Specified position
		=> Card on position
*/
Card* Hand::Get(unsigned char index){
	if(index>=count){
		return NULL;
	}else{
		return cards[index];
	}
}
/*
	Uses the card
		=> index Position of the card in hand
		<= Card to be used
*/
Card* Hand::Use(unsigned char index){
	if(index>=count){
		cerr << "Card does not exists" << endl;
		return NULL;
	}
	Card* tmp = cards[index];
	cards[index]=cards[--count];
	return tmp;
}
/*
	Adds the card to hand
		=> card Card to be added
		<= Has been added?
*/
bool Hand::Add(Card* card){
	if(count==SIZE){
		return false;
	}
	cards[count++]=card;
	return true;
}
/*
	Clears the hand
*/
void Hand::Clear(){
	count = 0;
}
