#ifndef ALGORITHM_H
#define ALGORITHM_H

#include "card.h"
#include "hand.h"
#include "lang.h"
#include "game.h"
#include <vector>
#include "semaphore.h"
using namespace std;
class Game;
class Algorithm{
	public:
		Algorithm(const char* player,unsigned char ch);
		Algorithm(const Algorithm& algorithm);
		void SetGame(Game* game);
		void AddPoints(unsigned char points);
		void ClearPoints();
		void SetReady();
		void SetReady(bool);
		virtual ~Algorithm();
		const char* m_player;
		void Identify();
		//GETTERS
		bool IsReady();//GETTER
		Semaphore* GetSemaphoreForVote();//GETTER
		Semaphore* GetSemaphore();//GETTER
		unsigned char GetPoints();//GETTER
		bool Send(Card* card);	//GETTER
		Hand* GetHand();//GETTER
		virtual Card* Play(bool force) = 0;//GETTER
		virtual char Vote() = 0;//GETTER
	protected:
		virtual void Used(Card* card,unsigned char player) = 0;
		unsigned char m_myID;
		const char* m_name = "algorithm";
		Game* m_game = nullptr;
	private:
		Hand* m_hand;
		unsigned char m_points = 0;
		bool m_ready = false;
		Semaphore* m_semaphore;
		Semaphore* m_semaphore_voting;
		bool m_copy = false;
};

#endif
