#ifndef UNITCOMPONENT_HPP
#define UNITCOMPONENT_HPP

#include "ECS.hpp"
#include "game.hpp"
#include "shared.hpp"
#include "weapon.hpp"
#include "SDL2/SDL.h"
#include <stdio.h>

extern std::unordered_map<std::string, int> all_units;

class UnitComponent : public Component {
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

        UnitComponent() = default;

        UnitComponent(const std::string in_name, const std::string in_class, const Unit_stats in_bases) {
            base_stats = in_bases;
            current_stats = in_bases;
            class_name = in_class;

            if ((class_name == "Fencer") || (class_name == "Mousquetaire") || (class_name == "Duelist") || (class_name == "Lord") || (class_name == "Duke") || (class_name == "Archer") || (class_name == "Marksman") || (class_name == "Mercenary") || (class_name == "Hero") || (class_name == "Trooper")) {
                mvt_type = "foot_slow";
            }

            if ((class_name == "Pickpocket") || (class_name == "Thief") || (class_name == "Assassin") || (class_name == "Demon")) {
                mvt_type = "foot_fast";
            }

            if ((class_name == "Mage") || (class_name == "Battlemage") || (class_name == "Sage") || (class_name == "Oracle") || (class_name == "Priest") || (class_name == "Cleric") || (class_name == "Bishop") || (class_name == "Incarnate") || (class_name == "Possessed")) {
                mvt_type = "mages";
            }

            if ((class_name == "Cavalier") || (class_name == "Archer rider") || (class_name == "Marksman rider") || (class_name == "Lord rider") || (class_name == "Duke rider")) {
                mvt_type = "riders_slow";
            }

            if ((class_name == "Paladin") || (class_name == "Troubadour")) {
                mvt_type = "riders_fast";
            }

            if ((class_name == "Pegasus knight") || (class_name == "Angel")) {
                mvt_type = "fliers";
            }

            if ((class_name == "Knight") || (class_name == "General")) {
                mvt_type = "armors";
            }

            if ((class_name == "Corsair") || (class_name == "Viking")) {
                mvt_type = "pirates";
            }

            if ((class_name == "Bandit") || (class_name == "Ravager")) {
                mvt_type = "bandits";
            }

            name = in_name;
        }

        UnitComponent(const std::string in_name, const Unit_stats in_bases) {
            base_stats = in_bases;
            current_stats = in_bases;
            name = in_name;
        }

        UnitComponent(const Unit_stats in_bases) {
            base_stats = in_bases;
            current_stats = in_bases;
        }

        void equipL(const char index) {
            equipped.left = index;
        }

        void unequipL() {
            equipped.left = -1;
        }

        void equipR(const char index) {
            equipped.right = index;
        }

        void unequipR() {
            equipped.right = -1;
        }

        void equip(const unsigned int index, const std::string hand = "right") {
            if (hand == "left") {
                equipped.left = index;
            } else {
                equipped.right = index;
            }
        }

        void unequip(const std::string hand = "right") {
            if (hand == "left") {
                equipped.left = -1;
            } else {
                equipped.right = -1;
            }
        }

        void take_damage(const unsigned char damage) {
            printf("%s takes %d damage \n", name, damage);
            current_hp = std::max(0, current_hp - damage);

            if (current_hp == 0) {death();};
        }

        std::string getMvttype() {
            printf("insinde unitcomponent%s\n", mvt_type.c_str());
            return (mvt_type);
        }

        void heal(const unsigned char healing) {
            printf("%s gets healed for %d\n", name, healing);
            current_hp = std::min(current_hp + healing, (int) current_stats.hp);
        }

        unsigned char getHp() const {
            return (current_hp);
        }

        unsigned int getLvl() const {
            return (ceil(current_hp / 100));
        }

        unsigned int getExp() const {
            return (exp);
        }

