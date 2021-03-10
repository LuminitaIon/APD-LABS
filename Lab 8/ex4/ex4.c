#include "mpi.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define ROOT 3

int main (int argc, char *argv[])
{
    int  numtasks, rank, len;
    char hostname[MPI_MAX_PROCESSOR_NAME];

    MPI_Init(&argc, &argv);
    MPI_Comm_size(MPI_COMM_WORLD, &numtasks);
    MPI_Comm_rank(MPI_COMM_WORLD,&rank);
    MPI_Get_processor_name(hostname, &len);

    int value = 0;
    int ret = 0;

    if (rank == ROOT) {

        // The ROOT process receives an element from any source.
        // Prints the element and the source. HINT: MPI_Status.
        MPI_Status status;
        int recv_msg = 0;
        while (recv_msg < numtasks - 1) {
          ret = MPI_Recv(&value, 1, MPI_INT, MPI_ANY_SOURCE, MPI_ANY_TAG, MPI_COMM_WORLD, &status);
          
          if (ret != 0) {
            printf("Can't receive data. I am process %d", rank);
          }
          printf("Received from process %d number %d\n", status.MPI_SOURCE, value);
          recv_msg++;
        }

    } else {

        // Generate a random number.
        srand(time(NULL));
        value = rand() % (rank * 50 + 1);

        printf("Process [%d] send %d.\n", rank, value);

        // Sends the value to the ROOT process.
        ret = MPI_Send(&value, 1, MPI_INT, ROOT, 0, MPI_COMM_WORLD);
        if (ret != 0) {
          printf("Can't send data. I am process %d", rank);
        }

    }

    MPI_Finalize();

}

