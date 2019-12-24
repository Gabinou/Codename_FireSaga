// Code créé par Gabriel Taillon
/*! \page handle main.cpp
* \file main.cpp
* \brief Main game algorithm page.
*/

#include "shared.hpp"
#include "inventory_item.hpp"

inventory_item::inventory_item() {
}

inventory_item::inventory_item(std::string in_name, char in_used) {
    strncpy(name, in_name.c_str(), sizeof(name));
    used = in_used;
}

inventory_item::~inventory_item(void) {
    // printf("Weapon %s is being deleted.\n" , name);
}
inventory_item::inventory_item(const inventory_item & source) {
    strcpy(name, source.name);
    used = source.used;
}
