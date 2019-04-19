#include "matvec.h"

int main(int argc, char** argv)
{
  int i, size;
  if(argc>1)
    size = atoi(argv[1]);
  else
    size = 5;
  vecd vec(size), circ_mat_vec(size), sol(size);
  matd circ_mat;
  std::default_random_engine generator;
  std::uniform_real_distribution<double> distribution(0.0, 1.0);
  std::chrono::time_point<std::chrono::high_resolution_clock> start, end;
  std::chrono::duration<double> slow_time, fft_time; 

  // Generate matrix and vector
  for(i=0; i<size; ++i)
  {
    circ_mat_vec[i] = distribution(generator);
    vec[i] = distribution(generator);
  }

  // Get solution using traditional matvec
  circ_mat = createCircMat(circ_mat_vec, size);  
  start = std::chrono::high_resolution_clock::now();
  sol = matVec(circ_mat, vec);  
  end = std::chrono::high_resolution_clock::now();
  slow_time = end - start;

  std::cout << "Serial Slow Time: " << slow_time.count() << " s\n";

  return 0;
}




