#include "networkplayer.h"
#include <cstring>
#include <iostream>

NetworkPlayer::funcptr NetworkPlayer::commands[COMMANDS] = {
	&NetworkPlayer::Login,
	&NetworkPlayer::Disconnect,
	&NetworkPlayer::SendCard,
	&NetworkPlayer::CreateGame,
	&NetworkPlayer::MyCards
};
//requires first letter distinct
#ifdef LANG_CS
const char* NetworkPlayer::texts[COMMANDS] = {
	"PRIHLASIT SE",
	"ODPOJIT",
	"ZAHRAT",
	"VYTVORIT HRU",
	"MOJE KARTY"
};
const char* NetworkPlayer::ranks[8] = {"7","8","9","10","SPODEK","SVRSEK","KRAL","ESO"};
const char* NetworkPlayer::colors[4] = {"CERVENY", "ZELENY", "KULE", "ZALUDY"};
#else
const char* NetworkPlayer::texts[COMMANDS] = {
	"LOGIN",
	"DISCONNECT",
	"GIVE",
	"CREATE GAME"
	"MY CARDS"
};
const char* NetworkPlayer::ranks[8] = {"7","8","9","X","JACK","QUEEN","KING","ACE"};
const char* NetworkPlayer::colors[4] = {"HEART", "LEAF", "BELL", "ACORN"};
#endif

NetworkPlayer::NetworkPlayer(const char* player,unsigned char ch): Algorithm(player,ch){
	server=NULL;
	sock=-1;
	name="network_player";
}

void NetworkPlayer::SetSocket(int sock){
	this->sock=sock;
}

void NetworkPlayer::SetServer(Server* server){
	this->server=server;
}

void NetworkPlayer::BadCommand(char* command){
#ifdef LANG_CS
	cerr << "Spatny prikaz: " << command;
#else
	cerr << "Bad command: " << command;
#endif
}
void NetworkPlayer::Call(char* command){
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

void NetworkPlayer::MyCards(char* message){
	char buff[MAX_LEN];
#ifdef LANG_CS
	strcpy(buff,"TVOJE KARTY JSOU");
	char* ptr = message+16;
#else
	strcpy(buff,"YOUR CARDS ARE");
	char* ptr = message+14;
#endif
	unsigned cardcount = GetHand()->Size();
	for(unsigned c=0; c<cardcount; c++){
		Card* card = GetHand()->Get(c);
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
	
void NetworkPlayer::Login(char* message){
	
}

void NetworkPlayer::Disconnect(char* message){
}

void NetworkPlayer::SendCard(char* message){
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
	unsigned cardcount = GetHand()->Size();
	for(unsigned c=0; c<cardcount; c++){
		Card* card = GetHand()->Get(c);
		if(card->GetColor()==a && card->GetRank()==b){
			card_to_play = card;
			return;
		}
	}
}

void NetworkPlayer::CreateGame(char* message){
	start_game(1,message);	
}

void NetworkPlayer::Used(Card*,unsigned char){
	
}
Card* NetworkPlayer::Play(bool force){
#ifdef LANG_CS
	char* message = "IT IS YOUR TURN";
#else
	char* message = "JSI NA TAHU";
#endif
	if(server==NULL){
		cerr << "SERVER IS NOT INITIALIZED, Call SetServer() to fix it" << endl;
		return NULL;
	}
	if(sock<0){
		cerr << "SOCKET IS NOT INITIALIZED, Call SetSocket() to fix it" << endl;
		return NULL;
	}
	server->Send(this->sock,message);
	Call(server->Receive(this->sock));
	return card_to_play;
}
