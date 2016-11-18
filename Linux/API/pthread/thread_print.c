#include <stdio.h>
#include <unistd.h>
#include <pthread.h>
static int counter;

void *func(void * arg)
{
	pthread_mutex_t *mutex = (pthread_mutex_t *)arg;
	for(int i = 0; i < 50000; ++i)
	{
		pthread_mutex_lock(mutex);
		counter++;
		pthread_mutex_unlock(mutex);
	}
	return 0;
}

int main()
{	
	pthread_mutex_t mutex;
	pthread_mutex_init(&mutex, NULL);
	pthread_t pid1, pid2;
	pthread_create(&pid1, NULL, func, (void*)&mutex);
	pthread_create(&pid2, NULL, func, (void*)&mutex);
	pthread_join(pid1, NULL);
	pthread_join(pid2, NULL);
	pthread_mutex_destroy(&mutex);
	printf("%d/n",counter);
	return 0;
}
