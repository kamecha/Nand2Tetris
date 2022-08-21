CC=g++
CFLAGS=-std=c++17 -g
TARGET=JackParser
SRC=main.cpp jackTokenizer.cpp compilationEngine.cpp
OBJS=main.o jackTokenizer.o compilationEngine.o

$(TARGET):	$(OBJS)
	$(CC) $(CFLAGS) $(OBJS) -o $@

$(OBJS):	$(SRC)
	$(CC) $(CFLAGS) -c $(SRC)

ExpressionLessSquare:
	./$(TARGET) ../ExpressionLessSquare/Main.jack
	./$(TARGET) ../ExpressionLessSquare/Square.jack
	./$(TARGET) ../ExpressionLessSquare/SquareGame.jack

Square:
	./$(TARGET) ../Square/Main.jack
	./$(TARGET) ../Square/Square.jack
	./$(TARGET) ../Square/SquareGame.jack

Array:
	./$(TARGET)	../ArrayTest/Main.jack

ExpressionLessSquareTest:
	make ExpressionLessSquare
	../../../tools/TextComparer.sh ../ExpressionLessSquare/Main.xml ./Main.xml
	../../../tools/TextComparer.sh ../ExpressionLessSquare/Square.xml ./Square.xml
	../../../tools/TextComparer.sh ../ExpressionLessSquare/SquareGame.xml ./SquareGame.xml

SquareTest:
	make Square
	../../../tools/TextComparer.sh ../Square/Main.xml ./Main.xml
	../../../tools/TextComparer.sh ../Square/Square.xml ./Square.xml
	../../../tools/TextComparer.sh ../Square/SquareGame.xml ./SquareGame.xml

ArrayTest:
	make Array
	../../../tools/TextComparer.sh ../ArrayTest/Main.xml ./Main.xml

.PHONY: clean
clean:
	rm *.o *.xml
