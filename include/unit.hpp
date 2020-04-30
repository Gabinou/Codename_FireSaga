#ifndef UNIT_HPP
#define UNIT_HPP

#include "filesystem.hpp"
#include "enums.hpp"
#include "weapon.hpp"
#include "linalg.hpp"
#include "probability.hpp"
#include "physfs.h"
#include "tinyxml2.h"
#include "utilities.hpp"

class Unit : public XML_IO {
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
    unsigned long long int skills = 0;
    short unsigned int equippable;
    unsigned short int exp = 0;
    unsigned short int id;
    unsigned short int base_exp = 0;
    unsigned short int entity;
    unsigned char class_index;
    unsigned char mvt_type;
    unsigned char army = 1;

    unsigned char current_hp = 0;
    char current_speed = 0;

    bool sex = false; // 0:F, 1:M. eg. hasPenis.
    bool show_danger = false;

    std::string name = "";
    std::string class_name = "";
    std::string army_name = "";
    std::string sex_name = "";
    std::vector<std::string> skill_names;

    Equipped equipped; // indices. -1 -> no equipment.
    Inventory_item equipment[DEFAULT::EQUIPMENT_SIZE], weapons[DEFAULT::WEAPONS_SIZE], items[DEFAULT::ITEMS_SIZE];

public:
    Unit();
    Unit(const unsigned short int in_id, const unsigned char in_class_index, const Unit_stats in_bases, const bool in_sex);
    Unit(const Unit & obj);

    void setSupports(std::vector<short int> in_supports);
    void setSupports(const Support in_supports[DEFAULT::SUPPORTS_MAX]);
    Support * getSupports();
    void supportUp(short int in_id);

    void setSex(const bool in_sex);
    bool getSex();

    bool isDanger();
    void showDanger();
    void hideDanger();

    unsigned char getMvttype();
    unsigned char getClassind();
    void setClassind(unsigned char in_class_ind);

    short unsigned int getEquippable();
    void equipsL(const unsigned char index);
    void equipsR(const unsigned char index);
    void unequipsR();
    void unequipsL();
    void equips(const unsigned short int index, const bool hand = true);
    void unequips(const bool hand = true);
    void setEquipped(Equipped in_equipped);
    void setEquipment(std::vector<Inventory_item> in_equipment);
    void setEquipment(const Inventory_item in_equipment[DEFAULT::EQUIPMENT_SIZE]);

    std::vector<Inventory_item> getEquipment();
    void addEquipment(Inventory_item in_item);
    void switchEquipment(Inventory_item in_equipment, const short int ind1, const short int ind2);
    void removeEquipment(unsigned char in_index);

    void takeItem(Inventory_item * out_array, const short int in_index, const short int out_index);
    void giveItem(Inventory_item * out_array, const short int in_index, const short int out_index);
    void dropItem(const short int in_index);

    unsigned char * getRange() const;

    void setEntity(const short int in_index);
    int getEntity();

    void setid(const unsigned short int in_id);
    unsigned short int getid();

    unsigned short int getExp() const;
    unsigned short int getLvl() const;
    void setBaseExp(const unsigned short int in_exp);
    void gainExp(const unsigned short int in_exp);

    unsigned long long int getSkills();
    void setSkills(unsigned long long int in_skills);

    void levelUp();

    unsigned char getHp() const;
    void setHp(const unsigned char in_hp);
    void takesDamage(const unsigned char damage);
    void getsHealed(const unsigned char healing);

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
    void setName(const char in_name);

    unsigned char getArmy();
    std::string getArmyName();
    void setArmy(const unsigned char in_army);

    bool canRetaliate(Unit * enemy) const;
    bool canDouble(Unit * enemy);
    bool canAttack();
    bool canEquip(const short unsigned int in_id);

    void combatStats();
    Combat_stats getCombatStats();
    unsigned char hit();
    unsigned char dodge();
    unsigned char critical();
    unsigned char favor();
    unsigned char totalMight(const bool dmg_type);
    unsigned char totalDef(const bool dmg_type);
    bool dmgType();
    char speed();

    void dies();

    void use(int in_ind);

    using XML_IO::writeXML;
    using XML_IO::readXML;
    void writeXML(tinyxml2::XMLDocument * in_doc, tinyxml2::XMLElement * in_pUnit);
    void readXML(tinyxml2::XMLElement * in_pUnit);
};

extern std::string stats2str(Unit_stats in_stats);

extern void baseUnits(std::unordered_map<int, Unit> * in_units);
extern void baseUnits(std::unordered_map<int, Unit> * in_units, std::vector<short int> toload);

extern void writeUnits_PC(const char * filename);
extern void writeUnits_NPC(const char * filename);
extern void writeAllUnits(const char * filename);

extern std::vector<short int> (*baseParties[CHAPTER::CHAP1 - CHAPTER::TEST + 1])();
extern std::vector<Map_arrival> (*mapArrivals[CHAPTER::END - CHAPTER::TEST + 1])();
extern std::vector<std::vector<Inventory_item>> (*arrivalEquipments[CHAPTER::END - CHAPTER::TEST + 1])();

void testXMLUnits();
void baseEnemyUnits();

#endif /* UNIT_HPP */