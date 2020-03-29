#ifndef UNIT_HPP
#define UNIT_HPP

#include "enums.hpp"
#include "weapon.hpp"
#include "linalg.hpp"
#include "probability.hpp"
#include "filesystem.hpp"
#include "physfs.h"
#include "tinyxml2.h"
#include "utilities.hpp"

class Unit {
    private:
        Weapon_stats temp_wpn;
        Weapon_stats right_wpn;
        Weapon_stats left_wpn;

        Combat_stats combat_stats;

        Supports supports;

        Unit_stats base_stats = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
        Unit_stats bonus_stats = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
        Unit_stats caps_stats = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
        Unit_stats malus_stats = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
        Unit_stats current_stats = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
        Unit_stats growths = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};

        Unit_state state;

        std::vector<Unit_stats> grown_stats;
        unsigned long long int skills = 0;
        short unsigned int equippable;
        unsigned short int exp = 0;
        unsigned short int id;
        unsigned short int base_exp = 0;
        unsigned short int entity;
        unsigned short int affiliation;
        unsigned char class_index;
        unsigned char mvt_type;

        unsigned char current_hp = 0;
        char current_speed = 0;
        Map_enemy map_enemy;

        short int position[2] = {0, 0};
        bool sex = false; // 0:F, 1:M

        std::string name = "";
        std::string class_name = "";
        std::string army_name = "";
        std::string sex_name = "";
        std::vector<std::string> skill_names;

        Equipped equipped; // these are indices. -1 means no equipment.
        Inventory_item equipment[DEFAULT::EQUIPMENT_SIZE], weapons[DEFAULT::WEAPONS_SIZE], items[DEFAULT::ITEMS_SIZE];

    public:
        Unit();
        Unit(const unsigned short int in_id, const unsigned char in_class_index, const Unit_stats in_bases, const bool in_sex);

        void setSupports(short int * in_names);

        void setSex(const bool in_sex);
        bool getSex();

        Map_enemy getMap_enemy();
        void setMap_enemy(Map_enemy in_map_enemy);

        short int * getPos();
        void setPos(const short int in_pos[2]);
        void setPos(const Point in_pos);

        unsigned char getMvttype();

        short unsigned int getEquippable();
        void equipsL(const unsigned char index);
        void equipsR(const unsigned char index);
        void unequipsR();
        void unequipsL();
        void equips(const unsigned short int index, const bool hand = true);
        void unequips(const bool hand = true);
        void setEquipped(Equipped in_equipped);
        void setEquipment(Inventory_item * in_equipment);
        void addEquipment(Inventory_item in_equipment);
        void switchEquipment(Inventory_item in_equipment, const short int ind1, const short int ind2);
        void removeEquipment(unsigned char in_index);

        void takeItem(Inventory_item * out_array, const short int in_index, const short int out_index);
        void giveItem(Inventory_item * out_array, const short int in_index, const short int out_index);
        void dropItem(const short int in_index);

        unsigned char * getRange() const;

        void setEntity(const short int in_index);
        int getEntity();

        unsigned short int getExp() const;
        unsigned short int getLvl() const;
        void setBaseExp(const unsigned short int in_exp);
        void gainExp(const unsigned short int in_exp);

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

        std::string getArmy();
        void setArmy(const std::string in_army);
        void setArmy(const char in_army);

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

        void write(const char * filename, const bool append = false);
        void writeXML(const char * filename, const bool append = false);
        void readXML(const char * filename);

};

extern std::vector<Unit> all_units;
extern std::vector<Unit> loaded_units;
extern std::vector<Unit> (*chapEnemyUnits[40])();

void baseUnits();
std::vector<Unit> baseUnits(std::vector<short int> toload);
void testXMLUnits();
void baseEnemyUnits();
extern std::vector<Unit> chap1EnemyUnits();

#endif /* UNIT_HPP */