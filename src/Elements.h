//
// Created by Zhengxiao Du on 2018/11/25.
//

#ifndef PGSOLVER_ELEMENTS_H
#define PGSOLVER_ELEMENTS_H

#include <utility>
#include <vector>
#include <map>
#include <unordered_map>
#include <queue>
#include <string>
#include <list>

namespace PowerGrid {

    bool column_compare(const std::pair<unsigned int, double> &a, const unsigned &b);

    class SparseRow : public std::list<std::pair<unsigned int, double>> {
    public:
        double &operator[](unsigned index);
    };

    struct CurrentSource;
    struct VoltageSource;
    struct Resistor;
    struct Node;

    struct Resistor {

        Resistor();

        ~Resistor();

        Resistor(std::string n, std::string pos, std::string neg, double value);

        void Add_Node(Node *n1, Node *n2);

        void
        Construct_Neighbor_UnshortedNodes
                (Node *root_node, Node *next_node, double *current_recorder, std::map<std::string, bool> &visit);

        std::string res_name;
        std::string pos_node, neg_node;
        double res_value;
        double curr_value;
        double power_value;
        double Vdrop;
        bool is_merged;
        double current_source_recorder;
        std::pair<Node *, Node *> shorted_node;
        std::map<Node *, std::map<Node *, Resistor *> > neighbor_UnshortedNodes;

    };

    struct VoltageSource {

        VoltageSource();

        VoltageSource(std::string n, std::string pos, std::string neg, double v);

        std::string volt_name;
        std::string pos_node, neg_node;
        double volt_value;
        double curr_value;
        double power_value;
    };

    struct CurrentSource {

        CurrentSource();

        CurrentSource(std::string n, std::string pos, std::string neg, double c);

        std::string curr_name;
        std::string pos_node, neg_node;
        double curr_value;
    };


    struct Node {

        Node();

        Node(std::string n);

        void Merge_Neighbor_NodeRes(Node *nei, std::vector<Resistor *> &res);

        void Add_Neighbor_Node(Node *nei, Resistor *res);

        void Add_Vsource(std::string n, VoltageSource *v);

        void Add_Isource(std::string n, CurrentSource *c);

        void Merge_Neighbor_Source(Node *nei);

        std::string node_name;
        double node_voltage;
        bool is_connect_V;
        bool is_grounded;
        bool is_stamp;
        bool is_merged;

        int layer_idx;
        int x_loc_idx, y_loc_idx;
        bool is_power_plane;

        void ParseNodeInfo
                (const std::string &line, const std::string &seperators);

        std::map<Node *, std::vector<Resistor *> > neighbor_NodeRes;
        std::map<std::string, std::vector<VoltageSource *> > connected_Vsource;
        std::map<std::string, std::vector<CurrentSource *> > connected_Isource;

    };


    class Elements {

    public :

        Elements();

        void Merge_Node(Node *root, Node *self, Node *merged_node);

        void CircuitPartition();

        void NodeMapping();

        void Output_Info();

        std::unordered_map<std::string, Node *> total_nodes;
        std::unordered_map<std::string, Node *> PowerPlaneNodes, GNDPlaneNodes;
        std::map<VoltageSource *, Node *> total_supply_Vsource;
        std::vector<Resistor *> total_shorted_Res;
        std::vector<Resistor *> total_unshorted_Res;
        std::vector<CurrentSource *> total_Isource;
        std::map<Node *, std::vector<Node *> > shorted_NodeMap;
        std::map<Node *, unsigned int> power_stamp_NodeList, gnd_stamp_NodeList;


    };

}

#endif //PGSOLVER_ELEMENTS_H
