//
// Created by Zhengxiao Du on 2018-12-08.
//

#ifndef PGSOLVER_GAUSSSOLVER_H
#define PGSOLVER_GAUSSSOLVER_H

#include "Solver.h"

class GaussSolver: public Solver {
public:
    GaussSolver() = default;
    double * solve(std::vector<PowerGrid::SparseRow> & A, double *b) override;

};


#endif //PGSOLVER_GAUSSSOLVER_H
