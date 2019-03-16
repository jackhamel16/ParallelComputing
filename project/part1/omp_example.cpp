#include <omp.h>
#include <iostream>
#include <vector>

int main()
{
  int dim = 10;
  std::vector<int> vec(dim);
  int total_threads;

  omp_set_num_threads(dim);
  #pragma omp parallel for
  for(int i=0; i < dim; ++i)
  {
    int id, nthrds;
    id = omp_get_thread_num();
    nthrds = omp_get_num_threads();
    if(id == 0) total_threads = nthrds;
    printf("Thread %d is working\n", id);
    vec[i] = id;
  } 
  std::cout << "Total threads: " << total_threads << std::endl;

  for (int i=0; i < dim; ++i)
    std::cout << vec[i] << " " << std::endl;

  return 0;
}
