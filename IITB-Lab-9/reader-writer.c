#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include <string.h>
#include <time.h>


struct read_write_lock
{
	// If required, use this strucure to create
	// reader-writer lock related variables
	int waiting_writer_count;
	int active_reader_count;
	int active_writer_count;
	pthread_mutex_t lock;
	pthread_cond_t writer_cv;
	pthread_cond_t reader_cv;

}rwlock;

struct func_arg{
	int thread_id;
	int delay;
};

long int data = 0;			//	Shared data variable

void InitalizeReadWriteLock(struct read_write_lock * rw)
{
	rw->waiting_writer_count=0;
	rw->active_reader_count=0;
	rw->active_writer_count=0;
	pthread_mutex_init(&rw->lock, NULL);
	pthread_cond_init(&rw->writer_cv, NULL);
	pthread_cond_init(&rw->reader_cv, NULL);
}


// The pthread based reader lock
void ReaderLock(struct read_write_lock * rw)
{
	pthread_mutex_lock(&rw->lock);
	while(rw->waiting_writer_count || rw->active_writer_count){
		pthread_cond_wait(&rw->reader_cv, &rw->lock);
	}
	rw->active_reader_count++;
	pthread_mutex_unlock(&rw->lock);
}	

// The pthread based reader unlock
void ReaderUnlock(struct read_write_lock * rw)
{
	pthread_mutex_lock(&rw->lock);
	rw->active_reader_count--;
	pthread_mutex_unlock(&rw->lock);
	if (!rw->active_reader_count){
		if(rw->waiting_writer_count){
			pthread_cond_signal(&rw->writer_cv);
		}
	}
}

// The pthread based writer lock
void WriterLock(struct read_write_lock * rw)
{
	pthread_mutex_lock(&rw->lock);
	rw->waiting_writer_count++;
	while(rw->active_reader_count || rw->active_writer_count){
		pthread_cond_wait(&rw->writer_cv, &rw->lock);
	}
	rw->waiting_writer_count--;
	rw->active_writer_count++;
	pthread_mutex_unlock(&rw->lock);
}

// The pthread based writer unlock
void WriterUnlock(struct read_write_lock * rw)
{
	pthread_mutex_lock(&rw->lock);
	rw->active_writer_count--;
	pthread_mutex_unlock(&rw->lock);
	if (rw->waiting_writer_count){
		pthread_cond_signal(&rw->writer_cv);
	}
	else{
		pthread_cond_broadcast(&rw->reader_cv);
	}
}

//	Call this function to delay the execution by 'delay' ms
void delay(int delay)
{
	struct timespec wait;

	if(delay <= 0)
		return;

	wait.tv_sec = delay / 1000;
	wait.tv_nsec = (delay % 1000) * 1000 * 1000;
	nanosleep(&wait, NULL);
}

// The pthread reader start function
void *ReaderFunction(void *args)
{
	//	Delay the execution by arrival time specified in the input
	
	//	....
	
	//  Get appropriate lock
	//	Display  thread ID and value of the shared data variable
	//
    //  Add a dummy delay of 1 ms before lock release  
	//	....
	struct func_arg* arg_struct = (struct func_arg *) args;
	sleep(arg_struct->delay);
	ReaderLock(&rwlock);
	printf("Reader %d Value %ld\n", arg_struct->thread_id, data);
	usleep(1000);
	ReaderUnlock(&rwlock);
}

// The pthread writer start function
void *WriterFunction(void *args)
{
	//	Delay the execution by arrival time specified in the input
	
	//	....
	//
	//  Get appropriate lock
	//	Increment the shared data variable
	//	Display  thread ID and value of the shared data variable
	//
    //  Add a dummy delay of 1 ms before lock release  
	//	....
	struct func_arg* arg_struct = (struct func_arg *) args;
	sleep(arg_struct->delay);
	WriterLock(&rwlock);
	data++;
	printf("Writer %d Value %ld\n", arg_struct->thread_id, data);
	usleep(1000);
	WriterUnlock(&rwlock);
}

int main(int argc, char *argv[])
{
	InitalizeReadWriteLock(&rwlock);
	long int reader_number = 0;
	long int writer_number = 0;
	long int thread_number = 0;
	long int total_threads = 0;	
	
	int count = 0;			// Number of 3 tuples in the inputs.	Assume maximum 10 tuples 
	int rws[10];			
	int nthread[10];
	int delay[10];
	//	Verifying number of arguments
	if(argc<4 || (argc-1)%3!=0)
	{
		printf("reader-writer <r/w> <no-of-threads> <thread-arrival-delay in ms> ...\n");		
		printf("Any number of readers/writers can be added with repetitions of above mentioned 3 tuple \n");
		exit(1);
	}
	//	Reading inputs
	for(int i=0; i<(argc-1)/3; i++)
	{
		char rw[2];
		strcpy(rw, argv[(i*3)+1]);

		if(strcmp(rw, "r") == 0 || strcmp(rw, "w") == 0)
		{
			if(strcmp(rw, "r") == 0)
				rws[i] = 1;					// rws[i] = 1, for reader
			else
				rws[i] = 2;					// rws[i] = 2, for writer
			
			nthread[i] = atol(argv[(i*3)+2]);		
			delay[i] = atol(argv[(i*3)+3]);

			count ++;						//	Number of tuples
			total_threads += nthread[i];	//	Total number of threads
		}
		else
		{
			printf("reader-writer <r/w> <no-of-threads> <thread-arrival-delay in ms> ...\n");
			printf("Any number of readers/writers can be added with repetitions of above mentioned 3 tuple \n");
			exit(1);
		}
	}
	pthread_t threads[total_threads];
	struct func_arg args[total_threads];

	int temp=0;
	for(int i=0; i<(argc-1)/3; i++){
		for (int j=0; j<nthread[i]; j++){
			args[temp].thread_id=temp;
			args[temp].delay=delay[i];
			if (rws[i]==1){
				pthread_create(&threads[temp],NULL, ReaderFunction, (void *)&args[temp]);
			}
			else if (rws[i]==2){
				pthread_create(&threads[temp],NULL, WriterFunction, (void *)&args[temp]);
			}
			temp++;
		}
	}
	for (int i=0; i<total_threads; i++){
		pthread_join(threads[i], NULL);
	}
	printf("Final Value is %ld", data);
	//	Create reader/writer threads based on the input and read and write.

	//  Clean up threads on exit

	exit(0);
}
