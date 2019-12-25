#ifndef UNITCOMPONENT_HPP
#define UNITCOMPONENT_HPP

#include "ECS.hpp"
#include "game.hpp"
#include "shared.hpp"
#include <stdio.h>
#include "SDL2/SDL.h"

extern std::unordered_map<string, int> all_units;

class UnitComponent : public Component {
    private:
        Equipped equipped;
        Weapon temp_wpn;
        Unit_stats base_stats;
        Unit_stats bonus_stats;
        Unit_stats caps_stats;
        Unit_stats malus_stats;
        Unit_stats current_stats;
        std::vector<Unit_stats> grown_stats;
        // SKILLS
        // SUPPORTS
        Unit_stats growths;
        unsigned char current_hp;
        unsigned int exp;
        std::string name;
        std::string class_name;
        struct ::inventory_item equipment[7], weapons[3], items[3];
    public:

        UnitComponent() = default;

        UnitComponent(std::string in_name, std::string in_class, Unit_stats in_bases) {
            base_stats = in_bases;
            class_name = in_class;
            name = in_name;
        }

        UnitComponent(std::string in_name, Unit_stats in_bases) {
            base_stats = in_bases;
            name = in_name;
        }

        UnitComponent(Unit_stats in_bases) {
            base_stats = in_bases;
        }

        void take_damage(const unsigned char damage) {
            printf("%s takes %d damage \n", name, damage);
            current_hp = std::max(0, current_hp - damage);

            if (current_hp == 0) {death();};
        }

        void heal(const unsigned char healing) {
            printf("%s gets healed for %d\n", name, healing);
            current_hp = std::min(current_hp + healing, (int) current_stats.hp);
        }

        unsigned char get_hp() const {
            return (current_hp);
        }

        unsigned int get_lvl() const {
            return (ceil(current_hp / 100));
        }

        unsigned int get_exp() const {
            return (exp);
        }

        void set_exp(const unsigned int in_exp) {
            exp = in_exp;
        }

        void set_hp(const unsigned char in_hp) {
            current_hp = in_hp;
        }

        void death() {
            printf("%s is dead.\n", name);
        }

        void set_bonus(Unit_stats in_stats) {
            bonus_stats = in_stats; // tested, works fine.
        }

        void set_malus(Unit_stats in_stats) {
            malus_stats = in_stats; // tested, works fine.
        }

        void set_caps(Unit_stats in_stats) {
            caps_stats = in_stats; // tested, works fine.
        }

        void set_stats(Unit_stats in_stats) {
            current_stats = in_stats; // tested, works fine.
            current_hp = current_stats.hp;
        }

        void set_bases(Unit_stats in_stats) {
            base_stats = in_stats; // tested, works fine.
            current_stats = in_stats; // tested, works fine.
            current_hp = base_stats.hp;
        }

        void set_growths(Unit_stats in_growths) {
            growths = in_growths; // tested, works fine.
        }

        unsigned char attack_damage() {
            unsigned char unit_power = 0;
            unsigned char wpn_dmg = temp_wpn.dmg;

            if (temp_wpn.dmg_type == 0) {
                // Physical attack_damage.
                unit_power = current_stats.str;
            } else {
                // Magical attack_damage.
                unit_power = current_stats.mag;
            };

            int attack_damage = wpn_dmg + unit_power;

            return (attack_damage);
        }

        unsigned char combat_damage(Entity & enemy, const bool critical) {
            unsigned char terrain_def = 0;
            unsigned char enemy_def = 0 ;
            unsigned char unit_power = 0;
            unsigned char wpn_dmg = temp_wpn.dmg;
            unsigned char crit_factor = 1;

            if (temp_wpn.dmg_type == 0) {
                // Physical attack_damage.
                unit_power = current_stats.str;
                enemy_def = enemy.getComponent<UnitComponent>().current_stats.def;
            } else {
                // Magical attack_damage.
                unit_power = current_stats.mag;
                enemy_def = enemy.getComponent<UnitComponent>().current_stats.res;
            };

            if (critical) {crit_factor = 3;};

            unsigned char attack_damage = crit_factor * (std::max(wpn_dmg + unit_power - enemy_def - terrain_def, 0)); // Modern FE style. for crit_factor = 3

            // int attack_damage = crit_factor*(wpn_dmg + unit_power) - enemy_def - terrain_def);  // FE4-FE5 style. for crit_factor = 2

            return (attack_damage);
        }

        std::string get_name() {
            return (name);
        }

        void set_name(const std::string in_name) {
            name = in_name;
        }
        void set_name(const char in_name) {
            name = in_name;
        }

        unsigned char avoid() {
            unsigned char supports = 0;
            unsigned char terrain_avoid = 0;
            unsigned char unit_avoid = current_stats.skl * 2 + current_stats.luck;
            unsigned char avoid = terrain_avoid + unit_avoid + supports;
            return (avoid);
        }

        unsigned char critical() {
            unsigned char supports = 0 ;
            unsigned char unit_skill = 0;
            unsigned char critical = temp_wpn.crit + unit_skill + supports;
            return (critical);
        }

        bool retaliation(Entity & enemy) {
            // int unit_position[2] = {entity->getComponent<PositionComponent>().getPos()[0], entity->getComponent<PositionComponent>().getPos()[1]};
            int * unit_position;
            int * enemy_position;
            unit_position = entity->getComponent<PositionComponent>().getPos();
            enemy_position = enemy.getComponent<PositionComponent>().getPos();
            unsigned char distance = std::abs(enemy_position[0] - unit_position[0]) + std::abs(enemy_position[1] - unit_position[1]);
            // printf("Distance %d \n", distance);
            bool retaliates = false;

            for (int i = 0; i < 3; i++) {
                if ((distance >= temp_wpn.range[0]) && (distance <= temp_wpn.range[1])) {
                    retaliates = 1;
                }
            }

            return (retaliates);
        }

