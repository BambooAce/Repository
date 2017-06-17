CC = g++
FLAGS = -Wall
INCPATH = -I ./
OBJS = msgitem.o fileclient.o handlestream.o
BIN = client server

all: $(BIN)

client: $(OBJS) client.o
	$(CC) $(FLAGS) -o $@ $<

server: $(OBJS) server.o
	$(CC) $(FLAGS) -o $@ $<

%.o: %.cpp
	$(CC) $(FLAGS) $(INCPATH) -c $< -o $@

clean:
	rm -rf *.o
