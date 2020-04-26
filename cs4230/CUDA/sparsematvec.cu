#include <stdio.h>


#define __min(x,y) ((x)<(y)?(x):(y))

;
#define N 1024

extern int cudaMemcpy();					//Memcopy
extern int cudaFree();
extern void __syncthreads();
extern int cudaMemcpyToSymbol();				//Memcopy
extern __global__ void mv_GPU(float *, float (*)[N], float *);  //Kernal

int compare(float *a, float *b, int size, double threshold) {
    printf("CHECK2\n");
    int i;
    for (i=0; i<size; i++) {
      if (abs(a[i]-b[i]) > threshold) return 0;
    }
    return 1;
}

void normalMV(int *ptr, int nr, float *t, float *data, int *indices, float *b){
  int i;
  int j;
  for (i=0; i<nr; i++) {
     for (j = ptr[i]; j<ptr[i+1]; j++) {
        t[i] = t[i] + data[j] * b[indices[j]];
     }
  }

  /************************************************
  int i;
  int j;
  for(i=0;i<N;i++)
    for(j=0;j<N;j++)
		a[i] = a[i] +  c[j*N+i] * b[j] ;
  ************************************************/
}

extern __global__ void mv_GPU(int *ptr, int nr, float *t, float *data, int *indices, float *b)
  {

    int i;
    int j;
    for (i=0; i<nr; i++) {
       for (j = ptr[i]; j<ptr[i+1]; j++) {
          t[i] = t[i] + data[j] * b[indices[j]];
       }
    }

    /*******************************************************************
    int bx;
    int tx;
    float acpy;
    __shared__ float bcpy[32];
    int k;
    int j;

    bx = blockIdx.x;
    tx = threadIdx.x;
    int i = bx * blockDim.x + tx;
    int tiles = (N+blockDim.x-1)/blockDim.x;

    if (tx <= -(blockDim.x * bx) + N-1)
      {
        acpy = 0.0;
	// suif_tmp0 = ((float *)(float (*)[])a)[i];
      
    for (k = 0; k <= tiles; k++)
      {
          bcpy[tx] = b[blockDim.x * k + tx];
          __syncthreads();
          for (j = 32 * k; j <= __min(32 * k + 31, N-1); j++)
          {
              acpy = acpy + c[j][i] * bcpy[j - 32 * k];
          }
        __syncthreads();
      }
        a[i] = acpy;
      }
      ****************************************************************/
  }

main (int argc, char **argv) {
  FILE *fp;
  char line[1024];
  int *ptr, *indices;
  float *data, *bb, *t;
  int i;
  int n; // number of nonzero elements in data
  int nr; // number of rows in matrix
  int nc; // number of columns in matrix  
  
  // Open input file and read to end of comments
  if (argc !=2) abort();
  if ((fp = fopen(argv[1], "r")) == NULL) {
     abort();
  }
  
  fgets(line, 128, fp);
  while (line[0] == '%') {
     fgets(line, 128, fp);
  }

  // Read number of rows (nr), number of columns (nc) and
  // number of elements and allocate memory for ptr, indices, data, bb and t.
  sscanf(line,"%d %d %d\n", &nr, &nc, &n);
  ptr = (int *) malloc ((nr+1)*sizeof(int));
  indices = (int *) malloc(n*sizeof(int));
  data = (float *) malloc(n*sizeof(float));
  bb = (float *) malloc(nc*sizeof(float));
  t = (float *) malloc(nr*sizeof(float));

  // Read data in coordinate format and initialize sparse matrix
  int lastr=0;
  for (i=0; i<n; i++) {
     int r;
     fscanf(fp,"%d %d %f\n", &r, &(indices[i]), &(data[i]));
     indices[i]--;  // start numbering at 0
     if (r!=lastr) {
     	ptr[r-1] = i;
     	lastr = r;
     }
  }
  ptr[nr] = n;

  // initialize t to 0 and b with random data 
  for (i=0; i<nr; i++) {
     t[i] = 0.0;
  }
  for (i=0; i<nc; i++) {
     bb[i] = (float) rand()/1111111111;
  }

  // create CUDA event handles for timing purposes
  cudaEvent_t start_event, stop_event;
  float elapsed_time_seq, elapsed_time_gpu;

  cudaEventCreate(&start_event);
  cudaEventCreate(&stop_event);
  cudaEventRecord(start_event, 0);   
  normalMV(ptr,nr,t,data,indices, bb);
  cudaEventRecord(stop_event, 0);
  cudaEventSynchronize(stop_event);
  cudaEventElapsedTime(&elapsed_time_seq,start_event, stop_event);

    int *devO1Ptr;					// int * ptr
    int *devI1Ptr;				      	// int * indices
    float *devI2Ptr;				      	// float * data
    float *devI3Ptr;				      	// float * b
    float *devI4Ptr;				      	// float * t

    cudaMalloc((void **)&devO1Ptr, (nr+1)*sizeof(int));
    cudaMemcpy(devO1Ptr, ptr, (nr+1)*sizeof(int), cudaMemcpyHostToDevice);
    cudaMalloc((void **)&devI1Ptr, n*sizeof(int));
    cudaMemcpy(devI1Ptr,indices,n*sizeof(int), cudaMemcpyHostToDevice);
    cudaMalloc((void **)&devI2Ptr,n*sizeof(float));
    cudaMemcpy(devI2Ptr,data,n*sizeof(int), cudaMemcpyHostToDevice);
    cudaMalloc((void **)&devI3Ptr,nc*sizeof(float));
    cudaMemcpy(devI3Ptr,bb,nc*sizeof(int), cudaMemcpyHostToDevice);
    cudaMalloc((void **)&devI4Ptr,nr*sizeof(float));
    cudaMemcpy(devI4Ptr, t,nr*sizeof(float), cudaMemcpyHostToDevice);

    dim3 dimGrid((N+31)/32, 1);
    dim3 dimBlock(32, 1);

  cudaEventCreate(&start_event);
  cudaEventCreate(&stop_event);
  cudaEventRecord(start_event, 0);
  // int *ptr, int nr, float *t, float *data, int *indices, float *b
  mv_GPU<<<dimGrid,dimBlock>>>(devO1Ptr,nr,devI4Ptr,devI2Ptr,devI1Ptr,devI3Ptr);
  cudaEventRecord(stop_event, 0);
  cudaEventSynchronize(stop_event);
  float *d_a = NULL;
  cudaMemcpy(d_a,devI4Ptr,nr*sizeof(float), cudaMemcpyDeviceToHost);	   //Memcopy
  cudaEventElapsedTime(&elapsed_time_gpu,start_event, stop_event);


    cudaFree(devO1Ptr);
    cudaFree(devI1Ptr);
    cudaFree(devI2Ptr);
    cudaFree(devI3Ptr);
    cudaFree(devI4Ptr);

  printf("CHECK1\n");
  int res = compare( t, d_a, nr, 0.001);
  if (res == 1) {
    printf("VALID!\n  Sequential Time: %.2f msec\n  Parallel Time: %.2f msec\n Speedup = %.2f\n", 
       elapsed_time_seq, elapsed_time_gpu, elapsed_time_seq/elapsed_time_gpu);
  }
  else printf("INVALID...\n");
}
