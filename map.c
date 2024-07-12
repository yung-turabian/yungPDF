#include "map.h"
SMap* createSMap() {
		SMap* map;
		map = malloc(sizeof(SMap));

		return map;
}

void killSMap(SMap* map) {
		free(map);
}

int getIndex(SMap* map, char* key)
{
		int i;
		for(i=0;i<map->size;i++)
		{
				if(strcmp(map->keys[i], key) == 0) {
						return i;
				}
		}
		return -1; //not found
}

void insert(SMap* map, char* key, char* value) 
{
		int index = getIndex(map, key);
		if(index == -1) {
				strcpy(map->keys[map->size], key);
				strcpy(map->values[map->size++], value);
		} else { //key found
				strcpy(map->values[index], value);
		}
}

char* get(SMap* map, char* key) 
{
		int index = getIndex(map, key);
		if(index == -1) return NULL;
		else return map->values[index];
}

void printmap(SMap* map) 
{
		int i;
		for(i=0;i<map->size;i++) {
				fprintf(stdout, "%s: %s\n", map->keys[i], map->values[i]);
		}
}

