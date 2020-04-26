#include <string>

/*
  CS 3505 - Assignment #2

  This  is the unbounded .h class
  An unbounded is a representation of a positive Integer
*/
class unbounded
{
 private:
  /*
    String that holds the number
  */
  std::string number;

 public:
  /*
    no argument constructor
  */
  unbounded();

  /*
    String argument constructor
  */
  unbounded(std::string _number);
  
  /*
    This get method ment to be used to get the value of
    The unbounded
  */
  std::string get_value() const;

  /*
    This overrided the + operator
  */
  const unbounded operator+(const unbounded & rhs) const;

  /*
    This overrided the * operator
  */
  const unbounded operator*(const unbounded & rhs) const;

  /*
    This overrided the - operator
  */
  const unbounded operator-(const unbounded & rhs) const;

  /*
    This overrided the / operator
  */
  const unbounded operator/(const unbounded & rhs) const;

  /*
    This overrided the % operator
  */
  const unbounded operator%(const unbounded & rhs) const;

  /*
    creates a new unbounded from another unbounded
  */
  unbounded(const unbounded & other);

  /*
    This overrided the = operator
  */
  unbounded & operator=(const unbounded & rhs);
};
