#include "unit.hpp"
#include "shared.hpp"

Unit::Unit() {
    equipped.left = -1;
    equipped.right = -1;
}

Unit::Unit(const std::string in_name, const unsigned char in_class_index, const Unit_stats in_bases, const std::string in_sex, Map_enemy in_map_enemy, unsigned long long int in_skills) : Unit(in_name, in_class_index, in_bases, in_sex, in_map_enemy) {
    skills = in_skills;
}

Unit::Unit(const std::string in_name, const unsigned char in_class_index, const Unit_stats in_bases, const bool in_sex, Map_enemy in_map_enemy, unsigned long long int in_skills) : Unit(in_name, in_class_index, in_bases, in_sex, in_map_enemy) {
    skills = in_skills;
}

Unit::Unit(const std::string in_name, const unsigned char in_class_index, const Unit_stats in_bases, const std::string in_sex, Map_enemy in_map_enemy) : Unit(in_name, in_class_index, in_bases, in_sex) {
    setMap_enemy(in_map_enemy);
}

Unit::Unit(const std::string in_name, const unsigned char in_class_index, const Unit_stats in_bases, const bool in_sex, Map_enemy in_map_enemy) : Unit(in_name, in_class_index, in_bases, in_sex) {
    setMap_enemy(in_map_enemy);
}

Unit::Unit(const std::string in_name, const unsigned char in_class_index, const Unit_stats in_bases, const std::string in_sex) : Unit(in_name, in_class_index, in_bases) {
    setSex(in_sex);
}

Unit::Unit(const std::string in_name, const unsigned char in_class_index, const Unit_stats in_bases, const bool in_sex) : Unit(in_name, in_class_index, in_bases) {
    setSex(in_sex);
}

Unit::Unit(const std::string in_name, const unsigned char in_class_index, const Unit_stats in_bases) : Unit(in_name, in_bases) {
    class_index = in_class_index;
    autoMvttype();
    autoClassname();
    setEquippable();
}

Unit::Unit(const std::string in_name, const Unit_stats in_bases) : Unit(in_bases) {
    name = in_name;
}

Unit::Unit(const Unit_stats in_bases) : Unit() {
    base_stats = in_bases;
    current_stats = in_bases;
}

short int * Unit::getPos() {
    return(position);
}

void Unit::setPos(const short int in_pos[2]) {
    position[0] = in_pos[0];
    position[1] = in_pos[1];
}

void Unit::setPos(const Point in_pos) {
    position[0] = in_pos.y;
    position[1] = in_pos.x;
}

bool Unit::getSex() {
    return(sex);
}
void Unit::setSex(const bool in_sex) {
    sex = in_sex;
}

void Unit::setSex(const std::string in_sex) {
    if (in_sex == "F") {
        sex = 0;
    }
    if (in_sex == "M") {
        sex = 1;
    }  
}

Map_enemy Unit::getMap_enemy(){
    return(map_enemy);
}

void Unit::setMap_enemy(Map_enemy in_map_enemy){
    map_enemy = in_map_enemy;
}

void Unit::removeEquipment(unsigned char in_index) {
    Inventory_item empty;
    equipment[in_index] = empty; 
}

void Unit::addEquipment(Inventory_item in_equipment) {
    for (short unsigned int i = 0; i < DEFAULT::EQUIPMENT_SIZE; i++) {
        if (equipment[i].id == -1) {
            equipment[i] = in_equipment; 
            break;
        }
    }
}

void Unit::setEquipment(Inventory_item * in_equipment) {
    for (short unsigned int i = 0; i < DEFAULT::EQUIPMENT_SIZE; i++) {
        equipment[i] = in_equipment[i];
    }
}

void Unit::setEquipped(Equipped in_equipped) {
    equipped = in_equipped;
}

short unsigned int Unit::getEquippable() {
    return(equippable);
}

void Unit::setEquippable() {
    //This should not be in all unit objects. So much wasted space. Make it one shared callable function. Same for other heqvy switches. 
    // Switch with individual cases:
    // Easy to change equippable post-hoc
    switch(class_index) {
        case UNIT_CLASS::MERCENARY:
            equippable = WPN_TYPE::SHIELD + WPN_TYPE::SWORD + WPN_TYPE::OFFHAND;
            break;
        case UNIT_CLASS::LORD:
            equippable = WPN_TYPE::LANCE + WPN_TYPE::SHIELD;
            break;
        case UNIT_CLASS::DUELIST:
            equippable = WPN_TYPE::LANCE + WPN_TYPE::SHIELD;
            break;
        case UNIT_CLASS::THIEF:
            equippable = WPN_TYPE::LANCE + WPN_TYPE::SHIELD;
            break;
        case UNIT_CLASS::PEGASUS_KNIGHT:
            equippable = WPN_TYPE::LANCE + WPN_TYPE::SHIELD;
            break;
        case UNIT_CLASS::PIKEMAN:
            equippable = WPN_TYPE::LANCE + WPN_TYPE::SHIELD;
            break;
        case UNIT_CLASS::CAVALIER:
            equippable = WPN_TYPE::LANCE + WPN_TYPE::SHIELD;
            break;
        case UNIT_CLASS::KNIGHT:
            equippable = WPN_TYPE::LANCE + WPN_TYPE::SHIELD;
            break;
        case UNIT_CLASS::BANDIT:
            equippable = WPN_TYPE::SHIELD + WPN_TYPE::AXE;
            break;
        case UNIT_CLASS::CORSAIR:
            equippable = WPN_TYPE::SHIELD + WPN_TYPE::OFFHAND + WPN_TYPE::AXE;
            break;
        case UNIT_CLASS::VIKING:
            equippable = WPN_TYPE::SHIELD + WPN_TYPE::OFFHAND + WPN_TYPE::AXE;
            break;
        case UNIT_CLASS::PICKPOCKET:
            equippable = WPN_TYPE::OFFHAND + WPN_TYPE::SWORD;
            break;
        case UNIT_CLASS::FENCER:
            equippable = WPN_TYPE::OFFHAND + WPN_TYPE::SWORD;
            break;
        case UNIT_CLASS::MOUSQUETAIRE:
            equippable = WPN_TYPE::SWORD + WPN_TYPE::OFFHAND + WPN_TYPE::BOW; 
            break;
        case UNIT_CLASS::ASSASSIN:
            equippable = WPN_TYPE::SWORD + WPN_TYPE::OFFHAND + WPN_TYPE::BOW; 
            break;
        case UNIT_CLASS::MARKSMAN:
            equippable = WPN_TYPE::SWORD + WPN_TYPE::OFFHAND + WPN_TYPE::BOW; 
            break;
        case UNIT_CLASS::ARCHER:
            equippable = WPN_TYPE::BOW;
            break;
        case UNIT_CLASS::DUKE:
            equippable = WPN_TYPE::SWORD + WPN_TYPE::LANCE + WPN_TYPE::SHIELD + WPN_TYPE::OFFHAND;
            break;
        case UNIT_CLASS::PALADIN:
            equippable = WPN_TYPE::SWORD + WPN_TYPE::LANCE + WPN_TYPE::SHIELD + WPN_TYPE::OFFHAND;
            break;
        case UNIT_CLASS::GENERAL:
            equippable = WPN_TYPE::SWORD + WPN_TYPE::SHIELD + WPN_TYPE::AXE + WPN_TYPE::LANCE;
            break;
        case UNIT_CLASS::CLERIC:
            equippable = WPN_TYPE::STAFF;
            break;
        case UNIT_CLASS::PRIEST:
            equippable = WPN_TYPE::STAFF;
            break;
        case UNIT_CLASS::MAGE:
            equippable = WPN_TYPE::ELEMENTAL + WPN_TYPE::TRINKET;
            break;
        case UNIT_CLASS::HERO:
            equippable = WPN_TYPE::AXE + WPN_TYPE::SWORD + WPN_TYPE::SHIELD + WPN_TYPE::OFFHAND;
            break;
        case UNIT_CLASS::RAVAGER:
            equippable = WPN_TYPE::AXE + WPN_TYPE::SHIELD + WPN_TYPE::OFFHAND + WPN_TYPE::BOW;
            break;
        case UNIT_CLASS::BATTLEMAGE:
            equippable = WPN_TYPE::ELEMENTAL + WPN_TYPE::SHIELD + WPN_TYPE::SWORD + WPN_TYPE::TRINKET;
            break;
        case UNIT_CLASS::SAGE:
            equippable = WPN_TYPE::ELEMENTAL + WPN_TYPE::STAFF + WPN_TYPE::TRINKET;
            break;
        case UNIT_CLASS::TROUBADOUR:
            equippable = WPN_TYPE::ELEMENTAL + WPN_TYPE::STAFF + WPN_TYPE::TRINKET;
            break;
        case UNIT_CLASS::ORACLE:
            equippable = WPN_TYPE::STAFF + WPN_TYPE::ANGELIC + WPN_TYPE::TRINKET;
            break;
        case UNIT_CLASS::BISHOP:
            equippable = WPN_TYPE::STAFF + WPN_TYPE::ANGELIC + WPN_TYPE::TRINKET;
            break;
        case UNIT_CLASS::ANGEL:
            equippable = WPN_TYPE::ANGELIC + WPN_TYPE::SWORD + WPN_TYPE::LANCE + WPN_TYPE::SHIELD;
            break;
        case UNIT_CLASS::DEMON:
            equippable = WPN_TYPE::DEMONIC + WPN_TYPE::CLAW + WPN_TYPE::AXE;
            break;
        case UNIT_CLASS::DEMONIC_INCARNATE:
            equippable = WPN_TYPE::DEMONIC + WPN_TYPE::TRINKET;
            break;
        case UNIT_CLASS::ANGELIC_INCARNATE:
            equippable = WPN_TYPE::ANGELIC + WPN_TYPE::TRINKET;
            break;
    }
}

