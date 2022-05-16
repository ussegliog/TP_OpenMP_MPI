#include <iostream>
#include "array_functions.hpp"
#include "ArrayHandler.hpp"

// Global macros
#define SIZE_ARRAY 10
#define MAX_VALUE 99999

int main()
{
  // Instanciate a arrayHandler
  ArrayHandler * arrayHandler = new ArrayHandler(1, SIZE_ARRAY, MAX_VALUE);
  // Retrieve the single array
  int * array = arrayHandler->getFirstArray();

  // Sort the array
  sortArray(array, SIZE_ARRAY);

  // Gather arrays (useless, here)
  int * randArray = array;
  //int * randArray = gatherSortArrays(arrays, 1, SIZE_ARRAY);

  // Print the array
  for(int i = 0; i < SIZE_ARRAY; i++)
    std::cout << "Elements no " << i+1 << "::" << randArray[i] << std::endl;

  // Free memory
  delete arrayHandler;
  arrayHandler = 0;

  return 0;
}
