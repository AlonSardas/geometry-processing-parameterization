#include "lscm.h"
#include <Eigen/Dense>
#include <gtest/gtest.h>

// Mainly checks that everything compiles and run
TEST(LSCMTest, TestSanity) {
  Eigen::MatrixXd V(4, 3);
  V << 0, 0, 0, 1, 0, 0, 1, 1, 0, 0, 1, 0;

  Eigen::MatrixXi F(2, 3);
  F << 0, 1, 2, 0, 2, 3;

  Eigen::MatrixXd U;
  lscm(V, F, U);

  ASSERT_EQ(U.cols(), 2);
  ASSERT_EQ(U.rows(), V.rows());
}