void Unit::autoClassname() {
    switch(class_index) {
        case UNIT_CLASS::MERCENARY:
            class_name = "Mercenary";
            break;
        case UNIT_CLASS::LORD:
            class_name = "Lord";
            break;        
        case UNIT_CLASS::LORD_RIDER:
            class_name = "Lord rider";
            break;
        case UNIT_CLASS::DUELIST:
            class_name = "Duelist";
            break;
        case UNIT_CLASS::THIEF:
            class_name = "Thief";
            break;
        case UNIT_CLASS::PEGASUS_KNIGHT:
            class_name = "Pegasus knight";
            break;
        case UNIT_CLASS::PIKEMAN:
            class_name = "Pikeman";
            break;
        case UNIT_CLASS::CAVALIER:
            class_name = "Cavalier";
            break;
        case UNIT_CLASS::KNIGHT:
            class_name = "Knight";
            break;
        case UNIT_CLASS::BANDIT:
            class_name = "Bandit";
            break;
        case UNIT_CLASS::CORSAIR:
            class_name = "Corsair";
            break;
        case UNIT_CLASS::VIKING:
            class_name = "Viking";
            break;
        case UNIT_CLASS::PICKPOCKET:
            class_name = "Pickpocket";
            break;
        case UNIT_CLASS::FENCER:
            class_name = "Fencer";
            break;
        case UNIT_CLASS::MOUSQUETAIRE:
            class_name = "Mousquetaire";
            break;
        case UNIT_CLASS::ASSASSIN:
            class_name = "Assassin";
            break;
        case UNIT_CLASS::MARKSMAN:
            class_name = "Marksman";
            break;        
        case UNIT_CLASS::MARKSMAN_RIDER:
            class_name = "Marksman rider";
            break;
        case UNIT_CLASS::ARCHER:
            class_name = "Archer";
            break;        
        case UNIT_CLASS::ARCHER_RIDER:
            class_name = "Archer rider";
            break;
        case UNIT_CLASS::DUKE:
            class_name = "Duke";
            break;
        case UNIT_CLASS::DUKE_RIDER:
            class_name = "Duke rider";
            break;
        case UNIT_CLASS::PALADIN:
            class_name = "Paladin";
            break;
        case UNIT_CLASS::GENERAL:
            class_name = "General";
            break;
        case UNIT_CLASS::CLERIC:
            class_name = "Cleric";
            break;
        case UNIT_CLASS::PRIEST:
            class_name = "Priest";
            break;
        case UNIT_CLASS::MAGE:
            class_name = "Mage";
            break;
        case UNIT_CLASS::HERO:
            class_name = "Hero";
            break;
        case UNIT_CLASS::RAVAGER:
            class_name = "Ravager";
            break;
        case UNIT_CLASS::BATTLEMAGE:
            class_name = "Battlemage";
            break;
        case UNIT_CLASS::SAGE:
            class_name = "Sage";
            break;
        case UNIT_CLASS::TROUBADOUR:
            class_name = "Troubadour";
            break;
        case UNIT_CLASS::ORACLE:
            class_name = "Oracle";
            break;
        case UNIT_CLASS::BISHOP:
            class_name = "Bishop";
            break;
        case UNIT_CLASS::ANGEL:
            class_name = "Angel";
            break;
        case UNIT_CLASS::DEMON:
            class_name = "Demon";
            break;
        case UNIT_CLASS::DEMONIC_INCARNATE:
            class_name = "Demonic Incarnate";
            break;
        case UNIT_CLASS::ANGELIC_INCARNATE:
            class_name = "Angelic Incarnate";
            break;
    }
}

unsigned char Unit::getMvttype() {
    printf("inside unit %d\n", mvt_type);
    return (mvt_type);
}

void Unit::autoMvttype() {
    switch(class_index) {
        case UNIT_CLASS::MERCENARY:
            mvt_type = MVT_TYPE::FOOT_SLOW;
            break;
        case UNIT_CLASS::LORD:
            mvt_type = MVT_TYPE::FOOT_SLOW;
            break;
        case UNIT_CLASS::DUELIST:
            mvt_type = MVT_TYPE::FOOT_SLOW;
            break;
        case UNIT_CLASS::THIEF:
            mvt_type = MVT_TYPE::FOOT_FAST;
            break;
        case UNIT_CLASS::PEGASUS_KNIGHT:
            mvt_type = MVT_TYPE::FLIERS;
            break;
        case UNIT_CLASS::PIKEMAN:
            mvt_type = MVT_TYPE::FOOT_SLOW;
            break;
        case UNIT_CLASS::CAVALIER:
            mvt_type = MVT_TYPE::RIDERS_SLOW;
            break;
        case UNIT_CLASS::KNIGHT:
            mvt_type = MVT_TYPE::ARMORS;
            break;
        case UNIT_CLASS::BANDIT:
            mvt_type = MVT_TYPE::BANDITS;
            break;
        case UNIT_CLASS::CORSAIR:
            mvt_type = MVT_TYPE::PIRATES;
            break;
        case UNIT_CLASS::VIKING:
            mvt_type = MVT_TYPE::PIRATES;
            break;
        case UNIT_CLASS::PICKPOCKET:
            mvt_type = MVT_TYPE::FOOT_FAST;
            break;
        case UNIT_CLASS::FENCER:
            mvt_type = MVT_TYPE::FOOT_SLOW;
            break;
        case UNIT_CLASS::MOUSQUETAIRE:
            mvt_type = MVT_TYPE::FOOT_SLOW;
            break;
        case UNIT_CLASS::ASSASSIN:
            mvt_type = MVT_TYPE::FOOT_FAST;
            break;
        case UNIT_CLASS::MARKSMAN:
            mvt_type = MVT_TYPE::FOOT_SLOW;
            break;        
        case UNIT_CLASS::MARKSMAN_RIDER:
            mvt_type = MVT_TYPE::RIDERS_SLOW;
            break;
        case UNIT_CLASS::ARCHER:
            mvt_type = MVT_TYPE::FOOT_SLOW;
            break;        
        case UNIT_CLASS::ARCHER_RIDER:
            mvt_type = MVT_TYPE::RIDERS_SLOW;
            break;
        case UNIT_CLASS::DUKE:
            mvt_type = MVT_TYPE::FOOT_SLOW;
            break;
        case UNIT_CLASS::PALADIN:
            mvt_type = MVT_TYPE::RIDERS_FAST;
            break;
        case UNIT_CLASS::GENERAL:
            mvt_type = MVT_TYPE::ARMORS;
            break;
        case UNIT_CLASS::CLERIC:
            mvt_type = MVT_TYPE::MAGES;
            break;
        case UNIT_CLASS::PRIEST:
            mvt_type = MVT_TYPE::MAGES;
            break;
        case UNIT_CLASS::MAGE:
            mvt_type = MVT_TYPE::MAGES;
            break;
        case UNIT_CLASS::HERO:
            mvt_type = MVT_TYPE::FOOT_SLOW;
            break;
        case UNIT_CLASS::RAVAGER:
            mvt_type = MVT_TYPE::BANDITS;
            break;
        case UNIT_CLASS::BATTLEMAGE:
            mvt_type = MVT_TYPE::MAGES;
            break;
        case UNIT_CLASS::SAGE:
            mvt_type = MVT_TYPE::MAGES;
            break;
        case UNIT_CLASS::TROUBADOUR:
            mvt_type = MVT_TYPE::RIDERS_FAST;
            break;
        case UNIT_CLASS::ORACLE:
            mvt_type = MVT_TYPE::MAGES;
            break;
        case UNIT_CLASS::BISHOP:
            mvt_type = MVT_TYPE::MAGES;
            break;
        case UNIT_CLASS::ANGEL:
            mvt_type = MVT_TYPE::FLIERS;
            break;
        case UNIT_CLASS::DEMON:
            mvt_type = MVT_TYPE::FOOT_FAST;
            break;
        case UNIT_CLASS::DEMONIC_INCARNATE:
            mvt_type = MVT_TYPE::MAGES;
            break;
        case UNIT_CLASS::ANGELIC_INCARNATE:
            mvt_type = MVT_TYPE::MAGES;
            break;
    }
}

void Unit::equipsL(const char index) {
    equipped.left = index;
}

void Unit::equipsR(const char index) {
    equipped.right = index;
}

void Unit::unequipsL() {
    equipped.left = -1;
}

void Unit::unequipsR() {
    equipped.right = -1;
}

void Unit::equips(const short unsigned int index, const std::string hand) {
    if (hand == "left") {
        equipped.left = index;
    } else {
        equipped.right = index;
    }
}

void Unit::unequips(const std::string hand) {
    if (hand == "left") {
        equipped.left = -1;
    } else {
        equipped.right = -1;
    }
}

void Unit::takeItem(Inventory_item * out_array, short int in_index, short int out_index) {
    equipment[in_index] = out_array[out_index];
    Inventory_item empty;
    out_array[out_index] = empty;
}

void Unit::giveItem(Inventory_item * out_array, short int in_index, short int out_index) {
    out_array[out_index] = equipment[in_index];
    Inventory_item empty;
    equipment[in_index] = empty;
}

void Unit::dropItem(short int in_index) {
    Inventory_item empty;
    equipment[in_index] = empty;
}

void Unit::takesDamage(const unsigned char damage) { 
    printf("%s takes %d damage \n", name, damage);
    current_hp = std::max(0, current_hp - damage);

    if (current_hp == 0) {dies();};
}

void Unit::getsHealed(const unsigned char healing) {
    printf("%s gets healed for %d\n", name, healing);
    current_hp = std::min( (short int) (current_hp + healing), (short int) current_stats.hp);
}

