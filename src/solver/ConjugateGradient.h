//
// Created by Zhengxiao Du on 2018-12-11.
//

#ifndef PGSOLVER_CONJUGATEGRADIENT_H
#define PGSOLVER_CONJUGATEGRADIENT_H

#include "Solver.h"

class ConjugateGradient : public Solver {
public:
    double * solve(std::vector<SparseRow> & A, double *b) override;
};


#endif //PGSOLVER_CONJUGATEGRADIENT_H
