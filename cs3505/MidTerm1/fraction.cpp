#include "fraction.h"
#include <iostream>

using namespace std;

fraction::fraction(int numerator, int denominator)
{
  this->numerator = numerator;
  this->denominator = denominator;
}

const fraction fraction::operator+(const fraction & rhs) const
{
  int num1 = this->numerator;
  int num2 = rhs.numerator;
  int den1 = this->denominator;
  int den2 = rhs.denominator;

  int num3 = num1 + num2;
  int den3 = den1 + den2;

  return fraction(num3,den3);
}

const fraction fraction::operator-(const fraction & rhs) const
{
  int num1 = this->numerator;
  int num2 = rhs.numerator;
  int den1 = this->denominator;
  int den2 = rhs.denominator;

  int num3 = num1 + num2;
  int den3 = den1 + den2;

  return fraction(num3,den3);
}

const fraction fraction::operator/(const fraction & rhs) const
{
  int num1 = this->numerator;
  int num2 = rhs.numerator;
  int den1 = this->denominator;
  int den2 = rhs.denominator;

  int num3 = num1 + num2;
  int den3 = den1 + den2;

  return fraction(num3,den3);
}