        unsigned char * getRange() const {
            unsigned char * temp;
            static unsigned char range[2];
            unsigned char * right_range;

            if (equipped.left > 0) {
                temp = all_weapons[equipment[equipped.left].name].getStats().range;
                range[0] = temp[0];
                range[1] = temp[1];
            } else {
                range[0] = 1;
                range[1] = 1;
            }

            if (equipped.right > 0) {
                temp = all_weapons[equipment[equipped.left].name].getStats().range;
                right_range[0] = temp[0];
                right_range[1] = temp[1];
                range[0] = std::min(right_range[0], range[0]);
                range[1] = std::max(right_range[1], range[1]);
            }

            if ((equipped.left < 0) && (equipped.right < 0)) {
                range[0] = 0;
                range[1] = 0;
            }

            range[0] = 5;
            range[1] = 10;

            return (range);
        }


        void setExp(const unsigned int in_exp) {
            exp = in_exp;
        }

        void setHp(const unsigned char in_hp) {
            current_hp = in_hp;
        }

        void death() {
            printf("%s is dead.\n", name);
        }

        void setBonus(const Unit_stats in_stats) {
            bonus_stats = in_stats; // tested, works fine.
        }

        void setMalus(const Unit_stats in_stats) {
            malus_stats = in_stats; // tested, works fine.
        }

        void setCaps(const Unit_stats in_stats) {
            caps_stats = in_stats; // tested, works fine.
        }

        void setStats(const Unit_stats in_stats) {
            current_stats = in_stats; // tested, works fine.
            current_hp = current_stats.hp;
        }

        Unit_stats getStats() {
            return (base_stats);
        }

        void setBases(const Unit_stats in_stats) {
            base_stats = in_stats; // tested, works fine.
            current_stats = in_stats; // tested, works fine.
            current_hp = base_stats.hp;
        }

        void setGrowths(const Unit_stats in_growths) {
            growths = in_growths; // tested, works fine.
        }

        unsigned char attack_damage() {
            unsigned char unit_power = 0;
            unsigned char wpn_dmg;

            if (temp_wpn.dmg_type == 0) { // Physical dmg.
                wpn_dmg = temp_wpn.Pmight;
                unit_power = current_stats.str;
            } else { // Magical dmg.
                wpn_dmg = temp_wpn.Mmight;
                unit_power = current_stats.mag;
            };

            int attack_damage = wpn_dmg + unit_power;

            return (attack_damage);
        }

        unsigned char combat_damage(const Entity & enemy, const bool critical) {
            unsigned char terrain_def = 0;
            unsigned char enemy_def = 0 ;
            unsigned char unit_power = 0;
            unsigned char wpn_dmg;
            unsigned char crit_factor = 1;

            if (temp_wpn.dmg_type == 0) {
                // Physical attack_damage.
                wpn_dmg = temp_wpn.Pmight;
                unit_power = current_stats.str;
                enemy_def = enemy.getComponent<UnitComponent>().current_stats.def;
            } else {
                // Magical attack_damage.
                wpn_dmg = temp_wpn.Mmight;
                unit_power = current_stats.mag;
                enemy_def = enemy.getComponent<UnitComponent>().current_stats.res;
            };

            if (critical) {crit_factor = 3;};

            unsigned char attack_damage = crit_factor * (std::max(wpn_dmg + unit_power - enemy_def - terrain_def, 0)); // Modern FE style. for crit_factor = 3

            // int attack_damage = crit_factor*(wpn_dmg + unit_power) - enemy_def - terrain_def);  // FE4-FE5 style. for crit_factor = 2

            return (attack_damage);
        }

        std::string getName() {
            return (name);
        }

        void setName(const std::string in_name) {
            name = in_name;
        }

        void setName(const char in_name) {
            name = in_name;
        }

        void setArmy(const std::string in_army) {
            army = in_army;
        }

        void setArmy(const char in_army) {
            army = in_army;
        }

        unsigned char avoid() {
            unsigned char supports = 0;
            unsigned char terrain_avoid = 0;
            unsigned char unit_avoid = current_stats.dex * 2 + current_stats.luck;
            unsigned char avoid = terrain_avoid + unit_avoid + supports;
            return (avoid);
        }

