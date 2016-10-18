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
	m_semaphore = new Semaphore(0);
}

/*
	Destroys this player
*/
NetworkPlayer::~NetworkPlayer(){
	delete m_semaphore;
}

/*
	Is called when a card is used (with any player)
		=> card Player card
		=> player ID of the player which played card
*/
void NetworkPlayer::Used(Card* card,unsigned char player){
	if(m_commands && m_commands->GetServer()){
		char* buff = new char[MAX_LEN];
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
		delete[] buff;
	}
}
/*
	Defines the card which this player will use
		=> force Force the player to play? (he is not lay down the first card)
		<= Card which will player use
*/
Card* NetworkPlayer::Play(bool){
	m_commands->GetServer()->Send(m_commands->GetSocket(),RESPONSE_PLAY);
	m_semaphore->Wait();
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
	Gets semapohore for this object
*/
Semaphore* NetworkPlayer::GetSemaphore(){
	return m_semaphore;
}

/*
	Sets the card which should player use
*/
void NetworkPlayer::SetCard(Card* card){
	m_card = card;
}
