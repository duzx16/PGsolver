//
// Created by Zhengxiao Du on 2018/11/25.
//

#include "Parser.h"
#include <cctype>
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <map>

namespace PowerGrid {

    Parser::Parser() : pg_solver() {}

    Parser::~Parser() = default;

    void Parser::Parse_Line
            (const std::string &line, const std::string &seperators, std::vector<std::string> &line_element) {
        line_element.clear();
        std::string token;
        for (char i : line) {
            if (seperators.find(i) != std::string::npos) {
                if (!token.empty()) {
                    line_element.push_back(token);
                    token.clear();
                }
            } else token.push_back(i);
        }
        if (!token.empty()) {
            line_element.push_back(token);
            token.clear();
        }

    }

    void
    Parser::Parse_Input
            (const std::string &file_path) {
        std::ifstream in(file_path.c_str(), std::ios::in);

        if (!in.is_open()) {

            std::cerr << "no such file under current directory !" << std::endl;
            in.close();
            exit(1);

        } else {

            std::cout << "Open file success !" << std::endl;

            std::vector<std::string> line_elements;
            std::string line;
            while (std::getline(in, line)) {

                Parse_Line(line, "	 \n", line_elements);

                if (!line_elements.empty())
                    switch (char( std::toupper(line_elements[0].at(0))))
                {
                    case 'V': {
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
                    }
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

                    if (stod(line_elements[3]) == 0) {

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

            std::cout << "Parse input file complete ! " << std::endl;
            in.close();
        }

    }


}
