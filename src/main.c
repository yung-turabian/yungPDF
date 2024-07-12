#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <string.h>

#include "yung_markdown.h"
#include "../yungPDF/include/yungPDF.h"

#include <xtra/rand.h>
#include "queue.h"

#define point 1/72 //inch

queue *Tokens;
size_t currentReadLine;
size_t currentWriteLine;

queue* tokenize(FILE* file);
int fcounts(FILE *stream);

int main() {
	const char* filename = "Examples/test.md";

	//rng(0, 10);
	//char * str = rands(10);
	//printf("%s\n", str);
	//free(str);

	FILE* file = fopen(filename, "r");
	if(file==NULL) {
		fprintf(stdout, "[io] file can't be opened: %s\n", filename);
		return 1;
	} else {
		fprintf(stdout, "[io] parsing %s\n", filename); 
	}

	
	//Tokens = tokenize(file);
	if(fclose(file) == 0) {
		fprintf(stdout, "[io] %s closed properly ✅\n", filename);
	}
	
	PDF *pdf = pdfInit();



	destroyQueue(&Tokens);
	//pdfDestroy(&pdf);
	return 0;
}

