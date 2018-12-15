//
// Created by Zhengxiao Du on 2018-12-11.
//

#ifndef PGSOLVER_CONJUGATEGRADIENT_H
#define PGSOLVER_CONJUGATEGRADIENT_H

#include "Solver.h"

class ConjugateGradient : public Solver {
public:
    explicit ConjugateGradient(bool preconditioned = true, bool show_process = false) : Solver(show_process), precontioned(preconditioned) {}

    double *solve(std::vector<SparseRow> &A, double *b) override;

protected:
    bool precontioned;
};


#endif //PGSOLVER_CONJUGATEGRADIENT_H
