CC=g++
CFLAGS=-std=c++14 -g
TARGET=VMCompiler
SRC=main.cpp parser.cpp codeWriter.cpp
OBJS=main.o parser.o codeWriter.o

$(TARGET):	$(OBJS)
	$(CC) $(CFLAGS) $(OBJS) -o $@

$(OBJS):	$(SRC)
	$(CC) $(CFLAGS) -c $(SRC)

SimpleAdd:
	cp ../StackArithmetic/SimpleAdd/SimpleAdd.vm SimpleAdd.vm
	./$(TARGET) SimpleAdd.vm
	cp SimpleAdd.asm ../StackArithmetic/SimpleAdd/SimpleAdd.asm

StackTest:
	cp ../StackArithmetic/StackTest/StackTest.vm StackTest.vm
	./$(TARGET) StackTest.vm
	cp StackTest.asm ../StackArithmetic/StackTest/StackTest.asm

BasicTest: 
	cp ../MemoryAccess/BasicTest/BasicTest.vm BasicTest.vm
	./$(TARGET) BasicTest.vm
	cp BasicTest.asm ../MemoryAccess/BasicTest/BasicTest.asm

PointerTest: 
	cp ../MemoryAccess/PointerTest/PointerTest.vm PointerTest.vm
	./$(TARGET) PointerTest.vm
	cp PointerTest.asm ../MemoryAccess/PointerTest/PointerTest.asm

StaticTest:
	cp ../MemoryAccess/StaticTest/StaticTest.vm StaticTest.vm
	./$(TARGET) StaticTest.vm
	cp StaticTest.asm ../MemoryAccess/StaticTest/StaticTest.asm

clean:
	rm *.o *.asm
