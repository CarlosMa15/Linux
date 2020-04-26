/* This node class is used to build linked lists for the
 * string_set class.
 *
 * Peter Jensen
 * January 30, 2018
 */

#ifndef NODE_H
#define NODE_H

#include<string>

namespace cs3505
{
  // We're in a namespace - declarations will be within this namespace.
  //   (There are no definitions here.)

  /* Node class for holding elements. */
  
  class node
  {
    friend class string_set;   // This allows functions in string_set to access
			       //   private data (and constructor) within this class.
 
    private:
      node(const std::string & data);  // Constructor (changed to take a reference to avoid a copy)
      ~node();                         // Destructor

      std::string data;     // Variable to hold the element
      node        *next;    // Variable to point to the next node in the list
    
      // Below are a few definitions that I use to make sure I am cleaning
      //   up memory properly.  (Notice that I can switch back to public / private
      //   as many times as I like.)  These are not part of a normal
      //   node, but they are here because they help with debugging.
      // These functions and variables are declared static - they exist in the
      //   class, but not in any particular node object.  (The objects can access
      //   them, of course.)
    
    public:
      static long long constructor_count ();  // A debugging function
      static long long destructor_count ();   // A debugging function

    private:
      static long long constructor_calls;  // A debugging variable
      static long long destructor_calls;   // A debugging variable
  };
}

#endif
