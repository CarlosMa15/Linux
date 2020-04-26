/* A 'string set' is defined as a set of strings stored
 * in a hashtable that uses chaining to resolve collisions.
 *
 * Peter Jensen
 * January 30, 2018
 */

#include "string_set.h"
#include "node.h"
#include <iostream>  // For debugging, if needed.
#include <string>
#include <vector>    // Needed for the assignment.

namespace cs3505
{
  /*******************************************************
   * string_set member function definitions
   ***************************************************** */
  
  /** Constructor:  The parameter indicates the
    *   size of the hashtable that should be used
    *   to keep this set.
    */
  string_set::string_set(int capacity)
  {
    // Set up a hashtable of the specified capacity.

    this->capacity = capacity;
    this->count = 0;
    this->table = new node*[capacity];

    // The array must be cleared -- it will have bogus values in it.
    //   (NULL means 'no chain in this entry')

    for (int i = 0; i < capacity; i++)
      table[i] = NULL;

    // Do not change or remove the following line.
    //   It must execute for our auditing to be correct.

    string_set::constructor_calls++;
  }

  
  /** Copy constructor:  Initialize this set
    *   to contain exactly the same elements as
    *   another set.
    */
  string_set::string_set (const string_set & other)
  {
    // Give initial values to ensure the object is well-formed.

    table = NULL;
    count = 0;
    capacity = 0;

    // Use our assignment operator to complete this step.
    //   (dereference this, then assign to that instance)
 
    *this = other;

    // Do not change or remove the following line.
    //   It must execute for our auditing to be correct.

    string_set::constructor_calls++;
  }


  /** Destructor:  release any memory allocated
    *   for this object.
    */
  string_set::~string_set()
  {
    // Use a helper function to do all the work.

    clean();

    // Do not change or remove the following line.
    //   It must execute for our auditing to be correct.

    string_set::destructor_calls++;
  }


  /** Releases any memory that was allocated by
    *   this object.  This effectively destroys the
    *   set, so it should only be called if this object
    *   is destructing, or is being assigned.
    */
  void string_set::clean()
  {
    // Clean up the table (if any)

    if (table != NULL)
    {
      // Clean up (deallocate) any chains in the table.

      for (int i = 0; i < capacity; i++)
        if (table[i] != NULL)
	{
          delete table[i];
          table[i] = NULL;
        }

      // Release the table's memory.

      delete [] table;
    }

    // When 'this' object has been cleaned, it has no array.
    //   Set the fields appropriately.

    table = NULL;
    count = 0;
    capacity = 0;
  }


  /** Computes a table index for a given string.
    *   If two strings have the same contents, their
    *   hash code (table index) will be identical.
    * The hash code is guaranteed to be in the
    *   range [0..capacity).
    */  
  int string_set::hash (const std::string & s) const
  {
    // A well-known hash algorithm.  Do not change!!!

    long long hash = 0;
    for (int i = 0; i < s.length(); i++)
      hash = ((hash*1117) + s[i]) % capacity;

    return static_cast<int>(hash);
  }


  /** Adds the specified element to this set.  If the element
    *   is already in this set, no action is taken.
    */
  void string_set::add (const std::string & s)
  {
    // Determine which table entry chain might contain this string.

    int index = hash(s);

    // Walk the chain (the linked list).  Check each entry for the
    //   string.  If we find it, just bail out.  (No duplicates allowed.)

    node *current = table[index];
    while (current != NULL)
    {
      if (current->data == s)
        return;
      current = current->next;
    }
    
    // Make a new node, then link it in to the beginning of the chain.

    node *n = new node(s);  // The node is created, but points nowhere.
    n->next = table[index]; // Point the node to the head node of the chain.
    table[index] = n;       // Point the head of the chain to our new node.
   
    // We added a string - count it.

    count++;
  }


  /** Removes the specified element from this set.  If the
    *   element is not in the set, no action is taken.
    */
  void string_set::remove (const std::string & element)
  {
    int index = hash(element);

    bool first = true; // Checker for if first element
    
    node *before = table[index]; // Node Before the element we will delete

    node *current = table[index]; // current node
    
    //Finds node to delete
    while (current != NULL)
    {
      if (current->data == element){ // Stop once found
        break;
      }
      
      if(!first){ // Incriments the before
	before = before->next;
      }
      
      first = false; // Used to control before incrementaion
                     // Because both start at same point, just delay it once

      current = current->next; // increment current, always
    }

    if(current == NULL)
      return; // No element found

    if(first){ // Remove first
      table[index] = current->next;
    } else { // remove else where, in middle or end
      before->next = current->next;
    }
    current->next = NULL;
    delete current;

    count--;
    
    // STILL NEED TO DELETE THE NODE

    // Requirement:  When an element is removed, its
    //   enclosing node must be unlinked from that
    //   linked list, and then the node must be deleted
    //   to free up its memory.  The linked list and/or
    //   table must be appropriately updated.

    // To be completed as part of the assignment.
  }


