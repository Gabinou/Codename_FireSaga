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
    weapon_num[name]++;
    weapon_num[name]++;
    weapon_num[name]++;
    std::stringstream ss;
    // string a = printf("%04d\n", weapon_num[name]);
    // string a = weapon_num.ToString().PadLeft(4, '0');
    ss  << std::setfill('0') << std::setw(5) << 25;
    std::string s = ss.str();
    printf("%s \n", s.c_str());
    std::string new_name[100];
    // newname = strcat(in_name, strcat("_", s.c_str()));
    printf("%s \n", new_name + "hello");
    // ss << std::hex <<  std::setw(4) << std::setfill('0') << (int) weapon_num[name];

    // strcpy(name, s.c_str());
    // printf("%s \n", name);
    // printf("%s \n", s);
    used = in_used;
    weapon_num[name]++;
}

inventory_item::~inventory_item(void) {
    // printf("Weapon %s is being deleted.\n" , name);
}
inventory_item::inventory_item(const inventory_item& source) {
    strcpy(name, source.name);
    used = source.used;
    weapon_num[name]++;
}

