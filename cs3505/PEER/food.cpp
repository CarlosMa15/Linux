// **** Authored by Matthews Hooper and Pham ****
// **** Assignment04 for CS 3505 Spring 2018 ****

#include "food.h"

// default ctor
food::food()
{

}

// standard ctor
food::food(std::string n, int sl)
{
  name = n;
  shelf_life = sl;
}

// copy constructor
food::food(const food & other)
{
  name = other.name;
  shelf_life = other.shelf_life;
}
//return the string from the food
std::string food::get_name()
{
  return name;
}
//return the int for shelf life
int food::get_shelf_life()
{
  return shelf_life;
}
