#ifndef _CONFIG_H_
#define _CONFIG_H_

/*
	Class reprezenting game configuration
	Singleton
	Author: Martin Cervenka
	Version: 19.04.2016
*/
#include "algorithm.h"
using namespace std;
class Configuration{
	public:
		/*
			Returns the algorithm by its name
				=> name Class name of the algorithm
				=> player_name Name of created player
				=> id ID of new algorithm
		*/
		Algorithm* GetAlgorithm(char* name,char* player_name,unsigned id);
		/*
			Returns single configuration
				=> Configuration string / NULL if already created
				<= Configuration
		*/
		static Configuration* GetConfiguration(string str="");
		/*
			Returns if own rules are enabled
				<= Are own rules enabled
		*/
		bool AreOwnRules();
		/*
			Sets count of players
				=> count Count of players
		*/
		void SetCount(unsigned count);
		/*
			Returns all (array of) created algorithms
				<= Array of created algorithms
		*/
		Algorithm** GetAlgorithms();
		/*
			Returns count of players
				<= Count of players
		*/
		unsigned GetCount();
#ifndef TESTING
	private:
#endif
		/* sigle configuration instance */
		static Configuration* config;
		/* default algorithm name */
		static char* def;
		/*
			Creates new algorithm
				=> str string with configuration
		*/
		Configuration(string str);
		/*
			Loads the settings from string
				=> str string with configuration
		*/
		void Load(string str);
		/* are own rules enabled */
		bool rules = false;
		/* count of the players */
		unsigned count = 2;
		/* array of algorithms */
		Algorithm** algos;
};

#endif
