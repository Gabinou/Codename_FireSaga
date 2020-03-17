#include "unit.hpp"

Unit::Unit() {
    equipped.left = -1;
    equipped.right = -1;
}

Unit::Unit(const std::string in_name, const unsigned char in_class_index, const Unit_stats in_bases, const bool in_sex, Map_enemy in_map_enemy, unsigned long long int in_skills) : Unit(in_name, in_class_index, in_bases, in_sex, in_map_enemy) {
    skills = in_skills;
}

Unit::Unit(const std::string in_name, const unsigned char in_class_index, const Unit_stats in_bases, const bool in_sex, Map_enemy in_map_enemy) : Unit(in_name, in_class_index, in_bases, in_sex) {
    setMap_enemy(in_map_enemy);
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
    //This should not be in all unit objects. So much wasted space. Make it one shared callable function. Same for other heavy switches. 
    // Switch with individual cases:
    // Easy to change equippable post-hoc
    switch(class_index) {
        case UNIT::CLASS::MERCENARY:
            equippable = WPN::TYPE::SHIELD + WPN::TYPE::SWORD + WPN::TYPE::OFFHAND;
            break;
        case UNIT::CLASS::LORD:
            equippable = WPN::TYPE::LANCE + WPN::TYPE::SHIELD;
            break;
        case UNIT::CLASS::DUELIST:
            equippable = WPN::TYPE::LANCE + WPN::TYPE::SHIELD;
            break;
        case UNIT::CLASS::THIEF:
            equippable = WPN::TYPE::LANCE + WPN::TYPE::SHIELD;
            break;
        case UNIT::CLASS::PEGASUS_KNIGHT:
            equippable = WPN::TYPE::LANCE + WPN::TYPE::SHIELD;
            break;
        case UNIT::CLASS::PIKEMAN:
            equippable = WPN::TYPE::LANCE + WPN::TYPE::SHIELD;
            break;
        case UNIT::CLASS::CAVALIER:
            equippable = WPN::TYPE::LANCE + WPN::TYPE::SHIELD;
            break;
        case UNIT::CLASS::KNIGHT:
            equippable = WPN::TYPE::LANCE + WPN::TYPE::SHIELD;
            break;
        case UNIT::CLASS::BANDIT:
            equippable = WPN::TYPE::SHIELD + WPN::TYPE::AXE;
            break;
        case UNIT::CLASS::CORSAIR:
            equippable = WPN::TYPE::SHIELD + WPN::TYPE::OFFHAND + WPN::TYPE::AXE;
            break;
        case UNIT::CLASS::VIKING:
            equippable = WPN::TYPE::SHIELD + WPN::TYPE::OFFHAND + WPN::TYPE::AXE;
            break;
        case UNIT::CLASS::PICKPOCKET:
            equippable = WPN::TYPE::OFFHAND + WPN::TYPE::SWORD;
            break;
        case UNIT::CLASS::FENCER:
            equippable = WPN::TYPE::OFFHAND + WPN::TYPE::SWORD;
            break;
        case UNIT::CLASS::MOUSQUETAIRE:
            equippable = WPN::TYPE::SWORD + WPN::TYPE::OFFHAND + WPN::TYPE::BOW; 
            break;
        case UNIT::CLASS::ASSASSIN:
            equippable = WPN::TYPE::SWORD + WPN::TYPE::OFFHAND + WPN::TYPE::BOW; 
            break;
        case UNIT::CLASS::MARKSMAN:
            equippable = WPN::TYPE::SWORD + WPN::TYPE::OFFHAND + WPN::TYPE::BOW; 
            break;
        case UNIT::CLASS::ARCHER:
            equippable = WPN::TYPE::BOW;
            break;
        case UNIT::CLASS::DUKE:
            equippable = WPN::TYPE::SWORD + WPN::TYPE::LANCE + WPN::TYPE::SHIELD + WPN::TYPE::OFFHAND;
            break;
        case UNIT::CLASS::PALADIN:
            equippable = WPN::TYPE::SWORD + WPN::TYPE::LANCE + WPN::TYPE::SHIELD + WPN::TYPE::OFFHAND;
            break;
        case UNIT::CLASS::GENERAL:
            equippable = WPN::TYPE::SWORD + WPN::TYPE::SHIELD + WPN::TYPE::AXE + WPN::TYPE::LANCE;
            break;
        case UNIT::CLASS::CLERIC:
            equippable = WPN::TYPE::STAFF;
            break;
        case UNIT::CLASS::PRIEST:
            equippable = WPN::TYPE::STAFF;
            break;
        case UNIT::CLASS::MAGE:
            equippable = WPN::TYPE::ELEMENTAL + WPN::TYPE::TRINKET;
            break;
        case UNIT::CLASS::HERO:
            equippable = WPN::TYPE::AXE + WPN::TYPE::SWORD + WPN::TYPE::SHIELD + WPN::TYPE::OFFHAND;
            break;
        case UNIT::CLASS::RAVAGER:
            equippable = WPN::TYPE::AXE + WPN::TYPE::SHIELD + WPN::TYPE::OFFHAND + WPN::TYPE::BOW;
            break;
        case UNIT::CLASS::BATTLEMAGE:
            equippable = WPN::TYPE::ELEMENTAL + WPN::TYPE::SHIELD + WPN::TYPE::SWORD + WPN::TYPE::TRINKET;
            break;
        case UNIT::CLASS::SAGE:
            equippable = WPN::TYPE::ELEMENTAL + WPN::TYPE::STAFF + WPN::TYPE::TRINKET;
            break;
        case UNIT::CLASS::TROUBADOUR:
            equippable = WPN::TYPE::ELEMENTAL + WPN::TYPE::STAFF + WPN::TYPE::TRINKET;
            break;
        case UNIT::CLASS::ORACLE:
            equippable = WPN::TYPE::STAFF + WPN::TYPE::ANGELIC + WPN::TYPE::TRINKET;
            break;
        case UNIT::CLASS::BISHOP:
            equippable = WPN::TYPE::STAFF + WPN::TYPE::ANGELIC + WPN::TYPE::TRINKET;
            break;
        case UNIT::CLASS::ANGEL:
            equippable = WPN::TYPE::ANGELIC + WPN::TYPE::SWORD + WPN::TYPE::LANCE + WPN::TYPE::SHIELD;
            break;
        case UNIT::CLASS::DEMON:
            equippable = WPN::TYPE::DEMONIC + WPN::TYPE::CLAW + WPN::TYPE::AXE;
            break;
        case UNIT::CLASS::DEMONIC_INCARNATE:
            equippable = WPN::TYPE::DEMONIC + WPN::TYPE::TRINKET;
            break;
        case UNIT::CLASS::ANGELIC_INCARNATE:
            equippable = WPN::TYPE::ANGELIC + WPN::TYPE::TRINKET;
            break;
    }
}

