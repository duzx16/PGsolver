//
// Created by Zhengxiao Du on 2018/12/4.
//

#ifndef PGSOLVER_LINEARSOLVER_H
#define PGSOLVER_LINEARSOLVER_H

#include <vector>
#include <map>
#include "Solver.h"

class JacobiSolver : public Solver {
public:
    double *solve(std::vector<std::map<unsigned int, double> > &A, double *b) override;

    JacobiSolver(int iterate, double error, bool is_gs) : iterate_limit(iterate), error_limit(error), is_gs(is_gs) {}

private:
    int iterate_limit = 50000;
    double error_limit = 1e-10;
    bool is_gs;
};


#endif //PGSOLVER_LINEARSOLVER_H
