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
const char* Configuration::ms_def = "ProgrammerBot";

/*
	Returns the algorithm by its name
		=> name Class name of the algorithm
		=> player_name Name of created player
		=> id ID of created algorithm
*/
Algorithm* Configuration::GetAlgorithm(const char* name,const char* player_name,unsigned id){
	CMP(ALGONO,"AlgoNo"){
		return new AlgoNo(player_name,id); //deleted in destuctor (1/6)
	}else CMP(PERSON,"Person"){
		return new Person(player_name,id); //deleted in destuctor (1/6)
	}else CMP(PROGRAMMERBOT,"ProgrammerBot"){
		return new ProgrammerBot(player_name,id); //deleted in destuctor (1/6)
	}else CMP(NETWORKPLAYER,"NetworkPlayer"){
		return new NetworkPlayer(player_name,id); //deleted in destuctor (1/6)
	}
	if(!strcmp(name,ms_def)){
		return GetAlgorithm(ms_def,player_name,id); //deleted in destuctor (1/6)
	}else{
		//SUPERDEFAULT value
		return new ProgrammerBot(player_name,id); //deleted in destuctor (1/6)
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
	char* str = new char[512]; //deleted at the end of this function
	char* strtmp = str;
	char* command = new char[512]; //deleted at the end of this function
	char* parameter = new char[512]; //deleted at the end of this function
	char* name = new char[512]; //deleted at the end of this function (1/n) || in Algorithm destructor ((n-1)/n)
	strcpy(name,PLAYER);
	unsigned counter = 0;
	while(in.getline(str,512)){
		while(sscanf(strtmp,"%511[^=]=%511s",command,parameter)){
			if(!strcmp(command,PLAYER_COUNT)){
				SetCount(atoi(parameter));	
				if(m_algos == nullptr){
					m_algos = new Algorithm*[GetCount()]; //deleted in destructor
				}
			}else if(!strcmp(command,DEFAULT)){
				ms_def = parameter;
			}else if(!strcmp(command,PLAYER)){
				if(m_algos != nullptr && counter < GetCount()){
					m_algos[counter] = GetAlgorithm(parameter,name,counter);
					name = new char[512]; //deleted in Algorithm destuctor ((n-1)/n) || end of this function (1/n)
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
	delete[] name; //created at the start of this function (1/n) || at the middle of this function ((n-1)/n)
	delete[] parameter; //created at the start of this function
	delete[] command; //created at the start of this function
	delete[] str; //created at the start of this function
}
/*
	Destruct this configuration
*/
Configuration::~Configuration(){
	for(unsigned a=0; a<m_count; a++){
		delete m_algos[a]; //created in GetAlgorithm function
	}
	delete[] m_algos; //created in Load function
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
	Creates an configuration
		=> str string with configuration
*/
Configuration::Configuration(string str){
	m_algos = nullptr;
	Load(str);
}
/*
	Returns if own rules are enabled
		<= Are own rules enabled
*/
bool Configuration::AreOwnRules(){
	return m_rules;
}
