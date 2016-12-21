#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>

void* routine(void *i)
{
	sleep(1);
	printf("%d\n", *(int*)i);
	pthread_exit(NULL);
}

int main()
{
	int i = 0;
	int n = 10;

	pthread_t threads[10];
	for (i = 0; i < n; i++)
	{
		int* arg = malloc(sizeof(*arg));
		*arg = i;
		pthread_create(&threads[i], NULL, routine, (void *) arg);
	}
	for (i =0; i < n; i++)
	{
		pthread_join(threads[i], NULL);		// wait for ALL of them to finish before we join them together and return out
	}
	return 0;
}