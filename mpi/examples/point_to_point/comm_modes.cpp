#include "mpi.h"
#include <iostream>
#include <string>

// Communication functions
void buffered_send_recv(int rank, char *buf, int bufsize, char *inmsg,
                        std::string outmsg)
{
  int count, rc;
  MPI_Status Stat;
  
  if (rank == 0)
    {
      std::cout  << "Buffered ...." << std::endl;
      MPI_Buffer_attach(buf, bufsize);
      
      MPI_Bsend(outmsg.c_str(), bufsize, MPI_CHAR, 1, 1, MPI_COMM_WORLD);
      
      std::cout << "Task " << rank << " buffered send buffered , process will block on detach " << std::endl;;

      MPI_Buffer_detach(buf, &bufsize); 
    }
  else
    {
      MPI_Recv(inmsg, bufsize, MPI_CHAR, MPI_ANY_SOURCE, MPI_ANY_TAG, MPI_COMM_WORLD, &Stat);
      rc = MPI_Get_count(&Stat, MPI_CHAR, &count);
      std::cout << "Task  "<< rank << " : Received " << count << " char(s) from task " <<
        Stat.MPI_SOURCE << " with tag " <<  Stat.MPI_TAG << " and message : " << std::string(inmsg) << std::endl;
      
    }

}

void standard_send_recv(int rank, char *buf, int bufsize, char *inmsg,
                        std::string outmsg)
{
  int count, rc;
  MPI_Status Stat;
  
  if (rank == 0)
    {
      std::cout  << "Standard ...." << std::endl;
      MPI_Send(outmsg.c_str(), bufsize, MPI_CHAR, 1, 1, MPI_COMM_WORLD);
      std::cout << "Task " << rank << " process may will block here " << std::endl;;
    }
  else
    {
      MPI_Recv(inmsg, bufsize, MPI_CHAR, MPI_ANY_SOURCE, MPI_ANY_TAG, MPI_COMM_WORLD, &Stat);
      rc = MPI_Get_count(&Stat, MPI_CHAR, &count);
      std::cout << "Task  "<< rank << " : Received " << count << " char(s) from task " <<
        Stat.MPI_SOURCE << " with tag " <<  Stat.MPI_TAG << " and message : " << std::string(inmsg) << std::endl;
      
    }

}


// Main program
int main(int argc, char** argv) {

  MPI_Init(NULL, NULL);
  int rank;
  int numtasks;
  MPI_Comm_size(MPI_COMM_WORLD, &numtasks);
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);

  if (numtasks != 2 )
    {
      std::cout << "The number of process must be equal to 2" << std::endl;

      MPI_Finalize();
      return 1;
    }

  char* inmsg;
  std::string outmsg = "Testing";
  int bufsize = outmsg.length();
  char *buf = new char[bufsize]; 
  inmsg = new char[bufsize];

  // Send the same mesage with different modes
  buffered_send_recv(rank, buf, bufsize, inmsg, outmsg);
  standard_send_recv(rank, buf, bufsize, inmsg, outmsg);

  
  delete buf;
  delete inmsg;
  buf = 0;
  inmsg = 0;
  MPI_Finalize();
}
