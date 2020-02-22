#include "unit.hpp"
#include "shared.hpp"

Unit::Unit() {
    equipped.left = -1;
    equipped.right = -1;
}

Unit::Unit(const std::string in_name, const std::string in_class, const Unit_stats in_bases, const std::string in_sex, Map_enemy in_map_enemy) : Unit(in_name, in_class, in_bases, in_sex) {
    setMap_enemy(in_map_enemy);
}

Unit::Unit(const std::string in_name, const std::string in_class, const Unit_stats in_bases, const bool in_sex, Map_enemy in_map_enemy) : Unit(in_name, in_class, in_bases, in_sex) {
    setMap_enemy(in_map_enemy);
}

Unit::Unit(const std::string in_name, const std::string in_class, const Unit_stats in_bases, const std::string in_sex) : Unit(in_name, in_class, in_bases) {
    setSex(in_sex);
}

Unit::Unit(const std::string in_name, const std::string in_class, const Unit_stats in_bases, const bool in_sex) : Unit(in_name, in_class, in_bases) {
    setSex(in_sex);
}

Unit::Unit(const std::string in_name, const std::string in_class, const Unit_stats in_bases) : Unit(in_name, in_bases) {
    class_name = in_class;
    setMvttype();
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

void Unit::setEquipment(Inventory_item * in_equipment) {
    for (short unsigned int i = 0; i < DEFAULT::EQUIPMENT_SIZE; i++) {
        equipment[i] = in_equipment[i];
    }
}
void Unit::setEquipped(Equipped in_equipped) {
    equipped = in_equipped;
}

std::vector<std::string> Unit::getEquippables() {
    return(equippables);
}

void Unit::setEquippable() {
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

void Unit::setClassname() {
    switch(class_index) {
        case UNIT_CLASS::MERCENARY:
            class_name = "Mercenary";
            break;
        case UNIT_CLASS::LORD:
            class_name = "Lord";
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
        case UNIT_CLASS::ARCHER:
            class_name = "Archer";
            break;
        case UNIT_CLASS::DUKE:
            class_name = "Duke";
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

void Unit::setMvttype() {
    if ((class_index == UNIT_CLASS::FENCER) || (class_index == UNIT_CLASS::MOUSQUETAIRE) || (class_index == UNIT_CLASS::DUELIST) || (class_index == UNIT_CLASS::LORD) || (class_index == UNIT_CLASS::DUKE) || (class_index == UNIT_CLASS::ARCHER) || (class_index == UNIT_CLASS::MARKSMAN) || (class_index == UNIT_CLASS::MERCENARY) || (class_index == UNIT_CLASS::HERO) || (class_index == UNIT_CLASS::PIKEMAN)) {
        mvt_type = MVT_TYPE::FOOT_SLOW;
    }

    if ((class_index == UNIT_CLASS::PICKPOCKET) || (class_index == UNIT_CLASS::THIEF) || (class_index == UNIT_CLASS::ASSASSIN) || (class_index == UNIT_CLASS::DEMON)) {
        mvt_type = MVT_TYPE::FOOT_FAST;
    }

    if ((class_index == UNIT_CLASS::MAGE) || (class_index == UNIT_CLASS::BATTLEMAGE) || (class_index == UNIT_CLASS::SAGE) || (class_index == UNIT_CLASS::ORACLE) || (class_index == UNIT_CLASS::PRIEST) || (class_index == UNIT_CLASS::CLERIC) || (class_index == UNIT_CLASS::BISHOP) || (class_index == UNIT_CLASS::ANGELIC_INCARNATE) || (class_index == UNIT_CLASS::DEMONIC_INCARNATE) || (class_index == UNIT_CLASS::ANGEL) || (class_index == UNIT_CLASS::DEMON)) {
        mvt_type = MVT_TYPE::MAGES;
    }

    if ((class_index == UNIT_CLASS::CAVALIER) || (class_index == UNIT_CLASS::ARCHER_RIDER) || (class_index == UNIT_CLASS::MARKSMAN_RIDER) || (class_index == UNIT_CLASS::LORD_RIDER) || (class_index == UNIT_CLASS::DUKE_RIDER)) {
        mvt_type = MVT_TYPE::RIDERS_SLOW;
    }

    if ((class_index == UNIT_CLASS::PALADIN) || (class_index == UNIT_CLASS::TROUBADOUR)) {
        mvt_type = MVT_TYPE::RIDERS_FAST;
    }

    if ((class_index == UNIT_CLASS::PEGASUS_KNIGHT) || (class_index == UNIT_CLASS::ANGEL)) {
        mvt_type = MVT_TYPE::FLIERS;
    }

    if ((class_index == UNIT_CLASS::KNIGHT) || (class_index == UNIT_CLASS::GENERAL)) {
        mvt_type = MVT_TYPE::ARMORS;
    }

    if ((class_index == UNIT_CLASS::CORSAIR) || (class_index == UNIT_CLASS::VIKING)) {
        mvt_type = MVT_TYPE::PIRATES;
    }

    if ((class_index == UNIT_CLASS::BANDIT) || (class_index == UNIT_CLASS::RAVAGER)) {
        mvt_type = MVT_TYPE::BANDITS;
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

void Unit::setArmy(const std::string in_army) {
    army = in_army;
}

void Unit::setArmy(const char in_army) {
    army = in_army;
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

std::unordered_map<std::string, Unit> all_units;

void baseUnits() {
    printf("Making base units \n");
    Unit_stats temp;
    //hp,str,mag,skl,spd,luck,def,res,con,move
    temp = {17,  6,  2,  7,  7,   7,  4,  5,  6, 5};
    Unit main("Erwin", "Mercenary", temp, "M");
    temp = {48, 14, 25, 32, 34,  28, 19, 40, 15, 0};
    main.setCaps(temp);
    temp = {60, 50, 20, 60, 70,  40, 30, 20,  10, 0};
    main.setGrowths(temp);
    main.setExp(0);
    main.write("unit_test.txt");
    all_units["Erwin"] = main;
    
    temp = {18,  6,  2,  7,  7,   7,  4,  5,  6, 7};
    Unit reliable("Reliable", "Cavalier", temp, "M");
    temp = {48, 14, 25, 32, 34,  28, 19, 40, 15};
    reliable.setCaps(temp);
    temp = {60, 50, 20, 60, 70,  40, 30, 20,  10, 0};
    reliable.setGrowths(temp);
    reliable.setExp(100);
    all_units["Reliable"] = reliable;
    
    temp = {19,  6,  2,  7,  7,   7,  4,  5,  6,  7};
    Unit coward("Coward", "Cavalier", temp, "M");
    temp = {48, 14, 25, 32, 34,  28, 19, 40, 15};
    coward.setCaps(temp);
    temp = {60, 50, 20, 60, 70,  40, 30, 20,  10, 0};
    coward.setGrowths(temp);
    coward.setExp(200);
    all_units["Coward"] = coward;
    
    temp = {20,  6,  2,  7,  7,   7,  4,  5,  6,  6};
    Unit hjaigen("1H Jaigen", "Fencermaster", temp, "M");
    temp = {48, 14, 25, 32, 34,  28, 19, 40, 15};
    hjaigen.setCaps(temp);
    temp = {60, 50, 20, 60, 70,  40, 30, 20,  10, 0};
    hjaigen.setGrowths(temp);
    hjaigen.setExp(2200);
    all_units["1H Jaigen"] = hjaigen;

    temp = {14,  6,  2,  7,  7,   7,  4,  5,  6,  5};
    Unit lovely("Kiara", "Priestess", temp, "F");
    temp = {48, 14, 25, 32, 34,  28, 19, 40, 15};
    lovely.setCaps(temp);
    temp = {60, 50, 20, 60, 70,  40, 30, 20,  10, 0};
    lovely.setGrowths(temp);
    lovely.setExp(100);
    all_units["Kiara"] = lovely;
    
    temp = {16,  6,  2,  7,  7,   7,  4,  5,  6,  6};
    Unit hottie("Hottie", "Thief", temp, "M");
    temp = {48, 14, 25, 32, 34,  28, 19, 40, 15};
    hottie.setCaps(temp);
    temp = {60, 50, 20, 60, 70,  40, 30, 20,  10, 0};
    hottie.setGrowths(temp);
    hottie.setExp(2200);
    all_units["Hottie"] = hottie;
    
    temp = {22,  4,  5,  7,  6,   8,  4,  6,  5, 5}; // 4 or 5?
    Unit servil("Servil", "Knight", temp, "M");
    temp = {48, 14, 25, 32, 34,  28, 19, 40, 15};
    servil.setCaps(temp);
    temp = {60, 50, 20, 60, 70,  40, 30, 20,  10, 0};
    servil.setGrowths(temp);
    servil.setExp(500);
    all_units["Servil"] = servil;
    
    temp = {34,  4,  5,  7,  6,   8,  4,  6,  5, 5};
    Unit perignon("Pérignon", "Mage", temp, "M");
    temp = {48, 14, 25, 32, 34,  28, 19, 40, 15};
    perignon.setCaps(temp);
    temp = {60, 50, 20, 60, 70,  40, 30, 20,  10, 0};
    perignon.setGrowths(temp);
    perignon.setExp(1200);
    all_units["Pérignon"] = perignon;
    
    temp = {15,  4,  5,  7,  6,   8,  4,  6,  5, 5};
    Unit poet("Poet", "Mage", temp, "M");
    temp = {48, 14, 25, 32, 34,  28, 19, 40, 15};
    poet.setCaps(temp);
    temp = {60, 50, 20, 60, 70,  40, 30, 20,  10, 0};
    poet.setGrowths(temp);
    poet.setExp(400);
    all_units["Poet"] = poet;
    
    temp = {15,  4,  5,  7,  6,   8,  4,  6,  5, 5};
    Unit silou("Silou", "Mage", temp, "F");
    temp = {48, 14, 25, 32, 34,  28, 19, 40, 15};
    silou.setCaps(temp);
    temp = {60, 50, 20, 60, 70,  40, 30, 20,  10, 0};
    silou.setGrowths(temp);
    silou.setExp(400);
    all_units["Silou"] = silou;
}

void genericEnemies() {
    printf("Making generic enemies\n");
    Unit_stats temp;
    //hp,str,mag,skl,spd,luck,def,res,con,move

    temp = {15,  4,  5,  7,  6,   8,  4,  6,  5, 5};
    Unit generic_bandit("Bandit", "Bandit", temp, "M");
    temp = {48, 14, 25, 32, 34,  28, 19, 40, 15};
    generic_bandit.setCaps(temp);
    temp = {60, 50, 20, 60, 70,  40, 30, 20,  10, 0};
    generic_bandit.setGrowths(temp);
    generic_bandit.setExp(0);
    all_units["Bandit"] = generic_bandit;

    temp = {15,  4,  5,  7,  6,   8,  4,  6,  5, 5};
    Unit generic_Duelist("Duelist", "Duelist", temp, "M");
    temp = {48, 14, 25, 32, 34,  28, 19, 40, 15};
    generic_Duelist.setCaps(temp);
    temp = {60, 50, 20, 60, 70,  40, 30, 20,  10, 0};
    generic_Duelist.setGrowths(temp);
    generic_Duelist.setExp(0);
    all_units["Duelist"] = generic_Duelist;

    temp = {15,  4,  5,  7,  6,   8,  4,  6,  5, 5};
    Unit generic_Pickpocket("Pickpocket", "Pickpocket", temp, "M");
    temp = {48, 14, 25, 32, 34,  28, 19, 40, 15};
    generic_Pickpocket.setCaps(temp);
    temp = {60, 50, 20, 60, 70,  40, 30, 20,  10, 0};
    generic_Pickpocket.setGrowths(temp);
    generic_Pickpocket.setExp(0);
    all_units["Pickpocket"] = generic_Pickpocket;

    temp = {15,  4,  5,  7,  6,   8,  4,  6,  5, 5};
    Unit generic_Thief("Thief", "Thief", temp, "M");
    temp = {48, 14, 25, 32, 34,  28, 19, 40, 15};
    generic_Thief.setCaps(temp);
    temp = {60, 50, 20, 60, 70,  40, 30, 20,  10, 0};
    generic_Thief.setGrowths(temp);
    generic_Thief.setExp(0);
    all_units["Thief"] = generic_Thief;    

    temp = {15,  4,  5,  7,  6,   8,  4,  6,  5, 5};
    Unit generic_Assassin("Assassin", "Assassin", temp, "M");
    temp = {48, 14, 25, 32, 34,  28, 19, 40, 15};
    generic_Assassin.setCaps(temp);
    temp = {60, 50, 20, 60, 70,  40, 30, 20,  10, 0};
    generic_Assassin.setGrowths(temp);
    generic_Assassin.setExp(0);
    all_units["Assassin"] = generic_Assassin;

    temp = {15,  4,  5,  7,  6,   8,  4,  6,  5, 5};
    Unit generic_Archer("Archer", "Archer", temp, "M");
    temp = {48, 14, 25, 32, 34,  28, 19, 40, 15};
    generic_Archer.setCaps(temp);
    temp = {60, 50, 20, 60, 70,  40, 30, 20,  10, 0};
    generic_Archer.setGrowths(temp);
    generic_Archer.setExp(0);
    all_units["Archer"] = generic_Archer;

    temp = {15,  4,  5,  7,  6,   8,  4,  6,  5, 5};
    Unit generic_Marksman("Marksman", "Marksman", temp, "M");
    temp = {48, 14, 25, 32, 34,  28, 19, 40, 15};
    generic_Marksman.setCaps(temp);
    temp = {60, 50, 20, 60, 70,  40, 30, 20,  10, 0};
    generic_Marksman.setGrowths(temp);
    generic_Marksman.setExp(0);
    all_units["Marksman"] = generic_Marksman;

    temp = {15,  4,  5,  7,  6,   8,  4,  6,  5, 5};
    Unit generic_Mercenary("Mercenary", "Mercenary", temp, "M");
    temp = {48, 14, 25, 32, 34,  28, 19, 40, 15};
    generic_Mercenary.setCaps(temp);
    temp = {60, 50, 20, 60, 70,  40, 30, 20,  10, 0};
    generic_Mercenary.setGrowths(temp);
    generic_Mercenary.setExp(0);
    all_units["Mercenary"] = generic_Mercenary;

    temp = {15,  4,  5,  7,  6,   8,  4,  6,  5, 5};
    Unit generic_Hero("Hero", "Hero", temp, "M");
    temp = {48, 14, 25, 32, 34,  28, 19, 40, 15};
    generic_Hero.setCaps(temp);
    temp = {60, 50, 20, 60, 70,  40, 30, 20,  10, 0};
    generic_Hero.setGrowths(temp);
    generic_Hero.setExp(0);
    all_units["Hero"] = generic_Hero;

    temp = {15,  4,  5,  7,  6,   8,  4,  6,  5, 5};
    Unit generic_Corsair("Corsair", "Corsair", temp, "M");
    temp = {48, 14, 25, 32, 34,  28, 19, 40, 15};
    generic_Corsair.setCaps(temp);
    temp = {60, 50, 20, 60, 70,  40, 30, 20,  10, 0};
    generic_Corsair.setGrowths(temp);
    generic_Corsair.setExp(0);
    all_units["Corsair"] = generic_Corsair;

    temp = {15,  4,  5,  7,  6,   8,  4,  6,  5, 5};
    Unit generic_Viking("Viking", "Viking", temp, "M");
    temp = {48, 14, 25, 32, 34,  28, 19, 40, 15};
    generic_Viking.setCaps(temp);
    temp = {60, 50, 20, 60, 70,  40, 30, 20,  10, 0};
    generic_Viking.setGrowths(temp);
    generic_Viking.setExp(0);
    all_units["Viking"] = generic_Viking;

    temp = {15,  4,  5,  7,  6,   8,  4,  6,  5, 5};
    Unit generic_Ravager("Ravager", "Ravager", temp, "M");
    temp = {48, 14, 25, 32, 34,  28, 19, 40, 15};
    generic_Ravager.setCaps(temp);
    temp = {60, 50, 20, 60, 70,  40, 30, 20,  10, 0};
    generic_Ravager.setGrowths(temp);
    generic_Ravager.setExp(0);
    all_units["Ravager"] = generic_Ravager;

    temp = {15,  4,  5,  7,  6,   8,  4,  6,  5, 5};
    Unit generic_Cavalier("Cavalier", "Cavalier", temp, "M");
    temp = {48, 14, 25, 32, 34,  28, 19, 40, 15};
    generic_Cavalier.setCaps(temp);
    temp = {60, 50, 20, 60, 70,  40, 30, 20,  10, 0};
    generic_Cavalier.setGrowths(temp);
    generic_Cavalier.setExp(0);
    all_units["Cavalier"] = generic_Cavalier;

    temp = {15,  4,  5,  7,  6,   8,  4,  6,  5, 5};
    Unit generic_Paladin("Paladin", "Paladin", temp, "M");
    temp = {48, 14, 25, 32, 34,  28, 19, 40, 15};
    generic_Paladin.setCaps(temp);
    temp = {60, 50, 20, 60, 70,  40, 30, 20,  10, 0};
    generic_Paladin.setGrowths(temp);
    generic_Paladin.setExp(0);
    all_units["Paladin"] = generic_Paladin;

    temp = {15,  4,  5,  7,  6,   8,  4,  6,  5, 5};
    Unit generic_fencer("Fencer", "Fencer", temp, "M");
    temp = {48, 14, 25, 32, 34,  28, 19, 40, 15};
    generic_fencer.setCaps(temp);
    temp = {60, 50, 20, 60, 70,  40, 30, 20,  10, 0};
    generic_fencer.setGrowths(temp);
    generic_fencer.setExp(0);
    all_units["Fencer"] = generic_fencer;

    temp = {15,  4,  5,  7,  6,   8,  4,  6,  5, 5};
    Unit generic_mousquetaire("Mousquetaire", "Mousquetaire", temp, "M");
    temp = {48, 14, 25, 32, 34,  28, 19, 40, 15};
    generic_mousquetaire.setCaps(temp);
    temp = {60, 50, 20, 60, 70,  40, 30, 20,  10, 0};
    generic_mousquetaire.setGrowths(temp);
    generic_mousquetaire.setExp(0);
    all_units["Mousquetaire"] = generic_mousquetaire;
}

std::unordered_map<std::string, Unit> chaptestEnemies() {
    Unit_stats temp_stats;
    Inventory_item temp_equipment[DEFAULT::EQUIPMENT_SIZE];
    Equipped temp_equipped;
    Point temp_position;
    std::unordered_map<std::string, Unit> made_units;
    temp_stats = {15,  4,  5,  7,  6,   8,  4,  6,  5, 5};
    Unit bandit_test("Bandit", "Bandit", temp_stats, "M");
    temp_stats = {48, 14, 25, 32, 34,  28, 19, 40, 15};
    bandit_test.setCaps(temp_stats);
    temp_stats = {60, 50, 20, 60, 70,  40, 30, 20,  10, 0};
    bandit_test.setGrowths(temp_stats);
    bandit_test.setExp(0);
  //temp_equipment[0].id = "Iron Axe";
  //temp_equipment[1].id = "Wooden Shield";
    bandit_test.setEquipment(temp_equipment);
    temp_equipped = {0, 1};
    bandit_test.setEquipped(temp_equipped);
    temp_position = {4, 5};
    bandit_test.setPos(temp_position);
    made_units["Bandit1test"] = bandit_test;
    return(made_units);
} 

std::unordered_map<std::string, Unit> chap1Enemies() {
    Unit_stats temp_stats;
    Inventory_item temp_equipment[DEFAULT::EQUIPMENT_SIZE];
    Equipped temp_equipped;
    Point temp_position;
    std::unordered_map<std::string, Unit> made_units;
    temp_stats = {15,  4,  5,  7,  6,   8,  4,  6,  5, 5};
    Unit bandit_test("Bandit", "Bandit", temp_stats, "M");
    temp_stats = {48, 14, 25, 32, 34,  28, 19, 40, 15};
    bandit_test.setCaps(temp_stats);
    temp_stats = {60, 50, 20, 60, 70,  40, 30, 20,  10, 0};
    bandit_test.setGrowths(temp_stats);
    bandit_test.setExp(0);
  //temp_equipment[0].id = "Iron Axe";
  //temp_equipment[1].id = "Wooden Shield";
    bandit_test.setEquipment(temp_equipment);
    temp_equipped = {0, 1};
    bandit_test.setEquipped(temp_equipped);
    temp_position = {4, 5};
    bandit_test.setPos(temp_position);
    made_units["Bandit1test"] = bandit_test;
    return(made_units);
} 

std::unordered_map<std::string, Unit> chap2Enemies() {
    Unit_stats temp_stats;
    Inventory_item temp_equipment[DEFAULT::EQUIPMENT_SIZE];
    Equipped temp_equipped;
    Point temp_position;
    std::unordered_map<std::string, Unit> made_units;
    temp_stats = {15,  4,  5,  7,  6,   8,  4,  6,  5, 5};
    Unit bandit_test("Bandit", "Bandit", temp_stats, "M");
    temp_stats = {48, 14, 25, 32, 34,  28, 19, 40, 15};
    bandit_test.setCaps(temp_stats);
    temp_stats = {60, 50, 20, 60, 70,  40, 30, 20,  10, 0};
    bandit_test.setGrowths(temp_stats);
    bandit_test.setExp(0);
  //temp_equipment[0].id = "Iron Axe";
  //temp_equipment[1].id = "Wooden Shield";
    bandit_test.setEquipment(temp_equipment);
    temp_equipped = {0, 1};
    bandit_test.setEquipped(temp_equipped);
    temp_position = {4, 5};
    bandit_test.setPos(temp_position);
    made_units["Bandit1test"] = bandit_test;
    return(made_units);
}

std::unordered_map<std::string, Unit> chap3Enemies() {
    Unit_stats temp_stats;
    Inventory_item temp_equipment[DEFAULT::EQUIPMENT_SIZE];
    Equipped temp_equipped;
    Point temp_position;
    std::unordered_map<std::string, Unit> made_units;
    temp_stats = {15,  4,  5,  7,  6,   8,  4,  6,  5, 5};
    Unit bandit_test("Bandit", "Bandit", temp_stats, "M");
    temp_stats = {48, 14, 25, 32, 34,  28, 19, 40, 15};
    bandit_test.setCaps(temp_stats);
    temp_stats = {60, 50, 20, 60, 70,  40, 30, 20,  10, 0};
    bandit_test.setGrowths(temp_stats);
    bandit_test.setExp(0);
  //temp_equipment[0].id = "Iron Axe";
  //temp_equipment[1].id = "Wooden Shield";
    bandit_test.setEquipment(temp_equipment);
    temp_equipped = {0, 1};
    bandit_test.setEquipped(temp_equipped);
    temp_position = {4, 5};
    bandit_test.setPos(temp_position);
    made_units["Bandit1test"] = bandit_test;
    return(made_units);
} 

std::unordered_map<std::string, Unit> chap4Enemies() {
    Unit_stats temp_stats;
    Inventory_item temp_equipment[DEFAULT::EQUIPMENT_SIZE];
    Equipped temp_equipped;
    Point temp_position;
    std::unordered_map<std::string, Unit> made_units;
    temp_stats = {15,  4,  5,  7,  6,   8,  4,  6,  5, 5};
    Unit bandit_test("Bandit", "Bandit", temp_stats, "M");
    temp_stats = {48, 14, 25, 32, 34,  28, 19, 40, 15};
    bandit_test.setCaps(temp_stats);
    temp_stats = {60, 50, 20, 60, 70,  40, 30, 20,  10, 0};
    bandit_test.setGrowths(temp_stats);
    bandit_test.setExp(0);
  //temp_equipment[0].id = "Iron Axe";
  //temp_equipment[1].id = "Wooden Shield";
    bandit_test.setEquipment(temp_equipment);
    temp_equipped = {0, 1};
    bandit_test.setEquipped(temp_equipped);
    temp_position = {4, 5};
    bandit_test.setPos(temp_position);
    made_units["Bandit1test"] = bandit_test;
    return(made_units);
}

std::unordered_map<std::string, Unit> chap5Enemies() {
    Unit_stats temp_stats;
    Inventory_item temp_equipment[DEFAULT::EQUIPMENT_SIZE];
    Equipped temp_equipped;
    Point temp_position;
    std::unordered_map<std::string, Unit> made_units;
    temp_stats = {15,  4,  5,  7,  6,   8,  4,  6,  5, 5};
    Unit bandit_test("Bandit", "Bandit", temp_stats, "M");
    temp_stats = {48, 14, 25, 32, 34,  28, 19, 40, 15};
    bandit_test.setCaps(temp_stats);
    temp_stats = {60, 50, 20, 60, 70,  40, 30, 20,  10, 0};
    bandit_test.setGrowths(temp_stats);
    bandit_test.setExp(0);
  //temp_equipment[0].id = "Iron Axe";
  //temp_equipment[1].id = "Wooden Shield";
    bandit_test.setEquipment(temp_equipment);
    temp_equipped = {0, 1};
    bandit_test.setEquipped(temp_equipped);
    temp_position = {4, 5};
    bandit_test.setPos(temp_position);
    made_units["Bandit1test"] = bandit_test;
    return(made_units);
}

std::unordered_map<std::string, Unit> chap6Enemies() {
    Unit_stats temp_stats;
    Inventory_item temp_equipment[DEFAULT::EQUIPMENT_SIZE];
    Equipped temp_equipped;
    Point temp_position;
    std::unordered_map<std::string, Unit> made_units;
    temp_stats = {15,  4,  5,  7,  6,   8,  4,  6,  5, 5};
    Unit bandit_test("Bandit", "Bandit", temp_stats, "M");
    temp_stats = {48, 14, 25, 32, 34,  28, 19, 40, 15};
    bandit_test.setCaps(temp_stats);
    temp_stats = {60, 50, 20, 60, 70,  40, 30, 20,  10, 0};
    bandit_test.setGrowths(temp_stats);
    bandit_test.setExp(0);
  //temp_equipment[0].id = "Iron Axe";
  //temp_equipment[1].id = "Wooden Shield";
    bandit_test.setEquipment(temp_equipment);
    temp_equipped = {0, 1};
    bandit_test.setEquipped(temp_equipped);
    temp_position = {4, 5};
    bandit_test.setPos(temp_position);
    made_units["Bandit1test"] = bandit_test;
    return(made_units);
} 

std::unordered_map<std::string, Unit> chap7Enemies() {
    Unit_stats temp_stats;
    Inventory_item temp_equipment[DEFAULT::EQUIPMENT_SIZE];
    Equipped temp_equipped;
    Point temp_position;
    std::unordered_map<std::string, Unit> made_units;
    temp_stats = {15,  4,  5,  7,  6,   8,  4,  6,  5, 5};
    Unit bandit_test("Bandit", "Bandit", temp_stats, "M");
    temp_stats = {48, 14, 25, 32, 34,  28, 19, 40, 15};
    bandit_test.setCaps(temp_stats);
    temp_stats = {60, 50, 20, 60, 70,  40, 30, 20,  10, 0};
    bandit_test.setGrowths(temp_stats);
    bandit_test.setExp(0);
  //temp_equipment[0].id = "Iron Axe";
  //temp_equipment[1].id = "Wooden Shield";
    bandit_test.setEquipment(temp_equipment);
    temp_equipped = {0, 1};
    bandit_test.setEquipped(temp_equipped);
    temp_position = {4, 5};
    bandit_test.setPos(temp_position);
    made_units["Bandit1test"] = bandit_test;
    return(made_units);
}

std::unordered_map<std::string, Unit> chap8Enemies() {
    Unit_stats temp_stats;
    Inventory_item temp_equipment[DEFAULT::EQUIPMENT_SIZE];
    Equipped temp_equipped;
    Point temp_position;
    std::unordered_map<std::string, Unit> made_units;
    temp_stats = {15,  4,  5,  7,  6,   8,  4,  6,  5, 5};
    Unit bandit_test("Bandit", "Bandit", temp_stats, "M");
    temp_stats = {48, 14, 25, 32, 34,  28, 19, 40, 15};
    bandit_test.setCaps(temp_stats);
    temp_stats = {60, 50, 20, 60, 70,  40, 30, 20,  10, 0};
    bandit_test.setGrowths(temp_stats);
    bandit_test.setExp(0);
  //temp_equipment[0].id = "Iron Axe";
  //temp_equipment[1].id = "Wooden Shield";
    bandit_test.setEquipment(temp_equipment);
    temp_equipped = {0, 1};
    bandit_test.setEquipped(temp_equipped);
    temp_position = {4, 5};
    bandit_test.setPos(temp_position);
    made_units["Bandit1test"] = bandit_test;
    return(made_units);
}

std::unordered_map<std::string, Unit> chap9Enemies() {
    Unit_stats temp_stats;
    Inventory_item temp_equipment[DEFAULT::EQUIPMENT_SIZE];
    Equipped temp_equipped;
    Point temp_position;
    std::unordered_map<std::string, Unit> made_units;
    temp_stats = {15,  4,  5,  7,  6,   8,  4,  6,  5, 5};
    Unit bandit_test("Bandit", "Bandit", temp_stats, "M");
    temp_stats = {48, 14, 25, 32, 34,  28, 19, 40, 15};
    bandit_test.setCaps(temp_stats);
    temp_stats = {60, 50, 20, 60, 70,  40, 30, 20,  10, 0};
    bandit_test.setGrowths(temp_stats);
    bandit_test.setExp(0);
  //temp_equipment[0].id = "Iron Axe";
  //temp_equipment[1].id = "Wooden Shield";
    bandit_test.setEquipment(temp_equipment);
    temp_equipped = {0, 1};
    bandit_test.setEquipped(temp_equipped);
    temp_position = {4, 5};
    bandit_test.setPos(temp_position);
    made_units["Bandit1test"] = bandit_test;
    return(made_units);
}

std::unordered_map<std::string, Unit> chap10Enemies() {
    Unit_stats temp_stats;
    Inventory_item temp_equipment[DEFAULT::EQUIPMENT_SIZE];
    Equipped temp_equipped;
    Point temp_position;
    std::unordered_map<std::string, Unit> made_units;
    temp_stats = {15,  4,  5,  7,  6,   8,  4,  6,  5, 5};
    Unit bandit_test("Bandit", "Bandit", temp_stats, "M");
    temp_stats = {48, 14, 25, 32, 34,  28, 19, 40, 15};
    bandit_test.setCaps(temp_stats);
    temp_stats = {60, 50, 20, 60, 70,  40, 30, 20,  10, 0};
    bandit_test.setGrowths(temp_stats);
    bandit_test.setExp(0);
  //temp_equipment[0].id = "Iron Axe";
  //temp_equipment[1].id = "Wooden Shield";
    bandit_test.setEquipment(temp_equipment);
    temp_equipped = {0, 1};
    bandit_test.setEquipped(temp_equipped);
    temp_position = {4, 5};
    bandit_test.setPos(temp_position);
    made_units["Bandit1test"] = bandit_test;
    return(made_units);
}

std::unordered_map<std::string, Unit> chap11Enemies() {
    Unit_stats temp_stats;
    Inventory_item temp_equipment[DEFAULT::EQUIPMENT_SIZE];
    Equipped temp_equipped;
    Point temp_position;
    std::unordered_map<std::string, Unit> made_units;
    temp_stats = {15,  4,  5,  7,  6,   8,  4,  6,  5, 5};
    Unit bandit_test("Bandit", "Bandit", temp_stats, "M");
    temp_stats = {48, 14, 25, 32, 34,  28, 19, 40, 15};
    bandit_test.setCaps(temp_stats);
    temp_stats = {60, 50, 20, 60, 70,  40, 30, 20,  10, 0};
    bandit_test.setGrowths(temp_stats);
    bandit_test.setExp(0);
  //temp_equipment[0].id = "Iron Axe";
  //temp_equipment[1].id = "Wooden Shield";
    bandit_test.setEquipment(temp_equipment);
    temp_equipped = {0, 1};
    bandit_test.setEquipped(temp_equipped);
    temp_position = {4, 5};
    bandit_test.setPos(temp_position);
    made_units["Bandit1test"] = bandit_test;
    return(made_units);
}

std::unordered_map<std::string, Unit> chap12Enemies() {
    Unit_stats temp_stats;
    Inventory_item temp_equipment[DEFAULT::EQUIPMENT_SIZE];
    Equipped temp_equipped;
    Point temp_position;
    std::unordered_map<std::string, Unit> made_units;
    temp_stats = {15,  4,  5,  7,  6,   8,  4,  6,  5, 5};
    Unit bandit_test("Bandit", "Bandit", temp_stats, "M");
    temp_stats = {48, 14, 25, 32, 34,  28, 19, 40, 15};
    bandit_test.setCaps(temp_stats);
    temp_stats = {60, 50, 20, 60, 70,  40, 30, 20,  10, 0};
    bandit_test.setGrowths(temp_stats);
    bandit_test.setExp(0);
  //temp_equipment[0].id = "Iron Axe";
  //temp_equipment[1].id = "Wooden Shield";
    bandit_test.setEquipment(temp_equipment);
    temp_equipped = {0, 1};
    bandit_test.setEquipped(temp_equipped);
    temp_position = {4, 5};
    bandit_test.setPos(temp_position);
    made_units["Bandit1test"] = bandit_test;
    return(made_units);
}

std::unordered_map<std::string, Unit> chap13Enemies() {
    Unit_stats temp_stats;
    Inventory_item temp_equipment[DEFAULT::EQUIPMENT_SIZE];
    Equipped temp_equipped;
    Point temp_position;
    std::unordered_map<std::string, Unit> made_units;
    temp_stats = {15,  4,  5,  7,  6,   8,  4,  6,  5, 5};
    Unit bandit_test("Bandit", "Bandit", temp_stats, "M");
    temp_stats = {48, 14, 25, 32, 34,  28, 19, 40, 15};
    bandit_test.setCaps(temp_stats);
    temp_stats = {60, 50, 20, 60, 70,  40, 30, 20,  10, 0};
    bandit_test.setGrowths(temp_stats);
    bandit_test.setExp(0);
  //temp_equipment[0].id = "Iron Axe";
  //temp_equipment[1].id = "Wooden Shield";
    bandit_test.setEquipment(temp_equipment);
    temp_equipped = {0, 1};
    bandit_test.setEquipped(temp_equipped);
    temp_position = {4, 5};
    bandit_test.setPos(temp_position);
    made_units["Bandit1test"] = bandit_test;
    return(made_units);
}

std::unordered_map<std::string, Unit> chap14Enemies() {
    Unit_stats temp_stats;
    Inventory_item temp_equipment[DEFAULT::EQUIPMENT_SIZE];
    Equipped temp_equipped;
    Point temp_position;
    std::unordered_map<std::string, Unit> made_units;
    temp_stats = {15,  4,  5,  7,  6,   8,  4,  6,  5, 5};
    Unit bandit_test("Bandit", "Bandit", temp_stats, "M");
    temp_stats = {48, 14, 25, 32, 34,  28, 19, 40, 15};
    bandit_test.setCaps(temp_stats);
    temp_stats = {60, 50, 20, 60, 70,  40, 30, 20,  10, 0};
    bandit_test.setGrowths(temp_stats);
    bandit_test.setExp(0);
  //temp_equipment[0].id = "Iron Axe";
  //temp_equipment[1].id = "Wooden Shield";
    bandit_test.setEquipment(temp_equipment);
    temp_equipped = {0, 1};
    bandit_test.setEquipped(temp_equipped);
    temp_position = {4, 5};
    bandit_test.setPos(temp_position);
    made_units["Bandit1test"] = bandit_test;
    return(made_units);
}

std::unordered_map<std::string, Unit> chap15Enemies() {
    Unit_stats temp_stats;
    Inventory_item temp_equipment[DEFAULT::EQUIPMENT_SIZE];
    Equipped temp_equipped;
    Point temp_position;
    std::unordered_map<std::string, Unit> made_units;
    temp_stats = {15,  4,  5,  7,  6,   8,  4,  6,  5, 5};
    Unit bandit_test("Bandit", "Bandit", temp_stats, "M");
    temp_stats = {48, 14, 25, 32, 34,  28, 19, 40, 15};
    bandit_test.setCaps(temp_stats);
    temp_stats = {60, 50, 20, 60, 70,  40, 30, 20,  10, 0};
    bandit_test.setGrowths(temp_stats);
    bandit_test.setExp(0);
  //temp_equipment[0].id = "Iron Axe";
  //temp_equipment[1].id = "Wooden Shield";
    bandit_test.setEquipment(temp_equipment);
    temp_equipped = {0, 1};
    bandit_test.setEquipped(temp_equipped);
    temp_position = {4, 5};
    bandit_test.setPos(temp_position);
    made_units["Bandit1test"] = bandit_test;
    return(made_units);
}

std::unordered_map<std::string, Unit> chap16Enemies() {
    Unit_stats temp_stats;
    Inventory_item temp_equipment[DEFAULT::EQUIPMENT_SIZE];
    Equipped temp_equipped;
    Point temp_position;
    std::unordered_map<std::string, Unit> made_units;
    temp_stats = {15,  4,  5,  7,  6,   8,  4,  6,  5, 5};
    Unit bandit_test("Bandit", "Bandit", temp_stats, "M");
    temp_stats = {48, 14, 25, 32, 34,  28, 19, 40, 15};
    bandit_test.setCaps(temp_stats);
    temp_stats = {60, 50, 20, 60, 70,  40, 30, 20,  10, 0};
    bandit_test.setGrowths(temp_stats);
    bandit_test.setExp(0);
  //temp_equipment[0].id = "Iron Axe";
  //temp_equipment[1].id = "Wooden Shield";
    bandit_test.setEquipment(temp_equipment);
    temp_equipped = {0, 1};
    bandit_test.setEquipped(temp_equipped);
    temp_position = {4, 5};
    bandit_test.setPos(temp_position);
    made_units["Bandit1test"] = bandit_test;
    return(made_units);
}

std::unordered_map<std::string, Unit> chap17Enemies() {
    Unit_stats temp_stats;
    Inventory_item temp_equipment[DEFAULT::EQUIPMENT_SIZE];
    Equipped temp_equipped;
    Point temp_position;
    std::unordered_map<std::string, Unit> made_units;
    temp_stats = {15,  4,  5,  7,  6,   8,  4,  6,  5, 5};
    Unit bandit_test("Bandit", "Bandit", temp_stats, "M");
    temp_stats = {48, 14, 25, 32, 34,  28, 19, 40, 15};
    bandit_test.setCaps(temp_stats);
    temp_stats = {60, 50, 20, 60, 70,  40, 30, 20,  10, 0};
    bandit_test.setGrowths(temp_stats);
    bandit_test.setExp(0);
  //temp_equipment[0].id = "Iron Axe";
  //temp_equipment[1].id = "Wooden Shield";
    bandit_test.setEquipment(temp_equipment);
    temp_equipped = {0, 1};
    bandit_test.setEquipped(temp_equipped);
    temp_position = {4, 5};
    bandit_test.setPos(temp_position);
    made_units["Bandit1test"] = bandit_test;
    return(made_units);
}

std::unordered_map<std::string, Unit> chap18Enemies() {
    Unit_stats temp_stats;
    Inventory_item temp_equipment[DEFAULT::EQUIPMENT_SIZE];
    Equipped temp_equipped;
    Point temp_position;
    std::unordered_map<std::string, Unit> made_units;
    temp_stats = {15,  4,  5,  7,  6,   8,  4,  6,  5, 5};
    Unit bandit_test("Bandit", "Bandit", temp_stats, "M");
    temp_stats = {48, 14, 25, 32, 34,  28, 19, 40, 15};
    bandit_test.setCaps(temp_stats);
    temp_stats = {60, 50, 20, 60, 70,  40, 30, 20,  10, 0};
    bandit_test.setGrowths(temp_stats);
    bandit_test.setExp(0);
  //temp_equipment[0].id = "Iron Axe";
  //temp_equipment[1].id = "Wooden Shield";
    bandit_test.setEquipment(temp_equipment);
    temp_equipped = {0, 1};
    bandit_test.setEquipped(temp_equipped);
    temp_position = {4, 5};
    bandit_test.setPos(temp_position);
    made_units["Bandit1test"] = bandit_test;
    return(made_units);
}

std::unordered_map<std::string, Unit> chap19Enemies() {
    Unit_stats temp_stats;
    Inventory_item temp_equipment[DEFAULT::EQUIPMENT_SIZE];
    Equipped temp_equipped;
    Point temp_position;
    std::unordered_map<std::string, Unit> made_units;
    temp_stats = {15,  4,  5,  7,  6,   8,  4,  6,  5, 5};
    Unit bandit_test("Bandit", "Bandit", temp_stats, "M");
    temp_stats = {48, 14, 25, 32, 34,  28, 19, 40, 15};
    bandit_test.setCaps(temp_stats);
    temp_stats = {60, 50, 20, 60, 70,  40, 30, 20,  10, 0};
    bandit_test.setGrowths(temp_stats);
    bandit_test.setExp(0);
  //temp_equipment[0].id = "Iron Axe";
  //temp_equipment[1].id = "Wooden Shield";
    bandit_test.setEquipment(temp_equipment);
    temp_equipped = {0, 1};
    bandit_test.setEquipped(temp_equipped);
    temp_position = {4, 5};
    bandit_test.setPos(temp_position);
    made_units["Bandit1test"] = bandit_test;
    return(made_units);
}

std::unordered_map<std::string, Unit> chap20Enemies() {
    Unit_stats temp_stats;
    Inventory_item temp_equipment[DEFAULT::EQUIPMENT_SIZE];
    Equipped temp_equipped;
    Point temp_position;
    std::unordered_map<std::string, Unit> made_units;
    temp_stats = {15,  4,  5,  7,  6,   8,  4,  6,  5, 5};
    Unit bandit_test("Bandit", "Bandit", temp_stats, "M");
    temp_stats = {48, 14, 25, 32, 34,  28, 19, 40, 15};
    bandit_test.setCaps(temp_stats);
    temp_stats = {60, 50, 20, 60, 70,  40, 30, 20,  10, 0};
    bandit_test.setGrowths(temp_stats);
    bandit_test.setExp(0);
  //temp_equipment[0].id = "Iron Axe";
  //temp_equipment[1].id = "Wooden Shield";
    bandit_test.setEquipment(temp_equipment);
    temp_equipped = {0, 1};
    bandit_test.setEquipped(temp_equipped);
    temp_position = {4, 5};
    bandit_test.setPos(temp_position);
    made_units["Bandit1test"] = bandit_test;
    return(made_units);
}

std::unordered_map<std::string, Unit> chap21Enemies() {
    Unit_stats temp_stats;
    Inventory_item temp_equipment[DEFAULT::EQUIPMENT_SIZE];
    Equipped temp_equipped;
    Point temp_position;
    std::unordered_map<std::string, Unit> made_units;
    temp_stats = {15,  4,  5,  7,  6,   8,  4,  6,  5, 5};
    Unit bandit_test("Bandit", "Bandit", temp_stats, "M");
    temp_stats = {48, 14, 25, 32, 34,  28, 19, 40, 15};
    bandit_test.setCaps(temp_stats);
    temp_stats = {60, 50, 20, 60, 70,  40, 30, 20,  10, 0};
    bandit_test.setGrowths(temp_stats);
    bandit_test.setExp(0);
  //temp_equipment[0].id = "Iron Axe";
  //temp_equipment[1].id = "Wooden Shield";
    bandit_test.setEquipment(temp_equipment);
    temp_equipped = {0, 1};
    bandit_test.setEquipped(temp_equipped);
    temp_position = {4, 5};
    bandit_test.setPos(temp_position);
    made_units["Bandit1test"] = bandit_test;
    return(made_units);
}

std::unordered_map<std::string, Unit> chap22Enemies() {
    Unit_stats temp_stats;
    Inventory_item temp_equipment[DEFAULT::EQUIPMENT_SIZE];
    Equipped temp_equipped;
    Point temp_position;
    std::unordered_map<std::string, Unit> made_units;
    temp_stats = {15,  4,  5,  7,  6,   8,  4,  6,  5, 5};
    Unit bandit_test("Bandit", "Bandit", temp_stats, "M");
    temp_stats = {48, 14, 25, 32, 34,  28, 19, 40, 15};
    bandit_test.setCaps(temp_stats);
    temp_stats = {60, 50, 20, 60, 70,  40, 30, 20,  10, 0};
    bandit_test.setGrowths(temp_stats);
    bandit_test.setExp(0);
  //temp_equipment[0].id = "Iron Axe";
  //temp_equipment[1].id = "Wooden Shield";
    bandit_test.setEquipment(temp_equipment);
    temp_equipped = {0, 1};
    bandit_test.setEquipped(temp_equipped);
    temp_position = {4, 5};
    bandit_test.setPos(temp_position);
    made_units["Bandit1test"] = bandit_test;
    return(made_units);
}

std::unordered_map<std::string, Unit> chap23Enemies() {
    Unit_stats temp_stats;
    Inventory_item temp_equipment[DEFAULT::EQUIPMENT_SIZE];
    Equipped temp_equipped;
    Point temp_position;
    std::unordered_map<std::string, Unit> made_units;
    temp_stats = {15,  4,  5,  7,  6,   8,  4,  6,  5, 5};
    Unit bandit_test("Bandit", "Bandit", temp_stats, "M");
    temp_stats = {48, 14, 25, 32, 34,  28, 19, 40, 15};
    bandit_test.setCaps(temp_stats);
    temp_stats = {60, 50, 20, 60, 70,  40, 30, 20,  10, 0};
    bandit_test.setGrowths(temp_stats);
    bandit_test.setExp(0);
  //temp_equipment[0].id = "Iron Axe";
  //temp_equipment[1].id = "Wooden Shield";
    bandit_test.setEquipment(temp_equipment);
    temp_equipped = {0, 1};
    bandit_test.setEquipped(temp_equipped);
    temp_position = {4, 5};
    bandit_test.setPos(temp_position);
    made_units["Bandit1test"] = bandit_test;
    return(made_units);
}

std::unordered_map<std::string, Unit> chap24Enemies() {
    Unit_stats temp_stats;
    Inventory_item temp_equipment[DEFAULT::EQUIPMENT_SIZE];
    Equipped temp_equipped;
    Point temp_position;
    std::unordered_map<std::string, Unit> made_units;
    temp_stats = {15,  4,  5,  7,  6,   8,  4,  6,  5, 5};
    Unit bandit_test("Bandit", "Bandit", temp_stats, "M");
    temp_stats = {48, 14, 25, 32, 34,  28, 19, 40, 15};
    bandit_test.setCaps(temp_stats);
    temp_stats = {60, 50, 20, 60, 70,  40, 30, 20,  10, 0};
    bandit_test.setGrowths(temp_stats);
    bandit_test.setExp(0);
  //temp_equipment[0].id = "Iron Axe";
  //temp_equipment[1].id = "Wooden Shield";
    bandit_test.setEquipment(temp_equipment);
    temp_equipped = {0, 1};
    bandit_test.setEquipped(temp_equipped);
    temp_position = {4, 5};
    bandit_test.setPos(temp_position);
    made_units["Bandit1test"] = bandit_test;
    return(made_units);
}

std::unordered_map<std::string, Unit> chap25Enemies() {
    Unit_stats temp_stats;
    Inventory_item temp_equipment[DEFAULT::EQUIPMENT_SIZE];
    Equipped temp_equipped;
    Point temp_position;
    std::unordered_map<std::string, Unit> made_units;
    temp_stats = {15,  4,  5,  7,  6,   8,  4,  6,  5, 5};
    Unit bandit_test("Bandit", "Bandit", temp_stats, "M");
    temp_stats = {48, 14, 25, 32, 34,  28, 19, 40, 15};
    bandit_test.setCaps(temp_stats);
    temp_stats = {60, 50, 20, 60, 70,  40, 30, 20,  10, 0};
    bandit_test.setGrowths(temp_stats);
    bandit_test.setExp(0);
  //temp_equipment[0].id = "Iron Axe";
  //temp_equipment[1].id = "Wooden Shield";
    bandit_test.setEquipment(temp_equipment); 
    temp_equipped = {0, 1};
    bandit_test.setEquipped(temp_equipped);
    temp_position = {4, 5};
    bandit_test.setPos(temp_position);
    made_units["Bandit1test"] = bandit_test;
    return(made_units);
}

std::unordered_map<std::string, Unit> (*chapEnemies[40])() = {chaptestEnemies, chap1Enemies, chap2Enemies, chap3Enemies, chap4Enemies, chap5Enemies, chap6Enemies, chap7Enemies, chap8Enemies, chap9Enemies, chap10Enemies, chap11Enemies, chap12Enemies, chap13Enemies, chap14Enemies, chap15Enemies, chap16Enemies, chap17Enemies, chap18Enemies, chap19Enemies, chap20Enemies, chap21Enemies, chap22Enemies, chap23Enemies, chap24Enemies, chap25Enemies};