unsigned char Unit::getHp() const {
    return (current_hp);
}

unsigned short int Unit::getLvl() const {
    return (ceil(current_hp / 100));
}

unsigned short int Unit::getExp() const {
    return (exp);
}

unsigned char * Unit::getRange() const {
    // DESIGN QUESTION: what about equipping only an offhand? Should offhand have ranges? 
    // Can you attack with only offhand weapons? how to treat their hit rate?

    unsigned char * temp;
    static unsigned char range[2];
    unsigned char * right_range;

    // if (equipped.left > 0) {
    //     temp = all_weapons[equipment[equipped.left].name].getStats().range;
    //     range[0] = temp[0]; range[1] = temp[1];
    // } else {
    //     range[0] = 1;
    //     range[1] = 1;
    // }

    // if (equipped.right > 0) {
    //     temp = all_weapons[equipment[equipped.left].name].getStats().range;
    //     right_range[0] = temp[0];
    //     right_range[1] = temp[1];
    //     range[0] = std::min(right_range[0], range[0]);
    //     range[1] = std::max(right_range[1], range[1]);
    // }

    // if ((equipped.left < 0) && (equipped.right < 0)) {
    //     range[0] = 0;
    //     range[1] = 0;
    // }

    range[0] = 5;
    range[1] = 10;

    return (range);
}

void Unit::setEntity(short int in_index) {
    entity = in_index;
}

int Unit::getEntity() {
    return(entity);
}

void Unit::setExp(const short unsigned int in_exp) {
    exp = in_exp;
}

void Unit::addExp(const short unsigned int in_exp) {
    exp += in_exp;
}

void Unit::setHp(const unsigned char in_hp) {
    current_hp = in_hp;
}

void Unit::dies() {
    printf("%s is dead.\n", name.c_str());
}

void Unit::setBonus(const Unit_stats in_stats) {
    bonus_stats = in_stats;
}

Unit_stats Unit::getBonus() {
    return(bonus_stats);
}

void Unit::setMalus(const Unit_stats in_stats) {
    malus_stats = in_stats;
}

Unit_stats Unit::getMalus() {
    return(malus_stats);
}

void Unit::setCaps(const Unit_stats in_stats) {
    caps_stats = in_stats;
}

Unit_stats Unit::getCaps() {
    return(caps_stats);
}

void Unit::setStats(const Unit_stats in_stats) {
    current_stats = in_stats;
    current_hp = current_stats.hp;
}

Unit_stats Unit::getStats() {
    return (base_stats);
}

void Unit::setBases(const Unit_stats in_stats) {
    base_stats = in_stats;
    current_stats = in_stats;
    current_hp = base_stats.hp;
}
Unit_stats Unit::getBases() {
    return(base_stats);
}

void Unit::setGrowths(const Unit_stats in_growths) {
    growths = in_growths;
}

Unit_stats Unit::getGrowths() {
    return(growths);
}

bool Unit::isEquippable(std::string in_name) {
    bool equippable = false;
    // std::string wpn_type = all_weapons[in_name].getType();
    // for (int i = 0; i < equippables.size(); i++) {
    //     if (equippables[i] == wpn_type) {
    //         equippable = true;
    //     }
    // }
    return(equippable);
} 

bool Unit::canAttack() {
    bool out;
    struct Wpn_types {
        unsigned short int left;
        unsigned short int right;
    } wpn_types;
    if (equipped.left > 0) {
        wpn_types.left = all_weapons[equipment[equipped.left].id].getType();
        if ((wpn_types.left != WPN_TYPE::SHIELD)  & (wpn_types.left != WPN_TYPE::TRINKET) & (wpn_types.left != WPN_TYPE::STAFF)) {
            out = true;
        } 
    }
    if (equipped.right > 0) {
        wpn_types.right = all_weapons[equipment[equipped.right].id].getType();
        if ((wpn_types.right != WPN_TYPE::SHIELD)  & (wpn_types.right != WPN_TYPE::TRINKET) & (wpn_types.right != WPN_TYPE::STAFF)) {
            out = true;
         } 
    }
    return(out);
}

// bool Unit::dmgType() { //Useless?
//     // Assumes canAttack().
//     struct Dmg_types{
//         bool left = false;
//         bool right = false;
//     } dmg_types;    
//     struct Wpn_types{
//         std::string left;
//         std::string right;
//     } wpn_types;
//     bool out = false;

//     wpn_types.left = all_weapons[equipment[equipped.left].name].getType();
//     wpn_types.right = all_weapons[equipment[equipped.right].name].getType();


//     if ((wpn_types.right.empty()) & (wpn_types.right.empty()) ) {

//     }
//     if ((wpn_types.left != "shield") & (wpn_types.left != "trinket")  & (wpn_types.left != "staff")) {
//         dmg_types.left = all_weapons[equipment[equipped.left].name].getStats().dmg_type;
//     }

//     if (wpn_types.right != "shield") & (wpn_types.left != "trinket")  & (wpn_types.left != "staff") {
//         dmg_types.right = all_weapons[equipment[equipped.right].name].getStats().dmg_type;
//     }

//     // Classes who can have different weapons in both hands.
//     // Battlemage. Angel. Demon.
//     // The only important case is battlemage attacking with sword and trinket. But since we add all mights... Battlemage infusion should have constant damage output based on mage's magic power.
//     // Can battlemages give infused weapons to other units? Yes.
//     if (dmg_types.right != dmg_types.left) {

//     } else {
//         out = dmg_types.right;   
//     }

//     return(out);
// }

unsigned char Unit::totalMight(bool dmg_type) {
    // Damage type is determined by the main weapon of attack.
    unsigned char unit_power = 0;
    unsigned char wpn_dmg;

    if (!dmg_type) { // Physical dmg.
        wpn_dmg = temp_wpn.Pmight;
        unit_power = current_stats.str;
    } else { // Magical dmg.
        wpn_dmg = temp_wpn.Mmight;
        unit_power = current_stats.mag;
    };

    short int total_might = wpn_dmg + unit_power;

    return (total_might);
}

unsigned char Unit::totalDef(bool dmg_type){
    unsigned char total_def = 0;
    // if (dmg_type){
    //     total_def += current_stats.res;
    //     total_def += all_weapons[equipment[equipped.left].name].getBonus().res;
    //     total_def += all_weapons[equipment[equipped.right].name].getBonus().res;
    //     if (all_weapons[equipment[equipped.right].name].getType() == "shield") {
    //         total_def += all_weapons[equipment[equipped.right].name].getStats().Mmight;
    //     }
    //     if (all_weapons[equipment[equipped.left].name].getType() == "shield") {
    //         total_def += all_weapons[equipment[equipped.left].name].getStats().Mmight;
    //     }
    // } else {
    //     total_def += current_stats.def;
    //     total_def += all_weapons[equipment[equipped.left].name].getBonus().def;
    //     total_def += all_weapons[equipment[equipped.right].name].getBonus().def;
    //     if (all_weapons[equipment[equipped.right].name].getType() == "shield") {
    //         total_def += all_weapons[equipment[equipped.right].name].getStats().Pmight;
    //     }
    //     if (all_weapons[equipment[equipped.left].name].getType() == "shield") {
    //         total_def += all_weapons[equipment[equipped.left].name].getStats().Pmight;
    //     }
    // }
    return(total_def);
}   


std::string Unit::getName() {
    return (name);
}

void Unit::setName(const std::string in_name) {
    name = in_name;
}

void Unit::setName(const char in_name) {
    name = in_name;
}

void Unit::setArmy(const std::string in_army_name) {
    army_name = in_army_name;
}

void Unit::setArmy(const char in_army_name) {
    army_name = in_army_name;
}

Combat_stats Unit::getCombatStats() {
    return(combat_stats);
}

void Unit::combatStats() {
    combat_stats.hit = hit();
    combat_stats.dodge = dodge();
    combat_stats.crit = critical();
    combat_stats.favor = favor();
    speed();
}

unsigned char Unit::hit() {
    //*DESIGN QUESTION* In vesteria saga, dex*3.
    unsigned char supports = 0;
    unsigned char unit_acc = current_stats.dex * 3 + current_stats.luck;
    unsigned char hit = temp_wpn.combat.hit + unit_acc + supports;
    return (hit);
}

unsigned char Unit::dodge() {
    unsigned char supports = 0;
    unsigned char terrain_dodge = 0;
    unsigned char unit_dodge = current_stats.dex * 2 + current_stats.luck;
    unsigned char dodge = terrain_dodge + unit_dodge + supports;
    return (dodge);
}

unsigned char Unit::critical() {
    unsigned char supports = 0 ;
    unsigned char unit_skill = 0;
    unsigned char critical = temp_wpn.combat.crit + unit_skill + supports;
    return (critical);
}

unsigned char Unit::favor() {
    unsigned char supports = 0 ;
    unsigned char favor = (ceil(current_stats.luck / 2.)) + supports;
    return (favor);
}

bool Unit::canRetaliate(Unit * enemy) const {
    short int * unit_position;
    short int * enemy_position;
    bool retaliates = false;

    enemy_position = enemy->getPos();
    unsigned char distance = std::abs(enemy_position[0] - position[0]) + std::abs(enemy_position[1] - position[1]);

    for (int i = 0; i < 3; i++) {
        if ((distance >= temp_wpn.range[0]) && (distance <= temp_wpn.range[1])) {
            retaliates = 1;
        }
    }
    return (retaliates);
}

bool Unit::canDouble(Unit * enemy) {
    speed();
    bool doubles = ((current_speed - enemy->speed()) > 4);
    return (doubles);
}

