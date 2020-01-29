#include "unit.hpp"

Unit::Unit() {

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

Unit::Unit(const Unit_stats in_bases) {
    base_stats = in_bases;
    current_stats = in_bases;
}

int * Unit::getPos() {
    return(position);
}

void Unit::setPos(const int in_pos[2]) {
    position[0] = in_pos[0];
    position[1] = in_pos[1];
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

std::vector<std::string> Unit::getEquippables() {
    return(equippables);
}

void Unit::setEquippable() {
    equippables.clear();
    if ((class_name == "Mercenary") || (class_name == "Lord") || (class_name == "Duelist") || (class_name == "Thief")) {
        equippables.push_back("shield");
        equippables.push_back("sword");
        equippables.push_back("offhand");
    }
    if ((class_name == "Pegasus knight") || (class_name == "Trooper") || (class_name == "Cavalier") || (class_name == "Knight")) {
        equippables.push_back("lance");
        equippables.push_back("shield");
    }
    if (class_name == "Bandit") {
        equippables.push_back("shield");
        equippables.push_back("axe");
    }
    if ((class_name == "Corsair") || (class_name == "Viking")) {
        equippables.push_back("shield");
        equippables.push_back("axe");
        equippables.push_back("offhand");
    }
    if ((class_name == "Fencer") || (class_name == "Pickpocket")){
        equippables.push_back("offhand");
        equippables.push_back("sword");
    }
    if ((class_name == "Mousquetaire") || (class_name == "Assassin") || (class_name == "Marksman")) {
        equippables.push_back("sword");
        equippables.push_back("offhand");
        equippables.push_back("bow");
    }
    if (class_name == "Archer") {
        equippables.push_back("bow");
    }
    if ((class_name == "Duke") || (class_name == "Paladin")) {
        equippables.push_back("sword");
        equippables.push_back("lance");
        equippables.push_back("shield");
        equippables.push_back("offhand");
    }
    if (class_name == "General") {
        equippables.push_back("sword");
        equippables.push_back("shield");
        equippables.push_back("axe");
        equippables.push_back("lance");
    }
    if ((class_name == "Cleric") || (class_name == "Priest")) {
        equippables.push_back("staff");
    }
    if (class_name == "Mage") {
        equippables.push_back("elemental");
        equippables.push_back("trinket");
    }
    if (class_name == "Hero") {
        equippables.push_back("axe");
        equippables.push_back("sword");
        equippables.push_back("shield");
        equippables.push_back("offhand");
    }
    if (class_name == "Ravager") {
        equippables.push_back("axe");
        equippables.push_back("shield");
        equippables.push_back("offhand");
        equippables.push_back("bow");
    }
    if (class_name == "Battlemage") {
        equippables.push_back("elemental");
        equippables.push_back("shield");
        equippables.push_back("sword");
        equippables.push_back("trinket");
    }
    if ((class_name == "Sage") || (class_name == "Troubadour")) {
        equippables.push_back("elemental");
        equippables.push_back("staff");
        equippables.push_back("trinket");
    }
    if ((class_name == "Bishop") || (class_name == "Oracle")) {
        equippables.push_back("staff");
        equippables.push_back("angelic");
        equippables.push_back("trinket");
    }
    if (class_name == "Angel") {
        equippables.push_back("angelic");
        equippables.push_back("sword");
        equippables.push_back("lance");
        equippables.push_back("shield");
    }
    if (class_name == "Demon") {
        equippables.push_back("demonic");
        equippables.push_back("claw");
        equippables.push_back("axe");
    }
    if (class_name == "Demonic Incarnate") {
        equippables.push_back("demonic");
        equippables.push_back("trinket");
    }
    if (class_name == "Angelic Incarnate") {
        equippables.push_back("angelic");
        equippables.push_back("trinket");
    }
}


std::string Unit::getMvttype() {
    printf("inside unit %s\n", mvt_type.c_str());
    return (mvt_type);
}

void Unit::setMvttype() {
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

void Unit::equips(const unsigned int index, const std::string hand) {
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

void Unit::takeItem(Inventory_item * out_array, int in_index, int out_index) {
    equipment[in_index] = out_array[out_index];
    Inventory_item empty;
    out_array[out_index] = empty;
}

void Unit::giveItem(Inventory_item * out_array, int in_index, int out_index) {
    out_array[out_index] = equipment[in_index];
    Inventory_item empty;
    equipment[in_index] = empty;
}

void Unit::dropItem(int in_index) {
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
    current_hp = std::min(current_hp + healing, (int) current_stats.hp);
}

unsigned char Unit::getHp() const {
    return (current_hp);
}

unsigned int Unit::getLvl() const {
    return (ceil(current_hp / 100));
}

unsigned int Unit::getExp() const {
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

void Unit::setEntity(int in_index) {
    entity = in_index;
}

int Unit::getEntity() {
    return(entity);
}

void Unit::setExp(const unsigned int in_exp) {
    exp = in_exp;
}

void Unit::addExp(const unsigned int in_exp) {
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
    std::string wpn_type = all_weapons[in_name].getType();
    bool equippable = false;
    for (int i = 0; i < equippables.size(); i++) {
        if (equippables[i] == wpn_type) {
            equippable = true;
        }
    }
    return(equippable);
} 

bool Unit::canAttack() {
    bool out;
    struct Wpn_types {
        std::string left;
        std::string right;
    } wpn_types;
    if (equipped.left > 0) {
        wpn_types.left = all_weapons[equipment[equipped.left].name].getType();
        if ((wpn_types.left != "shield")  & (wpn_types.left != "trinket") & (wpn_types.left != "staff")) {
            out = true;
        } 
    }
    if (equipped.right > 0) {
        wpn_types.right = all_weapons[equipment[equipped.right].name].getType();
        if ((wpn_types.right != "shield")  & (wpn_types.right != "trinket") & (wpn_types.right != "staff")) {
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

    int total_might = wpn_dmg + unit_power;

    return (total_might);
}

unsigned char Unit::totalDef(bool dmg_type){
    unsigned char total_def = 0;
    if (dmg_type){
        total_def += current_stats.res;
        total_def += all_weapons[equipment[equipped.left].name].getBonus().res;
        total_def += all_weapons[equipment[equipped.right].name].getBonus().res;
        if (all_weapons[equipment[equipped.right].name].getType() == "shield") {
            total_def += all_weapons[equipment[equipped.right].name].getStats().Mmight;
        }
        if (all_weapons[equipment[equipped.left].name].getType() == "shield") {
            total_def += all_weapons[equipment[equipped.left].name].getStats().Mmight;
        }
    } else {
        total_def += current_stats.def;
        total_def += all_weapons[equipment[equipped.left].name].getBonus().def;
        total_def += all_weapons[equipment[equipped.right].name].getBonus().def;
        if (all_weapons[equipment[equipped.right].name].getType() == "shield") {
            total_def += all_weapons[equipment[equipped.right].name].getStats().Pmight;
        }
        if (all_weapons[equipment[equipped.left].name].getType() == "shield") {
            total_def += all_weapons[equipment[equipped.left].name].getStats().Pmight;
        }
    }
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
    int * unit_position;
    int * enemy_position;
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
    fclose(fp);
}

std::unordered_map<std::string, Unit> all_units;

void baseUnits() {
    printf("Making base units \n");
    Unit_stats temp;
    //hp,str,mag,skl,spd,luck,def,res,con,move
    temp = {17,  6,  2,  7,  7,   7,  4,  5,  6, 5};
    Unit main("Main", "Mercenary", temp, "M");
    temp = {48, 14, 25, 32, 34,  28, 19, 40, 15, 0};
    main.setCaps(temp);
    temp = {60, 50, 20, 60, 70,  40, 30, 20,  10, 0};
    main.setGrowths(temp);
    main.setExp(0);
    main.write("unit_test.txt");
    all_units["Main"] = main;
    
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
    Unit lovely("Lovely", "Priestess", temp, "F");
    temp = {48, 14, 25, 32, 34,  28, 19, 40, 15};
    lovely.setCaps(temp);
    temp = {60, 50, 20, 60, 70,  40, 30, 20,  10, 0};
    lovely.setGrowths(temp);
    lovely.setExp(100);
    all_units["Lovely"] = lovely;
    
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