        unsigned char critical() {
            unsigned char supports = 0 ;
            unsigned char unit_skill = 0;
            unsigned char critical = temp_wpn.combat.crit + unit_skill + supports;
            return (critical);
        }

        bool retaliation(const Entity & enemy) {
            int * unit_position;
            int * enemy_position;
            unit_position = entity->getComponent<PositionComponent>().getPos();
            enemy_position = enemy.getComponent<PositionComponent>().getPos();
            unsigned char distance = std::abs(enemy_position[0] - unit_position[0]) + std::abs(enemy_position[1] - unit_position[1]);
            bool retaliates = false;

            for (int i = 0; i < 3; i++) {
                if ((distance >= temp_wpn.range[0]) && (distance <= temp_wpn.range[1])) {
                    retaliates = 1;
                }
            }

            return (retaliates);
        }

        bool combat_double(Entity & enemy) const {
            unsigned char enemy_speed = enemy.getComponent<UnitComponent>().current_stats.agi;
            bool doubles = ((current_stats.agi - wpn_weighed_down() - enemy_speed) > 4);
            return (doubles);
        }

        unsigned char wpn_weighed_down() const {
            //*DESIGN QUESTION* What should be the influence of weapons?
            // Average of Con and Str? Con+Str/2?
            return (std::max(temp_wpn.wgt - current_stats.con, 0));
        }

        unsigned char combat_critical(const Entity & enemy) {
            unsigned char supports = 0;
            unsigned char unit_skill = 0;
            unsigned char enemy_favor = enemy.getComponent<UnitComponent>().favor();
            unsigned char critical = std::max(0, temp_wpn.combat.crit + unit_skill + supports - enemy_favor);
            return (critical);
        }

        unsigned char favor() {
            unsigned char supports = 0 ;
            unsigned char favor = (ceil(current_stats.luck / 2.)) + supports;
            return (favor);
        }

        unsigned char accuracy() {
            //*DESIGN QUESTION* In vesteria saga, dex*3.
            unsigned char supports = 0;
            unsigned char unit_acc = current_stats.dex * 3 + current_stats.luck;
            unsigned char accuracy = temp_wpn.combat.hit + unit_acc + supports;
            return (accuracy);
        }

        unsigned char combat_hit(const Entity & enemy) {
            int enemy_avoid = enemy.getComponent<UnitComponent>().avoid();
            unsigned char hit = std::max(0, accuracy() - enemy_avoid);
            return (hit);
        }

        unsigned char attack(const Entity & enemy) {
            printf("%s attacks %s\n", name, enemy.getComponent<UnitComponent>().getName());
            bool unit_hits = (getRN() < combat_hit(enemy));
            bool unit_crits = (getRN() < combat_critical(enemy));
            /* *DESIGN QUESTION* Should a random number always be rolled for crits, even if the hit doesn't connect?
            * I think so. Always same number of RN rolled.
            * But what about crit animations? Should crit animations be shown to miss? Fire Emblem thinks not. Me too.
            */
            // unit.take_damage( -= combat_damage(enemy, unit_crits);
            enemy.getComponent<UnitComponent>().take_damage(combat_damage(enemy, unit_crits));
            return (combat_damage(enemy, unit_crits));
            // return (1);
        }

        void combat(Entity & enemy) {
            printf("%s fights %s\n", name, enemy.getComponent<UnitComponent>().getName());
            bool unit_doubles = combat_double(enemy);
            bool enemy_retaliates = enemy.getComponent<UnitComponent>().retaliation(enemy);
            bool enemy_doubles = 0;
            attack(enemy);

            if (enemy_retaliates) {
                enemy.getComponent<UnitComponent>().attack(*entity);
                printf("enemy %s retaliates %d\n", enemy.getComponent<UnitComponent>().getName(), enemy_retaliates);
                enemy_doubles = enemy.getComponent<UnitComponent>().combat_double(*entity);
            };

            if (unit_doubles) {
                printf("%s doubles\n", name);
                attack(enemy);
            };

            if (enemy_doubles) {
                printf("%s doubles\n", enemy.getComponent<UnitComponent>().getName());
                enemy.getComponent<UnitComponent>().attack(*entity);
            };
        }

