#include "tutte.h"
#include <igl/boundary_loop.h>
#include <igl/cotmatrix.h>
#include <igl/map_vertices_to_circle.h>
#include <igl/slice_into.h>

void tutte(const Eigen::MatrixXd &V, const Eigen::MatrixXi &F,
           Eigen::MatrixXd &U) {
  Eigen::SparseMatrix<double> L;
  igl::cotmatrix(V, F, L);

  Eigen::VectorXi boundary;
  igl::boundary_loop(F, boundary);

  std::vector<bool> is_boundary(V.rows(), false);
  for (int b : boundary) {
    is_boundary[b] = true;
  }
  Eigen::VectorXi interior(V.rows() - boundary.size());
  int idx = 0;
  for (int i = 0; i < V.rows(); i++) {
    if (!is_boundary[i]) {
      interior(idx++) = i;
    }
  }

  U.resize(V.rows(), 2);

  Eigen::SparseMatrix<double> L_II, L_IB;
  igl::slice(L, interior, interior, L_II);
  igl::slice(L, interior, boundary, L_IB);

  Eigen::MatrixXd boundary_uv;
  igl::map_vertices_to_circle(V, boundary, boundary_uv);
  for (int i = 0; i < boundary.size(); i++) {
    U.row(boundary[i]) = boundary_uv.row(i);
  }

  Eigen::MatrixXd U_B;
  igl::slice(U, boundary, 1, U_B);
  Eigen::MatrixXd rhs = -L_IB * U_B;

  Eigen::SimplicialLDLT<Eigen::SparseMatrix<double>> solver;
  solver.compute(L_II);
  assert(solver.info() == Eigen::Success);
  Eigen::MatrixXd U_I = solver.solve(rhs);
  igl::slice_into(U_I, interior, 1, U);
  // igl::slice_into(Eigen::MatrixXd::Zero(interior.rows(), 2), interior, 1, U);
}
