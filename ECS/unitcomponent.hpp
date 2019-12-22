#ifndef UNITCOMPONENT_HPP
#define UNITCOMPONENT_HPP
#include "ECS.hpp"
#include "game.hpp"
#include "shared.hpp"
#include "SDL2/SDL.h"

struct Equipped {
    int left;
    int right;
    //Index in big array of all weapons? Or pointer to weapon?
};

struct Unit_stats {
    unsigned char hp;
    unsigned char str;
    unsigned char mag;
    unsigned char spd;
    unsigned char skl;
    unsigned char luck;
    unsigned char def;
    unsigned char res;
    unsigned char con;
};

struct Weapon {
    unsigned char dmg;
    unsigned char hit;
    unsigned char dodge;
    unsigned char crit;
    unsigned char favor;
    unsigned char wgt;
    unsigned char range[2]; // [min_range, max_range]
    bool hand; //0 is 1 hand. 1 is 2 hands.
    bool dmg_type; // 0 is 1 physical. 1 magic.
};

class UnitComponent : public Component {
    private:
        Equipped equipped;
        Weapon temp_wpn;
        Unit_stats base_stats;
        Unit_stats bonus_stats;
        Unit_stats malus_stats;
        Unit_stats current_stats;
        Unit_stats growths;
        unsigned char current_hp;
        std::string name;
    public:
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
        void set_hp(const unsigned char in_hp) {
            current_hp = in_hp;
        }

        void death() {
            printf("%s is dead.\n", name);
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

        unsigned char combat_damage(const Entity & enemy, const bool critical) {
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

        bool retaliation(const Entity & enemy) {
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

        bool combat_double(const Entity & enemy) const {
            unsigned char enemy_speed = enemy.getComponent<UnitComponent>().current_stats.spd;
            bool doubles = ((current_stats.spd - wpn_weighed_down() - enemy_speed) > 4);
            return (doubles);
        }

        unsigned char wpn_weighed_down() const {
            //*DESIGN QUESTION* What should be the influence of weapons?
            // Average of Con and Str?
            return (std::max(temp_wpn.wgt - current_stats.con, 0));
        }

        unsigned char combat_critical(const Entity & enemy) {
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

        unsigned char combat_hit(const Entity & enemy) {
            int enemy_avoid = enemy.getComponent<UnitComponent>().avoid();
            unsigned char hit = std::max(0, accuracy() - enemy_avoid);
            return (hit);
        }

        unsigned char attack(const Entity & enemy) {
            printf("%s attacks %s\n", name, enemy.getComponent<UnitComponent>().get_name());
            bool unit_hits = (get_rand() < combat_hit(enemy));
            bool unit_crits = (get_rand() < combat_critical(enemy));
            // printf("%s crits %d \n", name, unit_crits);
            // printf("%d crit chance \n", combat_critical(enemy));
            /* *DESIGN QUESTION* Should a random number always be rolled for crits, even if the hit doesn't connect?
            * I think so. Always same number of rand rolled.
            * But what about crit animations? Should crit animations be shown to miss? Fire Emblem thinks not.
            */
            // unit.take_damage( -= combat_damage(enemy, unit_crits);
            enemy.getComponent<UnitComponent>().take_damage(combat_damage(enemy, unit_crits));
            // return (combat_damage(unit_crits));
            return (1);
        }

        // void unit::combat(unit & enemy) {
        //     printf("%s fights %s\n", name, enemy.name);
        //     bool unit_doubles = combat_double(enemy);
        //     bool enemy_retaliates = enemy.retaliation(static_cast<const unit &>(*this));
        //     bool enemy_doubles = 0;
        //     printf("%s doubles %d\n", name, unit_doubles);
        //     // printf("enemy retaliates %d\n", enemy_retaliates);
        //     attack(enemy);

        //     if (enemy_retaliates) {
        //         enemy.attack(static_cast<unit &>(*this));
        //         enemy_doubles = enemy.combat_double(static_cast<const unit &>(*this));
        //         printf("%s doubles %d\n", enemy.name, enemy_doubles);
        //     };

        //     if (unit_doubles) {attack(enemy);};

        //     if (enemy_doubles) {enemy.attack(static_cast<unit &>(*this));};

        //     // retaliation();
        // }

        // void unit::enemy_select(const unit & enemy) {
        //     combat_probs[0] = combat_hit(enemy);
        //     combat_probs[1] = combat_critical(enemy);
        // }



};


#endif /* UNITCOMPONENT_HPP */