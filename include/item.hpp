#ifndef ITEM_HPP
#define ITEM_HPP

#include "enums.hpp"
#include "structs.hpp"
#include "physfs.h"
#include "tinyxml2.h"
#include "filesystem.hpp"
#include "utilities.hpp"
#include "string.h"
// #ifndef STB_SPRINTF_IMPLEMENTATION //Why no need?
// #define STB_SPRINTF_IMPLEMENTATION
#include "stb_sprintf.h"
//#endif /* STB_SPRINTF_IMPLEMENTATION */

class Item : public XML_IO, public JSON_IO {
protected:
    Unit_stats bonus_stats = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
    Unit_stats malus_stats = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};

    bool sellable = true;
    bool equippable = true; // unused by weapons
    uint16_t id = 0; // 0 means empty.
    uint16_t type = 0;
    uint64_t effect = 0;
    int16_t use_effect = -1;

    std::vector<uint16_t> users; // item only usable by users. empty = everyone
    std::string name = "";
    std::string description = "";
public:
    Item();
    Item(uint16_t in_type, uint8_t in_id);

    std::vector<uint16_t> getUsers();
    void setUsers(std::vector<uint16_t> in_users);
    bool isSellable();
    void setSellable(bool in_sellable);

    void setBonus(Unit_stats in_bonus);
    Unit_stats getBonus();
    void setMalus(Unit_stats in_malus);
    Unit_stats getMalus();
    void setEffect(uint64_t in_effect);
    uint64_t getEffect();
    void setEffective(uint16_t in_effective);
    uint16_t getEffective();
    void setDescription(std::string in_description);
    std::string getDescription();
    void setName(std::string in_name);
    std::string getName();
    void setUser();
    uint16_t getType();
    void setType(uint16_t in_type);

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
// std::vector<Weapon> baseWeapons(std::vector<int16_t> toload);

#endif /* ITEM_HPP */