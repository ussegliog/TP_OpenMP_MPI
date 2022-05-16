#include "mpi.h"
#include <iostream>

int main(int argc, char *argv[])
{
    int numtasks, rank, dest, source, rc, count, tag=1;
    char inmsg, outmsg='x';
    int flag;
    MPI_Status Stat;
    MPI_Request request = MPI_REQUEST_NULL;

    MPI_Init(&argc,&argv);
    MPI_Comm_size(MPI_COMM_WORLD, &numtasks);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    if (rank == 0)
      {
        dest = 1;
        source = 1;
        rc = MPI_Irecv(&inmsg, 1, MPI_CHAR, source, tag, MPI_COMM_WORLD, &request);
        MPI_Test(&request, &flag, &Stat);

        rc = MPI_Isend(&outmsg, 1, MPI_CHAR, dest, tag, MPI_COMM_WORLD, &request);

        // re try to recv from rank 1 if failed (standard recv)
        if (! flag)
          {
            rc = MPI_Recv(&inmsg, 1, MPI_CHAR, source, tag, MPI_COMM_WORLD, &Stat);
          }
    }
    else if (rank == 1)
      {
        dest = 0;
        source = 0;
        MPI_Status Stat_retry;
        rc = MPI_Irecv(&inmsg, 1, MPI_CHAR, source, tag, MPI_COMM_WORLD, &request);
        MPI_Wait(&request, &Stat);

        rc = MPI_Isend(&outmsg, 1, MPI_CHAR, dest, tag, MPI_COMM_WORLD, &request);
        MPI_Test(&request, &flag, &Stat_retry);
        // Send twice to make sure to receive information
        if (flag)
          {
            rc = MPI_Isend(&outmsg, 1, MPI_CHAR, dest, tag, MPI_COMM_WORLD, &request);
          }

      }

    rc = MPI_Get_count(&Stat, MPI_CHAR, &count);
    std::cout << "Task " << rank << " : Received "  << count << " char(s) from task " <<
      Stat.MPI_SOURCE << " with tag " << Stat.MPI_TAG << std::endl;

    MPI_Finalize();
}
