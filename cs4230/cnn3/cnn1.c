#include <stdio.h>
#include <omp.h>

/* 
   Simple CNN code.  This code will work for all layers.  Read in arguments in specific order.  Only works for correct 
   number of arguments.
  
   Compile with 
     module load intel
     icc -O3 -fopenmp -o cnn_par cnn.assign.c

   Submit to queue using sbatch

   Vary parallelization strategies, loop order (when safe), different scheduling strategies, with and without reductions, 
   number of threads
*/

int main(int argc, char *argv[]) {
  int n,k,c,p,q,r,s,ii,ij,cc;

  // READ PROBLEM SIZES
  if (argc != 10) exit(1);
  int N = atoi(argv[1]);
  int C = atoi(argv[2]);
  int K = atoi(argv[3]);
  int H = atoi(argv[4]);
  int W = atoi(argv[5]);
  int R = atoi(argv[6]);
  int S = atoi(argv[7]);
  int u = atoi(argv[8]);
  int v = atoi(argv[9]);
  int P = (H-R)/u + 1; 
  int Q = (W-S)/v + 1;

  // ALLOCATE MEMORY 
  float output_seq[128][128][55][55]; 
  memset(output_seq,0.0,128*128*55*55*sizeof(float));
  float output_par[128][128][55][55]; 
  memset(output_par,0.0,128*128*55*55*sizeof(float));
  float input[128][112][112][832];
  float weight[128][3][3][832];
 
   // ASSIGN INITIAL VALUES FOR INPUT AND WEIGHT
  for (n=0; n<N; n++) { 
    for (p=0; p<(P-1)*u+R; p++) {
      for (q=0; q<(Q-1)*v+S; q ++) { 
	for (c=0; c<C; c++) {
	  input [n][p][q][c] = ((float) (n+c+p+q)/7);
	}
      }
    }
  }

  for (k=0; k<K; k++) { 
    for (r =0; r<R; r++) { 
      for (s =0; s<S; s++) {
	for (c=0; c<C; c++) { 
	  weight[k][r][s][c] = ((float) (k+c+r+s)/11);
	}}}}

      // SEQUENTIAL CALCULATION
   double seq_start = omp_get_wtime();
    for (n=0; n<N; n++) { // minibatch size
      for (k=0; k<K; k ++) { // output feature map
        for (c=0; c<C; c ++) { // input feature map
          for (p=0; p<P; p ++) { // output height
            ij = p * u; // input height
            for (q =0; q<Q; q ++) { // output width
              ii = q * v; // input width
	      for (r=0; r<R; r ++) { // filter height
	        for (s =0; s< S; s ++) {// filter width
	        output_seq[n][k][p][q] += input [n][ij+r][ii+s][c] * weight[k][r][s][c];
	    } } } } } } }
  double seq_end = omp_get_wtime();
  double seq_time = seq_end - seq_start;

   float temp;
   int b = 16;
  // TODO: ADD PARALLEL CALCULATION, AND TIMING
  double start = omp_get_wtime();
  //for(c=0;c<C;c+=b) {
  for (n=0;n<N;n++) { // minibatch size
    for (k=0;k<K;k++) { // output feature map
      for (p=0;p<P;p++) { // output height
	ij=p*u; // input height
	for (q=0;q<Q;q++) { // output width
	  ii=q*v; // input width
	  temp = 0.0;
	  for (r=0;r<R;r++) { // filter height
	    for (s=0;s< S;s++) {// filter width
	      for(c=0;c<C;c+=b) {
		//float temp = 0.0;
                #pragma omp simd
		for(cc=c;cc<c+b;cc++) {
		  temp += input[n][ij+r][ii+s][cc] * weight[k][r][s][cc];
	        } 
		  //output_par[n][k][p][q] = temp;
	      } 
	    } 
	  } 
	  output_par[n][k][p][q] = temp; 
	} 
      } 
    } 
  }
  double end = omp_get_wtime();
  double par_time = end - start;


    // VERIFY CORRECTNESS BY COMPARING OUTPUTS
    for (n=0; n<N; n++) { // minibatch size
      for (k=0; k<K; k ++) { // output feature map
        for (c=0; c<C; c ++) { // input feature map
          for (p=0; p<P; p ++) { // output height
            for (q =0; q<Q; q ++) { // output width
	      if(abs(output_seq[n][k][p][q]-output_par[n][k][p][q])> .0001) {
		printf("Outputs do not match!!!\n");
                 exit(2);
              }
	    }}}}}

    // PRINT OUT SPEEDUP
    printf ("Sequential time = %f, Parallel time = %f, Speedup = %f\n",seq_time, par_time, seq_time/par_time);
}  
