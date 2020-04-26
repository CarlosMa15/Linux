#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "defs.h"

#define W 4

/******************************************************
 * PINWHEEL KERNEL
 *
 * Your different versions of the pinwheel kernel go here
 ******************************************************/

/* 
 * naive_pinwheel - The naive baseline version of pinwheel 
 */
char naive_pinwheel_descr[] = "naive_pinwheel: baseline implementation";
void naive_pinwheel(pixel *src, pixel *dest)
{
  int i, j;

  for (i = 0; i < src->dim; i++)
    for (j = 0; j < src->dim; j++) {
      /* Check whether we're in the diamond region */
      if ((fabs(i + 0.5 - src->dim/2) + fabs(j + 0.5 - src->dim/2)) < src->dim/2) {
        /* In diamond region, so rotate and grayscale */
        int s_idx = RIDX(i, j, src->dim);
        int d_idx = RIDX(src->dim - j - 1, i, src->dim);
        dest[d_idx].red = ((int)src[s_idx].red + src[s_idx].green + src[s_idx].blue) / 3;
        dest[d_idx].green = ((int)src[s_idx].red + src[s_idx].green + src[s_idx].blue) / 3;
        dest[d_idx].blue = ((int)src[s_idx].red + src[s_idx].green + src[s_idx].blue) / 3;
      } else {
        /* Not in diamond region, so keep the same */
        int s_idx = RIDX(i, j, src->dim);
        int d_idx = RIDX(i, j, src->dim);
        dest[d_idx] = src[s_idx];
      }
    }
}

char my_pinwheel_descr[] = "my_pinwheel: 2 i loops with 3 j loops inside";
void my_pinwheel(pixel *src, pixel *dest)
{
  int i, j, k, l, a;
  int dim = src->dim;
  int div = dim>>1;
  int d_idx  = 1;
  int d = ((div - 1) * dim) + 1;
  l = div;
  a = 0 - div + 1;
  for (i = 0; i < div; i++){
    for(j=0;(-j>= a);j++){
      dest[d_idx] = src[d_idx++];
    }
    a++;
    k = d;
    for(;j<l;j++){
      pixel pix  = src[d_idx];
      int source = (pix.red + pix.green + pix.blue) /3;   // * 0.3333333333333333;
      pixel* p = &dest[d];
      p->red = source;
      p->green = source;
      p->blue = source;
      d -= dim;
      d_idx++;
    }
    l++;
    d = k + dim + 1;
    for(;j<dim;j++){
      dest[d_idx] = src[d_idx++];
    }
  }
  d -= dim;
  a = 0;
  l = dim - 1;
  for (i = div; i < dim; i++){
    for(j=0;-j>= a;j++){
      dest[d_idx] = src[d_idx++];
    }
    a--;
    k =  d;
    for(;j< l;j++){
      pixel pix = src[d_idx];
      int source = (pix.red + pix.green + pix.blue) / 3;   // * 0.3333333333333333;
      pixel* p = &dest[d];
      p->red = source;
      p->green = source;
      p->blue = source;
      d = d -dim;
      d_idx++;
    }
    d = k -dim + 1;
    l--;

    for(;j<dim;j++){
      dest[d_idx] = src[d_idx++];
    }
  }
}

char my1_pinwheel_descr[] = "my1_pinwheel";
void my1_pinwheel(pixel *src, pixel *dest)
{
  int i, j;
  int dim = src->dim;
  int div = dim >> 1;
  double temp = 0.5 - div;
  double temp1;

  for (i = 0; i < div; i++) {
    temp1 = -temp -i;
    for (j = 0; j < dim; j+= 4) {

      float z = (temp1 + fabs(j + 0.5 -div)) < div;
      float z1 = (temp1 + fabs(j+1 + 0.5 - div)) < div;
      float z2 = (temp1 + fabs(j+2 + 0.5 - div)) < div;
      float z3 = (temp1 + fabs(j+3 + 0.5 - div)) < div;

      int s_idx = RIDX(i, j, dim);
      int s_idx1 = RIDX(i, j+1, dim);
      int s_idx2 = RIDX(i, j+2, dim);
      int s_idx3 = RIDX(i, j+3, dim);

      int d_idx = RIDX(dim - j - 1, i, dim);
      int d_idx1 = RIDX(dim - j - 2, i, dim);
      int d_idx2 = RIDX(dim - j - 3, i, dim);
      int d_idx3 = RIDX(dim - j - 4, i, dim);

      pixel source = src[s_idx];
      pixel source1 = src[s_idx1];
      pixel source2 = src[s_idx2];
      pixel source3 = src[s_idx3];

      if(z){
	dest[d_idx].red = ((int)source.red + source.green + source.blue) / 3;
        dest[d_idx].green = ((int)source.red + source.green + source.blue) / 3;
        dest[d_idx].blue = ((int)source.red + source.green + source.blue) / 3;
      } else {
	dest[s_idx] = source;
      }

      if(z1){
	dest[d_idx1].red = ((int)source1.red + source1.green + source1.blue) / 3;
        dest[d_idx1].green = ((int)source1.red + source1.green + source1.blue) / 3;
        dest[d_idx1].blue = ((int)source1.red + source1.green + source1.blue) / 3;
      } else {
	dest[s_idx1] = source1;
      }

      if(z2){
	dest[d_idx2].red = ((int)source2.red + source2.green + source2.blue) / 3;
        dest[d_idx2].green = ((int)source2.red + source2.green + source2.blue) / 3;
        dest[d_idx2].blue = ((int)source2.red + source2.green + source2.blue) / 3;
      } else {
	dest[s_idx2] = source2;
      }

      if(z3){
	dest[d_idx3].red = ((int)source3.red + source3.green + source3.blue) / 3;
        dest[d_idx3].green = ((int)source3.red + source3.green + source3.blue) / 3;
        dest[d_idx3].blue = ((int)source3.red + source3.green + source3.blue) / 3;
      } else {
	dest[s_idx3] = source3;
      }
    }
  }

  for (; i < dim; i++) {
    temp1 = temp + i;
    for (j = 0; j < dim; j+= 4) {
      /* Check whether we're in the diamond region */
      if ((/*fabs(i + 0.5 - dim/2)*/ temp1 + fabs(j + 0.5 - div)) < div) {
        /* In diamond region, so rotate and grayscale */
        int s_idx = RIDX(i, j, dim);
        int d_idx = RIDX(dim - j - 1, i, dim);
	pixel source = src[s_idx];
        dest[d_idx].red = ((int)source.red + source.green + source.blue) / 3;
        dest[d_idx].green = ((int)src[s_idx].red + src[s_idx].green + src[s_idx].blue) / 3;
        dest[d_idx].blue = ((int)src[s_idx].red + src[s_idx].green + src[s_idx].blue) / 3;
      } else {
        /* Not in diamond region, so keep the same */
        int s_idx = RIDX(i, j, dim);
        dest[s_idx] = src[s_idx];
      }

      /* Check whether we're in the diamond region */
      if ((/*fabs(i + 0.5 - dim/2)*/ temp1 + fabs(j+1 + 0.5 - div)) < div) {
        /* In diamond region, so rotate and grayscale */
        int s_idx = RIDX(i, j + 1, src->dim);
        int d_idx = RIDX(dim - j- 1 - 1, i, dim);
	pixel source = src[s_idx];
	
        dest[d_idx].red = ((int)source.red + source.green + source.blue) / 3;
        dest[d_idx].green = ((int)src[s_idx].red + src[s_idx].green + src[s_idx].blue) / 3;
        dest[d_idx].blue = ((int)src[s_idx].red + src[s_idx].green + src[s_idx].blue) / 3;
      } else {
        /* Not in diamond region, so keep the same */
        int s_idx = RIDX(i,j + 1, dim);
        dest[s_idx] = src[s_idx];
      }

      /* Check whether we're in the diamond region */
      if ((/*fabs(i + 0.5 - dim/2)*/ temp1 + fabs(j+2 + 0.5 - div)) < div) {
        /* In diamond region, so rotate and grayscale */
        int s_idx = RIDX(i, j + 2, src->dim);
        int d_idx = RIDX(dim - j- 2 - 1, i, dim);
	pixel source = src[s_idx];
	
        dest[d_idx].red = ((int)source.red + source.green + source.blue) / 3;
        dest[d_idx].green = ((int)src[s_idx].red + src[s_idx].green + src[s_idx].blue) / 3;
        dest[d_idx].blue = ((int)src[s_idx].red + src[s_idx].green + src[s_idx].blue) / 3;
      } else {
        /* Not in diamond region, so keep the same */
        int s_idx = RIDX(i,j + 2, dim);
        dest[s_idx] = src[s_idx];
      }

      /* Check whether we're in the diamond region */
      if ((/*fabs(i + 0.5 - dim/2)*/temp1 + fabs(j+3 + 0.5 - div)) < div) {
        /* In diamond region, so rotate and grayscale */
        int s_idx = RIDX(i, j + 3, src->dim);
        int d_idx = RIDX(dim - j- 3 - 1, i, dim);
	pixel source = src[s_idx];
	
        dest[d_idx].red = ((int)source.red + source.green + source.blue) / 3;
        dest[d_idx].green = ((int)src[s_idx].red + src[s_idx].green + src[s_idx].blue) / 3;
        dest[d_idx].blue = ((int)src[s_idx].red + src[s_idx].green + src[s_idx].blue) / 3;
      } else {
        /* Not in diamond region, so keep the same */
        int s_idx = RIDX(i,j + 3, dim);
        dest[s_idx] = src[s_idx];
      }

    }
  }
}

