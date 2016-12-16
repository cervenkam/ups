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
		void SetFirstCard(const Card* card);
		const Card* FirstCard();//NON VALID GETTER
		unsigned char GetStarted();
		char* GetName();
		bool IsHeStarted(unsigned char ID);
		Algorithm* GetAlgorithm(unsigned player);//NON VALID GETTER
		unsigned GetCountOfPlayers();
		bool VotePasses();
		const char* GetAlgorithmName(unsigned);
		const Deck* GetCardsOnTable();
	private:
		void Loop();
		void DetermineWinner(const Card* card,unsigned& player, unsigned& winner);
		void PrintWinners(unsigned max_value);
		unsigned FindMaxPoints(unsigned& count);
		void FoundSameCards(unsigned);
		bool UseValidCard(const Card*,unsigned&,unsigned&);
		bool UseNoCard(unsigned&,unsigned&);
		const Card* m_first = nullptr;
		unsigned char m_players;
		unsigned char m_started = 0;
		Deck* m_deck;
		Deck* m_on_table;
		Algorithm** m_algos;
		unsigned char m_points = 0;
		char* m_name;
		Configuration* m_conf;
		std::thread* m_thr = nullptr;
		bool m_end_of_game = false;
		bool Prepare();
		bool ChooseCard(unsigned&,unsigned&,const Card*&, bool&);
		bool ReactOnCard(unsigned&,unsigned&,const Card*&, bool&);
		bool FillHands();
		bool OneHand(unsigned& winner, unsigned& player);
		bool UseCard(const Card* card,unsigned& player, unsigned& winner);
		bool SameCards();
};

#endif
