#ifndef PGSOLVER_SOLVER_H
#define PGSOLVER_SOLVER_H

#include <vector>
#include <map>
#include "../Elements.h"

class Solver {
public:
    virtual double *solve(std::vector<SparseRow> &A, double *b) = 0;

    explicit Solver(bool show_process = false) : show_process(show_process) {}

    virtual ~Solver() = default;

protected:
    bool show_process;
};

#endif //PGSOLVER_SOLVER_H
