#include "lang.h"
#include "config.h"
#include "stdmcr.h"
#include "game.h"
#include "hand.h"
#include "deck.h"
#include "algono.h"
#include "programmerbot.h"
#include "person.h"
#include <iostream>
#include <fstream>
#include <thread>
unsigned counter_of_same_cards = 0;
// -> test -> test
bool Game::IsHeStarted(unsigned char ID){
	return ID==m_started;
}
void Game::SetStarted(unsigned char ID){
	m_started=ID;
}
unsigned char Game::GetStarted(){
	return m_started;
}
void Game::SetName(char* name){
	m_name = name;
}
char* Game::GetName(){
	return m_name;
}
Game::Game(Configuration* conf){
	m_conf=conf;
	m_players=conf->GetCount();
	m_algos=conf->GetAlgorithms();
	for(unsigned a=0; a<m_players; a++){
		m_algos[a]->SetGame(this);
	}
	m_name = "unnamed game";
	m_deck = new Deck(); //deleted in destructor
	m_thr = nullptr;
}

Game::~Game(){
	StopParallel();
	STDMSG("1;35","Deleted:    Game");
	delete m_conf; //creates Commands::CreateGame
	delete m_deck; //created in constructor
}
Algorithm* Game::GetAlgorithm(unsigned player){
	return m_algos[player];
}
unsigned Game::GetCountOfPlayers(){
	return m_players;
}
void Game::End(){
	OUT(END_GAME << endl);
	//Find max
	unsigned count = 0;
	unsigned max_value = FindMaxPoints(count);
	OUT(((count>1)?WINNERS:WINNER) << ": " << endl);
	PrintWinners(max_value);
	for(unsigned a=0; a<m_players; a++){
		OUT(m_algos[a]->m_player << " " << HAS << " " << (unsigned)m_algos[a]->GetPoints() <<
			((m_algos[a]->GetPoints()==0)?"":"0") << " " << POINTS << endl);
	}
	SetFirstCard(nullptr);
	SetStarted(0);
}
unsigned Game::FindMaxPoints(unsigned& count){
	unsigned max_value = 0;
	for(unsigned a=0; a<m_players; a++){
		if(m_algos[a]->GetPoints() > max_value){
			max_value = m_algos[a]->GetPoints();
			count = 1;
		}else if(m_algos[a]->GetPoints() == max_value){
			count++;
		}
	}
	return max_value;	
}

