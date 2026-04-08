#include "lscm.h"
#include "vector_area_matrix.h"
#include <igl/cotmatrix.h>
#include <igl/eigs.h>
#include <igl/repdiag.h>

void align_by_SVD(Eigen::MatrixXd &U);

void lscm(const Eigen::MatrixXd &V, const Eigen::MatrixXi &F,
          Eigen::MatrixXd &U) {
  Eigen::SparseMatrix<double> M;
  igl::massmatrix(V, F, igl::MASSMATRIX_TYPE_DEFAULT, M);
  Eigen::SparseMatrix<double> L;
  igl::cotmatrix(V, F, L);

  Eigen::SparseMatrix<double> B;
  igl::repdiag(M, 2, B);

  Eigen::SparseMatrix<double> LL;
  igl::repdiag(L, 2, LL);
  Eigen::SparseMatrix<double> A;
  vector_area_matrix(F, A);
  Eigen::SparseMatrix<double> Q = -LL - 2.0 * A;

  Eigen::MatrixXd sol_U;
  Eigen::VectorXd sol_eigenvalues;

  bool eigs_success =
      igl::eigs(Q, B, 1, igl::EIGS_TYPE_SM, sol_U, sol_eigenvalues);
  assert(eigs_success && "igl::eigs failed to converge");
  std::cout << sol_eigenvalues << std::endl;

  U.resize(V.rows(), 2);
  U.col(0) = sol_U.col(0).head(V.rows());
  U.col(1) = sol_U.col(0).tail(V.rows());

  align_by_SVD(U);
}

void align_by_SVD(Eigen::MatrixXd &U) {
  // Align using singular value decomposition
  Eigen::VectorXd mean = U.colwise().mean();
  Eigen::MatrixXd U_centered = U.rowwise() - mean.transpose();
  // Covariance matrix (2x2)
  Eigen::Matrix2d cov = U_centered.transpose() * U_centered;
  // SVD of covariance
  Eigen::JacobiSVD<Eigen::Matrix2d> svd(cov, Eigen::ComputeFullU |
                                                 Eigen::ComputeFullV);
  Eigen::Matrix2d R = svd.matrixU();
  U = U_centered * R;
}
