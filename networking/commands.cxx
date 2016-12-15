#include "commands.h"
#include "lang.h"
#include <cstring>
#include <iostream>
#include <unistd.h>
#include "../stdmcr.h"

Commands::funcptr Commands::ms_commands[COMMANDS] = {
	&Commands::Login,
	&Commands::Disconnect,
	&Commands::SendCard,
	&Commands::_CreateGame,
	&Commands::_MyCards,
	&Commands::_Welcome,
	&Commands::_GetCountOfCards,
	&Commands::_SendMessage,
	&Commands::_Vote,
	&Commands::_Table
};
const char* const Commands::ms_texts[COMMANDS] = {
	QUERY_LOGIN,
	QUERY_DISCONNECT,
	QUERY_PLAY,
	QUERY_CREATE_GAME,
	QUERY_MY_CARDS,
	QUERY_WELCOME,
	QUERY_COUNT_OF_CARDS,
	QUERY_MESSAGE,
	QUERY_VOTE,
	QUERY_TABLE,
};

Commands::Commands(){
	m_sock = -1;
	m_server = nullptr;
	m_player = nullptr;
	m_game = nullptr;
	m_semaphore = new Semaphore(0); //deleted in destructor
	STDMSG("1;35","Created:    Commands");
}

void Commands::SendMessage(const char* const message) const{
	char buff[MAX_LEN];
	if(!GetPlayer()){
		BadCommand("Player not exists");
		return;
	}
	if(!GetGame()){
		BadCommand("Game not exists");
		return;
	}
	Append(Append(Add(buff,RESPONSE_MESSAGE),m_player->m_player),message);
	unsigned count = m_game->GetCountOfPlayers();
	for(unsigned a=0; a<count; a++){
		Algorithm* algo = m_game->GetAlgorithm(a);
		NetworkPlayer* np;
		if((np = dynamic_cast<NetworkPlayer*>(algo)))
			np->TrySend(buff);
	}
}
Commands::Commands(int sock,Server* server,NetworkPlayer* player,Game* game){
	SetSocket(sock);
	SetServer(server);
	SetPlayer(player);
	if(GetPlayer()){
		GetPlayer()->SetGameForBothMeAndBot(game);
	}
}


bool Commands::TrySend(Commands* cmd,const char* const msg) const{
	if(cmd==nullptr){
		return false;
	}
	return TrySend(cmd->m_sock,msg);
}
bool Commands::TrySend(int sock,const char* const msg) const{
	if(GetServer() && IsConnected() && IsRunning()){
		m_server->Send(sock,msg);
		return true;
	}
	return false;
}
char* Commands::TryReceive(int sock) const{
	if(GetServer() && IsConnected() && IsRunning()){
		return m_server->Receive(sock);
	}
	return nullptr;
}
void Commands::Start(){
	Welcome(nullptr);
	if(IsRunning()){
		for(char* msg;(msg = TryReceive(m_sock));){
			Call(msg);
			delete[] msg;
		}
		SetConnected(false);
		Disconnect(nullptr);
	}
}

Semaphore* Commands::GetSemaphore() const{
	return m_semaphore;
}
void Commands::SetConnected(bool is_connected){
	m_is_connected = is_connected;
}
bool Commands::IsConnected() const{
	return m_is_connected;
}
void Commands::SetRunning(bool running){
	m_running = running;
}
bool Commands::IsRunning() const{
	return m_running;
}
void Commands::SetSocket(int sock){
	m_sock=sock;
}

int Commands::GetSocket() const{
	return m_sock;
}

void Commands::SetServer(Server* const server){
	m_server=server;
}

Server* Commands::GetServer() const{
	return m_server;
}

void Commands::SetPlayer(NetworkPlayer* const player){
	m_player=player;
}

NetworkPlayer* Commands::GetPlayer() const{
	return m_player;
}

void Commands::SetGame(Game* const game){
	m_game=game;
}

Game* Commands::GetGame() const{
	return m_game;
}

void Commands::SetThread(thread* const thread){
	m_thread = thread;
}

thread* Commands::GetThread() const{
	return m_thread;
}

void Commands::Welcome(const char* const) const{
	TrySend(m_sock,RESPONSE_WELCOME);
}
void Commands::BadCommand(const char* const command) const{
	char* buff = new char[MAX_LEN]; //deleted at the end of this function
	char* ptr = Add(buff,BAD_COMMAND);
	ptr = Append(ptr,command);
	TrySend(m_sock,buff);
	delete[] buff;
}

void Commands::Call(const char* const command){
	if(command == nullptr){
		return;
	}
	int index = FindMethod(command);
	if(index>=0 && index<COMMANDS){
		(this->*ms_commands[index])(command+strlen(ms_texts[index])+1);
	}else{
		BadCommand(command);
	}
	return;
}

