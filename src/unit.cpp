// Code créé par Gabriel Taillon
// Note on standards. Fuck all that complicated bullcrap. I'm writing for c++11. More convenient and the rest seems like bullshit.
/*! \page handle unit.cpp
* \file unit.cpp 
* \brief unit class.
*/
#include "shared.hpp"
#include "unit.hpp"
#include "weapon.hpp"

using namespace std;

char wpn_types[][12]  = {"swd", "lance", "axe", "bow", "mgc_wind", "mgc_fire", "mgc_thunder",  "mgc_dark",  "mgc_light", "staff"};

const char* unit::get_equipped() const{
    // For some reason passing the pointer works as I intended.
    return(equipped); // This is an array. Normally size one. Can be made larger for equipping multiple things,
}

void unit::equip_weapon(std::vector<unsigned int> in_equipped) {
    // printf(" \n", in_equipped)
    // printf("%s\n", all_weapons[equipment[in_equipped[0]].name].type);
    char current_type[20];
    int current_type_ind;
    strncpy(current_type, all_weapons[equipment[in_equipped[0]].name].type, 20);
    for (int i = 0; i < sizeof(wpn_types)/sizeof(wpn_types[0]); i++) {
            // printf("weapon types %s %s\n", wpn_types[i], current_type);
            // printf("weapon types %d\n", strcmp(current_type, wpn_types[i]) == 0);
            // bool cond  = (strcmp(current_type, wpn_types[i]) == 0);
            bool cond = strcmp(current_type, wpn_types[i]) == 0;
        // printf("current_type: %s\n", current_type);
        if (strcmp(current_type, wpn_types[i]) == 0){
            current_type_ind = i;
        };
    }
    // printf("Current type %s\n", current_type);
    // printf("Current type index %s\n", current_type_ind);
    if (in_equipped.size() != 0) {
        // printf("current_type_ind %d\n", current_type_ind);
        // printf("wpn_exp %d\n", wpn_exp[current_type_ind]);
        if (wpn_exp[current_type_ind] > 0) {
            equipped[0] = (unsigned int) in_equipped[0]; 
        } else {
            string msg;
            msg += name;
            msg += " cannot equip ";
            msg += equipment[in_equipped[0]].name;
            msg += " \n";
            throw msg;
        }
        
    };
    attack_probs[0] = accuracy();
    attack_probs[1] = avoid();
    attack_probs[2] = critical();
    attack_probs[3] = favor();
}

void unit::take_damage(unsigned char damage) {
    printf("%s takes %d damage \n", name, damage); 
    current_hp = std::max(0, current_hp - damage);
    if (current_hp == 0) {death();};

}
void unit::heal(unsigned char healing) {
    printf("%s gets healed for %d\n", name, healing); 
    current_hp = std::min(current_hp + healing, (int) stats[0]);
}

unsigned char unit::get_hp() const {
  return(current_hp);
}

void unit::death() {
    printf("%s is dead.\n", name);
}

void unit::read(const char *filename, char skip) {
    // 2019/07/30: skip should be a multiple of *number of lines written to weapon.txt* which is 8.
    FILE *f = fopen(filename, "r");
    char line[500];
    for (int i = 0; i < skip; i++) {
        fgets(line, sizeof(line), f); // skips skip lines.
    }
    fgets(line, sizeof(line), f);
    line[strlen(line)-1] = 0;  //fgets also includes the \n in the line. This removes it.
    strncpy(name, line, sizeof(name));   
}

