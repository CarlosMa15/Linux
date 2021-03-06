// Compiling: 
//    module load intel
//    mpicc matmul-assign.c -o matmul
// Executing:
//    mpiexec -n 2 ./matmul
// Sbatch execution:
//    sbatch script.matmul

#include "stdio.h"
#include "mpi.h"

//#define N 2
// Can adjust N through #define
#define N 16
#define P 2

void
main(int argc, char *argv[]) {
  FILE *f;
  int i, j, k, error, rank, size;
  float a[N][N], b[N][N], c[N][N], myc[N/P][N], tmpb[N/P][N], mya[N/P][N], myb[N/P][N], tmpdata;
  MPI_Request sendreq, rcvreq;
  MPI_Status status;


   MPI_Init(&argc, &argv);
   MPI_Comm_rank( MPI_COMM_WORLD, &rank );
   MPI_Comm_size( MPI_COMM_WORLD, &size );


   // sequential MM 
   if (rank == 0) {

     // read in matrix
     f = fopen("matrixA.dat","r");
     for (i = 0; i<N; i++) {   
       for (j = 0; j<N; j++) {   
	 error = fscanf(f,"%f",&tmpdata);
	 a[i][j] = tmpdata;
	 b[i][j] = tmpdata+1.;
       }
     }
     fclose(f);  

     // After computing each point, output sequential results.
     for (i = 0; i< N; i++) {
       for (j = 0; j<N; j++) {
         c[i][j] = 0.;
         for (k=0; k<N; k++) {
           c[i][j] += a[i][k] * b[k][j];
         }
         printf("SEQ: c[%d][%d] = %f\n",i,j,c[i][j]);
       }
     }

   }

   MPI_Scatter(a, N*N/P, MPI_FLOAT, mya, N*N/P, MPI_FLOAT, 0, MPI_COMM_WORLD);
   MPI_Scatter(b, N*N/P, MPI_FLOAT, myb, N*N/P, MPI_FLOAT, 0, MPI_COMM_WORLD);

   for(i = 0;i < N/P;i++) {
     for(j = 0;j < N;j++) {
       myc[i][j] = 0.0;
     }
   }

   MPI_Isend(myb, N*N/P, MPI_FLOAT, (rank + 1) % P, 0, MPI_COMM_WORLD, &sendreq);
   int src = (rank == 0) ? P-1 : rank - 1;
   MPI_Irecv(tmpb, N*N/P, MPI_FLOAT, src, 0, MPI_COMM_WORLD, &rcvreq);

   for (i=0;i<N/P;i++) {
     for (j=0;j<N;j++) {
       for (k=0;k<N/P;k++) {
	 myc[i][j]+=mya[i][k+(rank*(N/P))]*myb[k][j];
       }
     }
   }

   MPI_Wait(&rcvreq, &status);
   
   for (i=0;i<N/P;i++) {
     for (j=0;j<N;j++) {
       for (k=0;k<N/P;k++) {
	 myc[i][j]+=mya[i][k+((rank + 1)*(N/P))%N]*tmpb[k][j];
       }
     }
   }

   for (i = 0; i<N/P; i++) {   
      for (j = 0; j<N; j++) {   
	printf("PAR, RANK %d: c[%d][%d] = %f\n", rank, i+rank*N/P,j,myc[i][j]);
      }
   }

   MPI_Finalize();
}
