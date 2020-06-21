#ifndef WEAPON_HPP
#define WEAPON_HPP

#include "item.hpp"
#include "utilities.hpp"
#include "filesystem.hpp"
#include <cstdint>
// #ifndef STB_SPRINTF_IMPLEMENTATION //Why no need?
// #define STB_SPRINTF_IMPLEMENTATION
#include "stb_sprintf.h"
//#endif /* STB_SPRINTF_IMPLEMENTATION */

class Weapon : public Item {
private:
    Weapon_stats stats = {0, 0, 0, 0, 0, 0, 0, 0, 0, {0, 0}, {0, 0}, 0, 0};
    uint16_t effective = 0; // is a unit type.
    Infusion infused;
    bool infuseable;
    bool attackable = false; // Offhands are not attackable: cannot attack, only provides bonus stats.
public:
    Weapon();
    Weapon(uint16_t in_type, Weapon_stats in_stats, uint8_t in_id);

    void setStats(Weapon_stats in_stats);
    Weapon_stats getStats();

    void attackablefromType();
    void setAttackable(bool in_attackable);
    bool canAttack();

    void infuse(uint8_t in_mag, uint16_t in_type);
    Infusion getInfused();
    void setInfuseable(bool in_infuseable);
    bool canInfuse();
    void setEffective(uint16_t in_effective);
    uint16_t getEffective();

    using Item::writeJSON;
    using Item::readJSON;
    void readJSON(cJSON * in_json);
    void writeJSON(cJSON * in_json);

    using Item::writeXML;
    using Item::readXML;
    void writeXML(tinyxml2::XMLDocument * in_doc, tinyxml2::XMLElement * in_pWpn);
    void readXML(tinyxml2::XMLElement * in_pWpn);
};

#endif /* WEAPON_HPP */