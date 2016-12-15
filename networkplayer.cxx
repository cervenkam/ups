/*
	Network player algorithm
	Author: Martin Cervenka
	Version: 06.10.2016
*/
#include "networkplayer.h"
#include "networking/lang.h"
#include "stdmcr.h"
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
	m_vote = 0;
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
	Sets the vote for this player
		=> vote
*/
void NetworkPlayer::SetVote(char vote){
	m_vote = vote;
}
/*
	Calls when it is end of game
		=> max_points Maximal points, which winner has
		=> count Count of winners
*/
void NetworkPlayer::EndOfGame(unsigned max_points,unsigned count){
	const char* result;
	char* tmp;
	char* buff = new char[MAX_LEN];
	tmp = Add(buff,RESPONSE_RESULT);
	if(GetPoints()==max_points){
		if(count==1){
			result = RESULT_WIN;
		}else{
			result = RESULT_DRAW;
		}
	}else{
		result = RESULT_LOSE;
	}
	tmp = Append(tmp,result);
	GetCommands()->TrySend(GetCommands()->GetSocket(),buff);
	delete[] buff;
}
void NetworkPlayer::NewHand() const{
	if(m_commands){
		m_commands->MyCards(nullptr);
		m_commands->GetCountOfCards(nullptr);
		m_commands->Table(nullptr);
	}
}
/*
	Is called when a card is used (with any player)
		=> card Player card
		=> player ID of the player which played card
*/
void NetworkPlayer::Used(const Card* card,unsigned char player){
	m_bot->Used(card,player);
	if(m_commands && m_commands->IsConnected()){
		char* buff = new char[MAX_LEN]; //deleted at the end of this function
		const char* text = "NULL";
		if(card != nullptr){
			text = card->ToString();
		}
		Append(Append(Add(buff,RESPONSE_USED_CARD),text),m_commands->GetAlgorithmName(player));
		m_commands->TrySend(m_commands->GetSocket(),buff);
		delete[] buff; //created at the start of this function
	}
}
/*
	Defines the card which this player will use
		=> force Force the player to play? (he is not lay down the first card)
		<= Card which will player use
*/
const Card* NetworkPlayer::Play(bool force) const{
	if(m_commands == nullptr){
		return m_bot->Play(force);
	}
	if(m_commands->IsConnected()){
		m_commands->TrySend(m_commands->GetSocket(),RESPONSE_PLAY);
	}
	if(Wait(TIMEOUT)){
		return m_bot->Play(force);
	}
	return m_card;
}
void NetworkPlayer::TrySend(char* buff){
	GetCommands()->TrySend(GetCommands(),buff);
}
/*
	Selects if this player wants to start new game
		<= 1 want to, -1 dont want to, 0 don't care
*/
char NetworkPlayer::Vote() const{
	if(m_commands == nullptr){
		return m_bot->Vote();
	}
	m_commands->TrySend(m_commands->GetSocket(),RESPONSE_VOTE);
	if(VoteWait(TIMEOUT_VOTE)){
		return -1;
	}
	return m_vote;
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
void NetworkPlayer::SetCard(const Card* card){
	m_card = card;
}
void NetworkPlayer::SetNextCard(unsigned color,unsigned rank){
	const Card* next_card = nullptr;
	unsigned cardcount = GetCardCount();
	for(unsigned c=0; c<cardcount; c++){
		const Card* card = GetCard(c);
		if(card->GetColor()==color && card->GetRank()==rank){
			next_card = card;
			break;
		}
	}
	SetCard(next_card);
}