/* 
 * pinwheel - Your current working version of pinwheel
 * IMPORTANT: This is the version you will be graded on
 */
char pinwheel_descr[] = "pinwheel: Current working version";
void pinwheel(pixel *src, pixel *dest)
{
  my_pinwheel(src, dest);
}

/*********************************************************************
 * register_pinwheel_functions - Register all of your different versions
 *     of the pinwheel kernel with the driver by calling the
 *     add_pinwheel_function() for each test function. When you run the
 *     driver program, it will test and report the performance of each
 *     registered test function.  
 *********************************************************************/

void register_pinwheel_functions() {
  add_pinwheel_function(&pinwheel, pinwheel_descr);
  add_pinwheel_function(&my_pinwheel, my_pinwheel_descr);
  add_pinwheel_function(&my1_pinwheel, my1_pinwheel_descr);
  add_pinwheel_function(&naive_pinwheel, naive_pinwheel_descr);
}


/***************************************************************
 * GLOW KERNEL
 * 
 * Starts with various typedefs and helper functions for the glow
 * function, and you may modify these any way you like.
 **************************************************************/

/* A struct used to compute averaged pixel value */
typedef struct {
  int red;
  int green;
  int blue;
} pixel_sum;

/* 
 * initialize_pixel_sum - Initializes all fields of sum to 0 
 */
static void initialize_pixel_sum(pixel_sum *sum) 
{
  sum->red = sum->green = sum->blue = 0;
}

/* 
 * accumulate_sum - Accumulates field values of p in corresponding 
 * fields of sum 
 */
static void accumulate_weighted_sum(pixel_sum *sum, pixel p, double weight) 
{
  sum->red += (int) p.red * weight;
  sum->green += (int) p.green * weight;
  sum->blue += (int) p.blue * weight;
}

/* 
 * assign_sum_to_pixel - Computes averaged pixel value in current_pixel 
 */
static void assign_sum_to_pixel(pixel *current_pixel, pixel_sum sum) 
{
  current_pixel->red = (unsigned short)sum.red;
  current_pixel->green = (unsigned short)sum.green;
  current_pixel->blue = (unsigned short)sum.blue;
}

/* 
 * weighted_combo - Returns new pixel value at (i,j) 
 */
static pixel weighted_combo(int dim, int i, int j, pixel *src) 
{
  int ii, jj;
  pixel_sum sum;
  pixel current_pixel;
  double weights[3][3] = { { 0.16, 0.00, 0.16 },
                           { 0.00, 0.30, 0.00 },
                           { 0.16, 0.00, 0.16 } };

  if ((i -1 >= 0) && (j -1 >= 0) && (i - 1 < dim) && (j -1 < dim)){
      	pixel p = src[RIDX(i-1,j-1,dim)];
      	sum.red += (int) p.red * .16;
		sum.green += (int) p.green * .16;
		sum.blue += (int) p.blue * .16;
      }

      if ((i -1 >= 0) && (j + 1 >= 0) && (i - 1 < dim) && (j + 1 < dim)){
      	pixel p = src[RIDX(i-1,j + 1,dim)];
      	sum.red += (int) p.red * .16;
		sum.green += (int) p.green * .16;
		sum.blue += (int) p.blue * .16;
      }

      if ((i >= 0) && (j >= 0) && (i < dim) && (j < dim)){
      	pixel p = src[RIDX(i,j,dim)];
      	sum.red += (int) p.red * .3;
		sum.green += (int) p.green * .3;
		sum.blue += (int) p.blue * .3;
      }

      if ((i + 1 >= 0) && (j -1 >= 0) && (i + 1 < dim) && (j -1 < dim)){
      	pixel p = src[RIDX(i + 1,j-1,dim)];
      	sum.red += (int) p.red * .16;
		sum.green += (int) p.green * .16;
		sum.blue += (int) p.blue * .16;
      }

      if ((i + 1 >= 0) && (j + 1 >= 0) && (i + 1 < dim) && (j + 1 < dim)){
      	pixel p = src[RIDX(i+1,j+1,dim)];
      	sum.red += (int) p.red * .16;
		sum.green += (int) p.green * .16;
		sum.blue += (int) p.blue * .16;
      }
  /*
  initialize_pixel_sum(&sum);
  for (ii=-1; ii < 2; ii++) {
    for (jj=-1; jj < 2; jj++) { 
      if ((i + ii >= 0) && (j + jj >= 0) && (i + ii < dim) && (j + jj < dim)) {
      	
        accumulate_weighted_sum(&sum,
                                src[RIDX(i+ii,j+jj,dim)],
                                weights[ii+1][jj+1]);
        
      }
    }
  }
  */
  
  assign_sum_to_pixel(&current_pixel, sum);

  return current_pixel;
}

