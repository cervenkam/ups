#ifndef CARD_H
#define CARD_H

/*
	Class reprezenting playing card
	Author: Martin Cervenka
	Version: 30.03.2016
*/

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
		/*
			Card constructor
				=> value Value of the card (0-31)
		*/
		Card(unsigned char value);
		/*
			Card destructor
		*/
		~Card();
		/*
			Returns the color of this card
				<= Color of the card
		*/
		unsigned char GetColor() const;
		/*
			Returns the rank of this card
				<= Rank of the card
		*/
		unsigned char GetRank() const;
		/*
			Is the card playable on this card
				=> card THE card
				<= Is playable?
		*/
		bool IsPlayable(Card* card) const;
		/*
			Sends information of the card to output stream
				=> os Output stream where will be card sent
				=> card Sending card
				<= Output stream sent as os
		*/
		friend ostream& operator<<(ostream& os,const Card& card);
		/*
			Is this card valuable
				<= Is valuable
		*/
		bool IsValuable() const;
		/*
			Is this card special
				<= Is special
		*/
		bool IsSpecial() const;
		/*
			Converts number reprezentation to string
				<= String reprezentation
		*/
		char* ToString();
		/*
			Converts string reprezentation to number reprezentation
				=> str String reprezentation
				<= Number reprezentation
		*/
		static unsigned char FromString(char* str);
#ifndef TESTING
	private:
#endif
		/* Value of this card (0-31)*/
		unsigned char value;
		/* Card name */
		char* name;
		/* Cards rank names */
		static const char* ranks[8];
		/* Card color names */
		static const char* colors[4];
};

#endif
