/*
	Class reprezenting cards deck
	Author: Martin Cervenka
	Version: 30.03.2016
*/
#include "deck.h"
#include "card.h"
#include "stdmcr.h"
#include <iostream>
#include <fstream>
/*
	Shuffles this deck
*/
void Deck::Shuffle(){
	unsigned char size=Size();
	srand(time(nullptr));
	for(unsigned char a=0; a<size; a++){
		unsigned char random = rand()%size;
		//swap
		Card* tmp = m_cards[random];
		m_cards[random]=m_cards[a];
		m_cards[a]=tmp;
	}
}
/*
	Prints this deck
*/
void Deck::Print(){
	unsigned char size=Size();
	for(unsigned a=0; a<size; a++){
		OUT(*m_cards[a]);
		if((a&15)==15){
			OUT(endl);
		}else{
			OUT("  ");
		}
	}
	OUT(endl);
}
/*
	Pops the first card from this deck
	Removes the card
		<= First card on this deck
*/
Card* Deck::Pop(){
	unsigned char size=Size();
	Card* popped = m_cards[size-1];
	m_count--;
	return popped;	
}
/*
	Looks on the first card from this deck
	Does not remove the card
		<= First card on this deck
*/
Card* Deck::Peek(){
	return m_cards[Size()-1];
}
/*
	Push the card to the deck
		=> card Card to be pushed
		<= Is successfully pushed?
*/
bool Deck::Push(Card* card){
	unsigned char size = Size();
	if(size == ms_COUNT){
		return false;
	}else{
		m_cards[m_count]=card;
		m_count++;
		return true;
	}
}
/*
	Returns size of this deck
		<= Size of this deck
*/
unsigned char Deck::Size(){
	return m_count;
}
/*
	Fills the deck with the cards (in order)
*/
void Deck::Fill(){
	m_count=0;
	for(unsigned char a=0; a<ms_COUNT; a++){
		Card* card = new Card(a); //deleted in destructor (x/32) ||  Hand::Clear function ((32-x)/32)
		Push(card); 
	}
}
/*
	Throw away cards because of 3, 5 etc. number of players
		=> count Count of the card which will be throwed away from this deck
*/
void Deck::ThrowAway(unsigned char count){
	Fill();
	for(unsigned char a=0; a<count; a++){
		//4 because deck begins with 4 cards of rank 7
		m_cards[a+4]=m_cards[ms_COUNT-a-1];
	}
	m_count-=count;
}

/*
	Destruct the deck
*/
Deck::~Deck(){
	for(unsigned char a=0; a<m_count; a++){
		delete m_cards[a]; //created in Fill function (x/32)
	}
}