/******************************************************
 * Your different versions of the glow kernel go here
 ******************************************************/

char my_glow_descr[] = "my_glow: Done Nothing";
void my_glow(pixel *src, pixel *dst) 
{
  int i, j, ii, jj, a, c, b;
  pixel_sum sum;
  pixel current_pixel;
  double weights[3][3] = { { 0.16, 0.00, 0.16 },
                           { 0.00, 0.30, 0.00 },
                           { 0.16, 0.00, 0.16 } };
 
  a = 1;
  int dim = src->dim;
  pixel p;
  c = dim - 1;
  int counter = 1;
  
  for (i = 0; i < 1; i++){
    for (j=0; j < 1; j++){

      sum.red=sum.green=sum.blue=0;
      b = RIDX(0,0,dim);
      p = src[b];
      sum.red += (int) p.red * .3;
      sum.green += (int) p.green * .3;
      sum.blue += (int) p.blue * .3;
      b = RIDX(1,1,dim);
      p = src[b];
      sum.red += (int) p.red * .16;
      sum.green += (int) p.green * .16;
      sum.blue += (int) p.blue * .16;
      current_pixel.red = (unsigned short)sum.red;
      current_pixel.green = (unsigned short)sum.green;
      current_pixel.blue = (unsigned short)sum.blue;
      dst[a++] = current_pixel;
    }

    for (; j < 2; j++){
      sum.red=sum.green=sum.blue=0;
      b = RIDX(0,1,dim);
      p = src[b];
      sum.red += (int) p.red * .3;
      sum.green += (int) p.green * .3;
      sum.blue += (int) p.blue * .3;
      b = RIDX(1,0,dim);
      p = src[b];
      sum.red += (int) p.red * .16;
      sum.green += (int) p.green * .16;
      sum.blue += (int) p.blue * .16;
      b = RIDX(1,2,dim);
      p = src[b];
      sum.red += (int) p.red * .16;
      sum.green += (int) p.green * .16;
      sum.blue += (int) p.blue * .16;
      current_pixel.red = (unsigned short)sum.red;
      current_pixel.green = (unsigned short)sum.green;
      current_pixel.blue = (unsigned short)sum.blue;
      dst[a++] = current_pixel;
    }
    for (; j < c; j++){
      sum.red=sum.green=sum.blue=0;
      b = RIDX(0,j,dim);
      p = src[b];
      sum.red += (int) p.red * .3;
      sum.green += (int) p.green * .3;
      sum.blue += (int) p.blue * .3;
      b = RIDX(1,j-1,dim);
      p = src[b];
      sum.red += (int) p.red * .16;
      sum.green += (int) p.green * .16;
      sum.blue += (int) p.blue * .16;
      b = RIDX(1,1+j,dim);
      p = src[b];
      sum.red += (int) p.red * .16;
      sum.green += (int) p.green * .16;
      sum.blue += (int) p.blue * .16;
      current_pixel.red = (unsigned short)sum.red;
      current_pixel.green = (unsigned short)sum.green;
      current_pixel.blue = (unsigned short)sum.blue;
      dst[a++] = current_pixel;
    }

    //Loop can be Improved, It has more than 1 loop
    for (; j < dim; j++){

      //printf("Counter:%d Dim:%d j:%d i:%d\n",counter++,dim,j,i);
      //printf("LOOP:%d %d\n",j,dim);
      /*
      sum.red=sum.green=sum.blue=0;
      b = RIDX(i,j,dim);
      p = src[b];
      sum.red += (int) p.red * .3;
      sum.green += (int) p.green * .3;
      sum.blue += (int) p.blue * .3;
      b = RIDX(i+1,j-1,dim);
      p = src[b];
      sum.red += (int) p.red * .16;
      sum.green += (int) p.green * .16;
      sum.blue += (int) p.blue * .16;
      b = RIDX(i+1,j+1,dim);
      p = src[b];
      sum.red += (int) p.red * .16;
      sum.green += (int) p.green * .16;
      sum.blue += (int) p.blue * .16;
      current_pixel.red = (unsigned short)sum.red;
      current_pixel.green = (unsigned short)sum.green;
      current_pixel.blue = (unsigned short)sum.blue;
      dst[a++] = current_pixel;
      */
      
      sum.red=sum.green=sum.blue=0;
      //printf("START!!!!!!!!!!!!!!!!!!!!!!!!!!!\n");
      b = RIDX(i-1,j-1,dim);
      for (ii=-1; ii < 2; ii++){
	b = RIDX(i+ii,j-1,dim);
	for (jj=-1; jj < 2; jj++) {
	  pixel p = src[b];
	  //printf("i:%d j:%d ii:%d jj:%d\n",i,j,ii,jj);
	  double weight = weights[ii+1][jj+1];
	  //if(weight == 0.0) break;
	  b++;
	  if(weight == 0.0) continue;
	  //printf("W:%f\n",weight);
	  if ((i + ii >= 0) && (j + jj >= 0) && (i + ii < dim) && (j + jj < dim)) {
	    //printf("WRITE!!!!!!!!!!!!!!!!!!!!!!!!!!!\n");
	    sum.red += (int) p.red * weight;
	    sum.green += (int) p.green * weight;
	    sum.blue += (int) p.blue * weight;
	  }
	}
      }
      //printf("END!!!!!!!!!!!!!!!!!!!!!!!!!!!\n");

      current_pixel.red = (unsigned short)sum.red;
      current_pixel.green = (unsigned short)sum.green;
      current_pixel.blue = (unsigned short)sum.blue;
      dst[a++] = current_pixel;
      
    }
  }

  //i = 1 loop!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
  for (; i < 2; i++){
    for (j=0; j < 1; j++){
      
      sum.red=sum.green=sum.blue=0;
      b = RIDX(i-1,j+1,dim);
      p = src[b];
      sum.red += (int) p.red * .16;
      sum.green += (int) p.green * .16;
      sum.blue += (int) p.blue * .16;
      b = RIDX(i,j,dim);
      p = src[b];
      sum.red += (int) p.red * .3;
      sum.green += (int) p.green * .3;
      sum.blue += (int) p.blue * .3;
      b = RIDX(i+1,j+1,dim);
      p = src[b];
      sum.red += (int) p.red * .16;
      sum.green += (int) p.green * .16;
      sum.blue += (int) p.blue * .16;
      current_pixel.red = (unsigned short)sum.red;
      current_pixel.green = (unsigned short)sum.green;
      current_pixel.blue = (unsigned short)sum.blue;
      dst[a++] = current_pixel;
      
    }
    
    for (; j < c; j++){
      sum.red=sum.green=sum.blue=0;
      b = RIDX(i-1,j-1,dim);
      p = src[b];
      sum.red += (int) p.red * .16;
      sum.green += (int) p.green * .16;
      sum.blue += (int) p.blue * .16;
      b += 2;
      p = src[b];
      sum.red += (int) p.red * .16;
      sum.green += (int) p.green * .16;
      sum.blue += (int) p.blue * .16;
      b = b + dim - 1;
      p = src[b];
      sum.red += (int) p.red * .3;
      sum.green += (int) p.green* .3;
      sum.blue += (int) p.blue * .3;
      b = b + dim- 1;
      p = src[b];
      sum.red += (int) p.red * .16;
      sum.green += (int) p.green * .16;
      sum.blue += (int) p.blue * .16;
      b += 2;
      p = src[b];
      b = b - dim - dim -1;
      sum.red += (int) p.red * .16;
      sum.green += (int) p.green * .16;
      sum.blue += (int) p.blue * .16;
      current_pixel.red = (unsigned short)sum.red;
      current_pixel.green = (unsigned short)sum.green;
      current_pixel.blue = (unsigned short)sum.blue;
      dst[a++] = current_pixel;
    }

    for (; j < dim; j++){
      sum.red=sum.green=sum.blue=0;
      b = RIDX(i-1,j-1,dim);
      for (ii=-1; ii < 2; ii++){
	b = RIDX(i+ii,j-1,dim);
	for (jj=-1; jj < 2; jj++) {
	  pixel p = src[b];
	  double weight = weights[ii+1][jj+1];
	  b++;
	  if(weight == 0.0) continue;
	  if ((i + ii >= 0) && (j + jj >= 0) && (i + ii < dim) && (j + jj < dim)) {
	    sum.red += (int) p.red * weight;
	    sum.green += (int) p.green * weight;
	    sum.blue += (int) p.blue * weight;
	  }
	}
      }

      current_pixel.red = (unsigned short)sum.red;
      current_pixel.green = (unsigned short)sum.green;
      current_pixel.blue = (unsigned short)sum.blue;
      dst[a++] = current_pixel;
    }
  }

  // i = 2 through c - 1!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
  for (; i < c; i++){
    b = RIDX(i-1,1,dim);
    for (j=0; j < 1; j++){
      sum.red=sum.green=sum.blue=0;
      p = src[b];
      sum.red += (int) p.red * .16;
      sum.green += (int) p.green * .16;
      sum.blue += (int) p.blue * .16;
      b  = b + dim - 1;
      p = src[b];
      sum.red += (int) p.red * .3;
      sum.green += (int) p.green * .3;
      sum.blue += (int) p.blue * .3;
      b += dim;
      b++;
      p = src[b];
      sum.red += (int) p.red * .16;
      sum.green += (int) p.green * .16;
      sum.blue += (int) p.blue * .16;
      current_pixel.red = (unsigned short)sum.red;
      current_pixel.green = (unsigned short)sum.green;
      current_pixel.blue = (unsigned short)sum.blue;
      dst[a++] = current_pixel;
      b = b - dim - dim -1;     
    }
    for (; j < c; j += 2){
      //1
      sum.red=sum.green=sum.blue=0;
      p = src[b];
      sum.red += (int)p.red * .16;
      sum.green += (int) p.green * .16;
      sum.blue += (int) p.blue * .16;
      b += 2;
      p = src[b];
      sum.red += (int) p.red * .16;
      sum.green += (int) p.green * .16;
      sum.blue += (int) p.blue * .16;
      b = b + dim - 1;
      p = src[b];
      sum.red += (int) p.red * .3;
      sum.green += (int) p.green* .3;
      sum.blue += (int) p.blue * .3;
      b = b + dim- 1;
      p = src[b];
      sum.red += (int) p.red * .16;
      sum.green += (int) p.green * .16;
      sum.blue += (int) p.blue * .16;
      b += 2;
      p = src[b];
      b = b - dim - dim -1;
      sum.red += (int) p.red * .16;
      sum.green += (int) p.green * .16;
      sum.blue += (int) p.blue * .16;
      current_pixel.red = (unsigned short)sum.red;
      current_pixel.green = (unsigned short)sum.green;
      current_pixel.blue = (unsigned short)sum.blue;
      dst[a++] = current_pixel;
      //2
      sum.red=sum.green=sum.blue=0;
      p = src[b];
      sum.red += (int) p.red * .16;
      sum.green += (int) p.green * .16;
      sum.blue += (int) p.blue * .16;
      b += 2;
      p = src[b];
      sum.red += (int) p.red * .16;
      sum.green += (int) p.green * .16;
      sum.blue += (int) p.blue * .16;
      b = b + dim - 1;
      p = src[b];
      sum.red += (int) p.red * .3;
      sum.green += (int) p.green* .3;
      sum.blue += (int) p.blue * .3;
      b = b + dim- 1;
      p = src[b];
      sum.red += (int) p.red * .16;
      sum.green += (int) p.green * .16;
      sum.blue += (int) p.blue * .16;
      b += 2;
      p = src[b];
      b = b - dim - dim -1;
      sum.red += (int) p.red * .16;
      sum.green += (int) p.green * .16;
      sum.blue += (int) p.blue * .16;
      current_pixel.red = (unsigned short)sum.red;
      current_pixel.green = (unsigned short)sum.green;
      current_pixel.blue = (unsigned short)sum.blue;
      dst[a++] = current_pixel;
      //3
      /*
      sum.red=sum.green=sum.blue=0;
      p = src[b];
      sum.red += (int) p.red * .16;
      sum.green += (int) p.green * .16;
      sum.blue += (int) p.blue * .16;
      b += 2;
      p = src[b];
      sum.red += (int) p.red * .16;
      sum.green += (int) p.green * .16;
      sum.blue += (int) p.blue * .16;
      b = b + dim - 1;
      p = src[b];
      sum.red += (int) p.red * .3;
      sum.green += (int) p.green* .3;
      sum.blue += (int) p.blue * .3;
      b = b + dim- 1;
      p = src[b];
      sum.red += (int) p.red * .16;
      sum.green += (int) p.green * .16;
      sum.blue += (int) p.blue * .16;
      b += 2;
      p = src[b];
      b = b - dim - dim -1;
      sum.red += (int) p.red * .16;
      sum.green += (int) p.green * .16;
      sum.blue += (int) p.blue * .16;
      current_pixel.red = (unsigned short)sum.red;
      current_pixel.green = (unsigned short)sum.green;
      current_pixel.blue = (unsigned short)sum.blue;
      dst[a++] = current_pixel;
      */
      //4
      /*
      sum.red=sum.green=sum.blue=0;
      p = src[b];
      sum.red += (int) p.red * .16;
      sum.green += (int) p.green * .16;
      sum.blue += (int) p.blue * .16;
      b += 2;
      p = src[b];
      sum.red += (int) p.red * .16;
      sum.green += (int) p.green * .16;
      sum.blue += (int) p.blue * .16;
      b = b + dim - 1;
      p = src[b];
      sum.red += (int) p.red * .3;
      sum.green += (int) p.green* .3;
      sum.blue += (int) p.blue * .3;
      b = b + dim- 1;
      p = src[b];
      sum.red += (int) p.red * .16;
      sum.green += (int) p.green * .16;
      sum.blue += (int) p.blue * .16;
      b += 2;
      p = src[b];
      b = b - dim - dim -1;
      sum.red += (int) p.red * .16;
      sum.green += (int) p.green * .16;
      sum.blue += (int) p.blue * .16;
      current_pixel.red = (unsigned short)sum.red;
      current_pixel.green = (unsigned short)sum.green;
      current_pixel.blue = (unsigned short)sum.blue;
      dst[a++] = current_pixel;
      */

    }
    for (; j < dim; j++){
      /*
      sum.red=sum.green=sum.blue=0;
      b = RIDX(i-1,j-1,dim);
      p = src[b];
      sum.red += (int) p.red * .16;
      sum.green += (int) p.green * .16;
      sum.blue += (int) p.blue * .16;
      b = RIDX(i,j,dim);
      p = src[b];
      sum.red += (int) p.red * .3;
      sum.green += (int) p.green * .3;
      sum.blue += (int) p.blue * .3;
      b = RIDX(i+1,j-1,dim);
      p = src[b];
      sum.red += (int) p.red * .16;
      sum.green += (int) p.green * .16;
      sum.blue += (int) p.blue * .16;
      b = RIDX(i+1,j+1,dim);
      p = src[b];
      sum.red += (int) p.red * .16;
      sum.green += (int) p.green * .16;
      sum.blue += (int) p.blue * .16;
      current_pixel.red = (unsigned short)sum.red;
      current_pixel.green = (unsigned short)sum.green;
      current_pixel.blue = (unsigned short)sum.blue;
      dst[a++] = current_pixel;
      */
      
      //printf("STRART!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!:%d\n",dim);
      sum.red=sum.green=sum.blue=0;
      b = RIDX(i-1,j-1,dim);
      for (ii=-1; ii < 2; ii++){
	b = RIDX(i+ii,j-1,dim);
	for (jj=-1; jj < 2; jj++) {
	  pixel p = src[b];
	  //printf("ii:%d jj:%d\n",ii,jj);
	  double weight = weights[ii+1][jj+1];
	  //printf("W:%f\n",weight);
	  b++;
	  if(weight == 0.0) continue;
	  if ((i + ii >= 0) && (j + jj >= 0) && (i + ii < dim) && (j + jj < dim)) {
	    //printf("WRITE!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!:%d\n",dim);
	    sum.red += (int) p.red * weight;
	    sum.green += (int) p.green * weight;
	    sum.blue += (int) p.blue * weight;
	  }
	}
      }
      //printf("END!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!:%d\n",dim);

      current_pixel.red = (unsigned short)sum.red;
      current_pixel.green = (unsigned short)sum.green;
      current_pixel.blue = (unsigned short)sum.blue;
      dst[a++] = current_pixel;
      
    }
  }

  //Loop and loops inside can be improved!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
  for (; i < dim; i++){
    for (j=0; j < 1; j++){
      sum.red=sum.green=sum.blue=0;
      b = RIDX(i-1,j-1,dim);
      for (ii=-1; ii < 2; ii++){
	b = RIDX(i+ii,j-1,dim);
	for (jj=-1; jj < 2; jj++) {
	  pixel p = src[b];
	  double weight = weights[ii+1][jj+1];
	  b++;
	  if(weight == 0.0) continue;
	  if ((i + ii >= 0) && (j + jj >= 0) && (i + ii < dim) && (j + jj < dim)) {
	    sum.red += (int) p.red * weight;
	    sum.green += (int) p.green * weight;
	    sum.blue += (int) p.blue * weight;
	  }
	}
      }

      current_pixel.red = (unsigned short)sum.red;
      current_pixel.green = (unsigned short)sum.green;
      current_pixel.blue = (unsigned short)sum.blue;
      dst[a++] = current_pixel;
    }
    for (; j < 2; j++){
      sum.red=sum.green=sum.blue=0;
      b = RIDX(i-1,j-1,dim);
      for (ii=-1; ii < 2; ii++){
	b = RIDX(i+ii,j-1,dim);
	for (jj=-1; jj < 2; jj++) {
	  pixel p = src[b];
	  double weight = weights[ii+1][jj+1];
	  b++;
	  if(weight == 0.0) continue;
	  if ((i + ii >= 0) && (j + jj >= 0) && (i + ii < dim) && (j + jj < dim)) {
	    sum.red += (int) p.red * weight;
	    sum.green += (int) p.green * weight;
	    sum.blue += (int) p.blue * weight;
	  }
	}
      }

      current_pixel.red = (unsigned short)sum.red;
      current_pixel.green = (unsigned short)sum.green;
      current_pixel.blue = (unsigned short)sum.blue;
      dst[a++] = current_pixel;
    }
    for (; j < c; j++){
      sum.red=sum.green=sum.blue=0;
      b = RIDX(i-1,j-1,dim);
      for (ii=-1; ii < 2; ii++){
	b = RIDX(i+ii,j-1,dim);
	for (jj=-1; jj < 2; jj++) {
	  pixel p = src[b];
	  double weight = weights[ii+1][jj+1];
	  b++;
	  if(weight == 0.0) continue;
	  if ((i + ii >= 0) && (j + jj >= 0) && (i + ii < dim) && (j + jj < dim)) {
	    sum.red += (int) p.red * weight;
	    sum.green += (int) p.green * weight;
	    sum.blue += (int) p.blue * weight;
	  }
	}
      }

      current_pixel.red = (unsigned short)sum.red;
      current_pixel.green = (unsigned short)sum.green;
      current_pixel.blue = (unsigned short)sum.blue;
      dst[a++] = current_pixel;
    }
    for (; j < dim; j++){
      sum.red=sum.green=sum.blue=0;
      b = RIDX(i-1,j-1,dim);
      for (ii=-1; ii < 2; ii++){
	b = RIDX(i+ii,j-1,dim);
	for (jj=-1; jj < 2; jj++) {
	  pixel p = src[b];
	  double weight = weights[ii+1][jj+1];
	  b++;
	  if(weight == 0.0) continue;
	  if ((i + ii >= 0) && (j + jj >= 0) && (i + ii < dim) && (j + jj < dim)) {
	    sum.red += (int) p.red * weight;
	    sum.green += (int) p.green * weight;
	    sum.blue += (int) p.blue * weight;
	  }
	}
      }

      current_pixel.red = (unsigned short)sum.red;
      current_pixel.green = (unsigned short)sum.green;
      current_pixel.blue = (unsigned short)sum.blue;
      dst[a++] = current_pixel;
    }
  }
}

