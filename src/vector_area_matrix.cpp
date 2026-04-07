#include "vector_area_matrix.h"
#include <igl/boundary_loop.h>

typedef Eigen::Triplet<double> T;

void vector_area_matrix(const Eigen::MatrixXi &F,
                        Eigen::SparseMatrix<double> &A) {
  int V_size = F.maxCoeff() + 1;
  A.resize(V_size * 2, V_size * 2);

  Eigen::VectorXi boundary;
  igl::boundary_loop(F, boundary);

  std::vector<T> triplets;
  triplets.reserve(2 * boundary.rows());

  for (int t = 0; t < boundary.rows() - 1; ++t) {
    int i = boundary(t);
    int j = boundary(t + 1);
    triplets.push_back(T(i, V_size + j, 0.5));
    triplets.push_back(T(j, V_size + i, -0.5));
  }
  int i = boundary(boundary.rows() - 1);
  int j = boundary(0);
  triplets.push_back(T(i, V_size + j, 0.5));
  triplets.push_back(T(j, V_size + i, -0.5));

  A.setFromTriplets(triplets.begin(), triplets.end());
  // Make symmetric
  Eigen::SparseMatrix<double> At = A.transpose();
  A = 0.5 * (A + At);
}
