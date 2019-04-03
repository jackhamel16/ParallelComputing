#include <mpi.h>
#include <stdlib.h>
#include <stdio.h>
#define sqr(x)    ((x)*(x))
long random(void);

double dboard(int darts, int rank, int size)
{
  double x_coord,       /* x coordinate, between -1 and 1  */
         y_coord,       /* y coordinate, between -1 and 1  */
         pi,            /* pi  */
         r;             /* random number between 0 and 1  */
  int score,            /* number of darts that hit circle */
      n;
  long rd;
  unsigned long cconst; /* used to convert integer random number */
                        /* between 0 and 2^31 to double random number */
                        /* between 0 and 1  */

  cconst = 2 << (31 - 1); 
  cconst = RAND_MAX;
  score = 0;
  srand(rank); 
  /* "throw darts at board" */
  for (n = 1; n <= darts; n++)  {
    /* generate random numbers for x and y coordinates */
    rd = rand();
    //printf("Rand - %ld\t",rd);
    r = (double)rd/cconst;
    //printf("%10.8f\n",r);
    x_coord = (2.0 * r) - 1.0;
    r = (double)rand()/cconst;
    y_coord = (2.0 * r) - 1.0;
  
    /* if dart lands in circle, increment score */
    if ((sqr(x_coord) + sqr(y_coord)) <= 1.0)
      score++;
  } 
  pi = 4.0 * (double)score/(double)darts;

  // Send score and darts from all processors to master processor
  return(pi);
}

int main(int argc, char** argv)
{
  int rank, size;

  MPI_Status status;
  MPI_Init(&argc, &argv);
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  MPI_Comm_size(MPI_COMM_WORLD, &size);
  
  double pi = dboard(100000, rank, size); 
 
  if(rank==0) {
    double procpi;
    for(int proc=1; proc<size; ++proc) {
      MPI_Recv(&procpi, 1, MPI_DOUBLE, proc, 1, MPI_COMM_WORLD, &status);
      pi += procpi;
    }
    pi = pi / size;
    printf("pi = %f \n", pi);
  }
  else {
    MPI_Send(&pi, 1, MPI_DOUBLE, 0, 1, MPI_COMM_WORLD);
  } 

  MPI_Finalize();

  return 0;
}