void Unit::autoClassname() {
    switch(class_index) {
        case UNIT::CLASS::MERCENARY:
            class_name = "Mercenary";
            break;
        case UNIT::CLASS::LORD:
            class_name = "Lord";
            break;        
        case UNIT::CLASS::LORD_RIDER:
            class_name = "Lord rider";
            break;
        case UNIT::CLASS::DUELIST:
            class_name = "Duelist";
            break;
        case UNIT::CLASS::THIEF:
            class_name = "Thief";
            break;
        case UNIT::CLASS::PEGASUS_KNIGHT:
            class_name = "Pegasus knight";
            break;
        case UNIT::CLASS::PIKEMAN:
            class_name = "Pikeman";
            break;
        case UNIT::CLASS::CAVALIER:
            class_name = "Cavalier";
            break;
        case UNIT::CLASS::KNIGHT:
            class_name = "Knight";
            break;
        case UNIT::CLASS::BANDIT:
            class_name = "Bandit";
            break;
        case UNIT::CLASS::CORSAIR:
            class_name = "Corsair";
            break;
        case UNIT::CLASS::VIKING:
            class_name = "Viking";
            break;
        case UNIT::CLASS::PICKPOCKET:
            class_name = "Pickpocket";
            break;
        case UNIT::CLASS::FENCER:
            class_name = "Fencer";
            break;
        case UNIT::CLASS::MOUSQUETAIRE:
            class_name = "Mousquetaire";
            break;
        case UNIT::CLASS::ASSASSIN:
            class_name = "Assassin";
            break;
        case UNIT::CLASS::MARKSMAN:
            class_name = "Marksman";
            break;        
        case UNIT::CLASS::MARKSMAN_RIDER:
            class_name = "Marksman rider";
            break;
        case UNIT::CLASS::ARCHER:
            class_name = "Archer";
            break;        
        case UNIT::CLASS::ARCHER_RIDER:
            class_name = "Archer rider";
            break;
        case UNIT::CLASS::DUKE:
            class_name = "Duke";
            break;
        case UNIT::CLASS::DUKE_RIDER:
            class_name = "Duke rider";
            break;
        case UNIT::CLASS::PALADIN:
            class_name = "Paladin";
            break;
        case UNIT::CLASS::GENERAL:
            class_name = "General";
            break;
        case UNIT::CLASS::CLERIC:
            class_name = "Cleric";
            break;
        case UNIT::CLASS::PRIEST:
            class_name = "Priest";
            break;
        case UNIT::CLASS::MAGE:
            class_name = "Mage";
            break;
        case UNIT::CLASS::HERO:
            class_name = "Hero";
            break;
        case UNIT::CLASS::RAVAGER:
            class_name = "Ravager";
            break;
        case UNIT::CLASS::BATTLEMAGE:
            class_name = "Battlemage";
            break;
        case UNIT::CLASS::SAGE:
            class_name = "Sage";
            break;
        case UNIT::CLASS::TROUBADOUR:
            class_name = "Troubadour";
            break;
        case UNIT::CLASS::ORACLE:
            class_name = "Oracle";
            break;
        case UNIT::CLASS::BISHOP:
            class_name = "Bishop";
            break;
        case UNIT::CLASS::ANGEL:
            class_name = "Angel";
            break;
        case UNIT::CLASS::DEMON:
            class_name = "Demon";
            break;
        case UNIT::CLASS::DEMONIC_INCARNATE:
            class_name = "Demonic Incarnate";
            break;
        case UNIT::CLASS::ANGELIC_INCARNATE:
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
        case UNIT::CLASS::MERCENARY:
            mvt_type = UNIT::MVT::FOOT_SLOW;
            break;
        case UNIT::CLASS::LORD:
            mvt_type = UNIT::MVT::FOOT_SLOW;
            break;
        case UNIT::CLASS::DUELIST:
            mvt_type = UNIT::MVT::FOOT_SLOW;
            break;
        case UNIT::CLASS::THIEF:
            mvt_type = UNIT::MVT::FOOT_FAST;
            break;
        case UNIT::CLASS::PEGASUS_KNIGHT:
            mvt_type = UNIT::MVT::FLIERS;
            break;
        case UNIT::CLASS::PIKEMAN:
            mvt_type = UNIT::MVT::FOOT_SLOW;
            break;
        case UNIT::CLASS::CAVALIER:
            mvt_type = UNIT::MVT::RIDERS_SLOW;
            break;
        case UNIT::CLASS::KNIGHT:
            mvt_type = UNIT::MVT::ARMORS;
            break;
        case UNIT::CLASS::BANDIT:
            mvt_type = UNIT::MVT::BANDITS;
            break;
        case UNIT::CLASS::CORSAIR:
            mvt_type = UNIT::MVT::PIRATES;
            break;
        case UNIT::CLASS::VIKING:
            mvt_type = UNIT::MVT::PIRATES;
            break;
        case UNIT::CLASS::PICKPOCKET:
            mvt_type = UNIT::MVT::FOOT_FAST;
            break;
        case UNIT::CLASS::FENCER:
            mvt_type = UNIT::MVT::FOOT_SLOW;
            break;
        case UNIT::CLASS::MOUSQUETAIRE:
            mvt_type = UNIT::MVT::FOOT_SLOW;
            break;
        case UNIT::CLASS::ASSASSIN:
            mvt_type = UNIT::MVT::FOOT_FAST;
            break;
        case UNIT::CLASS::MARKSMAN:
            mvt_type = UNIT::MVT::FOOT_SLOW;
            break;        
        case UNIT::CLASS::MARKSMAN_RIDER:
            mvt_type = UNIT::MVT::RIDERS_SLOW;
            break;
        case UNIT::CLASS::ARCHER:
            mvt_type = UNIT::MVT::FOOT_SLOW;
            break;        
        case UNIT::CLASS::ARCHER_RIDER:
            mvt_type = UNIT::MVT::RIDERS_SLOW;
            break;
        case UNIT::CLASS::DUKE:
            mvt_type = UNIT::MVT::FOOT_SLOW;
            break;
        case UNIT::CLASS::PALADIN:
            mvt_type = UNIT::MVT::RIDERS_FAST;
            break;
        case UNIT::CLASS::GENERAL:
            mvt_type = UNIT::MVT::ARMORS;
            break;
        case UNIT::CLASS::CLERIC:
            mvt_type = UNIT::MVT::MAGES;
            break;
        case UNIT::CLASS::PRIEST:
            mvt_type = UNIT::MVT::MAGES;
            break;
        case UNIT::CLASS::MAGE:
            mvt_type = UNIT::MVT::MAGES;
            break;
        case UNIT::CLASS::HERO:
            mvt_type = UNIT::MVT::FOOT_SLOW;
            break;
        case UNIT::CLASS::RAVAGER:
            mvt_type = UNIT::MVT::BANDITS;
            break;
        case UNIT::CLASS::BATTLEMAGE:
            mvt_type = UNIT::MVT::MAGES;
            break;
        case UNIT::CLASS::SAGE:
            mvt_type = UNIT::MVT::MAGES;
            break;
        case UNIT::CLASS::TROUBADOUR:
            mvt_type = UNIT::MVT::RIDERS_FAST;
            break;
        case UNIT::CLASS::ORACLE:
            mvt_type = UNIT::MVT::MAGES;
            break;
        case UNIT::CLASS::BISHOP:
            mvt_type = UNIT::MVT::MAGES;
            break;
        case UNIT::CLASS::ANGEL:
            mvt_type = UNIT::MVT::FLIERS;
            break;
        case UNIT::CLASS::DEMON:
            mvt_type = UNIT::MVT::FOOT_FAST;
            break;
        case UNIT::CLASS::DEMONIC_INCARNATE:
            mvt_type = UNIT::MVT::MAGES;
            break;
        case UNIT::CLASS::ANGELIC_INCARNATE:
            mvt_type = UNIT::MVT::MAGES;
            break;
    }
}

