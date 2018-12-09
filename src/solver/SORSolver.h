//
// Created by Zhengxiao Du on 2018/12/4.
//

#ifndef PGSOLVER_LINEARSOLVER_H
#define PGSOLVER_LINEARSOLVER_H

#include <vector>
#include <map>
#include "Solver.h"

class SORSolver : public Solver {
public:
    double *solve(std::vector<PowerGrid::SparseRow> &A, double *b) override;

    SORSolver(int iterate, double error, bool is_gs, double omega = 1.0) : iterate_limit(iterate), error_limit(error),
                                                                           is_gs(is_gs), omega(omega) {
        if (!is_gs) {
            this->omega = 1.0;
        }
    }

private:
    int iterate_limit = 50000;
    double error_limit = 1e-10;
    bool is_gs;
    double omega;
};


#endif //PGSOLVER_LINEARSOLVER_H
