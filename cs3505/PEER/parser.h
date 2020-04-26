// **** Authored by Matthews Pham and Hooper ****
// **** Assignment04 for CS 3505 Spring 2018 ****

#ifndef PARSER_H
#define PARSER_H

#include <string>
#include "food.h"
#include <map>
#include <boost/date_time/gregorian/gregorian.hpp>
class parser
{

public:

  void get_food(std::string const& s, std::map<std::string, food> & food_items);
  
  std::string getFirstWord(std::string const& s);
  
  void get_warehouse(std::string const& s, std::map<std::string, std::map<std::string, std::map<boost::gregorian::date, long long > > > & warehouses);
  
  boost::gregorian::date getStart(std::string const& s);

  std::string parse_date(boost::gregorian::date const& _date);
};
 
#endif
