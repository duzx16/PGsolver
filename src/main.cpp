#include <iostream>
#include "Parser.h"
#include "solver/EigenSolver.h"
#include "solver/SORSolver.h"
#include "solver/EigenSolver.h"

int main(int argc, char *argv[]) {
    std::ios_base::sync_with_stdio(false);
    if (argc == 3) {
        clock_t start, end;

        std::string input_file_name(argv[1]), output_file_name(argv[2]);
//        // Jacobi迭代
//        auto *solver = new SORSolver(10000, 1e-8, false, 1.0);
//        // GS迭代
//        auto *solver = new SORSolver(10000, 1e-8, true, 1.0);
        // 超松弛迭代
//        auto *solver = new SORSolver(50000, 1e-10, true, 1.5);
        auto *solver = new EigenSolver();
        PowerGrid::Parser parser(*solver);
        start = clock();
        parser.Parse_Input(input_file_name);
        end = clock();
        std::cout << "Parse Input time = " << (double) (end - start) / CLOCKS_PER_SEC << std::endl;
        start = clock();
        parser.pg_solver.NodeMapping();
        end = clock();
        std::cout << "Node Mapping elapsed time = " << (double) (end - start) / CLOCKS_PER_SEC << std::endl;

//        start = clock();
//        parser.pg_solver.StampPowerPlane();
//        parser.pg_solver.StampGNDPlane();
//        end = clock();
//        std::cout << "Node Stamp elapsed time = " << (double) (end - start) / CLOCKS_PER_SEC << std::endl;
//
//        start = clock();
//        parser.pg_solver.SolvePowerPlane();
//        parser.pg_solver.SolveGNDPlane();
//        end = clock();
//        std::cout << "Eigen Solver elapsed time = " << (double) (end - start) / CLOCKS_PER_SEC << std::endl;
//        parser.pg_solver.Output_DC_Solution(output_file_name);
//
//        start = clock();
//        parser.pg_solver.Cal_Total_Current_Power();
//        end = clock();
//        std::cout << "Current and power calculation time = " << (double) (end - start) / CLOCKS_PER_SEC << std::endl;
//        parser.pg_solver.Output_Element_Solution();

        delete solver;
    } else
        std::cerr << "invalid input arguments !" << std::endl;
    return 0;
}