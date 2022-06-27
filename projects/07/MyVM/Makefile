CC=g++
CFLAGS=-std=c++14 -g
TARGET=VMCompiler
SRC=main.cpp parser.cpp codeWriter.cpp
OBJS=main.o parser.o codeWriter.o

$(TARGET):	$(OBJS)
	$(CC) $(CFLAGS) $(OBJS) -o $@

$(OBJS):	$(SRC)
	$(CC) $(CFLAGS) -c $(SRC)

BasicTest: 
	cp ../MemoryAccess/BasicTest/BasicTest.vm BasicTest.vm
	./$(TARGET) BasicTest.vm
	cp BasicTest.asm ../MemoryAccess/BasicTest/BasicTest.asm

clean:
	rm *.o *.asm