char Unit::speed() {
    //*DESIGN QUESTION* What should be the influence of weapons?
    // Average of Con and Str? Con+Str/2?
    char current_speed = current_stats.agi - temp_wpn.wgt + current_stats.con + current_stats.str/2;
    return(current_speed);
}

void Unit::write(const char * filename, const char * mode) {
    FILE * fp;
    fp = fopen(filename, mode);
    fprintf(fp, "%s \n", name.c_str());
    if (sex) {
        fprintf(fp, "%s %s", "M", class_name.c_str());
    } else {
        fprintf(fp, "%s %s", "F", class_name.c_str());
    }
    fprintf(fp, "Stats, HP, Str, Mag, Skl, Spd, Luck, Def, Res, Con, Move\n");
    fprintf(fp, "Base stats,\t%d,\t%d,\t%d,\t%d,\t%d,\t%d,\t%d,\t%d,\t%d,\t%d,\t%d\n", base_stats.hp, base_stats.str, base_stats.mag, base_stats.dex, base_stats.agi, base_stats.luck, base_stats.def, base_stats.res, base_stats.con, base_stats.move, base_stats.prof);
    fprintf(fp, "Growths,\t%d,\t%d,\t%d,\t%d,\t%d,\t%d,\t%d,\t%d,\t%d,\t%d,\t%d\n", growths.hp, growths.str, growths.mag, growths.dex, growths.agi, growths.luck, growths.def, growths.res, growths.con, growths.move, growths.prof);
    fprintf(fp, "Caps,\t\t%d,\t%d,\t%d,\t%d,\t%d,\t%d,\t%d,\t%d,\t%d,\t%d,\t%d\n", caps_stats.hp, caps_stats.str, caps_stats.mag, caps_stats.dex, caps_stats.agi, caps_stats.luck, caps_stats.def, caps_stats.res, caps_stats.con, caps_stats.move, caps_stats.prof);
    fprintf(fp, "\n");
    fclose(fp);
}

std::vector<Unit> all_units(UNIT::END);
std::vector<Unit> loaded_units;

void baseUnits() {
    printf("Making base units \n");
    Unit temp_unit;
    Unit_stats temp;
    Inventory_item temp_wpn;
    //hp,str,mag,skl,spd,luck,def,res,con,move
    temp = {17,  6,  2,  7,  7,   7,  4,  5,  6, 5};
    temp_unit = Unit("Erwin", UNIT_CLASS::MERCENARY, temp, "M");
    temp = {48, 14, 25, 32, 34,  28, 19, 40, 15, 0};
    temp_unit.setCaps(temp);
    temp = {60, 50, 20, 60, 70,  40, 30, 20,  10, 0};
    temp_unit.setGrowths(temp);
    temp_unit.setExp(0);
    temp_unit.write("unit_test.txt");
    all_units[UNIT::ERWIN] = temp_unit;
    
    temp = {18,  6,  2,  7,  7,   7,  4,  5,  6, 7};
    temp_unit = Unit("Reliable", UNIT_CLASS::CAVALIER, temp, "M");
    temp = {48, 14, 25, 32, 34,  28, 19, 40, 15};
    temp_unit.setCaps(temp);
    temp = {60, 50, 20, 60, 70,  40, 30, 20,  10, 0};
    temp_unit.setGrowths(temp);
    temp_unit.setExp(100);
    all_units[UNIT::RELIABLE] = temp_unit;
    
    temp = {19,  6,  2,  7,  7,   7,  4,  5,  6,  7};
    temp_unit = Unit("Coward", UNIT_CLASS::CAVALIER, temp, "M");
    temp = {48, 14, 25, 32, 34,  28, 19, 40, 15};
    temp_unit.setCaps(temp);
    temp = {60, 50, 20, 60, 70,  40, 30, 20,  10, 0};
    temp_unit.setGrowths(temp);
    temp_unit.setExp(200);
    all_units[UNIT::COWARD] = temp_unit;
    
    temp = {20,  6,  2,  7,  7,   7,  4,  5,  6,  6};
    temp_unit = Unit("1H Jaigen", UNIT_CLASS::MOUSQUETAIRE, temp, "M");
    temp = {48, 14, 25, 32, 34,  28, 19, 40, 15};
    temp_unit.setCaps(temp);
    temp = {60, 50, 20, 60, 70,  40, 30, 20,  10, 0};
    temp_unit.setGrowths(temp);
    temp_unit.setExp(2200);
    all_units[UNIT::JAIGEN1H] = temp_unit;

    temp = {14,  6,  2,  7,  7,   7,  4,  5,  6,  5};
    temp_unit = Unit("Kiara", UNIT_CLASS::CLERIC, temp, "F");
    temp = {48, 14, 25, 32, 34,  28, 19, 40, 15};
    temp_unit.setCaps(temp);
    temp = {60, 50, 20, 60, 70,  40, 30, 20,  10, 0};
    temp_unit.setGrowths(temp);
    temp_unit.setExp(100);
    all_units[UNIT::KIARA] = temp_unit;
    
    temp = {16,  6,  2,  7,  7,   7,  4,  5,  6,  6};
    temp_unit = Unit("Hottie", UNIT_CLASS::PICKPOCKET, temp, "M");
    temp = {48, 14, 25, 32, 34,  28, 19, 40, 15};
    temp_unit.setCaps(temp);
    temp = {60, 50, 20, 60, 70,  40, 30, 20,  10, 0};
    temp_unit.setGrowths(temp);
    temp_unit.setExp(2200);
    all_units[UNIT::HOTTIE] = temp_unit;
    
    temp = {22,  4,  5,  7,  6,   8,  4,  6,  5, 5}; // 4 or 5?
    temp_unit = Unit("Servil", UNIT_CLASS::KNIGHT, temp, "M");
    temp = {48, 14, 25, 32, 34,  28, 19, 40, 15};
    temp_unit.setCaps(temp);
    temp = {60, 50, 20, 60, 70,  40, 30, 20,  10, 0};
    temp_unit.setGrowths(temp);
    temp_unit.setExp(500);
    all_units[UNIT::SERVIL] = temp_unit;
    
    temp = {34,  4,  5,  7,  6,   8,  4,  6,  5, 5};
    temp_unit = Unit("Pérignon", UNIT_CLASS::MAGE, temp, "M");
    temp = {48, 14, 25, 32, 34,  28, 19, 40, 15};
    temp_unit.setCaps(temp);
    temp = {60, 50, 20, 60, 70,  40, 30, 20,  10, 0};
    temp_unit.setGrowths(temp);
    temp_unit.setExp(1200);
    all_units[UNIT::PERIGNON] = temp_unit;
    
    temp = {15,  4,  5,  7,  6,   8,  4,  6,  5, 5};
    temp_unit = Unit("Poet", UNIT_CLASS::MAGE, temp, "M");
    temp = {48, 14, 25, 32, 34,  28, 19, 40, 15};
    temp_unit.setCaps(temp);
    temp = {60, 50, 20, 60, 70,  40, 30, 20,  10, 0};
    temp_unit.setGrowths(temp);
    temp_unit.setExp(400);
    all_units[UNIT::POET] = temp_unit;
    
    temp = {15,  4,  5,  7,  6,   8,  4,  6,  5, 5};
    temp_unit = Unit("Silou", UNIT_CLASS::MAGE, temp, "F");
    temp = {48, 14, 25, 32, 34,  28, 19, 40, 15};
    temp_unit.setCaps(temp);
    temp = {60, 50, 20, 60, 70,  40, 30, 20,  10, 0};
    temp_unit.setGrowths(temp);
    temp_unit.setExp(400);
    temp_wpn.id = WEAPON::BALL_LIGHTNING;
    temp_unit.addEquipment(temp_wpn);
    all_units[UNIT::SILOU] = temp_unit;
}

