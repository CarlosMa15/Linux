/*
 *Assignemt 4
 *Carlos Martinez & Lukas Gust
 *March 07, 2018
 *
 *Struct header for a request object.
 *A request hold a upc for the item requested
 *and the date that it was requested.
 */

#ifndef REQUEST_H
#define REQUEST_H

#include <string>
#include <boost/date_time/gregorian/gregorian.hpp>

struct request 
{
  std::string upc;
  boost::gregorian::date date;
  bool operator< (const request & rhs) const;
};

#endif
