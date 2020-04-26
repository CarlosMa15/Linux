#include "unbounded.h"
#include <iostream>

using namespace std;

int main()
{
  unbounded x("564675879");
  unbounded y("7867");

  unbounded z = x + y;
  unbounded a = x * y;
  unbounded c = y - x;
  unbounded d = x - y;
  unbounded e = x / y;
  unbounded f = y / x;
  unbounded i = x % y;
  unbounded g = y % x;
  unbounded b = y;

  cout<<endl<<"X = "<<x.get_value()<<endl;
  cout<<"Y = "<<y.get_value()<<endl;
  cout<<"B = Y: "<<b.get_value()<<endl;
  cout<<"Z = X + Y: "<< z.get_value()<<endl;
  cout<<"A = X * Y: "<< a.get_value()<<endl;
  cout<<"C = Y - X: "<< c.get_value()<<endl;
  cout<<"D = X - Y: "<< d.get_value()<<endl;
  cout<<"E = X / Y: "<< e.get_value()<<endl;
  cout<<"F = Y / X: "<< f.get_value()<<endl;
  cout<<"I = X % Y: "<< i.get_value()<<endl;
  cout<<"G = Y % X: "<< g.get_value()<<endl;
  
  return 0;
}
