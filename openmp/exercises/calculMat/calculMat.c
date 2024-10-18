////////////////////////////////////////////////////////////////////////////////
//
// calculMat.c --- matrix calculations.
//
////////////////////////////////////////////////////////////////////////////////

#include <stdbool.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <sys/time.h>

// OpenMP header
#include "omp.h"

// Number of iterations
#ifndef VAL_N
#define VAL_N 60000000
#endif

////////// OpenMP uses for some matrix calculations ///////////
void cal_mat_opt(float *a, float *b, float *c, float *d, float *e,
		long int n, int *nbIterationsPerthreads, const int nbThreads)
{
  long int i;
  int nbIterations_currentThread = 0;

  //#pragma omp parallel default(none), shared(a, b, c, d, e, n, nbIterationsPerthreads, nbThreads), private(nbIterations_currentThread)
#pragma omp parallel default(none), shared(a, b, c, d, e, n, nbIterationsPerthreads), private(nbIterations_currentThread)
  {
    nbIterations_currentThread = 0;
    // Private thread memory ("thread" stack)
    double a_int;
    double b_int;
    double c_int;
    double d_int;
    double e_int;


#pragma omp for schedule(static), private(a_int, b_int, c_int, d_int, e_int)
    for (i=0; i<n; i++)
      {
	// Store values into private thread memory
	b_int = b[i];
	c_int = c[i];
	d_int = d[i];
	e_int = e[i];

	// Calculation (only on thread memory)
	a_int = b_int + c_int + d_int + e_int;

	// Dummy loop (the number of iteration can be decreased)
	for (int j = 0; j < 100; j++)
	  {
	    a_int += b_int*c_int - d_int * e_int;
	    a_int += b_int +c_int - d_int * e_int;
	    a_int += b_int - c_int * d_int * e_int;
	    a_int += b_int + c_int * d_int * e_int;
	    a_int += 5*b_int - c_int + d_int - 5*e_int;
	  }

	nbIterations_currentThread++;
	// Assign value to heap (inside input array)
	a[i] = a_int;

      } // Implicit barrier

    // Assign value to heap (inside input array)
    nbIterationsPerthreads[omp_get_thread_num()] = nbIterations_currentThread;

    // Explicit barrier to wait all threads and be sure that nbIterationsPerThreads was updated
    // by all threads
#pragma omp barrier

#pragma omp single
    {
      // For each thread, print the number of iteration
      for (int j = 0; j < nbThreads; j++)
	{
	  printf("nbIterations : %d for thread %d \n", nbIterationsPerthreads[j], j);
	}
      printf("Value for the array a[0] = %f \n", a[0]);
    }
  } // barrier here
}

void cal_mat(float *a, float *b, float *c, float *d, float *e,
	     long int n, int *nbIterationsPerthreads, const int nbThreads)
{
  long int i;
  bool with_iterationDisplay = false;

  //#pragma omp parallel default(none), shared(a, b, c, d, e, n, nbIterationsPerthreads, nbThreads, with_iterationDisplay)
#pragma omp parallel default(none), shared(a, b, c, d, e, n, nbIterationsPerthreads, with_iterationDisplay)
  {
#pragma omp for schedule(static)
    for (i = 0; i < n; i++)
      {
	a[i] = b[i] + c[i] + d[i] + e[i];

	// Dummy loop (the number of iteration can be decreased)
	for (int j = 0; j < 100; j++)
	  {
	    a[i] += b[i]*c[i] - d[i] * e[i];
	    a[i] += b[i] +c[i] - d[i] * e[i];
	    a[i] += b[i] - c[i] * d[i] * e[i];
	    a[i] += b[i] + c[i] * d[i] * e[i];
	    a[i] += 5*b[i] - c[i] + d[i] - 5*e[i];
	  }
	nbIterationsPerthreads[omp_get_thread_num()]++;
      } // Implicit barrier

#pragma omp single
	{
	  if (with_iterationDisplay)
	    {
	      // For each thread, print the number of iteration
	      for (int j = 0; j < nbThreads; j++)
		{
		  printf("nbIterations : %d for thread %d \n", nbIterationsPerthreads[j], j);
		}
	       }
	  printf("Value for the array a[0] = %f \n", a[0]);

      } // barrier here
  }
}


///////////////// main /////////////
int main(int argc, char *argv[])
{
  // Get nb threads
  int nb_threads;
#pragma omp parallel
  {
    nb_threads = omp_get_num_threads();
  }
  fprintf(stdout, "\n\n   Matrix calcultations with %d threads\n", nb_threads);


  // Declarations
  float* a, *b, *c, *d, *e;
  int n =VAL_N;

  int nbIterationsPerthreads[nb_threads]; // false-sharing possible here

  // Allocations
  a = (float*) calloc(n, sizeof(float));
  b = (float*) malloc(n * sizeof(float));
  c = (float*) malloc(n * sizeof(float));
  d = (float*) malloc(n * sizeof(float));
  e = (float*) malloc(n * sizeof(float));

  // Initzialiation
  for (long int k = 0; k < n; k++)
    {
      b[k] = 1.;
      c[k] = 2.;
      d[k] = 3.;
      e[k] = 4.;
    }
  memset(nbIterationsPerthreads, 0, nb_threads*sizeof(int));

  // CPU time.
  clock_t t_cpu_0 = clock();

  // Elapsed time
  struct timeval t_elapsed_0;
  gettimeofday(&t_elapsed_0, NULL);


  ///////////////
  // Call cal_mat function : cal_mat or cal_mat_opt
  ///////////////
  cal_mat(a, b, c, d, e, n, nbIterationsPerthreads, nb_threads);

  // Elapsed time at the end
  struct timeval t_elapsed_1;
  gettimeofday(&t_elapsed_1, NULL);
  double t_elapsed = (t_elapsed_1.tv_sec - t_elapsed_0.tv_sec) + (t_elapsed_1.tv_usec - t_elapsed_0.tv_usec) / (double)1000000;

  //  CPU time at the end
  clock_t t_cpu_1 = clock();
  double t_cpu = (t_cpu_1 - t_cpu_0) / (double)CLOCKS_PER_SEC;


  // Free Memory
  free(a);
  a = 0;
  free(b);
  b = 0;
  free(c);
  c = 0;
  free(d);
  d = 0;
  free(e);
  e = 0;

  // Display results
  fprintf(stdout, "\n\n"
	  "   Nb iterations              : %10d\n"
	  "   Temps elapsed              : %10.3E sec.\n"
	  "   Temps CPU                  : %10.3E sec.\n",
	  n, t_elapsed, t_cpu
	  );

  return EXIT_SUCCESS;

  return 0;
}
