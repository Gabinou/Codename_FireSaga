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

char wpn_types[][12]  = {"swd", "lance", "axe", "bow", "mgc_wind", "mgc_fire", "mgc_thunder",  "mgc_dark",  "mgc_light", "staff"}; // I don't understand. This line does not seem to work anywhere else. why?

const char* unit::get_equipped() const{
    // For some reason passing the pointer works as I intended.
    return(equipped); // This is an array. Normally size one. Can be made larger for equipping multiple things,
}

void unit::equip_weapon(std::vector<int> in_equipped) {
    // printf(" \n", in_equipped)
    // printf("%s\n", all_weapons[equipment[in_equipped[0]].name].type);
    char current_type[20];
    int current_type_ind;
    // printf("in_equiupped[0] %d \n", in_equipped[0]);
    if (in_equipped[0] == -1) {
        
        equipped[0] = -1;
    } else {
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
                equipped[0] = (int) in_equipped[0]; 
                attack_probs[0] = accuracy();
                attack_probs[1] = avoid();
                attack_probs[2] = critical();
                attack_probs[3] = favor();
            } else {
                string msg;
                msg += name;
                msg += " cannot equip ";
                msg += equipment[in_equipped[0]].name;
                msg += " \n";
                // throw msg; // THIS LINE breaks the code on Gabinours-PC
            }
            
        };
    }
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

void unit::write(std::string filename) { 
    std::ofstream out(filename, std::ios_base::app);
    out << name << "\n";
    out << "Class: \t\t" << type << "\n";
    out << "Bases: \t\t";
    for (int i = 0; i < sizeof(stats_base)/sizeof(stats_base[0]); i++) {
        if (i == (sizeof(stats_base)/sizeof(stats_base[0])) - 1) {
            out << (int) stats_base[i] << "\n";
        } else {
            out << (int) stats_base[i] << ", ";
        }
    };
    out << "Stats: \t\t" // must stay like this cause of busted stats variable that needs more space to not crash.
    << (int) stats[0] << ", " << (int) stats[1] << ", " 
    << (int) stats[2] << ", " << (int) stats[3] << ", " 
    << (int) stats[4] << ", " << (int) stats[5] << ", " 
    << (int) stats[6] << ", " << (int) stats[7] << ", " 
    << (int) stats[8] << ", " << (int) stats[9] << "\n"
    << (int) stats[10] << "\n"
    << "Growths: \t";
    for (int i = 0; i < sizeof(growths)/sizeof(growths[0]); i++) {
        if (i == (sizeof(growths)/sizeof(growths[0])) - 1) {
            out << (int) growths[i] << "\n";
        } else {
            out << (int) growths[i] << ", ";
        }
    };
    out << "Equipped: \t" << (int) get_equipped()[0] << "\n";
    out << "Equipment: \t";
    for (int i = 0; i < sizeof(equipment)/sizeof(equipment[0]); i++) {
        out << equipment[i].name << ", ";
    };
    out << "\n" << "Used(eq): \t";
    for (int i = 0; i < sizeof(equipment)/sizeof(equipment[0]); i++) {
        out << (int) equipment[i].used << ", ";
    };
    out << "\n" << "Weapons: \t";
    for (int i = 0; i < sizeof(weapons)/sizeof(weapons[0]); i++) {
        out << weapons[i].name << ", ";
    };
    out << "\n" << "Items: \t\t"; 
    for (int i = 0; i < sizeof(items)/sizeof(items[0]); i++) {
        out << items[i].name << ", ";
    };
    out << "\n" << "supports: \t";
    for (int i = 0; i < sizeof(supports)/sizeof(supports[0]); i++) {
        if (i == (sizeof(supports)/sizeof(supports[0])) - 1) {
            out << supports[i] << "\n";
        } else {
            out << supports[i] << ", ";
        }
    };
    out << "supp_pts: \t";
    for (int i = 0; i < sizeof(supp_pts)/sizeof(supp_pts[0]); i++) {
        if (i == (sizeof(supp_pts)/sizeof(supp_pts[0])) - 1) {
            out << (int) supp_pts[i] << "\n";
        } else {
            out << (int) supp_pts[i] << ", ";
        }
    };
    out << "love_grt: \t";
    for (int i = 0; i < sizeof(support_growths)/sizeof(support_growths[0]); i++) {
        if (i == (sizeof(support_growths)/sizeof(support_growths[0])) - 1) {
            out << (int) support_growths[i] << "\n";
        } else {
            out << (int) support_growths[i] << ", ";
        }
    };
    out << "wpn_exp: \t";
    for (int i = 0; i < sizeof(wpn_exp)/sizeof(wpn_exp[0]); i++) {
        if (i == (sizeof(wpn_exp)/sizeof(wpn_exp[0])) - 1) {
            out << (int) wpn_exp[i] << "\n";
        } else {
            out << (int) wpn_exp[i] << ", ";
        }
    };
    out << "Exp: \t\t" << (int) exp << "\n";
    // out << "Mounted: \t" << mounted << "\n";
    // out << "Flying: \t"  << flying << "\n";
    // out << "Armored: \t" << armored << "\n";
    // out << "Promoted: \t" << promoted << "\n";
    out << std::endl;
}


