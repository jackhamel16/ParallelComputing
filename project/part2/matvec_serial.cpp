#include <iostream>
#include <vector>
#include <cmath>
#include <complex>
#include<fftw3.h>

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

matd createCircMat(vecd circ_mat_row, int mat_dim)
{
  matd circ_mat(5, vecd(5));
  for(int r=0; r<mat_dim; ++r)
    for(int c=0; c<mat_dim; ++c)
      circ_mat[r][c] = circ_mat_row[(mat_dim-r+c)%mat_dim];

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

int main()
{
  int i;
  vecd circ_mat_row = {4,2,5,5,7};
  vecd vec = {2,5.2,6,6,8.9};

  int size = circ_mat_row.size();
  matd circ_mat = createCircMat(circ_mat_row, size);  
  vecd result = matVec(circ_mat, vec);  

  fftw_complex circ_mat_row_fftw[size];
  fftw_complex ft_mat_row[size];
  fftw_complex vec_fftw[size];
  fftw_complex ft_vec[size];
  fftw_complex answer_fftw[size];
  fftw_complex ft_answer[size];

  fftw_plan mat_plan = fftw_plan_dft_1d(size, circ_mat_row_fftw, ft_mat_row,
                                        FFTW_FORWARD, FFTW_ESTIMATE);
  fftw_plan vec_plan = fftw_plan_dft_1d(size, vec_fftw, ft_vec, FFTW_FORWARD,
                                        FFTW_ESTIMATE);
  fftw_plan answer_plan = fftw_plan_dft_1d(size, answer_fftw, ft_answer,
                                           FFTW_BACKWARD, FFTW_ESTIMATE);

  //Fill fftw objects
  for(i=0; i<size; ++i)
  {
    circ_mat_row_fftw[i][REAL] = circ_mat_row[i];
    circ_mat_row_fftw[i][IMAG] = 0.0;
    vec_fftw[i][REAL] = vec[i];
    vec_fftw[i][IMAG] = 0.0;
  }
 
  fftw_execute(mat_plan);
  fftw_execute(vec_plan);
 
  //effect matvec using ffts
  std::complex<double> cmplx_prod;
  for(i=0; i<size; ++i)
  {
    cmplx_prod = std::complex<double>(ft_mat_row[i][REAL], ft_mat_row[i][IMAG])
                 * std::complex<double>(ft_vec[i][REAL], ft_vec[i][IMAG]);
    ft_answer[i][REAL] = cmplx_prod.real();
    ft_answer[i][IMAG] = cmplx_prod.imag();
  }

  fftw_execute(answer_plan);

  std::cout << "Answer = \n";
  for(i=0; i<size; ++i)
    std::cout << answer_fftw[i][IMAG] << " ";
  std::cout << std::endl;

  fftw_destroy_plan(mat_plan);
  fftw_destroy_plan(vec_plan); 
  fftw_destroy_plan(answer_plan);

  

  return 0;
}




