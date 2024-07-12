CC=clang
DEPS=

SRCS=main.c queue.c yungPDF/src/*.c yung_markdown.c map.c
LIBS= -lxtra

TARGET=parser

all: 
	$(CC) $(SRCS) $(LIBS) -o $(TARGET) 

clean:
	rm parser
