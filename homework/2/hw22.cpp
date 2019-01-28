#include <iostream>
#include <vector>

using namespace std;

int main()
{
  const int n = 3000;
  vector<vector<int>> mat(n);
  vector<vector<int>> new_mat(n);
  
  //for(int idx = 0; idx < n; ++idx)
  //{
    //mat[i].resize(n);
  //}

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
  for(int row = 0; row < n; ++row)
  {
    for(int col = 0; col < n; ++col)
    {
      new_mat[row][col] = mat[col][row];
    }
  }

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
