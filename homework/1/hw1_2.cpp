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
  const int x_total = 100;
  const double t_min = 0;
  const double t_max = 10;
  const int t_total = 10000;
  double dx = (x_max - x_min) / x_total;
  double dt = (t_max - t_min) / t_total;

  Eigen::Matrix<double, x_total, 1> x_vec = Eigen::VectorXd::LinSpaced(x_total, x_min, x_max);
  Eigen::Matrix<double, x_total, 1> y_vec = Eigen::MatrixXd::Zero(x_total, 1);
  Eigen::Matrix<double, x_total, 1> v_vec = Eigen::MatrixXd::Zero(x_total, 1);
  Eigen::Matrix<double, x_total, 1> a_vec = Eigen::MatrixXd::Zero(x_total, 1);

  std::ofstream outfile("hw_out.dat");
  outfile << std::scientific << std::setprecision(15);

  for(int x_idx = 0; x_idx < x_total; ++x_idx)
  {
    y_vec[x_idx] = std::exp(-std::pow(x_vec[x_idx] - 5, 2));
  }

  for(int t_idx = 0; t_idx < t_total; ++t_idx)
  {
    a_vec[0] = 0;
    a_vec[x_total - 1] = 0;
    for(int x_idx = 1; x_idx < x_total - 1; ++x_idx)
    {
      a_vec[x_idx] = gamma / (dx * dx) * (y_vec[x_idx + 1] + y_vec[x_idx - 1]
                     - 2 * y_vec[x_idx]);
    }
    y_vec = y_vec + v_vec * dt;
    v_vec = v_vec + a_vec * dt;
    //y_vec[x_idx] = y_vec[x_idx] + v_vec[x_idx] * dt;
    //v_vec[x_idx] = v_vec[x_idx] + a_vec[x_idx] * dt;
    outfile << y_vec.transpose() << std::endl;
  }

  return 0;
}
