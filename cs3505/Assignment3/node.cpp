/* This node class is used to build linked lists for the
 * string_set class.
 *
 * Peter Jensen
 * January 30, 2018
 */

#include "node.h"
#include <string>

namespace cs3505
{
  // We're in a namespace.  We are not in any class.  Symbols defined
  //   here are globally available.

  /*******************************************************
   * node static definitions:
   *
   *     These are for debugging purposes only.  They help 
   * the programmer audit their memory usage.
   *
   *     Do not change anything below this point.
   ***************************************************** */

  // Since static variables are not in objects, we need to define
  // storage for them.  These variables are -here-, not in any object.
  // (This is the ONLY copy of these variables that will exist.)
  // I've changed the visibility from the version I did in class.
  // Also, I've qualified the access to the static variables to
  // make their scope and usage clear.  (See code below.)
  
  long long node::constructor_calls = 0;
  long long node::destructor_calls = 0;

  /** Returns the number of times any node
    *   constructor has been called.
    */
  long long node::constructor_count ()
  {
    return node::constructor_calls;
  }

  
  /** Returns the number of times the node
    *   destructor has been called.
    */
  long long node::destructor_count ()
  {
    return node::destructor_calls;
  }

  /*******************************************************
   * node member function definitions
   ***************************************************** */

  /** Constructor:  Creates a node containing
    *   an element.  It is initialized to
    *   not point to any other node.
    */
  node::node(const std::string & s)
    : next(NULL),  // This syntax is used to call member variable constructors (or initialize them).
      data(s)      // This calls the copy constructor - we are making a copy of the string.
  {
    // Do not change or remove the following line.
    //   It must execute for our auditing to be correct.

    node::constructor_calls++;

    // No other work needed - the initializers took care of everything.
  }

  
  /** Destructor:  release any memory allocated
    *   for this object.
    */
  node::~node()
  {
    // I'm not convinced that the recursive delete is the
    //   best approach.  I'll keep it (and you'll keep it too).

    if (this->next != NULL)
      delete this->next;

    // Do not change or remove the following line.
    //   It must execute for our auditing to be correct.

    node::destructor_calls++;

    // Invalidate the entry so that it is not accidentally used.

    this->next = NULL;      
  }

  // End of the namespace
}
