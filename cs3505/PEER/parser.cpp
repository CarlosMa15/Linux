// **** Authored by Matthews Pham and Hooper ****
// **** Assignment04 for CS 3505 Spring 2018 ***

#include <sstream>
#include <boost/algorithm/string.hpp>
#include "parser.h"

//return the first word of the line
std::string parser::getFirstWord(std::string const& s)
{
     return s.substr(0, s.find(" "));
}

//gets the food and adds it to our map
void parser::get_food(std::string const& line, std::map<std::string, food> & food_items)
{     
  std::istringstream ss;
  ss.str(line);

  std::string upc_code;
  int shelf_life;
  std::string name;
  std::string garbage;
  std::string name_tail;

  // consume irrelevant tokens
  // move to UPC code
  for (int i = 0; i < 4; i++)
    ss >> garbage;
   
  // extract UPC Code
  ss >> upc_code;

  // trim whitespace
  boost::algorithm::trim(upc_code);

  // consume irrelevant tokens
  // move to Shelf Life
  for (int i = 0; i < 2; i++)
    ss >> garbage;

  // extract Shelf Life
  ss >> shelf_life;

  // consume irrelevant token
  // move to name
  ss >> garbage;

  // extract name
  getline(ss, name);
  boost::algorithm::trim(name);

  food * f = new food(name, shelf_life);
  food_items[upc_code] = *f;
}

//populate warehouses with location name
void parser::get_warehouse(std::string const& line, std::map<std::string, std::map<std::string, std::map<boost::gregorian::date, long long > > > & warehouses)
{ 
  std::istringstream ss;
  ss.str(line);
  std::string garbage;
  std::string location;

  // consume irrelevant tokens
  // move to warehouse name
  for (int i = 0; i < 2; i++)
    ss >> garbage;

  // extract warehouse location
  getline(ss, location);
  boost::algorithm::trim(location);

  warehouses[location];
}

//gets the date and returns it as a boost gregorian date
boost::gregorian::date parser::getStart(std::string const& line)
{
  std::istringstream ss;
  ss.str(line);
  std::string garbage;
  std::string date_as_string;
  boost::gregorian::date _date;
   
  // consume irrelevant tokens
  // move to start date
  for (int i = 0; i < 2 ; i++)
    ss >> garbage;

  ss >> date_as_string;
  _date = boost::gregorian::from_us_string(date_as_string);

  return _date;
}

//gets the date as a boost format and returns it as a string
std::string parser::parse_date(boost::gregorian::date const& _date)
{

  const std::locale fmt(std::locale::classic(),new boost::gregorian::date_facet("%m/%d/%Y"));

  std::ostringstream oss;

  oss.imbue(fmt);
  oss << _date;

  std::string str = oss.str();
  return str;
}
