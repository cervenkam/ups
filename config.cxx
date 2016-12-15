/*
	Class reprezenting game configuration
	Singleton
	Author: Martin Cervenka
	Version: 19.04.2016
*/
#include "stdmcr.h"
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
		m_persons++;
		return new Person(player_name,id); //deleted in destuctor (1/6)
	}else CMP(PROGRAMMERBOT,"ProgrammerBot"){
		return new ProgrammerBot(player_name,id); //deleted in destuctor (1/6)
	}else CMP(NETWORKPLAYER,"NetworkPlayer"){
		m_at_least_one_player = true;
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
	char* command = new char[512]; //deleted at the end of this function
	char* parameter = new char[512]; //deleted at the end of this function
	unsigned counter = 0;
	ReadLines(in,counter,command,parameter);
	if(m_algos){
		for(unsigned a=counter; a<m_count; a++){
			char* name = new char[512]; //deleted at the end of this function (1/n) || in Algorithm destructor ((n-1)/n)
			strcpy(name,PLAYER);
			m_algos[a] = GetAlgorithm(ms_def,name,a);
		}
	}else{
		m_valid = false;
	}
	delete[] parameter; //created at the start of this function
	delete[] command; //created at the start of this function
}
void Configuration::ReadLines(istringstream& in, unsigned& counter,char* command,char* parameter){
	char* name = new char[512]; //deleted at the end of this function (1/n) || in Algorithm destructor ((n-1)/n)
	char* str = new char[512]; //deleted at the end of this function
	char* strtmp = str;
	while(in.getline(str,512)){
		while(sscanf(strtmp,"%511[^=]=%511s",command,parameter)){
			if(!ParseOneParameter(counter,strtmp,name,command,parameter)){
				break;
			}
		}
	}
	delete[] str; //created at the start of this function
	delete[] name; //created at the start of this function (1/n) || in ParseOneParameter function ((n-1)/n)
}
void Configuration::InitAlgorithms(unsigned count){
	SetCount(count);	
	if(m_algos == nullptr){
		m_algos = new Algorithm*[GetCount()]; //deleted in destructor
	}
}
void Configuration::AddAlgorithm(unsigned& counter,char*& name, char* parameter){
	if(m_algos != nullptr && counter < GetCount()){
		m_algos[counter] = GetAlgorithm(parameter,name,counter);
		name = new char[512]; //deleted in Algorithm destuctor ((n-1)/n) || in caller function (1/n)
		strcpy(name,PLAYER);
		counter++;
	}
}
bool Configuration::ParseOneParameter(unsigned& counter,char*& strtmp,char*& name,char* command,char* parameter){
	if(!strcmp(command,PLAYER_COUNT)){
		InitAlgorithms(atoi(parameter));
	}else if(!strcmp(command,DEFAULT) && parameter){
		ms_def = parameter;
	}else if(!strcmp(command,PLAYER) && parameter){
		AddAlgorithm(counter,name,parameter);
	}else if(!strcmp(command,NAME) && parameter){
		strcpy(name,parameter);
	}else if(!strcmp(command,RULES)){
		m_rules=true;
	}
	return UpdatePointer(strtmp);
}
bool Configuration::UpdatePointer(char*& strtmp){
	while(strtmp && strtmp[0]!=' ' && strtmp[0]!='\0'){
		strtmp++;
	}
	if(strtmp && strtmp[0]=='\0'){
		return false;
	}
	if(strtmp){
		strtmp++;
	}
	return true;
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

bool Configuration::IsValid(){
	return m_valid && m_at_least_one_player && (m_persons < 2) && (m_count > 1) && (m_count < 8);
}
