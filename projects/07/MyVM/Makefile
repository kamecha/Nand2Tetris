CC=g++
CFLAGS=-std=c++14 -g
TARGET=Assembler
SRC=main.cpp parser.cpp codeWriter.cpp
OBJS=main.o parser.o codeWriter.o

$(TARGET):	$(OBJS)
	$(CC) $(CFLAGS) $(OBJS) -o $@

$(OBJS):	$(SRC)
	$(CC) $(CFLAGS) -c $(SRC)

clean:
	rm *.o
