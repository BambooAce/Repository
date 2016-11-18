#include <unistd.h>
#include <pthread.h>
#include <stdio.h>

pthread_cond_t S1 = PTHREAD_COND_INITIALIZER;
pthread_cond_t S2 = PTHREAD_COND_INITIALIZER;
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
/**
   一个线程可以调用pthread_cond_wait在一个Condition Variable上阻塞等待，这个函数做以下三步操作：
       释放Mutex
	   阻塞等待
	当被唤醒时，重新获得Mutex并返回
**/
void* driver(void *arg)
{
	arg = NULL;
	while(1)
	{
		pthread_cond_wait(&S1, &mutex);
		printf("Start run....\n");
		printf("Running....\n");
		printf("Stop....\n");
		pthread_cond_signal(&S2);
	}
}

void* saler(void *arg)
{
	arg = NULL;
	while(1)
	{
		printf("Sale ticket.....\n");
		printf("Close the door......\n");
		pthread_cond_signal(&S1);
		pthread_cond_wait(&S2, &mutex);
		printf("Open the door.......\n");
		printf("People down......\n");
		printf("Close the door.....\n");
		pthread_cond_signal(&S1);
	}
}

int main()
{
//	pthread_cond_init();
	pthread_t driverman, salerman;
	pthread_create(&driverman, NULL, driver, NULL);
	pthread_create(&salerman, NULL, saler, NULL);
	pthread_join(driverman, NULL);
	pthread_join(salerman, NULL);
//	pthread_cond_destroy();
	return 0;
}

