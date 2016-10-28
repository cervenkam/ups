#ifndef _CONFIG_H_
#define _CONFIG_H_

#include "algorithm.h"
using namespace std;
class Algorithm;
class Configuration{
	public:
		Configuration(string str);
		~Configuration();
		void SetCount(unsigned count);
		//GETTERS
		Algorithm* GetAlgorithm(const char* name,const char* player_name,unsigned id);//GETTER
		bool AreOwnRules();//GETTER
		Algorithm** GetAlgorithms();//GETTER
		unsigned GetCount();//GETTER
	private:
		bool ParseOneParameter(unsigned&,char*&,char*&,char*,char*);
		void ReadLines(istringstream&,unsigned&,char*&,char*,char*);
		void AddAlgorithm(unsigned&,char*&,char*);
		void InitAlgorithms(unsigned);
		bool UpdatePointer(char*& strtmp);
		static const char* ms_def;
		void Load(string str);
		bool m_rules = false;
		unsigned m_count = 2;
		Algorithm** m_algos;
};

#endif
