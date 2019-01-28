#include <iostream>

int main()
{
  const int n = 3000;
  int (*mat)[n] = new int mat[n][n];// =  {{1, 2, 3}, {4, 5, 6}, {7, 8, 9}};
  int new_mat[n][n];
  
  // Filling Matrix
  for(int idx1 = 0; idx1 < n; ++idx1)
  {
    for(int idx2 = 0; idx2 < n; ++idx2)
    {
      mat[idx1][idx2] = idx1;
    }
  }
     
  // Transposing Matrix
  for(int idx1 = 0; idx1 < n; ++idx1)
  {
    for(int idx2 = 0; idx2 < n; ++idx2)
    {
      new_mat[idx1][idx2] = mat[idx2][idx1];
    }
  }

  // Printing Transpose
  //for(int idx1 = 0; idx1 < n; ++idx1)
  //{
    //for(int idx2 = 0; idx2 < n; ++idx2)
    //{
      //std::cout << new_mat[idx1][idx2] << " "; 
    //}
    //std::cout << std::endl;
  //}

  return 0;
}
