#include "unbounded.h"
#include <string>
#include <iostream>
#include <sstream>

/*
  This .cpp class takes the unbounded.h class and implements
  the given methods. Based off our implementation, an unbounded
  is like a presentation of a whole number. anything that is
  negative becomes zero. unbounded allow +, -, *, %, and =
  operations. This .cpp class passes the tester.o tests
*/

using namespace std;

/*
 unbounded contrsuctor from another unbounded
*/
unbounded::unbounded(const unbounded & other)
{
  this->number = other.number;
}

/*
  This methods overrides the % operator
*/
const unbounded unbounded::operator%(const unbounded & rhs) const
{
  string left = this->number;
  string right = rhs.number;

  unbounded divident(left);
  unbounded divisor(right);

  // This uses the / operator to see how many divisors fit
  // into the divident
  unbounded temp = divident / divisor;

  // This uses the * operator to get the right amount to
  // subtract from the divident
  unbounded temp1 = temp * divisor;

  // This gets the remainder from subtracting the right amount
  unbounded temp2 = divident - temp1;

  return temp2;
}

/*
  This overrides the / operator
*/
const unbounded unbounded::operator/(const unbounded & rhs) const
{
  string left = this->number;
  string right = rhs.number;

  unbounded divident(left);
  unbounded divisor(right);

  // Checks if they are the same
  if(divident.get_value() == divisor.get_value()) {
    return unbounded("1");

  // Check if divisor is greater than
  } else if(divident.get_value().length() < divisor.get_value().length()) {
    return unbounded("0");

  // Checks if divisor are same length than if greater than
  } else if(divident.get_value().length() == divisor.get_value().length() 
    && divident.get_value() < divisor.get_value()){
    return unbounded("0");

  // Checks what the Quotient is
  } else {

    int counter = 1;
    int done = 0;

    /*
      This while loops checks how many divisors fit into the quotient
      Before it out grows it. keeping track of it with a counter that
      is the solution to the division problem
    */
    while(done == 0){
      stringstream ss;
      ss << counter;
      string counterString = ss.str();
      unbounded temp(counterString);
      unbounded temp1 = divisor * temp;
      counter += 1;
      
      //Checks if they are equal
      if(temp1.get_value().length() == divident.get_value().length() 
	 && temp1.get_value() > divident.get_value()){
	done = 1;
	counter -= 1;
      // Checks if greater than
      } else if(temp1.get_value().length() > divident.get_value().length()){
	done = 1;
      // Checks if equal length but greater
      } else if(temp1.get_value() == divident.get_value()){
	done = 1;
      }
    }
    
    // Returns the counter with correct answer
    counter -= 1;
    stringstream ss;
    ss << counter;
    string counterString = ss.str();
    return unbounded(counterString);
  }
}

/*
 * This overrides the - operator to subtract two unboundeds
*/
const unbounded unbounded::operator-(const unbounded & rhs) const
{
  string left = this->number;
  string right = rhs.number;

  if(right.length() > left.length()){
    return unbounded("0");
  } else if(right.length() == left.length() 
	    && right > left){
    return unbounded("0");
  } else {

    // Start with an empty string to store the result
    string result = "";
  
    // We'll go through digits right-to-left, so start at the end.
    int left_pos  = left.length()  - 1;
    int right_pos = right.length() - 1;

    int carry = 0;

    // Loop, adding columns until no more digits remain.
    while (carry > 0 || left_pos >=  0 || right_pos >=  0)
    {
      // Get the digit from each string (and retreat the index), or get a 0 
      // if no digits remain.
      int left_digit;
      int right_digit;
      if(left_pos >= 0){
	left_digit  = left[left_pos--] - '0';
      } else {
	left_digit = 0;
	left_pos -= 1;
      }

      if(right_pos >= 0){
	right_digit = right[right_pos--] - '0';
      }else {
	right_digit = 0;
	right_pos -= 1;
      }

      // If you borrowed you have to subtract one from the number
      if(carry == 1) {
	left_digit -= 1;
	carry = 0;
      }
      
      // If borrowed you need to borrow again
      if(right_digit > left_digit && carry == 1)
      {
	left_digit += 10;
      } else if(right_digit > left_digit && ++left_pos > 0){
	carry = 1;
	left_digit += 10;
	left_pos -= 1;
      }
      
      // If at the end carry needs to be 0 to stop looping
      if(left_pos <= 0 && right_pos <= 0)
      {
	carry = 0;
      }
      // calculates sum
      int sum = left_digit - right_digit;
      
      // if sum is negative the overall sum it negative
      // making the unbounded zero
      if(sum < 0)
      {
	return unbounded("0");
      }
      
      // Adds number to the string
      stringstream ss;
      ss << sum;
      string sumString = ss.str();
      result = sumString + result;
    }

    // Strip out any leading 0's from our result (but leave at least one 
    // digit).
    // (Only useful for subtraction, but I'm giving it to you here.)
    while (result[0] == '0' && result.length() > 1)
    result.erase(0, 1);

    return result;
  }
}