void genericEnemyUnits() {
    printf("Making generic enemies\n");
    Unit_stats temp;
    Unit temp_unit;
    //hp,str,mag,skl,spd,luck,def,res,con,move

    temp = {15,  4,  5,  7,  6,   8,  4,  6,  5, 5};
    temp_unit = Unit("Bandit", UNIT_CLASS::BANDIT, temp, "M");
    temp = {48, 14, 25, 32, 34,  28, 19, 40, 15};
    temp_unit.setCaps(temp);
    temp = {60, 50, 20, 60, 70,  40, 30, 20,  10, 0};
    temp_unit.setGrowths(temp);
    temp_unit.setExp(0);
    all_units[UNIT::GENERIC_BANDIT] = temp_unit;

    temp = {15,  4,  5,  7,  6,   8,  4,  6,  5, 5};
    temp_unit = Unit("Duelist", UNIT_CLASS::DUELIST, temp, "M");
    temp = {48, 14, 25, 32, 34,  28, 19, 40, 15};
    temp_unit.setCaps(temp);
    temp = {60, 50, 20, 60, 70,  40, 30, 20,  10, 0};
    temp_unit.setGrowths(temp);
    temp_unit.setExp(0);
    all_units[UNIT::GENERIC_DUELIST] = temp_unit;

    temp = {15,  4,  5,  7,  6,   8,  4,  6,  5, 5};
    temp_unit = Unit("Pickpocket", UNIT_CLASS::PICKPOCKET, temp, "M");
    temp = {48, 14, 25, 32, 34,  28, 19, 40, 15};
    temp_unit.setCaps(temp);
    temp = {60, 50, 20, 60, 70,  40, 30, 20,  10, 0};
    temp_unit.setGrowths(temp);
    temp_unit.setExp(0);
    all_units[UNIT::GENERIC_PICKPOCKET] = temp_unit;

    temp = {15,  4,  5,  7,  6,   8,  4,  6,  5, 5};
    temp_unit = Unit("Thief", UNIT_CLASS::THIEF, temp, "M");
    temp = {48, 14, 25, 32, 34,  28, 19, 40, 15};
    temp_unit.setCaps(temp);
    temp = {60, 50, 20, 60, 70,  40, 30, 20,  10, 0};
    temp_unit.setGrowths(temp);
    temp_unit.setExp(0);
    all_units[UNIT::GENERIC_THIEF] = temp_unit;    

    temp = {15,  4,  5,  7,  6,   8,  4,  6,  5, 5};
    temp_unit = Unit("Assassin", UNIT_CLASS::ASSASSIN, temp, "M");
    temp = {48, 14, 25, 32, 34,  28, 19, 40, 15};
    temp_unit.setCaps(temp);
    temp = {60, 50, 20, 60, 70,  40, 30, 20,  10, 0};
    temp_unit.setGrowths(temp);
    temp_unit.setExp(0);
    all_units[UNIT::GENERIC_ASSASSIN] = temp_unit;

    temp = {15,  4,  5,  7,  6,   8,  4,  6,  5, 5};
    temp_unit = Unit("Archer", UNIT_CLASS::ARCHER, temp, "M");
    temp = {48, 14, 25, 32, 34,  28, 19, 40, 15};
    temp_unit.setCaps(temp);
    temp = {60, 50, 20, 60, 70,  40, 30, 20,  10, 0};
    temp_unit.setGrowths(temp);
    temp_unit.setExp(0);
    all_units[UNIT::GENERIC_ARCHER] = temp_unit;

    temp = {15,  4,  5,  7,  6,   8,  4,  6,  5, 5};
    temp_unit = Unit("Marksman", UNIT_CLASS::MARKSMAN, temp, "M");
    temp = {48, 14, 25, 32, 34,  28, 19, 40, 15};
    temp_unit.setCaps(temp);
    temp = {60, 50, 20, 60, 70,  40, 30, 20,  10, 0};
    temp_unit.setGrowths(temp);
    temp_unit.setExp(0);
    all_units[UNIT::GENERIC_MARKSMAN] = temp_unit;

    temp = {15,  4,  5,  7,  6,   8,  4,  6,  5, 5};
    temp_unit = Unit("Mercenary", UNIT_CLASS::MERCENARY, temp, "M");
    temp = {48, 14, 25, 32, 34,  28, 19, 40, 15};
    temp_unit.setCaps(temp);
    temp = {60, 50, 20, 60, 70,  40, 30, 20,  10, 0};
    temp_unit.setGrowths(temp);
    temp_unit.setExp(0);
    all_units[UNIT::GENERIC_MERCENARY] = temp_unit;

    temp = {15,  4,  5,  7,  6,   8,  4,  6,  5, 5};
    temp_unit = Unit("Hero", UNIT_CLASS::HERO, temp, "M");
    temp = {48, 14, 25, 32, 34,  28, 19, 40, 15};
    temp_unit.setCaps(temp);
    temp = {60, 50, 20, 60, 70,  40, 30, 20,  10, 0};
    temp_unit.setGrowths(temp);
    temp_unit.setExp(0);
    all_units[UNIT::GENERIC_HERO] = temp_unit;

    temp = {15,  4,  5,  7,  6,   8,  4,  6,  5, 5};
    temp_unit = Unit("Corsair", UNIT_CLASS::CORSAIR, temp, "M");
    temp = {48, 14, 25, 32, 34,  28, 19, 40, 15};
    temp_unit.setCaps(temp);
    temp = {60, 50, 20, 60, 70,  40, 30, 20,  10, 0};
    temp_unit.setGrowths(temp);
    temp_unit.setExp(0);
    all_units[UNIT::GENERIC_CORSAIR] = temp_unit;

    temp = {15,  4,  5,  7,  6,   8,  4,  6,  5, 5};
    temp_unit = Unit("Viking", UNIT_CLASS::VIKING, temp, "M");
    temp = {48, 14, 25, 32, 34,  28, 19, 40, 15};
    temp_unit.setCaps(temp);
    temp = {60, 50, 20, 60, 70,  40, 30, 20,  10, 0};
    temp_unit.setGrowths(temp);
    temp_unit.setExp(0);
    all_units[UNIT::GENERIC_VIKING] = temp_unit;

    temp = {15,  4,  5,  7,  6,   8,  4,  6,  5, 5};
    temp_unit = Unit("Ravager", UNIT_CLASS::RAVAGER, temp, "M");
    temp = {48, 14, 25, 32, 34,  28, 19, 40, 15};
    temp_unit.setCaps(temp);
    temp = {60, 50, 20, 60, 70,  40, 30, 20,  10, 0};
    temp_unit.setGrowths(temp);
    temp_unit.setExp(0);
    all_units[UNIT::GENERIC_RAVAGER] = temp_unit;

    temp = {15,  4,  5,  7,  6,   8,  4,  6,  5, 5};
    temp_unit = Unit("Cavalier", UNIT_CLASS::CAVALIER, temp, "M");
    temp = {48, 14, 25, 32, 34,  28, 19, 40, 15};
    temp_unit.setCaps(temp);
    temp = {60, 50, 20, 60, 70,  40, 30, 20,  10, 0};
    temp_unit.setGrowths(temp);
    temp_unit.setExp(0);
    all_units[UNIT::GENERIC_CAVALIER] = temp_unit;

    temp = {15,  4,  5,  7,  6,   8,  4,  6,  5, 5};
    temp_unit = Unit("Paladin", UNIT_CLASS::PALADIN, temp, "M");
    temp = {48, 14, 25, 32, 34,  28, 19, 40, 15};
    temp_unit.setCaps(temp);
    temp = {60, 50, 20, 60, 70,  40, 30, 20,  10, 0};
    temp_unit.setGrowths(temp);
    temp_unit.setExp(0);
    all_units[UNIT::GENERIC_PALADIN] = temp_unit;

    temp = {15,  4,  5,  7,  6,   8,  4,  6,  5, 5};
    temp_unit = Unit("Fencer", UNIT_CLASS::FENCER, temp, "M");
    temp = {48, 14, 25, 32, 34,  28, 19, 40, 15};
    temp_unit.setCaps(temp);
    temp = {60, 50, 20, 60, 70,  40, 30, 20,  10, 0};
    temp_unit.setGrowths(temp);
    temp_unit.setExp(0);
    all_units[UNIT::GENERIC_FENCER] = temp_unit;

    temp = {15,  4,  5,  7,  6,   8,  4,  6,  5, 5};
    temp_unit = Unit("Mousquetaire", UNIT_CLASS::MOUSQUETAIRE, temp, "M");
    temp = {48, 14, 25, 32, 34,  28, 19, 40, 15};
    temp_unit.setCaps(temp);
    temp = {60, 50, 20, 60, 70,  40, 30, 20,  10, 0};
    temp_unit.setGrowths(temp);
    temp_unit.setExp(0);
    all_units[UNIT::GENERIC_MOUSQUETAIRE] = temp_unit;
}

std::vector<Unit> chaptestEnemyUnits() {
    Unit_stats temp_stats;
    Unit temp_unit;
    Inventory_item temp_equipment[DEFAULT::EQUIPMENT_SIZE];
    Equipped temp_equipped;
    Point temp_position;
    std::vector<Unit> made_units;
    temp_stats = {15,  4,  5,  7,  6,   8,  4,  6,  5, 5};
    temp_unit = Unit("Bandit", UNIT_CLASS::BANDIT, temp_stats, "M");
    temp_stats = {48, 14, 25, 32, 34,  28, 19, 40, 15};
    temp_unit.setCaps(temp_stats);
    temp_stats = {60, 50, 20, 60, 70,  40, 30, 20,  10, 0};
    temp_unit.setGrowths(temp_stats);
    temp_unit.setExp(0);
  //temp_equipment[0].id = "Iron Axe";
  //temp_equipment[1].id = "Wooden Shield";
    temp_unit.setEquipment(temp_equipment);
    temp_equipped = {0, 1};
    temp_unit.setEquipped(temp_equipped);
    temp_position = {4, 5};
    temp_unit.setPos(temp_position);
    made_units.push_back(temp_unit);
    return(made_units);
} 

std::vector<Unit> chap1EnemyUnits() {
    Unit_stats temp_stats;
    Unit temp_unit;
    Inventory_item temp_equipment[DEFAULT::EQUIPMENT_SIZE];
    Equipped temp_equipped;
    Point temp_position;
    std::vector<Unit> made_units;
    temp_stats = {15,  4,  5,  7,  6,   8,  4,  6,  5, 5};
    temp_unit = Unit("Bandit", UNIT_CLASS::BANDIT, temp_stats, "M");
    temp_stats = {48, 14, 25, 32, 34,  28, 19, 40, 15};
    temp_unit.setCaps(temp_stats);
    temp_stats = {60, 50, 20, 60, 70,  40, 30, 20,  10, 0};
    temp_unit.setGrowths(temp_stats);
    temp_unit.setExp(0);
  //temp_equipment[0].id = "Iron Axe";
  //temp_equipment[1].id = "Wooden Shield";
    temp_unit.setEquipment(temp_equipment);
    temp_equipped = {0, 1};
    temp_unit.setEquipped(temp_equipped);
    temp_position = {4, 5};
    temp_unit.setPos(temp_position);
    made_units.push_back(temp_unit);
    return(made_units);
} 

