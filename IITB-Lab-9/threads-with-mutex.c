#include <pthread.h>
#include <stdio.h>

#define NTHREADS 100

long int counter = 0;

pthread_mutex_t mymutex;

//	The thread starter function
void *myThread()
{
	for(int i=0; i<1000; i++){
        pthread_mutex_lock(&mymutex);
		counter++;
        pthread_mutex_unlock(&mymutex);
    }
}

int main()
{
	// Create space for pthread variables
	pthread_t tid[NTHREADS];
    pthread_mutex_init(&mymutex, NULL);
	for(int i=0; i<NTHREADS; i++)
	{
		//	Create a thread with default attributes and no arguments
		pthread_create(&tid[i], NULL, myThread, NULL);
	}

	for(int i=0; i<NTHREADS; i++)
	{
		//	Wait for all threads to finish
		pthread_join(tid[i], NULL);
	}

	printf("Counter: %ld \n", counter);
    pthread_mutex_destroy(&mymutex);

	return 0;
}
