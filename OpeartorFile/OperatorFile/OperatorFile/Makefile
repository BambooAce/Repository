CC=g++
RM=rm -rf
OPT=-D LINUX
BIN=client server
CS=./Csocket/Csocket.cpp
all:$(BIN)

client:$(CS) client.cpp
	$(CC) $(OPT) $^ -o $@
	strip $@

server:$(CS) server.cpp
	$(CC) $(OPT) $^ -o $@
	strip $@

clean:
	$(RM) $(BIN)
