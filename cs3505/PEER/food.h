// **** Authored by Matthews Hooper and Pham ****
// **** Assignment04 for CS 3505 Spring 2018 ****

#ifndef FOOD_H
#define FOOD_H

#include <string>

class food
{
private:
  std::string name;
  int shelf_life;

public:
  // default ctor
  food();

  // standard ctor
  food(std::string name, int shelf_life);

  // copy ctor
  food(const food & other);

  // accessor for name
  std::string get_name();

  // accessor for shelf life
  int get_shelf_life();
};

#endif
