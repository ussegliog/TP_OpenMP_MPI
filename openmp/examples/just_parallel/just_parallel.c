#include <stdio.h>
#include <stdlib.h>
#include "omp.h"

void fonc(int id, double * A);

int main(int argc, char *argv[])
{
  double A[1000];
  omp_set_num_threads(4);

    
#pragma omp parallel
  {
    int ID = omp_get_thread_num();
    
    fonc(ID, A);
 }
  return 0;
}


void fonc(int id, double * A)
{
  int var_loc = 0;
  
  printf("adresse de A (partagee) : %p pour l'id %d \n", A, id);
  printf("adresse de var_loc (locale et privee) : %p pour l'id %d \n", &var_loc, id);
}
