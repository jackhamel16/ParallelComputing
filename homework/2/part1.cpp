#include <iostream>
#include <vector>
#include <time.h>

using namespace std;

int main(int argc, char* argv[])
{
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
  for(int row = 0; row < n; ++row)
  {
    for(int col = 0; col < n; ++col)
    {
      new_mat[row][col] = mat[col][row];
    }
  }
  end = clock();
  total_time = end - start;
  std::cout << total_time / CLOCKS_PER_SEC << std::endl;
  
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
