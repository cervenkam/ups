#ifndef CARD_H
#define CARD_H

#define CARD_7 0
#define CARD_8 1
#define CARD_9 2
#define CARD_X 3
#define CARD_s 4
#define CARD_S 5
#define CARD_K 6
#define CARD_E 7 
#define CARD_RED 0
#define CARD_GREEN 8
#define CARD_YELLOW 16
#define CARD_BLUE 24

#include <ostream>
using namespace std;
class Card{
	public:
		Card(unsigned char value);
		~Card();
		//GETTERS
		unsigned char GetColor() const;//GETTER
		unsigned char GetRank() const;//GETTER
		friend ostream& operator<<(ostream& os,const Card& card);//GETTER
		bool IsPlayable(Card* card) const;//GETTER
		bool IsSpecial() const;//GETTER
		bool IsValuable() const;//GETTER
		char* ToString();//GETTER
		static unsigned char FromString(char* str);//GETTER
	private:
		unsigned char m_value;
		char* m_name;
		static const char* ms_ranks[8]; //OK
		static const char* ms_colors[4]; //OK
};

#endif
