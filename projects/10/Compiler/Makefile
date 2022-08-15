CC=g++
CFLAGS=-std=c++17 -g
TARGET=JackParser
SRC=main.cpp jackTokenizer.cpp
OBJS=main.o jackTokenizer.o

$(TARGET):	$(OBJS)
	$(CC) $(CFLAGS) $(OBJS) -o $@

$(OBJS):	$(SRC)
	$(CC) $(CFLAGS) -c $(SRC)

Square:
	./$(TARGET) ../Square/Square.jack
	./$(TARGET) ../Square/SquareGame.jack
	./$(TARGET) ../Square/Main.jack

SquareTest:
	../../../tools/TextComparer.sh ../Square/SquareT.xml ./SquareT.xml
	../../../tools/TextComparer.sh ../Square/SquareGameT.xml ./SquareGameT.xml
	../../../tools/TextComparer.sh ../Square/MainT.xml ./MainT.xml

ExpressionLessSquare:
	./$(TARGET) ../ExpressionLessSquare/Square.jack

clean:
	rm *.o *.xml
