//
// Created by Zhengxiao Du on 2018-12-09.
//

#include "SolverFactory.h"
#include "GaussSolver.h"
#include "EigenSolver.h"
#include "SORSolver.h"
#include "ConjugateGradient.h"
#include <cctype>

Solver *SolverFactory::create(std::string method, double param, bool show_process) {
    std::for_each(method.begin(), method.end(), [](char &c) {
        c = ::tolower(c);
    });
    if (method == "jacobi") {
        return new SORSolver(10000, 1e-10, false, 1.0, show_process);
    } else if (method == "gs") {
        return new SORSolver(10000, 1e-10, true, 1.0, show_process);
    } else if (method == "eigen") {
        return new EigenSolver();
    } else if (method == "gauss") {
        return new GaussSolver(show_process);
    } else if (method == "sor") {
        return new SORSolver(10000, 1e-10, true, param, show_process);
    } else if (method == "cg") {
        return new ConjugateGradient(false, show_process);
    } else if (method == "pcg") {
        return new ConjugateGradient(true, show_process);
    }
    return nullptr;
}
