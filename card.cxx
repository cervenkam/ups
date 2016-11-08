/*
	Class reprezenting playing card
	Author: Martin Cervenka
	Version: 30.03.2016
*/
#include <ostream>
#include <iostream>
#include <cstring>
#include "card.h"
#include "stdmcr.h"
#ifdef LANG_CS
const char* Card::ms_ranks[8] = {"7","8","9","10","SPODEK","SVRSEK","KRAL","ESO"};
const char* Card::ms_colors[4] = {"CERVENY", "ZELENY", "KULE", "ZALUDY"};
#else
const char* Card::ms_ranks[8] = {"7","8","9","X","JACK","QUEEN","KING","ACE"};
const char* Card::ms_colors[4] = {"HEART", "LEAF", "BELL", "ACORN"};
#endif
using namespace std; 
/*
	Card constructor
		=> value Value of the card (0-31)
*/
Card::Card(unsigned char value){
	m_value = value;
	unsigned color = GetColor();
	unsigned rank = GetRank();
	unsigned c_len = strlen(ms_colors[color]);
	unsigned r_len = strlen(ms_ranks[rank]);
	m_name = new char[r_len+c_len+2]; //deleted in destructor
	strcpy(m_name,ms_colors[color]);
	m_name[c_len]=' ';
	strcpy(m_name+c_len+1,ms_ranks[rank]);
}

/*
	Card destructor
*/
Card::~Card(){
	delete[] m_name; //created in constructor;
}
/*
	Returns the rank of this card
		<= Rank of the card
*/
unsigned char Card::GetRank() const{
	return m_value>>2; 
}
/*
	Returns the color of this card
		<= Color of the card
*/
unsigned char Card::GetColor() const{
	return m_value&3;
}
/*
	Sends information of the card to output stream
		=> os Output stream where will be card sent
		=> card Sending card
		<= Output stream sent as os
*/
ostream& operator<<(ostream& cout,const Card& card){
	return COLOR((card.IsSpecial()?7:1) << ";3" << (card.GetColor()+1)," " << Card::ms_ranks[card.GetRank()][0] << " ");
}
/*
	Is the card playable on this card
		=> card THE card
		<= Is playable?
*/
bool Card::IsPlayable(const Card* card) const{
	if(card==nullptr){
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
	return ((m_value&0b1100) == 0b1100);
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
char* Card::ToString() const{
	return m_name;	
}
/*
	Converts string reprezentation to number reprezentation
		=> str String reprezentation
		<= Number reprezentation
*/
unsigned char Card::FromString(const char* const str){
	if(!str || !strcmp(str,"NULL")){
		return 1<<5;
	}
	char* copy_of_str = new char[strlen(str)+1]; //deleted at the end of this function
	strcpy(copy_of_str,str);
	char* sec = copy_of_str;
	unsigned len = strlen(copy_of_str);
	unsigned a;
	for(a=0; a<len; a++){
		if(copy_of_str[a]==' '){
			copy_of_str[a]='\0';
			sec = copy_of_str+a+1;
			break;
		}
	}
	if(a==len){
		return 1<<5;
	}
	unsigned c=ColorFromString(copy_of_str);
	unsigned v=ValueFromString(sec);
	delete[] copy_of_str; //created at the start of this function
	return ((v<<2)|c);
}
unsigned Card::ValueFromString(char* str){
	unsigned b=0;
	char l2 = str[0];
#ifdef LANG_CS
	unsigned len2 = strlen(str);
	char m2 = len2==0?'\0':str[1];
	b|=(l2>='E')? 4:0; //Spodek, Svrsek, Kral, Eso
	b|=(l2!='S' && l2!='7' && l2!='8')? 2:0; //9, 10, Kral, Eso
	b|=(m2>='S' || l2=='8' || l2=='1')? 1:0; //8, 10, Svrsek, Eso
#else
	b|=(l2>='A' && l2!='X')? 4:0; //Jack, Queen, King, Ace
	b|=(l2>='9' && l2!='J' && l2!='Q')? 2:0; //9, X, King, Ace
	b|=((l2&2)==0 && l2!='9')? 1:0; //8, X, Queen, Ace
#endif
	if(strcmp(ms_ranks[b],str)){
		return 1<<6;
	}
	return b;
}
unsigned Card::ColorFromString(char* str){
	unsigned a=0;
	char l1 = str[0];
#ifdef LANG_CS
	unsigned len1 = strlen(str);
	char m1 = len1==0?'\0':str[1];
	a|=(m1!='E')? 2:0; //Kule, Zaludy
	a|=(l1=='Z')? 1:0; //Zeleny, Zaludy
#else
	a|=(l1<='B')? 2:0; //Bell, Acorn
	a|=(l1=='A' || l1=='L')? 1:0; //Leaf, Acorn
#endif
	if(strcmp(ms_colors[a],str)){
		return 1<<6;
	}
	return a;
}
