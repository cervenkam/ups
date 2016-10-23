/*
	Class reprezenting the game
	Author: Martin Cervenka
	Version: 30.03.2016
*/
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
/*
	Is a player with given ID a starting player of this round?
		=> ID Id of player
		<= Is he?
*/
bool Game::IsHeStarted(unsigned char ID){
	return ID==m_started;
}
/*
	Sets the starting player of this round
		=> ID Starting player's ID
*/
void Game::SetStarted(unsigned char ID){
	m_started=ID;
}
/*
	Returns the starting player of this round
		<= Starting player's ID
*/
unsigned char Game::GetStarted(){
	return m_started;
}

/*
	Sets the name of this game
		<= name Name of this game
*/
void Game::SetName(char* name){
	m_name = name;
}

/*
	Gets the name of this game
		<= Name of this game
*/
char* Game::GetName(){
	return m_name;
}

/*
	Creates a game
		=> conf Game configuration
*/
Game::Game(Configuration* conf){
	m_conf=conf;
	m_players=conf->GetCount();
	m_algos=conf->GetAlgorithms();
	for(unsigned a=0; a<m_players; a++){
		m_algos[a]->SetGame(this);
	}
	m_deck = new Deck(); //deleted in destructor
	m_thr = nullptr;
}

Game::~Game(){
	StopParallel();
	STDMSG("1;35","Deleted:    Game");
	delete m_conf; //creates Commands::CreateGame
	delete m_deck; //created in constructor
}
/*
	Returns the game algorithm
		=> player Player ID
		<= Player algorithm
*/

Algorithm* Game::GetAlgorithm(unsigned player){
	return m_algos[player];
}

/*
	Returns number of players
		<= Number of players
*/
unsigned Game::GetCountOfPlayers(){
	return m_players;
}

