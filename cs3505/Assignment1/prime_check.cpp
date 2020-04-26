#include<iostream>
#include<stdlib.h>

using namespace std;

/*
Author: Carlos Martines
Class: Computer Science 3505
Assignment: Assignment #1

This Program Takes A Number As An Argument
And Lets the User Know If The Number Is Prime
Or Composite. The Number Zero And One Are Not
Considered To Be Prime Nor Composite. The
Algorithm is Imspired For The Primality Test
From Wikipedia Sudo Code And Other Sudo Code
From Multiply Yahoo Answer Forms.
*/
int main(int argc, char* argv[]){
  int number = 0;
  bool isPrime = true; //Prime Checker

  if(argc < 2){
    cout<<"Invalid Number of Arguments"<<endl;
  } else {
    number = atoi(argv[1]);

    // This checks if a number is prime
    for(int i =2; i <= number / 2; i++){
      if(number % i == 0) {
        isPrime = false;
        break;
      }
    }
  }

  // This Displays the Output
  if(argc > 2){
    cout<<"Invalid Number of Arguments"<<endl;
  }else if(number == 0 && argc == 2) {
    cout<<"Composite"<<endl;
  }else if(number == 1 && argc == 2){
    cout<<"Composite"<<endl;
  } else if(number < 0 && argc == 2){
    cout<<"Composite"<<endl;
  } else if(isPrime && argc == 2){
    cout<<"Prime"<<endl;
  } else if(argc == 2){
    cout<<"Composite"<<endl;
  }

  return 0;
}
