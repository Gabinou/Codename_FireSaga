#ifndef WEAPON_HPP
#define WEAPON_HPP

#include "shared.hpp"

class Weapon {
    private:
        Weapon_stats stats;
        //hp,str,mag,skl,spd,luck,def,res,con,move
        Unit_stats bonus = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
        Unit_stats malus = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
        std::string effect = "None";
        std::string type;
        Weapon_type types; //this one is better
        std::string name;
        std::string effective;
        std::string description;
    public:
        Weapon();
        Weapon(Weapon_stats in_stats);
        Weapon(std::string in_name, Weapon_stats in_stats);
        Weapon(std::string in_name, std::string in_type, Weapon_stats in_stats);

        void setStats(Weapon_stats in_stats);
        Weapon_stats getStats();
        void setBonus(Unit_stats in_bonus);
        Unit_stats getBonus();
        void setMalus(Unit_stats in_malus);
        Unit_stats getMalus(); //may be uncessesary?
        void setEffect(std::string in_effect);
        std::string getEffect();
        void setEffective(std::string in_effective);
        std::string getEffective();
        void setDescription(std::string in_description);
        std::string getDescription();
        std::string getName();
        std::string getType();

        void write(const char * filename, const char * mode = "a");
};

extern std::unordered_map<std::string, Weapon> all_weapons;

void baseWeapons();

#endif /* WEAPON_HPP */