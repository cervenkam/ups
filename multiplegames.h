#ifndef _MULTIPLE_GAMES_H_
#define _MULTIPLE_GAMES_H_

#include <thread>
#include <vector>
using namespace std;
class MultipleGames{
	public:
		MultipleGames();
		~MultipleGames();
		void CreateGame(int argc,char** argv);
		unsigned GetNumberOfGames();
		thread* GetGame(unsigned);
	private:
		vector<thread*> games;
};

#endif
