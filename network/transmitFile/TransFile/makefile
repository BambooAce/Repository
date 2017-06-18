CC = g++
FLAGS = -Wall -g
INCPATH = -I ./
LIBS = -lpthread -lcrypto
OBJS = msgitem.o fileclient.o handlestream.o
BIN = client server

all: $(BIN)

client: $(OBJS) client.o
	$(CC) $^ $(LIBS) -o $@

server: $(OBJS) server.o
	$(CC) $^ $(LIBS) -o $@

%.o: %.cpp
	$(CC) $(FLAGS) $(INCPATH) -c $< -o $@

clean:
	rm -rf *.o