unit::unit(std::string in_name, std::string in_unit_class, char in_id, 
           std::vector<unsigned int> in_stats_base, std::vector<unsigned int> in_growths, std::vector<unsigned int> in_skills,
           std::vector<unsigned int> in_love_pts, std::vector<unsigned int> in_love_growths,
           std::vector<unsigned int> in_wpn_exp, std::vector<unsigned int> in_position,
           std::vector<unsigned int> in_equipped, 
           std::vector<inventory_item> in_equipment,
           std::vector<inventory_item> in_weapons, 
           std::vector<inventory_item> in_items,
           unsigned short in_exp, std::vector<std::string> in_lovers,
           bool in_mounted, bool in_flying, bool in_armored, bool in_promoted) {

    exp = in_exp;
    for (int i = 0; i < 7 ; i++) {
        if (i < in_equipment.size()) {
            equipment[i] = in_equipment[i];
        } else {
            equipment[i] = inventory_item("", -1); // Empty equipment item. -1 Makes no sense can can be used to check for errors later.
        }
    }
    strncpy(name, in_name.c_str(), sizeof(in_name));
    strncpy(type, in_unit_class.c_str(), sizeof(in_unit_class));
    for (int i = 0; i < in_stats_base.size(); i++) {
        stats_base[i] = (unsigned int) in_stats_base[i];
        stats[i] = (unsigned int) in_stats_base[i];
        growths[i] = (unsigned int) in_growths[i];
        wpn_exp[i] = (unsigned int) in_wpn_exp[i];
        stats_bonus[i] = 0;
        // For some reason, stats_base cannot be printed unless (int) all_units["Marth"].stats_base[0]. + 0 also works.
    }
    current_hp = stats_base[0];

    // printf("Did I equip the weapon successfully %d \n", get_equipped()[0]);
    // printf("Weapon size: %d\n", in_weapons.size());
    for (int i = 0; i < 4; i++) {

        if (i < in_weapons.size()) {
            weapons[i] = in_weapons[i];
        } else {
            weapons[i] = inventory_item("", -1); // Empty equipment item. -1 Makes no sense can can be used to check for errors later.
        };
        if (i < in_items.size()) {
            items[i] = in_items[i];
        } else {
            items[i] = inventory_item("", -1); // Empty equipment item. -1 Makes no sense can can be used to check for errors later.
        };
    }  
    for (int i = 0; i < in_skills.size(); i++) {
        skills[i] = (unsigned int) in_skills[i];
        position[i] = (unsigned int) in_position[i];
    }  
    for (int i = 0; i < in_lovers.size(); i++) {
        strncpy(lovers[i], in_lovers[i].c_str(), sizeof(in_lovers[i]));
        love_growths[i] = (unsigned int) in_love_growths[i];
        love_pts[i] = (unsigned int) in_love_pts[i];
        wpn_exp[i] = (unsigned int) in_wpn_exp[i];
    }    
    id = in_id;
    try {
        equip_weapon(in_equipped);
    } catch (std::string msg) { 
        equipped[0] = -1;
    }
    mounted = in_mounted;
    flying = in_flying;
    armored = in_armored;
    promoted = in_promoted;
}

unit::unit(){

}

unsigned char unit::attack_damage() {
    char unit_power = 0;
    char wpn_dmg = all_weapons[equipment[equipped[0]].name].stats[0];
    if (all_weapons[equipment[equipped[0]].name].dmg_type == 0) {
        // Physical attack_damage.
        unit_power = stats[1];
    } else {
        // Magical attack_damage.
        unit_power = stats[2];
    };
    int attack_damage = wpn_dmg + unit_power;
    // printf("wpn_dmg %d\n", wpn_dmg);
    // printf("unit_power %d\n", unit_power);
    return(attack_damage);
}

unsigned char unit::combat_damage(const unit& enemy, bool critical) {
    char terrain_def = 0;
    char enemy_def = 0 ;
    char unit_power = 0;
    char wpn_dmg = all_weapons[equipment[equipped[0]].name].stats[0];
    if (all_weapons[equipment[equipped[0]].name].dmg_type == 0) {
        // Physical attack_damage.
        unit_power = stats[1];
        enemy_def = enemy.stats[6];
    } else {
        // Magical attack_damage.
        unit_power = stats[2];
        enemy_def = enemy.stats[7];
    };
    // printf("unit_power %d\n", unit_power);
    // printf("enemy_def  %d\n", enemy_def );
    unsigned char crit_factor = 1;
    if (critical) {crit_factor=3;};
    int attack_damage = crit_factor*(std::max(wpn_dmg + unit_power - enemy_def - terrain_def, 0)); // Modern FE style. for crit_factor = 3
    // int attack_damage = crit_factor*(wpn_dmg + unit_power) - enemy_def - terrain_def);  // FE4-FE5 style. for crit_factor = 2
    if (attack_damage <= 0) {attack_damage = 0;};
    return(attack_damage);
}

unsigned char unit::avoid(){
    char supports = 0;
    char terrain_avoid = 0;
    unsigned char unit_avoid = stats[4]*2 + stats[5];
    unsigned char avoid = terrain_avoid + unit_avoid + supports ;
    return(avoid);
}

unsigned char unit::critical(){
    char supports = 0 ;
    char unit_skill = 0;
    unsigned char wpn_crit = all_weapons[equipment[equipped[0]].name].stats[2];
    unsigned char critical = wpn_crit + unit_skill + supports;
    return(critical);
}

