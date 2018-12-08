//
// Created by Zhengxiao Du on 2018/12/4.
//

#include "SORSolver.h"
#include <cmath>

double norm2(const double *x1, const double *x2, unsigned dim) {
    double sum = 0;
    for (int i = 0; i < dim; ++i) {
        sum += (x1[i] - x2[i]) * (x1[i] - x2[i]);
    }
    return sqrt(sum / dim);
}

double *SORSolver::solve(std::vector<std::map<unsigned int, double> > &A, double *b) {
    unsigned long matrix_rank = A.size();
    // 使用Jacobi迭代或者GS迭代
    // 将矩阵的对角元归一化
    for (int i = 0; i < matrix_rank; ++i) {
        double main_ele = 0;
        for (auto &it: A[i]) {
            if (it.first == i) {
                main_ele = it.second;
            }
        }
        if (main_ele != 0.0) {
            for (auto &it: A[i]) {
                it.second /= main_ele;
            }
            b[i] /= main_ele;
        } else {
            fprintf(stderr, "The main element at row %d is zero\n", i);
            exit(1);
        }
    }
    // allocate the space for the answer
    auto *answer = new double[matrix_rank];
    auto *new_answer = new double[matrix_rank];
    for (int i = 0; i < matrix_rank; ++i) {
        answer[i] = 0.0;
        new_answer[i] = 0.0;
    }
    for (int num = 0; num < iterate_limit; ++num) {
        for (int i = 0; i < matrix_rank; ++i) {
            double x_i = 0.0;
            for (const auto &it: A[i]) {
                if (it.first < i && is_gs) {
                    x_i -= it.second * new_answer[it.first];
                } else {
                    x_i -= it.second * answer[it.first];
                }
            }
            x_i += b[i];
            x_i = x_i * omega + answer[i];
            new_answer[i] = x_i;
        }
        double error = norm2(answer, new_answer, matrix_rank);
        printf("%le\n", error);
        if (error < error_limit) {
            break;
        }
        std::swap(new_answer, answer);
    }
    return answer;
}
