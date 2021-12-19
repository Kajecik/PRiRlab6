#include <stdio.h>
#include <mpi.h>
#include <math.h>

double PI(int N)
{
    double sum = 0;
    double fraction;
    for (int n = 1; n <= N; n++)
    {
        fraction = pow(-1, n - 1) / (2 * n - 1);
        sum += fraction;
    }
    
    return 4 * sum;
}


int main(int argc, char **argv)
 {
    int N, processNumber;
    int tag = 10;
    double pi;

    MPI_Status status;

    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &processNumber);
    MPI_Comm_size(MPI_COMM_WORLD, &N);

    if (processNumber == 0)
    {
        pi = PI(processNumber + 1);
        printf("\nProcess number = %d\n", processNumber);
        printf("Present value PI: %f\n", pi);
        MPI_Send(&pi, 1, MPI_DOUBLE, processNumber + 1, tag, MPI_COMM_WORLD);
    }

    if (processNumber > 0 && processNumber < N - 1)
    {
        MPI_Recv(&pi, 1, MPI_DOUBLE, processNumber - 1, tag, MPI_COMM_WORLD, &status);

        printf("\nProcess number = %d\n", processNumber);
        printf("Previous number PI: %f\n", pi);
        pi = PI(processNumber + 1);
        printf("Present value PI: %f\n", pi);

        MPI_Send(&pi, 1, MPI_DOUBLE, processNumber + 1, tag, MPI_COMM_WORLD);
    }

    if (processNumber == N - 1)
    {
        MPI_Recv(&pi, 1, MPI_DOUBLE, processNumber - 1, tag, MPI_COMM_WORLD, &status);
        printf("\nProcess number = %d\n", processNumber);
        printf("Previous value PI: %f\n", pi);
        pi = PI(processNumber + 1);
        printf("Present value PI: %f\n", pi);
    }

    MPI_Finalize();
    return 0;
}