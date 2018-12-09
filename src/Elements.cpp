#include <utility>

//
// Created by Zhengxiao Du on 2018/11/25.
//

#include "Elements.h"
#include <iostream>
#include <fstream>
#include <string>

namespace PowerGrid {

    Resistor::Resistor() = default;

    Resistor::~Resistor() = default;

    Resistor::Resistor(std::string n, std::string pos, std::string neg, double value)
            : res_name(std::move(n)), pos_node(std::move(pos)), neg_node(std::move(neg)), res_value(value), is_merged(false),
              current_source_recorder(0) {}

    void Resistor::Add_Node(Node *n1, Node *n2) {
        shorted_node = std::make_pair(n1, n2);
    }


    void Resistor::
    Construct_Neighbor_UnshortedNodes
            (Node *root_node, Node *next_node, double *current_recorder, std::map<std::string, bool> &visit) {
        visit[next_node->node_name] = true;

        for (auto &curr_source : next_node->connected_Isource) {
            if (curr_source.first == "pos") {
                for (auto &each_curr : curr_source.second) {
                    *current_recorder += each_curr->curr_value;
                }
            } else if (curr_source.first == "neg") {
                for (auto &each_curr : curr_source.second) {
                    *current_recorder -= each_curr->curr_value;
                }
            }
        }

        for (auto &nei_node : next_node->neighbor_NodeRes) {
            if (!visit[nei_node.first->node_name]) {
                for (auto &nei_res : nei_node.second) {

                    if (nei_res->res_value != 0) {
                        neighbor_UnshortedNodes[root_node]
                                .insert(std::pair<Node *, Resistor *>(nei_node.first, nei_res));
                    } else if (nei_res->res_value == 0) {
                        Construct_Neighbor_UnshortedNodes(root_node, nei_node.first, current_recorder, visit);
                    }
                }
            }
        }

    }


    VoltageSource::VoltageSource() = default;

    VoltageSource::VoltageSource(std::string n, std::string pos, std::string neg, double v)
            : volt_name(std::move(n)), pos_node(std::move(pos)), neg_node(std::move(neg)), volt_value(v) {}


    CurrentSource::CurrentSource() = default;

    CurrentSource::CurrentSource(std::string n, std::string pos, std::string neg, double c)
            : curr_name(std::move(n)), pos_node(std::move(pos)), neg_node(std::move(neg)), curr_value(c) {}


    Node::Node() = default;

    Node::Node(std::string n)
            : node_name(std::move(n)), is_connect_V(false), is_grounded(false),
              is_stamp(true), is_merged(false), is_power_plane(false) {
        ParseNodeInfo(n, "_nX");
    }


    void Node::ParseNodeInfo
            (const std::string &line, const std::string &seperators) {
        std::vector<std::string> line_element;
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

        if (line_element.size() == 3) {
            layer_idx = std::stoi(line_element[0]);
            x_loc_idx = std::stoi(line_element[1]);
            y_loc_idx = std::stoi(line_element[2]);
        }

    }


    void Node::Merge_Neighbor_NodeRes(Node *nei, std::vector<Resistor *> &res) {
        if (nei != nullptr) neighbor_NodeRes[nei].insert(neighbor_NodeRes[nei].end(), res.begin(), res.end());
        else std::cerr << "Null Neighbor Node, or resistor !" << std::endl;
    }

    void Node::Add_Neighbor_Node(Node *nei, Resistor *res) {
        if (nei != nullptr && res != nullptr) neighbor_NodeRes[nei].emplace_back(res);
        else std::cerr << "Null Neighbor Node, or resistor !" << std::endl;
    }

    void Node::Add_Vsource(std::string n, VoltageSource *v) {
        if (v != nullptr) {
            connected_Vsource[n].emplace_back(v);
            is_connect_V = true;
        } else std::cerr << "Null voltage source, or resistor !" << std::endl;
    }

    void Node::Add_Isource(std::string n, CurrentSource *c) {
        if (c != nullptr) connected_Isource[n].emplace_back(c);
        else std::cerr << "Null current source, or resistor !" << std::endl;
    }

    void Node::Merge_Neighbor_Source(Node *nei) {
        if (!nei->connected_Vsource.empty()) {
            if (!nei->connected_Vsource["pos"].empty())
                connected_Vsource["pos"].insert(connected_Vsource["pos"].end(), nei->connected_Vsource["pos"].begin(),
                                                nei->connected_Vsource["pos"].end());
            if (!nei->connected_Vsource["neg"].empty())
                connected_Vsource["neg"].insert(connected_Vsource["neg"].end(), nei->connected_Vsource["neg"].begin(),
                                                nei->connected_Vsource["neg"].end());
        }

        if (!nei->connected_Isource.empty()) {
            if (!nei->connected_Isource["pos"].empty())
                connected_Isource["pos"].insert(connected_Isource["pos"].end(), nei->connected_Isource["pos"].begin(),
                                                nei->connected_Isource["pos"].end());
            if (!nei->connected_Isource["neg"].empty())
                connected_Isource["neg"].insert(connected_Isource["neg"].end(), nei->connected_Isource["neg"].begin(),
                                                nei->connected_Isource["neg"].end());
        }
    }