        bool combat_double(Entity & enemy) const {
            unsigned char enemy_speed = enemy.getComponent<UnitComponent>().current_stats.spd;
            bool doubles = ((current_stats.spd - wpn_weighed_down() - enemy_speed) > 4);
            return (doubles);
        }

        unsigned char wpn_weighed_down() const {
            //*DESIGN QUESTION* What should be the influence of weapons?
            // Average of Con and Str?
            return (std::max(temp_wpn.wgt - current_stats.con, 0));
        }

        unsigned char combat_critical(Entity & enemy) {
            unsigned char supports = 0;
            unsigned char unit_skill = 0;
            unsigned char enemy_favor = enemy.getComponent<UnitComponent>().favor();
            unsigned char critical = std::max(0, temp_wpn.crit + unit_skill + supports - enemy_favor);
            return (critical);
        }

        unsigned char favor() {
            unsigned char supports = 0 ;
            unsigned char favor = (ceil(current_stats.luck / 2.)) + supports;
            return (favor);
        }

        unsigned char accuracy() {
            unsigned char supports = 0;
            unsigned char unit_acc = current_stats.skl * 2 + current_stats.luck;
            unsigned char accuracy = temp_wpn.hit + unit_acc + supports;
            return (accuracy);
        }

        unsigned char combat_hit(Entity & enemy) {
            int enemy_avoid = enemy.getComponent<UnitComponent>().avoid();
            unsigned char hit = std::max(0, accuracy() - enemy_avoid);
            return (hit);
        }

        unsigned char attack(Entity & enemy) {
            printf("%s attacks %s\n", name, enemy.getComponent<UnitComponent>().get_name());
            bool unit_hits = (get_rand() < combat_hit(enemy));
            bool unit_crits = (get_rand() < combat_critical(enemy));
            // printf("%s crits %d \n", name, unit_crits);
            // printf("%d crit chance \n", combat_critical(enemy));
            /* *DESIGN QUESTION* Should a random number always be rolled for crits, even if the hit doesn't connect?
            * I think so. Always same number of rand rolled.
            * But what about crit animations? Should crit animations be shown to miss? Fire Emblem thinks not. Me too.
            */
            // unit.take_damage( -= combat_damage(enemy, unit_crits);
            enemy.getComponent<UnitComponent>().take_damage(combat_damage(enemy, unit_crits));
            return (combat_damage(enemy, unit_crits));
            // return (1);
        }

        void combat(Entity & enemy) {
            printf("%s fights %s\n", name, enemy.getComponent<UnitComponent>().get_name());
            bool unit_doubles = combat_double(enemy);
            bool enemy_retaliates = enemy.getComponent<UnitComponent>().retaliation(enemy);
            bool enemy_doubles = 0;
            attack(enemy);

            if (enemy_retaliates) {
                enemy.getComponent<UnitComponent>().attack(*entity);
                printf("enemy %s retaliates %d\n", enemy.getComponent<UnitComponent>().get_name(), enemy_retaliates);
                enemy_doubles = enemy.getComponent<UnitComponent>().combat_double(*entity);
            };

            if (unit_doubles) {
                printf("%s doubles\n", name);
                attack(enemy);
            };

            if (enemy_doubles) {
                printf("%s doubles\n", enemy.getComponent<UnitComponent>().get_name());
                enemy.getComponent<UnitComponent>().attack(*entity);
            };
        }

        // void write(std::string filename) {
        //     FILE * fp;
        //     fp = fopen(filename.c_str(), "w+");
        //     fprintf(fp, name);
        //     fprintf(fp, "Base stats, %d,", base_stats.hp);
        //     fclose(fp);
        // }

        void write(const char * filename) {
            FILE * fp;
            fp = fopen(filename, "w+");
            fprintf(fp, name.c_str());
            fprintf(fp, "\n");
            fprintf(fp, "%s", class_name);
            fprintf(fp, "Stats, HP, Str, Mag, Skl, Spd, Luck, Def, Res, Con, Move\n");
            fprintf(fp, "Base stats,\t%d,\t%d,\t%d,\t%d,\t%d,\t%d,\t%d,\t%d,\t%d,\t%d\n", base_stats.hp, base_stats.str, base_stats.mag, base_stats.skl, base_stats.spd, base_stats.luck, base_stats.def, base_stats.res, base_stats.con, base_stats.move);
            fprintf(fp, "Growths,\t%d,\t%d,\t%d,\t%d,\t%d,\t%d,\t%d,\t%d,\t%d,\t%d\n", growths.hp, growths.str, growths.mag, growths.skl, growths.spd, growths.luck, growths.def, growths.res, growths.con, growths.move);
            fprintf(fp, "Caps,\t\t%d,\t%d,\t%d,\t%d,\t%d,\t%d,\t%d,\t%d,\t%d,\t%d\n", caps_stats.hp, caps_stats.str, caps_stats.mag, caps_stats.skl, caps_stats.spd, caps_stats.luck, caps_stats.def, caps_stats.res, caps_stats.con, caps_stats.move);
            fclose(fp);
        }

};


#endif /* UNITCOMPONENT_HPP */