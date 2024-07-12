#ifndef MAP_H
#define MAP_H

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define MAX_SIZE 100

// TODO, no error checking at all
typedef struct {
		size_t size;
		char keys[MAX_SIZE][100];
		char values[MAX_SIZE][100];
} SMap;


SMap* createSMap();

void killSMap(SMap* map);

int getIndex(SMap* map, char* key);

void insert(SMap* map, char* key, char* value);

char* get(SMap* map, char* key);

void printmap(SMap* map);




#endif
