CC=g++
CFLAGS=-std=c++17 -g
TARGET=JackParser
SRC=main.cpp jackTokenizer.cpp compilationEngine.cpp
OBJS=main.o jackTokenizer.o compilationEngine.o

$(TARGET):	$(OBJS)
	$(CC) $(CFLAGS) $(OBJS) -o $@

$(OBJS):	$(SRC)
	$(CC) $(CFLAGS) -c $(SRC)

.PHONY: clean
clean:
	rm *.o *.xml
