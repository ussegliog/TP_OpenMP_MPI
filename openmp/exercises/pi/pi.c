////////////////////////////////////////////////////////////////////////////////
//
// pi.c --- Pi estimation.
//
////////////////////////////////////////////////////////////////////////////////

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include <sys/time.h>

// OpenMP header
#include <omp.h>


// Number of intervals
#ifndef VAL_N
#define VAL_N 10000000
#endif

// Integrate function
double f(double a)
{
  return 4/(1 + a*a);
}


////////// OpenMP uses ///////////
double pi_estimation1(int n, double h)
{
  double Pi_calcule = 0;

  // Dummy loop (do not touch)
  for(int k=0; k<50; k++)
    {
      // Calcul de Pi
      Pi_calcule = 0;
#pragma omp parallel for reduction(+ : Pi_calcule) schedule(static)
    for(int i = 0; i<n; i++)
      {
	double x = h * (i + 0.5);
	Pi_calcule += f(x);
      }
    Pi_calcule = h * Pi_calcule;
    }

  return Pi_calcule;
  
}

double pi_estimation2(int n, double h)
{
  double Pi_calcule = 0;
  double Pi_calcule_loc;
  double x;
  
  // Dummy loop (do not touch)
  for(int k=0; k<50; k++)
    {
    // Pi estimation
    Pi_calcule = 0;
    Pi_calcule_loc = 0;
    
#pragma omp parallel firstprivate(Pi_calcule_loc, x)
    {
#pragma omp for schedule(static)
      for(int i = 0; i< n; i++)
	{
	  x = h * (i + 0.5);
	  Pi_calcule_loc += f(x);
	}
#pragma omp critical
	{
	  Pi_calcule += Pi_calcule_loc;
	}
    }

    Pi_calcule = h * Pi_calcule;
  }

  printf("Pi_calcule %f\n", Pi_calcule);
  return Pi_calcule;
  
}


/////////// main ///////////
int main()
{

  //         __         
  //  Aim :  ||  estimation.
  //
  //                   / 1
  //                  |       4            __
  //                  |   ----------  dx = ||
  //                  |    1 + x**2
  //                 / 0

  // Get nb threads
  int nb_threads;
#pragma omp parallel
  {
    nb_threads = omp_get_num_threads();
  }
  fprintf(stdout, "\n\n   Pi estimation with %d threads\n", nb_threads);


  int n=VAL_N;

  // Size of each interval.
  double h = 1.0 / n;

  // CPU time.
  clock_t t_cpu_0 = clock();

  // Elapsed time
  struct timeval t_elapsed_0;
  gettimeofday(&t_elapsed_0, NULL);

  ///////////////
  // Call Pi estimation funtion : pi_estimation1 or pi_estimation2
  double Pi_calcule = pi_estimation2(n, h);
  ///////////////
  
  // Elapsed time at the end
  struct timeval t_elapsed_1;
  gettimeofday(&t_elapsed_1, NULL);
  double t_elapsed = (t_elapsed_1.tv_sec - t_elapsed_0.tv_sec) + (t_elapsed_1.tv_usec - t_elapsed_0.tv_usec) / (double)1000000;

  //  CPU time at the end
  clock_t t_cpu_1 = clock();
  double t_cpu = (t_cpu_1 - t_cpu_0) / (double)CLOCKS_PER_SEC;

  // Gap between "true" value and estimated one.
  double Pi_estime = acos(-1);   // "true" value
  double gap = fabs(Pi_estime - Pi_calcule);

  // Display results
  fprintf(stdout, "\n\n"
	  "   Nombre d intervalles       : %10d\n"
	  "   | Pi_estime - Pi_calcule | : %10.3E\n"
	  "   Temps elapsed              : %10.3E sec.\n"
	  "   Temps CPU                  : %10.3E sec.\n",
	  n, gap, t_elapsed, t_cpu
	  );

  return EXIT_SUCCESS;
}
