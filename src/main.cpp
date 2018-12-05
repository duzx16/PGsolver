#include <iostream>
#include "Parser.h"
#include "solver/EigenSolver.h"
#include "solver/JacobiSolver.h"

int main(int argc, char *argv[]) {

    if (argc == 3) {
        clock_t start, end;

        std::string input_file_name(argv[1]), output_file_name(argv[2]);
        auto *solver = new JacobiSolver(10000, 1e-8, true);
        PowerGrid::Parser parser(*solver);
        start = clock();
        parser.Parse_Input(input_file_name);
        parser.pg_solver.NodeMapping();
        end = clock();
        std::cout << "Node Mapping elapsed time = " << (double) (end - start) / CLOCKS_PER_SEC << std::endl;

        parser.pg_solver.StampPowerPlane();
        parser.pg_solver.StampGNDPlane();

        start = clock();
        parser.pg_solver.SolvePowerPlane();
        parser.pg_solver.SolveGNDPlane();
        end = clock();
        std::cout << "Eigen Solver elapsed time = " << (double) (end - start) / CLOCKS_PER_SEC << std::endl;
        parser.pg_solver.Output_DC_Solution(output_file_name);

        start = clock();
        parser.pg_solver.Cal_Total_Current_Power();
        end = clock();
        std::cout << "Current and power calculation time = " << (double) (end - start) / CLOCKS_PER_SEC << std::endl;
        parser.pg_solver.Output_Element_Solution();

        delete solver;
    } else
        std::cerr << "invalid input arguments !" << std::endl;

    return 0;
}