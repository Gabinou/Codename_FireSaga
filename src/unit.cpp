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
    autoSex_name();
}

Unit::Unit(const std::string in_name, const unsigned char in_class_index, const Unit_stats in_bases) : Unit(in_name, in_bases) {
    class_index = in_class_index;
    autoMvttype();
    autoClass_name();
    autoSkill_names();
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
    // This should not be in all unit objects. So much wasted space. Make it one shared callable function. Same for other heavy switches. 
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

void Unit::autoSkill_names() {
    skill_names.clear();
    if ((skills & UNIT::SKILL::CANTO) > 0) {
        skill_names.push_back("Canto");
    }
    if ((skills & UNIT::SKILL::SKILLED_RIDER) > 0) {
        skill_names.push_back("Canto");
    }    
    if ((skills & UNIT::SKILL::SPRINT) > 0) {
        skill_names.push_back("Sprint");
    } 
    if ((skills & UNIT::SKILL::SWITCH) > 0) {
        skill_names.push_back("Switch");
    }   
    if ((skills & UNIT::SKILL::MOUNTAINWALK) > 0) {
        skill_names.push_back("Mountainwalk");
    }    
    if ((skills & UNIT::SKILL::WATERWALK) > 0) {
        skill_names.push_back("Waterwalk");
    }
    if ((skills & UNIT::SKILL::CRIT_KILLS) > 0) {
        skill_names.push_back("CritKill");
    }
    if ((skills & UNIT::SKILL::DISMEMBER) > 0) {
        skill_names.push_back("Dismember");
    }
    if ((skills & UNIT::SKILL::ATK_RANGE_P1) > 0) {
        skill_names.push_back("Range+1");
    }
    if ((skills & UNIT::SKILL::DIVINE_SHIELD) > 0) {
        skill_names.push_back("Divine Shield");
    }
    if ((skills & UNIT::SKILL::NO_CRIT) > 0) {
        skill_names.push_back("No crit");
    }
    if ((skills & UNIT::SKILL::NO_COUNTER) > 0) {
        skill_names.push_back("No counter");
    }
    if ((skills & UNIT::SKILL::MAX_DESPAIR) > 0) {
        skill_names.push_back("Maxima of Despair");
    }
    if ((skills & UNIT::SKILL::TUNNELING) > 0) {
        skill_names.push_back("Tunneling");
    }
    if ((skills & UNIT::SKILL::SCOUTING) > 0) {
        skill_names.push_back("Scouting");
    }
    if ((skills & UNIT::SKILL::ASSASSINATE) > 0) {
        skill_names.push_back("Assassinate");
    }
    if ((skills & UNIT::SKILL::LOCKPICK) > 0) {
        skill_names.push_back("Lockpick");
    }
    if ((skills & UNIT::SKILL::NO_LOCKPICK) > 0) {
        skill_names.push_back("Pick");
    }
    if ((skills & UNIT::SKILL::IMMUNE_MAGIC) > 0) {
        skill_names.push_back("Immune to magic");
    }
    if ((skills & UNIT::SKILL::IMMUNE_ELEMENTAL) > 0) {
        skill_names.push_back("Immune to elemental");
    }
    if ((skills & UNIT::SKILL::IMMUNE_DEMONIC) > 0) {
        skill_names.push_back("Immune to demonic");
    }
    if ((skills & UNIT::SKILL::IMMUNE_ANGELIC) > 0) {
        skill_names.push_back("Immune to angelic");
    }
    if ((skills & UNIT::SKILL::LIFESTEAL_RN) > 0) {
        skill_names.push_back("Sol");
    }
    if ((skills & UNIT::SKILL::INFUSE) > 0) {
        skill_names.push_back("Infuse");
    }
    if ((skills & UNIT::SKILL::DOUBLE_EXP) > 0) {
        skill_names.push_back("Double EXP");
    }
    if ((skills & UNIT::SKILL::AMBIDEXTRY) > 0) {
        skill_names.push_back("Ambidextry");
    }
    if ((skills & UNIT::SKILL::TWO_HAND_STYLE) > 0) {
        skill_names.push_back("Two-hand Style");
    }
    if ((skills & UNIT::SKILL::PIERCE_RN) > 0) {
        skill_names.push_back("Luna");
    }
    if ((skills & UNIT::SKILL::COUNTER) > 0) {
        skill_names.push_back("Counter");
    }
    if ((skills & UNIT::SKILL::THRUST_SWORD_BONUS) > 0) {
        skill_names.push_back("Thrust sword bonus");
    }
    if ((skills & UNIT::SKILL::SHIELD_BONUS) > 0) {
        skill_names.push_back("Shield bonus");
    }
    if ((skills & UNIT::SKILL::OFFHAND_BONUS) > 0) {
        skill_names.push_back("Offhand bonus");
    }
    if ((skills & UNIT::SKILL::BOW_BONUS) > 0) {
        skill_names.push_back("Bow bonus");
    }
}

void Unit::autoSex_name() {
    if(sex) {
        sex_name = "M";
    } else {
        sex_name = "F";
    }
}


void Unit::autoClass_name() {
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
    SDL_Log("Computing unit range\n");
    static unsigned char range[2] = {0, 0};

    if (equipped.left > 0) {
        unsigned char * temp = all_weapons[equipment[equipped.left].id].getStats().range;
        range[0] = temp[0]; range[1] = temp[1];
    }

    if (equipped.right > 0) {
        unsigned char * temp = all_weapons[equipment[equipped.right].id].getStats().range;
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

void Unit::xmlreadequipment(tinyxml2::XMLElement * in_pEquipment) {
    tinyxml2::XMLElement * pItem = in_pEquipment->FirstChildElement("Item");
    tinyxml2::XMLElement * pId;
    tinyxml2::XMLElement * pUsed;
    int bufint;
    for (int i = 1; i < DEFAULT::EQUIPMENT_SIZE; i++) {
        pId = pItem->FirstChildElement("id");
        pUsed = pItem->FirstChildElement("Used");
        pId->QueryIntText(&bufint);
        equipment[i].id = bufint;
        pUsed->QueryIntText(&bufint);
        equipment[i].used = bufint;
        pItem->NextSiblingElement("Item");
    }
}

void Unit::xmlreadstats(tinyxml2::XMLElement * in_pStats, Unit_stats * in_stats) {
    tinyxml2::XMLElement * ptemp = in_pStats->FirstChildElement("hp");
    unsigned int bufint;
    ptemp->QueryUnsignedText(&bufint);
    in_stats->hp = (unsigned char)bufint;

    ptemp = in_pStats->FirstChildElement("str");
    ptemp->QueryUnsignedText(&bufint);
    in_stats->str = (unsigned char)bufint;

    ptemp = in_pStats->FirstChildElement("mag");
    ptemp->QueryUnsignedText(&bufint);
    in_stats->mag = (unsigned char)bufint;

    ptemp = in_pStats->FirstChildElement("agi");
    ptemp->QueryUnsignedText(&bufint);
    in_stats->agi = (unsigned char)bufint;

    ptemp = in_pStats->FirstChildElement("dex");
    ptemp->QueryUnsignedText(&bufint);
    in_stats->dex = (unsigned char)bufint;

    ptemp = in_pStats->FirstChildElement("luck");
    ptemp->QueryUnsignedText(&bufint);
    in_stats->luck = (unsigned char)bufint;

    ptemp = in_pStats->FirstChildElement("def");
    ptemp->QueryUnsignedText(&bufint);
    in_stats->def = (unsigned char)bufint;

    ptemp = in_pStats->FirstChildElement("res");
    ptemp->QueryUnsignedText(&bufint);
    in_stats->res = (unsigned char)bufint;

    ptemp = in_pStats->FirstChildElement("con");
    ptemp->QueryUnsignedText(&bufint);
    in_stats->con = (unsigned char)bufint;

    ptemp = in_pStats->FirstChildElement("move");
    ptemp->QueryUnsignedText(&bufint);
    in_stats->move = (unsigned char)bufint;

    ptemp = in_pStats->FirstChildElement("prof");
    ptemp->QueryUnsignedText(&bufint);
    in_stats->prof = (unsigned char)bufint;
} 

void Unit::xmlwritestats(tinyxml2::XMLDocument * in_doc, tinyxml2::XMLElement * in_pStats, Unit_stats * in_stats) {
    tinyxml2::XMLElement * php = in_doc->NewElement("hp");
    tinyxml2::XMLElement * pstr = in_doc->NewElement("str");
    tinyxml2::XMLElement * pmag = in_doc->NewElement("mag");
    tinyxml2::XMLElement * pagi = in_doc->NewElement("agi");
    tinyxml2::XMLElement * pdex = in_doc->NewElement("dex");
    tinyxml2::XMLElement * pluck = in_doc->NewElement("luck");
    tinyxml2::XMLElement * pdef = in_doc->NewElement("def");
    tinyxml2::XMLElement * pres = in_doc->NewElement("res");
    tinyxml2::XMLElement * pcon = in_doc->NewElement("con");
    tinyxml2::XMLElement * pmove = in_doc->NewElement("move");
    tinyxml2::XMLElement * pprof = in_doc->NewElement("prof");
    in_pStats->InsertEndChild(php);
    in_pStats->InsertEndChild(pstr);
    in_pStats->InsertEndChild(pmag);
    in_pStats->InsertEndChild(pagi);
    in_pStats->InsertEndChild(pdex);
    in_pStats->InsertEndChild(pluck);
    in_pStats->InsertEndChild(pdef);
    in_pStats->InsertEndChild(pres);
    in_pStats->InsertEndChild(pcon);
    in_pStats->InsertEndChild(pmove);
    in_pStats->InsertEndChild(pprof);
    php->SetText(in_stats->hp);
    pstr->SetText(in_stats->str);
    pmag->SetText(in_stats->mag);
    pagi->SetText(in_stats->agi);
    pdex->SetText(in_stats->dex);
    pluck->SetText(in_stats->luck);
    pdef->SetText(in_stats->def);
    pres->SetText(in_stats->res);
    pcon->SetText(in_stats->con);
    pmove->SetText(in_stats->move);
    pprof->SetText(in_stats->prof);
}

void Unit::readXML(const char * filename) {
    PHYSFS_file * fp;
    fp = PHYSFS_openRead(filename);
    unsigned int bufint;
    unsigned int filelen = PHYSFS_fileLength(fp);
    char filebuffer[filelen];
    const char * buffer;
    PHYSFS_readBytes(fp, filebuffer, filelen);
    PHYSFS_close(fp);
    tinyxml2::XMLDocument xmlDoc;
    if (xmlDoc.Parse(filebuffer, filelen) != 0) {
        SDL_Log("XML file parsing failed.");
    }
    tinyxml2::XMLElement * ptemp;
    tinyxml2::XMLElement * pUnit = xmlDoc.FirstChildElement("Unit");
    if (pUnit) {        
        ptemp = pUnit->FirstChildElement("Name");
        name = ptemp->GetText();
        ptemp = pUnit->FirstChildElement("Sex");
        ptemp->QueryBoolText(&sex);    
        ptemp = pUnit->FirstChildElement("SkillsCode");
        buffer = ptemp->GetText();
        // skills = strtoull(buffer, NULL, 16);
        sscanf(buffer, "%llx", &skills);
        ptemp = pUnit->FirstChildElement("BaseExp");
        ptemp->QueryUnsignedText(&bufint);
        base_exp = (unsigned short int)bufint;
        ptemp = pUnit->FirstChildElement("Exp");
        ptemp->QueryUnsignedText(&bufint);
        exp = (unsigned short int)bufint;
        ptemp = pUnit->FirstChildElement("Classid");
        ptemp->QueryUnsignedText(&bufint);
        class_index = (unsigned char)bufint;
        ptemp = pUnit->FirstChildElement("Stats");
        xmlreadstats(ptemp, &current_stats);
        ptemp = pUnit->FirstChildElement("Growths");
        xmlreadstats(ptemp, &growths);
        ptemp = pUnit->FirstChildElement("Caps");
        xmlreadstats(ptemp, &caps_stats);
        ptemp = pUnit->FirstChildElement("Bases");
        xmlreadstats(ptemp, &base_stats);
        ptemp = pUnit->FirstChildElement("Equipment");
        xmlreadequipment(ptemp);
        ptemp = pUnit->FirstChildElement("Level-ups");
    }
// 

    PHYSFS_close(fp);

} 

void Unit::writeXML(const char * filename, const bool append) {
    SDL_Log("Printing to XML using TinyXML2 and PhysFS\n");
    PHYSFS_file * fp;
    char buffer[DEFAULT::BUFFER_SIZE];
    if (append) {
        fp = PHYSFS_openAppend(filename);
    } else {
        fp = PHYSFS_openWrite(filename);
    }
    tinyxml2::XMLDocument xmlDoc;
    xmlDoc.InsertFirstChild(xmlDoc.NewDeclaration());
    
    tinyxml2::XMLElement * pUnit = xmlDoc.NewElement("Unit");
    xmlDoc.InsertEndChild(pUnit);
    
    tinyxml2::XMLElement * pName = xmlDoc.NewElement("Name");
    pUnit->InsertEndChild(pName);
    pName->SetText(name.c_str());
    
    // tinyxml2::XMLElement * pSexName = xmlDoc.NewElement("SexName");
    // pUnit->InsertEndChild(pSexName);
    // pSexName->SetText(sex_name.c_str());

    tinyxml2::XMLElement * pSex = xmlDoc.NewElement("Sex");
    pUnit->InsertEndChild(pSex);
    pSex->SetText(sex);
    
    // tinyxml2::XMLElement * pClass = xmlDoc.NewElement("Class");
    // pUnit->InsertEndChild(pClass);
    // pClass->SetText(class_name.c_str());
    
    tinyxml2::XMLElement * pClassid = xmlDoc.NewElement("Classid");
    pUnit->InsertEndChild(pClassid);
    pClassid->SetText(class_index);

    tinyxml2::XMLElement * pExp = xmlDoc.NewElement("Exp");
    pUnit->InsertEndChild(pExp);
    pExp->SetText(exp);

    tinyxml2::XMLElement * pBaseExp = xmlDoc.NewElement("BaseExp");
    pUnit->InsertEndChild(pBaseExp);
    pBaseExp->SetText(base_exp);
    
    tinyxml2::XMLElement * pStats = xmlDoc.NewElement("Stats");
    pUnit->InsertEndChild(pStats);
    xmlwritestats(&xmlDoc, pStats, &current_stats);
    
    tinyxml2::XMLElement * pGrowths = xmlDoc.NewElement("Growths");
    pUnit->InsertEndChild(pGrowths);
    xmlwritestats(&xmlDoc, pGrowths, &growths);
    
    tinyxml2::XMLElement * pCaps = xmlDoc.NewElement("Caps");
    pUnit->InsertEndChild(pCaps);
    xmlwritestats(&xmlDoc, pCaps, &caps_stats);
    
    tinyxml2::XMLElement * pBases = xmlDoc.NewElement("Bases");
    pUnit->InsertEndChild(pBases);
    xmlwritestats(&xmlDoc, pBases, &base_stats);
    if (grown_stats.size() > 0) {
        tinyxml2::XMLElement * pGrown = xmlDoc.NewElement("Level-ups");
        pUnit->InsertEndChild(pGrown);
        tinyxml2::XMLElement * pGrownLevel;
        for (int i = 0; i < grown_stats.size(); i++) {
            itoa(i, buffer, 10);
            pGrownLevel = xmlDoc.NewElement(buffer);
            pGrown->InsertEndChild(pGrownLevel);
            xmlwritestats(&xmlDoc, pGrownLevel, &base_stats);
        }
    }  
    if (skill_names.size() > 0) {
        tinyxml2::XMLElement * pSkills = xmlDoc.NewElement("Skills");
        tinyxml2::XMLElement * pSkill;
        for (int i = 0; i < skill_names.size(); i++) {
            pSkill = xmlDoc.NewElement("Skill");
            pSkills->InsertEndChild(pSkill);
            pSkill->SetText(skill_names[i].c_str());
        }
    }
    tinyxml2::XMLElement * pSkillsCode = xmlDoc.NewElement("SkillsCode");
    sprintf(buffer, "0x%llx", skills);
    pSkillsCode->SetText(buffer);
    pUnit->InsertEndChild(pSkillsCode);

    tinyxml2::XMLElement * pEquipment = xmlDoc.NewElement("Equipment");
    pUnit->InsertEndChild(pEquipment);
    tinyxml2::XMLElement * pItem;
    tinyxml2::XMLElement * pUsed;
    tinyxml2::XMLElement * pId;
    for (int i = 0; i < DEFAULT::EQUIPMENT_SIZE; i++) {
        pItem = xmlDoc.NewElement("Item");
        pEquipment->InsertEndChild(pItem);
        pId = xmlDoc.NewElement("id");
        pUsed = xmlDoc.NewElement("Used");
        sprintf(buffer, "%d", equipment[i].id);
        pId->SetText(buffer);
        sprintf(buffer, "%d", equipment[i].used);
        pUsed->SetText(buffer);
        pItem->InsertEndChild(pId);
        pItem->InsertEndChild(pUsed);
    }
    
    tinyxml2::XMLPrinter printer;

    xmlDoc.Print(&printer);
    char longbuffer[printer.CStrSize()];
    sprintf(longbuffer, printer.CStr());
    
    PHYSFS_writeBytes(fp, longbuffer, printer.CStrSize());

    PHYSFS_close(fp);
}
void Unit::writeFS(const char * filename, const bool append) {
    // Maybe this function should write constant number of bytes per line...
    // Easier to read.
    PHYSFS_file * fp;
    if (append) {
        fp = PHYSFS_openWrite(filename);
    } else {
        fp = PHYSFS_openAppend(filename);
    }
    size_t buflen;
    char varbuf[DEFAULT::BUFFER_SIZE];
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
    // -> NOT easy to parse...
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
    temp_unit.writeXML("unit_test.xml");
    printf("Made units.\n");
    all_units[UNIT::NAME::ERWIN] = temp_unit;

    temp_unit = Unit();
    temp_unit.readXML("unit_test.xml");
    temp_unit.writeXML("unit_rewrite.xml");

    
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