/*
 * naive_glow - The naive baseline version of glow 
 */
char naive_glow_descr[] = "naive_glow: baseline implementation";
void naive_glow(pixel *src, pixel *dst) 
{
  int i, j;
    
  for (i = 0; i < src->dim; i++)
    for (j = 0; j < src->dim; j++)
      dst[RIDX(i, j, src->dim)] = weighted_combo(src->dim, i, j, src);
}

/*
 * glow - Your current working version of glow. 
 * IMPORTANT: This is the version you will be graded on
 */
char glow_descr[] = "glow: Current working version";
void glow(pixel *src, pixel *dst) 
{
  my3_glow(src, dst);
}

char my1_glow_descr[] = "my1_glow: No Mult by Zero";
void my1_glow(pixel *src, pixel *dst) 
{
  int i, j, ii, jj;
  pixel_sum sum;
  pixel current_pixel;
  int dim = src->dim;
  double weights[3][3] = { { 0.16, 0.00, 0.16 },
                           { 0.00, 0.30, 0.00 },
                           { 0.16, 0.00, 0.16 } };

  for (i = 0; i < dim; i++){
    for (j = 0; j < dim; j++){

      sum.red = sum.green = sum.blue = 0;
      for (ii=-1; ii < 2; ii++){
		
		for (jj=-1; jj < 2; jj++){
		  pixel p = src[RIDX(i+ii,j+jj,dim)];
		  double weight = weights[ii+1][jj+1];
		  if (weight == 0) continue;
		  if ((i + ii >= 0) && (j + jj >= 0) && (i + ii < dim) && (j + jj < dim)){
		    sum.red += (int) p.red * weight;
		    sum.green += (int) p.green * weight;
		    sum.blue += (int) p.blue * weight;
		  }
		}
      }

      current_pixel.red = (unsigned short)sum.red;
      current_pixel.green = (unsigned short)sum.green;
      current_pixel.blue = (unsigned short)sum.blue;

      dst[RIDX(i, j, dim)] = current_pixel;

    }
  }
}

