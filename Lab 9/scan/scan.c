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

    for (int i = 1; i < procs; i *= 2) {
        // TODO
        if (rank + 1 < procs) {
            //printf("pas %d: %d ii trimite lui %d, sum=%d\n", i, rank, (rank - (i / 2)), s);
            ret = MPI_Send(&s, 1, MPI_INT, (rank + i), 0, MPI_COMM_WORLD);
        }

        if(rank - i >= 0) {
            ret = MPI_Recv(&recv_value, 1, MPI_INT, (rank - i), 0, MPI_COMM_WORLD, &status);
            s += recv_value;
        }
    }

    printf("Process [%d] has result = %d\n", rank, s);

    MPI_Finalize();

}

