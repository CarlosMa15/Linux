/*
 *Assignemt 4
 *Carlos Martinez & Lukas Gust
 *March 07, 2018
 *
 *Authors: Lukas Gust & Carlos Martinez
 */

#ifndef WAREHOUSE_H
#define WAREHOUSE_H

#include <vector>
#include <map>
#include <set>
#include <string>
#include "foodItem.h"
#include "request.h"
#include <boost/date_time/gregorian/gregorian.hpp>

/*
 *A class representation of a warehouse of a given name. 
 *Keeps track of item stock and item expiration.
 *Processes requests and accepts recipients of items. Processes each day. 
 */
class warehouse{
 private:
  // Private Fields
  std::string name;                          //name of the warehouse 
  std::vector<foodItem> stock;               //list of items the warehouse in stock
  std::map<std::string, foodItem> upcLookup; //UPC to item for easy adding
  std::map<std::string, long long> itemCount;//UPC to count for easy counting
  std::vector<request> requests;             //list of requests that need to be processed
  std::set<request> unfilledReq;             //set of unfilled requests
 
 public:
  warehouse();
  warehouse(std::string name, std::vector<foodItem> & foodItems);
  int getItemCount(const std::string & upc);
  std::string getName();
  std::set<request> getUnfilledReq();
  void requestItems(const request req, int count);
  void receiveItems(const std::string upc, int count);
  void nextDay(std::map<std::string, std::set<std::string> > &temp_map);
  void end(std::map<std::string, std::set<std::string> > &temp_map);

 private:
  //Private helper method
  void processRequests(std::map<std::string, std::set<std::string> > &temp_map);
};

#endif