/*
 * This is just a default contructor set it to zero
*/
unbounded::unbounded()
{
  this->number = "0";
}

/*
 *This is just a contructor with one string pirameter
*/
unbounded::unbounded(string _number)
{
  this->number = _number;
}

/*
 * This is just a method that returns the string of the number
 * This is just like a toString method
*/
string unbounded::get_value() const
{
  return this->number;
}

/*
 * This method just overrides the + operator in the assignment
 * Adds to string integers, and returns a new string out of it
*/
const unbounded unbounded::operator+(const unbounded & rhs) const
{

  string left = this->number;
  string right = rhs.number;
  
  // Start with an empty string to store the result
  string result = "";
  
  // We'll go through digits right-to-left, so start at the end.
  int left_pos  = left.length()  - 1;
  int right_pos = right.length() - 1;
  int carry = 0;
  
  // Loop, adding columns until no more digits remain.
  while (carry > 0 || left_pos >= 0 || right_pos >= 0)
  {
    // Get the digit from each string (and retreat the index), or get a 0 
    // if no digits remain.
    int left_digit  = left_pos  >= 0 ? left[left_pos--]   - '0' : 0;
    int right_digit = right_pos >= 0 ? right[right_pos--] - '0' : 0;
    
    // Calcuate the sum of the digits.
    int sum = carry + left_digit + right_digit;
    carry = sum / 10;
    sum   = sum % 10;
    
    // Put the sum into the new string (at the left side)
    result.insert (0, 1, (char)('0'+sum) );
  }
  
  // Strip out any leading 0's from our result (but leave at least one 
  // digit).
  // (Only useful for subtraction, but I'm giving it to you here.)
  while (result[0] == '0' && result.length() > 1)
    result.erase(0, 1);

  return result;
}

/*
*  Multiplies two integers stored in strings, building a string result.
*  The algorithm is long multiplication, starting with the most 
*  significant
*  digit of the multiplier.
*
*  Note:  It is safe to modify the parameter objects below because they
*  are local variables and local string instances.  They are not
*  references.
*
* This is just the same code that the teacher provided just with
* a little modification.
*/
const unbounded unbounded::operator*(const unbounded & rhs) const
{
  // Perform long multiplication.  The result is the working sum and
  // will be the product.  Do a small example on paper to see this
  // algorithm work.
  string left = this->number;
  string right = rhs.number;
  string result = "0";

  int right_pos = 0;

  while (right_pos < right.length())
  {
    result.append("0");  // Multiply the product by 10.
    int right_digit = right[right_pos++] - '0';
    for (int i = 0; i < right_digit; i++){
      unbounded _left(left);
      unbounded _result(result);
      _result = _result + _left;
      result = _result.get_value();
    }
  }
  return result;
}

/*
  This method overrides the = operator, simple
*/
unbounded & unbounded::operator=(const unbounded & rhs)
{
  this->number = rhs.number;
}
