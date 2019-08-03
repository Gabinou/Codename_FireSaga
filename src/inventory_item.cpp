// Code créé par Gabriel Taillon
// Note on standards. Fuck all that complicated bullcrap. I'm writing for c++11. More convenient and the rest seems like bullshit.
/*! \page handle main.cpp
* \file main.cpp 
* \brief Main game algorithm page.
*/

#include "inventory_item.hpp"

inventory_item::inventory_item() {

}

inventory_item::inventory_item(std::string in_name, char in_used){
    strncpy(name, in_name.c_str(), sizeof(in_name));
    used = in_used;
}

inventory_item::~inventory_item(void) {
    // printf("Weapon %s is being deleted.\n" , name);
}
inventory_item::inventory_item(const inventory_item& source) {
    strcpy(name, source.name);
    used = source.used;
}

