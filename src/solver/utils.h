//
// Created by Zhengxiao Du on 2018-12-11.
//

#ifndef PGSOLVER_UTILS_H
#define PGSOLVER_UTILS_H

#include "../Elements.h"

double vector_dot(const double *vec1, const double *vec2, unsigned dim);

double norm(const SparseRow &vec);

double norm(const double *vec, unsigned dim);

double vector_dot(const SparseRow &vec1, const SparseRow &vec2);

void matrix_multiply(const std::vector<SparseRow> &mat, double * vec, double * r);

#endif //PGSOLVER_UTILS_H
