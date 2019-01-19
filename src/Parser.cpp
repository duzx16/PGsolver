//
// Created by Zhengxiao Du on 2018/11/25.
//

#include "Parser.h"
#include <cctype>
#include <stdlib.h>
#include <iostream>
#include <sstream>
#include <stdio.h>
#include <cctype>
#include <string>
#include <vector>
#include <map>
#include <sstream>

namespace PowerGrid {

    Parser::Parser(Solver &linearSolver) : pg_solver(linearSolver) {}

    void Parser::Parse_Line
            (const char *line, ssize_t linesz, std::vector<std::string> &line_element) {
        line_element.clear();
        int start = 0;
        for (unsigned i = 0; i < linesz; ++i) {
            if (isspace(line[i])) {
                if (i - start > 0) {
                    line_element.emplace_back(line + start, i - start);
                    start = i + 1;
                } else {
                    start = i + 1;
                }
            }
        }
        if (start < linesz) {
            line_element.emplace_back(line + start, linesz - start);
        }
    }

    void
    Parser::Parse_Input
            (const std::string &file_path) {
        FILE *ifile = fopen(file_path.c_str(), "r");

        if (ifile == nullptr) {
            std::cerr << "no such file under current directory !" << std::endl;
            exit(1);
        } else {
#ifndef NO_OUTPUT
            std::cout << "Open file success !" << std::endl;
#endif

            std::vector<std::string> line_elements;
            char *line = nullptr;
            size_t linesz = 0;
            while (getline(&line, &linesz, ifile) > 0) {

                Parse_Line(line, linesz, line_elements);

                if (!line_elements.empty())
                    switch (char( std::toupper(line_elements[0].at(0))))
                {
                    case 'V':
                        if (stod(line_elements[3]) != 0) {
                            // V != 0 两节点间存在电势差
                            volt = new PowerGrid::VoltageSource
                                    (line_elements[0], line_elements[1], line_elements[2], stod(line_elements[3]));

                            if (pg_solver.total_nodes[line_elements[1]] == nullptr) {
                                node = new PowerGrid::Node(line_elements[1]);
                                pg_solver.total_nodes[line_elements[1]] = node;
                            }

                            if (pg_solver.total_nodes[line_elements[2]] == nullptr) {
                                node = new PowerGrid::Node(line_elements[2]);
                                pg_solver.total_nodes[line_elements[2]] = node;
                            }

                            pg_solver.total_nodes[line_elements[1]]->Add_Vsource("pos", volt);
                            pg_solver.total_nodes[line_elements[2]]->Add_Vsource("neg", volt);
                            pg_solver.total_supply_Vsource[volt] = pg_solver.total_nodes[line_elements[1]];

                        } else if (stod(line_elements[3]) == 0) {
                            // V = 0 两节点短路
                            res = new PowerGrid::Resistor
                                    (line_elements[0], line_elements[1], line_elements[2], stod(line_elements[3]));

                            if (pg_solver.total_nodes[line_elements[1]] == nullptr) {
                                node = new PowerGrid::Node(line_elements[1]);
                                pg_solver.total_nodes[line_elements[1]] = node;
                            }

                            if (pg_solver.total_nodes[line_elements[2]] == nullptr) {
                                node = new PowerGrid::Node(line_elements[2]);
                                pg_solver.total_nodes[line_elements[2]] = node;
                            }
                            // 转化为电阻为0的边
                            pg_solver.total_nodes[line_elements[1]]->
                                    Add_Neighbor_Node(pg_solver.total_nodes[line_elements[2]], res);

                            pg_solver.total_nodes[line_elements[2]]->
                                    Add_Neighbor_Node(pg_solver.total_nodes[line_elements[1]], res);

                            res->Add_Node
                                    (pg_solver.total_nodes[line_elements[1]], pg_solver.total_nodes[line_elements[2]]);

                            pg_solver.total_shorted_Res.emplace_back(res);

                        }
                    break;
                    case 'I':

                        curr = new PowerGrid::CurrentSource
                                (line_elements[0], line_elements[1], line_elements[2], stod(line_elements[3]));

                    if (pg_solver.total_nodes[line_elements[1]] == nullptr) {
                        node = new PowerGrid::Node(line_elements[1]);
                        pg_solver.total_nodes[line_elements[1]] = node;
                    }


                    if (pg_solver.total_nodes[line_elements[2]] == nullptr) {
                        node = new PowerGrid::Node(line_elements[2]);
                        pg_solver.total_nodes[line_elements[2]] = node;
                    }

                    pg_solver.total_nodes[line_elements[1]]->Add_Isource("pos", curr);
                    pg_solver.total_nodes[line_elements[2]]->Add_Isource("neg", curr);
                    pg_solver.total_Isource.emplace_back(curr);

                    break;
                    case 'R':

                        res = new PowerGrid::Resistor
                                (line_elements[0], line_elements[1], line_elements[2], stod(line_elements[3]));

                    if (pg_solver.total_nodes[line_elements[1]] == nullptr) {
                        node = new PowerGrid::Node(line_elements[1]);
                        pg_solver.total_nodes[line_elements[1]] = node;
                    }


                    if (pg_solver.total_nodes[line_elements[2]] == nullptr) {
                        node = new PowerGrid::Node(line_elements[2]);
                        pg_solver.total_nodes[line_elements[2]] = node;
                    }

                    pg_solver.total_nodes[line_elements[1]]->
                            Add_Neighbor_Node(pg_solver.total_nodes[line_elements[2]], res);

                    pg_solver.total_nodes[line_elements[2]]->
                            Add_Neighbor_Node(pg_solver.total_nodes[line_elements[1]], res);

                    if (stod(line_elements[3]) < 1e-10) {

                        res->Add_Node
                                (pg_solver.total_nodes[line_elements[1]], pg_solver.total_nodes[line_elements[2]]);

                        pg_solver.total_shorted_Res.emplace_back(res);

                    } else {

                        res->Add_Node
                                (pg_solver.total_nodes[line_elements[1]], pg_solver.total_nodes[line_elements[2]]);

                        pg_solver.total_unshorted_Res.emplace_back(res);
                    }
                    break;
                    case '*' :
                        break;
                    case '.':
                        break;
                    default :
                        break;
                }
            }
#ifndef NO_OUTPUT
            std::cout << "Parse input file complete ! " << std::endl;
#endif
            free(line);
            fclose(ifile);
        }

    }


}
