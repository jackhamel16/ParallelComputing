#include <iostream>
#include <vector>
#include <algorithm>
#include <time.h>

using namespace std;

int main(int argc, char* argv[])
{
  const int bsize = 16;
  int n = 4000;
  if(argc > 1)
  {
    n = atoi(argv[1]);
  }

  vector<vector<int>> mat(n);
  vector<vector<int>> new_mat(n);

  clock_t start, end;
  double total_time;

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
  start = clock();
  for(int row = 0; row < n; row = row + bsize)
  {
    for(int col = 0; col < n; col = col + bsize)
    {
        //int climit = min(row + bsize - 1, n)
      for(int row1 = row; row1 < min(row + bsize - 1, n); ++row1)
      {
        for(int col1 = col; col1 < min(col + bsize - 1, n); ++col1)
        {
          new_mat[row1][col1] = mat[col1][row1];
        }
      }  
    }
  }
  end = clock();
  total_time = end - start;
  cout << total_time / CLOCKS_PER_SEC << endl;
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
