#include <stdio.h>
#include <stdlib.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>
#include "mpi.h"

int main(int argc, char *argv[])
{
    int myRank;
    int p;

    int n, localSum, sum=0, *arr;
    double localMean=0, mean, globalMean=0.0, sdMean, sd;

    MPI_Init (&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &myRank);
    MPI_Comm_size(MPI_COMM_WORLD, &p);

    if(myRank==0)
    {
        printf("enter n: ");
        scanf("%d",&n);
        printf("\nthe numbers:\n[");
    }
    MPI_Bcast(&n, 1, MPI_INT, 0, MPI_COMM_WORLD);

    if(myRank!=0)
    {
        arr=malloc(n*sizeof(int));
        localSum=0;
        srand(time(NULL)+(&myRank));

        int i=0,x;
        for(i;i<n;i++)
        {
            x=rand()%20;
            arr[i]=x;
            localSum+=x;
            printf("%d, ", x);
        }
    }
    MPI_Allreduce(&localSum, &sum, 1, MPI_INT, MPI_SUM, MPI_COMM_WORLD);

    if(myRank!=0)
    {
        int x=n*(p-1);
        localMean=0;
        mean=sum/(double)x;

        int i=0;
        for(i;i<n;i++)
            localMean+= (arr[i]-mean)*(double)(arr[i]-mean);
    }
    MPI_Reduce(&localMean, &globalMean, 1, MPI_DOUBLE, MPI_SUM, 0, MPI_COMM_WORLD);

    if(myRank==0)
    {
        sdMean=globalMean/(double)(n*(p-1));
        sd=sqrt(sdMean);
        printf("]\nhave a standard deviation of: %.3f\n",sd);
    }


    MPI_Finalize();
    return 0;
}
