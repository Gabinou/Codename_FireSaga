// Code créé par Gabriel Taillon
// Note on standards. Fuck all that complicated bullcrap. I'm writing for c++11. More convenient and the rest seems like bullshit.
/*! \page handle unit_claass.cpp
* \file unit_class.cpp 
* \brief unit_class class.
*/
#include "shared.hpp"
#include "unit.hpp"
#include "weapon.hpp"
#include "unit_class.hpp"
using namespace std;


unit_class::~unit_class(void) {
   // printf("Unit %s is being deleted.\n", name.c_str());
}

unit_class::unit_class(void) {
   // printf("Unit %s is being deleted.\n", name.c_str());
}

void unit_class::write(std::string filename) {  
    std::ofstream out(filename, std::ios_base::app);
    out << "Class: \t\t" << name << "\n";
    out << "Mounted: \t" << mounted << "\n";
    out << "Flying: \t"  << flying << "\n";
    out << "Armored: \t" << armored << "\n";
    out << "Dragon: \t" << dragon << "\n";
    out << "Promotion: \t";
    for (int i = 0; i < promotion.size(); i++) {
        if (i == promotion.size() - 1) {
            out << promotion[i] << "\n";
        } else {
            out << promotion[i] << ", ";
        }
    };
    out << "Weapon: \t";
    if (usable_wpn.size() == 0) {
        out << "\n";
    }
    for (int i = 0; i < usable_wpn.size(); i++) {
        if (i == usable_wpn.size() - 1) {
            out << usable_wpn[i] << "\n";
        } else {
            out << usable_wpn[i] << ", ";
        }
    };
    out << "Caps: \t\t";
    for (int i = 0; i < sizeof(stats_caps)/sizeof(stats_caps[0]); i++) {
        if (i == (sizeof(stats_caps)/sizeof(stats_caps[0])) - 1) {
            out << (int) stats_caps[i] << "\n";
        } else {
            out << (int) stats_caps[i] << ", ";
        }
    };
    out << std::endl;
}

void unit_class::read(const char *filename, char skip) { 
    // 2019/07/30: skip should be a multiple of *number of lines written to units.txt* which is 20.
    std::ifstream infile(filename);
    std::string line;
    std::vector<int> temp;
    std::string new_name;
    std::vector<std::string> tempstr;
    int j = 0;
    while (j < skip + 9) {
        std::getline(infile, line);
        // // std::cout << line << endl;
        if (j>=skip) {
            std::istringstream iss(line);
            strncpy(name, line.substr(9, line.size()).c_str(), sizeof(line));
            // std::cout << name << endl;
            std::getline(infile, line);
            temp = csv_from_line(line.substr(8, line.size()));
            mounted = temp[0];
            std::getline(infile, line);
            temp = csv_from_line(line.substr(8, line.size()));
            flying = temp[0];
            std::getline(infile, line);
            temp = csv_from_line(line.substr(8, line.size()));
            armored = temp[0];
            std::getline(infile, line);
            temp = csv_from_line(line.substr(8, line.size()));
            dragon = temp[0];
            std::getline(infile, line);
            tempstr = css_from_line(line.substr(10, line.size()));
            for (int i = 0; i < temp.size(); i++) {
                promotion.push_back(tempstr[i]);
            }
            std::getline(infile, line);
            tempstr = css_from_line(line.substr(9, line.size()));
            for (int i = 0; i < temp.size(); i++) {
                usable_wpn.push_back(tempstr[i]);
            }
            std::getline(infile, line);
            // std::cout << line.substr(8, line.size()) << endl;
            temp = csv_from_line(line.substr(8, line.size()));
            for (int i = 0; i < temp.size(); i++) {
                stats_caps[i] = (int) temp[i];
               // std::cout << (int) temp[i] << endl; 
            }
            j+=9;
        }
        j++;
    }
}

unit_class::unit_class(std::string in_name, bool in_mounted, bool in_flying, bool in_armored, bool in_dragon, std::vector<unsigned int> in_stats_caps, std::vector<std::string> in_promotion, std::vector<std::string> in_usable_wpn) { 
    strncpy(name, in_name.c_str(), sizeof(in_name));
    mounted = in_mounted;
    flying = in_flying;
    armored = in_armored;
    for (int i = 0; i < in_promotion.size(); i++) {
        promotion.push_back(in_promotion[i]);
    };
    for (int i = 0; i < in_usable_wpn.size(); i++) {
        usable_wpn.push_back(in_usable_wpn[i]);
    };
    dragon = in_dragon;
    for (int i = 0; i < in_stats_caps.size(); i++) {
        stats_caps[i] = (unsigned int) in_stats_caps[i]; 
    }
}



