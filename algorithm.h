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
		bool IsReady();
		unsigned char GetPoints();
		bool Send(const Card* card);	
		void VoteNotify() const;
		void Notify() const;
		bool VoteWait(unsigned) const;
		bool Wait(unsigned) const;
		bool AddCard(const Card*);
		void ClearCards();
		const Card* UseCard(unsigned); //NON VALID GETTER
		unsigned GetCardCount() const;
		virtual const Card* Play(bool force) const = 0;//NON VALID GETTER
		virtual char Vote() const = 0;
		void FillCardsString(char* buff);
		const Card* GetCard(unsigned) const; //NON VALID GETTER
	protected:
		virtual void Used(const Card* card,unsigned char player) = 0;
		unsigned char m_myID;
		const char* m_name = "algorithm";
		Game* m_game = nullptr;
	private:
		Hand* GetHand() const;
		Semaphore* GetSemaphoreForVote();
		Semaphore* GetSemaphore();
		Hand* m_hand;
		unsigned char m_points = 0;
		bool m_ready = false;
		Semaphore* m_semaphore;
		Semaphore* m_semaphore_voting;
		bool m_copy = false;
};

#endif
