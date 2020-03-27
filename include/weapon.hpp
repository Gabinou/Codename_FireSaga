#ifndef WEAPON_HPP
#define WEAPON_HPP

#include "enums.hpp"
#include "structs.hpp"
#include "physfs.h"
#include "tinyxml2.h"
#include "filesystem.hpp"
#include "utilities.hpp"
#include "string.h"
#include <string>

class Weapon {
    private:
        Weapon_stats stats = {0, 0, 0, 0, 0, 0, 0, 0, 0, {0, 0}, {0, 0}, 0, 0};
        // Pmight, Mmight, hit, dodge, crit, favor, wgt, uses, prof, range, hand, dmg_type, cost
        Unit_stats bonus_stats = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
        Unit_stats malus_stats = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
        long unsigned int effect = 0;
        short unsigned int type = 0;
        short unsigned int effective = 0; // is a unit type.
        std::string name = "";
        std::string description = "";
        short int id = 0;
    public:
        Weapon();

        Weapon(short unsigned int in_type, Weapon_stats in_stats, unsigned char in_id);

        void setStats(Weapon_stats in_stats);
        Weapon_stats getStats();
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
        void  setName(std::string in_name);
        std::string getName();
        short unsigned int getType();
        void setType(short unsigned int in_type);

        void write(const char * filename, const char * mode = "a");
        void writeXML(const char * filename, const bool append = false);
        void readXML(const char * filename);
};

extern Inventory_item convoy[200];
extern std::vector<Weapon> all_weapons;
extern std::vector<Weapon> loaded_weapons;

void baseWeapons();
void testXMLWeapons();
std::vector<Weapon> baseWeapons(std::vector<short int> toload);

#endif /* WEAPON_HPP */