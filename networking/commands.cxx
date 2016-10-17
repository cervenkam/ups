#include "commands.h"
#include "../config.h"
#include "lang.h"
#include <cstring>
#include <iostream>
#include <unistd.h>

Commands::funcptr Commands::commands[COMMANDS] = {
	&Commands::Login,
	&Commands::Disconnect,
	&Commands::SendCard,
	&Commands::CreateGame,
	&Commands::MyCards,
	&Commands::Welcome,
	&Commands::GetCountOfCards
};
//requires first letter distinct
const char* Commands::texts[COMMANDS] = {
	QUERY_LOGIN,
	QUERY_DISCONNECT,
	QUERY_PLAY,
	QUERY_CREATE_GAME,
	QUERY_MY_CARDS,
	QUERY_WELCOME,
	QUERY_COUNT_OF_CARDS,
};

Commands::Commands(){
	this->sock = -1;
	this->server = NULL;
	this->player = NULL;
	this->game = NULL;
}

Commands::Commands(int sock,Server* server,NetworkPlayer* player,Game* game){
	SetSocket(sock);
	SetServer(server);
	SetPlayer(player);
	SetGame(game);
}

void Commands::Start(){
	Welcome(NULL);
	while(1){
		Call(this->server->Receive(this->sock));
	}
}

void Commands::SetSocket(int sock){
	this->sock=sock;
}

int Commands::GetSocket(){
	return this->sock;
}

void Commands::SetServer(Server* server){
	this->server=server;
}

Server* Commands::GetServer(){
	return this->server;
}

void Commands::SetPlayer(NetworkPlayer* player){
	this->player=player;
}

NetworkPlayer* Commands::GetPlayer(){
	return this->player;
}

void Commands::SetGame(Game* game){
	this->game=game;
}

Game* Commands::GetGame(){
	return this->game;
}
void Commands::Welcome(char*){
	this->server->Send(this->sock,RESPONSE_WELCOME);
}

void Commands::BadCommand(const char* command){
	cerr << BAD_COMMAND << command << endl;
}
void Commands::Call(char* command){
	unsigned max_len = 0;
	unsigned index = 0;
	unsigned cmdlen = strlen(command);
	for(unsigned a=0; a<COMMANDS; a++){
		unsigned len = strlen(texts[a]);
		unsigned min_len = len > cmdlen ? cmdlen : len;
		unsigned b;
		for(b=0; b<min_len; b++){
			if(command[b] != texts[a][b]){
				break;
			}
		}
		if(max_len < b){
			max_len = b;
			index = a;
		}
	}
	if(max_len > 0){
		cout << (command+strlen(texts[index])+1) << endl;
		(this->*commands[index])(command+strlen(texts[index])+1);
	}else{
		BadCommand(command);
	}
	return;
}

void Commands::MyCards(char*){
	char* buff = new char[MAX_LEN];
	strcpy(buff,RESPONSE_YOUR_CARDS);
	char* ptr = buff+strlen(RESPONSE_YOUR_CARDS);
	unsigned cardcount = player->GetHand()->Size();
	cout << "Count: " << cardcount << endl;
	for(unsigned c=0; c<cardcount; c++){
		Card* card = player->GetHand()->Get(c);
		char* str = card->ToString();
		cout << str << endl;
		ptr[0] = ' ';
		ptr++;
		strcpy(ptr,str);
		ptr+=strlen(str);
	}
	this->server->Send(this->sock,buff);
	delete[] buff;
}
void Commands::GetCountOfCards(char*){
	char* buff = new char[MAX_LEN];
	strcpy(buff,RESPONSE_COUNT_CARDS);
	char* ptr = buff+strlen(RESPONSE_COUNT_CARDS);
	unsigned playerscount = this->game->GetCountOfPlayers();
	for(unsigned c=0; c<playerscount; c++){
		Algorithm* alg = this->game->GetAlgorithm(c);
		if(alg != this->player){
			ptr[0] = ' ';
			ptr++;
			strcpy(ptr,alg->player);
			ptr+=strlen(alg->player);
			ptr[0] = ' ';
			ptr++;
			ptr+=sprintf(ptr,"%d",alg->GetHand()->Size());
		}
	}
	this->server->Send(this->sock,buff);
	delete[] buff;
}
	
void Commands::Login(char* message){
	char* player_name = message;
	unsigned games_len = server->GetCountOfGames();
	for(unsigned a=0; a<games_len; a++){
		Game* game = server->GetGame(a);
		unsigned players_len = game->GetCountOfPlayers();
		for(unsigned b=0; b<players_len; b++){
			Algorithm* algo = game->GetAlgorithm(b);
			if(!strcmp(algo->player,player_name)){
				if((this->player = dynamic_cast<NetworkPlayer*>(algo))){	
					if(this->player->IsReady()){
						this->server->Send(this->sock,RESPONSE_PLAYER_EXISTS);
						return;
					}
					this->player->SetCommands(this);
					this->game = game;
					this->server->Send(this->sock,RESPONSE_A_GAME);
					this->player->SetReady();
					break;
				}else{
					this->server->Send(this->sock,RESPONSE_NO_GAME);
					return;
				}
			}
		}
	}
	if(this->player == NULL){
		this->server->Send(this->sock,RESPONSE_NO_GAME);
		return;
	}else{
		bool game_is_ready = true;
		unsigned players_len = this->game->GetCountOfPlayers();
		for(unsigned a=0; a<players_len; a++){
			if(!game->GetAlgorithm(a)->IsReady()){
				game_is_ready = false;
				break;
			}
		}
		if(game_is_ready){
			game->StartParallel();
		}
	}
}

void Commands::Disconnect(char*){
	close(this->sock);
}

void Commands::SendCard(char* message){
	this->card_to_play = NULL;
	//finding the card
	unsigned value = Card::FromString(message);
	cout << message << " -> " << value << endl;
	if(value>>5){
		return;
	}
	unsigned a = value&3;
	unsigned b = value>>2;
	unsigned cardcount = player->GetHand()->Size();
	for(unsigned c=0; c<cardcount; c++){
		Card* card = player->GetHand()->Get(c);
		if(card->GetColor()==a && card->GetRank()==b){
			card_to_play = card;
			return;
		}
	}
	player->SetCard(card_to_play);
	cout << "NOTIFY" << endl;
	player->GetSemaphore()->Notify();
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
	Configuration* conf = Configuration::GetConfiguration(params);
	Game* game = new Game(conf->GetCount(),conf->GetAlgorithms());
	game->SetName(game_name);
	server->AddGame(game);
	this->server->Send(this->sock,RESPONSE_GAME_CREATED);
}
