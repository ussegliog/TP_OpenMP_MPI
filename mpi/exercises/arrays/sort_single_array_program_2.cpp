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
    }
  else
    {
      array = new int[SIZE_ARRAY];
    }

  // Build counts and displacements to have at the end a whole contiguous array from all process
  int send_counts[numtasks];
  int recv_count = size_part;
  int send_displacements[numtasks];
  for (int k = 0; k < numtasks; k++)
    {
      // Send to each process
      send_counts[k] = size_part;
      // Split the init array to numtasks equal parts
      send_displacements[k] = k*(size_part);
    }

  // Scatter the big array to everybody's part
  MPI_Scatterv(array, send_counts, send_displacements, MPI_INT,
               array , recv_count, MPI_INT, 0, MPI_COMM_WORLD);

  // Sort the array (part of the array for each process)
  sortArray(array, size_part);

  // Build counts and displacements to have at the end a whole contiguous array from all process
  int receive_counts[numtasks];
  int receive_displacements[numtasks];
  for (int k = 0; k < numtasks; k++)
    {
      // Each process sends one array of size_part elts
      receive_counts[k] = size_part;
      // The array is the n_task part of the whole array
      receive_displacements[k] = k*size_part;
    }

  // Gather all sorted arrays as contiguous array
  int send_count = size_part;
  MPI_Gatherv(array, send_count, MPI_INT, array, receive_counts, receive_displacements,
              MPI_INT, 0, MPI_COMM_WORLD);

  if (rank == 0)
    {
      // Re Sort the whole array
      sortArray(array, SIZE_ARRAY);
      // Print the array
      for(int i = 0; i < SIZE_ARRAY; i++)
        std::cout << "Elements no " << i+1 << "::" << array[i] << std::endl;
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
