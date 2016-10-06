#include "testing.h"
#include "../config.h"
#include "../game.h"
#include <unistd.h>
#include <thread>
#include "../programmerbot.h"

static Game* game;
static unsigned count = 3;
static Algorithm** algos = new Algorithm*[count];
static bool running;
static const unsigned tests = 100;
NAMESPACE(5)
	unsigned score_sum;
	void run_games(){
		running = true;
		Algorithm** algos = new Algorithm*[count];
		for(unsigned a=0; a<count; a++){
			algos[a] = new ProgrammerBot("FOR_TESTING_ONLY",a);
		}
		for(unsigned a=0; a<tests; a++){
			Game* game = new Game(count,algos);
			try{
				game->Start();
			}catch(exception& e){
				return;
			}
			for(unsigned b=0; b<game->players; b++){
				score_sum += algos[b]->GetPoints();
			}
			delete game;
		}
		running = false;
	}
	FIRST_TEST("Create players - count")
		for(unsigned a=0; a<count; a++){
			algos[a] = new ProgrammerBot("FOR_TESTING_ONLY",a);
		}
		game = new Game(count,algos);
		ASSERT((unsigned)game->players,count);
	TEST(0,1,"Multiple games test")
		{
			thread games(run_games);
			games.detach();
		}
		sleep(tests/50);
		ASSERT(running,false);
	TEST(1,2,"Prepare test")
		game->Prepare();
		ASSERT((unsigned)game->deck->Size(),18); //32 - 2out - 4*3players = 18
	TEST(2,3,"Fill hands - no effect test")
		game->FillHands();
		ASSERT((unsigned)game->deck->Size(),18); 
	TEST(3,4,"Defined deck")
		unsigned char deck[18]{
			 0, 1, 2, 3, 4, 5,
			 6, 7, 8, 9,10,11,
			12,13,14,15,16,17
		};//TODO
		unsigned char p[3][4]{
			{18,19,20,21},
			{22,23,24,25},
			{26,27,28,29}
		};//TODO
		for(unsigned a=0; a<18; a++){
			game->deck->cards[a]->value=deck[a];
		}
		for(unsigned a=0; a<count; a++){
			for(unsigned b=0; b<Hand::SIZE; b++){
				game->algos[a]->hand->Set(b,new Card(p[a][b]));
			}
		}
		ASSERT((unsigned)game->deck->Size(),18)
	TEST(4,5,"Check score sum")
		ASSERT(tests*9,score_sum)
	TEST(5,6,"Own rules in hand")
		delete game;
		Configuration::config->rules = true;
		game = new Game(count,algos);
		unsigned char p[4]{0,1,2,3};
		for(unsigned a=0; a<Hand::SIZE; a++){
			game->algos[0]->hand->Set(a,new Card(p[a]));
		}
		game->FillHands();
		ASSERT((unsigned)game->algos[0]->points,10)
	END
	START(4,5,6)
END
