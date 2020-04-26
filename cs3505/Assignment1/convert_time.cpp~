#include<iostream>
#include<cmath>
#include<iomanip>

using namespace std;

/*
Author: Carlos Martinez
Class: Computer Science 3505
Assignment: Assignment #1

This Program Asks From A Positive Number In Seconds
The Outputs the Time In The Form Of HH:MM:SS, This Does
Not Deal With Negative Nor Huges Amounts Of Time Given
*/
int main() {
  // Variables That Keep Track Of Info
  double totalSeconds;
  int time;
  int hours;
  int minutes;
  int seconds;

  // Get Input From User Saved in Time
  cout<<"Enter a positive time duration in seconds: ";
  cin>>totalSeconds;
  time =((int)round(totalSeconds));

  if(totalSeconds >= 0){
    cout<<time<<" seconds ---> ";
  }

  // Calculates The Time Info Individual Parts
  hours = time / 3600;
  time = time % 3600;
  minutes = time / 60;
  time = time % 60;
  seconds = round(time);
  
  if (totalSeconds < 0) {
    cout<<"Negative numbers are not allowed"<<endl;
  } else {
    // This Outputs And Formats The Given Information
    cout<<setw(2)<<setfill('0')<<hours<<":"
      <<setw(2)<<setfill('0')<<minutes<<":"
      <<setw(2)<<setfill('0')<<seconds<<endl;
  }
  return 0;
}
