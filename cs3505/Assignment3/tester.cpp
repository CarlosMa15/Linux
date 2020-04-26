/*
 * This is a tester that I started to write in class.  It reads
 * words from a text file, then adds the words to two sets: A built-in
 * set class, and our string_set class.  After reading the file, it
 * prints out all the words stored in the STL set object.  At the end
 * of the test, it prints out the sizes of both sets to see that they
 * are the same.
 *
 * After the test completes, I make sure the local variabls are properly
 * cleaned up.
 *
 * If the comments wrap lines, widen your emacs window.
 *
 * Peter Jensen
 * January 30, 2018
 */

#include <iostream>
#include <fstream>
#include <set>
#include <iterator>
#include "string_set.h"
#include "node.h"

// For convenience only:

using namespace std;

// Note:  Our classes were declared in a cs3505 namepsace.
//        Instead of 'using namespace cs3505', I qualify the class names 
//        below with cs3505::

int main ()
{
  // Open up another block.  This way, when the block ends,
  // variables local to the block will be destroyed, but main
  // will still be running.  (Did you know that you can open
  // up a block at any time to control local variable scope and
  // lifetime?)
  
  {
// Create the two sets.  Declaring the local variables constructs the objects.
  
    set<string>         stl_set;  // The built-in set class - no constructor parameters.

    cs3505::string_set  our_set(1000);  // Our set class, with a hashtable of 1000 slots.
  
    // Open the file stream for reading.  (We'll be able to use it just like
    //   the keyboard stream 'cin'.)

    ifstream in("Yankee.txt");

    // Loop for reading the file.  Note that it is controlled
    //   from within the loop (see the 'break').
    
    while (true)
    {
      // Read a word (don't worry about punctuation)
      
      string word;
      in >> word;

      // If the read failed, we're probably at end of file
      //   (or else the disk went bad).  Exit the loop.
      
      if (in.fail())
	break;

      // Word successfully read.  Add it to both sets.
      
      stl_set.insert(word);
      our_set.add(word);
    }

    // Close the file.

    in.close();

    // Print out all the words in the reference solution.

    for (set<string>::iterator it = stl_set.begin(); it != stl_set.end(); it++)
    {
      string word = *it;
      cout << word << endl;
    }

    // Print out the number of words found in each set.

    cout << "STL set contains " << stl_set.size() << " unique words.\n";
    cout << "Our set contains " << our_set.size() << " unique words.\n"; 
    
    // Done.  Notice that this code block ends below.  Any local
    // variables declared within this block will be automatically
    // destroyed.  Local objects will have their destructors
    // called.  (Blocks are great for controlling scope/lifetime.)
  }

// Now that the objects have been destroyed, I will simply call my auditing
  // code to print out how many times constructors have been called, and
  // how many times destructors have been called.  They should exactly match.
  // If not, we have a memory problem.

  cout << "Class cs3505::string_set:" << endl;
  cout << "    Objects created:  " << cs3505::string_set::constructor_count() << endl;
  cout << "    Objects deleted:  " << cs3505::string_set::destructor_count() << endl;
  cout << endl;

  cout << "Class cs3505::node:" << endl;
  cout << "    Objects created:  " << cs3505::node::constructor_count() << endl;
  cout << "    Objects deleted:  " << cs3505::node::destructor_count() << endl;
  cout << endl;

  // Now we're really done.  End main.
  
  return 0;
}
