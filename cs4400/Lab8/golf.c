#include <stdlib.h>
#include <stdio.h>

int main(int argc, char const *argv[])
{
  int firstchild = fork();

  if( firstchild == 0)
  {
    // Do work here to fork to create target pstree diagram
    /*
    //Default
    fork();
    */
    
    /*
    //First
    int a = fork();
    if(a) {
      int b = fork();
    }
    */
    
    /*
    //Third
    int a = fork();
    if(a) {
      int b = fork();
      if(b) {
	int c = fork();
      } else {
	fork();
      }
    } else {
      int d = fork();
      if(d) {
	fork();
      } else {
	fork();
      }
    }
    */

    //Forth
    int a = fork();
    if(a) {
      int b = fork();
      if(b) {
	//int c = fork(); //4,1 X
      } else {
	int c= fork(); // 3,2
	if(c) {
	  fork(); // new 4,1
	} else {
	  fork(); // 3,3
	}
      }
    } else {
      int d = fork();
      if(d) {
	int y = fork(); // 2,1
	if(!y) {
	  fork();
	}
      } else {
	int z = fork(); // 1,4
	if(z) {
	  //fork(); // in front of 2,1 X
	} else {
	  fork(); // 1,5
	}
      }
    }

    // Put all calls to fork() above this line, and don't modify below.

    sleep(2); /* required so that execve will run before the process quits, don't do this sort of thing in real code */
    return 0;
  }
  else 
  {
    sleep(1); /* make it so child finishes forking before calling execve */


    // parent process calls execve on pstree

    char pid[20]; // This is where we will put the pid argument to the pstree execution
    char* args[] = {"/usr/bin/pstree", "-p" , pid, NULL};
    char* environ[] = {NULL};

    sprintf(pid, "%u", firstchild);
    execve( args[0], args, environ);
  }
}
