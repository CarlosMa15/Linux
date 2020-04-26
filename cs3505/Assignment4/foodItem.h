/*
 *Assignemt 4
 *Carlos Martinez & Lukas Gust
 *March 07, 2018
 */

#ifndef FOODITEM_H
#define FOODITEM_H

#include <string>

/*
 *Represents a food item with a upc and a name and its current shelf life
 */
class foodItem {
  private:
    std::string upc;
    int shelfLife; //current shelf life
    std::string name;

  public:
    foodItem();
    foodItem(std::string upc, int shelfLife, std::string name);
    std::string getUPC();
    int getShelfLife();
    std::string getName();
    void setLife(int newLife);
};

#endif
