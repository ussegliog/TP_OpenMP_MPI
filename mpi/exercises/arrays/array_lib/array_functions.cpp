#include "array_functions.hpp"

//// Pool of functions /////
int * generateRandomArray(int sz, int max_value)
{
  int * randArray = new int[sz];
  for(int i = 0; i < sz; i++)
    randArray[i] = std::experimental::randint(1, max_value); //Generate numbers

  return randArray;
}

void sortArray(int * array, int size)
{
  int i, j = 1;
  int c = size - 2;

  for(i=0 ; i < size-1 ; i++)
    {
      for(j; j < size; j++)
        {
          // change iter j with i to sort numbers
          if(array[i] > array[j])
            {
              std::swap(array[i], array[j]);
            }
        }

      j = j - c;
      c--;
    }
}

int * gatherSortArrays(int ** arrays, int nbArrays, int size,
                       int max_value)
{
  int * gatherArray = new int[size*nbArrays];

  // Init counters to zero
  int counters[nbArrays];
  for(int i=0 ; i < nbArrays; i++)
    {
      counters[i] = 0;
    }

  for (int j = 0; j < (size*nbArrays); j++)
    {
      // Init min value and counter
      int minCounterId = 0;
      int minValue = max_value;

      // Find the min between arrays at each counter
      for (int k = 0; k < nbArrays; k ++)
        {
          // Check if counter has reached the last index of the array
          if (counters[k] < size)
            {
              // Compare values at counter index
              if (arrays[k][counters[k]] < minValue)
                {
                  minValue = arrays[k][counters[k]];
                  minCounterId = k;
                }
            }
        }


      // Assign minimun value and increment counter for the specific array
      gatherArray[j] = minValue;
      counters[minCounterId] ++;
    }

  return gatherArray;

}