void Unit::equipsL(const unsigned char index) {
    equipped.left = index;
}

void Unit::equipsR(const unsigned char index) {
    equipped.right = index;
}

void Unit::unequipsL() {
    equipped.left = -1;
}

void Unit::unequipsR() {
    equipped.right = -1;
}

void Unit::equips(const short unsigned int index, const bool hand) {
    if (hand) {
        equipped.left = index;
    } else {
        equipped.right = index;
    }
}

void Unit::unequips(const bool hand) {
    if (hand) {
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

    static unsigned char range[2] = {0, 0};

    if (equipped.left > 0) {
        unsigned char * temp = all_weapons[equipment[equipped.left].id].getStats().range;
        range[0] = temp[0]; range[1] = temp[1];
    }

    if (equipped.right > 0) {
        unsigned char * temp = all_weapons[equipment[equipped.left].id].getStats().range;
        range[0] = std::min(temp[0], range[0]);
        range[1] = std::max(temp[1], range[1]);
    }

    if ((equipped.left < 0) && (equipped.right < 0)) {
        range[0] = 0;
        range[1] = 0;
    }

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

void Unit::setBaseExp(const short unsigned int in_exp) {
    exp = in_exp;
    base_exp = in_exp;
}

void Unit::gainExp(const short unsigned int in_exp) {
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

bool Unit::canEquip(unsigned short int in_id) {
    short unsigned int wpn_type = all_weapons[in_id].getType();
    return(((equippable & wpn_type) > 0));
} 

bool Unit::canAttack() {
    bool out;
    struct Wpn_types {
        unsigned short int left;
        unsigned short int right;
    } wpn_types;
    if (equipped.left > 0) {
        wpn_types.left = all_weapons[equipment[equipped.left].id].getType();
        if ((wpn_types.left != WPN::TYPE::SHIELD)  & (wpn_types.left != WPN::TYPE::TRINKET) & (wpn_types.left != WPN::TYPE::STAFF)) {
            out = true;
        } 
    }
    if (equipped.right > 0) {
        wpn_types.right = all_weapons[equipment[equipped.right].id].getType();
        if ((wpn_types.right != WPN::TYPE::SHIELD)  & (wpn_types.right != WPN::TYPE::TRINKET) & (wpn_types.right != WPN::TYPE::STAFF)) {
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

void Unit::read(const char * filename) {
    FILE * fp;
    fp = fopen(filename, "r");

    fclose(fp);
}
void Unit::writeFS(const char * filename, const bool append) {
    // Maybe this function should write constant number of bytes per line...
    // Easier to read.
    if (append) {
        PHYSFS_file * fp = PHYSFS_openWrite(filename);
    } else {
        PHYSFS_file * fp = PHYSFS_openAppend(filename);
    }
    size_t buflen;
    char varbuf[BUFFER_SIZE];
    int retlen;

    sprintf(varbuf, "%s \n", name.c_str());
    buflen = strlen(varbuf);
    retlen = PHYSFS_writeBytes(fp, varbuf, buflen);
    if (sex) {
        sprintf(varbuf, "%s \n", "M");
    } else {
        sprintf(varbuf, "%s \n", "F");
    }
    buflen = strlen(varbuf);
    retlen = PHYSFS_writeBytes(fp, varbuf, buflen);
    sprintf(varbuf, "%s \n", class_name.c_str());
    buflen = strlen(varbuf);
    retlen = PHYSFS_writeBytes(fp, varbuf, buflen);
    sprintf(varbuf, "Exp: \t%d \n", exp);
    buflen = strlen(varbuf);
    retlen = PHYSFS_writeBytes(fp, varbuf, buflen);
    sprintf(varbuf, "Stats: HP, Str, Mag, Skl, Spd, Luck, Def, Res, Con, Move\n");
    buflen = strlen(varbuf);
    retlen = PHYSFS_writeBytes(fp, varbuf, buflen);
    sprintf(varbuf, "Base stats:\t%d,\t%d,\t%d,\t%d,\t%d,\t%d,\t%d,\t%d,\t%d,\t%d,\t%d\n", base_stats.hp, base_stats.str, base_stats.mag, base_stats.dex, base_stats.agi, base_stats.luck, base_stats.def, base_stats.res, base_stats.con, base_stats.move, base_stats.prof);
    buflen = strlen(varbuf);
    retlen = PHYSFS_writeBytes(fp, varbuf, buflen);
    sprintf(varbuf, "Growths:\t%d,\t%d,\t%d,\t%d,\t%d,\t%d,\t%d,\t%d,\t%d,\t%d,\t%d\n", growths.hp, growths.str, growths.mag, growths.dex, growths.agi, growths.luck, growths.def, growths.res, growths.con, growths.move, growths.prof);
    buflen = strlen(varbuf);
    retlen = PHYSFS_writeBytes(fp, varbuf, buflen);
    sprintf(varbuf, "Caps:\t\t%d,\t%d,\t%d,\t%d,\t%d,\t%d,\t%d,\t%d,\t%d,\t%d,\t%d\n", caps_stats.hp, caps_stats.str, caps_stats.mag, caps_stats.dex, caps_stats.agi, caps_stats.luck, caps_stats.def, caps_stats.res, caps_stats.con, caps_stats.move, caps_stats.prof);
    buflen = strlen(varbuf);
    retlen = PHYSFS_writeBytes(fp, varbuf, buflen);
    sprintf(varbuf, "Level-ups:\n");
    for (int i = 0; i < grown_stats.size(); i++) {
        sprintf(varbuf, "%d:\t\t%d,\t%d,\t%d,\t%d,\t%d,\t%d,\t%d,\t%d,\t%d,\t%d,\t%d\n", exp/10, grown_stats[i].hp, grown_stats[i].str, grown_stats[i].mag, grown_stats[i].dex, grown_stats[i].agi, grown_stats[i].luck, grown_stats[i].def, grown_stats[i].res, grown_stats[i].con, grown_stats[i].move, grown_stats[i].prof);
    }
    buflen = strlen(varbuf);
    retlen = PHYSFS_writeBytes(fp, varbuf, buflen);
    sprintf(varbuf, "\nEquipment:\n");
    for (int i = 0; i < DEFAULT::EQUIPMENT_SIZE; i++) {
        sprintf(varbuf, "%d, \t%d\n", equipment[i].id, equipment[i].used);
    }
    buflen = strlen(varbuf);
    retlen = PHYSFS_writeBytes(fp, varbuf, buflen);
    PHYSFS_close(fp);
}

void Unit::write(const char * filename, const char * mode) {
    // Why simple .txt files.
    // -> Simple. No external library.
    // -> NO FILE SHARING. Files unique to the game.
    // -> File structure easy to understand.
    // -> Easy to parse?
    FILE * fp = fopen(filename, mode);
    fprintf(fp, "%s \n", name.c_str());
    if (sex) {
        fprintf(fp, "%s \n", "M");
    } else {
        fprintf(fp, "%s \n", "F");
    }
    fprintf(fp, "%s \n", class_name.c_str());
    fprintf(fp, "Exp: \t%d \n", exp);
    fprintf(fp, "Stats: HP, Str, Mag, Skl, Spd, Luck, Def, Res, Con, Move\n");
    fprintf(fp, "Base stats:\t%d,\t%d,\t%d,\t%d,\t%d,\t%d,\t%d,\t%d,\t%d,\t%d,\t%d\n", base_stats.hp, base_stats.str, base_stats.mag, base_stats.dex, base_stats.agi, base_stats.luck, base_stats.def, base_stats.res, base_stats.con, base_stats.move, base_stats.prof);
    fprintf(fp, "Growths:\t%d,\t%d,\t%d,\t%d,\t%d,\t%d,\t%d,\t%d,\t%d,\t%d,\t%d\n", growths.hp, growths.str, growths.mag, growths.dex, growths.agi, growths.luck, growths.def, growths.res, growths.con, growths.move, growths.prof);
    fprintf(fp, "Caps:\t\t%d,\t%d,\t%d,\t%d,\t%d,\t%d,\t%d,\t%d,\t%d,\t%d,\t%d\n", caps_stats.hp, caps_stats.str, caps_stats.mag, caps_stats.dex, caps_stats.agi, caps_stats.luck, caps_stats.def, caps_stats.res, caps_stats.con, caps_stats.move, caps_stats.prof);
    fprintf(fp, "Level-ups:\n");
    for (int i = 0; i < grown_stats.size(); i++) {
        fprintf(fp, "%d:\t\t%d,\t%d,\t%d,\t%d,\t%d,\t%d,\t%d,\t%d,\t%d,\t%d,\t%d\n", exp/10, grown_stats[i].hp, grown_stats[i].str, grown_stats[i].mag, grown_stats[i].dex, grown_stats[i].agi, grown_stats[i].luck, grown_stats[i].def, grown_stats[i].res, grown_stats[i].con, grown_stats[i].move, grown_stats[i].prof);
    }
    fprintf(fp, "\nEquipment:\n");
    for (int i = 0; i < DEFAULT::EQUIPMENT_SIZE; i++) {
        fprintf(fp, "%d, \t%d\n", equipment[i].id, equipment[i].used);
    }
    fclose(fp);
}

std::vector<Unit> all_units(UNIT::NAME::END);
std::vector<Unit> loaded_units;

void baseUnits() {
    printf("Making base units \n");
    Unit temp_unit;
    Unit_stats temp;
    Inventory_item temp_wpn;
    //hp,str,mag,skl,spd,luck,def,res,con,move
    // printf("Made unit.\n");

    temp = {17,  6,  2,  7,  7,   7,  4,  5,  6, 5};
    temp_unit = Unit("Erwin", UNIT::CLASS::MERCENARY, temp, UNIT::SEX::M);
    temp = {48, 14, 25, 32, 34,  28, 19, 40, 15, 0};
    temp_unit.setCaps(temp);
    temp = {60, 50, 20, 60, 70,  40, 30, 20,  10, 0};
    temp_unit.setGrowths(temp);
    temp_unit.setBaseExp(0);
    printf("Made unit.\n");
    temp_unit.write("unit_test.txt", "w");
    printf("Made units.\n");
    all_units[UNIT::NAME::ERWIN] = temp_unit;
    
    temp = {18,  6,  2,  7,  7,   7,  4,  5,  6, 7};
    temp_unit = Unit("Reliable", UNIT::CLASS::CAVALIER, temp, UNIT::SEX::M);
    temp = {48, 14, 25, 32, 34,  28, 19, 40, 15};
    temp_unit.setCaps(temp);
    temp = {60, 50, 20, 60, 70,  40, 30, 20,  10, 0};
    temp_unit.setGrowths(temp);
    temp_unit.setBaseExp(100);
    all_units[UNIT::NAME::RELIABLE] = temp_unit;
    
    temp = {19,  6,  2,  7,  7,   7,  4,  5,  6,  7};
    temp_unit = Unit("Coward", UNIT::CLASS::CAVALIER, temp, UNIT::SEX::M);
    temp = {48, 14, 25, 32, 34,  28, 19, 40, 15};
    temp_unit.setCaps(temp);
    temp = {60, 50, 20, 60, 70,  40, 30, 20,  10, 0};
    temp_unit.setGrowths(temp);
    temp_unit.setBaseExp(200);
    all_units[UNIT::NAME::COWARD] = temp_unit;
    
    temp = {20,  6,  2,  7,  7,   7,  4,  5,  6,  6};
    temp_unit = Unit("1H Jaigen", UNIT::CLASS::MOUSQUETAIRE, temp, UNIT::SEX::M);
    temp = {48, 14, 25, 32, 34,  28, 19, 40, 15};
    temp_unit.setCaps(temp);
    temp = {60, 50, 20, 60, 70,  40, 30, 20,  10, 0};
    temp_unit.setGrowths(temp);
    temp_unit.setBaseExp(2200);
    all_units[UNIT::NAME::JAIGEN1H] = temp_unit;

    temp = {14,  6,  2,  7,  7,   7,  4,  5,  6,  5};
    temp_unit = Unit("Kiara", UNIT::CLASS::CLERIC, temp, UNIT::SEX::F);
    temp = {48, 14, 25, 32, 34,  28, 19, 40, 15};
    temp_unit.setCaps(temp);
    temp = {60, 50, 20, 60, 70,  40, 30, 20,  10, 0};
    temp_unit.setGrowths(temp);
    temp_unit.setBaseExp(100);
    all_units[UNIT::NAME::KIARA] = temp_unit;
    
    temp = {16,  6,  2,  7,  7,   7,  4,  5,  6,  6};
    temp_unit = Unit("Hottie", UNIT::CLASS::PICKPOCKET, temp, UNIT::SEX::M);
    temp = {48, 14, 25, 32, 34,  28, 19, 40, 15};
    temp_unit.setCaps(temp);
    temp = {60, 50, 20, 60, 70,  40, 30, 20,  10, 0};
    temp_unit.setGrowths(temp);
    temp_unit.setBaseExp(2200);
    all_units[UNIT::NAME::HOTTIE] = temp_unit;
    
    temp = {22,  4,  5,  7,  6,   8,  4,  6,  5, 5}; // 4 or 5?
    temp_unit = Unit("Servil", UNIT::CLASS::KNIGHT, temp, UNIT::SEX::M);
    temp = {48, 14, 25, 32, 34,  28, 19, 40, 15};
    temp_unit.setCaps(temp);
    temp = {60, 50, 20, 60, 70,  40, 30, 20,  10, 0};
    temp_unit.setGrowths(temp);
    temp_unit.setBaseExp(500);
    all_units[UNIT::NAME::SERVIL] = temp_unit;
    
    temp = {34,  4,  5,  7,  6,   8,  4,  6,  5, 5};
    temp_unit = Unit("Pérignon", UNIT::CLASS::MAGE, temp, UNIT::SEX::M);
    temp = {48, 14, 25, 32, 34,  28, 19, 40, 15};
    temp_unit.setCaps(temp);
    temp = {60, 50, 20, 60, 70,  40, 30, 20,  10, 0};
    temp_unit.setGrowths(temp);
    temp_unit.setBaseExp(1200);
    all_units[UNIT::NAME::PERIGNON] = temp_unit;
    
    temp = {15,  4,  5,  7,  6,   8,  4,  6,  5, 5};
    temp_unit = Unit("Poet", UNIT::CLASS::MAGE, temp, UNIT::SEX::M);
    temp = {48, 14, 25, 32, 34,  28, 19, 40, 15};
    temp_unit.setCaps(temp);
    temp = {60, 50, 20, 60, 70,  40, 30, 20,  10, 0};
    temp_unit.setGrowths(temp);
    temp_unit.setBaseExp(400);
    all_units[UNIT::NAME::POET] = temp_unit;
    
    temp = {15,  4,  5,  7,  6,   8,  4,  6,  5, 5};
    temp_unit = Unit("Silou", UNIT::CLASS::MAGE, temp, UNIT::SEX::F);
    temp = {48, 14, 25, 32, 34,  28, 19, 40, 15};
    temp_unit.setCaps(temp);
    temp = {60, 50, 20, 60, 70,  40, 30, 20,  10, 0};
    temp_unit.setGrowths(temp);
    temp_unit.setBaseExp(400);
    temp_wpn.id = WPN::NAME::BALL_LIGHTNING;
    temp_unit.addEquipment(temp_wpn);
    all_units[UNIT::NAME::SILOU] = temp_unit;
}

void genericEnemyUnits() {
    printf("Making generic enemies\n");
    Unit_stats temp;
    Unit temp_unit;
    //hp,str,mag,skl,spd,luck,def,res,con,move
    temp = {15,  4,  5,  7,  6,   8,  4,  6,  5, 5};
    temp_unit = Unit("Bandit", UNIT::CLASS::BANDIT, temp, UNIT::SEX::M);
    temp = {48, 14, 25, 32, 34,  28, 19, 40, 15};
    temp_unit.setCaps(temp);
    temp = {60, 50, 20, 60, 70,  40, 30, 20,  10, 0};
    temp_unit.setGrowths(temp);
    temp_unit.setBaseExp(0);
    all_units[UNIT::NAME::GENERIC_BANDIT] = temp_unit;

    temp = {15,  4,  5,  7,  6,   8,  4,  6,  5, 5};
    temp_unit = Unit("Duelist", UNIT::CLASS::DUELIST, temp, UNIT::SEX::M);
    temp = {48, 14, 25, 32, 34,  28, 19, 40, 15};
    temp_unit.setCaps(temp);
    temp = {60, 50, 20, 60, 70,  40, 30, 20,  10, 0};
    temp_unit.setGrowths(temp);
    temp_unit.setBaseExp(0);
    all_units[UNIT::NAME::GENERIC_DUELIST] = temp_unit;

    temp = {15,  4,  5,  7,  6,   8,  4,  6,  5, 5};
    temp_unit = Unit("Pickpocket", UNIT::CLASS::PICKPOCKET, temp, UNIT::SEX::M);
    temp = {48, 14, 25, 32, 34,  28, 19, 40, 15};
    temp_unit.setCaps(temp);
    temp = {60, 50, 20, 60, 70,  40, 30, 20,  10, 0};
    temp_unit.setGrowths(temp);
    temp_unit.setBaseExp(0);
    all_units[UNIT::NAME::GENERIC_PICKPOCKET] = temp_unit;

    temp = {15,  4,  5,  7,  6,   8,  4,  6,  5, 5};
    temp_unit = Unit("Thief", UNIT::CLASS::THIEF, temp, UNIT::SEX::M);
    temp = {48, 14, 25, 32, 34,  28, 19, 40, 15};
    temp_unit.setCaps(temp);
    temp = {60, 50, 20, 60, 70,  40, 30, 20,  10, 0};
    temp_unit.setGrowths(temp);
    temp_unit.setBaseExp(0);
    all_units[UNIT::NAME::GENERIC_THIEF] = temp_unit;    

    temp = {15,  4,  5,  7,  6,   8,  4,  6,  5, 5};
    temp_unit = Unit("Assassin", UNIT::CLASS::ASSASSIN, temp, UNIT::SEX::M);
    temp = {48, 14, 25, 32, 34,  28, 19, 40, 15};
    temp_unit.setCaps(temp);
    temp = {60, 50, 20, 60, 70,  40, 30, 20,  10, 0};
    temp_unit.setGrowths(temp);
    temp_unit.setBaseExp(0);
    all_units[UNIT::NAME::GENERIC_ASSASSIN] = temp_unit;

    temp = {15,  4,  5,  7,  6,   8,  4,  6,  5, 5};
    temp_unit = Unit("Archer", UNIT::CLASS::ARCHER, temp, UNIT::SEX::M);
    temp = {48, 14, 25, 32, 34,  28, 19, 40, 15};
    temp_unit.setCaps(temp);
    temp = {60, 50, 20, 60, 70,  40, 30, 20,  10, 0};
    temp_unit.setGrowths(temp);
    temp_unit.setBaseExp(0);
    all_units[UNIT::NAME::GENERIC_ARCHER] = temp_unit;

    temp = {15,  4,  5,  7,  6,   8,  4,  6,  5, 5};
    temp_unit = Unit("Marksman", UNIT::CLASS::MARKSMAN, temp, UNIT::SEX::M);
    temp = {48, 14, 25, 32, 34,  28, 19, 40, 15};
    temp_unit.setCaps(temp);
    temp = {60, 50, 20, 60, 70,  40, 30, 20,  10, 0};
    temp_unit.setGrowths(temp);
    temp_unit.setBaseExp(0);
    all_units[UNIT::NAME::GENERIC_MARKSMAN] = temp_unit;

    temp = {15,  4,  5,  7,  6,   8,  4,  6,  5, 5};
    temp_unit = Unit("Mercenary", UNIT::CLASS::MERCENARY, temp, UNIT::SEX::M);
    temp = {48, 14, 25, 32, 34,  28, 19, 40, 15};
    temp_unit.setCaps(temp);
    temp = {60, 50, 20, 60, 70,  40, 30, 20,  10, 0};
    temp_unit.setGrowths(temp);
    temp_unit.setBaseExp(0);
    all_units[UNIT::NAME::GENERIC_MERCENARY] = temp_unit;

    temp = {15,  4,  5,  7,  6,   8,  4,  6,  5, 5};
    temp_unit = Unit("Hero", UNIT::CLASS::HERO, temp, UNIT::SEX::M);
    temp = {48, 14, 25, 32, 34,  28, 19, 40, 15};
    temp_unit.setCaps(temp);
    temp = {60, 50, 20, 60, 70,  40, 30, 20,  10, 0};
    temp_unit.setGrowths(temp);
    temp_unit.setBaseExp(0);
    all_units[UNIT::NAME::GENERIC_HERO] = temp_unit;

    temp = {15,  4,  5,  7,  6,   8,  4,  6,  5, 5};
    temp_unit = Unit("Corsair", UNIT::CLASS::CORSAIR, temp, UNIT::SEX::M);
    temp = {48, 14, 25, 32, 34,  28, 19, 40, 15};
    temp_unit.setCaps(temp);
    temp = {60, 50, 20, 60, 70,  40, 30, 20,  10, 0};
    temp_unit.setGrowths(temp);
    temp_unit.setBaseExp(0);
    all_units[UNIT::NAME::GENERIC_CORSAIR] = temp_unit;

    temp = {15,  4,  5,  7,  6,   8,  4,  6,  5, 5};
    temp_unit = Unit("Viking", UNIT::CLASS::VIKING, temp, UNIT::SEX::M);
    temp = {48, 14, 25, 32, 34,  28, 19, 40, 15};
    temp_unit.setCaps(temp);
    temp = {60, 50, 20, 60, 70,  40, 30, 20,  10, 0};
    temp_unit.setGrowths(temp);
    temp_unit.setBaseExp(0);
    all_units[UNIT::NAME::GENERIC_VIKING] = temp_unit;

    temp = {15,  4,  5,  7,  6,   8,  4,  6,  5, 5};
    temp_unit = Unit("Ravager", UNIT::CLASS::RAVAGER, temp, UNIT::SEX::M);
    temp = {48, 14, 25, 32, 34,  28, 19, 40, 15};
    temp_unit.setCaps(temp);
    temp = {60, 50, 20, 60, 70,  40, 30, 20,  10, 0};
    temp_unit.setGrowths(temp);
    temp_unit.setBaseExp(0);
    all_units[UNIT::NAME::GENERIC_RAVAGER] = temp_unit;

    temp = {15,  4,  5,  7,  6,   8,  4,  6,  5, 5};
    temp_unit = Unit("Cavalier", UNIT::CLASS::CAVALIER, temp, UNIT::SEX::M);
    temp = {48, 14, 25, 32, 34,  28, 19, 40, 15};
    temp_unit.setCaps(temp);
    temp = {60, 50, 20, 60, 70,  40, 30, 20,  10, 0};
    temp_unit.setGrowths(temp);
    temp_unit.setBaseExp(0);
    all_units[UNIT::NAME::GENERIC_CAVALIER] = temp_unit;

    temp = {15,  4,  5,  7,  6,   8,  4,  6,  5, 5};
    temp_unit = Unit("Paladin", UNIT::CLASS::PALADIN, temp, UNIT::SEX::M);
    temp = {48, 14, 25, 32, 34,  28, 19, 40, 15};
    temp_unit.setCaps(temp);
    temp = {60, 50, 20, 60, 70,  40, 30, 20,  10, 0};
    temp_unit.setGrowths(temp);
    temp_unit.setBaseExp(0);
    all_units[UNIT::NAME::GENERIC_PALADIN] = temp_unit;

    temp = {15,  4,  5,  7,  6,   8,  4,  6,  5, 5};
    temp_unit = Unit("Fencer", UNIT::CLASS::FENCER, temp, UNIT::SEX::M);
    temp = {48, 14, 25, 32, 34,  28, 19, 40, 15};
    temp_unit.setCaps(temp);
    temp = {60, 50, 20, 60, 70,  40, 30, 20,  10, 0};
    temp_unit.setGrowths(temp);
    temp_unit.setBaseExp(0);
    all_units[UNIT::NAME::GENERIC_FENCER] = temp_unit;

    temp = {15,  4,  5,  7,  6,   8,  4,  6,  5, 5};
    temp_unit = Unit("Mousquetaire", UNIT::CLASS::MOUSQUETAIRE, temp, UNIT::SEX::M);
    temp = {48, 14, 25, 32, 34,  28, 19, 40, 15};
    temp_unit.setCaps(temp);
    temp = {60, 50, 20, 60, 70,  40, 30, 20,  10, 0};
    temp_unit.setGrowths(temp);
    temp_unit.setBaseExp(0);
    all_units[UNIT::NAME::GENERIC_MOUSQUETAIRE] = temp_unit;
}

std::vector<Unit> chaptestEnemyUnits() {
    Unit_stats temp_stats;
    Unit temp_unit;
    Inventory_item temp_equipment[DEFAULT::EQUIPMENT_SIZE];
    Equipped temp_equipped;
    Point temp_position;
    std::vector<Unit> made_units;
    temp_stats = {15,  4,  5,  7,  6,   8,  4,  6,  5, 5};
    temp_unit = Unit("Bandit", UNIT::CLASS::BANDIT, temp_stats, 1);
    temp_stats = {48, 14, 25, 32, 34,  28, 19, 40, 15};
    temp_unit.setCaps(temp_stats);
    temp_stats = {60, 50, 20, 60, 70,  40, 30, 20,  10, 0};
    temp_unit.setGrowths(temp_stats);
    temp_unit.setBaseExp(0);
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
    temp_unit = Unit("Bandit", UNIT::CLASS::BANDIT, temp_stats, 1);
    temp_stats = {48, 14, 25, 32, 34,  28, 19, 40, 15};
    temp_unit.setCaps(temp_stats);
    temp_stats = {60, 50, 20, 60, 70,  40, 30, 20,  10, 0};
    temp_unit.setGrowths(temp_stats);
    temp_unit.setBaseExp(0);
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
    temp_unit = Unit("Bandit", UNIT::CLASS::BANDIT, temp_stats, 1);
    temp_stats = {48, 14, 25, 32, 34,  28, 19, 40, 15};
    temp_unit.setCaps(temp_stats);
    temp_stats = {60, 50, 20, 60, 70,  40, 30, 20,  10, 0};
    temp_unit.setGrowths(temp_stats);
    temp_unit.setBaseExp(0);
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
    temp_unit = Unit("Bandit", UNIT::CLASS::BANDIT, temp_stats, 1);
    temp_stats = {48, 14, 25, 32, 34,  28, 19, 40, 15};
    temp_unit.setCaps(temp_stats);
    temp_stats = {60, 50, 20, 60, 70,  40, 30, 20,  10, 0};
    temp_unit.setGrowths(temp_stats);
    temp_unit.setBaseExp(0);
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
    temp_unit = Unit("Bandit", UNIT::CLASS::BANDIT, temp_stats, 1);
    temp_stats = {48, 14, 25, 32, 34,  28, 19, 40, 15};
    temp_unit.setCaps(temp_stats);
    temp_stats = {60, 50, 20, 60, 70,  40, 30, 20,  10, 0};
    temp_unit.setGrowths(temp_stats);
    temp_unit.setBaseExp(0);
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
    temp_unit = Unit("Bandit", UNIT::CLASS::BANDIT, temp_stats, 1);
    temp_stats = {48, 14, 25, 32, 34,  28, 19, 40, 15};
    temp_unit.setCaps(temp_stats);
    temp_stats = {60, 50, 20, 60, 70,  40, 30, 20,  10, 0};
    temp_unit.setGrowths(temp_stats);
    temp_unit.setBaseExp(0);
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
    temp_unit = Unit("Bandit", UNIT::CLASS::BANDIT, temp_stats, 1);
    temp_stats = {48, 14, 25, 32, 34,  28, 19, 40, 15};
    temp_unit.setCaps(temp_stats);
    temp_stats = {60, 50, 20, 60, 70,  40, 30, 20,  10, 0};
    temp_unit.setGrowths(temp_stats);
    temp_unit.setBaseExp(0);
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
    temp_unit = Unit("Bandit", UNIT::CLASS::BANDIT, temp_stats, 1);
    temp_stats = {48, 14, 25, 32, 34,  28, 19, 40, 15};
    temp_unit.setCaps(temp_stats);
    temp_stats = {60, 50, 20, 60, 70,  40, 30, 20,  10, 0};
    temp_unit.setGrowths(temp_stats);
    temp_unit.setBaseExp(0);
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
    temp_unit = Unit("Bandit", UNIT::CLASS::BANDIT, temp_stats, 1);
    temp_stats = {48, 14, 25, 32, 34,  28, 19, 40, 15};
    temp_unit.setCaps(temp_stats);
    temp_stats = {60, 50, 20, 60, 70,  40, 30, 20,  10, 0};
    temp_unit.setGrowths(temp_stats);
    temp_unit.setBaseExp(0);
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
    temp_unit = Unit("Bandit", UNIT::CLASS::BANDIT, temp_stats, 1);
    temp_stats = {48, 14, 25, 32, 34,  28, 19, 40, 15};
    temp_unit.setCaps(temp_stats);
    temp_stats = {60, 50, 20, 60, 70,  40, 30, 20,  10, 0};
    temp_unit.setGrowths(temp_stats);
    temp_unit.setBaseExp(0);
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
    temp_unit = Unit("Bandit", UNIT::CLASS::BANDIT, temp_stats, 1);
    temp_stats = {48, 14, 25, 32, 34,  28, 19, 40, 15};
    temp_unit.setCaps(temp_stats);
    temp_stats = {60, 50, 20, 60, 70,  40, 30, 20,  10, 0};
    temp_unit.setGrowths(temp_stats);
    temp_unit.setBaseExp(0);
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
    temp_unit = Unit("Bandit", UNIT::CLASS::BANDIT, temp_stats, 1);
    temp_stats = {48, 14, 25, 32, 34,  28, 19, 40, 15};
    temp_unit.setCaps(temp_stats);
    temp_stats = {60, 50, 20, 60, 70,  40, 30, 20,  10, 0};
    temp_unit.setGrowths(temp_stats);
    temp_unit.setBaseExp(0);
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
    temp_unit = Unit("Bandit", UNIT::CLASS::BANDIT, temp_stats, 1);
    temp_stats = {48, 14, 25, 32, 34,  28, 19, 40, 15};
    temp_unit.setCaps(temp_stats);
    temp_stats = {60, 50, 20, 60, 70,  40, 30, 20,  10, 0};
    temp_unit.setGrowths(temp_stats);
    temp_unit.setBaseExp(0);
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
    temp_unit = Unit("Bandit", UNIT::CLASS::BANDIT, temp_stats, 1);
    temp_stats = {48, 14, 25, 32, 34,  28, 19, 40, 15};
    temp_unit.setCaps(temp_stats);
    temp_stats = {60, 50, 20, 60, 70,  40, 30, 20,  10, 0};
    temp_unit.setGrowths(temp_stats);
    temp_unit.setBaseExp(0);
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
    temp_unit = Unit("Bandit", UNIT::CLASS::BANDIT, temp_stats, 1);
    temp_stats = {48, 14, 25, 32, 34,  28, 19, 40, 15};
    temp_unit.setCaps(temp_stats);
    temp_stats = {60, 50, 20, 60, 70,  40, 30, 20,  10, 0};
    temp_unit.setGrowths(temp_stats);
    temp_unit.setBaseExp(0);
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
    temp_unit = Unit("Bandit", UNIT::CLASS::BANDIT, temp_stats, 1);
    temp_stats = {48, 14, 25, 32, 34,  28, 19, 40, 15};
    temp_unit.setCaps(temp_stats);
    temp_stats = {60, 50, 20, 60, 70,  40, 30, 20,  10, 0};
    temp_unit.setGrowths(temp_stats);
    temp_unit.setBaseExp(0);
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
    temp_unit = Unit("Bandit", UNIT::CLASS::BANDIT, temp_stats, 1);
    temp_stats = {48, 14, 25, 32, 34,  28, 19, 40, 15};
    temp_unit.setCaps(temp_stats);
    temp_stats = {60, 50, 20, 60, 70,  40, 30, 20,  10, 0};
    temp_unit.setGrowths(temp_stats);
    temp_unit.setBaseExp(0);
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
    temp_unit = Unit("Bandit", UNIT::CLASS::BANDIT, temp_stats, 1);
    temp_stats = {48, 14, 25, 32, 34,  28, 19, 40, 15};
    temp_unit.setCaps(temp_stats);
    temp_stats = {60, 50, 20, 60, 70,  40, 30, 20,  10, 0};
    temp_unit.setGrowths(temp_stats);
    temp_unit.setBaseExp(0);
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
    temp_unit = Unit("Bandit", UNIT::CLASS::BANDIT, temp_stats, 1);
    temp_stats = {48, 14, 25, 32, 34,  28, 19, 40, 15};
    temp_unit.setCaps(temp_stats);
    temp_stats = {60, 50, 20, 60, 70,  40, 30, 20,  10, 0};
    temp_unit.setGrowths(temp_stats);
    temp_unit.setBaseExp(0);
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
    temp_unit = Unit("Bandit", UNIT::CLASS::BANDIT, temp_stats, 1);
    temp_stats = {48, 14, 25, 32, 34,  28, 19, 40, 15};
    temp_unit.setCaps(temp_stats);
    temp_stats = {60, 50, 20, 60, 70,  40, 30, 20,  10, 0};
    temp_unit.setGrowths(temp_stats);
    temp_unit.setBaseExp(0);
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
    temp_unit = Unit("Bandit", UNIT::CLASS::BANDIT, temp_stats, 1);
    temp_stats = {48, 14, 25, 32, 34,  28, 19, 40, 15};
    temp_unit.setCaps(temp_stats);
    temp_stats = {60, 50, 20, 60, 70,  40, 30, 20,  10, 0};
    temp_unit.setGrowths(temp_stats);
    temp_unit.setBaseExp(0);
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
    temp_unit = Unit("Bandit", UNIT::CLASS::BANDIT, temp_stats, 1);
    temp_stats = {48, 14, 25, 32, 34,  28, 19, 40, 15};
    temp_unit.setCaps(temp_stats);
    temp_stats = {60, 50, 20, 60, 70,  40, 30, 20,  10, 0};
    temp_unit.setGrowths(temp_stats);
    temp_unit.setBaseExp(0);
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
    temp_unit = Unit("Bandit", UNIT::CLASS::BANDIT, temp_stats, 1);
    temp_stats = {48, 14, 25, 32, 34,  28, 19, 40, 15};
    temp_unit.setCaps(temp_stats);
    temp_stats = {60, 50, 20, 60, 70,  40, 30, 20,  10, 0};
    temp_unit.setGrowths(temp_stats);
    temp_unit.setBaseExp(0);
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
    temp_unit = Unit("Bandit", UNIT::CLASS::BANDIT, temp_stats, 1);
    temp_stats = {48, 14, 25, 32, 34,  28, 19, 40, 15};
    temp_unit.setCaps(temp_stats);
    temp_stats = {60, 50, 20, 60, 70,  40, 30, 20,  10, 0};
    temp_unit.setGrowths(temp_stats);
    temp_unit.setBaseExp(0);
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
    temp_unit = Unit("Bandit", UNIT::CLASS::BANDIT, temp_stats, 1);
    temp_stats = {48, 14, 25, 32, 34,  28, 19, 40, 15};
    temp_unit.setCaps(temp_stats);
    temp_stats = {60, 50, 20, 60, 70,  40, 30, 20,  10, 0};
    temp_unit.setGrowths(temp_stats);
    temp_unit.setBaseExp(0);
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
    temp_unit = Unit("Bandit", UNIT::CLASS::BANDIT, temp_stats, 1);
    temp_stats = {48, 14, 25, 32, 34,  28, 19, 40, 15};
    temp_unit.setCaps(temp_stats);
    temp_stats = {60, 50, 20, 60, 70,  40, 30, 20,  10, 0};
    temp_unit.setGrowths(temp_stats);
    temp_unit.setBaseExp(0);
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



