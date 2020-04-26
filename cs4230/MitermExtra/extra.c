#include <stdio.h>
#include <omp.h>

/************************************************************************
   Simple CNN code.  This code will work for all layers.
   Read in arguments in specific order.  Only works for correct 
   number of arguments.
  
   Compile with 
     module load intel
     icc -O3 -fopenmp -o cnn_par cnn.assign.c

   Submit to queue using sbatch

   Vary parallelization strategies, loop order (when safe),
   different scheduling strategies, with and without reductions, 
   number of threads
************************************************************************/

int main(int argc, char *argv[]) {
  int n = 1024;
  int i, j, k, ii, jj;


  // ALLOCATE MEMORY 
  float A1[n][n]; 
  memset(A1,0.0,n*n*sizeof(float));
  float A2[n][n]; 
  memset(A2,0.0,n*n*sizeof(float));
 
  int count = 0;
  for(i = 0;i < n;i++) {
    for(j = 0;j < n;j++) {
      A1[i][j] = count;
      A2[i][j] = count++;
    }
  }

      // SEQUENTIAL CALCULATION
   double seq_start = omp_get_wtime();

   for (k = 0; k<n-1; k++) {
     for (i = k+1; i<n; i++) {
       A1[k][i] = A1[k][i] / A1[k][k];
     }         
     for (i = k+1; i<n; i++) {
       for (j = k+1; j<n; j++) {
	 A1[j][i] = A1[j][i] - A1[k][i]*A1[j][k];
       }
     }
   }

  double seq_end = omp_get_wtime();
  double seq_time = seq_end - seq_start;


  // TODO: ADD PARALLEL CALCULATION, AND TIMING
  double start = omp_get_wtime();
  #pragma omp parallel for private(i,j,k,n)
  for (k = 0; k<n-1; k++) {
    #pragma omp parallel for private(i,j,k,n)
     for (i = k+1; i<n; i++) {
       A1[k][i] = A1[k][i] / A1[k][k];
     }
     #pragma omp parallel for private(i,j,k,n)
     for (j = k+1; j<n; j++) {
       //Slows done .1 through .2
       for (i = k+1; i<n; i++) {
	 A1[j][i] = A1[j][i] - A1[k][i]*A1[j][k];
       }
     }
   }

  double end = omp_get_wtime();
  double par_time = end - start;


    // VERIFY CORRECTNESS BY COMPARING OUTPUTS
    for (i = 0;i < n; i++) { // minibatch size
      for (j = 0;j < n;j++) { // output feature map
	      if(abs(A1[n][k]-A2[n][k])> .0001) {
		printf("Outputs do not match!!!\n");
                 exit(2);
              }
	    }}

    // PRINT OUT SPEEDUP
    printf ("Sequential time = %f, Parallel time = %f, Speedup = %f\n",seq_time, par_time, seq_time/par_time);
}  