void unit::read(const char *filename, short int skip) {
    // 2019/07/30: skip should be a multiple of *number of lines written to units.txt* which is 20.
    std::ifstream infile(filename);
    std::string line;
    std::vector<int> temp;
    std::string new_name;
    std::vector<std::string> tempstr;
    printf("%d \n", skip);
    int j = 0;
    while (j < skip + 16) {
        std::getline(infile, line);
        if (j>=skip) {
            std::cout << j << line << endl;
            std::istringstream iss(line);
            // std::cout << line << endl;
            strncpy(name, line.c_str(), sizeof(line));
            std::getline(infile, line);
            // std::cout << line << endl;
            strncpy(type, line.substr(9, line.size()).c_str(), sizeof(line));
            std::getline(infile, line);
            temp = csv_from_line(line.substr(8, line.size()));
            for (int i = 0; i < temp.size(); i++) {
                stats_base[i] = temp[i];
            }
            std::getline(infile, line);
            temp = csv_from_line(line.substr(9, line.size()));
            for (int i = 0; i < temp.size(); i++) {
                stats[i] = temp[i];
            }
            std::getline(infile, line);
            temp = csv_from_line(line.substr(9, line.size()));
            for (int i = 0; i < temp.size(); i++) {
                growths[i] = temp[i];
            }
            std::getline(infile, line);
            temp = csv_from_line(line.substr(9, line.size()));
            for (int i = 0; i < temp.size(); i++) {
                equipped[i] = temp[i];
            }
            std::getline(infile, line);
            tempstr = css_from_line(line.substr(10, line.size()));
            std::getline(infile, line);
            // std::cout << line.substr(10, line.size()) << endl;
            temp = csv_from_line(line.substr(10, line.size()));
            for (int i = 0; i < temp.size(); i++) {
                equipment[i] = inventory_item(tempstr[i], temp[i]);
            }
            std::getline(infile, line); //weapon line
            std::getline(infile, line); //items line
            // std::cout << "THIS" << temp[0] << endl;
            // std::cout << get_equipped()[0] << endl;
            // std::cout << get_equipped()[1] << endl;
            std::getline(infile, line);
            tempstr = css_from_line(line.substr(9, line.size()));
            for (int i = 0; i < tempstr.size(); i++) {
                strncpy(supports[i], tempstr[i].c_str(), sizeof(supports[i]));
            }
            std::getline(infile, line);
            temp = csv_from_line(line.substr(10, line.size()));
            for (int i = 0; i < temp.size(); i++) {
                supp_pts[i] = temp[i];
            }
            std::getline(infile, line);
            temp = csv_from_line(line.substr(10, line.size()));
            for (int i = 0; i < temp.size(); i++) {
                supp_pts[i] = temp[i];
            }
            std::getline(infile, line);
            temp = csv_from_line(line.substr(10, line.size()));
            for (int i = 0; i < temp.size(); i++) {
                wpn_exp[i] = temp[i];
            }
            std::getline(infile, line);
            exp = csv_from_line(line.substr(7, line.size()))[0];
            j+=16;
        }
        j++;
    }
}

unit::unit(std::string in_name, std::string in_unit_class, char in_id, 
           std::vector<unsigned int> in_stats_base, std::vector<unsigned int> in_growths, std::vector<unsigned int> in_skills,
           std::vector<unsigned int> in_supp_pts, std::vector<unsigned int> in_support_growths,
           std::vector<unsigned int> in_wpn_exp, std::vector<unsigned int> in_position,
           std::vector<int> in_equipped, 
           std::vector<inventory_item> in_equipment,
           std::vector<inventory_item> in_weapons, 
           std::vector<inventory_item> in_items,
           unsigned short in_exp, std::vector<std::string> in_supports) {

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
    for (int i = 0; i < in_supports.size(); i++) {
        strncpy(supports[i], in_supports[i].c_str(), sizeof(in_supports[i]));
        support_growths[i] = (unsigned int) in_support_growths[i];
        supp_pts[i] = (unsigned int) in_supp_pts[i];
        wpn_exp[i] = (unsigned int) in_wpn_exp[i];
    }    
    id = in_id;
    try {
        equip_weapon(in_equipped);
    } catch (std::string msg) { 
        equipped[0] = -1;
    }
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
