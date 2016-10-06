#include "commands.h"
#include "../config.h"
#include <cstring>
#include <iostream>
#include <unistd.h>

Commands::funcptr Commands::commands[COMMANDS] = {
	&Commands::Login,
	&Commands::Disconnect,
	&Commands::SendCard,
	&Commands::CreateGame,
	&Commands::MyCards,
	&Commands::Ping
};
//requires first letter distinct
#ifdef LANG_CS
const char* Commands::texts[COMMANDS] = {
	"PRIHLASIT SE",
	"ODPOJIT",
	"ZAHRAT",
	"VYTVORIT HRU",
	"MOJE KARTY",
	"PING"
};
const char* Commands::ranks[8] = {"7","8","9","10","SPODEK","SVRSEK","KRAL","ESO"};
const char* Commands::colors[4] = {"CERVENY", "ZELENY", "KULE", "ZALUDY"};
#else
const char* Commands::texts[COMMANDS] = {
	"LOGIN",
	"DISCONNECT",
	"GIVE",
	"CREATE GAME",
	"MY CARDS",
	"PING"
};
const char* Commands::ranks[8] = {"7","8","9","X","JACK","QUEEN","KING","ACE"};
const char* Commands::colors[4] = {"HEART", "LEAF", "BELL", "ACORN"};
#endif

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
	while(1){
		cout << "SOCKET: " << this->sock << ", SERVER: " << this->server << endl;
		Call(this->server->Receive(this->sock));
	}
}

void Commands::SetSocket(int sock){
	this->sock=sock;
}

void Commands::SetServer(Server* server){
	this->server=server;
}

void Commands::SetPlayer(NetworkPlayer* player){
	this->player=player;
}

void Commands::SetGame(Game* game){
	this->game=game;
}

void Commands::Ping(char*){
	this->server->Send(this->sock,"PONG");
}

void Commands::BadCommand(const char* command){
#ifdef LANG_CS
	cerr << "Spatny prikaz: " << command;
#else
	cerr << "Bad command: " << command;
#endif
}
void Commands::Call(char* command){
	cout << "CALL: " << command << endl;
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
	char buff[MAX_LEN];
#ifdef LANG_CS
	strcpy(buff,"TVOJE KARTY JSOU");
	char* ptr = message+16;
#else
	strcpy(buff,"YOUR CARDS ARE");
	char* ptr = message+14;
#endif
	unsigned cardcount = player->GetHand()->Size();
	for(unsigned c=0; c<cardcount; c++){
		Card* card = player->GetHand()->Get(c);
		const char* clr = colors[card->GetColor()];
		unsigned len_clr = strlen(clr);
		ptr[0] = ' ';
		strcpy(ptr++,clr);
		ptr+=len_clr;
		const char* rnk = ranks[card->GetRank()];
		unsigned len_rnk = strlen(rnk);
		ptr[0] = ' ';
		strcpy(ptr++,rnk);
		ptr+=len_rnk;
	}
}
	
void Commands::Login(char*){
	
}

void Commands::Disconnect(char*){
	close(this->sock);
}

void Commands::SendCard(char* message){
	this->card_to_play = NULL;
	char* message2 = message;
	unsigned len = strlen(message);
	for(unsigned a=0; a<len; a++){
		if(message[a]==' '){
			message[a]='\0';
			message2 = message+a+1;
		}
	}
	unsigned a=0;
	char l1 = message[0];
	char l2 = message2[0];
#ifdef LANG_CS
	unsigned len1 = strlen(message);
	char m1 = len1=0?'\0':message[1];
	a|=(m1!='E')? 2:0; //Kule, Zaludy
	a|=(l1=='Z')? 1:0; //Zeleny, Zaludy
#else
	a|=(l1<='B')? 2:0; //Bell, Acorn
	a|=(l1=='A' || l1=='L')? 1:0; //Leaf, Acorn
#endif
	if(!strcmp(colors[a],message)){
		BadCommand("Bad card color");
		return;
	}
	unsigned b=0;
#ifdef LANG_CS
	unsigned len2 = strlen(message2);
	char m2 = len2=0?'\0':message2[1];
	b|=(l2>='E')? 4:0; //Spodek, Svrsek, Kral, Eso
	b|=(l2!='S' && l2!='7' && l2!='8')? 2:0; //9, 10, Kral, Eso
	b|=(m2>='S' || l2=='8' || l2=='1')? 1:0; //8, 10, Svrsek, Eso
#else
	b|=(l2>='A')? 4:0; //Jack, Queen, King, Ace
	b|=(l2>='9' && l2!='J' && l2!='Q')? 2:0; //9, X, King, Ace
	b|=((l2&2)==0 && l2!='9')? 1:0; //8, X, Queen, Ace //TODO
#endif
	if(!strcmp(ranks[b],message)){
		BadCommand("Bad card rank");
	}
	//finding the card
	unsigned cardcount = player->GetHand()->Size();
	for(unsigned c=0; c<cardcount; c++){
		Card* card = player->GetHand()->Get(c);
		if(card->GetColor()==a && card->GetRank()==b){
			card_to_play = card;
			return;
		}
	}
}

void Commands::CreateGame(char* message){
	Configuration* conf = Configuration::GetConfiguration(message);
	Game* game = new Game(conf->GetCount(),conf->GetAlgorithms());
	server->AddGame(game);
	game->Start();
}
