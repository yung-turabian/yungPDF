#ifndef YUNGMARKDOWN_H
#define YUNGMARKDOWN_H

#include <string.h>
#include <ctype.h>
#include <xtra/io.h>

#include "queue.h"

typedef enum {
	// Single-character tokens.
	HEADING, SUBHEADING, SUBSUBHEADING, BLOCKQUOTE,
	
	BOLD, ITALIC,

	PLAIN_TEXT,
	// formating
	INDENT, BLANK_LINE, SPACE, NEW_LINE,

	GREATER_THAN,
	ORDERED_LIST_START,
	ORDERED_LIST_END,
	UNORDERED_LIST_START,
	UNORDERED_LIST_END,
	CODE_START,
	CODE_END,
	HORIZONTAL_RULE,
	LINK_START,
	LINK_END,
	IMAGE_START,
	IMAGE_END,
} TOKEN_TYPE;

typedef struct {
	char value[128];
	TOKEN_TYPE type;
	size_t line;
	size_t col;
} Token;


queue* tokenize(FILE* file);

#endif