/*
	Ends the game
*/
void Game::End(){
	OUT(END_GAME << endl);
	//Find max
	unsigned max_value = 0;
	unsigned count = 0;
	for(unsigned a=0; a<m_players; a++){
		if(m_algos[a]->GetPoints() > max_value){
			max_value = m_algos[a]->GetPoints();
			count = 1;
		}else if(m_algos[a]->GetPoints() == max_value){
			count++;
		}
	}
	if(count > 1){
		OUT(WINNERS << ": " << endl);
	}else{
		OUT(WINNER << ": " << endl);
	}
	for(unsigned a=0; a<m_players; a++){
		if(m_algos[a]->GetPoints() == max_value){
			OUT("\t");
			COLOR("1;41",m_algos[a]->m_player);
			OUT(endl);
		}
	}
	for(unsigned a=0; a<m_players; a++){
		OUT(m_algos[a]->m_player << " " << HAS << " " << (unsigned)m_algos[a]->GetPoints() <<
			((m_algos[a]->GetPoints()==0)?"":"0") << " " << POINTS << endl);
	}
	SetFirstCard(nullptr);
	SetStarted(0);
}
/*
	Starts the game
*/
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
/*
	Stops the game in a thread
*/
void Game::StopParallel(){
	m_end_of_game = true;
	for(unsigned a=0; a<m_players; a++){
		m_algos[a]->GetSemaphore()->Notify();
		m_algos[a]->GetSemaphoreForVote()->Notify();
	}
	if(m_thr != nullptr){
		m_thr->join();
		delete m_thr; //created in StartParallel function
		m_thr = nullptr;
	}
}
/*
	Game in a thread
*/
void Game::StartParallel(){
	if(m_thr != nullptr){
		cerr << "Game is already running" << endl;
		return;
	}
	m_thr = new thread(&Game::Start,this); //deleted in Stopparallel function
}
/*
	Prepares the game
		<= Can we continue?
*/
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
/*
	Does multiple loops
*/
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
/*
	Choose card from player's hand
		=> player Player who plays
		=> winner Actual leader of this hand
		=> card Choosed card
		=> started Is this player started?
		<= Is end of game?
*/
bool Game::ChooseCard(unsigned& player,unsigned& winner,Card*& card, bool& started){
	started = IsHeStarted(player);
	//select the card
	card = m_algos[player]->Play(!started || !FirstCard());
	//if the game ends
	if(m_end_of_game){
		return true; //END
	}
	//if he is making the next loop
	if(started && card){
		//check the card if it is compatible
		if(FirstCard() && !FirstCard()->IsPlayable(card)){
			//force next loop
			card = nullptr;
			started = false;
			STDMSG("0;31","Game:       " << m_algos[player]->m_player << " " << CANT_FORCE);
			return false;		
		} 
		STDMSG("0;31","Game:       " << m_algos[player]->m_player << " " << LEADS);
		winner = player;
	//if player started and do not want to continue
	}else if(started){
		m_algos[winner]->AddPoints(m_points);
		if(m_points){
			STDMSG("0;31","Game:       " << m_algos[winner]->m_player << " " << RECEIVED << " " << (unsigned)(m_points*10) << " " << POINTS);
		}
		m_points=0;
	}
	return true;
}
/*
	Fills all player's hands
		<= Is end of game?
*/
bool Game::FillHands(){
	//fill the hands
	for(unsigned y=0;; y++, y%=m_players){
		unsigned char b = (y+GetStarted())%m_players;
		if(m_deck->Size()>0 && m_algos[b]->GetHand()->Add(m_deck->Peek())){
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
/*
	Does one loop in game
		=> winner Actual leader of this hand
		=> player Player who plays
		<= Is end of game?
*/
bool Game::OneHand(unsigned& winner,unsigned& player){
	Card* card = nullptr;
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
/*
	Uses card which player chose
		=> card Card which player chose
		=> player Player who chose card
		=> winner Actual leader of this hand
		<= Is end of game?
*/
bool Game::UseCard(Card* card,unsigned& player, unsigned& winner){
	//use the card
	m_algos[player]->Send(card);
	if(!card){
		counter_of_same_cards = 0;
		//end of the hand
		SetStarted(winner);
		//set actual player as previous (because loop header moves it to proper value)
		player = (winner+(m_players-1))%m_players;
		if(FillHands()){
			return true;
		}
		//end of game
		if(!m_algos[GetStarted()]->GetHand()->Size()){
			OUT(m_algos[GetStarted()]->m_player << " " << RECEIVED << " " << LAST_POINT << endl);
			m_algos[GetStarted()]->AddPoints(1);
			End();
			return true;
		}
	}else{
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
	}
	return false;
}
/*
	Check same cards, if there are full hand of same card, ends the game
		<= Is end of the game
*/
bool Game::SameCards(){
	//check the same cards
	for(unsigned y=0; y<m_players; y++){
		unsigned char b = (y+GetStarted())%m_players;
		Hand* hand = m_algos[b]->GetHand();
		unsigned size = hand->Size();
		//not full hand of same cards
		if(size!=Hand::ms_SIZE){
			return false;
		}
		//check only full hand
		for(unsigned c=1; c<size; c++){
			if(hand->Get(c-1)->GetRank()!=hand->Get(c)->GetRank()){
				goto outer;
			}
		}
		for(unsigned c=0; c<m_players; c++){
			m_algos[c]->ClearPoints();
		}
		m_algos[b]->AddPoints(10);
		for(unsigned c=0; c<m_players; c++){
			m_algos[c]->GetHand()->Clear();
		}
		OUT(m_algos[b]->m_player << " " << SAME_CARDS_HAND << endl);
		End();
		return true;
		outer:;
	}
	return false;
}
/*
	Determines the winner of this hand
		=> card Used card
		=> player Player who used the card
		=> winner Actual winner
*/
void Game::DetermineWinner(Card* card,unsigned& player, unsigned& winner){
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
	Hand* hand = m_algos[player]->GetHand();
	unsigned char handsize = hand->Size();
	for(unsigned char b=0; b<handsize; b++){
		if(hand->Get(b)==card){
			hand->Use(b); 
		}
	}
}
/*
	Prints the game
*/
void Game::Print(){
	OUT("Deck content:" << endl);
	m_deck->Print();
	OUT(endl);
	for(unsigned char a=0; a<m_players; a++){
		Algorithm* alg = m_algos[a];
		unsigned char size = alg->GetHand()->Size();
		for(unsigned char b=0; b<size; b++){
			OUT(*alg->GetHand()->Get(b));
		}
		OUT("\t");
	}
	OUT(endl);
}
/*
	Returns the card which should another player react on
		<= Card to be reacted on
*/
Card* Game::FirstCard(){
	return m_first;
}
/*
	Sets the card which should another player react on
		=> card Card to be reacted on
*/
void Game::SetFirstCard(Card* card){
	m_first=card;
}
