//
// Created by Zhengxiao Du on 2018/11/25.
//

#include "PGSolver.h"
#include <iomanip>
#include <fstream>
#include <iostream>
#include <math.h>

namespace PowerGrid {

    void PG_Solver::MergeNodeMap(PowerGrid::Node *root, double volt) {
        for (auto &each_shorted_node : shorted_NodeMap[root]) {
            each_shorted_node->node_voltage = volt;
            MergeNodeMap(each_shorted_node, volt);
        }
    }

    void PG_Solver::Output_DC_Solution(const std::string &file_name) {

        std::fstream fout(file_name.c_str(), std::ios::out);

        for (auto &node : PowerPlaneNodes) {
            if (node.first != "0")
                fout << node.first << " " << node.second->node_voltage << std::endl;
            else
                fout << "G" << " " << node.second->node_voltage << std::endl;
        }


        fout << std::endl << std::endl;
        std::cout << "Output solution complete !" << std::endl;
        fout.close();

    }


    void PG_Solver::Cal_Total_Current_Power() {

        std::cout << "Calculating current and power dissipation ..." << std::endl;

        for (auto &supply_node : total_supply_Vsource) {
            double total_current = 0;
            for (auto &neighbor : supply_node.second->neighbor_NodeRes) {
                if (neighbor.first->is_stamp)
                    for (auto &nei_res : neighbor.second) {
                        total_current +=
                                (supply_node.first->volt_value - neighbor.first->node_voltage) / nei_res->res_value;
                    }
            }
            supply_node.first->curr_value = -1 * total_current;
            supply_node.first->power_value = supply_node.first->volt_value * fabs(supply_node.first->curr_value);
        }


        for (auto &each_res : total_unshorted_Res) {
            each_res->Vdrop = each_res->shorted_node.first->node_voltage - each_res->shorted_node.second->node_voltage;
            each_res->curr_value = each_res->Vdrop / each_res->res_value;
            each_res->power_value = pow(each_res->curr_value, 2) * each_res->res_value;
        }


        for (auto &via : total_shorted_Res) {
            double total_outward_current = 0;
            for (auto &node : via->neighbor_UnshortedNodes) {
                for (auto &each_nei : node.second) {
                    total_outward_current +=
                            (node.first->node_voltage - each_nei.first->node_voltage) / each_nei.second->res_value;
                }
            }
            via->curr_value = -1 * (total_outward_current + via->current_source_recorder);
        }

        std::cout << "Current and power dissipation Calculation complete !" << std::endl;

    }

    void PG_Solver::Output_Element_Solution() {

        std::fstream fout("List", std::ios::out);

        fout << "#--------------------------- Voltage Source ---------------------------" << std::endl;
        for (auto &volt_source : total_supply_Vsource) {
            fout << volt_source.first->volt_name
                 << std::setw(20)
                 << "voltage value = " << volt_source.first->volt_value << "V"
                 << std::setw(20)
                 << "current = " << volt_source.first->curr_value << "A"
                 << std::setw(20)
                 << "power = " << volt_source.first->power_value << "W"
                 << std::endl;
        }
        fout << "#----------------------------------------------------------------------" << std::endl << std::endl;

        fout << "#--------------------------- Shorted Resistors ---------------------------" << std::endl;
        for (auto &volt_source : total_shorted_Res) {
            fout << volt_source->res_name
                 << std::setw(20)
                 << "resistor value = " << volt_source->res_value << "ohm"
                 << std::setw(20)
                 << "current = " << volt_source->curr_value << "A"
                 << std::endl;
        }
        fout << "#----------------------------------------------------------------------" << std::endl << std::endl;

        fout << "#--------------------------- UnShorted Resistors ---------------------------" << std::endl;
        for (auto &each_res : total_unshorted_Res) {
            fout << each_res->res_name
                 << std::setw(20)
                 << "resistor value = " << each_res->res_value << "ohm"
                 << std::setw(20)
                 << "Vdrop = " << each_res->Vdrop << "V"
                 << std::setw(20)
                 << "current = " << each_res->curr_value << "A"
                 << std::setw(20)
                 << "power = " << each_res->power_value << "W" << std::endl;
        }
        fout << "#----------------------------------------------------------------------" << std::endl;

        std::cout << "Output element list file complete !" << std::endl;

    }
}
