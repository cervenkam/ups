#include "commands.h"
#include "../config.h"
#include "lang.h"
#include <cstring>
#include <iostream>
#include <unistd.h>
#include "../stdmcr.h"

Commands::funcptr Commands::ms_commands[COMMANDS] = {
	&Commands::Login,
	&Commands::Disconnect,
	&Commands::SendCard,
	&Commands::CreateGame,
	&Commands::MyCards,
	&Commands::Welcome,
	&Commands::GetCountOfCards,
	&Commands::SendMessage,
	&Commands::Vote
};
//requires first letter distinct
const char* Commands::ms_texts[COMMANDS] = {
	QUERY_LOGIN,
	QUERY_DISCONNECT,
	QUERY_PLAY,
	QUERY_CREATE_GAME,
	QUERY_MY_CARDS,
	QUERY_WELCOME,
	QUERY_COUNT_OF_CARDS,
	QUERY_MESSAGE,
	QUERY_VOTE
};

Commands::Commands(){
	m_sock = -1;
	m_server = nullptr;
	m_player = nullptr;
	m_game = nullptr;
	m_semaphore = new Semaphore(0); //deleted in destructor
}

void Commands::SendMessage(char* message){
	char buff[MAX_LEN];
	strcpy(buff,RESPONSE_MESSAGE);
	char* ptr = buff+strlen(RESPONSE_MESSAGE);
	ptr[0] = ' ';
	ptr++;
	strcpy(ptr,m_player->m_player);
	ptr+=strlen(m_player->m_player);
	ptr[0] = ' ';
	ptr++;
	strcpy(ptr++,message);
	unsigned count = m_game->GetCountOfPlayers();
	for(unsigned a=0; a<count; a++){
		Algorithm* algo = m_game->GetAlgorithm(a);
		NetworkPlayer* np;
		if((np = dynamic_cast<NetworkPlayer*>(algo))){
			Commands* cmd = np->GetCommands();
			if(cmd!=nullptr){
				m_server->Send(cmd->m_sock,buff);
			}
		}
	}
}
Commands::Commands(int sock,Server* server,NetworkPlayer* player,Game* game){
	SetSocket(sock);
	SetServer(server);
	SetPlayer(player);
	player->SetGameForBothMeAndBot(game);
}

void Commands::Start(){
	Welcome(nullptr);
	while(m_running){
		while(m_running && m_is_connected){
			Call(m_server->Receive(this,m_sock));
		}
		if(!m_running){
			Disconnect(nullptr);
			return;
		}
		STDMSG("0;36","Waiting");
		m_semaphore->Wait(30000);	
		if(!m_is_connected){
			Disconnect(nullptr);
			return;
		}
	}
}

void Commands::SetConnected(bool is_connected){
	m_is_connected = is_connected;
}
bool Commands::IsConnected(){
	return m_is_connected;
}
void Commands::SetRunning(bool running){
	m_running = running;
}
bool Commands::IsRunning(){
	return m_running;
}
void Commands::SetSocket(int sock){
	m_sock=sock;
}

int Commands::GetSocket(){
	return m_sock;
}

void Commands::SetServer(Server* server){
	m_server=server;
}

Server* Commands::GetServer(){
	return m_server;
}

void Commands::SetPlayer(NetworkPlayer* player){
	m_player=player;
}

NetworkPlayer* Commands::GetPlayer(){
	return m_player;
}

void Commands::SetGame(Game* game){
	m_game=game;
}

Game* Commands::GetGame(){
	return m_game;
}

void Commands::SetThread(thread* thread){
	m_thread = thread;
}

thread* Commands::GetThread(){
	return m_thread;
}

void Commands::Welcome(char*){
	m_server->Send(m_sock,RESPONSE_WELCOME);
}
void Commands::BadCommand(const char* command){
	cerr << BAD_COMMAND << command << endl;
}
void Commands::Call(char* command){
	if(command == nullptr){
		m_is_connected=false;
		return;
	}
	unsigned max_len = 0;
	unsigned index = 0;
	unsigned cmdlen = strlen(command);
	for(unsigned a=0; a<COMMANDS; a++){
		unsigned len = strlen(ms_texts[a]);
		unsigned min_len = len > cmdlen ? cmdlen : len;
		unsigned b;
		for(b=0; b<min_len; b++){
			if(command[b] != ms_texts[a][b]){
				break;
			}
		}
		if(max_len < b){
			max_len = b;
			index = a;
		}
	}
	if(max_len > 0){
		(this->*ms_commands[index])(command+strlen(ms_texts[index])+1);
	}else{
		BadCommand(command);
	}
	return;
}

