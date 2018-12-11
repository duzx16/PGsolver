#ifndef PGSOLVER_SOLVER_H
#define PGSOLVER_SOLVER_H

#include <vector>
#include <map>
#include "../Elements.h"

class Solver {
public:
    virtual double *solve(std::vector<PowerGrid::SparseRow> &A, double *b) = 0;

    virtual ~Solver() = default;
};

#endif //PGSOLVER_SOLVER_H
