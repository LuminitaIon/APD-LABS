#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

#define NUM_THREADS 2

int a = 0;
pthread_mutex_t mutex; //declaram mutex-ul

void *f(void *arg)
{
	
	int r;
	r = pthread_mutex_lock(&mutex);
	if(r) {
		printf("Nu se poate face lock\n");
	}

	a += 2; // zona asta e critica
	
	r = pthread_mutex_unlock(&mutex);
	if(r) {
		printf("Nu se poate face unlock\n");
	}

	pthread_exit(NULL);
}

int main(int argc, char *argv[])
{
	int i, r;
	void *status;
	pthread_t threads[NUM_THREADS];
	int arguments[NUM_THREADS];

	pthread_mutex_init(&mutex, NULL); // initializam mutex-ul

	for (i = 0; i < NUM_THREADS; i++) {
		arguments[i] = i;
		r = pthread_create(&threads[i], NULL, f, &arguments[i]);

		if (r) {
			printf("Eroare la crearea thread-ului %d\n", i);
			exit(-1);
		}
	}

	for (i = 0; i < NUM_THREADS; i++) {
		r = pthread_join(threads[i], &status);

		if (r) {
			printf("Eroare la asteptarea thread-ului %d\n", i);
			exit(-1);
		}
	}

	pthread_mutex_destroy(&mutex); //il distrugem

	printf("a = %d\n", a);

	return 0;
}
