#include "stdmcr.h"
#include <cstring>
#include <cstdio>

char* Add(char* array,const char* text){
	strcpy(array,text);
	return array+strlen(text);
}
char* Append(char* array, const int value){
	char* ptr = array;
	ptr[0]=' ';
	ptr++;
	return ptr+sprintf(ptr,"%d",value);
}

char* Append(char* array, const char* text){
	char* ptr = array;
	ptr[0]=' ';
	ptr++;
	strcpy(ptr,text);
	ptr+=strlen(text);
	return ptr;	
}

