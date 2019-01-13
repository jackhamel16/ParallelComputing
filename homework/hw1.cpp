#include <iostream>
#include <iomanip>
#include <fstream>
#include <math.h>
#include <Eigen/Dense>

int main()
{
  const double gamma = 1;
  const double x_min = 0;
  const double x_max = 10;
  const int x_total = 500;
  const double t_min = 0;
  const double t_max = 10;
  const int t_total = 1e2;
  double dx = x_max / x_total;
  double dt = t_max / t_total;

  Eigen::Matrix<double, x_total, 1> y_vec = Eigen::MatrixXd::Zero(x_total, 1);
  Eigen::Matrix<double, x_total, 1> v_vec = Eigen::MatrixXd::Zero(x_total, 1);
  Eigen::Matrix<double, x_total, 1> a_vec = Eigen::MatrixXd::Zero(x_total, 1);
  //Eigen::Matrix<double, x_total, 1> x_vec = Eigen::VectorXd::LinSpaced(x_total, x_min, x_max);
  //Eigen::Matrix<double, t_total, 1> t_vec = Eigen::VectorXd::LinSpaced(t_total, t_min, t_max);

  for(int y_idx = 1; y_idx < x_total - 1; ++y_idx)
  {
    y_vec[y_idx] = exp(std::pow(y_idx * dx - 5, 2));
  }

  std::ofstream outfile("hw_out.dat");
  outfile << std::scientific << std::setprecision(15);

  for(double t = 0; t < t_max; t = t + dt)
  {
    for(int y_idx = 1; y_idx < x_total - 1; ++y_idx)
    {
      a_vec[y_idx] = gamma * ( y_vec[y_idx - 1] + y_vec[y_idx + 1]
                     - 2 * y_vec[y_idx] ) / (dx * dx);
      y_vec[y_idx] = y_vec[y_idx] + v_vec[y_idx] * dt;
      v_vec[y_idx + 1] = v_vec[y_idx] + a_vec[y_idx] * dt;
    }
    outfile << y_vec.transpose() << std::endl;
  }

  return 0;
}
