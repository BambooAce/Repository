CC=gcc
FLAGS = -lpthread
GNU = -std=c99
CPP = pthread0.c thread_print.c pthread_cond.c

all: pthread0 thread_print pthread_cond

pthread0:pthread0.o
	$(CC)  $< $(FLAGS) -o $@

thread_print:thread_print.o
	$(CC)  $< $(FLAGS) -o $@

pthread_cond:pthread_cond.o
	$(CC)  $< $(FLAGS) -o $@

pthread0.o:pthread0.c
	$(CC) -c $(GNU) $< -o $@

pthread_cond.o:pthread_cond.c
	$(CC) -c $(GNU) $< -o $@

thread_print.o:thread_print.c
	$(CC) -c $(GNU) $< -o $@

clean:
	rm *.o
	find . -executable -type f |xargs rm
