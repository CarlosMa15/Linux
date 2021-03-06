/*
 *Assignemt 4
 *Carlos Martinez & Lukas Gust
 *March 07, 2018
 *
 *Warehouses keep track of requests and unfilled requests.
 *It also processes receivals.
 */

#include "warehouse.h"
#include "foodItem.h"
#include "request.h"
#include <string>
#include <set>
#include <vector>
#include <algorithm>

/*
 *Default Ctor for a warehouse object.
 */
warehouse::warehouse()
{
  this-> name = "";
}

/*
 *Ctor for warehouse object. Initializes the fields needed.
 *
 *name is the name of the warehouse, foodItems is a list of 
 * the foodItems that exist in a current file context. Warehouses 
 * are constructed this way after reading all of the fooditems.
 */
warehouse::warehouse(std::string name, std::vector<foodItem> & foodItems)
{
  this->name = name;
    
  for(std::vector<foodItem>::iterator it = foodItems.begin() ; it != foodItems.end() ; it++)
  {
    upcLookup[it->getUPC()] = (*it);
    itemCount[it->getUPC()] = 0;
  }
}

/*
 *Accessor for the count of a particular item.
 */
int warehouse::getItemCount(const std::string & upc)
{
  return itemCount[upc];
}

/*
 *Accessor for the name of the warehouse
 */
std::string warehouse::getName()
{
  return name;
}

/*
 *Accessor for the list of unfilled orders. Accessable at any time.
 */
std::set<request> warehouse::getUnfilledReq()
{
  return unfilledReq;
}

/*
 *Process a request for an amount of a particular item. 
 *Put the request in a queue for processing at the end of the day.
 */
void warehouse::requestItems(const request req, int count)
{
  for(int i = 0; i < count; i++)
    this->requests.push_back(req);
}

/*
 *Process a receive for an amount of a particular item.
 *Adds to the list of the stock and updates the count values
 */
void warehouse::receiveItems(const std::string upc, int count)
{
  for(int i = 0; i < count; i++)
  {  
    stock.push_back(upcLookup[upc]);
    itemCount[upc] += 1;
  }
}

/*
 *Helper to process the requests that can be processed given the 
 * warehouse has it in its stock
 */
void warehouse::processRequests(std::map<std::string, std::set<std::string> > &temp_map)
{
  //process requests
  for(std::vector<request>::iterator req = requests.begin(); req != requests.end(); req++)
  {   
    bool check = false;
    
    if(itemCount[req->upc] > 0)
      check = true;

    bool unfilled = true;
    //decrement count and erase from stock
    itemCount[req->upc] -= 1;
    for(std::vector<foodItem>::iterator item = stock.begin(); item != stock.end(); item++)
    {
      if(item->getUPC() == req->upc)
      {
	unfilled = false;
	stock.erase(item);
	break;
      }
    }

    if(unfilled)
      unfilledReq.insert(*req);//add request to unfilled using an unfilled set
    
    if(check && itemCount[req->upc] <= 0)
      temp_map[req->upc].erase(this->name);
  }
  
  requests.clear();
}

/*
 *Advance to the next day.
 *Wrap up all requests and update stock accordingly and count and update each item in stocks shelf life.
 *Remove any expired stock.
 */
void warehouse::nextDay(std::map<std::string, std::set<std::string> > &temp_map)
{
  processRequests(temp_map);

  //find and remove expired items
  std::vector<foodItem>::iterator item = stock.begin();
  while(item != stock.end() && (!stock.empty()))
  {
    std::string upcCode = item->getUPC();

    bool check = itemCount[upcCode] > 0;

    int newLife = item->getShelfLife() - 1;
    if(newLife == 0)
    {
      itemCount[upcCode] -= 1;
      item = stock.erase(item);
    } 
    else
    {
      item->setLife(newLife);
      item++;
    }
    
    if(check && itemCount[upcCode] <= 0)
	temp_map[upcCode].erase(this->name);
  }
}

/*
 *This method is called when the file has reached its end. 
 *Process the hanging requests for that day and end.
 * Does not update expiration.
 */
void warehouse::end(std::map<std::string, std::set<std::string> > &temp_map)
{
  processRequests(temp_map);
  //process any requests that you have from the current day. 
  //Dont advance a day keep expiration as is
}
