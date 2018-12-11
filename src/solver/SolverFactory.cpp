//
// Created by Zhengxiao Du on 2018-12-09.
//

#include "SolverFactory.h"
#include "GaussSolver.h"
#include "EigenSolver.h"
#include "SORSolver.h"
#include <cctype>

Solver *SolverFactory::create(std::string method, double param) {
    std::for_each(method.begin(), method.end(), [](char & c){
        c = ::tolower(c);
    });
    if (method == "jacobi") {
        return new SORSolver(10000, 1e-8, false, 1.0);
    } else if (method == "gs") {
        return new SORSolver(10000, 1e-8, true, 1.0);
    } else if (method == "eigen") {
        return new EigenSolver();
    } else if (method == "gauss") {
        return new GaussSolver();
    } else if (method == "sor") {
        return new SORSolver(10000, 1e-8, true, param);
    }
    return nullptr;
}
