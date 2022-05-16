#include <iostream>
#include "array_functions.hpp"
#include "ArrayHandler.hpp"

// Global macros
#define NUMBER_OF_ARRAYS 3
#define SIZE_ARRAY 10
#define MAX_VALUE 99999


int main()
{
  // Instanciate a arrayHandler
  ArrayHandler * arrayHandler = new ArrayHandler(NUMBER_OF_ARRAYS,
                                                 SIZE_ARRAY, MAX_VALUE);
  // Retrieve arrays
  int ** arrays = arrayHandler->getArrays();

  // Sort arrays
  for (int id_array = 0; id_array < NUMBER_OF_ARRAYS;
       id_array++)
    {
      // Sort the array
      sortArray(arrays[id_array], SIZE_ARRAY);
    }

  // Gather arrays
  int * randArray = gatherSortArrays(arrays, NUMBER_OF_ARRAYS, SIZE_ARRAY);

  // Print the array
  for(int i = 0; i < NUMBER_OF_ARRAYS*SIZE_ARRAY; i++)
    std::cout << "Elements no " << i+1 << "::" << randArray[i] << std::endl;


  // Free memory
  delete arrayHandler;
  arrayHandler = 0;
  delete randArray;
  randArray = 0;

  return 0;
}
