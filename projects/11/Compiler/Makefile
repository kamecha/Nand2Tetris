CC=g++
CFLAGS=-std=c++20 -g
TARGET=JackParser
SRC=main.cpp jackTokenizer.cpp compilationEngine.cpp symbolTable.cpp
OBJS=main.o jackTokenizer.o compilationEngine.o symbolTable.o

$(TARGET):	$(OBJS)
	$(CC) $(CFLAGS) $(OBJS) -o $@

$(OBJS):	$(SRC)
	$(CC) $(CFLAGS) -c $(SRC)

Array:
	make $(TARGET)
	rm *.o
	./$(TARGET) ../../10/ArrayTest/Main.jack

Square:
	make $(TARGET)
	rm *.o
	./$(TARGET) ../../10/Square/Main.jack

.PHONY: clean
clean:
	rm *.o *.xml
