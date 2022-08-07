CC=g++
CFLAGS=-std=c++17 -g
TARGET=JackParser
SRC=main.cpp jackTokenizer.cpp
OBJS=main.o jackTokenizer.o

$(TARGET):	$(OBJS)
	$(CC) $(CFLAGS) $(OBJS) -o $@

$(OBJS):	$(SRC)
	$(CC) $(CFLAGS) -c $(SRC)

ExpressionLessSquare:
	./$(TARGET) ../ExpressionLessSquare/Square.jack

clean:
	rm *.o
