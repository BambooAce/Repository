CC=gcc
ARG=--std=c99
SRC=selectclient.c selectserver.c
all:
	for src in $(SRC);do\
			${CC} $(ARG) $$src -o $${src%.*};\
			done
clean:
	rm selectclient	selectserver