  /** Returns true if the specified element in in this set,
    *   false otherwise.
    */
  bool string_set::contains (const std::string & element) const
  { 
    // Determine which table entry chain might contain this string.

    int index = hash(element);

    // Walk the chain (the linked list).  Check each entry for the
    //   string.  If we find it, return true.

    node *current = table[index];
    while (current != NULL)
    {
      if (current->data == element){
        return true;
      }
      current = current->next;
    }
   
    // To be completed as part of the assignment.

    return false;  // Stub - update/change as needed.
  }


  /** Returns a count of the number of elements
    *   in this set.
    */
  int string_set::size() const
  {
    return this->count;
  }


  /*** Assignment operator ***/
  
  /** This function overloads the assignment operator.  It
    *   clears out this set, builds an empty table, and copies
    *   the entries from the right hand side (rhs) set into
    *   this set.
    */
  string_set & string_set::operator= (const string_set & rhs)
  {
    // If we are assigning this object to this object,
    //   do nothing.  (This is important!)

    if (this == &rhs)  // Compare addresses (not object contents)
      return *this;  // Do nothing if identical

    // Wipe away anything that is stored in this object.
    
    clean();
    
    // Create a new set (new table) and populate it with the entries
    //   from the set in rhs.  Use the capacity from rhs.  Hint:
    //   see the first constructor above (but you cannot call it).
    
    // Requirement:  Do not permanently point to arrays or nodes in rhs.  
    //   When rhs is destroyed, it will delete its array and nodes, 
    //   and we cannot count on their existence.  Instead, you will
    //   create a new array for this object, traverse rhs,
    //   and add one entry to this set for every entry in rhs.
    
    // To be completed as part of the assignment.

    // Sets things just like constructor
    this->capacity = rhs.capacity;
    this->count = 0;
    this->table = new node*[capacity];

    // The array must be cleared -- it will have bogus values in it.
    //   (NULL means 'no chain in this entry')

    for (int i = 0; i < capacity; i++)
      table[i] = NULL;
    
    // gets the elements from the rhs and adds it to this string_set
    // the rhs should have no deplucates to worry about, so add everything
    std::vector<std::string> rhsElements = rhs.get_elements();

    for(std::vector<std::string>::iterator iter = rhsElements.begin();
	iter != rhsElements.end();iter++ ){

      std::string iter1 = *iter; // element to add
      // Make a new node, then link it in to the beginning of the chain.
      int index = hash(iter1);
      node *n = new node(iter1);  // The node is created, but points nowhere.
      n->next = table[index]; // Point the node to the head node of the chain.
      table[index] = n;       // Point the head of the chain to our new node.

      count++;
    }

    // Done with assignment operator.
    
    return *this;
  }

  /** This function populates and returns a std::vector<std::string>
    * with elements from this set.  The elements in the vector will be in the
    * same order as the natural ordering in this hashtable (table[0] first, etc.).
    * The size of the vector will be exactly equal to the element count
    * in this hashtable.
    */
  std::vector<std::string> string_set::get_elements() const{
    std::vector<std::string> array; // vector holding all the values
	 
    for (int i = 0; i < capacity; i++){ // looking at every index of the array
  	 node *current = table[i];
  	 while (current != NULL)
	 { // Adding every element in the chain(link list)
	   array.push_back(current->data);
	   current = current->next;
	 }
    }

      return array;
  }  


  /*******************************************************
   * string_set static definitions:
   *
   *     These are for debugging purposes only.  They help 
   * the programmer audit their memory usage.
   *
   *     Do not change anything below this point.
   ***************************************************** */

  // Since static variables are not in objects, we need to define
  // storage for them.  These variables are -here-, not in any object.
  // (This is the ONLY copy of these variables that will exist.)
  
  long long string_set::constructor_calls = 0;
  long long string_set::destructor_calls = 0;


  /** Returns the number of times any string_set
    *   constructor has been called.
    */
  long long string_set::constructor_count ()
  {
    return string_set::constructor_calls;
  }

  
  /** Returns the number of times the string_set
    *   destructor has been called.
    */
  long long string_set::destructor_count ()
  {
    return string_set::destructor_calls;
  }
}
