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
	m_name="networkplayer";
	m_commands = nullptr;
	m_card = nullptr;
	m_bot = new ProgrammerBot(*this); //deleted in destuctor
}

/*
	Sets the game for me and my bot
		=> Game in which this algorithm acts
*/
void NetworkPlayer::SetGameForBothMeAndBot(Game* game){
	m_bot->SetGame(game);
	SetGame(game);
}
/*
	Destroys this player
*/
NetworkPlayer::~NetworkPlayer(){
	delete m_bot; //created in constructor
}

/*
	Is called when a card is used (with any player)
		=> card Player card
		=> player ID of the player which played card
*/
void NetworkPlayer::Used(Card* card,unsigned char player){
	m_bot->Used(card,player);
	if(m_commands && m_commands->GetServer()){
		char* buff = new char[MAX_LEN]; //deleted at the end of this function
		char* tmp = buff;
		strcpy(buff,RESPONSE_USED_CARD);
		tmp+=strlen(RESPONSE_USED_CARD);
		tmp[0]=' ';
		tmp++;
		const char* text = "NULL";
		if(card != nullptr){
			text = card->ToString();
		}
		strcpy(tmp,text);
		tmp+=strlen(text);
		sprintf(tmp++," %s",m_commands->GetGame()->GetAlgorithm(player)->m_player); //automaticly appends '\0'
		m_commands->GetServer()->Send(m_commands->GetSocket(),buff);
		delete[] buff; //created at the start of this function
	}
}
/*
	Defines the card which this player will use
		=> force Force the player to play? (he is not lay down the first card)
		<= Card which will player use
*/
Card* NetworkPlayer::Play(bool force){
	if(m_commands == nullptr){
		return m_bot->Play(force);
	}
	m_commands->GetServer()->Send(m_commands->GetSocket(),RESPONSE_PLAY);
	GetSemaphore()->Wait();
	return m_card;
}

/*
	Gets the commands from this player
		<= commands Commands from player
*/
Commands* NetworkPlayer::GetCommands(){
	return m_commands;
}

/*
	Sets the commands for this player
		=> commands Commands for player
*/
void NetworkPlayer::SetCommands(Commands* commands){
	m_commands = commands;
}

/*
	Sets the card which should player use
*/
void NetworkPlayer::SetCard(Card* card){
	m_card = card;
}
