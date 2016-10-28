#ifndef GAME_H
#define GAME_H

#include "deck.h"
#include "algorithm.h"
#include "config.h"
#include <thread>
class Algorithm;
class Configuration;
class Game{
	public:
		Game(Configuration* conf);
		~Game();
		void Start();
		void End();
		void Print();
		void SetName(char* name);
		void SetStarted(unsigned char ID);
		void StartParallel();
		void StopParallel();
		void SetFirstCard(Card* card);
		//GETTERS
		Card* FirstCard();//GETTER
		unsigned char GetStarted();//GETTER
		char* GetName();//GETTER
		bool IsHeStarted(unsigned char ID);//GETTER
		Algorithm* GetAlgorithm(unsigned player);//GETTER
		unsigned GetCountOfPlayers();//GETTER
		bool VotePasses();//GETTER
	private:
		void Loop();
		void DetermineWinner(Card* card,unsigned& player, unsigned& winner);
		void PrintWinners(unsigned max_value);
		unsigned FindMaxPoints(unsigned& count);
		void FoundSameCards(unsigned);
		bool UseValidCard(Card*,unsigned&,unsigned&);
		bool UseNoCard(unsigned&,unsigned&);
		Card* m_first = nullptr;
		unsigned char m_players;
		unsigned char m_started = 0;
		Deck* m_deck;
		Algorithm** m_algos;
		unsigned char m_points = 0;
		char* m_name;
		Configuration* m_conf;
		thread* m_thr = nullptr;
		bool m_end_of_game = false;
		//GETTERS
		bool Prepare();//GETTER
		bool ChooseCard(unsigned&,unsigned&,Card*&, bool&);//GETTER
		bool ReactOnCard(unsigned&,unsigned&,Card*&, bool&);//GETTER
		bool FillHands();//GETTER
		bool OneHand(unsigned& winner, unsigned& player);//GETTER
		bool UseCard(Card* card,unsigned& player, unsigned& winner);//GETTER
		bool SameCards();//GETTER
};

#endif
