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

}

int main()
{
    pthread_t thread1;
    struct Arg args;
    args.num = 100;
    args.sale = 99.99;
    args.str = (char *)malloc(1024);
    pthread_create(&thread1, NULL, func, (void *)&args);

    return 0;
}
