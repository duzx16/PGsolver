//
// Created by Zhengxiao Du on 2018/12/5.
//

#include "EigenSolver.h"

#include <iostream>
#include "Eigen/Eigen"
#include "Eigen/SparseCholesky"
#include "Eigen/SparseLU"

typedef Eigen::Triplet<double> triplet;

double *EigenSolver::solve(std::vector<std::map<unsigned int, double> > &A, double *b) {
    unsigned long matrix_dim = A.size();
    std::vector<triplet> tripletList;
    Eigen::SparseMatrix<double> Mat(matrix_dim, matrix_dim);
    Eigen::VectorXd b_vec(matrix_dim), x;


    for (unsigned int row_idx = 0; row_idx < A.size(); ++row_idx) {
        for (const auto &row_data : A[row_idx]) {
            tripletList.emplace_back(row_idx, row_data.first, row_data.second);
        }
    }
    Mat.setFromTriplets(tripletList.begin(), tripletList.end());


    for (unsigned int i = 0; i < matrix_dim; ++i) {
        b_vec[i] = b[i];
    }

    Eigen::SimplicialLLT<Eigen::SparseMatrix<double>> solver;
    solver.compute(Mat);
    if (solver.info() != Eigen::Success) {
        std::cerr << "Matrix decomposition failed !" << std::endl;
        return nullptr;
    }
    x = solver.solve(b_vec);
    if (solver.info() != Eigen::Success) {
        std::cerr << "Solving failed !" << std::endl;
        return nullptr;
    }
    auto *answer = new double[matrix_dim];
    for (int j = 0; j < matrix_dim; ++j) {
        answer[j] = x[j];
    }
    return answer;
}
