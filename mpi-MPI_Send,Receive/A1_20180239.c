#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "mpi.h"

int main(int argsc, char *argsv[])
{
    int myRank;
    int P;
    int tag=0;
    char message[100];
    MPI_Status status;

    MPI_Init(&argsc, &argsv);
    MPI_Comm_rank(MPI_COMM_WORLD, &myRank);
    MPI_Comm_size(MPI_COMM_WORLD, &P);

    if(myRank==0)
    {
        int lower_bound, upper_bound;
        printf("enter the lower bound: ");
        scanf("%d", &lower_bound);
        printf("enter the upper bound: ");
        scanf("%d", &upper_bound);
        int size=upper_bound-lower_bound+1;

        int *nums =malloc(size*sizeof(int));

        int i=0;
        for(;i<=size;i++)
            nums[i]=lower_bound+i;

        int portionSize=size/(P-1), remainder=size%(P-1);
        for(i=1;i<P;i++)
        {
            MPI_Send(&portionSize, 1, MPI_INT, i, tag, MPI_COMM_WORLD);
            MPI_Send(nums+((i-1)*portionSize), portionSize, MPI_INT, i, tag, MPI_COMM_WORLD);
        }
        if(remainder>0)
        {
            MPI_Send(&remainder, 1, MPI_INT, P-1, tag, MPI_COMM_WORLD);
            MPI_Send(nums+((P-2)*portionSize), remainder, MPI_INT, P-1, tag, MPI_COMM_WORLD);
        }

        int count=0;
        for(i=1;i<P;i++)
        {
            int subCount;
            MPI_Recv(&subCount, 1, MPI_INT, i, tag, MPI_COMM_WORLD, &status);
            MPI_Recv(message, 100, MPI_CHAR, i, tag, MPI_COMM_WORLD, &status );
			printf("%s\n" , message);
            count+=subCount;
        }
        printf("Total number of prime numbers is: %d\n", count);

    }else{
        int size, subCount=0;
        MPI_Recv(&size, 1, MPI_INT, 0, tag, MPI_COMM_WORLD, &status);
        int *arr=malloc(size*sizeof(int));
        MPI_Recv(arr, size, MPI_INT, 0, tag, MPI_COMM_WORLD, &status);

        int i=0;
        for(;i<size;i++)
        {
            if(arr[i]<=1)
                continue;
            else if(arr[i]<=3)
                subCount++;
            else
            {
                int j=2,isPrime=1;
                for(;j<arr[i];j++)
                {
                    if(arr[i]%j==0)
                    {
                        isPrime=0;
                        break;
                    }
                }
                if(isPrime==1)
                    subCount++;
            }
        }

        MPI_Send(&subCount, 1, MPI_INT, 0, tag, MPI_COMM_WORLD);
        sprintf( message, "Total number of prime numbers in P%d is: %d", myRank, subCount);
        MPI_Send(message, strlen(message)+1, MPI_CHAR, 0, tag, MPI_COMM_WORLD);

    }


    MPI_Finalize();
    return 0;
}
