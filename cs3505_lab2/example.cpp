#include<iostream>

using namespace std;

void swap(int *x, int *y)
{
  int temp = *x;
  *x = *y;
  *y = temp;
}

int main()
{
  int a = 10;
  int b = 20;

  cout<<"a = "<<a<<" b = "<<b<<endl;
  
  swap(&a,&b);

  cout<<"SWAPED"<<endl;

  cout<<"a = "<<a<<" b = "<<b<<endl;
}
