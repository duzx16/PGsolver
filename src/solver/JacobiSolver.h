//
// Created by Zhengxiao Du on 2018/12/4.
//

#ifndef PGSOLVER_LINEARSOLVER_H
#define PGSOLVER_LINEARSOLVER_H

#include <vector>
#include <map>
#include "Solver.h"

class JacobiSolver: public Solver {
public:
    double * solve(std::vector<std::map<unsigned int, double> > & A, double *b) override;
private:
    int iterate_limit = 50000;
    double error_limit = 1e-10;
};


#endif //PGSOLVER_LINEARSOLVER_H
