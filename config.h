#ifndef _CONFIG_H_
#define _CONFIG_H_

#include "algorithm.h"
class Algorithm;
class Configuration{
	public:
		Configuration(std::string str);
		~Configuration();
		void SetCount(unsigned count);
		bool AreOwnRules();
		Algorithm** GetAlgorithms();
		unsigned GetCount();
		bool IsValid();
	private:
		Algorithm* GetAlgorithm(const char* name,const char* player_name,unsigned id);
		bool ParseOneParameter(unsigned&,char*&,char*&,char*,char*);
		void ReadLines(std::istringstream&,unsigned&,char*,char*);
		void AddAlgorithm(unsigned&,char*&,char*);
		void InitAlgorithms(unsigned);
		bool UpdatePointer(char*& strtmp);
		static const char* ms_def;
		void Load(std::string str);
		bool m_rules = false;
		unsigned m_count = 2;
		Algorithm** m_algos;
		bool m_valid = true;
		bool m_at_least_one_player = false;
		unsigned m_persons = 0;
};

#endif
