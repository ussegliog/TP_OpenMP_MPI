

#include <stdio.h>
#include <stdlib.h>
#include "omp.h"

void fonc(int id, double * A);

int main(int argc, char *argv[])
{
  double A[1000];
  omp_set_num_threads(4);

  int var_firstPriv = 12;
  int var_priv = 12;
  int var_shared = 12;
  
#pragma omp parallel default(none), private(A), firstprivate(var_firstPriv), \
  private(var_priv), shared(var_shared)
  {
    int ID = omp_get_thread_num();

    var_firstPriv += 42;
    var_priv += 42;
    
#pragma omp atomic
      var_shared += 42;
#pragma omp crtical
    {
      printf("valeurs dans // pour l'id %d : var_firstPriv = %d,  var_priv = %d,  var_shared = %d  \n", ID, var_firstPriv, var_priv, var_shared);
    }
    
    fonc(ID, A);
 }

  printf("valeurs hors // : var_firstPriv = %d,  var_priv = %d,  var_shared = %d  \n", var_firstPriv, var_priv, var_shared);
  
  return 0;
}


void fonc(int id, double * A)
{
  printf("adresse de A : %p pour l'id %d \n", A, id);
}
