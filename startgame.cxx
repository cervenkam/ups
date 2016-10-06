/*
	Main module of program
	Author: Martin Cervenka
	Version: 30.03.2016
*/
//#define TESTING //TODO
#include "startgame.h"
#include "game.h"
#include "stdmcr.h"
#include "config.h"
#include "lang.h"
#include <iostream>
#include <algorithm>
using namespace std;
/*

*/
string* concat_params(int argc,char** argv){
	string* param = new string();
	for(int a=1; a<argc; a++){
		*param += argv[a];
		*param += ' ';
	}
	param->pop_back();
	replace(param->begin(),param->end(),'-','\n');
	return param;
}
/*
	Main function, starts the game
*/
int start_game(int argc,char** argv){
	char end;
	string* params = concat_params(argc,argv);
	cout << "CALLED" << endl;
	Configuration* conf = Configuration::GetConfiguration(*params);
	delete params;
	do{
		CLEAR;
		MOVE("0;0",endl);
		Game game(conf->GetCount(),conf->GetAlgorithms());
		/*Hand* hand = game.algos[0]->hand; //TODO
		for(unsigned a=0; a<4; a++){ //TODO
			hand->Set(a,new Card(a)); //TODO
		}//TODO*/
		game.Start();
		OUT(CONTINUE);
		cin >> end;
	}while(end!=NO[0]);
	return 0;
}