        void write(const char * filename) {
            FILE * fp;
            fp = fopen(filename, "w+");
            fprintf(fp, name.c_str());
            fprintf(fp, "\n");
            fprintf(fp, "%s", class_name);
            fprintf(fp, "Stats, HP, Str, Mag, Skl, Spd, Luck, Def, Res, Con, Move\n");
            fprintf(fp, "Base stats,\t%d,\t%d,\t%d,\t%d,\t%d,\t%d,\t%d,\t%d,\t%d,\t%d\n", base_stats.hp, base_stats.str, base_stats.mag, base_stats.dex, base_stats.agi, base_stats.luck, base_stats.def, base_stats.res, base_stats.con, base_stats.move);
            fprintf(fp, "Growths,\t%d,\t%d,\t%d,\t%d,\t%d,\t%d,\t%d,\t%d,\t%d,\t%d\n", growths.hp, growths.str, growths.mag, growths.dex, growths.agi, growths.luck, growths.def, growths.res, growths.con, growths.move);
            fprintf(fp, "Caps,\t\t%d,\t%d,\t%d,\t%d,\t%d,\t%d,\t%d,\t%d,\t%d,\t%d\n", caps_stats.hp, caps_stats.str, caps_stats.mag, caps_stats.dex, caps_stats.agi, caps_stats.luck, caps_stats.def, caps_stats.res, caps_stats.con, caps_stats.move);
            fclose(fp);
        }

};