std::vector<Unit> chap2EnemyUnits() {
    Unit_stats temp_stats;
    Unit temp_unit;
    Inventory_item temp_equipment[DEFAULT::EQUIPMENT_SIZE];
    Equipped temp_equipped;
    Point temp_position;
    std::vector<Unit> made_units;
    temp_stats = {15,  4,  5,  7,  6,   8,  4,  6,  5, 5};
    temp_unit = Unit("Bandit", UNIT_CLASS::BANDIT, temp_stats, "M");
    temp_stats = {48, 14, 25, 32, 34,  28, 19, 40, 15};
    temp_unit.setCaps(temp_stats);
    temp_stats = {60, 50, 20, 60, 70,  40, 30, 20,  10, 0};
    temp_unit.setGrowths(temp_stats);
    temp_unit.setExp(0);
  //temp_equipment[0].id = "Iron Axe";
  //temp_equipment[1].id = "Wooden Shield";
    temp_unit.setEquipment(temp_equipment);
    temp_equipped = {0, 1};
    temp_unit.setEquipped(temp_equipped);
    temp_position = {4, 5};
    temp_unit.setPos(temp_position);
    made_units.push_back(temp_unit);
    return(made_units);
}

std::vector<Unit> chap3EnemyUnits() {
    Unit_stats temp_stats;
    Unit temp_unit;
    Inventory_item temp_equipment[DEFAULT::EQUIPMENT_SIZE];
    Equipped temp_equipped;
    Point temp_position;
    std::vector<Unit> made_units;
    temp_stats = {15,  4,  5,  7,  6,   8,  4,  6,  5, 5};
    temp_unit = Unit("Bandit", UNIT_CLASS::BANDIT, temp_stats, "M");
    temp_stats = {48, 14, 25, 32, 34,  28, 19, 40, 15};
    temp_unit.setCaps(temp_stats);
    temp_stats = {60, 50, 20, 60, 70,  40, 30, 20,  10, 0};
    temp_unit.setGrowths(temp_stats);
    temp_unit.setExp(0);
  //temp_equipment[0].id = "Iron Axe";
  //temp_equipment[1].id = "Wooden Shield";
    temp_unit.setEquipment(temp_equipment);
    temp_equipped = {0, 1};
    temp_unit.setEquipped(temp_equipped);
    temp_position = {4, 5};
    temp_unit.setPos(temp_position);
    made_units.push_back(temp_unit);
    return(made_units);
} 

std::vector<Unit> chap4EnemyUnits() {
    Unit_stats temp_stats;
    Unit temp_unit;
    Inventory_item temp_equipment[DEFAULT::EQUIPMENT_SIZE];
    Equipped temp_equipped;
    Point temp_position;
    std::vector<Unit> made_units;
    temp_stats = {15,  4,  5,  7,  6,   8,  4,  6,  5, 5};
    temp_unit = Unit("Bandit", UNIT_CLASS::BANDIT, temp_stats, "M");
    temp_stats = {48, 14, 25, 32, 34,  28, 19, 40, 15};
    temp_unit.setCaps(temp_stats);
    temp_stats = {60, 50, 20, 60, 70,  40, 30, 20,  10, 0};
    temp_unit.setGrowths(temp_stats);
    temp_unit.setExp(0);
  //temp_equipment[0].id = "Iron Axe";
  //temp_equipment[1].id = "Wooden Shield";
    temp_unit.setEquipment(temp_equipment);
    temp_equipped = {0, 1};
    temp_unit.setEquipped(temp_equipped);
    temp_position = {4, 5};
    temp_unit.setPos(temp_position);
    made_units.push_back(temp_unit);
    return(made_units);
}

std::vector<Unit> chap5EnemyUnits() {
    Unit_stats temp_stats;
    Unit temp_unit;
    Inventory_item temp_equipment[DEFAULT::EQUIPMENT_SIZE];
    Equipped temp_equipped;
    Point temp_position;
    std::vector<Unit> made_units;
    temp_stats = {15,  4,  5,  7,  6,   8,  4,  6,  5, 5};
    temp_unit = Unit("Bandit", UNIT_CLASS::BANDIT, temp_stats, "M");
    temp_stats = {48, 14, 25, 32, 34,  28, 19, 40, 15};
    temp_unit.setCaps(temp_stats);
    temp_stats = {60, 50, 20, 60, 70,  40, 30, 20,  10, 0};
    temp_unit.setGrowths(temp_stats);
    temp_unit.setExp(0);
  //temp_equipment[0].id = "Iron Axe";
  //temp_equipment[1].id = "Wooden Shield";
    temp_unit.setEquipment(temp_equipment);
    temp_equipped = {0, 1};
    temp_unit.setEquipped(temp_equipped);
    temp_position = {4, 5};
    temp_unit.setPos(temp_position);
    made_units.push_back(temp_unit);
    return(made_units);
}

std::vector<Unit> chap6EnemyUnits() {
    Unit_stats temp_stats;
    Unit temp_unit;
    Inventory_item temp_equipment[DEFAULT::EQUIPMENT_SIZE];
    Equipped temp_equipped;
    Point temp_position;
    std::vector<Unit> made_units;
    temp_stats = {15,  4,  5,  7,  6,   8,  4,  6,  5, 5};
    temp_unit = Unit("Bandit", UNIT_CLASS::BANDIT, temp_stats, "M");
    temp_stats = {48, 14, 25, 32, 34,  28, 19, 40, 15};
    temp_unit.setCaps(temp_stats);
    temp_stats = {60, 50, 20, 60, 70,  40, 30, 20,  10, 0};
    temp_unit.setGrowths(temp_stats);
    temp_unit.setExp(0);
  //temp_equipment[0].id = "Iron Axe";
  //temp_equipment[1].id = "Wooden Shield";
    temp_unit.setEquipment(temp_equipment);
    temp_equipped = {0, 1};
    temp_unit.setEquipped(temp_equipped);
    temp_position = {4, 5};
    temp_unit.setPos(temp_position);
    made_units.push_back(temp_unit);
    return(made_units);
} 

std::vector<Unit> chap7EnemyUnits() {
    Unit_stats temp_stats;
    Unit temp_unit;
    Inventory_item temp_equipment[DEFAULT::EQUIPMENT_SIZE];
    Equipped temp_equipped;
    Point temp_position;
    std::vector<Unit> made_units;
    temp_stats = {15,  4,  5,  7,  6,   8,  4,  6,  5, 5};
    temp_unit = Unit("Bandit", UNIT_CLASS::BANDIT, temp_stats, "M");
    temp_stats = {48, 14, 25, 32, 34,  28, 19, 40, 15};
    temp_unit.setCaps(temp_stats);
    temp_stats = {60, 50, 20, 60, 70,  40, 30, 20,  10, 0};
    temp_unit.setGrowths(temp_stats);
    temp_unit.setExp(0);
  //temp_equipment[0].id = "Iron Axe";
  //temp_equipment[1].id = "Wooden Shield";
    temp_unit.setEquipment(temp_equipment);
    temp_equipped = {0, 1};
    temp_unit.setEquipped(temp_equipped);
    temp_position = {4, 5};
    temp_unit.setPos(temp_position);
    made_units.push_back(temp_unit);
    return(made_units);
}

std::vector<Unit> chap8EnemyUnits() {
    Unit_stats temp_stats;
    Unit temp_unit;
    Inventory_item temp_equipment[DEFAULT::EQUIPMENT_SIZE];
    Equipped temp_equipped;
    Point temp_position;
    std::vector<Unit> made_units;
    temp_stats = {15,  4,  5,  7,  6,   8,  4,  6,  5, 5};
    temp_unit = Unit("Bandit", UNIT_CLASS::BANDIT, temp_stats, "M");
    temp_stats = {48, 14, 25, 32, 34,  28, 19, 40, 15};
    temp_unit.setCaps(temp_stats);
    temp_stats = {60, 50, 20, 60, 70,  40, 30, 20,  10, 0};
    temp_unit.setGrowths(temp_stats);
    temp_unit.setExp(0);
  //temp_equipment[0].id = "Iron Axe";
  //temp_equipment[1].id = "Wooden Shield";
    temp_unit.setEquipment(temp_equipment);
    temp_equipped = {0, 1};
    temp_unit.setEquipped(temp_equipped);
    temp_position = {4, 5};
    temp_unit.setPos(temp_position);
    made_units.push_back(temp_unit);
    return(made_units);
}

std::vector<Unit> chap9EnemyUnits() {
    Unit_stats temp_stats;
    Unit temp_unit;
    Inventory_item temp_equipment[DEFAULT::EQUIPMENT_SIZE];
    Equipped temp_equipped;
    Point temp_position;
    std::vector<Unit> made_units;
    temp_stats = {15,  4,  5,  7,  6,   8,  4,  6,  5, 5};
    temp_unit = Unit("Bandit", UNIT_CLASS::BANDIT, temp_stats, "M");
    temp_stats = {48, 14, 25, 32, 34,  28, 19, 40, 15};
    temp_unit.setCaps(temp_stats);
    temp_stats = {60, 50, 20, 60, 70,  40, 30, 20,  10, 0};
    temp_unit.setGrowths(temp_stats);
    temp_unit.setExp(0);
  //temp_equipment[0].id = "Iron Axe";
  //temp_equipment[1].id = "Wooden Shield";
    temp_unit.setEquipment(temp_equipment);
    temp_equipped = {0, 1};
    temp_unit.setEquipped(temp_equipped);
    temp_position = {4, 5};
    temp_unit.setPos(temp_position);
    made_units.push_back(temp_unit);
    return(made_units);
}

