#include <iostream>
#include <vector>

using namespace std;

class matrix
{
 public:
  matrix(int, int);
  std::vector<int> dimensions();
  int operator()(int, int );
  void set_val(int, int, int);

 private:
  int n, m, vec_size;
  int coord2idx
}

matrx::matrix(int n, int m)
  : n(n),
    m(m),
    vec_size(n * m)
{
}

std::vector<int> matrix::dimensions()
{
  return std::vector<int>(n, m);
}

int matrix::operator()(int row, int col)
{
  return

int main()
{
  const int n = 3;
  int num_of_elements = n * n;
  vector<int> mat;
  
  for(int mat_idx = 0; mat_idx < num_of_elements; ++mat_idx)
  {
  

  return 0;
}
