#include<stdio.h>

int main(int argc, char** argv){
  char *name = argc[1];
  printf("Hello %s\n",argv[1]);
  printf("Hello %s\n",argv[2]);
  
  return 0;
}
