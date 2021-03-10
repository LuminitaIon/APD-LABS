#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define NUM_THREADS 10
#define NUM_HELLOS 100

void *f(void *arg) {
  	long id = (long)arg;
  	printf("Hello World din thread-ul %ld!\n", id);
  	pthread_exit(NULL);
}

void *g(void *arg) {
  	long id = (long)arg;
  	printf("SALUT din thread-ul %ld!\n", id);
  	pthread_exit(NULL);
}
void *f_100(void *arg) {
	long thread_id = (long)arg;
	
	for (int hello_id = 0; hello_id < NUM_HELLOS; hello_id++) {
		printf("Hello World (%02d) din thread-ul %ld!\n", hello_id, thread_id);
	}

	pthread_exit(NULL);
}

void ex_2() {
  	int r;
	long cores = sysconf(_SC_NPROCESSORS_CONF);
	
	printf("%ld cores detected.\n\n", cores);

	pthread_t threads[cores];
	for (long id = 0; id < cores; id++) {
		r = pthread_create(&threads[id], NULL, f, (void *)id);

		if (r) {
	  		printf("Eroare la crearea thread-ului %ld\n", id);
	  		exit(-1);
		}
  	}

  	for (long id = 0; id < cores; id++) {
  		void *status;
		r = pthread_join(threads[id], &status);

		if (r) {
	  		printf("Eroare la asteptarea thread-ului %ld\n", id);
	  		exit(-1);
		}
  	}
}

void ex_3() {
	int r;
	long cores = sysconf(_SC_NPROCESSORS_CONF);
	
	printf("%ld cores detected.\n\n", cores);

	pthread_t threads[cores];
	for (long id = 0; id < cores; id++) {
		r = pthread_create(&threads[id], NULL, f_100, (void *)id);

		if (r) {
	  		printf("Eroare la crearea thread-ului %ld\n", id);
	  		exit(-1);
		}
  	}

  	for (long id = 0; id < cores; id++) {
  		void *status;
		r = pthread_join(threads[id], &status);

		if (r) {
	  		printf("Eroare la asteptarea thread-ului %ld\n", id);
	  		exit(-1);
		}
  	}
}

void ex_4() {
	pthread_t thread1, thread2;

	int r = pthread_create(&thread1, NULL, f, (void *)1);
	if (r) {
		printf("Eroare la asteptarea thread-ului %d\n", 1);
		exit(-1);
	}

	r = pthread_create(&thread2, NULL, g, (void *)2);
	if (r) {
		printf("Eroare la asteptarea thread-ului %d\n", 2);
		exit(-1);
	}

	void *status;
	r = pthread_join(thread1, &status);
	if (r) {
		printf("Eroare la asteptarea thread-ului %d\n", 1);
		exit(-1);
	}
	
	r = pthread_join(thread2, &status);
	if (r) {
		printf("Eroare la asteptarea thread-ului %d\n", 2);
		exit(-1);
	}
}

int main(int argc, char *argv[]) {
	ex_2();
	printf("\n");

  	ex_3();
	printf("\n");

	ex_4();

  	pthread_exit(NULL);
}
