#include <iostream>
#include "Parser.h"
#include "solver/SolverFactory.h"
#include "CLI11.hpp"
#include <omp.h>


int main(int argc, char *argv[]) {
    std::ios_base::sync_with_stdio(false);
    CLI::App app{"PowerGrid Solver"};
    std::string input_filename, output_filename, method = "pcg";
    double omega = 1.5;
    int show_process = 0, timer = 0;
    app.add_option("input", input_filename, "The filename to get the input");
    app.add_option("output, -o, --output", output_filename, "The filename to output result");
    app.add_option("-m, --method", method,
                   "The method to solve the linear equations\nsupport: eigen, cg(conjugate gradient), pcg(preconditioned conjugate gradient), sor(successive over-relaxation), gauss, jacobi, gs(gauss-seidel)");
    app.add_option("-r, --ratio", omega, "The relaxation factor omega in sor");
    app.add_flag("-p, --process", show_process, "Show the computation process");
    app.add_flag("-t, --timer", timer, "Only show the time of computation");
    app.set_help_flag("-h, --help", "Show help");

    CLI11_PARSE(app, argc, argv);

    if (!input_filename.empty()) {
        if (output_filename.empty()) {
            output_filename = input_filename + ".output";
        }
        clock_t start, end;

        auto *solver = SolverFactory::create(method, omega, show_process > 0);
        PowerGrid::Parser parser(*solver);
        start = clock();
        parser.Parse_Input(input_filename);
        end = clock();
        if (timer > 0)
            std::cout << "Parse Input time = " << (double) (end - start) / CLOCKS_PER_SEC << std::endl;
        start = clock();
        parser.pg_solver.NodeMapping();
        end = clock();
        if (timer > 0)
            std::cout << "Node Mapping elapsed time = " << (double) (end - start) / CLOCKS_PER_SEC << std::endl;

        start = clock();
        parser.pg_solver.StampPowerPlane();
        parser.pg_solver.StampGNDPlane();
        end = clock();
        if (timer > 0)
            std::cout << "Node Stamp elapsed time = " << (double) (end - start) / CLOCKS_PER_SEC << std::endl;

        double start_time, end_time;
        start_time = omp_get_wtime();
        parser.pg_solver.SolvePowerPlane();
        parser.pg_solver.SolveGNDPlane();
        end_time = omp_get_wtime();
        if (timer > 0)
            std::cout << "Eigen Solver elapsed time = " << (end_time - start_time) << std::endl;
        parser.pg_solver.Output_DC_Solution(output_filename);
        //        start = clock();
//        parser.pg_solver.Cal_Total_Current_Power();
//        end = clock();
//        std::cout << "Current and power calculation time = " << (double) (end - start) / CLOCKS_PER_SEC << std::endl;
//        parser.pg_solver.Output_Element_Solution();
        delete solver;
    }


    return 0;
}