void baseUnits(Manager * manager) {
    printf("Making base units \n");
    manager->addEntity();
    Unit_stats temp;
    //hp,str,mag,skl,spd,luck,def,res,con,move
    temp = {17,  6,  2,  7,  7,   7,  4,  5,  6, 5};
    manager->getEntities().back()->addComponent<UnitComponent>("Main", "Mercenary", temp);
    temp = {48, 14, 25, 32, 34,  28, 19, 40, 15, 0};
    manager->getEntities().back()->getComponent<UnitComponent>().setCaps(temp);
    temp = {60, 50, 20, 60, 70,  40, 30, 20,  10, 0};
    manager->getEntities().back()->getComponent<UnitComponent>().setGrowths(temp);
    manager->getEntities().back()->getComponent<UnitComponent>().setExp(0);
    manager->getEntities().back()->getComponent<UnitComponent>().write("unit_test.txt");
    all_units["Main"] = manager->getEntities().size() - 1;

    manager->addEntity();
    temp = {18,  6,  2,  7,  7,   7,  4,  5,  6, 7};
    manager->getEntities().back()->addComponent<UnitComponent>("Reliable", "Cavalier", temp);
    temp = {48, 14, 25, 32, 34,  28, 19, 40, 15};
    manager->getEntities().back()->getComponent<UnitComponent>().setCaps(temp);
    temp = {60, 50, 20, 60, 70,  40, 30, 20,  10, 0};
    manager->getEntities().back()->getComponent<UnitComponent>().setGrowths(temp);
    manager->getEntities().back()->getComponent<UnitComponent>().setExp(100);
    all_units["Reliable"] = manager->getEntities().size() - 1;

    manager->addEntity();
    temp = {19,  6,  2,  7,  7,   7,  4,  5,  6,  7};
    manager->getEntities().back()->addComponent<UnitComponent>("Coward", "Cavalier", temp);
    temp = {48, 14, 25, 32, 34,  28, 19, 40, 15};
    manager->getEntities().back()->getComponent<UnitComponent>().setCaps(temp);
    temp = {60, 50, 20, 60, 70,  40, 30, 20,  10, 0};
    manager->getEntities().back()->getComponent<UnitComponent>().setGrowths(temp);
    manager->getEntities().back()->getComponent<UnitComponent>().setExp(200);
    all_units["Coward"] = manager->getEntities().size() - 1;

    manager->addEntity();
    temp = {20,  6,  2,  7,  7,   7,  4,  5,  6,  6};
    manager->getEntities().back()->addComponent<UnitComponent>("1H Jaigen", "Fencermaster", temp);
    temp = {48, 14, 25, 32, 34,  28, 19, 40, 15};
    manager->getEntities().back()->getComponent<UnitComponent>().setCaps(temp);
    temp = {60, 50, 20, 60, 70,  40, 30, 20,  10, 0};
    manager->getEntities().back()->getComponent<UnitComponent>().setGrowths(temp);
    manager->getEntities().back()->getComponent<UnitComponent>().setExp(2200);
    all_units["1H Jaigen"] = manager->getEntities().size() - 1;

    manager->addEntity();
    temp = {14,  6,  2,  7,  7,   7,  4,  5,  6,  5};
    manager->getEntities().back()->addComponent<UnitComponent>("Lovely", "Priestess", temp);
    temp = {48, 14, 25, 32, 34,  28, 19, 40, 15};
    manager->getEntities().back()->getComponent<UnitComponent>().setCaps(temp);
    temp = {60, 50, 20, 60, 70,  40, 30, 20,  10, 0};
    manager->getEntities().back()->getComponent<UnitComponent>().setGrowths(temp);
    manager->getEntities().back()->getComponent<UnitComponent>().setExp(100);
    all_units["Lovely"] = manager->getEntities().size() - 1;

    manager->addEntity();
    temp = {16,  6,  2,  7,  7,   7,  4,  5,  6,  6};
    manager->getEntities().back()->addComponent<UnitComponent>("Hottie", "Thief", temp);
    temp = {48, 14, 25, 32, 34,  28, 19, 40, 15};
    manager->getEntities().back()->getComponent<UnitComponent>().setCaps(temp);
    temp = {60, 50, 20, 60, 70,  40, 30, 20,  10, 0};
    manager->getEntities().back()->getComponent<UnitComponent>().setGrowths(temp);
    manager->getEntities().back()->getComponent<UnitComponent>().setExp(2200);
    all_units["Hottie"] = manager->getEntities().size() - 1;

    manager->addEntity();
    temp = {22,  4,  5,  7,  6,   8,  4,  6,  5, 5}; // 4 or 5?
    manager->getEntities().back()->addComponent<UnitComponent>("Servil", "Knight", temp);
    temp = {48, 14, 25, 32, 34,  28, 19, 40, 15};
    manager->getEntities().back()->getComponent<UnitComponent>().setCaps(temp);
    temp = {60, 50, 20, 60, 70,  40, 30, 20,  10, 0};
    manager->getEntities().back()->getComponent<UnitComponent>().setGrowths(temp);
    manager->getEntities().back()->getComponent<UnitComponent>().setExp(500);
    all_units["Servil"] = manager->getEntities().size() - 1;

    manager->addEntity();
    temp = {34,  4,  5,  7,  6,   8,  4,  6,  5, 5};
    manager->getEntities().back()->addComponent<UnitComponent>("Pérignon", "Mage", temp);
    temp = {48, 14, 25, 32, 34,  28, 19, 40, 15};
    manager->getEntities().back()->getComponent<UnitComponent>().setCaps(temp);
    temp = {60, 50, 20, 60, 70,  40, 30, 20,  10, 0};
    manager->getEntities().back()->getComponent<UnitComponent>().setGrowths(temp);
    manager->getEntities().back()->getComponent<UnitComponent>().setExp(1200);
    all_units["Pérignon"] = manager->getEntities().size() - 1;

    manager->addEntity();
    temp = {15,  4,  5,  7,  6,   8,  4,  6,  5, 5};
    manager->getEntities().back()->addComponent<UnitComponent>("Poet", "Mage", temp);
    temp = {48, 14, 25, 32, 34,  28, 19, 40, 15};
    manager->getEntities().back()->getComponent<UnitComponent>().setCaps(temp);
    temp = {60, 50, 20, 60, 70,  40, 30, 20,  10, 0};
    manager->getEntities().back()->getComponent<UnitComponent>().setGrowths(temp);
    manager->getEntities().back()->getComponent<UnitComponent>().setExp(400);
    all_units["Poet"] = manager->getEntities().size() - 1;

    manager->addEntity();
    temp = {15,  4,  5,  7,  6,   8,  4,  6,  5, 5};
    manager->getEntities().back()->addComponent<UnitComponent>("Silou", "Mage", temp);
    temp = {48, 14, 25, 32, 34,  28, 19, 40, 15};
    manager->getEntities().back()->getComponent<UnitComponent>().setCaps(temp);
    temp = {60, 50, 20, 60, 70,  40, 30, 20,  10, 0};
    manager->getEntities().back()->getComponent<UnitComponent>().setGrowths(temp);
    manager->getEntities().back()->getComponent<UnitComponent>().setExp(400);
    all_units["Silou"] = manager->getEntities().size() - 1;

    manager->addEntity();
    temp = {15,  4,  5,  7,  6,   8,  4,  6,  5, 5};
    manager->getEntities().back()->addComponent<UnitComponent>("Poet", "Mage", temp);
    temp = {48, 14, 25, 32, 34,  28, 19, 40, 15};
    manager->getEntities().back()->getComponent<UnitComponent>().setCaps(temp);
    temp = {60, 50, 20, 60, 70,  40, 30, 20,  10, 0};
    manager->getEntities().back()->getComponent<UnitComponent>().setGrowths(temp);
    manager->getEntities().back()->getComponent<UnitComponent>().setExp(400);
    all_units["Arm Thief"] = manager->getEntities().size() - 1;

    manager->addEntity();
    temp = {15,  4,  5,  7,  6,   8,  4,  6,  5, 5};
    manager->getEntities().back()->addComponent<UnitComponent>("Poet", "Mage", temp);
    temp = {48, 14, 25, 32, 34,  28, 19, 40, 15};
    manager->getEntities().back()->getComponent<UnitComponent>().setCaps(temp);
    temp = {60, 50, 20, 60, 70,  40, 30, 20,  10, 0};
    manager->getEntities().back()->getComponent<UnitComponent>().setGrowths(temp);
    manager->getEntities().back()->getComponent<UnitComponent>().setExp(400);
    all_units["Mage2"] = manager->getEntities().size() - 1;

    manager->addEntity();
    temp = {15,  4,  5,  7,  6,   8,  4,  6,  5, 5};
    manager->getEntities().back()->addComponent<UnitComponent>("Silou", "Mage", temp);
    temp = {48, 14, 25, 32, 34,  28, 19, 40, 15};
    manager->getEntities().back()->getComponent<UnitComponent>().setCaps(temp);
    temp = {60, 50, 20, 60, 70,  40, 30, 20,  10, 0};
    manager->getEntities().back()->getComponent<UnitComponent>().setGrowths(temp);
    manager->getEntities().back()->getComponent<UnitComponent>().setExp(400);
    all_units["Mage2"] = manager->getEntities().size() - 1;

    manager->addEntity();
    temp = {15,  4,  5,  7,  6,   8,  4,  6,  5, 5};
    manager->getEntities().back()->addComponent<UnitComponent>("Silou", "Mage", temp);
    temp = {48, 14, 25, 32, 34,  28, 19, 40, 15};
    manager->getEntities().back()->getComponent<UnitComponent>().setCaps(temp);
    temp = {60, 50, 20, 60, 70,  40, 30, 20,  10, 0};
    manager->getEntities().back()->getComponent<UnitComponent>().setGrowths(temp);
    manager->getEntities().back()->getComponent<UnitComponent>().setExp(400);
    all_units["Mage3"] = manager->getEntities().size() - 1;
}

#endif /* UNITCOMPONENT_HPP */