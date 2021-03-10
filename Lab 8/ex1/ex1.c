#include "mpi.h"
#include <stdio.h>
#include <stdlib.h>

int main (int argc, char *argv[])
{
    int  numtasks, rank;
    MPI_Init(&argc, &argv);
    MPI_Comm_size(MPI_COMM_WORLD, &numtasks);
    MPI_Comm_rank(MPI_COMM_WORLD,&rank);

    int recv_num;
    int random_num, ret = 0, inc_val = 2;
    MPI_Status status;
    srand(30);

    // First process starts the circle.
    if (rank == 0) {
        // First process starts the circle.
        // Generate a random number.
        random_num = rand() % 100;
        printf("Process %d: The number is %d\n", rank, random_num);
        // Send the number to the next process.
        ret = MPI_Send(&random_num, 1, MPI_INT, (rank + 1), 0, MPI_COMM_WORLD);
        if (ret != 0) {
          printf("Can't send data. I am process %d", rank);
        }

    } else if (rank == numtasks - 1) {
        // Last process close the circle.
        // Receives the number from the previous process.
        ret = MPI_Recv(&random_num, 1, MPI_INT, (rank - 1), 0, MPI_COMM_WORLD, &status);
        if (ret != 0) {
          printf("Can't receive data. I am process %d", rank);
        }

        // Increments the number.
        random_num += inc_val;

        // print received number
        printf("Process %d: The number is %d\n", rank, random_num);
        
        // Sends the number to the first process.
        ret = MPI_Send(&random_num, 1, MPI_INT, 0, 0, MPI_COMM_WORLD);
        if (ret != 0) {
          printf("Can't send data. I am process %d", rank);
        }

    } else {
        // Middle process.
        // Receives the number from the previous process.
        ret = MPI_Recv(&random_num, 1, MPI_INT, (rank - 1), 0, MPI_COMM_WORLD, &status);
        if (ret != 0) {
          printf("Can't receive data. I am process %d", rank);
        }
        // Increments the number.
        random_num += inc_val;
        
        // print received number
        printf("Process %d: The number is %d\n", rank, random_num);

        // Sends the number to the next process.
        ret = MPI_Send(&random_num, 1, MPI_INT, (rank + 1), 0, MPI_COMM_WORLD);
        if (ret != 0) {
          printf("Can't send data. I am process %d", rank);
        }
    }

    MPI_Finalize();

}
