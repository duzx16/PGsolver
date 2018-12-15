//
// Created by Zhengxiao Du on 2018/11/25.
//

#include "Stamping.h"
#include <iostream>
#include <fstream>
#include <algorithm>
#include <list>

namespace PowerGrid {

    Stamping::Stamping() = default;

    void Stamping::StampPowerPlane() {
        // 生成需要求解的矩阵
        matrix_dim = static_cast<unsigned int>(power_stamp_NodeList.size());
#ifndef NO_OUTPUT
        std::cout << "Total power plane matrix dimension = " << matrix_dim << std::endl;
#endif
        MNA.resize(matrix_dim);
        source_vec = new double[matrix_dim]();
#ifndef NO_OUTPUT
        std::cout << "Stampping ..." << std::endl;
#endif

        for (auto &stamp_node : power_stamp_NodeList) {
            for (auto &nei_node : stamp_node.first->neighbor_NodeRes) {
                if (nei_node.first->is_grounded) {
                    for (auto &each_res : nei_node.second) {
                        if (each_res->res_value != 0)
                            MNA[stamp_node.second][stamp_node.second] += 1 / each_res->res_value;
                    }
                } else if (nei_node.first->is_connect_V && !nei_node.first->is_grounded) {
                    for (auto &each_res : nei_node.second) {
                        if (each_res->res_value != 0) {
                            MNA[stamp_node.second][stamp_node.second] += 1 / each_res->res_value;
                            source_vec[stamp_node.second]
                                    += nei_node.first->node_voltage / each_res->res_value;
                        }
                    }
                } else if (!nei_node.first->is_connect_V && !nei_node.first->is_grounded &&
                           nei_node.first->is_stamp) {
                    for (auto &each_res : nei_node.second) {
                        if (each_res->res_value != 0) {
                            MNA[stamp_node.second][stamp_node.second] += 1 / each_res->res_value;
                            MNA[stamp_node.second][power_stamp_NodeList[nei_node.first]]
                                    += -1 / each_res->res_value;
                        }
                    }
                }

            }
            for (auto &curr : stamp_node.first->connected_Isource) {
                if (curr.first == "pos") {
                    for (auto &each_curr : curr.second)
                        source_vec[stamp_node.second] += -each_curr->curr_value;
                } else if (curr.first == "neg") {
                    for (auto &each_curr : curr.second)
                        source_vec[stamp_node.second] += +each_curr->curr_value;
                }
            }
        }
#ifndef NO_OUTPUT
        std::cout << "Stammping Power Plane Complete !" << std::endl;
#endif

    }


    void Stamping::StampGNDPlane() {
        // 生成需要求解的矩阵
        matrix_dim_gnd = static_cast<unsigned int>(gnd_stamp_NodeList.size());
#ifndef NO_OUTPUT
        std::cout << "Total gnd plane matrix dimension = " << matrix_dim_gnd << std::endl;
#endif
        MNA_gnd.resize(matrix_dim_gnd);
        source_vec_gnd = new double[matrix_dim_gnd]();
#ifndef NO_OUTPUT
        std::cout << "Stampping ..." << std::endl;
#endif


        for (auto &stamp_node : gnd_stamp_NodeList) {
            for (auto &nei_node : stamp_node.first->neighbor_NodeRes) {
                if (nei_node.first->is_grounded) {
                    for (auto &each_res : nei_node.second) {
                        if (each_res->res_value != 0)
                            MNA_gnd[stamp_node.second][stamp_node.second] += 1 / each_res->res_value;
                    }
                } else if (nei_node.first->is_connect_V && !nei_node.first->is_grounded) {
                    for (auto &each_res : nei_node.second) {
                        if (each_res->res_value != 0) {
                            MNA_gnd[stamp_node.second][stamp_node.second] += 1 / each_res->res_value;
                            source_vec_gnd[stamp_node.second]
                                    += nei_node.first->node_voltage / each_res->res_value;
                        }
                    }
                } else if (!nei_node.first->is_connect_V && !nei_node.first->is_grounded &&
                           nei_node.first->is_stamp) {
                    for (auto &each_res : nei_node.second) {
                        if (each_res->res_value != 0) {
                            MNA_gnd[stamp_node.second][stamp_node.second] += 1 / each_res->res_value;
                            MNA_gnd[stamp_node.second][gnd_stamp_NodeList[nei_node.first]]
                                    += -1 / each_res->res_value;
                        }
                    }
                }

            }
            for (auto &curr : stamp_node.first->connected_Isource) {
                if (curr.first == "pos") {
                    for (auto &each_curr : curr.second)
                        source_vec_gnd[stamp_node.second] += -each_curr->curr_value;
                } else if (curr.first == "neg") {
                    for (auto &each_curr : curr.second)
                        source_vec_gnd[stamp_node.second] += +each_curr->curr_value;
                }
            }
        }
#ifndef NO_OUTPUT
        std::cout << "Stammping GND Plane Complete !" << std::endl;
#endif

    }
}

