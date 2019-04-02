#include <iostream>
#include <vector>
#include <cmath>
#include <complex>
#include <random>
#include <fftw3.h>

typedef std::vector<double> vecd; 
typedef std::vector<std::vector<double>> matd; 

#define REAL 0
#define IMAG 1
#define img std::complex<double>(0.0,1.0)

vecd matVec(matd mat, vecd vec)
{
  // computes simple mat vec
  // Assumes dimensions are correct
  int row_size = mat.size(), col_size = mat[0].size();
  vecd answer(row_size, 0.0);
  for(int r=0; r<row_size; ++r)
  {
    for(int c=0; c<col_size; ++c)
    {
      answer[r] += mat[r][c] * vec[c];
    }
  }
  return answer;
}

void testMatVec()
{
  matd mat = {{1,2,3},{4,5,6},{7,8,9}};
  vecd vec = {2.2, 5.7, 2};
  std::cout << "Matrix = \n";
  for(int r=0; r<3; ++r)
  {
    for(int c=0; c<3; ++c)
      std::cout << mat[r][c] << " ";
    std::cout << std::endl;
  }   
  std::cout << "Vector = \n";
  for(int i=0; i<3; ++i)
    std::cout << vec[i] << " ";
  std::cout << std::endl;

  std::cout << "Resultant Vector = \n";
  vecd my_matvec = matVec(mat, vec);
  for(int i=0; i<3; ++i)
    std::cout << my_matvec[i] << " ";
  std::cout << std::endl;
}

matd createCircMat(vecd circ_mat_vec, int mat_dim)
{
  matd circ_mat(mat_dim, vecd(mat_dim));
  for(int r=0; r<mat_dim; ++r)
    for(int c=0; c<mat_dim; ++c)
      circ_mat[c][r] = circ_mat_vec[(mat_dim-r+c)%mat_dim];

  return circ_mat;
}

void printMat(matd mat, int dim)
{
  for(int r=0; r<dim; ++r)
  {
    for(int c=0; c<dim; ++c)
      std::cout << mat[r][c] << " ";
    std::cout << std::endl;
  }
}

int main(int argc, char** argv)
{
  int i, size;
  if(argc>1)
    size = atoi(argv[1]);
  else
    size = 5;
  vecd vec(size), circ_mat_vec(size), fft_sol(size), sol(size);
  matd circ_mat;
  std::default_random_engine generator;
  std::uniform_real_distribution<double> distribution(0.0, 1.0);
  fftw_complex circ_mat_vec_fftw[size], ft_mat_vec[size], vec_fftw[size],
               ft_vec[size], fft_sol_fftw[size], ft_fft_sol[size];

  // Generate matrix and vector
  for(i=0; i<size; ++i)
  {
    circ_mat_vec[i] = distribution(generator);
    vec[i] = distribution(generator);
    circ_mat_vec_fftw[i][REAL] = circ_mat_vec[i];
    circ_mat_vec_fftw[i][IMAG] = 0.0;
    vec_fftw[i][REAL] = vec[i];
    vec_fftw[i][IMAG] = 0.0;
  }

  // Get solution using traditional matvec
  circ_mat = createCircMat(circ_mat_vec, size);  
  sol = matVec(circ_mat, vec);  

  // Set up FFT routines (plans) 
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
  std::complex<double> cmplx_prod;
  for(i=0; i<size; ++i)
  {
    cmplx_prod = std::complex<double>(ft_mat_vec[i][REAL], ft_mat_vec[i][IMAG])
                 * std::complex<double>(ft_vec[i][REAL], ft_vec[i][IMAG]) / 
                 (std::complex<double>)size;
    ft_fft_sol[i][REAL] = cmplx_prod.real();
    ft_fft_sol[i][IMAG] = cmplx_prod.imag();
  }

  // Inverse FFT solution
  fftw_execute(ift_sol_plan);

//  std::cout << "FFT Answer = \n";
//  for(i=0; i<size; ++i)
//    std::cout << fft_sol_fftw[i][REAL] << " ";
//  std::cout << std::endl;
//  std::cout << "Answer = \n";
//  for(i=0; i<size; ++i)
//    std::cout << sol[i] << " ";
//  std::cout << std::endl;

  // Cleanup
  fftw_destroy_plan(ft_mat_plan);
  fftw_destroy_plan(ft_vec_plan); 
  fftw_destroy_plan(ift_sol_plan);

  return 0;
}




