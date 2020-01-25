#ifndef UNIT_HPP
#define UNIT_HPP

#include "shared.hpp"
#include "weapon.hpp"

class Unit {
    private:
        Equipped equipped; // these are indices. -1 means no equipment.
        Weapon_stats temp_wpn;
        Unit_stats base_stats;
        Unit_state state;
        Unit_stats bonus_stats;
        Unit_stats caps_stats;
        Unit_stats malus_stats;
        Unit_stats current_stats;
        std::vector<Unit_stats> grown_stats;
        // SKILLS
        // SUPPORTS/BONDS
        Unit_stats growths;
        unsigned char current_hp;
        unsigned int exp;
        std::string name;
        std::string class_name;
        std::string mvt_type;
        std::string army; //affilistion?
        bool sex; // 0:F, 1:M
        Inventory_item equipment[7], weapons[3], items[3];
    public:

        Unit();

        Unit(const std::string in_name, const std::string in_class, const Unit_stats in_bases);

        Unit(const std::string in_name, const Unit_stats in_bases);

        Unit(const Unit_stats in_bases);

        void setMvttype(const std::string in_class);
        std::string getMvttype();

        void equipL(const char index);
        void equipR(const char index);
        void unequipR();
        void unequipL();
        void equip(const unsigned int index, const std::string hand = "right");
        void unequip(const std::string hand = "right");

        void take_damage(const unsigned char damage);

        void heal(const unsigned char healing);

        unsigned char getHp() const;

        unsigned int getLvl() const;

        unsigned int getExp() const;

        unsigned char * getRange() const;

        void setExp(const unsigned int in_exp);

        void setHp(const unsigned char in_hp);

        void death();

        void setBonus(const Unit_stats in_stats);

        void setMalus(const Unit_stats in_stats);

        void setCaps(const Unit_stats in_stats);

        void setStats(const Unit_stats in_stats);

        Unit_stats getStats();

        void setBases(const Unit_stats in_stats);

        void setGrowths(const Unit_stats in_growths);

        unsigned char attack_damage();

        unsigned char combat_damage(const bool critical);

        std::string getName();

        void setName(const std::string in_name);

        void setName(const char in_name);

        void setArmy(const std::string in_army);

        void setArmy(const char in_army);

        unsigned char avoid();

        unsigned char critical();

        bool retaliation();

        bool combat_double() const;

        unsigned char wpn_weighed_down() const;

        unsigned char combat_critical();

        unsigned char favor();

        unsigned char accuracy();

        unsigned char combat_hit();

        unsigned char attack();

        void combat();

        void write(const char * filename);
};

extern std::unordered_map<std::string, Unit> all_units;

void baseUnits();

#endif /* UNIT_HPP */