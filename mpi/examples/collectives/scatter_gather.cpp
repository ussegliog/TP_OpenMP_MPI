#include "mpi.h"
#include <iostream>

int main(int argc, char** argv) {

  int num_elements = 2;
  MPI_Init(NULL, NULL);
  int world_rank, nb_ranks;
  MPI_Comm_rank(MPI_COMM_WORLD, &world_rank);
  MPI_Comm_size(MPI_COMM_WORLD, &nb_ranks);

  int* send_data = NULL;
  int* recv_data = NULL;

  int irecv = -1;
  // Init send_data, only in rank 0
  if (world_rank == 0)
    {
      send_data = new int[nb_ranks];
      recv_data = new int[nb_ranks];

      for (int i = 0; i < nb_ranks; i++)
        {
          send_data[i] = i;
        }
    }

  // Scatter information to all processes
  MPI_Scatter(send_data, 1, MPI_INT, &irecv, 1, MPI_INT, 0, MPI_COMM_WORLD);

  // For each rank, do some stuff (+ 1, here)
  irecv++;

  // Gather results
  MPI_Gather(&irecv, 1, MPI_INT, recv_data, 1, MPI_INT, 0, MPI_COMM_WORLD);

  // Display data
  if (world_rank == 0)
    {
      for (int i = 0; i < nb_ranks; i++)
        {
          std::cout << " i : " << i << "; recv_data[i] = " << recv_data[i]
                    << std::endl;
        }
    }

  if (send_data)
    {
      delete send_data;
      send_data = 0;
    }
  if (recv_data)
    {
      delete recv_data;
      recv_data = 0;
    }
  MPI_Finalize();
}
