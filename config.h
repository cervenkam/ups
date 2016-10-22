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
class Algorithm;
class Configuration{
	public:
		/*
			Creates new algorithm
				=> str string with configuration
		*/
		Configuration(string str);
		/*
			Destruct this configuration
		*/
		~Configuration();
		/*
			Returns the algorithm by its name
				=> name Class name of the algorithm
				=> player_name Name of created player
				=> id ID of new algorithm
		*/
		Algorithm* GetAlgorithm(const char* name,const char* player_name,unsigned id);
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
	private:
		/* default algorithm name */
		static const char* ms_def;
		/*
			Loads the settings from string
				=> str string with configuration
		*/
		void Load(string str);
		/* are own rules enabled */
		bool m_rules = false;
		/* count of the players */
		unsigned m_count = 2;
		/* array of algorithms */
		Algorithm** m_algos;
};

#endif
