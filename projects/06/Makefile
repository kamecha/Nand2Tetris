CC=g++
CFLAGS=-std=c++14 -g
TARGET=Assembler
SRC=assembler.cpp parser.cpp code.cpp symbolTable.cpp
OBJS=assembler.o parser.o code.o symbolTable.o

$(TARGET):	$(OBJS)
	$(CC) $(CFLAGS) $(OBJS) -o $@

$(OBJS):	$(SRC)
	$(CC) $(CFLAGS) -c $(SRC)

clean:
	rm *.o
