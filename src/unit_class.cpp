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
    out << "Promoted: \t" << promoted << "\n";
    out << "Dragon: \t" << dragon << "\n";
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

}

unit_class::unit_class(std::string in_name, bool in_mounted, bool in_flying, bool in_armored, bool in_promoted, bool in_dragon, std::vector<unsigned int> in_stats_caps) { 
    strncpy(name, in_name.c_str(), sizeof(in_name));
    mounted = in_mounted;
    flying = in_flying;
    armored = in_armored;
    promoted = in_promoted;
    dragon = in_dragon;
    for (int i = 0; i < in_stats_caps.size(); i++) {
        stats_caps[i] = (unsigned int) in_stats_caps[i]; 
    }
}



