CC = g++
FLAGS =  -g -fPIC -fPIE
INCPATH = -I ./ -I /usr/include/qt5/QtCore -I /usr/include/qt5
LIB = $(shell mysql_config --libs)
LIBS = -lssl -lcrypto $(LIB) -lQt5Core
OBJ = db.o iniparser.o streamoper.o ngserver.o mem.o main.o deamon.o machinfo.o gameinfo.o	log.o singleconn.o
BIN = ngserver

$(BIN): $(OBJ)
	$(CC) $(FLAGS) $^ $(LIBS) -o $@
	#@if [ ! -e bin ];then mkdir bin; fi
	#@mv $(BIN) bin/

%.o: %.cpp
	$(CC) -c $(FLAGS) $<  $(INCPATH) -o $@

clean:
	rm -rf *.o