////////////////////////////////////////////////////////////////////////////////////////////
char my2_glow_descr[] = "my2_glow: No Mult by Zero, Unroll, Blocking";
void my2_glow(pixel *src, pixel *dst) 
{
  int i, j, ii, jj;
  pixel_sum sum;
  pixel_sum sum1;
  pixel_sum sum2;
  pixel_sum sum3;
  pixel current_pixel;
  pixel current_pixel1;
  pixel current_pixel2;
  pixel current_pixel3;
  int dim = src->dim;
  int dim1 = dim - 4;
  double weights[3][3] = { { 0.16, 0.00, 0.16 },
                           { 0.00, 0.30, 0.00 },
                           { 0.16, 0.00, 0.16 } };
  
  for (i = 0; i < 4; i++){
    for (j = 0; j < dim; j++){
      sum.red = sum.green = sum.blue = 0;
      for (ii=-1; ii < 2; ii++){
	for (jj=-1; jj < 2; jj++){
	  pixel p = src[RIDX(i+ii,j+jj,dim)];
	  double weight = weights[ii+1][jj+1];
	  if (weight == 0) continue;
	  if ((i + ii >= 0) && (j + jj >= 0) && (i + ii < dim) && (j + jj < dim)){
	    sum.red += (int) p.red * weight;
	    sum.green += (int) p.green * weight;
	    sum.blue += (int) p.blue * weight;
	  }
	}
      }
      current_pixel.red = (unsigned short)sum.red;
      current_pixel.green = (unsigned short)sum.green;
      current_pixel.blue = (unsigned short)sum.blue;
      dst[RIDX(i, j, dim)] = current_pixel;
    }
  }

  for (; i < dim1; i++){
    for (j = 0; j < 4; j++){
      sum.red = sum.green = sum.blue = 0;
      for (ii=-1; ii < 2; ii++){
	for (jj=-1; jj < 2; jj++){
	  pixel p = src[RIDX(i+ii,j+jj,dim)];
	  double weight = weights[ii+1][jj+1];
	  if (weight == 0) continue;
	  if ((i + ii >= 0) && (j + jj >= 0) && (i + ii < dim) && (j + jj < dim)){
	    sum.red += (int) p.red * weight;
	    sum.green += (int) p.green * weight;
	    sum.blue += (int) p.blue * weight;
	  }
	}
      }
      current_pixel.red = (unsigned short)sum.red;
      current_pixel.green = (unsigned short)sum.green;
      current_pixel.blue = (unsigned short)sum.blue;
      dst[RIDX(i, j, dim)] = current_pixel;
    }
    ///////////////////////////////////////////////////////////////////////////////////
    /*
    for (; j < dim1; j+=2){
      sum.red=sum.green=sum.blue=0;
      sum1.red = sum1.green = sum1.blue = 0; //
      //sum2.red = sum2.green = sum2.blue = 0; //
      //sum3.red = sum3.green = sum3.blue = 0; //
      int b = RIDX(i-1,j-1,dim);
      pixel p = src[b];
      pixel p1 = src[b+1];                  //
      //pixel p2 = src[b+2];                  //
      //pixel p3 = src[b+3];                  //
      sum.red += (int) p.red * .16;
      sum.green += (int) p.green * .16;
      sum.blue += (int) p.blue * .16;
      sum1.red += (int) p1.red * .16;     //
      sum1.green += (int) p1.green * .16;  //
      sum1.blue += (int) p1.blue * .16;  //
      //sum2.red += (int) p2.red * .16;      //
      //sum2.green += (int) p2.green * .16;  //
      //sum2.blue += (int) p2.blue * .16;    //
      //sum3.red += (int) p3.red * .16;      //
      //sum3.green += (int) p3.green * .16;  //
      //sum3.blue += (int) p3.blue * .16;    //
      b = RIDX(i-1,j+1,dim);
      p = src[b];
      p1 = src[b+1];                      //
      //p2 = src[b+2];                      //
      //p3 = src[b+3];                      //
      sum.red += (int) p.red * .16;
      sum.green += (int) p.green * .16;
      sum.blue += (int) p.blue * .16;
      sum1.red += (int) p1.red * .16;      //
      sum1.green += (int) p1.green * .16;  //
      sum1.blue += (int) p1.blue * .16;    //
      //sum2.red += (int) p2.red * .16;      //
      //sum2.green += (int) p2.green * .16;  //
      //sum2.blue += (int) p2.blue * .16;    //
      //sum3.red += (int) p3.red * .16;      //
      //sum3.green += (int) p3.green * .16;  //
      //sum3.blue += (int) p3.blue * .16;    //
      b = RIDX(i,j,dim);
      p = src[b];
      p1 = src[b+1];                      //
      //p2 = src[b+2];                      //
      //p3 = src[b+3];                      //
      sum.red += (int) p.red * .3;
      sum.green += (int) p.green* .3;
      sum.blue += (int) p.blue * .3;
      sum1.red += (int) p1.red * .3;      //
      sum1.green += (int) p1.green * .3;  //
      sum1.blue += (int) p1.blue * .3;    //
      //printf("BLUE:%d MULT:%d DIV:%d DIM:%d\n",(int)(p.blue),(int)(p.blue * .16),(int)(p.blue /6.25),dim);
      //sum2.red += (int) p2.red * .3;      //
      //sum2.green += (int) p2.green * .3;  //
      //sum2.blue += (int) p2.blue * .3;    //
      //sum3.red += (int) p3.red * .3;      //
      //sum3.green += (int) p3.green * .3;  //
      //sum3.blue += (int) p3.blue * .3;    //
      b = RIDX(i+1,j-1,dim);
      p = src[b];
      p1 = src[b+1];                       //
      //p2 = src[b+2];                       //
      //p3 = src[b+3];                       //
      sum.red += (int) p.red * .16;
      sum.green += (int) p.green * .16;
      sum.blue += (int) p.blue * .16;
      sum1.red += (int) p1.red * .16;      //
      sum1.green += (int) p1.green * .16;  //
      sum1.blue += (int) p1.blue * .16;    //
      //sum2.red += (int) p2.red * .16;      //
      //sum2.green += (int) p2.green * .16;  //
      //sum2.blue += (int) p2.blue * .16;    //
      //sum3.red += (int) p3.red * .16;      //
      //sum3.green += (int) p3.green * .16;  //
      //sum3.blue += (int) p3.blue * .16;    //
      b = RIDX(i+1,j+1,dim);
      p = src[b];
      p1 = src[b+1];                       //
      //p2 = src[b+2];                       //
      //p3 = src[b+3];                       //
      //b = b - dim - dim -1;
      sum.red += (int) p.red * .16;
      sum.green += (int) p.green * .16;
      sum.blue += (int) p.blue * .16;
      sum1.red += (int) p1.red * .16;      //
      sum1.green += (int) p1.green * .16;  //
      sum1.blue += (int) p1.blue * .16;    //
      //sum2.red += (int) p2.red * .16;      //
      //sum2.green += (int) p2.green * .16;  //
      //sum2.blue += (int) p2.blue * .16;    //
      //sum3.red += (int) p3.red * .16;      //
      //sum3.green += (int) p3.green * .16;  //
      //sum3.blue += (int) p3.blue * .16;    //
      current_pixel.red = (unsigned short)sum.red;
      current_pixel.green = (unsigned short)sum.green;
      current_pixel.blue = (unsigned short)sum.blue;
      current_pixel1.red = (unsigned short)sum1.red;     //
      current_pixel1.green = (unsigned short)sum1.green; //
      current_pixel1.blue = (unsigned short)sum1.blue;   //
      //current_pixel2.red = (unsigned short)sum2.red;     //
      //current_pixel2.green = (unsigned short)sum2.green; //
      //current_pixel2.blue = (unsigned short)sum2.blue;   //
      //current_pixel3.red = (unsigned short)sum3.red;     //
      //current_pixel3.green = (unsigned short)sum3.green; //
      //current_pixel3.blue = (unsigned short)sum3.blue;   //
      dst[RIDX(i, j, dim)] = current_pixel;
      dst[RIDX(i, j+1, dim)] = current_pixel1;
      //dst[RIDX(i, j+2, dim)] = current_pixel2;
      //dst[RIDX(i, j+3, dim)] = current_pixel3;
      */
    for (; j < dim1; j+=4){
      sum.red = sum.green = sum.blue = 0;
      sum1.red = sum1.green = sum1.blue = 0;
      sum2.red = sum2.green = sum2.blue = 0;
      sum3.red = sum3.green = sum3.blue = 0;
      for (ii=-1; ii < 2; ii++){
	for (jj=-1; jj < 2; jj++){
	  int a = RIDX(i+ii,j+jj,dim);
	  //printf("RIDX:%d DIM:%d\n",RIDX(i+ii,j+jj,dim),dim);
	  pixel p = src[a];  //RIDX(i+ii,j+jj,dim)];
	  //printf("RIDX:%d DIM:%d\n",RIDX(i+ii,j+jj+1,dim),dim);
	  pixel p1 = src[a+1]; //RIDX(i+ii,j+jj+1,dim)];
	  //printf("RIDX:%d DIM:%d\n",RIDX(i+ii,j+jj+2,dim),dim);
	  pixel p2 = src[a+2]; //RIDX(i+ii,j+jj+2,dim)];
	  //printf("RIDX:%d DIM:%d\n",RIDX(i+ii,j+jj+3,dim),dim);
	  pixel p3 = src[a+3]; //RIDX(i+ii,j+jj+3,dim)];
	  double weight = weights[ii+1][jj+1];
	  if (weight == 0) continue;

	  sum.red += (int) p.red * weight;
	  sum.green += (int) p.green * weight;
	  sum.blue += (int) p.blue * weight;
	  sum1.red += (int) p1.red * weight;
	  sum1.green += (int) p1.green * weight;
	  sum1.blue += (int) p1.blue * weight;
	  sum2.red += (int) p2.red * weight;
	  sum2.green += (int) p2.green * weight;
	  sum2.blue += (int) p2.blue * weight;
	  sum3.red += (int) p3.red * weight;
	  sum3.green += (int) p3.green * weight;
	  sum3.blue += (int) p3.blue * weight;
	}
      }
      current_pixel.red = (unsigned short)sum.red;
      current_pixel.green = (unsigned short)sum.green;
      current_pixel.blue = (unsigned short)sum.blue;
      current_pixel1.red = (unsigned short)sum1.red;
      current_pixel1.green = (unsigned short)sum1.green;
      current_pixel1.blue = (unsigned short)sum1.blue;
      current_pixel2.red = (unsigned short)sum2.red;
      current_pixel2.green = (unsigned short)sum2.green;
      current_pixel2.blue = (unsigned short)sum2.blue;
      current_pixel3.red = (unsigned short)sum3.red;
      current_pixel3.green = (unsigned short)sum3.green;
      current_pixel3.blue = (unsigned short)sum3.blue;
      int a = RIDX(i, j, dim);
      dst[a] = current_pixel;     //RIDX(i, j, dim)
      dst[a+1] = current_pixel1;  //RIDX(i, j+1, dim)
      dst[a+2] = current_pixel2;  //RIDX(i, j+2, dim)
      dst[a+3] = current_pixel3;  //RIDX(i, j+3, dim)


    }

    /////////////////////////////////////////////////////////////////////////////////
    for (; j < dim; j++){
      sum.red = sum.green = sum.blue = 0;
      for (ii=-1; ii < 2; ii++){
	for (jj=-1; jj < 2; jj++){
	  pixel p = src[RIDX(i+ii,j+jj,dim)];
	  double weight = weights[ii+1][jj+1];
	  if (weight == 0) continue;
	  if ((i + ii >= 0) && (j + jj >= 0) && (i + ii < dim) && (j + jj < dim)){
	    sum.red += (int) p.red * weight;
	    sum.green += (int) p.green * weight;
	    sum.blue += (int) p.blue * weight;
	  }
	}
      }
      current_pixel.red = (unsigned short)sum.red;
      current_pixel.green = (unsigned short)sum.green;
      current_pixel.blue = (unsigned short)sum.blue;
      dst[RIDX(i, j, dim)] = current_pixel;
    }
  }
  for (; i < dim; i++){
    for (j = 0; j < dim; j++){
      sum.red = sum.green = sum.blue = 0;
      for (ii=-1; ii < 2; ii++){
	for (jj=-1; jj < 2; jj++){
	  pixel p = src[RIDX(i+ii,j+jj,dim)];
	  double weight = weights[ii+1][jj+1];
	  if (weight == 0) continue;
	  if ((i + ii >= 0) && (j + jj >= 0) && (i + ii < dim) && (j + jj < dim)){
	    sum.red += (int) p.red * weight;
	    sum.green += (int) p.green * weight;
	    sum.blue += (int) p.blue * weight;
	  }
	}
      }
      current_pixel.red = (unsigned short)sum.red;
      current_pixel.green = (unsigned short)sum.green;
      current_pixel.blue = (unsigned short)sum.blue;
      dst[RIDX(i, j, dim)] = current_pixel;
    }
  }
}

