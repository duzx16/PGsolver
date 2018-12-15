//
// Created by Zhengxiao Du on 2018-12-09.
//

#ifndef PGSOLVER_SOLVERFACTORY_H
#define PGSOLVER_SOLVERFACTORY_H

#include "Solver.h"
#include <string>

class SolverFactory {
public:
    static Solver *create(std::string method, double param = 0.0, bool show_process = false);
};


#endif //PGSOLVER_SOLVERFACTORY_H
