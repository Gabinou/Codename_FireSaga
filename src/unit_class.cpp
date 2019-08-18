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

}

void unit_class::read(const char *filename, char skip) { 

}

unit_class::unit_class(std::string in_name, bool in_mounted, bool in_flying, bool in_armored, bool in_promoted, bool in_dragon, std::vector<unsigned int> in_stats_max) { 
    strncpy(name, in_name.c_str(), sizeof(in_name));
    mounted = in_mounted;
    flying = in_flying;
    armored = in_armored;
    promoted = in_promoted;
    dragon = in_dragon;
    for (int i = 0; i < in_stats_max.size(); i++) {
        stats_max[i] = (unsigned int) in_stats_max[i]; 
    }
}



