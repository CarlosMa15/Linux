/*
 *Assignemt 4
 *Carlos Martinez & Lukas Gust
 *March 07, 2018
 */

#include "foodItem.h"

/*
 *Default ctor for a food item with bogus values
 */
foodItem::foodItem(){
  this->upc = "0";
  this->shelfLife = 0;
  this->name = "";
}

/*
 *Ctor using unique upc, initial shelfLife and its name.
 */
foodItem::foodItem(std::string upc, int shelfLife, std::string name){
  this->upc = upc;
  this->shelfLife = shelfLife;
  this->name = name;
}

/*
 *Accessor for the upc of a foodItem
 */
std::string foodItem::getUPC(){
  return this->upc;
}

/*
 *Accessor for the shelflife of a foodItem. This does not remain constant.
 */
int foodItem::getShelfLife(){
  return this->shelfLife;
}

/*
 *Accessor for the name of a foodItem
 */
std::string foodItem::getName(){
  return this->name;
}

/*
 *Modifies the life to the given parameter
 */
void foodItem::setLife(int newLife)
{
  shelfLife = newLife;
}
