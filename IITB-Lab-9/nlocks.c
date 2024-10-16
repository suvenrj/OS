#include <pthread.h>
#include <stdio.h>

#define NTHREADS 10

long int counter[10];
pthread_mutex_t locks[10];
int args[10];

//	The thread starter function
void *myThread(void * arg)
{
	int index = *(int *) arg;
	for(int i=0; i<1000; i++){
		pthread_mutex_lock(&locks[index]);
		counter[index]++;
		pthread_mutex_unlock(&locks[index]);
	}
		
}

int main()
{
	// Create space for pthread variables
	pthread_t tid[NTHREADS];
	for (int i=0; i<NTHREADS; i++){
		pthread_mutex_init(&locks[i], NULL);
		counter[i]=0;
		args[i]=i;
	}

	for(int i=0; i<NTHREADS; i++)
	{
		//	Create a thread with default attributes and no arguments
		pthread_create(&tid[i], NULL, myThread, (void *)&args[i]);
	}
	for (int i=0; i<10; i++){
		for (int j=0; j<1000; j++){
			pthread_mutex_lock(&locks[i]);
			counter[i]++;
			pthread_mutex_unlock(&locks[i]);
		}
	}
	for(int i=0; i<NTHREADS; i++)
	{
		//	Wait for all threads to finish
		pthread_join(tid[i], NULL);
	}

	for (int i=0; i<NTHREADS; i++){
		printf("Counter %d: %ld \n",i, counter[i]);
		pthread_mutex_destroy(&locks[i]);
	}

	return 0;
}
