#include "unit.hpp"

Unit::Unit() {

}

Unit::Unit(const std::string in_name, const std::string in_class, const Unit_stats in_bases) {
    base_stats = in_bases;
    current_stats = in_bases;
    class_name = in_class;
    setMvttype(in_class);
    name = in_name;
}

Unit::Unit(const std::string in_name, const Unit_stats in_bases) {
    base_stats = in_bases;
    current_stats = in_bases;
    name = in_name;
}

Unit::Unit(const Unit_stats in_bases) {
    base_stats = in_bases;
    current_stats = in_bases;
}

void Unit::setMvttype(const std::string in_class) {
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

void Unit::setPos(const int in_pos[2]) {
    position[0] = in_pos[0];
    position[1] = in_pos[1];
}

int * Unit::getPos() {
    return(position);
}

void Unit::equipsL(const char index) {
    equipped.left = index;
}

void Unit::unequipsL() {
    equipped.left = -1;
}

void Unit::equipsR(const char index) {
    equipped.right = index;
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

void Unit::takesDamage(const unsigned char damage) {
    printf("%s takes %d damage \n", name, damage);
    current_hp = std::max(0, current_hp - damage);

    if (current_hp == 0) {dies();};
}

std::string Unit::getMvttype() {
    printf("insinde unitcomponent%s\n", mvt_type.c_str());
    return (mvt_type);
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
    unsigned char * temp;
    static unsigned char range[2];
    unsigned char * right_range;

    if (equipped.left > 0) {
        temp = all_weapons[equipsment[equipped.left].name].getStats().range;
        range[0] = temp[0]; range[1] = temp[1];
    } else {
        range[0] = 1;
        range[1] = 1;
    }

    if (equipped.right > 0) {
        temp = all_weapons[equipsment[equipped.left].name].getStats().range;
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

void Unit::setExp(const unsigned int in_exp) {
    exp = in_exp;
}

void Unit::setHp(const unsigned char in_hp) {
    current_hp = in_hp;
}

void Unit::dies() {
    printf("%s is dead.\n", name);
}

void Unit::setBonus(const Unit_stats in_stats) {
    bonus_stats = in_stats; // tested, works fine.
}

void Unit::setMalus(const Unit_stats in_stats) {
    malus_stats = in_stats; // tested, works fine.
}

void Unit::setCaps(const Unit_stats in_stats) {
    caps_stats = in_stats; // tested, works fine.
}

void Unit::setStats(const Unit_stats in_stats) {
    current_stats = in_stats; // tested, works fine.
    current_hp = current_stats.hp;
}

Unit_stats Unit::getStats() {
    return (base_stats);
}

void Unit::setBases(const Unit_stats in_stats) {
    base_stats = in_stats; // tested, works fine.
    current_stats = in_stats; // tested, works fine.
    current_hp = base_stats.hp;
}

void Unit::setGrowths(const Unit_stats in_growths) {
    growths = in_growths; // tested, works fine.
}

unsigned char Unit::attack_damage() {
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

unsigned char Unit::combat_damage(const bool critical) {
    unsigned char terrain_def = 0;
    unsigned char enemy_def = 0 ;
    unsigned char unit_power = 0;
    unsigned char wpn_dmg;
    unsigned char crit_factor = 1;

    if (temp_wpn.dmg_type == 0) {
        // Physical attack_damage.
        wpn_dmg = temp_wpn.Pmight;
        unit_power = current_stats.str;
        // enemy_def = enemy.getComponent<UnitComponent>().current_stats.def;
    } else {
        // Magical attack_damage.
        wpn_dmg = temp_wpn.Mmight;
        unit_power = current_stats.mag;
        // enemy_def = enemy.getComponent<UnitComponent>().current_stats.res;
    };

    if (critical) {crit_factor = 3;};

    unsigned char attack_damage = crit_factor * (std::max(wpn_dmg + unit_power - enemy_def - terrain_def, 0)); // Modern FE style. for crit_factor = 3

    // int attack_damage = crit_factor*(wpn_dmg + unit_power) - enemy_def - terrain_def);  // FE4-FE5 style. for crit_factor = 2

    return (attack_damage);
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

bool Unit::canRetaliate(Unit * enemy) {
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

bool Unit::canDouble() const {
    // unsigned char enemy_speed = enemy.getComponent<UnitComponent>().current_stats.agi;
    // bool doubles = ((current_stats.agi - wpn_weighed_down() - enemy_speed) > 4);
    // return (doubles);
    return (true);
}

unsigned char Unit::wpn_weighed_down() const {
    //*DESIGN QUESTION* What should be the influence of weapons?
    // Average of Con and Str? Con+Str/2?
    return (std::max(temp_wpn.wgt - current_stats.con, 0));
}

unsigned char Unit::combat_critical() {
    // unsigned char supports = 0;
    // unsigned char unit_skill = 0;
    // unsigned char enemy_favor = enemy.getComponent<UnitComponent>().favor();
    // unsigned char critical = std::max(0, temp_wpn.fights.crit + unit_skill + supports - enemy_favor);
    // return (critical);
    return (1);
}

unsigned char Unit::favor() {
    unsigned char supports = 0 ;
    unsigned char favor = (ceil(current_stats.luck / 2.)) + supports;
    return (favor);
}

unsigned char Unit::accuracy() {
    //*DESIGN QUESTION* In vesteria saga, dex*3.
    unsigned char supports = 0;
    unsigned char unit_acc = current_stats.dex * 3 + current_stats.luck;
    unsigned char accuracy = temp_wpn.combat.hit + unit_acc + supports;
    return (accuracy);
}

unsigned char Unit::combat_hit() {
    // int enemy_dodge = enemy.getComponent<UnitComponent>().dodge();
    // unsigned char hit = std::max(0, accuracy() - enemy_dodge);
    // return (hit);
    return (1);
}

unsigned char Unit::attacks() {
    // printf("%s attacks %s\n", name, enemy.getComponent<UnitComponent>().getName());
    // bool unit_hits = (getRN() < combat_hit(enemy));
    // bool unit_crits = (getRN() < combat_critical(enemy));
    //  *DESIGN QUESTION* Should a random number always be rolled for crits, even if the hit doesn't connect?
    // * I think so. Always same number of RN rolled.
    // * But what about crit animations? Should crit animations be shown to miss? Fire Emblem thinks not. Me too.
    
    // // unit.takesDamage( -= combat_damage(enemy, unit_crits);
    // enemy.getComponent<UnitComponent>().takesDamage(combat_damage(enemy, unit_crits));
    // return (combat_damage(enemy, unit_crits));
    return (1);
}

void Unit::fights(Unit * enemy) {
    // printf("%s fights %s\n", name, enemy.getComponent<UnitComponent>().getName());
    // bool unit_doubles = canDouble(enemy);
    // bool enemy_retaliates = enemy.getComponent<UnitComponent>().canRetaliate(enemy);
    // bool enemy_doubles = 0;
    // attack(enemy);

    // if (enemy_retaliates) {
    //     enemy.getComponent<UnitComponent>().attack(*entity);
    //     printf("enemy %s retaliates %d\n", enemy.getComponent<UnitComponent>().getName(), enemy_retaliates);
    //     enemy_doubles = enemy.getComponent<UnitComponent>().canDouble(*entity);
    // };

    // if (unit_doubles) {
    //     printf("%s doubles\n", name);
    //     attack(enemy);
    // };

    // if (enemy_doubles) {
    //     printf("%s doubles\n", enemy.getComponent<UnitComponent>().getName());
    //     enemy.getComponent<UnitComponent>().attack(*entity);
    // };
}

void Unit::write(const char * filename) {
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

std::unordered_map<std::string, Unit> all_units;

void baseUnits() {
    printf("Making base units \n");
    Unit_stats temp;
    //hp,str,mag,skl,spd,luck,def,res,con,move
    temp = {17,  6,  2,  7,  7,   7,  4,  5,  6, 5};
    Unit main("Main", "Mercenary", temp);
    temp = {48, 14, 25, 32, 34,  28, 19, 40, 15, 0};
    main.setCaps(temp);
    temp = {60, 50, 20, 60, 70,  40, 30, 20,  10, 0};
    main.setGrowths(temp);
    main.setExp(0);
    main.write("unit_test.txt");
    all_units["Main"] = main;
    
    temp = {18,  6,  2,  7,  7,   7,  4,  5,  6, 7};
    Unit reliable("Reliable", "Cavalier", temp);
    temp = {48, 14, 25, 32, 34,  28, 19, 40, 15};
    reliable.setCaps(temp);
    temp = {60, 50, 20, 60, 70,  40, 30, 20,  10, 0};
    reliable.setGrowths(temp);
    reliable.setExp(100);
    all_units["Reliable"] = reliable;

    
    temp = {19,  6,  2,  7,  7,   7,  4,  5,  6,  7};
    Unit coward("Coward", "Cavalier", temp);
    temp = {48, 14, 25, 32, 34,  28, 19, 40, 15};
    coward.setCaps(temp);
    temp = {60, 50, 20, 60, 70,  40, 30, 20,  10, 0};
    coward.setGrowths(temp);
    coward.setExp(200);
    all_units["Coward"] = coward;

    
    temp = {20,  6,  2,  7,  7,   7,  4,  5,  6,  6};
    Unit hjaigen("1H Jaigen", "Fencermaster", temp);
    temp = {48, 14, 25, 32, 34,  28, 19, 40, 15};
    hjaigen.setCaps(temp);
    temp = {60, 50, 20, 60, 70,  40, 30, 20,  10, 0};
    hjaigen.setGrowths(temp);
    hjaigen.setExp(2200);
    all_units["1H Jaigen"] = hjaigen;

    
    temp = {14,  6,  2,  7,  7,   7,  4,  5,  6,  5};
    Unit lovely("Lovely", "Priestess", temp);
    temp = {48, 14, 25, 32, 34,  28, 19, 40, 15};
    lovely.setCaps(temp);
    temp = {60, 50, 20, 60, 70,  40, 30, 20,  10, 0};
    lovely.setGrowths(temp);
    lovely.setExp(100);
    all_units["Lovely"] = lovely;

    
    temp = {16,  6,  2,  7,  7,   7,  4,  5,  6,  6};
    Unit hottie("Hottie", "Thief", temp);
    temp = {48, 14, 25, 32, 34,  28, 19, 40, 15};
    hottie.setCaps(temp);
    temp = {60, 50, 20, 60, 70,  40, 30, 20,  10, 0};
    hottie.setGrowths(temp);
    hottie.setExp(2200);
    all_units["Hottie"] = hottie;

    
    temp = {22,  4,  5,  7,  6,   8,  4,  6,  5, 5}; // 4 or 5?
    Unit servil("Servil", "Knight", temp);
    temp = {48, 14, 25, 32, 34,  28, 19, 40, 15};
    servil.setCaps(temp);
    temp = {60, 50, 20, 60, 70,  40, 30, 20,  10, 0};
    servil.setGrowths(temp);
    servil.setExp(500);
    all_units["Servil"] = servil;

    
    temp = {34,  4,  5,  7,  6,   8,  4,  6,  5, 5};
    Unit perignon("Pérignon", "Mage", temp);
    temp = {48, 14, 25, 32, 34,  28, 19, 40, 15};
    perignon.setCaps(temp);
    temp = {60, 50, 20, 60, 70,  40, 30, 20,  10, 0};
    perignon.setGrowths(temp);
    perignon.setExp(1200);
    all_units["Pérignon"] = perignon;

    
    temp = {15,  4,  5,  7,  6,   8,  4,  6,  5, 5};
    Unit poet("Poet", "Mage", temp);
    temp = {48, 14, 25, 32, 34,  28, 19, 40, 15};
    poet.setCaps(temp);
    temp = {60, 50, 20, 60, 70,  40, 30, 20,  10, 0};
    poet.setGrowths(temp);
    poet.setExp(400);
    all_units["Poet"] = poet;

    
    temp = {15,  4,  5,  7,  6,   8,  4,  6,  5, 5};
    Unit silou("Silou", "Mage", temp);
    temp = {48, 14, 25, 32, 34,  28, 19, 40, 15};
    silou.setCaps(temp);
    temp = {60, 50, 20, 60, 70,  40, 30, 20,  10, 0};
    silou.setGrowths(temp);
    silou.setExp(400);
    all_units["Silou"] = silou;

}
