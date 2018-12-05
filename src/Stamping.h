//
// Created by Zhengxiao Du on 2018/11/25.
//

#ifndef PGSOLVER_STAMPING_H
#define PGSOLVER_STAMPING_H

#include "Elements.h"

namespace PowerGrid {

    class Stamping : public PowerGrid::Elements {

    public:

        Stamping();

        void StampPowerPlane();

        void StampGNDPlane();

    protected:

        unsigned int matrix_dim, matrix_dim_gnd;

        std::vector<std::map<unsigned int, double> >  MNA, MNA_gnd;

        double *source_vec, *source_vec_gnd;

    };
}

#endif //PGSOLVER_STAMPING_H
