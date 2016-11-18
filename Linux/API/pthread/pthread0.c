#include <pthread.h>
#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
struct Arg
{
    int num;
    double sale;
    char *str;
};

void * func(void *args)
{
	struct Arg * perm = (struct Arg *)args;
	perm->num = 666;
	perm->sale = 66.66;
	bcopy("Hello world", perm->str, 24);
	pthread_exit(perm);
}

int main()
{
    pthread_t thread1;
	struct Arg *get = NULL;
    struct Arg args;
    args.num = 100;
    args.sale = 99.99;
    args.str = (char *)malloc(1024);
    pthread_create(&thread1, NULL, func, (void *)&args);
	pthread_join(thread1, (void **)&get);
	printf("%d, %lf, %s\n",get->num, get->sale, get->str);
    return 0;
}
