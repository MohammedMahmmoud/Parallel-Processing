#include <omp.h>
#include <stdio.h>
#include <stdlib.h>

#define N 12
#define CHUNKSIZE 2

int main(int argc, char *argv[])
{

    int i,j, chunk;
    int r=4,c=5;


    scanf("%d",&r);
    scanf("%d",&c);

    char *arr[r];
    char *arr2[r];
    for (i=0; i<r; i++)
    {
        arr[i] = (char *)malloc(c * sizeof(char));
        arr2[i] = (char *)malloc(c * sizeof(char));
    }

    /* Some initializations */
    for (i = 0; i <  r; i++)
    {
        for (j = 0; j < c; j++)
        {
            scanf(" %c",&arr[i][j]);
        }
    }

    for (i = 0; i <  r; i++)
        {
            for (j = 0; j < c; j++)
            {
                printf ("%c" , arr[i][j] );

            }
            printf("\n");
        }


    #pragma omp parallel shared(arr,arr2) private(i) private(j)
    {

        #pragma omp for schedule(static)
        for (i = 0; i <  r; i++)
        {
            for (j = 0; j < c; j++)
            {
                if(j>0 && j<c-1)
                {
                    if(arr[i][j]=='o' && arr[i][j-1]=='x' && arr[i][j+1]=='x')
                    {
                        arr2[i][j]='x';
                    }
                    else
                        arr2[i][j]=arr[i][j];
                }
                else
                    arr2[i][j]=arr[i][j];

                printf ("IN thread %d arr2[%d][%d] = %c\n" , omp_get_thread_num() , i,j,arr2[i][j] );
            }
        }
    }   /* end of parallel region */

}
