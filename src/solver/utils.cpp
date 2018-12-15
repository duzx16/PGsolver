//
// Created by Zhengxiao Du on 2018-12-11.
//
#include "utils.h"
#include <omp.h>

double vector_dot(const double *vec1, const double *vec2, unsigned dim) {
    double r = 0.0;
    for (int i = 0; i < dim; ++i) {
        r += vec1[i] * vec2[i];
    }
    return r;
}

double vector_dot(const SparseRow &vec1, const SparseRow &vec2) {
    auto first = vec1.begin();
    double r = 0.0;
    for (const auto &iter: vec2) {
        while (first != vec1.end() && first->first < iter.first) {
            first++;
        }
        if (first != vec1.end() && first->first == iter.first) {
            r += iter.second * first->second;
        }
    }
    return 0;
}

double norm(const SparseRow &vec) {
    double r = 0.0;
    for (const auto &it: vec) {
        r += it.second * it.second;
    }
    return r;
}

void matrix_multiply(const std::vector<SparseRow> &mat, double *vec, double * r) {
//#pragma omp_set_num_threads(4)
//#pragma omp parallel for schedule(dynamic, 1)
#pragma omp parallel
#pragma omp for
    for (int i = 0; i < mat.size(); ++i) {
        r[i] = 0.0;
        for (const auto &ele: mat[i]) {
            r[i] += ele.second * vec[ele.first];
        }
    }
}

double norm(const double *vec, unsigned dim) {
    double r = 0.0;
    for (int i = 0; i < dim; ++i) {
        r += vec[i] * vec[i];
    }
    return r;
}
