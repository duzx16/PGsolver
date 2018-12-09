#include <iostream>
#include "Parser.h"
#include "solver/EigenSolver.h"
#include "solver/SORSolver.h"
#include "solver/EigenSolver.h"
#include "solver/GaussSolver.h"

//int main(int argc, char *argv[]) {
//    auto *solver = new GaussSolver();
//    std::vector<PowerGrid::SparseRow> A;
//    A.resize(3);
//    A[0].push_back({0, 0.001});
//    A[0].push_back({1, 2.000});
//    A[0].push_back({2, 3.000});
//    A[1].push_back({0, -1.000});
//    A[1].push_back({1, 3.712});
//    A[1].push_back({2, 4.623});
//    A[2].push_back({0, -2.0});
//    A[2].push_back({1, 1.072});
//    A[2].push_back({2, 5.643});
//    double b[3] = {1, 2, 3};
//    auto answer = solver->solve(A, b);
//    for (int i = 0; i < 3; ++i) {
//        printf("%lf\n", answer[i]);
//    }
//}


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
        auto *solver = new SORSolver(50000, 1e-10, true, 1.5);
        // 高斯消元
//        auto *solver = new GaussSolver();
//        auto *solver = new EigenSolver();
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