std::vector<Unit> chap10EnemyUnits() {
    Unit_stats temp_stats;
    Unit temp_unit;
    Inventory_item temp_equipment[DEFAULT::EQUIPMENT_SIZE];
    Equipped temp_equipped;
    Point temp_position;
    std::vector<Unit> made_units;
    temp_stats = {15,  4,  5,  7,  6,   8,  4,  6,  5, 5};
    temp_unit = Unit("Bandit", UNIT_CLASS::BANDIT, temp_stats, "M");
    temp_stats = {48, 14, 25, 32, 34,  28, 19, 40, 15};
    temp_unit.setCaps(temp_stats);
    temp_stats = {60, 50, 20, 60, 70,  40, 30, 20,  10, 0};
    temp_unit.setGrowths(temp_stats);
    temp_unit.setExp(0);
  //temp_equipment[0].id = "Iron Axe";
  //temp_equipment[1].id = "Wooden Shield";
    temp_unit.setEquipment(temp_equipment);
    temp_equipped = {0, 1};
    temp_unit.setEquipped(temp_equipped);
    temp_position = {4, 5};
    temp_unit.setPos(temp_position);
    made_units.push_back(temp_unit);
    return(made_units);
}

std::vector<Unit> chap11EnemyUnits() {
    Unit_stats temp_stats;
    Unit temp_unit;
    Inventory_item temp_equipment[DEFAULT::EQUIPMENT_SIZE];
    Equipped temp_equipped;
    Point temp_position;
    std::vector<Unit> made_units;
    temp_stats = {15,  4,  5,  7,  6,   8,  4,  6,  5, 5};
    temp_unit = Unit("Bandit", UNIT_CLASS::BANDIT, temp_stats, "M");
    temp_stats = {48, 14, 25, 32, 34,  28, 19, 40, 15};
    temp_unit.setCaps(temp_stats);
    temp_stats = {60, 50, 20, 60, 70,  40, 30, 20,  10, 0};
    temp_unit.setGrowths(temp_stats);
    temp_unit.setExp(0);
  //temp_equipment[0].id = "Iron Axe";
  //temp_equipment[1].id = "Wooden Shield";
    temp_unit.setEquipment(temp_equipment);
    temp_equipped = {0, 1};
    temp_unit.setEquipped(temp_equipped);
    temp_position = {4, 5};
    temp_unit.setPos(temp_position);
    made_units.push_back(temp_unit);
    return(made_units);
}

std::vector<Unit> chap12EnemyUnits() {
    Unit_stats temp_stats;
    Unit temp_unit;
    Inventory_item temp_equipment[DEFAULT::EQUIPMENT_SIZE];
    Equipped temp_equipped;
    Point temp_position;
    std::vector<Unit> made_units;
    temp_stats = {15,  4,  5,  7,  6,   8,  4,  6,  5, 5};
    temp_unit = Unit("Bandit", UNIT_CLASS::BANDIT, temp_stats, "M");
    temp_stats = {48, 14, 25, 32, 34,  28, 19, 40, 15};
    temp_unit.setCaps(temp_stats);
    temp_stats = {60, 50, 20, 60, 70,  40, 30, 20,  10, 0};
    temp_unit.setGrowths(temp_stats);
    temp_unit.setExp(0);
  //temp_equipment[0].id = "Iron Axe";
  //temp_equipment[1].id = "Wooden Shield";
    temp_unit.setEquipment(temp_equipment);
    temp_equipped = {0, 1};
    temp_unit.setEquipped(temp_equipped);
    temp_position = {4, 5};
    temp_unit.setPos(temp_position);
    made_units.push_back(temp_unit);
    return(made_units);
}

std::vector<Unit> chap13EnemyUnits() {
    Unit_stats temp_stats;
    Unit temp_unit;
    Inventory_item temp_equipment[DEFAULT::EQUIPMENT_SIZE];
    Equipped temp_equipped;
    Point temp_position;
    std::vector<Unit> made_units;
    temp_stats = {15,  4,  5,  7,  6,   8,  4,  6,  5, 5};
    temp_unit = Unit("Bandit", UNIT_CLASS::BANDIT, temp_stats, "M");
    temp_stats = {48, 14, 25, 32, 34,  28, 19, 40, 15};
    temp_unit.setCaps(temp_stats);
    temp_stats = {60, 50, 20, 60, 70,  40, 30, 20,  10, 0};
    temp_unit.setGrowths(temp_stats);
    temp_unit.setExp(0);
  //temp_equipment[0].id = "Iron Axe";
  //temp_equipment[1].id = "Wooden Shield";
    temp_unit.setEquipment(temp_equipment);
    temp_equipped = {0, 1};
    temp_unit.setEquipped(temp_equipped);
    temp_position = {4, 5};
    temp_unit.setPos(temp_position);
    made_units.push_back(temp_unit);
    return(made_units);
}

std::vector<Unit> chap14EnemyUnits() {
    Unit_stats temp_stats;
    Unit temp_unit;
    Inventory_item temp_equipment[DEFAULT::EQUIPMENT_SIZE];
    Equipped temp_equipped;
    Point temp_position;
    std::vector<Unit> made_units;
    temp_stats = {15,  4,  5,  7,  6,   8,  4,  6,  5, 5};
    temp_unit = Unit("Bandit", UNIT_CLASS::BANDIT, temp_stats, "M");
    temp_stats = {48, 14, 25, 32, 34,  28, 19, 40, 15};
    temp_unit.setCaps(temp_stats);
    temp_stats = {60, 50, 20, 60, 70,  40, 30, 20,  10, 0};
    temp_unit.setGrowths(temp_stats);
    temp_unit.setExp(0);
  //temp_equipment[0].id = "Iron Axe";
  //temp_equipment[1].id = "Wooden Shield";
    temp_unit.setEquipment(temp_equipment);
    temp_equipped = {0, 1};
    temp_unit.setEquipped(temp_equipped);
    temp_position = {4, 5};
    temp_unit.setPos(temp_position);
    made_units.push_back(temp_unit);
    return(made_units);
}

std::vector<Unit> chap15EnemyUnits() {
    Unit_stats temp_stats;
    Unit temp_unit;
    Inventory_item temp_equipment[DEFAULT::EQUIPMENT_SIZE];
    Equipped temp_equipped;
    Point temp_position;
    std::vector<Unit> made_units;
    temp_stats = {15,  4,  5,  7,  6,   8,  4,  6,  5, 5};
    temp_unit = Unit("Bandit", UNIT_CLASS::BANDIT, temp_stats, "M");
    temp_stats = {48, 14, 25, 32, 34,  28, 19, 40, 15};
    temp_unit.setCaps(temp_stats);
    temp_stats = {60, 50, 20, 60, 70,  40, 30, 20,  10, 0};
    temp_unit.setGrowths(temp_stats);
    temp_unit.setExp(0);
  //temp_equipment[0].id = "Iron Axe";
  //temp_equipment[1].id = "Wooden Shield";
    temp_unit.setEquipment(temp_equipment);
    temp_equipped = {0, 1};
    temp_unit.setEquipped(temp_equipped);
    temp_position = {4, 5};
    temp_unit.setPos(temp_position);
    made_units.push_back(temp_unit);
    return(made_units);
}

std::vector<Unit> chap16EnemyUnits() {
    Unit_stats temp_stats;
    Unit temp_unit;
    Inventory_item temp_equipment[DEFAULT::EQUIPMENT_SIZE];
    Equipped temp_equipped;
    Point temp_position;
    std::vector<Unit> made_units;
    temp_stats = {15,  4,  5,  7,  6,   8,  4,  6,  5, 5};
    temp_unit = Unit("Bandit", UNIT_CLASS::BANDIT, temp_stats, "M");
    temp_stats = {48, 14, 25, 32, 34,  28, 19, 40, 15};
    temp_unit.setCaps(temp_stats);
    temp_stats = {60, 50, 20, 60, 70,  40, 30, 20,  10, 0};
    temp_unit.setGrowths(temp_stats);
    temp_unit.setExp(0);
  //temp_equipment[0].id = "Iron Axe";
  //temp_equipment[1].id = "Wooden Shield";
    temp_unit.setEquipment(temp_equipment);
    temp_equipped = {0, 1};
    temp_unit.setEquipped(temp_equipped);
    temp_position = {4, 5};
    temp_unit.setPos(temp_position);
    made_units.push_back(temp_unit);
    return(made_units);
}

std::vector<Unit> chap17EnemyUnits() {
    Unit_stats temp_stats;
    Unit temp_unit;
    Inventory_item temp_equipment[DEFAULT::EQUIPMENT_SIZE];
    Equipped temp_equipped;
    Point temp_position;
    std::vector<Unit> made_units;
    temp_stats = {15,  4,  5,  7,  6,   8,  4,  6,  5, 5};
    temp_unit = Unit("Bandit", UNIT_CLASS::BANDIT, temp_stats, "M");
    temp_stats = {48, 14, 25, 32, 34,  28, 19, 40, 15};
    temp_unit.setCaps(temp_stats);
    temp_stats = {60, 50, 20, 60, 70,  40, 30, 20,  10, 0};
    temp_unit.setGrowths(temp_stats);
    temp_unit.setExp(0);
  //temp_equipment[0].id = "Iron Axe";
  //temp_equipment[1].id = "Wooden Shield";
    temp_unit.setEquipment(temp_equipment);
    temp_equipped = {0, 1};
    temp_unit.setEquipped(temp_equipped);
    temp_position = {4, 5};
    temp_unit.setPos(temp_position);
    made_units.push_back(temp_unit);
    return(made_units);
}

std::vector<Unit> chap18EnemyUnits() {
    Unit_stats temp_stats;
    Unit temp_unit;
    Inventory_item temp_equipment[DEFAULT::EQUIPMENT_SIZE];
    Equipped temp_equipped;
    Point temp_position;
    std::vector<Unit> made_units;
    temp_stats = {15,  4,  5,  7,  6,   8,  4,  6,  5, 5};
    temp_unit = Unit("Bandit", UNIT_CLASS::BANDIT, temp_stats, "M");
    temp_stats = {48, 14, 25, 32, 34,  28, 19, 40, 15};
    temp_unit.setCaps(temp_stats);
    temp_stats = {60, 50, 20, 60, 70,  40, 30, 20,  10, 0};
    temp_unit.setGrowths(temp_stats);
    temp_unit.setExp(0);
  //temp_equipment[0].id = "Iron Axe";
  //temp_equipment[1].id = "Wooden Shield";
    temp_unit.setEquipment(temp_equipment);
    temp_equipped = {0, 1};
    temp_unit.setEquipped(temp_equipped);
    temp_position = {4, 5};
    temp_unit.setPos(temp_position);
    made_units.push_back(temp_unit);
    return(made_units);
}

