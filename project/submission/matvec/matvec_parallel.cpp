#include <omp.h>
#include <fftw3.h>

#include "matvec.h"

#define N_THREADS 16

int main(int argc, char** argv)
{
  int i, size, n_threads;
  if(argc>2) {
    size = atoi(argv[1]);
    n_threads = atoi(argv[2]);
  }
  else if(argc>1) {
    size = atoi(argv[1]);
    n_threads = N_THREADS;
  }
  else {
    size = 5;
    n_threads = N_THREADS;
  }

  fftw_complex circ_mat_vec_fftw[size], ft_mat_vec[size], vec_fftw[size],
               ft_vec[size], fft_sol_fftw[size], ft_fft_sol[size];
  std::chrono::time_point<std::chrono::high_resolution_clock> start, end;
  std::chrono::duration<double> slow_time, fft_time; 

  omp_set_num_threads(n_threads);

  int thread_count;
  // Generate matrix and vector
  #pragma omp parallel for
  for(i=0; i<size; ++i)
  {
    if(omp_get_thread_num() == 0) thread_count = omp_get_num_threads();
    std::default_random_engine generator;
    std::uniform_real_distribution<double> distribution(0.0, 1.0);
    circ_mat_vec_fftw[i][REAL] = distribution(generator);
    circ_mat_vec_fftw[i][IMAG] = 0.0;
    vec_fftw[i][REAL] = distribution(generator);
    vec_fftw[i][IMAG] = 0.0;
  }

  std::cout << "thread count = " << thread_count << "\n";
  start = std::chrono::high_resolution_clock::now();

  // Set up FFT routines (plans) 
  fftw_init_threads();
  fftw_plan_with_nthreads(n_threads);

  fftw_plan ft_mat_plan = fftw_plan_dft_1d(size, circ_mat_vec_fftw, ft_mat_vec,
                                        FFTW_FORWARD, FFTW_ESTIMATE);
  fftw_plan ft_vec_plan = fftw_plan_dft_1d(size, vec_fftw, ft_vec, FFTW_FORWARD,
                                        FFTW_ESTIMATE);
  fftw_plan ift_sol_plan = fftw_plan_dft_1d(size, ft_fft_sol, fft_sol_fftw,
                                           FFTW_BACKWARD, FFTW_ESTIMATE);
  // Execute initial FFTs
  fftw_execute(ft_mat_plan);
  fftw_execute(ft_vec_plan);

  //Effect matvec using FFTs
  #pragma omp parallel for
  for(i=0; i<size; ++i)
  {
    std::complex<double> cmplx_prod = std::complex<double>(ft_mat_vec[i][REAL],
                   ft_mat_vec[i][IMAG]) * std::complex<double>(ft_vec[i][REAL],
                   ft_vec[i][IMAG]) / (std::complex<double>)size;
    ft_fft_sol[i][REAL] = cmplx_prod.real();
    ft_fft_sol[i][IMAG] = cmplx_prod.imag();
  }

  // Inverse FFT solution
  fftw_execute(ift_sol_plan);

  end = std::chrono::high_resolution_clock::now();
  fft_time = end - start;
  std::cout << "Parallel Time: " << fft_time.count() << " s\n";

  // Cleanup
  fftw_destroy_plan(ft_mat_plan);
  fftw_destroy_plan(ft_vec_plan); 
  fftw_destroy_plan(ift_sol_plan);

  return 0;
}