char my3_glow_descr[] = "my3_glow";
void my3_glow(pixel *src, pixel *dst) 
{
  int i, j, ii, jj;
  pixel_sum sum;
  pixel current_pixel;
  int dim = src->dim;
  

  for (i = 0; i < dim; i+=W){
    for (j = 0; j < dim; j+=W){
      for(ii = i;ii < i + W;ii++) {
      	for(jj = j;jj < j + W;jj++) {

	      sum.red = sum.green = sum.blue = 0;

	      if ((ii -1 >= 0) && (jj -1 >= 0) && (ii - 1 < dim) && (jj -1 < dim)){
	      	pixel p = src[RIDX(ii-1,jj-1,dim)];
	      	sum.red +=  (p.red << 2) /25;
			sum.green +=  (p.green << 2) /25;
			sum.blue +=  (p.blue << 2) / 25;
	      }

	      if ((ii -1 >= 0) && (jj + 1 >= 0) && (ii - 1 < dim) && (jj + 1 < dim)){
	      	pixel p = src[RIDX(ii-1,jj + 1,dim)];
	      	sum.red +=  (p.red << 2) /25;
			sum.green +=  (p.green << 2) /25;
			sum.blue +=  (p.blue << 2) / 25;
	      }

	      if ((ii >= 0) && (jj >= 0) && (ii < dim) && (jj < dim)){
	      	pixel p = src[RIDX(ii,jj,dim)];
	      	sum.red += p.red * .3;
			sum.green += p.green * .3;
			sum.blue += p.blue * .3;
	      }

	      if ((ii + 1 >= 0) && (jj -1 >= 0) && (ii + 1 < dim) && (jj -1 < dim)){
	      	pixel p = src[RIDX(ii + 1,jj-1,dim)];
	      	sum.red +=  (p.red << 2) /25;
			sum.green +=  (p.green << 2) /25;
			sum.blue +=  (p.blue << 2) / 25;
	      }

	      if ((ii + 1 >= 0) && (jj + 1 >= 0) && (ii + 1 < dim) && (jj + 1 < dim)){
	      	pixel p = src[RIDX(ii+1,jj+1,dim)];
	      	sum.red +=  (p.red << 2) /25;
			sum.green +=  (p.green << 2) /25;
			sum.blue +=  (p.blue << 2) / 25;
	      }

	      current_pixel.red = (unsigned short)sum.red;
	      current_pixel.green = (unsigned short)sum.green;
	      current_pixel.blue = (unsigned short)sum.blue;

	      dst[RIDX(ii, jj, dim)] = current_pixel;
   		}		
	  }
    }
  }
}