std::vector<Unit> chap19EnemyUnits() {
    Unit_stats temp_stats;
    Unit temp_unit;
    Inventory_item temp_equipment[DEFAULT::EQUIPMENT_SIZE];
    Equipped temp_equipped;
    Point temp_position;
    std::vector<Unit> made_units;
    temp_stats = {15,  4,  5,  7,  6,   8,  4,  6,  5, 5};
    temp_unit = Unit("Bandit", UNIT_CLASS::BANDIT, temp_stats, "M");
    temp_stats = {48, 14, 25, 32, 34,  28, 19, 40, 15};
    temp_unit.setCaps(temp_stats);
    temp_stats = {60, 50, 20, 60, 70,  40, 30, 20,  10, 0};
    temp_unit.setGrowths(temp_stats);
    temp_unit.setExp(0);
  //temp_equipment[0].id = "Iron Axe";
  //temp_equipment[1].id = "Wooden Shield";
    temp_unit.setEquipment(temp_equipment);
    temp_equipped = {0, 1};
    temp_unit.setEquipped(temp_equipped);
    temp_position = {4, 5};
    temp_unit.setPos(temp_position);
    made_units.push_back(temp_unit);
    return(made_units);
}

std::vector<Unit> chap20EnemyUnits() {
    Unit_stats temp_stats;
    Unit temp_unit;
    Inventory_item temp_equipment[DEFAULT::EQUIPMENT_SIZE];
    Equipped temp_equipped;
    Point temp_position;
    std::vector<Unit> made_units;
    temp_stats = {15,  4,  5,  7,  6,   8,  4,  6,  5, 5};
    temp_unit = Unit("Bandit", UNIT_CLASS::BANDIT, temp_stats, "M");
    temp_stats = {48, 14, 25, 32, 34,  28, 19, 40, 15};
    temp_unit.setCaps(temp_stats);
    temp_stats = {60, 50, 20, 60, 70,  40, 30, 20,  10, 0};
    temp_unit.setGrowths(temp_stats);
    temp_unit.setExp(0);
  //temp_equipment[0].id = "Iron Axe";
  //temp_equipment[1].id = "Wooden Shield";
    temp_unit.setEquipment(temp_equipment);
    temp_equipped = {0, 1};
    temp_unit.setEquipped(temp_equipped);
    temp_position = {4, 5};
    temp_unit.setPos(temp_position);
    made_units.push_back(temp_unit);
    return(made_units);
}

std::vector<Unit> chap21EnemyUnits() {
    Unit_stats temp_stats;
    Unit temp_unit;
    Inventory_item temp_equipment[DEFAULT::EQUIPMENT_SIZE];
    Equipped temp_equipped;
    Point temp_position;
    std::vector<Unit> made_units;
    temp_stats = {15,  4,  5,  7,  6,   8,  4,  6,  5, 5};
    temp_unit = Unit("Bandit", UNIT_CLASS::BANDIT, temp_stats, "M");
    temp_stats = {48, 14, 25, 32, 34,  28, 19, 40, 15};
    temp_unit.setCaps(temp_stats);
    temp_stats = {60, 50, 20, 60, 70,  40, 30, 20,  10, 0};
    temp_unit.setGrowths(temp_stats);
    temp_unit.setExp(0);
  //temp_equipment[0].id = "Iron Axe";
  //temp_equipment[1].id = "Wooden Shield";
    temp_unit.setEquipment(temp_equipment);
    temp_equipped = {0, 1};
    temp_unit.setEquipped(temp_equipped);
    temp_position = {4, 5};
    temp_unit.setPos(temp_position);
    made_units.push_back(temp_unit);
    return(made_units);
}

std::vector<Unit> chap22EnemyUnits() {
    Unit_stats temp_stats;
    Unit temp_unit;
    Inventory_item temp_equipment[DEFAULT::EQUIPMENT_SIZE];
    Equipped temp_equipped;
    Point temp_position;
    std::vector<Unit> made_units;
    temp_stats = {15,  4,  5,  7,  6,   8,  4,  6,  5, 5};
    temp_unit = Unit("Bandit", UNIT_CLASS::BANDIT, temp_stats, "M");
    temp_stats = {48, 14, 25, 32, 34,  28, 19, 40, 15};
    temp_unit.setCaps(temp_stats);
    temp_stats = {60, 50, 20, 60, 70,  40, 30, 20,  10, 0};
    temp_unit.setGrowths(temp_stats);
    temp_unit.setExp(0);
  //temp_equipment[0].id = "Iron Axe";
  //temp_equipment[1].id = "Wooden Shield";
    temp_unit.setEquipment(temp_equipment);
    temp_equipped = {0, 1};
    temp_unit.setEquipped(temp_equipped);
    temp_position = {4, 5};
    temp_unit.setPos(temp_position);
    made_units.push_back(temp_unit);
    return(made_units);
}

std::vector<Unit> chap23EnemyUnits() {
    Unit_stats temp_stats;
    Unit temp_unit;
    Inventory_item temp_equipment[DEFAULT::EQUIPMENT_SIZE];
    Equipped temp_equipped;
    Point temp_position;
    std::vector<Unit> made_units;
    temp_stats = {15,  4,  5,  7,  6,   8,  4,  6,  5, 5};
    temp_unit = Unit("Bandit", UNIT_CLASS::BANDIT, temp_stats, "M");
    temp_stats = {48, 14, 25, 32, 34,  28, 19, 40, 15};
    temp_unit.setCaps(temp_stats);
    temp_stats = {60, 50, 20, 60, 70,  40, 30, 20,  10, 0};
    temp_unit.setGrowths(temp_stats);
    temp_unit.setExp(0);
  //temp_equipment[0].id = "Iron Axe";
  //temp_equipment[1].id = "Wooden Shield";
    temp_unit.setEquipment(temp_equipment);
    temp_equipped = {0, 1};
    temp_unit.setEquipped(temp_equipped);
    temp_position = {4, 5};
    temp_unit.setPos(temp_position);
    made_units.push_back(temp_unit);
    return(made_units);
}

std::vector<Unit> chap24EnemyUnits() {
    Unit_stats temp_stats;
    Unit temp_unit;
    Inventory_item temp_equipment[DEFAULT::EQUIPMENT_SIZE];
    Equipped temp_equipped;
    Point temp_position;
    std::vector<Unit> made_units;
    temp_stats = {15,  4,  5,  7,  6,   8,  4,  6,  5, 5};
    temp_unit = Unit("Bandit", UNIT_CLASS::BANDIT, temp_stats, "M");
    temp_stats = {48, 14, 25, 32, 34,  28, 19, 40, 15};
    temp_unit.setCaps(temp_stats);
    temp_stats = {60, 50, 20, 60, 70,  40, 30, 20,  10, 0};
    temp_unit.setGrowths(temp_stats);
    temp_unit.setExp(0);
  //temp_equipment[0].id = "Iron Axe";
  //temp_equipment[1].id = "Wooden Shield";
    temp_unit.setEquipment(temp_equipment);
    temp_equipped = {0, 1};
    temp_unit.setEquipped(temp_equipped);
    temp_position = {4, 5};
    temp_unit.setPos(temp_position);
    made_units.push_back(temp_unit);
    return(made_units);
}

std::vector<Unit> chap25EnemyUnits() {
    Unit_stats temp_stats;
    Unit temp_unit;
    Inventory_item temp_equipment[DEFAULT::EQUIPMENT_SIZE];
    Equipped temp_equipped;
    Point temp_position;
    std::vector<Unit> made_units;
    temp_stats = {15,  4,  5,  7,  6,   8,  4,  6,  5, 5};
    temp_unit = Unit("Bandit", UNIT_CLASS::BANDIT, temp_stats, "M");
    temp_stats = {48, 14, 25, 32, 34,  28, 19, 40, 15};
    temp_unit.setCaps(temp_stats);
    temp_stats = {60, 50, 20, 60, 70,  40, 30, 20,  10, 0};
    temp_unit.setGrowths(temp_stats);
    temp_unit.setExp(0);
  //temp_equipment[0].id = "Iron Axe";
  //temp_equipment[1].id = "Wooden Shield";
    temp_unit.setEquipment(temp_equipment); 
    temp_equipped = {0, 1};
    temp_unit.setEquipped(temp_equipped);
    temp_position = {4, 5};
    temp_unit.setPos(temp_position);
    made_units.push_back(temp_unit);
    return(made_units);
}

std::vector<Unit> (*chapEnemyUnits[40])() = {chaptestEnemyUnits, chap1EnemyUnits, chap2EnemyUnits, chap3EnemyUnits, chap4EnemyUnits, chap5EnemyUnits, chap6EnemyUnits, chap7EnemyUnits, chap8EnemyUnits, chap9EnemyUnits, chap10EnemyUnits, chap11EnemyUnits, chap12EnemyUnits, chap13EnemyUnits, chap14EnemyUnits, chap15EnemyUnits, chap16EnemyUnits, chap17EnemyUnits, chap18EnemyUnits, chap19EnemyUnits, chap20EnemyUnits, chap21EnemyUnits, chap22EnemyUnits, chap23EnemyUnits, chap24EnemyUnits, chap25EnemyUnits};



