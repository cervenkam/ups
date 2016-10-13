/*
	Class reprezenting playing card
	Author: Martin Cervenka
	Version: 30.03.2016
*/
#include <ostream>
#include <cstring>
#include "card.h"
#include "stdmcr.h"
#ifdef LANG_CS
const char* Card::ranks[8] = {"7","8","9","10","SPODEK","SVRSEK","KRAL","ESO"};
const char* Card::colors[4] = {"CERVENY", "ZELENY", "KULE", "ZALUDY"};
#else
const char* Card::ranks[8] = {"7","8","9","X","JACK","QUEEN","KING","ACE"};
const char* Card::colors[4] = {"HEART", "LEAF", "BELL", "ACORN"};
#endif
using namespace std; 
/*
	Card constructor
		=> value Value of the card (0-31)
*/
Card::Card(unsigned char value){
	this->value = value;
	unsigned color = GetColor();
	unsigned rank = GetRank();
	unsigned c_len = strlen(colors[color]);
	unsigned r_len = strlen(ranks[rank]);
	name = new char[r_len+c_len+2];
	strcpy(name,colors[color]);
	name[c_len]=' ';
	strcpy(name+c_len+1,ranks[rank]);
}

/*
	Card destructor
*/
Card::~Card(){
	delete[] name;
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
	unsigned color = 1;
	if(card.IsSpecial()){
		color = 7;
	}
	return COLOR(color << ";3" << (card.GetColor()+1)," " << Card::ranks[card.GetRank()][0] << " ");
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
/*
	Converts number reprezentation to string
		<= String reprezentation
*/
char* Card::ToString(){
	fprintf(stdout,"%5d: %p\n",value,name);
	fflush(stdout);
	return name;	
}
/*
	Converts string reprezentation to number reprezentation
		=> str String reprezentation
		<= Number reprezentation
*/
unsigned char Card::FromString(char* str){
	char* sec = str;
	unsigned len = strlen(str);
	for(unsigned a=0; a<len; a++){
		if(str[a]==' '){
			str[a]='\0';
			sec = str+a+1;
		}
	}
	unsigned a=0;
	char l1 = str[0];
	char l2 = sec[0];
#ifdef LANG_CZ
	unsigned len1 = strlen(str);
	char m1 = len1=0?'\0':str[1];
	a|=(m1!='E')? 2:0; //Kule, Zaludy
	a|=(l1=='Z')? 1:0; //Zeleny, Zaludy
#else
	a|=(l1<='B')? 2:0; //Bell, Acorn
	a|=(l1=='A' || l1=='L')? 1:0; //Leaf, Acorn
#endif
	if(!strcmp(colors[a],str)){
		return 1<<5;
	}
	unsigned b=0;
#ifdef LANG_CZ
	unsigned len2 = strlen(sec);
	char m2 = len2=0?'\0':sec[1];
	b|=(l2>='E')? 4:0; //Spodek, Svrsek, Kral, Eso
	b|=(l2!='S' && l2!='7' && l2!='8')? 2:0; //9, 10, Kral, Eso
	b|=(m2>='S' || l2=='8' || l2=='1')? 1:0; //8, 10, Svrsek, Eso
#else
	b|=(l2>='A')? 4:0; //Jack, Queen, King, Ace
	b|=(l2>='9' && l2!='J' && l2!='Q')? 2:0; //9, X, King, Ace
	b|=((l2&2)==0 && l2!='9')? 1:0; //8, X, Queen, Ace //TODO
#endif
	if(!strcmp(ranks[b],sec)){
		return 1<<5;
	}
	return (b|(a<<2));
}
