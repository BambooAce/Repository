CC=g++
RM=rm -rf
OPT=-D LINUX
BIN=client server

all:$(BIN)

client:Csocket.cpp client.cpp
	$(CC) $(OPT) $^ -o $@
	strip $@

server:Csocket.cpp server.cpp
	$(CC) $(OPT) $^ -o $@
	strip $@

clean:
	$(RM) $(BIN)
