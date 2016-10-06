#include "multiplegames.h"
#include "startgame.h"

MultipleGames::MultipleGames(){

}

MultipleGames::~MultipleGames(){
	unsigned size = GetNumberOfGames();
	for(unsigned a=0; a<size; a++){
		delete games[a];
	}
}

void MultipleGames::CreateGame(int argc,char** argv){
	thread* t = new thread(start_game,argc,argv);
	games.push_back(t);
}

unsigned MultipleGames::GetNumberOfGames(){
	return games.size();
}

thread* MultipleGames::GetGame(unsigned game){
	return games[game];
}
