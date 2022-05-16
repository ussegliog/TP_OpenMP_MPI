#include "mpi.h"
#include <iostream>

int main(int argc, char** argv) {

  int num_elements = 2;
  MPI_Init(NULL, NULL);
  int world_rank;
  MPI_Comm_rank(MPI_COMM_WORLD, &world_rank);

  int* data = new int[2];
  // Init data, only in rank 0
  if (world_rank == 0)
    {
      data[0] = 42;
      data[1] = 43;
    }

  // Broadcast information to all processes
  MPI_Bcast(data, num_elements, MPI_INT, 0, MPI_COMM_WORLD);

  // Display data
  std::cout << "rank = " << world_rank << " and data[0] = " << data[0] <<
    " ;  data[1] = " << data[1] << std::endl; 
  
  delete data;
  data = 0;
  MPI_Finalize();
}
