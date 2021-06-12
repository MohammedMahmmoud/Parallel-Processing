#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <omp.h>
#include <mpi.h>


int main ( int argc , char * argv []){
	int n , myid , numprocs , i;
	double number , h , sum , x;
	MPI_Init(& argc ,& argv );

	MPI_Comm_size( MPI_COMM_WORLD ,& numprocs );
	MPI_Comm_rank( MPI_COMM_WORLD ,& myid );
	n = 0;
	{
		for(;;) {
			if ( myid == 0) {
				printf (" Enter the number of intervals : (0 quits ) ");
				scanf ("%d" ,&n );
			}
			MPI_Bcast(&n, 1, MPI_INT , 0 , MPI_COMM_WORLD );
			if ( n == 0)
				break ;
			else {
				sum = 0.0;
				//to sum all the (sum) from all process
				#pragma omp parallel for reduction (+:sum) private (i,x)
					for ( i = myid + 1; i <= n ; i += numprocs) {
						x = 1.0 / pow(2.0,( double ) i);
						sum+=x;
					}
				MPI_Reduce(& sum , &number , 1 , MPI_DOUBLE , MPI_SUM , 0 , MPI_COMM_WORLD );

				if ( myid == 0) {
					printf ("calculated is  : %.16f Error is: %.16f\n", number , fabs( number - 1 ));
				}
			}
		}
	}
	MPI_Finalize();
	return EXIT_SUCCESS;
}
