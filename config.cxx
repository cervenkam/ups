/*
	Class reprezenting game configuration
	Singleton
	Author: Martin Cervenka
	Version: 19.04.2016
*/
#include "config.h"
#include "lang.h"
#include "stdmcr.h"
#include "algorithm.h"
#include "person.h"
#include "algono.h"
#include "programmerbot.h"
#include "networkplayer.h"
#include <cstring>
#include <sstream>
#include <iostream>
#include <fstream>
#define CMP(MCR,NAME) if(!strcmp(name,MCR) || !strcmp(name,NAME))

using namespace std;
/* default algorithm name */
char* Configuration::ms_def = new char[14]{'P','r','o','g','r','a','m','m','e','r','B','o','t','\0'};
/* single instance of this class */
Configuration* Configuration::ms_config = nullptr;

/*
	Returns the algorithm by its name
		=> name Class name of the algorithm
		=> player_name Name of created player
		=> id ID of new algorithm
*/
Algorithm* Configuration::GetAlgorithm(char* name,char* player_name,unsigned id){
	CMP(ALGONO,"AlgoNo"){
		return new AlgoNo(player_name,id);
	}else CMP(PERSON,"Person"){
		return new Person(player_name,id);
	}else CMP(PROGRAMMERBOT,"ProgrammerBot"){
		return new ProgrammerBot(player_name,id);
	}else CMP(NETWORKPLAYER,"NetworkPlayer"){
		return new NetworkPlayer(player_name,id);
	}
	if(!strcmp(name,ms_def)){
		return GetAlgorithm(ms_def,player_name,id);
	}else{
		//SUPERDEFAULT value
		return new ProgrammerBot(player_name,id);
	}
}
/*
	Loads the settings from string
		=> in_str configuration string
*/
void Configuration::Load(string in_str){
	istringstream in(in_str);
	if(in.fail()){
		OUT(ERR_NOT_FOUND << endl);
		exit(1);
	}
	char* str = new char[512];
	char* strtmp = str;
	char* command = new char[512];
	char* parameter = new char[512];
	char* name = new char[512];
	strcpy(name,PLAYER);
	unsigned counter = 0;
	while(in.getline(str,512)){
		while(sscanf(strtmp,"%511[^=]=%511s",command,parameter)){
			if(!strcmp(command,PLAYER_COUNT)){
				SetCount(atoi(parameter));	
				m_algos = new Algorithm*[GetCount()];
			}else if(!strcmp(command,DEFAULT)){
				ms_def = parameter;
			}else if(!strcmp(command,PLAYER)){
				if(m_algos != nullptr && counter < GetCount()){
					m_algos[counter] = GetAlgorithm(parameter,name,counter);
					name = new char[512];
					strcpy(name,PLAYER);
					counter++;
				}
			}else if(!strcmp(command,NAME)){
				strcpy(name,parameter);
			}else if(!strcmp(command,RULES)){
				m_rules=true;
			}
			while(strtmp[0]!=' ' && strtmp[0]!='\0'){
				strtmp++;
			}
			if(strtmp[0]=='\0'){
				break;
			}
			strtmp++;
		}
	}
	for(unsigned a=counter; a<m_count; a++){
		m_algos[a] = GetAlgorithm(ms_def,name,a);
	}
	delete[] command;
	delete[] str;
}
/*
	Sets count of players
		=> count Count of players
*/
void Configuration::SetCount(unsigned count){
	m_count = count;
}
/*
	Returns count of players
		<= Count of players
*/
unsigned Configuration::GetCount(){
	return m_count;
}
/*
	Returns all (array of) created algorithms
		<= Array of created algorithms
*/
Algorithm** Configuration::GetAlgorithms(){
	return m_algos;
}
/*
	Returns single configuration
		=> Configuration string / empty string if already created
		<= Configuration
*/
Configuration* Configuration::GetConfiguration(string str){
	if(ms_config==nullptr || str!=""){
		ms_config = new Configuration(str);
	}
	return ms_config;
}
/*
	Creates new algorithm
		=> str string with configuration
*/
Configuration:: Configuration(string str){
	Load(str);
}
/*
	Returns if own rules are enabled
		<= Are own rules enabled
*/
bool Configuration::AreOwnRules(){
	return m_rules;
}
