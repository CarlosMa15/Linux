#include "assignment1.h"
#include <string.h>

/*********************************************************************************
* Treats its argument as a sequence of 8 bytes and returns a new unsigned long
* integer containing the same bytes, sorted numerically, with the smaller-valued
* bytes in the lower-order byte positions of the return value.

* EXAMPLE: byte_sort (0x0403deadbeef0201) returns 0xefdebead04030201
*********************************************************************************/
uint64_t byte_sort(uint64_t arg) {

	// This is used to store the seperate 8 bytes
	unsigned long bytes [8];

	// This is used for the for loop
	int i;

	// This is used to extract the byte
	unsigned long ff = 0x00000000000000ff;

	// Put each byte into an array of size 8
	for(i = 0; i < 8; i ++)
	{
		// The & with ff should return the 2 hex value
		bytes[i] = arg & ff;

		// this cuts the input and sets it up for the next iteration
		arg = arg >> 8;
	}

	// Sort the array	
	for(i = 0; i < 8; i ++)
	{
		// setting the position to the next lowest value
		int currentLowestIndexForNexLowestValue = i;

		// Finding the next lowest value
		int j;
		for(j = i; j < 8; j ++)
		{
			if(bytes[j] <= bytes[currentLowestIndexForNexLowestValue])
			{
				currentLowestIndexForNexLowestValue = j;
			}	
		}

		// setting lowest value in correct place
		int temp = bytes[i];
		bytes[i] = bytes[currentLowestIndexForNexLowestValue];
		bytes[currentLowestIndexForNexLowestValue] = temp;		
	}

	// Putting the result back together
	unsigned long result = 0;
	for(i = 0; i < 8; i ++)
	{
		result = result | (bytes[i] << 8*i);
	}
	return result;
}

/*********************************************************************************
nibble_sort() treats its argument as a sequence of 16 4-bit numbers,
and returns a new uint64_t containing the same nibbles, sorted
numerically, with smaller-valued nibbles towards the "small end" of
the uint64_t value that you return. The fact that nibbles and hex
digits correspond should make it easy to verify that your code is
working correctly.

EXAMPLE: nibble_sort(0x0403deadbeef0201) returns 0xfeeeddba43210000
*********************************************************************************/

uint64_t nibble_sort(uint64_t arg) {
	
	// Largest Byte --> Smallest Byte
	// Guaranteed 8 bytes
	
	unsigned long bytes [16];
	int i;
	unsigned long f = 0x000000000000000f;
	
	// Put each byte into an array of size 8
	for(i = 0; i < 16; i ++)
	{
		// This gets the next hex value
		bytes[i] = arg & f;

		// This cuts the input and gets it ready for next iteration
		arg = arg >> 4;
	}  

	// Sort the array	
	for(i = 0; i < 16; i ++)
	{
		// This is the index of the next lowest value we are trying to find
		int currentLowestIndexForNexLowestValue = i;

		// findind the next lowest value
		int j;
		for(j = i; j < 16; j ++)
		{
			if(bytes[j] <= bytes[currentLowestIndexForNexLowestValue])
			{
				currentLowestIndexForNexLowestValue = j;
			}	
		}

		// swap the value with the next lowest values
		int temp = bytes[i];
		bytes[i] = bytes[currentLowestIndexForNexLowestValue];
		bytes[currentLowestIndexForNexLowestValue] = temp;		
	}

	// puts the results together and returns
	unsigned long result = 0;
	for(i = 0; i < 16; i ++)
	{
		result = result | (bytes[i] << 4*i);
	}
	return result;
}

/**************************************************************************
This is a helper method to free up memory in from the list if malloc fails
**************************************************************************/
static void free_up_list_helper_method(struct elt *head)
{
	struct elt *temp;

	// This will iterate through the list till they get 
	// to null terminating value freeing up the list
	while(head != NULL)
	{
		temp = head->link;
		free(head);
		head = temp;
	}

}

struct elt* circular_list(const char* str) {

	// Pointers used to create and navagate the list
	struct elt *previous = NULL;
	struct elt *head = NULL;
	struct elt *current = NULL;
	
	// This allocates memory for the head
	head = (struct elt*) malloc(sizeof(struct elt));
	
	// If the call to malloc fails then it should return null
	if(head == NULL)
	{
		return NULL;
	}
	
	// sets the head of the list
	head -> val = str[0];
	previous = head;
	int i;

	// iterating through the letters of the string
	for (i = 1; i < strlen(str); i++)
	{

		// allocating data for next value
		current = (struct elt*) malloc(sizeof(struct elt));
		
		// If the call to malloc fails then it should return null and free memory
		if(current == NULL)
		{
			free_up_list_helper_method(head);
			return NULL;
		}

		// set the value of the next charactor from the string
		current -> val = str[i];
		
		// sets the prevoius pointer correctly for next iteration
		if(previous != NULL)
		{
			previous->link = current;
		}
		previous = current;
	}

	// returns the head of the list
	return head;
}

int main(int argc, char * argv[])
{

   uint64_t my_uint = 0x0403deadbeef0201;
   printf("Value of a: Hex: %X\n", my_uint);
   uint64_t my_byte_sorted_uint = byte_sort(my_uint);
   printf("Value of a: Hex: %X\n", my_byte_sorted_uint);
   uint64_t my_nibble_sort_uint = nibble_sort(my_uint);
   printf("Value of a: Hex: %X\n", my_nibble_sort_uint);
   struct elt* start = circular_list("Martinez");
   struct elt *temp=start;
   	while(temp!=NULL)
	{
	    // do something
		printf("Letter is: %c\n", temp->val);
		temp = temp->link;	           
	}

}