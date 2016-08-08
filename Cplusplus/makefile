#
# Makefile asignment $(SRC)
# for shell should $${SRC}
#
CC=g++
SRC=constructor.cpp\
	container.cpp\
	default.cpp\
	Test.cpp
all:
	@for src in $(SRC); do \
		$(CC) $$src -o $${src%.*}; \
	done
