#include "mpi.h"
#include <stdio.h>
#include <stdlib.h>

#define ROOT 0

int main (int argc, char *argv[])
{
    int  numtasks, rank;

    MPI_Init(&argc, &argv);
    MPI_Comm_size(MPI_COMM_WORLD, &numtasks);
    MPI_Comm_rank(MPI_COMM_WORLD,&rank);

    // Checks the number of processes allowed.
    if (numtasks != 2) {
        printf("Wrong number of processes. Only 2 allowed!\n");
        MPI_Finalize();
        return 0;
    }

    // How many numbers will be sent.
    int send_numbers = 10;
    int value = 0, ret = 0;

    if (rank == 0) {

        // Generate the random numbers.
        // Generate the random tags.
        // Sends the numbers with the tags to the second process.
        // Generate a random number.
        for (int i = 0; i < send_numbers; i++) {

            srand(i);
            value = rand() % 100;

            printf("Process [%d] send %d with tag %d.\n", rank, value, i);
            
            // Sends the value to the ROOT process.
            ret = MPI_Send(&value, 1, MPI_INT, 1, i, MPI_COMM_WORLD);
            if (ret != 0) {
              printf("Can't send data. I am process %d", rank);
            }
        }

    } else {

        // Receives the information from the first process.
        // Prints the numbers with their corresponding tags.

        MPI_Status status;
        int recv_msg = 0;
        while (recv_msg < send_numbers) {
          ret = MPI_Recv(&value, 1, MPI_INT, MPI_ANY_SOURCE, MPI_ANY_TAG, MPI_COMM_WORLD, &status);
          
          if (ret != 0) {
            printf("Can't receive data. I am process %d", rank);
          }

          printf("Received from process %d number %d with tag %d\n",
                status.MPI_SOURCE, value, status.MPI_TAG);
          recv_msg++;
        }
    }

    MPI_Finalize();

}

