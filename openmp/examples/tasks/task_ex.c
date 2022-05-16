#include <stdio.h>
#include <stdlib.h>
#include <unistd.h> 
#include "omp.h"

int main(int argc, char *argv[])
{
  omp_set_num_threads(4);

#pragma omp parallel
  {
#pragma omp single nowait
    {
      printf("A ");

#pragma omp task 
      {
	printf("car ");
      }
#pragma omp task
      {
	printf("race ");
      }
      
#pragma omp taskwait
      printf("is fun to watch \n");
    }
    
  }
  
  return 0;
}
