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
unsigned counter_of_same_cards = 0;
/* Determines the player which started this round */
unsigned char Game::started = 0;
/*
	Is a player with given ID a starting player of this round?
		=> ID Id of player
		<= Is he?
*/
bool Game::IsHeStarted(unsigned char ID){
	return ID==started;
}
/*
	Sets the starting player of this round
		=> ID Starting player's ID
*/
void Game::SetStarted(unsigned char ID){
	started=ID;
}
/*
	Returns the starting player of this round
		<= Starting player's ID
*/
unsigned char Game::GetStarted(){
	return started;
}
/*
	Creates new game
		=> players Number of players
		=> algos Players algos
*/
Game::Game(unsigned char players,Algorithm** algos){
#ifdef TESTING
	//clear file
	ofstream fout("last_game.txt");
	fout << endl;
	fout.close();
	//end of clear file
#endif
	this->players=players;
	this->algos = new Algorithm*[players];
	for(unsigned a=0; a<players; a++){
		this->algos[a] = algos[a];
	}
	deck = new Deck();
}
#ifdef TESTING
/*
	Creates new game
		=> players Number of players
*/
Game::Game(unsigned char players){
	this->players=players;
	algos = new Algorithm*[players];
	algos[0]=new Person("FOR_TESTING_ONLY",0);
	for(unsigned int a=1; a<players; a++){
		algos[a]=new ProgrammerBot("FOR_TESTING_ONLY",a);
	}
	deck = new Deck();
}
#endif
/*
	Ends the game
*/
void Game::End(){
#ifdef TESTING
	ofstream cout("last_game.txt",ofstream::app);
#endif
	OUT(END_GAME << endl);
	//Find max
	unsigned max_value = 0;
	unsigned count = 0;
	for(unsigned a=0; a<players; a++){
		if(algos[a]->GetPoints() > max_value){
			max_value = algos[a]->GetPoints();
			count = 1;
		}else if(algos[a]->GetPoints() == max_value){
			count++;
		}
	}
	if(count > 1){
		OUT(WINNERS << ": " << endl);
	}else{
		OUT(WINNER << ": " << endl);
	}
	for(unsigned a=0; a<players; a++){
		if(algos[a]->GetPoints() == max_value){
			OUT("\t");
			COLOR("1;41",algos[a]->player);
			OUT(endl);
		}
	}
	for(unsigned a=0; a<players; a++){
		OUT(Algorithm::algos[a]->player << " " << HAS << " " << (unsigned)algos[a]->GetPoints() <<
			((algos[a]->GetPoints()==0)?"":"0") << " " << POINTS << endl);
	}
	//reinitialize static variables
	Algorithm::SetFirstCard(NULL);
	SetStarted(0);
#ifdef TESTING
	cout.close();
#endif
}
/*
	Starts the game
*/
void Game::Start(){
	if(Prepare()){
		Loop();
	}
}
/*
	Prepares the game
		<= Can we continue?
*/
bool Game::Prepare(){
	//Preparing deck
	deck->ThrowAway(Deck::COUNT%players);
#ifndef TESING
	deck->Shuffle();
#endif
	if(FillHands()){
		return false;
	}

	//Clear points
	for(unsigned char a=0; a<players;a++){
		algos[a]->ClearPoints();
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
	for(unsigned a=0;;a++,a%=players){
		//card which player plays
		if(!OneHand(winner,a)){
			break;
		}
#ifdef TESTING
		ofstream cout("last_game.txt",ofstream::app);
		for(unsigned a=0; a<players; a++){
			Hand* hand = algos[a]->GetHand();
			OUT("Player " << (a+1) << " ");
			for(unsigned b=0; b<hand->Size(); b++){
				OUT(*hand->Get(b));
			}
			OUT(endl);
		}
		deck->Print();
		cout.close();
#endif
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
#ifdef TESTING
	ofstream cout("last_game.txt",ofstream::app);
#endif
	started = IsHeStarted(player);
	//select the card
	card = algos[player]->Play(!started || !Algorithm::FirstCard());
	//if he is making the next loop
	if(started && card){
		//check the card if it is compatible
		if(Algorithm::FirstCard() && !Algorithm::FirstCard()->IsPlayable(card)){
			//force next loop
			card = NULL;
			started = false;
			OUT(Algorithm::algos[player]->player << " " << CANT_FORCE << endl);
			return false;		
		} 
		COLOR("0;32",Algorithm::algos[player]->player << " " << LEADS << endl);
		winner = player;
	//if player started and do not want to continue
	}else if(started){
		algos[winner]->AddPoints(points);
		if(points){
			OUT(Algorithm::algos[winner]->player << " " << RECEIVED << " " << (unsigned)(points*10) << " " << POINTS << endl);
		}
		points=0;
	}
#ifdef TESTING
	cout.close();
#endif
	return true;
}
/*
	Fills all player's hands
		<= Is end of game?
*/
bool Game::FillHands(){
	//fill the hands
	for(unsigned y=0;; y++, y%=players){
		unsigned char b = (y+GetStarted())%players;
		if(deck->Size()>0 && algos[b]->GetHand()->Add(deck->Peek())){
			deck->Pop();
		}else{
			break;
		}
	}
	//checks own rule - 4 same cards in hand
	if(Configuration::GetConfiguration()->AreOwnRules() && SameCards()){
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
	Card* card = NULL;
	bool started;
	do{
		if(!ChooseCard(player,winner,card,started)){
			continue;
		}
	}while(!card && !started);
	//sets the first card
	if((card && !Algorithm::FirstCard()) || (!card && Algorithm::FirstCard())){
		Algorithm::SetFirstCard(card);
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
	algos[player]->Send(card);
	if(!card){
		counter_of_same_cards = 0;
		//end of the hand
		SetStarted(winner);
		//set actual player as previous (because loop header moves it to proper value)
		player = (winner+(players-1))%players;
		if(FillHands()){
			return true;
		}
		//end of game
		if(!algos[GetStarted()]->GetHand()->Size()){
#ifdef TESTING
	ofstream cout("last_game.txt",ofstream::app);
#endif
			OUT(algos[GetStarted()]->player << " " << RECEIVED << " " << LAST_POINT << endl);
#ifdef TESTING
	cout.close();
#endif
			algos[GetStarted()]->AddPoints(1);
			End();
			return true;
		}
	}else{
		if(Algorithm::FirstCard() && Algorithm::FirstCard()->GetRank()==card->GetRank()){
			counter_of_same_cards++;
		}else{
			counter_of_same_cards = 0;
		}
		if(counter_of_same_cards==4 && Configuration::GetConfiguration()->AreOwnRules()){
			for(unsigned c=0; c<players; c++){
				algos[c]->ClearPoints();
			}
			algos[player]->AddPoints(10);
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
	for(unsigned y=0; y<players; y++){
		unsigned char b = (y+GetStarted())%players;
		Hand* hand = algos[b]->GetHand();
		unsigned size = hand->Size();
		//not full hand of same cards
		if(size!=Hand::SIZE){
			return false;
		}
		//check only full hand
		for(unsigned c=1; c<size; c++){
			if(hand->Get(c-1)->GetRank()!=hand->Get(c)->GetRank()){
				goto outer;
			}
		}
		for(unsigned c=0; c<players; c++){
			algos[c]->ClearPoints();
		}
		algos[b]->AddPoints(10);
		for(unsigned c=0; c<players; c++){
			algos[c]->GetHand()->Clear();
		}
		OUT(algos[b]->player << " " << SAME_CARDS_HAND << endl);
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
#ifdef TESTING
	ofstream cout("last_game.txt",ofstream::app);
#endif
	if(card->IsValuable()){
		points++;
	}
	//Determine the winner
	if(Algorithm::FirstCard()->IsPlayable(card)){
		COLOR("0;31",NOW << " " << algos[player]->player << " " << LEADS << endl);
		winner = player;
		//player = (winner+(players-1))%players;
	}
	//Search for the card
	Hand* hand = algos[player]->GetHand();
	unsigned char handsize = hand->Size();
	for(unsigned char b=0; b<handsize; b++){
		if(hand->Get(b)==card){
			hand->Use(b);
		}
	}
#ifdef TESTING
	cout.close();
#endif
}
/*
	Prints the game
*/
void Game::Print(){
	OUT("Deck content:" << endl);
	deck->Print();
	OUT(endl);
	for(unsigned char a=0; a<players; a++){
		Algorithm* alg = algos[a];
		unsigned char size = alg->GetHand()->Size();
		for(unsigned char b=0; b<size; b++){
			OUT(*alg->GetHand()->Get(b));
		}
		OUT("\t");
	}
	OUT(endl);
}
