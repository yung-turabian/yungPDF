#include "yung_markdown.h"

#define MAX_BUFFER_SIZE 2048
#define MAX_STRING_SIZE 32

const char * const token_str[] = {
	[HEADING] = "HEADING",
	[SUBHEADING] = "SUBHEADING",
	[SUBSUBHEADING] = "SUBSUBHEADING",
	[PLAIN_TEXT] = "PLAINTEXT",
	[BOLD] = "BOLD",
	[ITALIC] = "ITALIC",
	[INDENT] = "INDENT",
	[BLANK_LINE] = "BLANK_LINE",
	[SPACE] = "SPACE",
	[NEW_LINE] = "NEW_LINE",
};

/*
 *	TODO fix column number read, bold and italic messes it up
 *		- break up function
 *
 *
 *
 */
queue* tokenize(FILE* file) {
	queue* tokens = createQueue(sizeof(Token));
	char line[MAX_BUFFER_SIZE];
	char str[MAX_STRING_SIZE];

	fseek(file, 0, SEEK_END);
	long f_size = ftell(file);
	fseek(file, 0, SEEK_SET);
	fprintf(stdout, "[tok] file size= %ld\n", f_size);
	
	size_t lineNum = 1;
	
	size_t lineLen = fcounts(file);

	 while(fgets(line, sizeof(line), file)){
		
		char* p = line;
		size_t colNum = 1;
		
		//printf("[tok_debug] line length= %d\n", lineLen);
		
		while(*p) {
			Token token;
			memset(&token, 0, sizeof(Token));
			token.line = lineNum;
			token.col = colNum;

			if(*p == '#') {
				int numHash = 0;
				while(p[numHash] == '#' && numHash < strlen(p)) {
					++numHash;
				}
				
				char* headingText = p + numHash;
				while (*headingText == ' ') ++headingText;
				
				strncpy(token.value, headingText, sizeof(token.value) - 1);
				token.value[strcspn(token.value, "\n")] = 0; 			
				
				switch (numHash) {
					case 1:
							token.type = HEADING;
							break;
					case 2:
							token.type = SUBHEADING;
							break;
					case 3:
							token.type = SUBSUBHEADING;
							break;
					default:
							token.type = PLAIN_TEXT;
				}
				enqueue(tokens, &token);
				p += strlen(p); //end of line
			}
			else if(*p == '\n' && lineLen == 1) {
				token.type = BLANK_LINE;
				token.line = lineNum;
				token.col = colNum;
				enqueue(tokens, &token);
				p++;
			}
			else if(*p == '*' && *(p+1) == '*') {
				char* start = p + 2;
				char* end = strstr(start, "**");

				if(end) {
					strncpy(token.value, start, end - start);
					token.value[end-start] = '\0';
					token.type = BOLD;
					colNum += end - start;
					enqueue(tokens, &token);
					p = end + 2; // past closing **
				} else {
					break; // no closing found
				}
			}
			else if (*p == '*') {
				
				char* start = p + 1;
				char* end = strstr(start, "*");

				if(end) {
					strncpy(token.value, start, end - start);
					token.value[end-start] = '\0';
					token.type = ITALIC;
					colNum += 2;
					enqueue(tokens, &token);
					p = end + 1; // past closing **
				} else {
					break; // no closing found
				}

			}
			else if (*p == ' ' && *(p+1) == ' ' && *(p+2) == ' ') {
				char* start = p;
				while(*p && isspace(*p) && *p != '#' && *p != '*' && *p != '\n' && *p != '\t') {
					p++;
					colNum++;
				}
				int len = p - start;
				strncpy(token.value, start, len);
				token.value[len] = '\0';
				token.type = INDENT;
				enqueue(tokens, &token);
			}
			else if (*p == ' ') {
				token.type = SPACE;
				p++;
				colNum++;
			}
			else if (*p == '\n') {
				token.type = NEW_LINE;
				p++;
			}
			else {
				char* start = p;
				while(*p && !isspace(*p) && *p != '#' && *p != '*' && *p != '\n' && *p != '\t') {
					p++;
					colNum++;
				}
				int len = p - start;
				strncpy(token.value, start, len);
				token.value[len] = '\0';
				token.type = PLAIN_TEXT;
				token.col = colNum - len;
							
				enqueue(tokens, &token);
			}
			fprintf(stdout, "[~tok @%zu:%zu] enqueue %s: %s\n", token.line, token.col, token_str[token.type],token.value);

			colNum++;

		}
		
		lineLen = fcounts(file);

		lineNum++;


	}

	fprintf(stdout, "[tok] finished ✅\n");

	return tokens;
}
