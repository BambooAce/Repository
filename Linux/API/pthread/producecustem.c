#include <fcntl.h>
#include <sys/stat.h>
#include <semaphore.h>
#include <pthread.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#define BUFSIZE 7
#define PRONUM 7
#define CUSNUM 7
typedef struct{
    char buff[BUFSIZE];
    sem_t produce;
    sem_t custemer;
}ARG;
void * producefunc(void *arg);
void * custemfunc(void *arg);
int main()
{
    ARG mem;
    memset(&mem.buff, 0, BUFSIZE);
    sem_init(&mem.produce, 0, BUFSIZE);
    sem_init(&mem.custemer, 0, 0);
    int m, n;
    pthread_setconcurrency(PRONUM + CUSNUM + 1);
    pthread_t proarr[PRONUM];
    pthread_t cusarr[CUSNUM];
    for(m = 0; m < PRONUM; ++m)
    {
        pthread_create(&proarr[m], NULL, producefunc, &mem);
    }
    for(n = 0; n < CUSNUM; ++n)
    {
       pthread_create(&cusarr[n], NULL, custemfunc, &mem);
    }
    for(m = 0; m < PRONUM; ++m)
    {
        pthread_join(proarr[m], NULL);
    }
    for(n = 0; n < CUSNUM; ++n)
    {
       pthread_join(cusarr[n], NULL);
    }
    sem_destroy(&mem.custemer);
    sem_destroy(&mem.produce);
    return 0;
}

void * producefunc(void *arg)
{
    ARG *buf = (ARG *)arg;
    pthread_t me = pthread_self();
    int i;
    while(1){
        sem_wait(&(buf->produce));
        for(i = 0; i < BUFSIZE; ++i)
        {
            if(buf->buff[i] == 0)
            {
                printf("Producer thread %ld put A\n", me);
                buf->buff[i] = 'A';
                break;
            }
        }
        sem_post(&(buf->custemer));
        sleep(2);
    }

}

void * custemfunc(void *arg)
{
    ARG *buf = (ARG *)arg;
    pthread_t me = pthread_self();
    int i;
    while(1){
        sem_wait(&(buf->custemer));
        for(i = 0; i < BUFSIZE; ++i)
        {
            if(buf->buff[i] == 'A')
            {
                printf("Custemer thread %ld get A\n", me);
                buf->buff[i] = 0;
                break;
            }
        }
        sem_post(&(buf->produce));
        sleep(1);
    }
}

