#include <stdio.h>
#include <mpi.h>
double integral_f(double x)
{
    return (x * x * x * x - 2 * x * x + 2) / (x * x - 2 * x + 2);
}
double work(int processId, int processCount, int a, int b)
{
    double result = 0;
    int n = (b - a) / processCount;
    int start = processId * n;
    int end = start + n;
    for (int i = start; i < end; i++)
    {
        result += integral_f(a + i - 0.5);
    }
    return result;
}
int main(int argc, char **argv)
{
    double startTime, time, maxTime, allResult, result;
    int processId, processCount, a = 0, b = 8000;
    MPI_Init(&argc, &argv);
    MPI_Comm_size(MPI_COMM_WORLD, &processCount);
    MPI_Comm_rank(MPI_COMM_WORLD, &processId);
    MPI_Barrier(MPI_COMM_WORLD);
    startTime = MPI_Wtime();
    result = work(processId, processCount, a, b);
    time = MPI_Wtime() - startTime;
    MPI_Reduce(&result, &allResult, 1, MPI_DOUBLE, MPI_SUM, 0, MPI_COMM_WORLD);
    MPI_Reduce(&time, &maxTime, 1, MPI_DOUBLE, MPI_MAX, 0, MPI_COMM_WORLD);
    printf("Result: %.8f for process %d. Time: %f \n", result, processId, time);
    MPI_Barrier(MPI_COMM_WORLD);
    MPI_Finalize();
    if (processId == 0)
    {
        printf("---------------------------------------\n");
        printf("Result: %.8f\n", allResult);
        printf("Time: %f\n", maxTime);
    }
    r
