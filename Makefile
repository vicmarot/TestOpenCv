CC = g++
CFLAGS = -g -Wall
SRCS = src/main.cpp
PROG = main
BIN = bin

OPENCV = `pkg-config opencv --cflags --libs`
LIBS = $(OPENCV) -std=c++11

$(PROG):$(SRCS)
	$(CC) $(CFLAGS) -o $(BIN)/$(PROG) $(SRCS) $(LIBS)
clean:
	rm $(BIN)/*