void Game::PrintWinners(unsigned max_value){
	for(unsigned a=0; a<m_players; a++){
		if(m_algos[a]->GetPoints() == max_value){
			OUT("\t");
			COLOR("1;41",m_algos[a]->m_player);
			OUT(endl);
		}
	}
}
void Game::Start(){
	do{
		if(Prepare()){
			Loop();
		}
	}while(VotePasses());
}
bool Game::VotePasses(){
	int votes = 0;
	for(unsigned a=0; a<m_players; a++){
		votes+=m_algos[a]->Vote();
	}
	return votes>0;
}
void Game::StopParallel(){
	m_end_of_game = true;
	for(unsigned a=0; a<m_players; a++){
		m_algos[a]->Notify();
		m_algos[a]->VoteNotify();
	}
	if(m_thr != nullptr){
		m_thr->join();
		delete m_thr; //created in StartParallel function
		m_thr = nullptr;
	}
}
void Game::StartParallel(){
	if(m_thr != nullptr){
		cerr << "Game is already running" << endl;
		return;
	}
	m_thr = new thread(&Game::Start,this); //deleted in Stopparallel function
}
bool Game::Prepare(){
	//Preparing deck
	m_deck->ThrowAway(Deck::ms_COUNT%m_players);
	m_deck->Shuffle();
	if(FillHands()){
		return false;
	}

	//Clear points
	for(unsigned char a=0; a<m_players;a++){
		m_algos[a]->ClearPoints();
	}
	return true;
}
void Game::Loop(){
	//player which win this hand
	unsigned winner = 0;
	SetStarted(0);
	//lets start the game
	for(unsigned a=0;;a++,a%=m_players){
		//card which player plays
		if(!OneHand(winner,a)){
			break;
		}
		//if it is end of game
		if(m_end_of_game){
			return;
		}
	}
}
bool Game::ChooseCard(unsigned& player,unsigned& winner,const Card*& card, bool& started){
	started = IsHeStarted(player);
	card = m_algos[player]->Play(!started || !FirstCard());
	if(!m_end_of_game && started && card){
		if(!ReactOnCard(player,winner,card,started)){
			return false;
		}
	}else if(!m_end_of_game && started){
		m_algos[winner]->AddPoints(m_points);
		if(m_points){
			STDMSG("0;31","Game:       " << m_algos[winner]->m_player << " " << RECEIVED << " " << (unsigned)(m_points*10) << " " << POINTS);
		}
		m_points=0;
	}
	return true;
}
bool Game::ReactOnCard(unsigned& player,unsigned& winner,const Card*& card, bool& started){
	if(FirstCard() && !FirstCard()->IsPlayable(card)){
		card = nullptr;
		started = false;
		STDMSG("0;31","Game:       " << m_algos[player]->m_player << " " << CANT_FORCE);
		return false;		
	} 
	STDMSG("0;31","Game:       " << m_algos[player]->m_player << " " << LEADS);
	winner = player;
	return true;
}
bool Game::FillHands(){
	//fill the hands
	for(unsigned y=0;; y++, y%=m_players){
		unsigned char b = (y+GetStarted())%m_players;
		if(m_deck->Size()>0 && m_algos[b]->AddCard(m_deck->Peek())){
			m_deck->Pop();
		}else{
			break;
		}
	}
	//checks own rule - 4 same cards in hand
	if(m_conf->AreOwnRules() && SameCards()){
		return true;
	}
	return false;
}
bool Game::OneHand(unsigned& winner,unsigned& player){
	const Card* card = nullptr;
	bool started;
	do{
		if(!ChooseCard(player,winner,card,started)){
			continue;
		}
	}while(!card && !started && !m_end_of_game);
	//if it is end of game
	if(m_end_of_game){
		return true;
	}
	//sets the first card
	if((card && !FirstCard()) || (!card && FirstCard())){
		SetFirstCard(card);
	}
	return !UseCard(card,player,winner);
}
bool Game::UseCard(const Card* card,unsigned& player, unsigned& winner){
	m_algos[player]->Send(card);
	if(!card){
		if(UseNoCard(player,winner)){
			return true;
		}
	}else{
		if(UseValidCard(card,player,winner)){
			return true;
		}
	}
	return false;
}
bool Game::UseValidCard(const Card* card,unsigned& player, unsigned& winner){
	if(FirstCard() && FirstCard()->GetRank()==card->GetRank()){
		counter_of_same_cards++;
	}else{
		counter_of_same_cards = 0;
	}
	if(counter_of_same_cards==4 && m_conf->AreOwnRules()){
		for(unsigned c=0; c<m_players; c++){
			m_algos[c]->ClearPoints();
		}
		m_algos[player]->AddPoints(10);
		End();
		return true;	
	}
	DetermineWinner(card,player,winner);
	return false;
}
bool Game::UseNoCard(unsigned& player, unsigned& winner){
	counter_of_same_cards = 0;
	//end of the hand
	SetStarted(winner);
	//set actual player as previous (because loop header moves it to proper value)
	player = (winner+(m_players-1))%m_players;
	if(FillHands()){
		return true;
	}
	//end of game
	if(!m_algos[GetStarted()]->GetCardCount()){
		OUT(m_algos[GetStarted()]->m_player << " " << RECEIVED << " " << LAST_POINT << endl);
		m_algos[GetStarted()]->AddPoints(1);
		End();
		return true;
	}
	return false;
}
bool Game::SameCards(){
	for(unsigned y=0; y<m_players; y++){
		unsigned char b = (y+GetStarted())%m_players;
		unsigned size = m_algos[b]->GetCardCount();
		if(size!=Hand::ms_SIZE){
			return false;
		}
		for(unsigned c=1; c<size; c++){
			if(m_algos[b]->GetCard(c-1)->GetRank()!=m_algos[b]->GetCard(c)->GetRank()){
				goto outer;
			}
		}
		FoundSameCards(b);
		return true;
		outer:;
	}
	return false;
}
void Game::FoundSameCards(unsigned winner_index){
	for(unsigned c=0; c<m_players; c++){
		m_algos[c]->ClearPoints();
	}
	m_algos[winner_index]->AddPoints(10);
	for(unsigned c=0; c<m_players; c++){
		m_algos[c]->ClearCards();
	}
	OUT(m_algos[winner_index]->m_player << " " << SAME_CARDS_HAND << endl);
	End();
}
void Game::DetermineWinner(const Card* card,unsigned& player, unsigned& winner){
	if(card->IsValuable()){
		m_points++;
	}
	//Determine the winner
	if(FirstCard()->IsPlayable(card)){
		STDMSG("0;31","Game:       " << NOW << " " << m_algos[player]->m_player << " " << LEADS);
		winner = player;
		//player = (winner+(players-1))%players;
	}
	//Search for the card
	unsigned char handsize = m_algos[player]->GetCardCount();
	for(unsigned char b=0; b<handsize; b++){
		if(m_algos[player]->GetCard(b)==card){
			m_algos[player]->UseCard(b); 
		}
	}
}
void Game::Print(){
	OUT("Deck content:" << endl);
	m_deck->Print();
	OUT(endl);
	for(unsigned char a=0; a<m_players; a++){
		Algorithm* alg = m_algos[a];
		unsigned char size = alg->GetCardCount();
		for(unsigned char b=0; b<size; b++){
			OUT(*alg->GetCard(b));
		}
		OUT("\t");
	}
	OUT(endl);
}
const Card* Game::FirstCard(){
	return m_first;
}
void Game::SetFirstCard(const Card* card){
	m_first=card;
}
const char* Game::GetAlgorithmName(unsigned index){
	return m_algos[index]->m_player;
}
