//
// Created by Zhengxiao Du on 2018/11/25.
//

#ifndef PGSOLVER_PGSOLVER_H
#define PGSOLVER_PGSOLVER_H

#include "Stamping.h"
#include "solver/Solver.h"

namespace PowerGrid {

    class PG_Solver : public Stamping {

    public :

        explicit PG_Solver(Solver &linearSolver);

        ~PG_Solver() = default;

        void SolvePowerPlane();

        void SolveGNDPlane();

        void Cal_Total_Current_Power();

        void MergeNodeMap(PowerGrid::Node *root, double volt);

        void Output_Element_Solution();

        void Output_DC_Solution(const std::string &file_name);

    private:
        Solver &linearSolver;

    };


}


#endif //PGSOLVER_PGSOLVER_H
