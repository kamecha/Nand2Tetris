CC=g++
CFLAGS=-std=c++17 -g
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

Fibonacci:
	cp ../ProgramFlow/FibonacciSeries/FibonacciSeries.vm FibonacciSeries.vm
	./$(TARGET) FibonacciSeries.vm
	rm FibonacciSeries.vm
	cp FibonacciSeries.asm ../ProgramFlow/FibonacciSeries/FibonacciSeries.asm

SimpleFunction:
	cp ../FunctionCalls/SimpleFunction/SimpleFunction.vm SimpleFunction.vm
	./$(TARGET) SimpleFunction.vm
	rm SimpleFunction.vm
	cp SimpleFunction.asm ../FunctionCalls/SimpleFunction/SimpleFunction.asm

FibonacciElement:
	./$(TARGET) ../FunctionCalls/FibonacciElement
	cp FibonacciElement.asm ../FunctionCalls/FibonacciElement/FibonacciElement.asm

NestedCall:
	./$(TARGET) ../FunctionCalls/NestedCall/Sys.vm
	cp Sys.asm ../FunctionCalls/NestedCall/NestedCall.asm

StaticsTest:
	./$(TARGET) ../FunctionCalls/StaticsTest
	cp StaticsTest.asm ../FunctionCalls/StaticsTest/StaticsTest.asm

clean:
	rm *.o *.asm *.vm
