//
// Created by Zhengxiao Du on 2018/11/25.
//

#ifndef PGSOLVER_PARSER_H
#define PGSOLVER_PARSER_H

#include "Elements.h"
#include "PGSolver.h"
#include "solver/Solver.h"

namespace PowerGrid {

    class Parser
    {

    public :

        explicit Parser(Solver & linearSolver);

        ~Parser() = default;

        void Parse_Input(const std::string &file_path);

        PG_Solver pg_solver;

    private :

        void Parse_Line
                (const std::string &line, const std::string &seperators, std::vector<std::string> &line_element);

        PowerGrid::Resistor *res;

        PowerGrid::VoltageSource *volt;

        PowerGrid::CurrentSource *curr;

        PowerGrid::Node *node;

    };
}


#endif //PGSOLVER_PARSER_H
