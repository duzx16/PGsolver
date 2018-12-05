//
// Created by Zhengxiao Du on 2018/12/5.
//

#include <iostream>
#include <cstdlib>
#include <utility>
#include <map>
#include <vector>
#include <cmath>
#include <fstream>
#include <algorithm>
#include <string>

using Solution_Pair = std::map<std::string, double > ;


void
Parse_Line
        (const std::string &line, const std::string &seperators, std::vector<std::string> &line_element)
{
    line_element.clear();
    std::string token;
    for (size_t i = 0;i<line.length();i++) {
        if (seperators.find(line[i]) != std::string::npos) {
            if (token.size() != 0) {
                line_element.push_back(token);
                token.clear();
            }
        }
        else token.push_back(line[i]);
    }
    if (token.size() != 0) {
        line_element.push_back(token);
        token.clear();
    }

}

void
Parse_Input_Sol
        (const std::string &file_path, Solution_Pair& SolPair)
{
    std::ifstream in(file_path.c_str(), std::ios::in);

    if (in.is_open()) {

        std::cout << "Open solution file success !" << std::endl;

        std::vector<std::string> line_elements;
        std::string line;

        while (std::getline(in, line))
        {
            Parse_Line(line, " 	", line_elements);
            SolPair.insert(std::pair<std::string, double>(line_elements[0], std::stod(line_elements[1])));
        }

    }
    std::cout << "Parse solution file complete !" << std::endl;
    in.close();
}



int main(int argc, char* argv[])
{
    Solution_Pair MySolPair;
    Solution_Pair SpiceSolPair;

    Parse_Input_Sol(argv[1], MySolPair);
    Parse_Input_Sol(argv[2], SpiceSolPair);

    std::vector<double> abs_error(SpiceSolPair.size());
    std::vector<double> rel_error(SpiceSolPair.size());
    unsigned int idx = 0;
    double error_sum = 0;
    for (auto& each_solution : MySolPair)
    {
        error_sum += fabs(each_solution.second - SpiceSolPair[each_solution.first]);
        abs_error[idx] = fabs(each_solution.second - SpiceSolPair[each_solution.first]);
        rel_error[idx] = fabs(each_solution.second - SpiceSolPair[each_solution.first]) / SpiceSolPair[each_solution.first];
        ++idx;
    }

    std::cout << " Avgerage Error 	= " << error_sum / SpiceSolPair.size() << std::endl;
    std::cout << " Max Absolute Error 	= " << *std::max_element(abs_error.begin(), abs_error.end()) << std::endl;
    std::cout << " Max Relative Error 	= " << *std::max_element(rel_error.begin(), rel_error.end()) << std::endl;

    return 0;
}

