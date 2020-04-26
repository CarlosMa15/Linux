INCLUDE=-I/usr/local/cuda/8.0/cuda/include 

SOURCE=sparsematvec.cu
#EXECUTABLE=dense

#SOURCE=spmv.cu
EXECUTABLE=sparse

$(EXECUTABLE): $(SOURCE)
	nvcc $(INCLUDE) $< -o $@ 

clean:
