#ifndef UNIT_HPP
#define UNIT_HPP

#include "shared.hpp"
#include "weapon.hpp"

class Unit {
    private:
        Weapon_stats temp_wpn;
        Weapon_stats right_wpn;
        Weapon_stats left_wpn;

        Combat_stats combat_stats;

        Unit_stats base_stats;
        Unit_state state;
        Unit_stats bonus_stats;
        Unit_stats caps_stats;
        Unit_stats malus_stats;
        Unit_stats current_stats;
        Unit_stats growths;

        std::vector<Unit_stats> grown_stats;
        std::vector<std::string> skills;
        std::vector<std::string> equippable;

        std::unordered_map<std::string, int> supports;
        unsigned char current_hp;
        char current_speed;
        unsigned int exp;
        int entity;
        int position[2] = {0, 0};
        bool sex; // 0:F, 1:M

        std::string name;
        std::string class_name;
        std::string mvt_type;
        std::string army; //affilistion?

        Equipped equipped; // these are indices. -1 means no equipment.
        Inventory_item equipment[7], weapons[3], items[3];

    public:
        Unit();
        Unit(const std::string in_name, const std::string in_class, const Unit_stats in_bases, const bool in_sex);
        Unit(const std::string in_name, const std::string in_class, const Unit_stats in_bases);
        Unit(const std::string in_name, const Unit_stats in_bases);
        Unit(const Unit_stats in_bases);

        int * getPos();
        void setPos(const int in_pos[2]);

        std::string getMvttype();
        void setMvttype();

        std::vector<std::string> getEquippable();
        void setEquippable();
        void equipsL(const char index);
        void equipsR(const char index);
        void unequipsR();
        void unequipsL();
        void equips(const unsigned int index, const std::string hand = "right");
        void unequips(const std::string hand = "right");

        void takeItem(Inventory_item * out_array, int in_index, int out_index);
        void giveItem(Inventory_item * out_array, int in_index, int out_index);
        void dropItem(int in_index);

        unsigned char * getRange() const;

        void setEntity(int in_index);
        int getEntity();

        unsigned int getExp() const;
        unsigned int getLvl() const;
        void setExp(const unsigned int in_exp);
        void addExp(const unsigned int in_exp);

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
        bool canDouble(Unit * enemy) const;

        void combatStats();
        Combat_stats getCombatStats();
        unsigned char hit();
        unsigned char dodge();
        unsigned char critical();
        unsigned char favor();
        unsigned char total_might();
        char speed();

        void dies();

        void write(const char * filename);
};

extern std::unordered_map<std::string, Unit> all_units;

void baseUnits();

#endif /* UNIT_HPP */