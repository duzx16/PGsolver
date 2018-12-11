//
// Created by Zhengxiao Du on 2018/12/5.
//

#ifndef PGSOLVER_EIGENSOLVER_H
#define PGSOLVER_EIGENSOLVER_H

#include "Solver.h"

class EigenSolver: public Solver {
public:
    double * solve(std::vector<SparseRow> & A, double *b) override;
};


#endif //PGSOLVER_EIGENSOLVER_H
