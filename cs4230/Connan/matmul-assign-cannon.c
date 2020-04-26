// Compiling: 
//    module load intel
//    mpicc matmul-assign.c -o matmul
// Executing:
//    mpiexec -n 2 matmul
// Sbatch execution:
//    sbatch script.matmul

#include "stdio.h"
#include "mpi.h"

#define N 9
#define P 9
#define SQRP 3

void
main(int argc, char *argv[]) {
  FILE *f;
  int i, j, k, error, rank, size;
  float a[N][N], b[N][N], c[N][N], myc[SQRP][SQRP], mya[SQRP][SQRP], myb[SQRP][SQRP], tmpdata;
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
	printf("SEQ: a[%d][%d] = %f\n",i,j,a[i][j]);
        b[i][j] = tmpdata+1.;
	printf("SEQ: b[%d][%d] = %f\n",i,j,b[i][j]);
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
         printf("SEQ: c[%d][%d] = %f\n", i,j,c[i][j]);
        }
     }
   }

   // TODO: Parallel Portion.  Distribute a and b into local copies 
   // mya and myb using Scatterv as in website pointed to by Lecture 21.  
   // Initialize myc to 0.

   int dis1[] = {0,3,6,27,30,33,54,57,60};
   int count[] = {3,3,3,3,3,3,3,3,3};

   MPI_Scatterv(a, count, dis1, MPI_FLOAT, &mya[0], 3, MPI_FLOAT, 0, MPI_COMM_WORLD);
   MPI_Scatterv(b, count, dis1, MPI_FLOAT, &myb[0], 3, MPI_FLOAT, 0, MPI_COMM_WORLD);

   for(i = 0; i < 9;i++) {
     dis1[i] = dis1[i] + 9;
   }

   MPI_Scatterv(a, count, dis1, MPI_FLOAT, &mya[1], 3, MPI_FLOAT, 0, MPI_COMM_WORLD);
   MPI_Scatterv(b, count, dis1, MPI_FLOAT, &myb[1], 3, MPI_FLOAT, 0, MPI_COMM_WORLD);

   for(i = 0; i < 9;i++) {
     dis1[i] = dis1[i] + 9;
   }

   MPI_Scatterv(a, count, dis1, MPI_FLOAT, &mya[2], 3, MPI_FLOAT, 0, MPI_COMM_WORLD);
   MPI_Scatterv(b, count, dis1, MPI_FLOAT, &myb[2], 3, MPI_FLOAT, 0, MPI_COMM_WORLD);

   for(i = 0;i < SQRP;i++) {
     for(j = 0;j < SQRP;j++) {
       myc[i][j] = 0.0;
     }
   }

   for(i = 0;i < SQRP;i++) {
     for(j = 0;j < SQRP;j++) {
       printf("Rank: %d, mya[%d][%d]: %f\n",rank,i,j,mya[i][j]);
       //printf("Rank: %d, myb[%d][%d]: %f\n",rank,i,j,myb[i][j]);
     }
   }


   // TODO: Now create Cartesian grid communicator (see website pointed to
   // by Lecture 21 and Sundar-communicators.pdf on Canvas)

   // TODO: Move a and b data within Cartesian Grid using initial skew 
   // operations (see p. 10 of Lecture 20.) 
 
   // TODO: Add following loop:
   // for (k=0; k<=SQRP-1; k++} {
   //    CALC: Should be like sequential code, but use
   //          myc, mya, and myb.  Adjust bounds for all loops to SQRP.  
   //          (More generally, (N/P/SQRP)).
   //    SHIFT: Shift A leftward and B upward by 1 in Cartesian grid.
   // }

   // Output local results to compare against sequential
   for (i = 0; i<SQRP; i++) {   
      for (j = 0; j<SQRP; j++) {   
	printf("PAR, RANK %d: c[%d][%d] = %f\n", rank, (rank/SQRP)+i,(rank % SQRP)+j,myc[i][j]);
      }
   }

   MPI_Finalize();
}
