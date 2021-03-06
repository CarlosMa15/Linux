/*
 *Authors: Lukas Gust & Carlos Martinez
 *March 8, 2016
 *This struct is intended to hold the necessary data
 *to process unfilled orders and the dates they occured.
 *Does not know the name of a upc.
 */

#include "request.h"

/*
 *Request struct definition for less than operator.
 *Sorts by date first then upc.
 */
bool request::operator< (const request & rhs) const
{
  if(this->date < rhs.date)
    return true;
  else if(this->date == rhs.date)
    return this->upc < rhs.upc;
  else
    return false; 
}
