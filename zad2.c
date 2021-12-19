#include <stdio.h>
#include <mpi.h>
#include <math.h>


int a = 1, b = 6;

double func(double x)
{
    return x * x;
}

double countNumberOfTrapezoids(int numberOfTrapezoids, int i)
{
    double h = (double)(b - a) / numberOfTrapezoids;
    double field = fabs(func(a + (i - 1) * h)) + fabs(func(a + i * h));
    return field / 2 * h;
}

double licz_pole_prostokatow(int numberOfRectangles, int i)
{

    double h = (double)(b - a) / numberOfRectangles;
    double field = fabs(func(a + (i - 1) * h) * h) + fabs(func(a + i * h) * h);
    return field;
}



int main(int argc, char **argv)
 {

    int numberOfTrapezoids, processNumber, i;
    int tag = 13;
    double trapezoidField, sum = 0;
    double rectangleField, sum2 = 0;
    MPI_Status status;

    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &processNumber);
    MPI_Comm_size(MPI_COMM_WORLD, &numberOfTrapezoids);

    if (processNumber == numberOfTrapezoids - 1)
    {
    	i = processNumber + 1;
        trapezoidField = countNumberOfTrapezoids(numberOfTrapezoids, i);
        rectangleField = licz_pole_prostokatow(numberOfTrapezoids, i - 1);
        sum += trapezoidField;
        sum2 += rectangleField;
        printf("\nNumer procesu = %d\n", processNumber);
        printf("Pole aktualnego trapezu %d wynosi %f\n", i, trapezoidField);
        printf("Pole aktualnego prostokata %d wynosi %f\n", i, rectangleField);
        printf("Suma wszystkich trapezow wynosi %f\n", sum);
        printf("Suma wszystkich prostokatow wynosi %f\n", sum2);

        MPI_Send(&sum, 1, MPI_DOUBLE, processNumber - 1, tag, MPI_COMM_WORLD);
        MPI_Send(&sum2, 1, MPI_DOUBLE, processNumber - 1, tag, MPI_COMM_WORLD);
    }

    if (processNumber > 0 && processNumber < numberOfTrapezoids - 1)
    {
        MPI_Recv(&sum, 1, MPI_DOUBLE, processNumber + 1, tag, MPI_COMM_WORLD, &status);
        MPI_Recv(&sum2, 1, MPI_DOUBLE, processNumber + 1, tag, MPI_COMM_WORLD, &status);

        i = processNumber + 1;
        trapezoidField = countNumberOfTrapezoids(numberOfTrapezoids, i);
        rectangleField = licz_pole_prostokatow(numberOfTrapezoids, i - 1);
        sum += trapezoidField;
        sum2 += rectangleField;
        printf("\nNumer procesu = %d\n", processNumber);
        printf("Pole aktualnego trapezu %d wynosi %f\n", i, trapezoidField);
        printf("Pole aktualnego prostokata %d wynosi %f\n", i, rectangleField);
        printf("Suma wszystkich trapezow wynosi %f\n", sum);
        printf("Suma wszystkich prostokatow wynosi %f\n", sum2);

        MPI_Send(&sum, 1, MPI_DOUBLE, processNumber -1, tag, MPI_COMM_WORLD);
        MPI_Send(&sum2, 1, MPI_DOUBLE, processNumber - 1, tag, MPI_COMM_WORLD);

    }

    if (processNumber == 0)
    {
        MPI_Recv(&sum, 1, MPI_DOUBLE, processNumber + 1, tag, MPI_COMM_WORLD, &status);
        MPI_Recv(&sum2, 1, MPI_DOUBLE, processNumber + 1, tag, MPI_COMM_WORLD, &status);

        i = processNumber + 1;
        trapezoidField = countNumberOfTrapezoids(numberOfTrapezoids, i);
        rectangleField = licz_pole_prostokatow(numberOfTrapezoids, i - 1);
        sum += trapezoidField;
        sum2 += rectangleField;
        printf("\nNumer procesu = %d\n", processNumber);
        printf("Pole aktualnego trapezu %d wynosi %f\n", i, trapezoidField);
        printf("Pole aktualnego prostokata %d wynosi %f\n", i, rectangleField);
        printf("Suma wszystkich trapezow wynosi %f\n", sum);
        printf("Suma wszystkich prostokatow wynosi %f\n", sum2);
    }

    MPI_Finalize();
    return 0;
}