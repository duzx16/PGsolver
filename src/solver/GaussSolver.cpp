//
// Created by Zhengxiao Du on 2018-12-08.
//

#include "GaussSolver.h"
#include <cmath>
#include <algorithm>

double *GaussSolver::solve(std::vector<PowerGrid::SparseRow> &A, double *b) {
    auto matrix_rank = A.size();
    auto answer = new double[matrix_rank];
    for (unsigned current = 0; current < matrix_rank; ++current) {
//        printf("begin %d %lu\n", current, A[current].size());
        int max_row = -1;
        double max_ele = 0.0;
        for (unsigned j = current; j < matrix_rank; ++j) {
            auto it = A[j].begin();
//            if (it != A[j].end() && it->first <= current) {
//                printf("%d %d\n", j, it->first);
//            }
            if (it != A[j].end() && it->first == current) {
                if (fabs(it->second) > max_ele) {
                    max_row = j;
                    max_ele = fabs(it->second);
                }
            }
        }
        if (max_ele != 0.0) {
            if (max_row != current) {
                std::iter_swap(A.begin() + current, A.begin() + max_row);
                std::swap(b[current], b[max_row]);
            }
            max_ele = A[current][current];
            for (int i = current + 1; i < matrix_rank; ++i) {
                auto first = A[i].begin();
                if (first != A[i].end() && first->first == current && first->second != 0.0) {
                    double m = first->second / max_ele;
                    first = A[i].erase(first);
                    for (const auto &iter: A[current]) {
                        if (iter.first == current)
                            continue;
                        while (first != A[i].end() && first->first < iter.first) {
                            first++;
                        }
                        if (first != A[i].end() && first->first == iter.first) {
                            first->second -= m * iter.second;
                        } else {
                            first = A[i].insert(first, {iter.first, -m * iter.second});
                        }
                        first++;
                    }
                    b[i] -= m * b[current];
                }
            }
        } else {
            fprintf(stderr, "The matrix is singular\n");
            exit(1);
        }
    }
    for (int k = matrix_rank - 1; k >= 0; --k) {
        answer[k] = b[k];
        double main_ele = 0.0;
        for (const auto &it: A[k]) {
            if (it.first != k) {
                answer[k] -= it.second * answer[it.first];
            } else {
                main_ele = it.second;
            }
        }
        answer[k] /= main_ele;
//        printf("%lf\n", answer[k]);
    }
    return answer;
}