    Elements::Elements() = default;


    void Elements::CircuitPartition() {
        // 划分VDD和VSS平面
        std::unordered_map<std::string, bool> visit;

        for (auto &VNode : total_supply_Vsource) {
            std::queue<Node *> NodeQueue;
            if (!visit[VNode.second->node_name]) {
                NodeQueue.push(VNode.second);
                visit[VNode.second->node_name] = true;
            }
            while (!NodeQueue.empty()) {
                Node *curr_node = NodeQueue.front();
                curr_node->is_power_plane = true;
                PowerPlaneNodes[curr_node->node_name] = curr_node;
                NodeQueue.pop();
                for (auto &each_nei : curr_node->neighbor_NodeRes) {
                    if (each_nei.first->node_name != "0" && !visit[each_nei.first->node_name]) {
                        NodeQueue.push(each_nei.first);
                        visit[each_nei.first->node_name] = true;
                    }
                }
            }

        }

        for (auto &node : total_nodes) {
            if (node.second->node_name != "0" && !node.second->is_power_plane) {
                GNDPlaneNodes[node.first] = node.second;
            }
        }

        std::cout << "Total Power Plane Nodes = " << PowerPlaneNodes.size() << std::endl;
        std::cout << "Total GND   Plane Nodes = " << GNDPlaneNodes.size() << std::endl;

        std::cout << "Circuit Partitioning complete !" << std::endl;
    }


    void Elements::Merge_Node
            (Node *root, Node *self, Node *merged_node) {

        shorted_NodeMap[root].emplace_back(merged_node);
        root->Merge_Neighbor_Source(merged_node);
        merged_node->is_stamp = false;
        merged_node->is_merged = true;

        for (auto &neighbor_nodes : merged_node->neighbor_NodeRes) {
            if (neighbor_nodes.first != self && neighbor_nodes.first->is_merged == false) {

                bool is_short = false;
                for (auto &each_neighbor_node_res : neighbor_nodes.second) {
                    if (each_neighbor_node_res->res_value == 0) {
                        root->Merge_Neighbor_NodeRes(neighbor_nodes.first, neighbor_nodes.second);
                        each_neighbor_node_res->is_merged = true;
                        Merge_Node(root, merged_node, neighbor_nodes.first);
                        is_short = true;
                        break;
                    }
                }
                if (!is_short) {
                    root->Merge_Neighbor_NodeRes(neighbor_nodes.first, neighbor_nodes.second);
                    neighbor_nodes.first->Merge_Neighbor_NodeRes(root, neighbor_nodes.second);
                }
            }
        }
        return;
    }


    void Elements::NodeMapping() {

        CircuitPartition();
        // 给每个节点赋予一个序号
        total_nodes["0"]->node_voltage = 0;
        total_nodes["0"]->is_grounded = true;
        total_nodes["0"]->is_stamp = false;

        for (auto &via : total_shorted_Res) {
            std::map<std::string, bool> visit;
            visit[via->shorted_node.second->node_name] = true;
            via->Construct_Neighbor_UnshortedNodes(via->shorted_node.first, via->shorted_node.first,
                                                   &via->current_source_recorder, visit);
        }

        // 融合短路节点
        for (auto &via : total_shorted_Res) {
            if (!via->is_merged) {

                if (via->shorted_node.second->node_name == "0") {
                    shorted_NodeMap[total_nodes["0"]]
                            .emplace_back(via->shorted_node.first);

                    via->shorted_node.first->node_voltage = 0;
                    via->shorted_node.first->is_grounded = true;
                    via->shorted_node.first->is_stamp = false;
                    via->is_merged = true;
                } else {
                    via->shorted_node.first->is_stamp = true;
                    via->is_merged = true;
                    Merge_Node(via->shorted_node.first, via->shorted_node.first, via->shorted_node.second);
                }
            }
        }

        // 看起来似乎是用不用求解电压
        for (auto &volt : total_supply_Vsource) {
            volt.second->node_voltage = volt.first->volt_value;
            volt.second->is_stamp = false;
        }

        unsigned int mapping_idx = 0;
        for (auto &node : PowerPlaneNodes) {
            if (node.second->is_stamp) {
                power_stamp_NodeList
                        .insert(std::pair<Node *, unsigned int>(node.second, mapping_idx));
                mapping_idx++;
            }
        }
        mapping_idx = 0;
        for (auto &node : GNDPlaneNodes) {
            if (node.second->is_stamp) {
                gnd_stamp_NodeList
                        .insert(std::pair<Node *, unsigned int>(node.second, mapping_idx));
                mapping_idx++;
            }
        }

        std::cout << "Node mapping complete !" << std::endl;

    }

