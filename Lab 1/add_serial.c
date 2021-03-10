#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define MIN(a,b) (((a) < (b))?(a):(b))
/*
    schelet pentru exercitiul 5
*/

int* arr;
int array_size;
long cores;

void *ex_5 (void *arg) {
	long thread_id = (long)arg;
	
	int start = thread_id * (double)array_size / cores;
	int end = MIN((thread_id + 1) * (double) array_size / cores, array_size);
	
	for (int i = start; i < end; i++) {
		arr[i] += 100;
	}
	pthread_exit(NULL);
}

int main(int argc, char *argv[]) {
    int r;
    if (argc < 2) {
        perror("Specificati dimensiunea array-ului\n");
        exit(-1);
    }

    array_size = atoi(argv[1]);

    arr = malloc(array_size * sizeof(int));
    for (int i = 0; i < array_size; i++) {
        arr[i] = i;
    }

//    for (int i = 0; i < array_size; i++) {
//       printf("%d", arr[i]);
//       if (i != array_size - 1) {
//           printf(" ");
//          } else {
//            printf("\n");
//        }
//    }

    // TODO: aceasta operatie va fi paralelizata
//  	for (int i = 0; i < array_size; i++) {
//        arr[i] += 100;
//    }

//    for (int i = 0; i < array_size; i++) {
//        printf("%d", arr[i]);
//       if (i != array_size - 1) {
//            printf(" ");
//        } else {
//            printf("\n");
//        }
//    }

	if (argc == 3) {
		cores = atoi(argv[2]);
	} else {
		cores = sysconf(_SC_NPROCESSORS_CONF);
	}
	
	pthread_t threads[cores];
   	for (long id = 0; id < cores; id++) {
       	 r = pthread_create(&threads[id], NULL, ex_5, (void *)id);
       	 
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

  	pthread_exit(NULL);
}
