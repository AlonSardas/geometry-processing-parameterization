#include "vector_area_matrix.h"
#include <gtest/gtest.h>

TEST(VectorAreaMatrixTest, SingleTriangle) {
  // Simple right triangle in xy plane
  Eigen::MatrixXd V(3, 3);
  V << 1, 1, 0, 2, 1, 0, 1, 2, 0;
  Eigen::MatrixXi F(1, 3);
  F << 0, 1, 2;

  Eigen::SparseMatrix<double> A;
  vector_area_matrix(F, A);

  Eigen::VectorXd U(2 * V.rows());
  U << V.col(0), V.col(1);
  double area = U.transpose() * A * U;
  // Area of this triangle is 0.5
  EXPECT_NEAR(area, 0.5, 1e-6);
}

TEST(VectorAreaMatrixTest, CubeWithoutBottom) {
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

  Eigen::SparseMatrix<double> A;
  vector_area_matrix(F, A);

  ASSERT_EQ(A.rows(), 2 * V.rows());
  ASSERT_EQ(A.cols(), 2 * V.rows());
  Eigen::SparseMatrix<double> diff =
      A - Eigen::SparseMatrix<double>(A.transpose());

  ASSERT_NEAR(diff.norm(), 0.0, 1e-10);

  Eigen::VectorXd U(2 * V.rows());
  U << V.col(0), V.col(1);
  double area = U.transpose() * A * U;
  EXPECT_NEAR(area, 1.0, 1e-6);
}

TEST(VectorAreaMatrixTest, MeshWithHole) {
  Eigen::MatrixXd V(8, 3);
  V << 0, 0, 0, 3, 0, 0, 3, 3, 0, 0, 3, 0, 1, 1, 0, 2, 1, 0, 2, 2, 0, 1, 2, 0;

  Eigen::MatrixXi F(8, 3);
  F << 0, 1, 5, 0, 5, 4, 1, 2, 6, 1, 6, 5, 2, 3, 7, 2, 7, 6, 3, 0, 4, 3, 4, 7;

  Eigen::SparseMatrix<double> A;
  vector_area_matrix(F, A);

  ASSERT_EQ(A.rows(), 2 * V.rows());
  ASSERT_EQ(A.cols(), 2 * V.rows());

  // Symmetry
  Eigen::SparseMatrix<double> diff =
      A - Eigen::SparseMatrix<double>(A.transpose());
  ASSERT_NEAR(diff.norm(), 0.0, 1e-10);

  // Area: outer square area - inner square area = 9 - 1 = 8
  Eigen::VectorXd U(2 * V.rows());
  U << V.col(0), V.col(1);
  double area = U.transpose() * A * U;
  EXPECT_NEAR(area, 8.0, 1e-6);
}