    void Elements::Output_Info() {

        std::ofstream fout("./PowerPlane", std::ios::out);
        /*
        fout << "#******************************" << std::endl;
        fout << "# Total supply voltage source : " << std::endl;
        for (auto& v : total_supply_Vsource)
        {
            fout << "--------------------------------" << std::endl;
            fout << v.first->volt_name << " " << v.first->pos_node << " " << v.first->neg_node << " "
                << v.first->volt_value << std::endl;
            fout << "--------------------------------" << std::endl;
        }
        fout << "#******************************" << std::endl << std::endl << std::endl;


        fout << "#******************************" << std::endl;
        fout << "# Total shorted Resistor : " << std::endl;
        for (auto& v : total_shorted_Res)
        {
            fout << "--------------------------------" << std::endl;
            fout << v->res_name << " " << v->pos_node << " " << v->neg_node << " "
                << v->res_value << std::endl;
            fout << "shorted node : " << std::endl;
            fout << v->shorted_node.first->node_name << " " << v->shorted_node.second->node_name << std::endl;
            fout << "--------------------------------" << std::endl;
        }
        fout << "#******************************" << std::endl << std::endl << std::endl;
        */

        fout << "#******************************" << std::endl;
        fout << "# Total Power Plane Nodes : " << std::endl;
        for (auto &node : PowerPlaneNodes) {
            fout << "--------------------------------" << std::endl;
            fout << "node name : " << node.second->node_name << std::endl;
            fout << "node layer: " << node.second->layer_idx << std::endl;
            fout << "node location = (" << node.second->x_loc_idx << ", "
                 << node.second->y_loc_idx << ")" << std::endl;

            /*
            fout << "is connected to suppply = " << node.second->is_connect_V << std::endl;
            fout << "is stamp    = " << node.second->is_stamp << std::endl;
            fout << "is grounded = " << node.second->is_grounded << std::endl;
            fout << "is merged = " << node.second->is_merged << std::endl;
            for (auto& nei : node.second->neighbor_NodeRes) {
                fout << "neighbor node name = " << nei.first->node_name << " ";
                fout << "connected resistor = ";
                for (auto& res : nei.second) { fout << res->res_name << "(" << res->res_value << ")" << " "; }
                fout << std::endl;
            }
            fout << std::endl;

            fout << "connected voltage source : ";
            for (auto &v : node.second->connected_Vsource)
            {
                fout << v.first << " ";
                for (auto &each_v : v.second)
                    fout << each_v->volt_name << " ";
            }
            fout << std::endl;
            fout << "connected current source : ";
            for (auto &i : node.second->connected_Isource)
            {
                fout << i.first << " ";
                for (auto &each_i : i.second)
                    fout << each_i->curr_name << " ";
            }
            fout << std::endl;
            */
            fout << "--------------------------------" << std::endl;
        }

        /*
        fout << "--------------------------------" << std::endl;
        fout << "node name : " << total_nodes["0"]->node_name << std::endl;
        fout << "node layer: " << total_nodes["0"]->layer_idx << std::endl;
        fout << "node location = (" << total_nodes["0"]->x_loc_idx << ", "
             << total_nodes["0"]->y_loc_idx << ")" << std::endl;

        fout << "#******************************" << std::endl << std::endl << std::endl;


        fout << "#******************************" << std::endl;
        fout << "# Total Shorted Node Map : " << std::endl;
        for (auto& list : shorted_NodeMap)
        {
            fout << list.first->node_name << " : ";
            for (auto& shorted_node : list.second)
            {
                fout << shorted_node->node_name << " ";
            }
            fout << std::endl;
        }
        fout << "#******************************" << std::endl << std::endl << std::endl;



        fout << "#******************************" << std::endl;
        fout << "# Total Stampping Nodes : " << std::endl;
        for (auto& list : power_stamp_NodeList)
        {
            fout << "stampping index : " << list.second << "=>" << list.first->node_name << std::endl;
        }
        fout << "#******************************" << std::endl << std::endl << std::endl;
        */

        std::cout << "Output lis file complete !" << std::endl;
        fout.close();
    }

}

