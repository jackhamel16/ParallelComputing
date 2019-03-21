#include <stdio.h>
#include <sys/time.h>

int main()
{
  struct timeval start, end;

  gettimeofday(&start, NULL);
  int A;
  for(int i=0; i<1000000; ++i)
  {
    A = i;
  }
  gettimeofday(&end, NULL);
  
  double elapsed_time = end.tv_sec - start.tv_sec;
  printf("run time: %f \n", elapsed_time); 

  return 0;
}
