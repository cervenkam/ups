/*
	Network player algorithm
	Author: Martin Cervenka
	Version: 06.10.2016
*/
#include "networkplayer.h"
#include "networking/lang.h"
#include "card.h"
#include "hand.h"
#include <iostream>
#include <cstring>

/*
	Network player constructor
		=> ch ID of this player
		=> player Name of the player
*/
NetworkPlayer::NetworkPlayer(const char* player,unsigned char ch): Algorithm(player,ch){
	name="networkplayer";
	commands = NULL;
	semaphore = new Semaphore(0);
}

/*
	Destroys this player
*/
NetworkPlayer::~NetworkPlayer(){
	delete semaphore;
}

/*
	Is called when a card is used (with any player)
		=> card Player card
		=> player ID of the player which played card
*/
void NetworkPlayer::Used(Card* card,unsigned char player){
	if(commands && commands->GetServer()){
		char* buff = new char[MAX_LEN];
		char* tmp = buff;
		strcpy(buff,USED_CARD);
		tmp+=strlen(USED_CARD);
		tmp[0]=' ';
		tmp++;
		const char* text = "NULL";
		if(card != NULL){
			text = card->ToString();
		}
		strcpy(tmp,text);
		tmp+=strlen(text);
		sprintf(tmp++," %u",player); //automaticly appends '\0'
		cout << "#" << buff << "#" << endl;
		commands->GetServer()->Send(commands->GetSocket(),buff);
		delete[] buff;
	}
}
/*
	Defines the card which this player will use
		=> force Force the player to play? (he is not lay down the first card)
		<= Card which will player use
*/
Card* NetworkPlayer::Play(bool force){
	commands->GetServer()->Send(commands->GetSocket(),PLAY);
	cout << "Waiting for a card" << endl;
	semaphore->Wait();
	cout << "I've got a card!" << endl;
	return NULL;
}

/*
	Sets the commands for this player
		=> commands Commands for player
*/
void NetworkPlayer::SetCommands(Commands* commands){
	this->commands = commands;
}

/*
	Gets semapohore for this object
*/
Semaphore* NetworkPlayer::GetSemaphore(){
	return semaphore;
}

/*
	Sets the card which should player use
*/
void NetworkPlayer::SetCard(Card* card){
	this->card = card;
}
