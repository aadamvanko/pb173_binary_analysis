//  usage:
//    gcc -o thread threads.c
//    ./thread time
//    time is the number of seconds

#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <unistd.h>
#include <stdint.h>

#define NUM_THREADS 2
#define SLEEP_TIME_MS 16

uint64_t lock;

extern void spin_lock(uint64_t* addr);
extern void spin_unlock(uint64_t* addr);

void *print1(void *argument)
{
	while (true)
	{
		spin_lock(&lock);
		printf("1 BEGIN\n");
		usleep(SLEEP_TIME_MS * 1000 + 1234);	
		printf("1 END\n");
		spin_unlock(&lock);	
	}
}

void *print2(void *argument)
{
	while (true)
	{
		spin_lock(&lock);
		printf("2 BEGIN\n");
		usleep(SLEEP_TIME_MS * 1000);
		printf("2 END\n");
		spin_unlock(&lock);
	}
}

int main(int argc, char *argv[])
{
	pthread_t threads[NUM_THREADS];
	int thread_args[NUM_THREADS];
	int rc1, rc2, rc3, i;

	/* setup all arguments */
	for (i=0; i<NUM_THREADS; ++i)
	{
		thread_args[i] = i;
		printf("In main: creating thread %d\n", i);
	}

	/* create all threads */
	rc1 = pthread_create(&threads[0], NULL, print1, NULL);
	rc2 = pthread_create(&threads[1], NULL, print2, NULL);

	/* wait for all threads to complete */
	rc1 = pthread_join(threads[0], NULL);
	rc2 = pthread_join(threads[1], NULL);
	for (i=0; i<NUM_THREADS; ++i)
	{
		thread_args[i] = i;
		printf("In main: destroing thread %d\n", i);
	}
	exit(EXIT_SUCCESS);
}
