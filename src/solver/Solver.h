#ifndef PGSOLVER_SOLVER_H
#define PGSOLVER_SOLVER_H

#include <vector>
#include <map>

class Solver {
public:
    virtual double * solve(std::vector<std::map<unsigned int, double> > & A, double *b) = 0;
};

#endif //PGSOLVER_SOLVER_H
