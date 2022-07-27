CC=g++
CFLAGS=-std=c++14 -g
TARGET=VMCompiler
SRC=main.cpp parser.cpp codeWriter.cpp
OBJS=main.o parser.o codeWriter.o

$(TARGET):	$(OBJS)
	$(CC) $(CFLAGS) $(OBJS) -o $@

$(OBJS):	$(SRC)
	$(CC) $(CFLAGS) -c $(SRC)


BasicLoop:
	cp ../ProgramFlow/BasicLoop/BasicLoop.vm BasicLoop.vm
	./$(TARGET) BasicLoop.vm
	rm BasicLoop.vm
	cp BasicLoop.asm ../ProgramFlow/BasicLoop/BasicLoop.asm

clean:
	rm *.o *.asm *.vm
