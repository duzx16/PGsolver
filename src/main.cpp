#include <iostream>
#include "Parser.h"
#include "solver/SolverFactory.h"
#include <omp.h>


int main(int argc, char *argv[]) {
    std::ios_base::sync_with_stdio(false);
    if (argc == 3) {
        clock_t start, end;

        std::string input_file_name(argv[1]), output_file_name(argv[2]);
        auto * solver = SolverFactory::create("cg", 1.5);
        PowerGrid::Parser parser(*solver);
        start = clock();
        parser.Parse_Input(input_file_name);
        end = clock();
        std::cout << "Parse Input time = " << (double) (end - start) / CLOCKS_PER_SEC << std::endl;
        start = clock();
        parser.pg_solver.NodeMapping();
        end = clock();
        std::cout << "Node Mapping elapsed time = " << (double) (end - start) / CLOCKS_PER_SEC << std::endl;

        start = clock();
        parser.pg_solver.StampPowerPlane();
        parser.pg_solver.StampGNDPlane();
        end = clock();
        std::cout << "Node Stamp elapsed time = " << (double) (end - start) / CLOCKS_PER_SEC << std::endl;

        double start_time, end_time;
        start_time = omp_get_wtime();
        parser.pg_solver.SolvePowerPlane();
        parser.pg_solver.SolveGNDPlane();
        end_time = omp_get_wtime();
        std::cout << "Eigen Solver elapsed time = " << (end_time - start_time) << std::endl;
        parser.pg_solver.Output_DC_Solution(output_file_name);

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