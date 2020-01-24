#ifndef UNIT_HPP
#define UNIT_HPP

#include "ECS.hpp"
#include "game.hpp"
#include "shared.hpp"
#include "weapon.hpp"
#include "SDL2/SDL.h"
#include <stdio.h>

// extern std::unordered_map<std::string, int> all_units;

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
        std::string army;
        Inventory_item equipment[7], weapons[3], items[3];
    public:

        Unit(const std::string in_name, const std::string in_class, const Unit_stats in_bases);

        Unit(const std::string in_name, const Unit_stats in_bases);

        Unit(const Unit_stats in_bases);

        void equipL(const char index);

        void unequipL();

        void equipR(const char index);

        void unequipR();

        void equip(const unsigned int index, const std::string hand = "right");

        void unequip(const std::string hand = "right");

        void take_damage(const unsigned char damage);

        std::string getMvttype();

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

        unsigned char combat_damage(const Entity & enemy, const bool critical);

        std::string getName();

        void setName(const std::string in_name);

        void setName(const char in_name);

        void setArmy(const std::string in_army);

        void setArmy(const char in_army);

        unsigned char avoid();

        unsigned char critical();

        bool retaliation(const Entity & enemy);

        bool combat_double(Entity & enemy) const;

        unsigned char wpn_weighed_down() const;

        unsigned char combat_critical(const Entity & enemy);

        unsigned char favor();

        unsigned char accuracy();

        unsigned char combat_hit(const Entity & enemy);

        unsigned char attack(const Entity & enemy);

        void combat(Entity & enemy);

        void write(const char * filename);
};

#endif /* UNIT_HPP */