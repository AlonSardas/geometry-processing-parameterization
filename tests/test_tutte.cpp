#include "tutte.h"
#include <Eigen/Dense>
#include <cmath>
#include <gtest/gtest.h>

TEST(TutteMapping, CubeWithoutBottom) {
  // Cube vertices (unit cube)
  Eigen::MatrixXd V(8, 3);
  V << 0, 0, 0, // 0
      1, 0, 0,  // 1
      0, 1, 0,  // 2
      1, 1, 0,  // 3
      0, 0, 1,  // 4
      1, 0, 1,  // 5
      0, 1, 1,  // 6
      1, 1, 1;  // 7

  Eigen::MatrixXi F(10, 3);
  // Top face (z=1): 4,5,6,7
  F << 4, 5, 7, 4, 7, 6,
      // Front face (y=0): 0,1,4,5
      0, 1, 5, 0, 5, 4,
      // Back face (y=1): 2,3,6,7
      2, 7, 3, 2, 6, 7,
      // Left face (x=0): 0,2,4,6
      0, 4, 6, 0, 6, 2,
      // Right face (x=1): 1,3,5,7
      1, 3, 7, 1, 7, 5;

  Eigen::MatrixXd U;
  tutte(V, F, U);

  // Output shape: #V by 2
  ASSERT_EQ(U.rows(), V.rows());
  ASSERT_EQ(U.cols(), 2);

  std::vector<int> boundary = {0, 1, 2, 3};

  const double tol = 1e-6;
  for (int b : boundary) {
    double r = std::sqrt(U(b, 0) * U(b, 0) + U(b, 1) * U(b, 1));
    EXPECT_NEAR(r, 1.0, tol)
        << "Boundary vertex " << b << " is not on the unit circle, r=" << r;
  }

  // Interior vertices (all non-boundary) should be strictly inside the unit
  // disk
  std::set<int> boundary_set(boundary.begin(), boundary.end());
  for (int i = 0; i < V.rows(); ++i) {
    if (boundary_set.count(i))
      continue;
    double r = std::sqrt(U(i, 0) * U(i, 0) + U(i, 1) * U(i, 1));
    EXPECT_LT(r, 1.0 - tol) << "Interior vertex " << i
                            << " is not strictly inside the unit disk, r=" << r;
  }

  //   std::cout << U << std::endl;
}