/********************************************************************* 
 * register_glow_functions - Register all of your different versions
 *     of the glow kernel with the driver by calling the
 *     add_glow_function() for each test function.  When you run the
 *     driver program, it will test and report the performance of each
 *     registered test function.  
 *********************************************************************/

void register_glow_functions() {
  add_glow_function(&glow, glow_descr);
  add_glow_function(&naive_glow, naive_glow_descr);
  add_glow_function(&my_glow, my_glow_descr);
  add_glow_function(&my1_glow, my1_glow_descr);
  add_glow_function(&my2_glow, my2_glow_descr);
  add_glow_function(&my3_glow, my3_glow_descr);
}

      /*
      sum.red=sum.green=sum.blue=0;
      int b = RIDX(i-1,j-1,dim);
      pixel p = src[b];
      sum.red += (int) p.red * .16;
      sum.green += (int) p.green * .16;
      sum.blue += (int) p.blue * .16;
      b = RIDX(i-1,j+1,dim);
      p = src[b];
      sum.red += (int) p.red * .16;
      sum.green += (int) p.green * .16;
      sum.blue += (int) p.blue * .16;
      b = RIDX(i,j,dim);
      p = src[b];
      sum.red += (int) p.red * .3;
      sum.green += (int) p.green* .3;
      sum.blue += (int) p.blue * .3;
      b = RIDX(i+1,j-1,dim);
      p = src[b];
      sum.red += (int) p.red * .16;
      sum.green += (int) p.green * .16;
      sum.blue += (int) p.blue * .16;
      b = RIDX(i+1,j+1,dim);
      p = src[b];
      b = b - dim - dim -1;
      sum.red += (int) p.red * .16;
      sum.green += (int) p.green * .16;
      sum.blue += (int) p.blue * .16;
      current_pixel.red = (unsigned short)sum.red;
      current_pixel.green = (unsigned short)sum.green;
      current_pixel.blue = (unsigned short)sum.blue;
      dst[RIDX(i, j, dim)] = current_pixel;
      */
