#include "mpi.h"
#include <iostream>
#include "array_functions.hpp"
#include "ArrayHandler.hpp"

// Global macros
#define NUMBER_OF_ARRAYS 4
#define SIZE_ARRAY 10
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

  if (NUMBER_OF_ARRAYS != numtasks or NUMBER_OF_ARRAYS != 4)
    {
      std::cout << "NUMBER_OF_ARRAYS must be equal to the number of process (4)" << std::endl;

      MPI_Finalize();
      return 1;
    }

  // Get the group of processes in MPI_COMM_WORLD
  MPI_Group world_group;
  MPI_Comm_group(MPI_COMM_WORLD, &world_group);

  // Build two new groups with two processes by group
  int number_of_group = 2;
  const int ranks_group_1[2] = {0, 1};
  const int ranks_group_2[2] = {2, 3};

  MPI_Group new_group;

  // Divide tasks into two distinct groups based upon rank
  if (rank < numtasks/number_of_group)
    {
      MPI_Group_incl(world_group, numtasks/number_of_group, ranks_group_1,
                     &new_group);
    }
  else
    {
      MPI_Group_incl(world_group, numtasks/number_of_group, ranks_group_2,
                     &new_group);
    }


  // Build a new communicator for the new group
  int number_of_arrays_in_group = NUMBER_OF_ARRAYS/number_of_group;
  int numtasks_group = 0;
  MPI_Comm new_comm;
  MPI_Comm_create(MPI_COMM_WORLD, new_group, &new_comm);
  MPI_Comm_size(new_comm, &numtasks_group);

  // Get rank inside the new_comm
  int rank_group = -1;
  MPI_Group_rank(new_group, &rank_group);

  int number_arrays_per_process = number_of_arrays_in_group/numtasks_group;

  int ** arrays;

  // Instanciate a arrayHandler if rank = 0 in one of the new groups
  if (rank_group == 0)
    {
      ArrayHandler * arrayHandler = new ArrayHandler(NUMBER_OF_ARRAYS,
                                                 SIZE_ARRAY, MAX_VALUE);
      // Retrieve arrays
      arrays = arrayHandler->getArrays();


      int count = 0;
      for (int n = 0; n < NUMBER_OF_ARRAYS/number_of_group; n ++)
        {
          for(int i = 0; i < SIZE_ARRAY; i++)
            {
              std::cout << "Elements no " << count+1 << "::" << arrays[n][i] << std::endl;
              count++;
            }
        }


      // Send arrays to others in the same group
      for (int k = 1; k < numtasks_group; k++)
        {
          dest = k;
          // Send a number of arrays
          for (int n_send = 0;
               n_send < number_arrays_per_process; n_send++)
            {
              tag = tag + n_send;
              int id_array = number_arrays_per_process*k + n_send;
              rc = MPI_Send(arrays[id_array], SIZE_ARRAY,
                            MPI_INT, dest,
                            tag, new_comm);
            }
        }
    }
  else
    {
      // Receive a number of arrays
      arrays = new int*[number_arrays_per_process];

      for (int n_recv = 0;
           n_recv < number_arrays_per_process; n_recv++)
        {
          // Allocate memory
          arrays[n_recv] = new int[SIZE_ARRAY];

          source = 0;
          tag = tag + n_recv;
          rc = MPI_Recv(arrays[n_recv], SIZE_ARRAY, MPI_INT, source, tag,
                        new_comm, &Stat);
        }
    }

  // Sort arrays
  for (int id_array = 0; id_array < number_arrays_per_process;
       id_array++)
    {
      // Sort the array
      sortArray(arrays[id_array], SIZE_ARRAY);
    }

  // Retrive arrays in each group
  if (rank_group == 0)
    {
      for (int k = 1; k < numtasks_group; k++)
        {
          source = k;
          for (int n_recv = 0;
               n_recv < number_arrays_per_process; n_recv++)
            {
              tag = tag + n_recv;
              int id_array = number_arrays_per_process*k + n_recv;
              rc = MPI_Recv(arrays[id_array], SIZE_ARRAY, MPI_INT, source,
                            tag, new_comm, &Stat);
            }
        }

      // Gather arrays
      int * randArray = gatherSortArrays(arrays, number_of_arrays_in_group,
                                         SIZE_ARRAY);

      // At the end, back to comm_world to gather arrays from groups
      if (rank == 0 && number_of_group != 1)
        {
          // n == nb_group : 2, here
          source = ranks_group_2[0];

          int ** arrays_world = new int*[number_of_group];
          arrays_world[0] = randArray;

          // Get the second gathered array from the other group
          tag = 99;
          int size = SIZE_ARRAY*number_of_arrays_in_group;
          arrays_world[1] = new int[size];

          rc = MPI_Recv(arrays_world[1], size, MPI_INT, source,
                        tag, MPI_COMM_WORLD, &Stat);

          int * randArrayWorld = gatherSortArrays(arrays_world,
                                                  number_of_group, size);

          // Print the array
          std::cout << "#############################" << std::endl;
          for(int i = 0; i < SIZE_ARRAY*NUMBER_OF_ARRAYS; i++)
            std::cout << "Elements no " << i+1 << "::" << randArrayWorld[i] << std::endl;

        }
      else
        {
          // Send the gathered array
          dest = 0;
          tag = 99;
          int size = SIZE_ARRAY*number_of_arrays_in_group;

          rc = MPI_Send(randArray, size, MPI_INT, dest,
                    tag, MPI_COMM_WORLD);
        }
      }
  else
    {
      // Send arrays
      dest = 0;
      for (int n_send = 0;
           n_send < number_arrays_per_process; n_send++)
        {
          tag = tag + n_send;

          rc = MPI_Send(arrays[n_send], SIZE_ARRAY, MPI_INT, dest,
                        tag, new_comm);
        }
    }

  // // Free memory
  // delete arrayHandler;
  // arrayHandler = 0;
  // delete randArray;
  // randArray = 0;

  // Free groups and comms
  MPI_Group_free(&new_group);
  if (MPI_COMM_NULL != new_comm)
    {
      MPI_Comm_free(&new_comm);
    }

  MPI_Finalize();
  return 0;
}
