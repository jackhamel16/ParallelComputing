#include <omp.h>
#include <iostream>
#include <vector>

void print_matrix(std::vector<std::vector<int>> matrix, int dim)
{
  std::cout << "Matrix:" << std::endl;
  for(int i=0; i < dim; ++i)
  {
    for(int j=0; j < dim; ++j)
      std::cout << matrix[i][j] << " ";
    std::cout << std::endl;
  }
}

int main()
{
  int dim = 10;
//  std::vector<std::vector<int>> matrix(dim, std::vector<int>(dim, 0));
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
//  #pragma omp parallel for
//  for(int i=0; i < dim; ++i)
//  {
//    for(int j=0; j < dim; ++j)
//    {
//      int id, nthrds;
//      id = omp_get_thread_num();
//      nthrds = omp_get_num_threads();
//      if(id == 0) total_threads = nthrds;
//      printf("Thread %d is working\n", id);
//
//      matrix[i][j] = id;
//    }
//  } 

  std::cout << "Total threads: " << total_threads << std::endl;

//  print_matrix(matrix, 3);
  for (int i=0; i < dim; ++i)
    std::cout << vec[i] << " " << std::endl;

  return 0;
}
