#include <iostream>
#include <vector>
#include <cmath>
//#include <omp.h>
//#include <

typedef std::vector<double> vecd; 
typedef std::vector<std::vector<double>> matd; 

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
  vecd circ_mat_row = {4,2,5,5,7};
  vecd vec = {2,5.2,6,6,8.9};

  int mat_dim = circ_mat_row.size();
  matd circ_mat = createCircMat(circ_mat_row, mat_dim);  
  vecd result = matVec(circ_mat, vec);  

  return 0;
}
