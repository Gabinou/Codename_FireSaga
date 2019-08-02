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
    std::stringstream nums;
    nums  << std::setfill('0') << std::setw(5) << weapon_num[name];
    std::string num = nums.str();
    std::string new_name;
    new_name += in_name;
    new_name += "_";
    new_name += num;
    strncpy(name, new_name.c_str(), sizeof(new_name));
    used = in_used;
    weapon_num[in_name]++;
}

inventory_item::~inventory_item(void) {
    // printf("Weapon %s is being deleted.\n" , name);
}
inventory_item::inventory_item(const inventory_item& source) {
    strcpy(name, source.name);
    used = source.used;
    weapon_num[name]++;
}

