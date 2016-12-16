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
class Card{
	public:
		Card(unsigned char value);
		~Card();
		unsigned char GetColor() const;
		unsigned char GetRank() const;
		friend std::ostream& operator<<(std::ostream& os,const Card& card);//NON VALID GETTER
		bool IsPlayable(const Card* card) const;
		bool IsSpecial() const;
		bool IsValuable() const;
		char* ToString() const;
		static unsigned char FromString(const char* const str);
	private:
		static unsigned ColorFromString(char* str);
		static unsigned ValueFromString(char* str);
		unsigned char m_value;
		char* m_name;
		static const char* ms_ranks[8]; //OK
		static const char* ms_colors[4]; //OK
};

#endif
