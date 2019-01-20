#include <iostream>
#include <iomanip>
#include <fstream>
#include <math.h>
#include <Eigen/Dense>

//#define EIGEN_STACK_ALLOCATION_LIMIT 100000000

int main()
{
  const double gamma = 1;
  const double x_min = 0;
  const double x_max = 10;
  const int x_total = 10;
  const double t_min = 0;
  const double t_max = 10;
  const int t_total = 100;
  double dx = (x_max - x_min) / x_total;
  double dt = (t_max - t_min) / t_total;

  Eigen::Matrix<double, x_total, 1> x_vec = Eigen::VectorXd::LinSpaced(x_total, x_min, x_max);
  Eigen::Matrix<double, Eigen::Dynamic, 1> t_vec = Eigen::VectorXd::LinSpaced(t_total, t_min, t_max);
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
      y_vec[x_idx] = y_vec[x_idx] + v_vec[x_idx] * dt;
      v_vec[x_idx] = v_vec[x_idx] + a_vec[x_idx] * dt;
    }
    outfile << y_vec.transpose() << std::endl;
  }

  //for(int y_idx = 0; y_idx < x_total; ++y_idx)
  //{
    //y_vec[y_idx] = exp(-std::pow(x_vec[y_idx] - 5, 2));
    //y_mat(0, y_idx) = exp(-std::pow(x_vec[y_idx] - 5, 2));
  //}

  //for(double t = 0; t < t_max; t = t + dt)
  //{
    //for(int y_idx = 1; y_idx < x_total - 1; ++y_idx)
    //{
      //a_vec[y_idx] = gamma * ( y_vec[y_idx - 1] + y_vec[y_idx + 1]
                     //- 2 * y_vec[y_idx] ) / (dx * dx);
      //y_vec[y_idx] = y_vec[y_idx] + v_vec[y_idx] * dt;
      //v_vec[y_idx + 1] = v_vec[y_idx] + a_vec[y_idx] * dt;
    //}
    //outfile << y_vec.transpose() << std::endl;
  //}

  return 0;
}
