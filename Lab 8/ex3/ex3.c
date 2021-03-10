#include "mpi.h"
#include <stdio.h>
#include <stdlib.h>

#define MULTI 5 // chunk dimension
#define ROOT 0

int main (int argc, char *argv[])
{
    int  numtasks, rank, len;
    char hostname[MPI_MAX_PROCESSOR_NAME];

    MPI_Init(&argc, &argv);
    MPI_Comm_size(MPI_COMM_WORLD, &numtasks);
    MPI_Comm_rank(MPI_COMM_WORLD,&rank);
    MPI_Get_processor_name(hostname, &len);

    int ret = 0;
    int num_elements = MULTI * numtasks; // total elements
    int *v_send = NULL; // full vector
    int *v_recv = (int *)malloc(MULTI * sizeof(int)); // partial vector
    int *v_result = (int *)malloc(MULTI * numtasks* sizeof(int)); // partial vector

    // ROOT process generates the values for the full vector.
    // Scatter the vector to all processes.
    if (rank == ROOT) {
        // malloc the vector
        v_send = malloc (MULTI * numtasks * sizeof(int));
        for (int i = 0; i < MULTI * numtasks; i++) {
          v_send[i] = i;
        }
    }

    ret = MPI_Scatter(v_send, MULTI, MPI_INT,
          v_recv, MULTI, MPI_INT, ROOT, MPI_COMM_WORLD);
    if (ret != 0) {
      printf("Process %d can't scatter\n", rank);
    }    
    
    /*
     * Prints the values received after scatter.
     * NOTE: If CHUNK changed, also change this line.
     */
    printf("Process [%d]: have elements: ", rank);
    for (int i = 0; i < MULTI; i++) {
      v_recv[i] += rank; 
      printf("%d ", v_recv[i]);
    }

    printf("\n");

    // Each process increments the values of the partial vector received.
    // Gathers the values from all the processes.
    // The ROOT process prints the elements received.

    ret = MPI_Gather(v_recv, MULTI, MPI_INT, v_result, MULTI, MPI_INT, ROOT, MPI_COMM_WORLD);
    if (ret != 0) {
      printf("Process %d can't scatter\n", rank);
    }   

    if (rank == ROOT) {
      printf("Result array  have elements: ");
      for (int i = 0; i < MULTI * numtasks; i++) {
        printf("%d ", v_result[i]);
      }

      printf("\n");
    }
    
    MPI_Finalize();

}