bool Commands::TotalCompare(const char* const command,int index){
	if(index<0){
		return false;
	}
	unsigned len = strlen(ms_texts[index]);
	for(unsigned a=0; a<len; a++){
		if(command[a]!=ms_texts[index][a]){
			return false;
		}
	}
	return true;
}

int Commands::FindMethod(const char* const command){
	unsigned max_len = 0;
	int index = -1;
	if(!command){
		BadCommand("NULL command");
		return index;
	}
	unsigned cmdlen = strlen(command);
	for(int a=0; a<COMMANDS; a++){
		unsigned len = strlen(ms_texts[a]);
		unsigned min_len = len > cmdlen ? cmdlen : len;
		unsigned b = NumberOfSameLetters(command,ms_texts[a],min_len);
		if(max_len < b){
			max_len = b;
			index = a;
		}
	}
	return TotalCompare(command,index)?index:-1;
}
unsigned Commands::NumberOfSameLetters(const char* const cmp1,const char* const cmp2,unsigned len){
	unsigned a;
	for(a=0; a<len; a++){
		if(cmp1[a] != cmp2[a]){
			break;
		}
	}
	return a;
}
void Commands::MyCards(const char* const) const{
	char* buff = new char[MAX_LEN]; //deleted at the end of this function
	char* ptr = Add(buff,RESPONSE_YOUR_CARDS);
	if(!GetPlayer()){
		BadCommand("Player does not exists");
	}else{
		GetPlayer()->FillCardsString(ptr);
		TrySend(m_sock,buff);
	}
	delete[] buff; //created an the start of this function
}
void Commands::GetCountOfCards(const char* const) const{
	char* buff = new char[MAX_LEN]; //deleted at the end of this function
	char* ptr = Add(buff,RESPONSE_COUNT_CARDS);
	if(!GetGame()){
		BadCommand("Game does not exists");
	}else{
		unsigned playerscount = GetGame()->GetCountOfPlayers();
		for(unsigned c=0; c<playerscount; c++){
			Algorithm* alg = GetGame()->GetAlgorithm(c);
			if(alg && alg != GetPlayer()){
				ptr = Append(ptr,alg->m_player);
				ptr = Append(ptr,alg->GetCardCount());
			}
		}
		TrySend(m_sock,buff);
	}
	delete[] buff; //created at the start of this function
}
	
void Commands::Login(const char* const message){
	const char* player_name = message;
	if(!GetServer()){
		BadCommand("Server does not exists");
		return;
	}
	if(GetGame()){
		BadCommand("You are already logged in");
		return;
	}
	unsigned games_len = GetServer()->GetCountOfGames();
	for(unsigned a=0; a<games_len; a++){
		Game* game = GetServer()->GetGame(a);
		FindPlayerInGame(game,player_name);
	}
	if(m_player == nullptr){
		TrySend(m_sock,RESPONSE_NO_GAME);
		return;
	}else{
		TryStartMyGame();
	}
}
void Commands::FindPlayerInGame(Game* game,const char* const player_name){
	if(!game){
		return;
	}
	unsigned players_len = game->GetCountOfPlayers();
	for(unsigned b=0; b<players_len; b++){
		Algorithm* algo = game->GetAlgorithm(b);
		if(algo && !strcmp(algo->m_player,player_name)){
			if((m_player = dynamic_cast<NetworkPlayer*>(algo))){	
				InitPlayer(game);
				break;
			}else{
				TrySend(m_sock,RESPONSE_NO_GAME);
				return;
			}
		}
	}
}
void Commands::TryStartMyGame(){
	bool game_is_ready = true;
	if(!GetGame()){
		BadCommand("Game not exists");
		return;
	}
	unsigned players_len = GetGame()->GetCountOfPlayers();
	for(unsigned a=0; a<players_len; a++){
		if(!GetGame()->GetAlgorithm(a)->IsReady()){
			game_is_ready = false;
			break;
		}
	}
	if(game_is_ready){
		GetGame()->StartParallel();
	}
}
void Commands::InitPlayer(Game* game){
	if(!GetPlayer()){
		BadCommand("Player does not exists");
		return;
	}
	if(GetPlayer()->IsReady()){
		TrySend(GetSocket(),RESPONSE_PLAYER_EXISTS);
		return;
	}
	GetPlayer()->SetCommands(this);
	SetConnected(true);
	m_game = game;
	TrySend(GetSocket(),RESPONSE_A_GAME);
	GetPlayer()->SetReady();
	GetPlayer()->SetGameForBothMeAndBot(GetGame());
}
void Commands::Disconnect(const char* const){
	if(!GetServer()){
		BadCommand("Server does not exists");
		return;
	}
	TrySend(GetSocket(),RESPONSE_BYE);
	close(GetSocket());
	SetConnected(false);
	SetRunning(false);
	GetServer()->TidyUp(this);
}

