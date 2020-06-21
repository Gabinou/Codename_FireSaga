#ifndef UNIT_HPP
#define UNIT_HPP

#include "filesystem.hpp"
#include "enums.hpp"
#include "item.hpp"
#include "weapon.hpp"
#include "linalg.hpp"
#include "probability.hpp"
#include "physfs.h"
#include "tinyxml2.h"
#include "utilities.hpp"

class Unit : public XML_IO, JSON_IO {
private:
    Weapon_stats temp_wpn;
    Weapon_stats right_wpn;
    Weapon_stats left_wpn;

    Combat_stats combat_stats;
    Unit_state state;

    Support supports[DEFAULT::SUPPORTS_MAX];

    Unit_stats base_stats = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
    Unit_stats bonus_stats = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
    Unit_stats caps_stats = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
    Unit_stats malus_stats = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
    Unit_stats current_stats = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
    Unit_stats growths = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};

    std::vector<Unit_stats> grown_stats;
    uint64_t skills = 0;
    uint16_t equippable;
    uint16_t exp = 0;
    uint16_t id;
    uint16_t base_exp = 0;
    uint16_t entity;
    int8_t class_index;
    int8_t mvt_type;
    int8_t army = 1;

    uint8_t current_hp = 0;
    int8_t current_speed = 0;

    bool sex = false; // 0:F, 1:M. eg. hasPenis.
    bool literate = false; // Reading/writing for scribe job.
    bool show_danger = false;
    bool waits;

    std::string name = "";
    std::string class_name = "";
    std::string army_name = "";
    std::string sex_name = "";
    std::vector<std::string> skill_names;

    std::unordered_map<int16_t, Weapon> * weapons = NULL;

    int8_t equipped[2] = {-1, -1};
    bool hands[2] = {true, true};
    Inventory_item equipment[DEFAULT::EQUIPMENT_SIZE]; // 0 for right hand, 1 for left hand

public:
    Unit();
    Unit(const int16_t in_id, const int8_t in_class_index, const Unit_stats in_bases, const bool in_sex);
    Unit(const Unit & obj);

    void init();
    void copyUnit(const Unit & obj);

    void setSupports(std::vector<int16_t> in_supports);
    void setSupports(const Support in_supports[DEFAULT::SUPPORTS_MAX]);
    Support * getSupports();
    void supportUp(int16_t in_id);

    void setSex(const bool in_sex);
    bool getSex();

    void setWeapons(std::unordered_map<int16_t, Weapon> * in_weapons);
    void checkWeapon(int16_t in_id);

    bool isDanger();
    void showDanger();
    void hideDanger();

    void setLiteracy(bool in_literacy);
    bool isLiterate();

    int8_t getMvttype();
    int8_t getClassind();
    void setClassind(int8_t in_class_ind);

    int16_t getEquippable();
    void equips(const bool hand = true, const int8_t to_equip = -1);
    void unequips(const bool hand = true);
    void setEquipment(std::vector<Inventory_item> in_equipment);
    void setEquipment(const Inventory_item in_equipment[DEFAULT::EQUIPMENT_SIZE]);

    std::vector<Inventory_item> getEquipment();
    void addEquipment(Inventory_item in_item);
    void switchEquipment(Inventory_item in_equipment, const int16_t ind1, const int16_t ind2);
    void removeEquipment(int8_t in_index);

    void takeItem(Inventory_item * out_array, const int16_t in_index, const int16_t out_index);
    void giveItem(Inventory_item * out_array, const int16_t in_index, const int16_t out_index);
    void dropItem(const int16_t in_index);

    uint8_t * getRange();

    void setEntity(const int16_t in_index);
    int getEntity();

    void setid(const int16_t in_id);
    int16_t getid();

    int16_t getExp() const;
    int16_t getLvl() const;
    void setBaseExp(const uint16_t in_exp);
    void gainExp(const uint16_t in_exp);

    uint64_t getSkills();
    void setSkills(uint64_t in_skills);

    void levelUp();

    uint8_t getHp() const;
    void setHp(const uint8_t in_hp);
    void takesDamage(const uint8_t damage);
    void getsHealed(const uint8_t healing);

    Unit_stats getBonus();
    void setBonus(const Unit_stats in_stats);
    Unit_stats getMalus();
    void setMalus(const Unit_stats in_stats);
    Unit_stats getCaps();
    void setCaps(const Unit_stats in_stats);
    Unit_stats getStats();
    void setStats(const Unit_stats in_stats);
    Unit_stats getBases();
    void setBases(const Unit_stats in_stats);
    Unit_stats getGrowths();
    void setGrowths(const Unit_stats in_growths);

    std::string getName();
    void setName(const std::string in_name);
    void setName(const int8_t in_name);

    uint8_t getArmy();
    std::string getArmyName();
    void setArmy(const uint8_t in_army);

    bool canRetaliate(Unit * enemy) const;
    bool canDouble(Unit * enemy);
    bool canAttack();
    bool canEquip(const int16_t in_id);

    void combatStats();
    Combat_stats getCombatStats();
    uint8_t hit();
    uint8_t dodge();
    uint8_t critical();
    uint8_t favor();
    uint8_t totalMight(const bool dmg_type);
    uint8_t totalDef(const bool dmg_type);
    bool dmgType();
    int8_t speed();

    bool isWaiting();
    void refresh();
    void wait();
    void dies();

    void use(int in_ind);

    using JSON_IO::writeJSON;
    using JSON_IO::readJSON;
    void readJSON(cJSON * in_junit);
    void writeJSON(cJSON * in_junit);

    using XML_IO::writeXML;
    using XML_IO::readXML;
    void writeXML(tinyxml2::XMLDocument * in_doc, tinyxml2::XMLElement * in_pUnit);
    void readXML(tinyxml2::XMLElement * in_pUnit);
};

extern std::vector<int16_t> init_party;

#endif /* UNIT_HPP */