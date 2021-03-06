/*
  Midterm 1 fraction class
*/

#ifndef FRACTION_H
#define FRACTION_H

class fraction
{
  friend class measurement;

 private:
  int numerator;
  int denominator;
  void reduce();
  
 public:
   fraction(int numerator, int denominator);
   const fraction operator+(const fraction & rhs) const;
   const fraction operator-(const fraction & rhs) const;
   const fraction operator/(const fraction & rhs) const;

};

#endif
