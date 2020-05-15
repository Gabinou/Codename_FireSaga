#ifndef WEAPON_HPP
#define WEAPON_HPP

#include "item.hpp"
// #ifndef STB_SPRINTF_IMPLEMENTATION //Why no need?
// #define STB_SPRINTF_IMPLEMENTATION
#include "stb_sprintf.h"
//#endif /* STB_SPRINTF_IMPLEMENTATION */

class Weapon : public Item {
private:
    Weapon_stats stats = {0, 0, 0, 0, 0, 0, 0, 0, 0, {0, 0}, {0, 0}, 0, 0};
    short unsigned int effective = 0; // is a unit type.
    Infusion infused;
public:
    Weapon();
    Weapon(short unsigned int in_type, Weapon_stats in_stats, unsigned char in_id);

    void setStats(Weapon_stats in_stats);
    Weapon_stats getStats();

    void infuse(unsigned char in_mag, short unsigned int in_type);
    Infusion getInfused();
    bool canInfuse();
    void setEffective(short unsigned int in_effective);
    short unsigned int getEffective();

    using Item::writeJSON;
    using Item::readJSON;
    void readJSON(cJSON * in_json);
    void writeJSON(cJSON * in_json);

    using Item::writeXML;
    using Item::readXML;
    void writeXML(tinyxml2::XMLDocument * in_doc, tinyxml2::XMLElement * in_pWpn);
    void readXML(tinyxml2::XMLElement * in_pWpn);
};

extern std::vector<Weapon> all_weapons;
extern std::vector<Weapon> loaded_weapons;

void baseWeapons();
void writeallXMLWeapons();
std::vector<Weapon> baseWeapons(std::vector<short int> toload);

#endif /* WEAPON_HPP */