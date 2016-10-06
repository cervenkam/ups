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
	srand(time(NULL));
	for(unsigned char a=0; a<size; a++){
		unsigned char random = rand()%size;
		//swap
		Card* tmp = cards[random];
		cards[random]=cards[a];
		cards[a]=tmp;
	}
}
/*
	Prints this deck
*/
void Deck::Print(){
#ifdef TESTING
	ofstream cout("last_game.txt",ofstream::app);
#endif
	unsigned char size=Size();
	for(unsigned a=0; a<size; a++){
		OUT(*cards[a]);
		if((a&15)==15){
			OUT(endl);
		}else{
			OUT("  ");
		}
	}
	OUT(endl);
#ifdef TESTING
	cout.close();
#endif
}
/*
	Pops the first card from this deck
	Removes the card
		<= First card on this deck
*/
Card* Deck::Pop(){
	unsigned char size=Size();
	Card* popped = cards[size-1];
	count--;
	return popped;	
}
/*
	Looks on the first card from this deck
	Does not remove the card
		<= First card on this deck
*/
Card* Deck::Peek(){
	return cards[Size()-1];
}
/*
	Push the card to the deck
		=> card Card to be pushed
		<= Is successfully pushed?
*/
bool Deck::Push(Card* card){
	unsigned char size = Size();
	if(size == COUNT){
		return false;
	}else{
		cards[count]=card;
		count++;
		return true;
	}
}
/*
	Returns size of this deck
		<= Size of this deck
*/
unsigned char Deck::Size(){
	return count;
}
/*
	Fills the deck with the cards (in order)
*/
void Deck::Fill(){
	count=0;
	for(unsigned char a=0; a<COUNT; a++){
		Push(new Card(a));
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
		cards[a+4]=cards[COUNT-a-1];
	}
	this->count-=count;
}
