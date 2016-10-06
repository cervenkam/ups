/*
	Class reprezenting game configuration
	Singleton
	Author: Martin Cervenka
	Version: 19.04.2016
*/
#include "config.h"
#include "lang.h"
#include "stdmcr.h"
#include <fstream>
#include "algorithm.h"
#include "person.h"
#include "algono.h"
#include "programmerbot.h"
#include "networkplayer.h"
#include <string.h>
#include <sstream>
#include <iostream>
#define CMP(MCR,NAME) if(!strcmp(name,MCR) || !strcmp(name,NAME))

using namespace std;
/* default algorithm name */
char* Configuration::def = new char[14]{'P','r','o','g','r','a','m','m','e','r','B','o','t','\0'};
/* single instance of this class */
Configuration* Configuration::config = NULL;

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
	if(name!=def){
		return GetAlgorithm(def,player_name,id);
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
	char str[512];
	char* command = new char[512];
	char* parameter = new char[512];
	char* name = new char[512];
	unsigned counter = 0;
	while(in.getline(str,512)){
		sscanf(str,"%511[^=]=%511s",command,parameter);
		if(!strcmp(command,PLAYER_COUNT)){
			SetCount(atoi(parameter));	
			algos = new Algorithm*[GetCount()];
		}else if(!strcmp(command,DEFAULT)){
			def = parameter;
		}else if(!strcmp(command,PLAYER)){
			algos[counter] = GetAlgorithm(parameter,name,counter);
			name = new char[512];
			counter++;
		}else if(!strcmp(command,NAME)){
			strcpy(name,parameter);
		}else if(!strcmp(command,RULES)){
			rules=true;
		}
	}
	for(unsigned a=counter; a<count; a++){
		algos[a] = GetAlgorithm(def,name,a);
	}
	delete[] command;
}
/*
	Sets count of players
		=> count Count of players
*/
void Configuration::SetCount(unsigned count){
	this->count = count;
}
/*
	Returns count of players
		<= Count of players
*/
unsigned Configuration::GetCount(){
	return count;
}
/*
	Returns all (array of) created algorithms
		<= Array of created algorithms
*/
Algorithm** Configuration::GetAlgorithms(){
	return algos;
}
/*
	Returns single configuration
		=> Configuration string / NULL if already created
		<= Configuration
*/
Configuration* Configuration::GetConfiguration(string str){
	if(config==NULL){
		cout << "Creating configuration by: " << endl << "\t#" << str << "#" << endl;
		config = new Configuration(str);
	}
	return config;
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
	return rules;
}
