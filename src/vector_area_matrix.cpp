#include "vector_area_matrix.h"
#include <igl/boundary_loop.h>

typedef Eigen::Triplet<double> T;

void vector_area_matrix(const Eigen::MatrixXi &F,
                        Eigen::SparseMatrix<double> &A) {
  int V_size = F.maxCoeff() + 1;
  A.resize(V_size * 2, V_size * 2);

  std::vector<std::vector<int>> loops;
  igl::boundary_loop(F, loops);

  std::vector<T> triplets;
  for (auto &loop : loops) {
    for (int t = 0; t < loop.size(); ++t) {
      int i = loop[t];
      int j = loop[(t + 1) % loop.size()];
      triplets.push_back(T(i, V_size + j, 0.5));
      triplets.push_back(T(j, V_size + i, -0.5));
    }
  }

  A.setFromTriplets(triplets.begin(), triplets.end());
  Eigen::SparseMatrix<double> At = A.transpose();
  A = 0.5 * (A + At);
}
