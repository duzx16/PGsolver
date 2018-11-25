//
// Created by Zhengxiao Du on 2018/11/25.
//

#ifndef PGSOLVER_PGSOLVER_H
#define PGSOLVER_PGSOLVER_H

#include "Stamping.h"

namespace PowerGrid {

    class PG_Solver : public Stamping {

    public :

        PG_Solver();

        ~PG_Solver();

        void SolvePowerPlane();

        void SolveGNDPlane();

        virtual void solve();

        void Cal_Total_Current_Power();

        void MergeNodeMap(PowerGrid::Node *root, double volt);

        void Output_Element_Solution();

        void Output_DC_Solution(const std::string &file_name);

    };

}


#endif //PGSOLVER_PGSOLVER_H
