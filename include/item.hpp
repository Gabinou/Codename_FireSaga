#ifndef ITEM_HPP
#define ITEM_HPP

#include "enums.hpp"
#include "structs.hpp"
#include "physfs.h"
#include "tinyxml2.h"
#include "filesystem.hpp"
#include "utilities.hpp"
#include "string.h"
#include <string>
// #ifndef STB_SPRINTF_IMPLEMENTATION //Why no need?
// #define STB_SPRINTF_IMPLEMENTATION
#include "stb_sprintf.h"
//#endif /* STB_SPRINTF_IMPLEMENTATION */

class Item : public XML_IO, JSON_IO, BASE_IO {
protected:
    Unit_stats bonus_stats = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
    Unit_stats malus_stats = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};

    bool sellable = true;
    bool equippable = true; // unused by weapons
    unsigned short int id = 0; // 0 means empty.
    short unsigned int type = 0;
    long unsigned int effect = 0;
    short int use_effect = -1;

    std::vector<unsigned short int> users; // item only usable by users. empty = everyone
    std::string name = "";
    std::string description = "";
public:
    Item();
    Item(short unsigned int in_type, unsigned char in_id);

    std::vector<unsigned short int> getUsers();
    void setUsers(std::vector<unsigned short int> in_users);
    bool isSellable();
    void setSellable(bool in_sellable);

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

    using JSON_IO::writeJSON;
    using JSON_IO::readJSON;
    void readJSON(cJSON * in_json);
    void writeJSON(cJSON * in_json);

    using XML_IO::writeXML;
    using XML_IO::readXML;
    void writeXML(tinyxml2::XMLDocument * in_doc, tinyxml2::XMLElement * in_pItem);
    void readXML(tinyxml2::XMLElement * in_pItem);
};

// extern std::vector<Weapon> all_weapons;
// extern std::vector<Weapon> loaded_weapons;

// void baseWeapons();
// void writeallXMLWeapons();
// std::vector<Weapon> baseWeapons(std::vector<short int> toload);

#endif /* ITEM_HPP */