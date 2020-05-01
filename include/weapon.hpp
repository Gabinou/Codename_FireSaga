#ifndef WEAPON_HPP
#define WEAPON_HPP

#include "enums.hpp"
#include "structs.hpp"
#include "physfs.h"
#include "unit.hpp"
#include "tinyxml2.h"
#include "filesystem.hpp"
#include "utilities.hpp"
#include "string.h"
#include <string>
// #ifndef STB_SPRINTF_IMPLEMENTATION //Why no need?
// #define STB_SPRINTF_IMPLEMENTATION
#include "stb_sprintf.h"
//#endif /* STB_SPRINTF_IMPLEMENTATION */

class Weapon : public XML_IO {
private:
    Weapon_stats stats = {0, 0, 0, 0, 0, 0, 0, 0, 0, {0, 0}, {0, 0}, 0, 0};
    // Pmight, Mmight, hit, dodge, crit, favor, wgt, uses, prof, range, hand, dmg_type, cost, heal
    Unit_stats bonus_stats = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
    Unit_stats malus_stats = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};

    unsigned short int id = 0; // 0 means empty.
    long unsigned int effect = 0;
    short unsigned int type = 0;
    short unsigned int effective = 0; // is a unit type.
    short int use_effect = -1;
    Infusion infused;

    bool sellable = true;

    std::vector<unsigned short int> users; // item only usable by users. empty = everyone
    std::string name = "";
    std::string description = "";
public:
    Weapon();
    Weapon(short unsigned int in_type, Weapon_stats in_stats, unsigned char in_id);

    void setStats(Weapon_stats in_stats);
    Weapon_stats getStats();
    void setBonus(Unit_stats in_bonus);
    Unit_stats getBonus();
    void setMalus(Unit_stats in_malus);
    Unit_stats getMalus();
    void setEffect(long unsigned int in_effect);
    long unsigned int getEffect();
    void setEffective(short unsigned int in_effective);
    short unsigned int getEffective();
    void setDescription(std::string in_description);
    std::string getDescription();
    void setName(std::string in_name);
    std::string getName();
    void setUser();
    short unsigned int getType();
    void setType(short unsigned int in_type);
    void infuse(unsigned char in_mag, short unsigned int in_type);
    Infusion getInfused();
    bool canInfuse();

    std::vector<unsigned short int> getUsers();
    void setUsers(std::vector<unsigned short int> in_users);
    bool isSellable();
    void setSellable(bool in_sellable);

    using XML_IO::writeXML;
    using XML_IO::readXML;
    void writeXML(tinyxml2::XMLDocument * in_doc, tinyxml2::XMLElement * in_pWpn);
    void readXML(tinyxml2::XMLElement * in_pWpn);
};

extern std::vector<Weapon> all_weapons;
extern std::vector<Weapon> loaded_weapons;

void baseWeapons();
void writeallXMLWeapons();
std::vector<Weapon> baseWeapons(std::vector<short int> toload);

#endif /* WEAPON_HPP */