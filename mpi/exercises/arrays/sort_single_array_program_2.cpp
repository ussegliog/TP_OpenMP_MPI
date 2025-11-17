#include "mpi.h"
#include <iostream>
#include "array_functions.hpp"
#include "ArrayHandler.hpp"

// Global macros
#define TEST 1
#if TEST == 1
#define SIZE_ARRAY 12
#else
# define SIZE_ARRAY 200000
#endif

#define MAX_VALUE 99999

int main(int argc, char *argv[])
{
  // Define some variables
  int numtasks, rank, dest, source, rc, count, tag=1;
  MPI_Status Stat;

  // Get some information about MPI exectuion
  MPI_Init(&argc,&argv);
  MPI_Comm_size(MPI_COMM_WORLD, &numtasks);
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);

  if (SIZE_ARRAY%numtasks != 0)
    {
      std::cout << "SIZE_ARRAY must be a multiple of the number of process" << std::endl;

      MPI_Finalize();
      return 1;
    }

  int * array;
  ArrayHandler * arrayHandler;
  int size_part = SIZE_ARRAY/numtasks;

  // Split the init array
  if (rank == 0)
    {
      // Instanciate a arrayHandler
      arrayHandler = new ArrayHandler(1, SIZE_ARRAY, MAX_VALUE);
      // Retrieve the single array
      array = arrayHandler->getFirstArray();

      #if TEST
      std::cout << "Init array : " << std::endl;
      for(int i = 0; i < SIZE_ARRAY; i++)
        {
          std::cout << "Elements no " << i+1 << "::" << array[i] << std::endl;
        }
      std::cout << "#############################" << std::endl;
      #endif
    }
  else
    {
      array = new int[SIZE_ARRAY];
    }


  // Scatter the big array to everybody's part
  MPI_Scatter(array, size_part, MPI_INT,
               array , size_part, MPI_INT, 0, MPI_COMM_WORLD);

  // Sort the array (part of the array for each process)
  sortArray(array, size_part);

  // Gather all sorted arrays as contiguous array
  MPI_Gather(array, size_part, MPI_INT, array, size_part,
              MPI_INT, 0, MPI_COMM_WORLD);

  if (rank == 0)
    {
      // Create arrays from continuous array without copies
      int ** arrays = new int*[numtasks];
      for (int i = 0; i < numtasks; i++)
	{
	  arrays[i]=&array[i*size_part];
	}


      // Gather all parts
      int * randArray = gatherSortArrays(arrays, numtasks, size_part);

      #if TEST
      // Print the array
      for(int i = 0; i < SIZE_ARRAY; i++)
	std::cout << "Elements no " << i+1 << "::" << randArray[i] << std::endl;
       #endif

      // Free Arrays
      delete arrays;
      arrays = 0;
      delete randArray;
      randArray = 0;

    }

  // Free memory
  if (rank == 0)
    {
      // Free memory
      delete arrayHandler;
      arrayHandler = 0;
    }
  else
    {
      delete array;
      array = 0;
    }

  MPI_Finalize();
  return 0;
}
