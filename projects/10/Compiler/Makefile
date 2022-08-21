CC=g++
CFLAGS=-std=c++17 -g
TARGET=JackParser
SRC=main.cpp jackTokenizer.cpp compilationEngine.cpp
OBJS=main.o jackTokenizer.o compilationEngine.o

$(TARGET):	$(OBJS)
	$(CC) $(CFLAGS) $(OBJS) -o $@

$(OBJS):	$(SRC)
	$(CC) $(CFLAGS) -c $(SRC)

Square:
	./$(TARGET) ../Square/Square.jack
	./$(TARGET) ../Square/SquareGame.jack
	./$(TARGET) ../Square/Main.jack

ExpressionLessSquare:
	./$(TARGET) ../ExpressionLessSquare/Square.jack

Array:
	./$(TARGET)	../ArrayTest/Main.jack

SquareTest:
	make Square
	../../../tools/TextComparer.sh ../Square/SquareT.xml ./SquareT.xml
	../../../tools/TextComparer.sh ../Square/SquareGameT.xml ./SquareGameT.xml
	../../../tools/TextComparer.sh ../Square/MainT.xml ./MainT.xml

ArrayTest:
	make clean
	make Array
	../../../tools/TextComparer.sh ../ArrayTest/MainT.xml ./MainT.xml

.PHONY: clean
clean:
	rm *.o *.xml
