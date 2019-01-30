#include <iostream>
#include <vector>

using namespace std;

int main()
{
  const int n = 10000;
  vector<vector<int>> mat(n);
  vector<vector<int>> new_mat(n);

  // Filling Matrix
  for(int row = 0; row < n; ++row)
  {
    mat[row].resize(n);
    new_mat[row].resize(n);
    for(int col = 0; col < n; ++col)
    {
      mat[row][col] = row;
    }
  }



  // Transposing Matrix
  for(int idx1 = 0; idx1 < n; ++idx1)
  {
    for(int idx2 = 0; idx2 <= idx1; ++idx2)
    {
      new_mat[idx1][idx2] = mat[idx2][idx1];
      new_mat[idx2][idx1] = mat[idx1][idx2];
    }
  }
  //for(int row = 0; row < n; ++row)
  //{
    //for(int col = 0; col < n; ++col)
    //{
      //new_mat[row][col] = mat[col][row];
    //}
  //}

  // Printing Transpose
  //for(int row = 0; row < n; ++row)
  //{
    //for(int col = 0; col < n; ++col)
    //{
      //std::cout << new_mat[row][col] << " "; 
    //}
    //std::cout << std::endl;
  //}

  return 0;
}
