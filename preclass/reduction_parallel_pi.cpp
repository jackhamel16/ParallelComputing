#include <omp.h>
#include <iostream>

static long num_steps  = 100000000;
double step;
int main()
{
  int i; 
  double pi, sum=0.0;
  step = 1.0 / (double)num_steps;
  #pragma omp parallel for reduction(+:sum)
  for (i = 0; i < num_steps; i++) 
    { 
      auto num_threads = omp_get_num_threads();
      double x = (i + 0.5) * step;
      sum = sum + 4.0 / (1.0 + x * x);
    }
  pi = step * sum;
  std::cout << num_threads << std::endl;
}
