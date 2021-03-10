#include "mpi.h"
#include <stdio.h>
#include <stdlib.h>

#define MASTER 0

int main (int argc, char *argv[])
{
    int procs, rank;

    MPI_Init(&argc, &argv);
    MPI_Comm_size(MPI_COMM_WORLD, &procs);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    int value = rank;
    int s = value;
    int recv_value = 0;
    int ret = 0;
    MPI_Status status;

    for (int i = 2; i <= procs; i *= 2) {
        // TODO
        if(rank % i == 0) {
        	ret = MPI_Recv(&recv_value, 1, MPI_INT, (rank + (i / 2)), 0, MPI_COMM_WORLD, &status);
        	s += recv_value;
        } else if (rank % (i / 2) == 0) {

        	ret = MPI_Send(&s, 1, MPI_INT, (rank - (i / 2)), 0, MPI_COMM_WORLD);
        }
    }

    if (rank == MASTER) {
        printf("Result = %d\n", s);
    }

    MPI_Finalize();

}

