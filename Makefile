CC=clang
DEPS=

SRCS= src/*.c yungPDF/src/*.c
LIBS= -lxtra

TARGET=parser

all: 
	$(CC) $(SRCS) $(LIBS) -o $(TARGET) 

clean:
	rm parser
