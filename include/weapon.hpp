#ifndef WEAPON_HPP
#define WEAPON_HPP

#include "shared.hpp"

class Weapon {
    private:
        Weapon_stats stats;
        //hp,str,mag,skl,spd,luck,def,res,con,move
        Unit_stats bonus = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
        Unit_stats malus = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
        long unsigned int effect;
        short unsigned int type;
        short unsigned int effective;
        std::string name;
        std::string description;
        unsigned char id;
    public:
        Weapon();
        Weapon(Weapon_stats in_stats);
        Weapon(std::string in_name, Weapon_stats in_stats);
        Weapon(std::string in_name, short unsigned int in_type, Weapon_stats in_stats);
        Weapon(std::string in_name, short unsigned int in_type, Weapon_stats in_stats, unsigned char in_id);

        void setStats(Weapon_stats in_stats);
        Weapon_stats getStats();
        void setBonus(Unit_stats in_bonus);
        Unit_stats getBonus();
        void setMalus(Unit_stats in_malus);
        Unit_stats getMalus(); //may be uncessesary?
        void setEffect(long unsigned int in_effect);
        long unsigned int getEffect();
        void setEffective(short unsigned int in_effective);
        short unsigned int getEffective();
        void setDescription(std::string in_description);
        std::string getDescription();
        void  setName(std::string in_name);
        std::string getName();
        short unsigned int getType();
        void setType(short unsigned int in_type);

        void write(const char * filename, const char * mode = "a");
};

extern std::vector<Weapon> all_weapons;
extern std::vector<Weapon> loaded_weapons;

void baseWeapons();
void baseWeapons2();
std::vector<Weapon> baseWeapons(std::vector<short int> toload);

#endif /* WEAPON_HPP */