bool unit::retaliation(const unit& enemy) const{
    unsigned char distance = abs(enemy.position[0] - position[0]) + abs(enemy.position[1] - position[1]);
    // printf("Distance %d \n", distance);
    bool out = 0;
    for (int i = 0; i < 3; i++) {
        if (distance == all_weapons[enemy.equipment[enemy.get_equipped()[0]].name].range[i]){
            out = 1;
        }
    }
    return(out);
}

bool unit::combat_double(const unit& enemy) const{
    unsigned char unit_speed = stats[4];
    unsigned char enemy_speed = enemy.stats[4];
    bool out = ((unit_speed - wpn_weighed_down() - enemy_speed) > 4);
    return(out);
}

unsigned char unit::wpn_weighed_down() const{
    //*DESIGN QUESTION* What should be the influence of weapons?
    unsigned char wpn_wght = all_weapons[equipment[equipped[0]].name].stats[3];
    unsigned char unit_con = stats[8];
    return(std::max(wpn_wght - unit_con, 0));
}

unsigned char unit::combat_critical(const unit& enemy){
    char supports = 0;
    char unit_skill = 0;
    unsigned char wpn_crit = all_weapons[equipment[equipped[0]].name].stats[2];
    unsigned char critical = std::max(0, wpn_crit + unit_skill + supports - enemy.attack_probs[3]);
    return(critical);
}

unsigned char unit::favor(){
    char supports = 0 ;
    char unit_favor = (ceil(stats[5]/2.)); // By default, integer division floors
    // For design simplicity, I think it is good to have percent values only change by increments of 1. Simple.
    unsigned char favor = unit_favor + supports;
    return(favor);
}

unsigned char unit::accuracy(){
    char supports = 0;
    unsigned char wpn_hit = all_weapons[equipment[equipped[0]].name].stats[1];
    unsigned char unit_acc = stats[3]*2 + stats[5];
    unsigned char accuracy = wpn_hit + unit_acc + supports;
    return(accuracy);
}

unsigned char unit::combat_hit(const unit& enemy){
    char supports = 0;
    unsigned char wpn_hit = all_weapons[equipment[equipped[0]].name].stats[1];
    unsigned char unit_acc = stats[3]*2 + stats[5];
    unsigned char accuracy = std::max(0, wpn_hit + unit_acc + supports - enemy.attack_probs[1]);
    return(accuracy);
}

unsigned char unit::attack(unit& enemy){
    printf("%s attacks %s\n", name, enemy.name);
    bool unit_hits = (get_rand() < combat_hit(enemy));    
    bool unit_crits = (get_rand() < combat_critical(enemy));
    // printf("%s crits %d \n", name, unit_crits);
    // printf("%d crit chance \n", combat_critical(enemy));
    /* *DESIGN QUESTION* Should a random number always be rolled for crits, even if the hit doesn't connect?
    * I think so. Always same number of rand rolled. 
    * But what about crit animations? Should crit animations be shown to miss? Fire Emblem thinks not.
    */
    // unit.take_damage( -= combat_damage(enemy, unit_crits);
    enemy.take_damage(combat_damage(enemy, unit_crits));
    return(combat_damage(enemy, unit_crits));
}

void unit::combat(unit& enemy){
    printf("%s fights %s\n", name, enemy.name);
    bool unit_doubles = combat_double(enemy);
    bool enemy_retaliates = enemy.retaliation(static_cast<const unit&>(*this));
    bool enemy_doubles = 0;

    printf("%s doubles %d\n", name, unit_doubles);
    // printf("enemy retaliates %d\n", enemy_retaliates);
    attack(enemy);
    if (enemy_retaliates) {
        enemy.attack(static_cast<unit&>(*this));
        enemy_doubles = enemy.combat_double(static_cast<const unit&>(*this));
        printf("%s doubles %d\n", enemy.name ,enemy_doubles);
    };

    if (unit_doubles) {attack(enemy);};
    if (enemy_doubles) {enemy.attack(static_cast<unit&>(*this));};

    // retaliation();

}

void unit::enemy_select(const unit& enemy) {
    combat_probs[0] = combat_hit(enemy);
    combat_probs[1] = combat_critical(enemy);
}

unit::~unit(void) {
   // printf("Unit %s is being deleted.\n", name.c_str());
}