void Commands::SendCard(const char* const message){
	if(!GetPlayer()){
		BadCommand("Player does not exists");
		return;
	}
	m_card_to_play = nullptr;
	unsigned value = Card::FromString(message);
	if(value>>6){
		return;
	}
	unsigned a = value&3;
	unsigned b = value>>2;
	GetPlayer()->SetNextCard(a,b);
	GetPlayer()->Notify();
}

void Commands::Vote(const char* const message) const{
	if(!GetPlayer()){
		BadCommand("Player does not exists");
		return;
	}
	int vote = 0;
	if(message && !strcmp(message,VOTE_YES)){
		vote = 1;
	}else if(message && !strcmp(message,VOTE_NO)){
		vote = -1;
	}
	GetPlayer()->SetVote(vote);
	GetPlayer()->VoteNotify();
}

void Commands::CreateGame(const char* const message) const{
	if(!message){
		BadCommand("Bad parameter");
		return;
	}
	if(!GetServer()){
		BadCommand("Server does not exists");
		return;
	}
	if(GetGame()){
		BadCommand("You are already logged in");
		return;
	}
	unsigned length = strlen(message);
	char* game_name = new char[length+1];
	strcpy(game_name,message);
	const char* params = game_name;
	for(unsigned a=1; a<length; a++){
		if(message[a-1]==' '){
			game_name[a-1]='\0';
			params=message+a;	
			break;
		}
	}
	if(game_name == params){
		BadCommand("Missing parameters in " QUERY_CREATE_GAME " command");
		return;
	}
	Configuration* configuration = new Configuration(params); //deleted in game destructor
	if(!configuration->IsValid()){
		BadCommand("Configuration is not valid");
		return;
	}
	Game* game = new Game(configuration); //deleted in Server::StopGame || Server destructor
	game->SetName(game_name);
	GetServer()->AddGame(game);
	TrySend(GetSocket(),RESPONSE_GAME_CREATED);
	delete[] game_name;
}
Commands::~Commands(){
	delete m_semaphore; //created in constructor
	delete m_thread; //created in Server::Start
	STDMSG("1;35","Deleted:    Commands");
}
void Commands::GarbageCollect(){
	if(!IsRunning()){
		GetSemaphore()->Notify();	
		GetServer()->RemoveCommands(this);
		
		GetThread()->join();
		if(GetPlayer()!=nullptr){
			GetPlayer()->SetReady(false);
			GetPlayer()->SetCommands(nullptr);
		}
		StopGame();
		STDMSG("0;36","Disconnected");
	}
}
void Commands::Table(const char*) const{
	char* buffer = new char[2048];
	char* ptr = buffer;
	if(!GetGame()){
		BadCommand("Game does not exists");
		return;
	}
	const Deck* on_table = GetGame()->GetCardsOnTable();
	ptr = Add(ptr,RESPONSE_TABLE);
	on_table->ForEach([&ptr](const Card* card){
		ptr = Append(ptr,card->ToString());
	});
	TrySend(m_sock,buffer);
	delete[] buffer;
}
void Commands::StopGame(){
	if(GetGame()){
		unsigned len = GetGame()->GetCountOfPlayers();
		bool should_be_stopped = true;
		for(unsigned a=0; a<len; a++){
			if(!FindReasonToStay(a)){
				should_be_stopped = false;
				break;
			}
		}
		if(should_be_stopped){
			GetGame()->StopParallel();
			if(GetServer()){
				GetServer()->RemoveGame(GetGame());
			}
		}
	}
}
bool Commands::FindReasonToStay(unsigned algo_index){
	if(!GetGame()){
		BadCommand("Game does not exists");
		return true;
	}
	Algorithm* algo = GetGame()->GetAlgorithm(algo_index);
	NetworkPlayer* player = nullptr;
	if((player = dynamic_cast<NetworkPlayer*>(algo))){	
		if(player->IsReady()){
			return false;
		}
	}
	return true;
}
void Commands::Join() const{
	if(GetThread()){
		GetThread()->join();
	}
}
void Commands::Notify() const{
	if(GetSemaphore()){
		GetSemaphore()->Notify();
	}
}
const char* Commands::GetAlgorithmName(unsigned index){
	if(!GetGame()){
		BadCommand("Game does not exists");
		return "";
	}
	return GetGame()->GetAlgorithmName(index);
}
void Commands::    _SendMessage(const char* msg){     SendMessage(msg); }
void Commands::     _BadCommand(const char* msg){      BadCommand(msg); }
void Commands::     _CreateGame(const char* msg){      CreateGame(msg); }
void Commands::           _Vote(const char* msg){            Vote(msg); }
void Commands::        _MyCards(const char* msg){         MyCards(msg); }
void Commands::        _Welcome(const char* msg){         Welcome(msg); }
void Commands::_GetCountOfCards(const char* msg){ GetCountOfCards(msg); }
void Commands::          _Table(const char* msg){           Table(msg); }
