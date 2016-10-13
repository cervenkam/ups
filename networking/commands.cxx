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
	&Commands::Welcome
};
//requires first letter distinct
const char* Commands::texts[COMMANDS] = {
	LOGIN,
	DISCONNECT,
	PLAY,
	CREATE_GAME,
	MY_CARDS,
	WELCOME
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
	this->server->Send(this->sock,WELCOME);
}

void Commands::BadCommand(const char* command){
	cerr << BAD_COMMAND << command;
}
void Commands::Call(char* command){
	unsigned a;
	for(a=0; a<COMMANDS; a++){
		if(command[0] == texts[a][0]){
			break;
		}
	}
	if(a==COMMANDS){
		BadCommand(command);
		return;
	}
	unsigned len = strlen(texts[a]);
	for(unsigned b=0; b<len; b++){
		if(texts[a][b] != command[b]){
			BadCommand(command);
			return;
		}
	}
	(this->*commands[a])(command+len+1);
}

void Commands::MyCards(char* message){
	char* buff = new char[MAX_LEN];
	strcpy(buff,YOUR_CARDS);
	char* ptr = buff+strlen(YOUR_CARDS);
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
					this->player->SetCommands(this);
					this->game = game;
					this->server->Send(this->sock,A_GAME);
					this->player->SetReady();
					break;
				}else{
					this->server->Send(this->sock,NO_GAME);
					return;
				}
			}
		}
	}
	if(this->player == NULL){
		this->server->Send(this->sock,NO_GAME);
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
	this->server->Send(this->sock,GAME_CREATED);
}
