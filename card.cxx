/*
	Class reprezenting playing card
	Author: Martin Cervenka
	Version: 30.03.2016
*/
#include <ostream>
#include "card.h"
#include "stdmcr.h"
using namespace std; 
/*
	Card constructor
		=> value Value of the card (0-31)
*/
Card::Card(unsigned char value){
	this->value = value;
}

/*
	Returns the rank of this card
		<= Rank of the card
*/
unsigned char Card::GetRank() const{
	return value>>2; 
}
/*
	Returns the color of this card
		<= Color of the card
*/
unsigned char Card::GetColor() const{
	return value&3;
}
/*
	Sends information of the card to output stream
		=> os Output stream where will be card sent
		=> card Sending card
		<= Output stream sent as os
*/
ostream& operator<<(ostream& cout,const Card& card){
	char ranks[8] = {'7','8','9','X','J','Q','K','A'}; //TODO language
	//return os << "\033[1;4" << (card.GetRank()+1) << "m " << (unsigned)card.GetColor() << " \033[0m";
	unsigned color = 1;
	if(card.IsSpecial()){
		color = 7;
	}
	return COLOR(color << ";3" << (card.GetColor()+1)," " << ranks[card.GetRank()] << " ");
}
/*
	Is the card playable on this card
		=> card THE card
		<= Is playable?
*/
bool Card::IsPlayable(Card* card) const{
	if(card==NULL){
		return true;
	}
	if(card->GetRank()==GetRank()){
		return true;
	}
	if(card->GetRank()==0){
		return true;		
	}
	return false;
}
/*
	Is this card valuable
		<= Is valuable
*/
bool Card::IsValuable() const{
	return ((value&0b1100) == 0b1100);
}
/*
	Is this card special
		<= Is special
*/
bool Card::IsSpecial() const{
	return IsValuable() || GetRank()==0;
}
