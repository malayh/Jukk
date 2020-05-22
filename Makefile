


CC      := g++
CFLAGS  := -g -MMD -MP -Wall -Wextra
LDFLAGS := -lpthread
LIBS    :=

SHELL := bash
RM    := rm

MFDIR    := $(dir $(realpath $(firstword $(MAKEFILE_LIST))))
BINDIR   := $(MFDIR)/bin
SRCDIR   := $(MFDIR)/src
OBJDIR   := $(MFDIR)/obj
INCDIR   := $(SRCDIR)/include

INCLUDE  := $(addprefix -I, $(INCDIR))


all: bin/test bin/server bin/client


# Objects
obj/protocol.o: src/protocol.cpp src/include/protocol.h
	g++ $(CFLAGS) $(INCLUDE) -c src/protocol.cpp -o obj/protocol.o

obj/client.o: src/client.cpp
	g++ $(CFLAGS) $(INCLUDE) -c src/client.cpp -o obj/client.o

obj/server.o: src/server.cpp
	g++ $(CFLAGS) $(INCLUDE) -c src/server.cpp -o obj/server.o

obj/test.o: src/test.cpp
	g++ $(CFLAGS) $(INCLUDE) -c src/test.cpp -o obj/test.o

obj/util.o: src/util.cpp src/include/util.h
	g++ $(CFLAGS) $(INCLUDE) -c src/util.cpp -o obj/util.o

#exes
bin/test: obj/test.o obj/util.o
	g++ obj/test.o obj/util.o -o bin/test

bin/client: obj/client.o obj/util.o obj/protocol.o
	g++ obj/util.o obj/protocol.o obj/client.o $(LDFLAGS) -o bin/client

bin/server: obj/server.o obj/protocol.o
	g++ obj/protocol.o obj/server.o  $(LDFLAGS) -o bin/server

clean:
	rm bin/*
	rm obj/*