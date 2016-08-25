#
# Makefile asignment $(SRC)
# for shell should $${SRC}
#
CC=g++
RM=rm -rf
SRC=constructor.cpp\
	container.cpp\
	default.cpp\
	Test.cpp\
	recover.cpp
all:
	@for src in $(SRC); do \
		$(CC) $$src -o $${src%.*}; \
	done

clean:
	@for src in $(SRC); do \
		$(RM)  $${src%.*}; \
	done
