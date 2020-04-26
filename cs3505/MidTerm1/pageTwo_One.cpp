#include <iostream>

using namespace std;

int main()
{
  char* array = new char[100];

  for(int i = 0;i < 100;i++)
  {
    array[i] = 'X';
    cout<<i + 1<<" : "<<array[i]<<endl;
  }

  delete [] array;
  cout<<"Deleted"<<endl;
}