void Commands::MyCards(char*){
	char* buff = new char[MAX_LEN]; //deleted at the end of this function
	strcpy(buff,RESPONSE_YOUR_CARDS);
	char* ptr = buff+strlen(RESPONSE_YOUR_CARDS);
	unsigned cardcount = m_player->GetHand()->Size();
	for(unsigned c=0; c<cardcount; c++){
		Card* card = m_player->GetHand()->Get(c);
		char* str = card->ToString();
		ptr[0] = ' ';
		ptr++;
		strcpy(ptr,str);
		ptr+=strlen(str);
	}
	m_server->Send(m_sock,buff);
	delete[] buff; //created an the start of this function
}
void Commands::GetCountOfCards(char*){
	char* buff = new char[MAX_LEN]; //deleted at the end of this function
	strcpy(buff,RESPONSE_COUNT_CARDS);
	char* ptr = buff+strlen(RESPONSE_COUNT_CARDS);
	unsigned playerscount = m_game->GetCountOfPlayers();
	for(unsigned c=0; c<playerscount; c++){
		Algorithm* alg = m_game->GetAlgorithm(c);
		if(alg != m_player){
			ptr[0] = ' ';
			ptr++;
			strcpy(ptr,alg->m_player);
			ptr+=strlen(alg->m_player);
			ptr[0] = ' ';
			ptr++;
			ptr+=sprintf(ptr,"%d",alg->GetHand()->Size());
		}
	}
	m_server->Send(m_sock,buff);
	delete[] buff; //created at the start of this function
}
	
void Commands::Login(char* message){
	char* player_name = message;
	unsigned games_len = m_server->GetCountOfGames();
	for(unsigned a=0; a<games_len; a++){
		Game* game = m_server->GetGame(a);
		unsigned players_len = game->GetCountOfPlayers();
		for(unsigned b=0; b<players_len; b++){
			Algorithm* algo = game->GetAlgorithm(b);
			if(!strcmp(algo->m_player,player_name)){
				if((m_player = dynamic_cast<NetworkPlayer*>(algo))){	
					if(m_player->IsReady()){
						m_server->Send(m_sock,RESPONSE_PLAYER_EXISTS);
						return;
					}
					m_player->SetCommands(this);
					m_game = game;
					m_server->Send(m_sock,RESPONSE_A_GAME);
					m_player->SetReady();
					break;
				}else{
					m_server->Send(m_sock,RESPONSE_NO_GAME);
					return;
				}
			}
		}
	}
	if(m_player == nullptr){
		m_server->Send(m_sock,RESPONSE_NO_GAME);
		return;
	}else{
		bool game_is_ready = true;
		unsigned players_len = m_game->GetCountOfPlayers();
		for(unsigned a=0; a<players_len; a++){
			if(!m_game->GetAlgorithm(a)->IsReady()){
				game_is_ready = false;
				break;
			}
		}
		if(game_is_ready){
			m_game->StartParallel();
		}
	}
}

void Commands::Disconnect(char*){
	m_server->Send(m_sock,RESPONSE_BYE);
	close(m_sock);
	m_is_connected = false;
	m_running = false;
	m_server->TidyUp(this);
}

void Commands::SendCard(char* message){
	m_card_to_play = nullptr;
	//finding the card
	unsigned value = Card::FromString(message);
	if(value>>6){
		return;
	}
	unsigned a = value&3;
	unsigned b = value>>2;
	unsigned cardcount = m_player->GetHand()->Size();
	for(unsigned c=0; c<cardcount; c++){
		Card* card = m_player->GetHand()->Get(c);
		if(card->GetColor()==a && card->GetRank()==b){
			m_card_to_play = card;
		}
	}
	m_player->SetCard(m_card_to_play);
	m_player->GetSemaphore()->Notify();
}

void Commands::Vote(char* message){
	int vote = 0;
	if(!strcmp(message,VOTE_YES)){
		vote = 1;
	}else if(!strcmp(message,VOTE_NO)){
		vote = -1;
	}
	m_player->SetVote(vote);
	m_player->GetSemaphoreForVote()->Notify();
}

void Commands::CreateGame(char* message){
	unsigned length = strlen(message);
	char* game_name = message;
	char* params = message;
	for(unsigned a=1; a<length; a++){
		if(message[a-1]==' '){
			message[a-1]='\0';
			params=message+a;	
			break;
		}
	}
	Configuration* configuration = new Configuration(params); //deleted in game destructor
	Game* game = new Game(configuration); //deleted in Server::StopGame || Server destructor
	game->SetName(game_name);
	m_server->AddGame(game);
	m_server->Send(m_sock,RESPONSE_GAME_CREATED);
}
Semaphore* Commands::GetSemaphore(){
	return m_semaphore;
}
Commands::~Commands(){
	delete m_semaphore; //created in constructor
	delete m_thread; //created in Server::Start
	STDMSG("1;35","Deleted:    Commands");
}
