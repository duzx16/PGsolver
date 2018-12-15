//
// Created by Zhengxiao Du on 2018-12-11.
//

#include "ConjugateGradient.h"
#include <stdio.h>
#include "utils.h"

double *ConjugateGradient::solve(std::vector<SparseRow> &A, double *b) {
    unsigned matrix_rank = A.size();
    auto Mi = new double[matrix_rank];
    for (int i = 0; i < matrix_rank; ++i) {
        if (precontioned) {
            Mi[i] = 0.0;
            for (const auto &ele: A[i]) {
                if (ele.first == i) {
                    Mi[i] = 1 / ele.second;
                }
            }
            if (Mi[i] == 0.0) {
                fprintf(stderr, "The matrix can't be solved\n");
            }
        } else {
            Mi[i] = 1.0;
        }

    }
    auto x_k = new double[matrix_rank];
    for (int i = 0; i < matrix_rank; ++i) {
        x_k[i] = 0.0;
    }
    auto Ax = new double[matrix_rank];
    matrix_multiply(A, x_k, Ax);
    auto r_k = new double[matrix_rank];
    auto p_k = new double[matrix_rank];
    for (int i = 0; i < matrix_rank; ++i) {
        r_k[i] = b[i] - Ax[i];
    }
    delete[]Ax;
    auto z_k = new double[matrix_rank];
    for (int i = 0; i < matrix_rank; ++i) {
        p_k[i] = z_k[i] = r_k[i] * Mi[i];
    }
    auto Ap = new double[matrix_rank];
    double alpha, beta, r_k2 = vector_dot(r_k, z_k, matrix_rank);
    int iter_num = 0;
    while (true) {
        iter_num += 1;
        if (show_process)
            printf("%le\n", r_k2);
        matrix_multiply(A, p_k, Ap);
        double pAp = vector_dot(p_k, Ap, matrix_rank);
        if (pAp < 1e-12) {
            break;
        }
        alpha = r_k2 / pAp;
        for (int i = 0; i < matrix_rank; ++i) {
            x_k[i] += alpha * p_k[i];
        }
        for (int i = 0; i < matrix_rank; ++i) {
            r_k[i] -= alpha * Ap[i];
        }
        if (norm(r_k, matrix_rank) < 1e-12) {
            break;
        }
        double old_rk2 = r_k2;
        for (int i = 0; i < matrix_rank; ++i) {
            z_k[i] = r_k[i] * Mi[i];
        }
        r_k2 = vector_dot(r_k, z_k, matrix_rank);
        beta = r_k2 / old_rk2;
        for (int i = 0; i < matrix_rank; ++i) {
            p_k[i] = p_k[i] * beta + z_k[i];
        }
    }
#ifndef NO_OUTPUT
    printf("Iter num:%d\n", iter_num);
#endif
    return x_k;
}
