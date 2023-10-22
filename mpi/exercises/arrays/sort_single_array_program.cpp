#include "mpi.h"
#include <iostream>
#include "array_functions.hpp"
#include "ArrayHandler.hpp"

// Global macros
#define SIZE_ARRAY 12
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
  else
    {
      if (rank == 0)
        {
          std::cout << "nb process : " << numtasks << std::endl;
        }
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

      std::cout << "Init array : " << std::endl;
      for(int i = 0; i < SIZE_ARRAY; i++)
        {
          std::cout << "Elements no " << i+1 << "::" << array[i] << std::endl;
        }
      std::cout << "#############################" << std::endl;
      
      // Send parts of array to others
      for (int k = 1; k < numtasks; k++)
        {
          dest = k;
          rc = MPI_Send(&array[k*size_part], size_part,
                        MPI_INT, dest,
                        tag, MPI_COMM_WORLD);
        }
    }
  else
    {
      array = new int[size_part];
      // Receive a part of the array
      source = 0;
      rc = MPI_Recv(array, size_part, MPI_INT, source, tag,
                    MPI_COMM_WORLD, &Stat);
    }


  // Sort the array (part of the array for each process)
  sortArray(array, size_part);

  // Gather arrays and print results
  if (rank == 0)
    {
      // Prepare arrays and assign the index 0 with process 0 data
      int ** arrays = new int*[numtasks];
      arrays[0] = array;

      // Receive all the parts from other process
      for (int k = 1; k < numtasks; k++)
        {
          arrays[k] = new int[size_part];
          source = k;

          rc = MPI_Recv(arrays[k], size_part, MPI_INT, source,
                        tag, MPI_COMM_WORLD, &Stat);
        }

      // Gather all the parts
      int * randArray = gatherSortArrays(arrays, numtasks, size_part);

      // Free arrays
      for (int k = 1; k < numtasks; k++)
        {
          delete arrays[k];
          arrays[k] = 0;
        }

      // Print the array
      for(int i = 0; i < SIZE_ARRAY; i++)
        std::cout << "Elements no " << i+1 << "::" << randArray[i] << std::endl;
    }
  else
    {
      // Send the part
      dest = 0;
      rc = MPI_Send(array, SIZE_ARRAY/numtasks, MPI_INT, dest,
                    tag, MPI_COMM_WORLD);
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
