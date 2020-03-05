// Code créé par Gabriel Taillon
#include "shared.hpp"
#include "game.hpp"

Inventory_item convoy[200];

std::vector<std::string> wpntype2str(short unsigned int in_type){
    std::vector<std::string> types;
    if ((in_type & WPN::TYPE::SWORD) > 0) {
        types.push_back("Sword");
    }
    if ((in_type & WPN::TYPE::LANCE) > 0) {
        types.push_back("Lance");
    }
    if ((in_type & WPN::TYPE::AXE) > 0) {
        types.push_back("Axe");
    }
    if ((in_type & WPN::TYPE::BOW) > 0) {
        types.push_back("Bow");
    }
    if ((in_type & WPN::TYPE::TRINKET) > 0) {
        types.push_back("Trinket");
    }
    if ((in_type & WPN::TYPE::OFFHAND) > 0) {
        types.push_back("Offhand");
    }
    if ((in_type & WPN::TYPE::ELEMENTAL) > 0) {
        types.push_back("Magic");
        // types.push_back("Elemental");
    }
    if ((in_type & WPN::TYPE::DEMONIC) > 0) {
        types.push_back("Demonic");
    }
    if ((in_type & WPN::TYPE::ANGELIC) > 0) {
        types.push_back("Angelic");
    }
    if ((in_type & WPN::TYPE::SHIELD) > 0) {
        types.push_back("Shield");
    }
    if ((in_type & WPN::TYPE::STAFF) > 0) {
        types.push_back("Staff");
    }
    return(types);
}

int pingpong(int current, int upper, int lower) {
    // returns pingpong index.
    // modulo: x % 2 = 0,1,2,0,1,2,0...for x++
    // pingpong(x, 2) = 0,1,2,1,0,1,2... for x++
    int mod_factor = (2 * (upper - lower) - 2);
    int term1 = mod_factor - (current % mod_factor);
    int term2 = current % mod_factor;
    return (std::min(term1, term2) + lower);
}

int geometricslide(int distance, float geo_factor) {
    // geometric slide cause the series is geometric:
    // distance/2 + distance/4 + distance/16.... 1
    // animates slides
    int sign = sgn(distance);
    int out = sign * std::max(sign * (int)(distance / geo_factor), 1);
    return (out);
    // std::abs() possible instead of sign*distance,
    // but this is more elegant.
}