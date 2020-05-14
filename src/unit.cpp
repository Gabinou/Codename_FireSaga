#include "unit.hpp"
#ifndef STB_SPRINTF_IMPLEMENTATION
#define STB_SPRINTF_IMPLEMENTATION
#include "stb_sprintf.h"
#endif /* STB_SPRINTF_IMPLEMENTATION */

Unit::Unit() {
    setXMLElement("Unit");
    equipsR(0);
    equipsL(1);
}

Unit::Unit(const unsigned short int in_id, const unsigned char in_class_index,  const Unit_stats in_bases, const bool in_sex) : Unit() {
    setBases(in_bases);
    setStats(in_bases);
    setid(in_id);
    setClassind(in_class_index);
    setSex(in_sex);
}

Unit::Unit(const Unit & obj)  {
    setSex(obj.sex);
    setid(obj.id);
    setBases(obj.base_stats);
    setStats(obj.current_stats);
    setGrowths(obj.growths);
    setCaps(obj.caps_stats);
    setClassind(obj.class_index);
    setEquipment(obj.equipment);
    setSkills(obj.skills);
    setArmy(obj.army);
    setSupports(obj.supports);
    exp = obj.exp;
    base_exp = obj.base_exp;
}

bool Unit::getSex() {
    return (sex);
}

void Unit::setSex(const bool in_sex) {
    sex = in_sex;
    sex_name = sexNames[sex];
}

void Unit::setid(const unsigned short int in_id) {
    id = in_id;
    name = unitNames[in_id];
    // setArmy(unitid2army(id));
}

unsigned short int Unit::getid() {
    return (id);
}

void Unit::use(int in_ind) {

}

unsigned long long int Unit::getSkills() {
    return (skills);
}

void Unit::setSkills(unsigned long long int in_skills) {
    skills = in_skills;
    skill_names = skillNames(skills);
}


void Unit::removeEquipment(unsigned char in_index) {
    Inventory_item empty;
    equipment[in_index] = empty;
}

void Unit::addEquipment(Inventory_item in_item) {
    for (short unsigned int i = 0; i < DEFAULT::EQUIPMENT_SIZE; i++) {
        if (equipment[i].id == -1) {
            SDL_Log("Equipped");
            equipment[i] = in_item;
            break;
        }
    }
}

void Unit::setEquipment(const Inventory_item in_equipment[DEFAULT::EQUIPMENT_SIZE]) {
    for (short unsigned int i = 0; i < DEFAULT::EQUIPMENT_SIZE; i++) {
        equipment[i] = in_equipment[i];
    }
}

void Unit::setEquipment(std::vector<Inventory_item> in_equipment) {
    if (in_equipment.size() <= DEFAULT::EQUIPMENT_SIZE) {
        for (short unsigned int i = 0; i < in_equipment.size(); i++) {
            equipment[i] = in_equipment[i];
        }
    } else {
        SDL_Log("Unit in_equipment is too large.");
    }
}

std::vector<Inventory_item> Unit::getEquipment() {
    std::vector<Inventory_item> out;

    for (short unsigned int i = 0; i < DEFAULT::EQUIPMENT_SIZE; i++) {
        if (equipment[i].id > 0) {
            out.push_back(equipment[i]);
        }
    }

    return (out);
}


void Unit::setEquipped(Equipped in_equipped) {
    equipped = in_equipped;
}

short unsigned int Unit::getEquippable() {
    return (equippable);
}

unsigned char Unit::getMvttype() {
    return (mvt_type);
}

unsigned char Unit::getClassind() {
    return (class_index);
}

void Unit::setClassind(unsigned char in_class_index) {
    class_index = in_class_index;
    mvt_type = mvtTypes[class_index];
    class_name = classNames[class_index];
    equippable = equippableCodes[class_index];
}

bool Unit::isDanger() {
    return (show_danger);
}

void Unit::showDanger() {
    show_danger = true;
}

void Unit::hideDanger() {
    show_danger = false;
}

void Unit::supportUp(short int in_id) {
    int i = 0;

    while ((supports[i].index = ! in_id) && (i < DEFAULT::SUPPORTS_MAX)) {
        i++;
    }

    if (i == DEFAULT::SUPPORTS_MAX) {
        SDL_Log("Could not find support of index %d", in_id);
    }

    supports[i].level += 1;
}

void Unit::setSupports(const Support in_supports[DEFAULT::SUPPORTS_MAX]) {
    for (int i = 0; i < DEFAULT::SUPPORTS_MAX; i++) {
        supports[i].index = in_supports[i].index;
        supports[i].level = in_supports[i].level;
    }
}

void Unit::setSupports(std::vector<short int> in_supports) {
    for (int i = 0; i < in_supports.size(); i++) {
        supports[i].index = in_supports[i];
        supports[i].level = 0;
    }
}

void Unit::equipsL(const unsigned char index) {
    if (equipment[index].id > 0) {
        equipped.left = index;
    }
}

void Unit::equipsR(const unsigned char index) {
    if (equipment[index].id > 0) {
        equipped.right = index;
    }
}

void Unit::unequipsL() {
    equipped.left = -1;
}

void Unit::unequipsR() {
    equipped.right = -1;
}

void Unit::equips(const short unsigned int index, const bool hand) {
    if (equipment[index].id > 0) {
        if (hand) {
            equipped.left = index;
        } else {
            equipped.right = index;
        }
    }
}

void Unit::unequips(const bool hand) {
    if (hand) {
        equipped.left = -1;
    } else {
        equipped.right = -1;
    }
}

void Unit::takeItem(Inventory_item * out_array, short int in_index,  short int out_index) {
    equipment[in_index] = out_array[out_index];
    Inventory_item empty;
    out_array[out_index] = empty;
}

void Unit::giveItem(Inventory_item * out_array, short int in_index,  short int out_index) {
    out_array[out_index] = equipment[in_index];
    Inventory_item empty;
    equipment[in_index] = empty;
}

void Unit::dropItem(short int in_index) {
    Inventory_item empty;
    equipment[in_index] = empty;
}

void Unit::takesDamage(const unsigned char damage) {
    SDL_Log("%s takes %d damage \n", name, damage);
    current_hp = std::max(0, current_hp - damage);

    if (current_hp == 0) {dies();};
}

void Unit::getsHealed(const unsigned char healing) {
    SDL_Log("%s gets healed for %d\n", name, healing);
    current_hp = std::min((short int)(current_hp + healing), (short int) current_stats.hp);
}

bool Unit::isWaiting() {
    return (waits);
}

void Unit::wait() {
    waits = true;
}

void Unit::refresh() {
    waits = false;
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

    if (equipped.left >= 0) {
        if (equipment[equipped.left].id > 0) {
            unsigned char * temp = all_weapons[equipment[equipped.left].id].getStats().range;
            range[0] = temp[0];
            range[1] = temp[1];
        }
    }

    if (equipped.right >= 0) {
        if (equipment[equipped.right].id > 0) {
            unsigned char * temp = all_weapons[equipment[equipped.right].id].getStats().range;
            range[0] = std::min(temp[0], range[0]);
            range[1] = std::max(temp[1], range[1]);
        }
    }

    if ((equipped.left < 0) && (equipped.right < 0)) {
        range[0] = 0;
        range[1] = 0;
    }

    return (range);
}

void Unit::setEntity(short int in_index) {
    entity = in_index;
}

int Unit::getEntity() {
    return (entity);
}

void Unit::setBaseExp(const short unsigned int in_exp) {
    exp = in_exp;
    base_exp = in_exp;
}

void Unit::gainExp(const short unsigned int in_exp) {
    exp += in_exp;

    if (((exp % 100) + in_exp) > 100) {
        // Never should have two level ups at one time.
        levelUp();
    }
}

void Unit::levelUp() {
    unsigned char prob;
    Unit_stats temp_stats = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};

    temp_stats.hp += (growths.hp / 100);
    temp_stats.str += (growths.str / 100);
    temp_stats.mag += (growths.mag / 100);
    temp_stats.dex += (growths.dex / 100);
    temp_stats.agi += (growths.agi / 100);
    temp_stats.luck += (growths.luck / 100);
    temp_stats.def += (growths.def / 100);
    temp_stats.res += (growths.res / 100);
    temp_stats.move += (growths.move / 100);
    temp_stats.prof += (growths.prof / 100);
    temp_stats.con += (growths.con / 100);

    prob = getURN();

    if ((prob <= (growths.hp % 100)) && (current_stats.hp < caps_stats.hp)) {
        temp_stats.hp += 1;
    }

    prob = getURN();

    if ((prob <= (growths.str % 100)) && (current_stats.str < caps_stats.str)) {
        temp_stats.str += 1;
    }

    prob = getURN();

    if ((prob <= (growths.mag % 100)) && (current_stats.mag < caps_stats.mag)) {
        temp_stats.mag += 1;
    }

    prob = getURN();

    if ((prob <= (growths.dex % 100)) && (current_stats.dex < caps_stats.dex)) {
        temp_stats.dex += 1;
    }

    prob = getURN();

    if ((prob <= (growths.agi % 100)) && (current_stats.agi < caps_stats.agi)) {
        temp_stats.agi += 1;
    }

    prob = getURN();

    if ((prob <= (growths.luck % 100)) && (current_stats.luck < caps_stats.luck)) {
        temp_stats.luck += 1;
    }

    prob = getURN();

    if ((prob <= (growths.def % 100)) && (current_stats.def < caps_stats.def)) {
        temp_stats.def += 1;
    }

    prob = getURN();

    if ((prob <= (growths.res % 100)) && (current_stats.res < caps_stats.res)) {
        temp_stats.res += 1;
    }

    prob = getURN();

    if ((prob <= (growths.con % 100)) && (current_stats.con < caps_stats.con)) {
        temp_stats.con += 1;
    }

    prob = getURN();

    if ((prob <= (growths.prof % 100)) && (current_stats.prof < caps_stats.prof)) {
        temp_stats.prof += 1;
    }

    prob = getURN();

    if ((prob <= (growths.move % 100)) && (current_stats.move < caps_stats.move)) {
        temp_stats.move += 1;
    }

    grown_stats.push_back(temp_stats);

    current_stats.hp += temp_stats.hp;
    current_stats.str += temp_stats.str;
    current_stats.mag += temp_stats.mag;
    current_stats.dex += temp_stats.dex;
    current_stats.agi += temp_stats.agi;
    current_stats.luck += temp_stats.luck;
    current_stats.def += temp_stats.def;
    current_stats.res += temp_stats.res;
    current_stats.move += temp_stats.move;
    current_stats.con += temp_stats.con;
    current_stats.prof += temp_stats.prof;
}

void Unit::setHp(const unsigned char in_hp) {
    current_hp = in_hp;
}

void Unit::dies() {
    SDL_Log("%s is dead.\n", name.c_str());
}

void Unit::setBonus(const Unit_stats in_stats) {
    bonus_stats = in_stats;
}

Unit_stats Unit::getBonus() {
    return (bonus_stats);
}

void Unit::setMalus(const Unit_stats in_stats) {
    malus_stats = in_stats;
}

Unit_stats Unit::getMalus() {
    return (malus_stats);
}

void Unit::setCaps(const Unit_stats in_stats) {
    caps_stats = in_stats;
}

Unit_stats Unit::getCaps() {
    return (caps_stats);
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
    return (base_stats);
}

void Unit::setGrowths(const Unit_stats in_growths) {
    growths = in_growths;
}

Unit_stats Unit::getGrowths() {
    return (growths);
}

bool Unit::canEquip(unsigned short int in_id) {
    short unsigned int wpntypecode = all_weapons[in_id].getType();
    return (((equippable & wpntypecode) > 0));
}

bool Unit::canAttack() {
    // THIS FUNCTION needs to be updated with the fact that some weapons can have multiple types. -> typecodes
    bool out;
    struct wpntypecodes {
        unsigned short int left;
        unsigned short int right;
    } wpntypecodes;

    if (equipped.left > 0) {
        wpntypecodes.left = all_weapons[equipment[equipped.left].id].getType();

        if ((wpntypecodes.left != ITEM::TYPE::SHIELD)  & (wpntypecodes.left != ITEM::TYPE::TRINKET) & (wpntypecodes.left != ITEM::TYPE::STAFF)) {
            out = true;
        }
    }

    if (equipped.right > 0) {
        wpntypecodes.right = all_weapons[equipment[equipped.right].id].getType();

        if ((wpntypecodes.right != ITEM::TYPE::SHIELD)  & (wpntypecodes.right != ITEM::TYPE::TRINKET) & (wpntypecodes.right != ITEM::TYPE::STAFF)) {
            out = true;
        }
    }

    return (out);
}

// bool Unit::dmgType() { //Useless?
//     // Assumes canAttack().
//     struct Dmg_types{
//         bool left = false;
//         bool right = false;
//     } dmg_types;
//     struct wpntypes{
//         std::string left;
//         std::string right;
//     } wpntypes;
//     bool out = false;

//     wpntypes.left = all_weapons[equipment[equipped.left].name].getType();
//     wpntypes.right = all_weapons[equipment[equipped.right].name].getType();


//     if ((wpntypes.right.empty()) & (wpntypes.right.empty()) ) {

//     }
//     if ((wpntypes.left != "shield") & (wpntypes.left != "trinket")  & (wpntypes.left != "staff")) {
//         dmg_types.left = all_weapons[equipment[equipped.left].name].getStats().dmg_type;
//     }

//     if (wpntypes.right != "shield") & (wpntypes.left != "trinket")  & (wpntypes.left != "staff") {
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

unsigned char Unit::totalDef(bool dmg_type) {
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
    return (total_def);
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

std::string Unit::getArmyName() {
    return (army_name);
}

unsigned char Unit::getArmy() {
    return (army);
}

void Unit::setArmy(const unsigned char in_army) {
    army = in_army;
    army_name = armyNames[army];
}

Combat_stats Unit::getCombatStats() {
    return (combat_stats);
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
    // short int * unit_position;
    // short int * enemy_position;
    // bool retaliates = false;

    // enemy_position = enemy->getPos();
    // unsigned char distance = std::abs(enemy_position[0] - position[0]) + std::abs(enemy_position[1] - position[1]);

    // for (int i = 0; i < 3; i++) {
    //     if ((distance >= temp_wpn.range[0]) && (distance <= temp_wpn.range[1])) {
    //         retaliates = 1;
    //     }
    // }
    return (false);
}

bool Unit::canDouble(Unit * enemy) {
    speed();
    bool doubles = ((current_speed - enemy->speed()) > 4);
    return (doubles);
}

char Unit::speed() {
    //*DESIGN QUESTION* What should be the influence of weapons?
    // Average of Con and Str? Con+Str/2?
    char current_speed = current_stats.agi - temp_wpn.wgt + current_stats.con + current_stats.str / 2;
    return (current_speed);
}

void Unit::readXML(tinyxml2::XMLElement * in_pUnit) {
    SDL_Log("Reading Unit element.");
    const char * buffer;
    unsigned int bufint;
    tinyxml2::XMLElement * ptemp;

    id = (unsigned short int)in_pUnit->IntAttribute("id");
    ptemp = in_pUnit->FirstChildElement("Name");

    if (!ptemp) {
        SDL_Log("Cannot get Name element");
    } else {
        name = ptemp->GetText();
    }

    ptemp = in_pUnit->FirstChildElement("Sex");

    if (!ptemp) {
        SDL_Log("Cannot get Sex element");
    } else {
        ptemp->QueryBoolText(&sex);
    }

    ptemp = in_pUnit->FirstChildElement("SkillsCode");

    if (!ptemp) {
        SDL_Log("Cannot get SkillsCode element");
    } else {
        buffer = ptemp->GetText();
        sscanf(buffer, "%llx", &skills);
    }

    ptemp = in_pUnit->FirstChildElement("BaseExp");

    if (!ptemp) {
        SDL_Log("Cannot get BaseExp element");
    } else {
        ptemp->QueryUnsignedText(&bufint);
        base_exp = (unsigned short int)bufint;
    }


    ptemp = in_pUnit->FirstChildElement("CurrentHP");

    if (!ptemp) {
        SDL_Log("Cannot get CurrentHP element");
    } else {
        ptemp->QueryUnsignedText(&bufint);
        current_hp = (unsigned char)bufint;
    }

    ptemp = in_pUnit->FirstChildElement("Exp");

    if (!ptemp) {
        SDL_Log("Cannot get Exp element");
    } else {
        ptemp->QueryUnsignedText(&bufint);
        exp = (unsigned short int)bufint;
    }


    ptemp = in_pUnit->FirstChildElement("Class");

    if (!ptemp) {
        SDL_Log("Cannot get Class element");
    } else {
        class_index = (unsigned char)ptemp->IntAttribute("id");;
    }

    ptemp = in_pUnit->FirstChildElement("Stats");

    if (!ptemp) {
        SDL_Log("Cannot get Stats element");
    } else {
        readXML_stats(ptemp, &current_stats);
    }

    ptemp = in_pUnit->FirstChildElement("Growths");

    if (!ptemp) {
        SDL_Log("Cannot get Growths element");
    } else {
        readXML_stats(ptemp, &growths);
    }


    ptemp = in_pUnit->FirstChildElement("Caps");

    if (!ptemp) {
        SDL_Log("Cannot get Caps element");
    } else {
        readXML_stats(ptemp, &caps_stats);
    }

    ptemp = in_pUnit->FirstChildElement("Bases");

    if (!ptemp) {
        SDL_Log("Cannot get Bases element");
    } else {
        readXML_stats(ptemp, &base_stats);
    }

    ptemp = in_pUnit->FirstChildElement("Equipment");

    if (!ptemp) {
        SDL_Log("Cannot get Equipment element");
    } else {
        readXML_items(ptemp, equipment);
    }

    tinyxml2::XMLElement * pLevelUps = in_pUnit->FirstChildElement("LevelUps");

    if (!pLevelUps) {
        SDL_Log("Cannot get levelUps element");
    } else {
        ptemp = pLevelUps->FirstChildElement("LevelUp");
        Unit_stats temp_stats = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};

        do {
            readXML_stats(ptemp, &temp_stats);
            grown_stats.push_back(temp_stats);
            ptemp = ptemp->NextSiblingElement("LevelUp");
        } while (ptemp);
    }

    combatStats();
    mvt_type = mvtTypes[class_index];
    class_name = classNames[class_index];
    sex_name = sexNames[sex];
    skill_names = skillNames(skills);
    speed();
}

void Unit::readJSON(cJSON * in_json) {
    // SDL_Log("Really reading JSON file");
    cJSON * junit = cJSON_GetObjectItemCaseSensitive(in_json, "unit");
    cJSON * jid = cJSON_GetObjectItemCaseSensitive(junit, "id");
    cJSON * jname = cJSON_GetObjectItemCaseSensitive(junit, "Name");
    cJSON * jsex = cJSON_GetObjectItemCaseSensitive(junit, "Sex");
    cJSON * jbase_exp = cJSON_GetObjectItemCaseSensitive(junit, "BaseExp");
    cJSON * jexp = cJSON_GetObjectItemCaseSensitive(junit, "Exp");
    cJSON * jcurrent_hp = cJSON_GetObjectItemCaseSensitive(junit, "CurrentHP");
    cJSON * jclass = cJSON_GetObjectItemCaseSensitive(junit, "Class");
    cJSON * jcurrent_stats = cJSON_GetObjectItemCaseSensitive(junit, "Stats");
    cJSON * jcaps_stats = cJSON_GetObjectItemCaseSensitive(junit, "Caps");
    cJSON * jgrowths = cJSON_GetObjectItemCaseSensitive(junit, "Growths");
    cJSON * jlevelups = cJSON_GetObjectItemCaseSensitive(junit, "Level-ups");
    cJSON * jitems = cJSON_GetObjectItemCaseSensitive(junit, "Items");

    id = cJSON_GetNumberValue(jid);
    name = cJSON_GetStringValue(jname);
    // SDL_Log("name: %s", name.c_str());
    sex = cJSON_IsTrue(jsex);
    base_exp = cJSON_GetNumberValue(jbase_exp);
    exp = cJSON_GetNumberValue(jexp);
    current_hp = cJSON_GetNumberValue(jcurrent_hp);
    class_name = cJSON_GetStringValue(jclass);
    readJSON_stats(jcurrent_stats, &current_stats);
    readJSON_stats(jcaps_stats, &caps_stats);
    readJSON_stats(jgrowths, &growths);

    cJSON * jlevelup = cJSON_GetObjectItemCaseSensitive(jlevelups, "Level-ups");
    Unit_stats temp_ustats;

    while (jlevelup != NULL) {
        readJSON_stats(jlevelup, &temp_ustats);
        grown_stats.push_back(temp_ustats);
        jlevelup = cJSON_GetObjectItemCaseSensitive(jlevelups, "Level-ups");
    };

    cJSON * jitem = cJSON_GetObjectItemCaseSensitive(jitems, "Item");

    Inventory_item temp_wstats;

    int i = 0;

    while ((jitem != NULL) && (i < DEFAULT::EQUIPMENT_SIZE)) {
        readJSON_stats(jitem, &temp_wstats);
        equipment[i] = temp_wstats;
        jitem = cJSON_GetObjectItemCaseSensitive(jitems, "Item");
        i++;
    };
}

void Unit::writeJSON(cJSON * in_json) {
    if (in_json != NULL) {

        cJSON * junit = cJSON_CreateObject();
        cJSON_AddItemToObject(in_json, "unit", junit);

        cJSON * jid = cJSON_CreateNumber(id);
        cJSON * jexp = cJSON_CreateNumber(base_exp);
        cJSON * jbase_exp = cJSON_CreateNumber(exp);
        cJSON * jcurrent_hp = cJSON_CreateNumber(current_hp);
        cJSON * jsex = cJSON_CreateBool(sex);
        cJSON * jname = cJSON_CreateString(name.c_str());
        cJSON * jclass = cJSON_CreateString(class_name.c_str());
        cJSON * jcurrent_stats = cJSON_CreateObject();
        writeJSON_stats(jcurrent_stats, &current_stats);
        cJSON * jcaps_stats = cJSON_CreateObject();
        writeJSON_stats(jcaps_stats, &caps_stats);
        cJSON * jbase_stats = cJSON_CreateObject();
        writeJSON_stats(jbase_stats, &base_stats);
        cJSON * jgrowths = cJSON_CreateObject();
        writeJSON_stats(jgrowths, &growths);
        cJSON * jgrown = cJSON_CreateObject();
        cJSON * jlevel = NULL;
        cJSON * jlevelup = NULL;

        cJSON_AddItemToObject(junit, "level", jlevel);
        cJSON_AddItemToObject(junit, "id", jid);
        cJSON_AddItemToObject(junit, "Name", jname);
        cJSON_AddItemToObject(junit, "Sex", jsex);
        cJSON_AddItemToObject(junit, "BaseExp", jbase_exp);
        cJSON_AddItemToObject(junit, "Exp", jexp);
        cJSON_AddItemToObject(junit, "CurrentHP", jcurrent_hp);
        cJSON_AddItemToObject(junit, "Class", jclass);
        cJSON_AddItemToObject(junit, "Stats", jcurrent_stats);
        cJSON_AddItemToObject(junit, "Caps", jcaps_stats);
        cJSON_AddItemToObject(junit, "Growths", jgrowths);
        cJSON_AddItemToObject(junit, "Level-ups", jgrown);

        for (short int i = 0; i < grown_stats.size(); i++) {
            jlevelup = cJSON_CreateObject();
            jlevel = cJSON_CreateNumber(i - base_exp / 100 + 2);
            cJSON_AddItemToObject(jlevelup, "level", jlevel);
            writeJSON_stats(jlevelup, &grown_stats[i]);
            cJSON_AddItemToObject(jgrown, "Level-up", jlevelup);
            // +2 -> +1 start at lvl1, +1 cause you level to level 2
        }


        cJSON * jitems = cJSON_CreateObject();
        writeJSON_items(jitems, equipment, DEFAULT::EQUIPMENT_SIZE);

        cJSON_AddItemToObject(junit, "Items", jitems);

    } else {
        SDL_Log("in_json is not NULL");
    }
}

void Unit::writeXML(tinyxml2::XMLDocument * in_doc, tinyxml2::XMLElement * in_pUnit) {
    char buffer[DEFAULT::BUFFER_SIZE];

    in_pUnit->SetAttribute("id", id);

    tinyxml2::XMLElement * pName = in_doc->NewElement("Name");
    in_pUnit->InsertEndChild(pName);
    pName->SetText(name.c_str());

    tinyxml2::XMLElement * pSex = in_doc->NewElement("Sex");
    in_pUnit->InsertEndChild(pSex);
    pSex->SetText(sex);
    pSex->SetAttribute("eg", "hasPenis");

    tinyxml2::XMLElement * pClass = in_doc->NewElement("Class");
    in_pUnit->InsertEndChild(pClass);
    pClass->SetText(class_name.c_str());
    pClass->SetAttribute("id", class_index);

    tinyxml2::XMLElement * pExp = in_doc->NewElement("Exp");
    in_pUnit->InsertEndChild(pExp);
    pExp->SetText(exp);

    tinyxml2::XMLElement * pBaseExp = in_doc->NewElement("BaseExp");
    in_pUnit->InsertEndChild(pBaseExp);
    pBaseExp->SetText(base_exp);

    tinyxml2::XMLElement * pCurrenthp = in_doc->NewElement("CurrentHP");
    in_pUnit->InsertEndChild(pCurrenthp);
    pCurrenthp->SetText(current_hp);

    tinyxml2::XMLElement * pStats = in_doc->NewElement("Stats");
    in_pUnit->InsertEndChild(pStats);
    writeXML_stats(in_doc, pStats, &current_stats);

    tinyxml2::XMLElement * pGrowths = in_doc->NewElement("Growths");
    in_pUnit->InsertEndChild(pGrowths);
    writeXML_stats(in_doc, pGrowths, &growths);

    tinyxml2::XMLElement * pCaps = in_doc->NewElement("Caps");
    in_pUnit->InsertEndChild(pCaps);
    writeXML_stats(in_doc, pCaps, &caps_stats);

    tinyxml2::XMLElement * pBases = in_doc->NewElement("Bases");
    in_pUnit->InsertEndChild(pBases);
    writeXML_stats(in_doc, pBases, &base_stats);

    if (grown_stats.size() > 0) {
        tinyxml2::XMLElement * pGrown = in_doc->NewElement("LevelUps");
        in_pUnit->InsertEndChild(pGrown);
        tinyxml2::XMLElement * pGrownLevel;

        for (int i = 0; i < grown_stats.size(); i++) {
            pGrownLevel = in_doc->NewElement("LevelUp");
            pGrown->InsertEndChild(pGrownLevel);
            writeXML_stats(in_doc, pGrownLevel, &(grown_stats[i]));
        }
    }

    if (skill_names.size() > 0) {
        tinyxml2::XMLElement * pSkills = in_doc->NewElement("Skills");
        tinyxml2::XMLElement * pSkill;

        for (int i = 0; i < skill_names.size(); i++) {
            pSkill = in_doc->NewElement("Skill");
            pSkills->InsertEndChild(pSkill);
            pSkill->SetText(skill_names[i].c_str());
        }
    }

    tinyxml2::XMLElement * pSkillsCode = in_doc->NewElement("SkillsCode");
    stbsp_sprintf(buffer, "0x%llx", skills);
    pSkillsCode->SetText(buffer);
    in_pUnit->InsertEndChild(pSkillsCode);
    tinyxml2::XMLElement * pEquipment = in_doc->NewElement("Equipment");
    in_pUnit->InsertEndChild(pEquipment);
    writeXML_items(in_doc, pEquipment, equipment, DEFAULT::EQUIPMENT_SIZE);
}

void baseUnits(std::unordered_map<int, Unit> * in_units, std::vector<short int> toload) {
    SDL_Log("Making base units \n");
    Unit temp_unit;
    Unit_stats temp;
    Inventory_item temp_wpn;
    std::vector<short int> temp_supports;
    Equipped temp_equipped;
    int index;

    for (short unsigned int i = 0; i < toload.size(); i++) {
        index = toload[i];
        temp_unit = Unit();

        switch (index) {
            case UNIT::NAME::ERWIN:
                //hp,str,mag,agi,dex,luck,def,res,con,move,prof
                temp = {17,  6,  2,  7,  7,   7,  4,  5,  6, 5, 4};
                temp_unit = Unit(UNIT::NAME::ERWIN, UNIT::CLASS::MERCENARY, temp, UNIT::SEX::M);
                temp = {48, 14, 25, 32, 34,  28, 19, 40, 15, 7, 20};
                temp_unit.setCaps(temp);
                temp = {60, 50, 20, 60, 70,  40, 30, 20,  10, 0};
                temp_unit.setGrowths(temp);
                temp_unit.setBaseExp(0);
                temp_wpn.id = ITEM::NAME::FLEURET;
                temp_unit.addEquipment(temp_wpn);
                temp_wpn.id = ITEM::NAME::KITCHEN_KNIFE;
                temp_unit.addEquipment(temp_wpn);
                temp_wpn.id = ITEM::NAME::POT_LID;
                temp_unit.addEquipment(temp_wpn);
                temp_supports = {UNIT::NAME::KIARA};
                temp_unit.setSupports(temp_supports);
                temp_unit.setArmy(UNIT::ARMY::ERWIN);
                in_units->erase(index);
                in_units->insert({index, temp_unit});
                break;

            case UNIT::NAME::RELIABLE:
                temp = {18,  6,  2,  7,  7,   7,  4,  5,  6, 7};
                temp_unit = Unit(UNIT::NAME::RELIABLE, UNIT::CLASS::CAVALIER, temp, UNIT::SEX::M);
                temp = {48, 14, 25, 32, 34,  28, 19, 40, 15};
                temp_unit.setCaps(temp);
                temp = {60, 50, 20, 60, 70,  40, 30, 20,  10, 0};
                temp_unit.setGrowths(temp);
                temp_unit.setBaseExp(100);
                temp_wpn.id = ITEM::NAME::IRON_LANCE;
                temp_unit.addEquipment(temp_wpn);
                temp_unit.setArmy(UNIT::ARMY::ERWIN);
                in_units->erase(index);
                in_units->insert({index, temp_unit});
                break;

            case UNIT::NAME::COWARD:
                temp = {19,  6,  2,  7,  7,   7,  4,  5,  6,  7};
                temp_unit = Unit(UNIT::NAME::COWARD, UNIT::CLASS::CAVALIER, temp, UNIT::SEX::M);
                temp = {48, 14, 25, 32, 34,  28, 19, 40, 15};
                temp_unit.setCaps(temp);
                temp = {60, 50, 20, 60, 70,  40, 30, 20,  10, 0};
                temp_unit.setGrowths(temp);
                temp_unit.setBaseExp(200);
                temp_unit.setArmy(UNIT::ARMY::ERWIN);
                in_units->erase(index);
                in_units->insert({index, temp_unit});
                break;

            case UNIT::NAME::JAIGEN1H:
                temp = {20,  6,  2,  7,  7,   7,  4,  5,  6,  6};
                temp_unit = Unit(UNIT::NAME::JAIGEN1H, UNIT::CLASS::MOUSQUETAIRE, temp, UNIT::SEX::M);
                temp = {48, 14, 25, 32, 34,  28, 19, 40, 15};
                temp_unit.setCaps(temp);
                temp = {60, 50, 20, 60, 70,  40, 30, 20,  10, 0};
                temp_unit.setGrowths(temp);
                temp_unit.setBaseExp(2200);
                temp_unit.setArmy(UNIT::ARMY::ERWIN);
                in_units->erase(index);
                in_units->insert({index, temp_unit});
                break;

            case UNIT::NAME::KIARA:
                temp = {14,  6,  2,  7,  7,   7,  4,  5,  6,  5};
                temp_unit = Unit(UNIT::NAME::KIARA, UNIT::CLASS::CLERIC, temp, UNIT::SEX::F);
                temp = {48, 14, 25, 32, 34,  28, 19, 40, 15};
                temp_unit.setCaps(temp);
                temp = {60, 50, 20, 60, 70,  40, 30, 20,  10, 0};
                temp_unit.setGrowths(temp);
                temp_unit.setBaseExp(100);
                temp_unit.setArmy(UNIT::ARMY::ERWIN);
                in_units->erase(index);
                in_units->insert({index, temp_unit});
                break;

            case UNIT::NAME::HOTTIE:
                temp = {16,  6,  2,  7,  7,   7,  4,  5,  6,  6};
                temp_unit = Unit(UNIT::NAME::HOTTIE, UNIT::CLASS::PICKPOCKET, temp, UNIT::SEX::M);
                temp = {48, 14, 25, 32, 34,  28, 19, 40, 15};
                temp_unit.setCaps(temp);
                temp = {60, 50, 20, 60, 70,  40, 30, 20,  10, 0};
                temp_unit.setGrowths(temp);
                temp_unit.setBaseExp(2200);
                temp_unit.setArmy(UNIT::ARMY::ERWIN);
                in_units->erase(index);
                in_units->insert({index, temp_unit});
                break;

            case UNIT::NAME::SERVIL:
                temp = {22,  4,  5,  7,  6,   8,  4,  6,  5, 5}; // 4 or 5?
                temp_unit = Unit(UNIT::NAME::SERVIL, UNIT::CLASS::KNIGHT, temp, UNIT::SEX::M);
                temp = {48, 14, 25, 32, 34,  28, 19, 40, 15};
                temp_unit.setCaps(temp);
                temp = {60, 50, 20, 60, 70,  40, 30, 20,  10, 0};
                temp_unit.setGrowths(temp);
                temp_unit.setBaseExp(500);
                temp_unit.setArmy(UNIT::ARMY::ERWIN);
                in_units->erase(index);
                in_units->insert({index, temp_unit});
                break;

            case UNIT::NAME::PERIGNON:
                temp = {34,  4,  5,  7,  6,   8,  4,  6,  5, 5};
                temp_unit = Unit(UNIT::NAME::PERIGNON, UNIT::CLASS::MAGE, temp, UNIT::SEX::M);
                temp = {48, 14, 25, 32, 34,  28, 19, 40, 15};
                temp_unit.setCaps(temp);
                temp = {60, 50, 20, 60, 70,  40, 30, 20,  10, 0};
                temp_unit.setGrowths(temp);
                temp_unit.setBaseExp(1200);
                temp_unit.setArmy(UNIT::ARMY::ERWIN);
                in_units->erase(index);
                in_units->insert({index, temp_unit});
                break;

            case UNIT::NAME::POET:
                temp = {15,  4,  5,  7,  6,   8,  4,  6,  5, 5};
                temp_unit = Unit(UNIT::NAME::POET, UNIT::CLASS::MAGE, temp, UNIT::SEX::M);
                temp = {48, 14, 25, 32, 34,  28, 19, 40, 15};
                temp_unit.setCaps(temp);
                temp = {60, 50, 20, 60, 70,  40, 30, 20,  10, 0};
                temp_unit.setGrowths(temp);
                temp_unit.setBaseExp(400);
                temp_unit.setArmy(UNIT::ARMY::ERWIN);
                in_units->erase(index);
                in_units->insert({index, temp_unit});
                break;

            case UNIT::NAME::SILOU:
                //hp,str,mag,agi,dex,luck,def,res,con,move,prof
                temp = {15,  4,  5,  7,  6,   8,  4,  6,  5,  5,  6};
                temp_unit = Unit(UNIT::NAME::SILOU, UNIT::CLASS::MAGE, temp, UNIT::SEX::F);
                temp = {48, 14, 25, 32, 34,  28, 19, 40, 15};
                temp_unit.setCaps(temp);
                temp = {60, 50, 20, 60, 70,  40, 30, 20,  10, 0};
                temp_unit.setGrowths(temp);
                temp_unit.setBaseExp(400);
                temp_wpn.id = ITEM::NAME::BALL_LIGHTNING;
                temp_unit.addEquipment(temp_wpn);
                temp_unit.equipsR(0);
                temp_unit.setArmy(UNIT::ARMY::ERWIN);
                in_units->erase(index);
                in_units->insert({index, temp_unit});
                break;

            case UNIT::NAME::BANDIT:
                temp = {15,  4,  5,  7,  6,   8,  4,  6,  5, 5};
                temp_unit = Unit(UNIT::NAME::BANDIT, UNIT::CLASS::BANDIT, temp, 1);
                temp = {48, 14, 25, 32, 34,  28, 19, 40, 15};
                temp_unit.setCaps(temp);
                temp = {60, 50, 20, 60, 70,  40, 30, 20,  10, 0};
                temp_unit.setGrowths(temp);
                temp_unit.setBaseExp(0);
                temp_wpn.id = ITEM::NAME::IRON_AXE;
                temp_unit.addEquipment(temp_wpn);
                temp_unit.equipsR(0);
                temp_unit.setArmy(UNIT::ARMY::ENEMY);
                in_units->erase(index);
                in_units->insert({index, temp_unit});
                break;

            case UNIT::NAME::DUELIST:
                temp = {15,  4,  5,  7,  6,   8,  4,  6,  5, 5};
                temp_unit = Unit(UNIT::NAME::DUELIST, UNIT::CLASS::DUELIST, temp, UNIT::SEX::M);
                temp = {48, 14, 25, 32, 34,  28, 19, 40, 15};
                temp_unit.setCaps(temp);
                temp = {60, 50, 20, 60, 70,  40, 30, 20,  10, 0};
                temp_unit.setGrowths(temp);
                temp_unit.setBaseExp(0);
                temp_unit.setArmy(UNIT::ARMY::ENEMY);
                in_units->erase(index);
                in_units->insert({index, temp_unit});
                break;

            case UNIT::NAME::PICKPOCKET:
                temp = {15,  4,  5,  7,  6,   8,  4,  6,  5, 5};
                temp_unit = Unit(UNIT::NAME::PICKPOCKET, UNIT::CLASS::PICKPOCKET, temp, UNIT::SEX::M);
                temp = {48, 14, 25, 32, 34,  28, 19, 40, 15};
                temp_unit.setCaps(temp);
                temp = {60, 50, 20, 60, 70,  40, 30, 20,  10, 0};
                temp_unit.setGrowths(temp);
                temp_unit.setBaseExp(0);
                temp_unit.setArmy(UNIT::ARMY::ENEMY);
                in_units->erase(index);
                in_units->insert({index, temp_unit});
                break;

            case UNIT::NAME::THIEF:
                temp = {15,  4,  5,  7,  6,   8,  4,  6,  5, 5};
                temp_unit = Unit(UNIT::NAME::THIEF, UNIT::CLASS::THIEF, temp, UNIT::SEX::M);
                temp = {48, 14, 25, 32, 34,  28, 19, 40, 15};
                temp_unit.setCaps(temp);
                temp = {60, 50, 20, 60, 70,  40, 30, 20,  10, 0};
                temp_unit.setGrowths(temp);
                temp_unit.setBaseExp(0);
                temp_unit.setArmy(UNIT::ARMY::ENEMY);
                in_units->erase(index);
                in_units->insert({index, temp_unit});
                break;

            case UNIT::NAME::ASSASSIN:
                temp = {15,  4,  5,  7,  6,   8,  4,  6,  5, 5};
                temp_unit = Unit(UNIT::NAME::ASSASSIN, UNIT::CLASS::ASSASSIN, temp, UNIT::SEX::M);
                temp = {48, 14, 25, 32, 34,  28, 19, 40, 15};
                temp_unit.setCaps(temp);
                temp = {60, 50, 20, 60, 70,  40, 30, 20,  10, 0};
                temp_unit.setGrowths(temp);
                temp_unit.setBaseExp(0);
                temp_unit.setArmy(UNIT::ARMY::ENEMY);
                in_units->erase(index);
                in_units->insert({index, temp_unit});
                break;

            case UNIT::NAME::ARCHER:
                temp = {15,  4,  5,  7,  6,   8,  4,  6,  5, 5};
                temp_unit = Unit(UNIT::NAME::ARCHER, UNIT::CLASS::ARCHER, temp, UNIT::SEX::M);
                temp = {48, 14, 25, 32, 34,  28, 19, 40, 15};
                temp_unit.setCaps(temp);
                temp = {60, 50, 20, 60, 70,  40, 30, 20,  10, 0};
                temp_unit.setGrowths(temp);
                temp_unit.setBaseExp(0);
                temp_unit.setArmy(UNIT::ARMY::ENEMY);
                in_units->erase(index);
                in_units->insert({index, temp_unit});
                break;

            case UNIT::NAME::MARKSMAN:
                temp = {15,  4,  5,  7,  6,   8,  4,  6,  5, 5};
                temp_unit = Unit(UNIT::NAME::MARKSMAN, UNIT::CLASS::MARKSMAN, temp, UNIT::SEX::M);
                temp = {48, 14, 25, 32, 34,  28, 19, 40, 15};
                temp_unit.setCaps(temp);
                temp = {60, 50, 20, 60, 70,  40, 30, 20,  10, 0};
                temp_unit.setGrowths(temp);
                temp_unit.setBaseExp(0);
                temp_unit.setArmy(UNIT::ARMY::ENEMY);
                in_units->erase(index);
                in_units->insert({index, temp_unit});
                break;

            case UNIT::NAME::MERCENARY:
                temp = {15,  4,  5,  7,  6,   8,  4,  6,  5, 5};
                temp_unit = Unit(UNIT::NAME::MERCENARY, UNIT::CLASS::MERCENARY, temp, UNIT::SEX::M);
                temp = {48, 14, 25, 32, 34,  28, 19, 40, 15};
                temp_unit.setCaps(temp);
                temp = {60, 50, 20, 60, 70,  40, 30, 20,  10, 0};
                temp_unit.setGrowths(temp);
                temp_unit.setBaseExp(0);
                temp_unit.setArmy(UNIT::ARMY::ENEMY);
                in_units->erase(index);
                in_units->insert({index, temp_unit});
                break;

            case UNIT::NAME::HERO:
                temp = {15,  4,  5,  7,  6,   8,  4,  6,  5, 5};
                temp_unit = Unit(UNIT::NAME::HERO, UNIT::CLASS::HERO, temp, UNIT::SEX::M);
                temp = {48, 14, 25, 32, 34,  28, 19, 40, 15};
                temp_unit.setCaps(temp);
                temp = {60, 50, 20, 60, 70,  40, 30, 20,  10, 0};
                temp_unit.setGrowths(temp);
                temp_unit.setBaseExp(0);
                temp_unit.setArmy(UNIT::ARMY::ENEMY);
                in_units->erase(index);
                in_units->insert({index, temp_unit});
                break;

            case UNIT::NAME::CORSAIR:
                temp = {15,  4,  5,  7,  6,   8,  4,  6,  5, 5};
                temp_unit = Unit(UNIT::NAME::CORSAIR, UNIT::CLASS::CORSAIR, temp, UNIT::SEX::M);
                temp = {48, 14, 25, 32, 34,  28, 19, 40, 15};
                temp_unit.setCaps(temp);
                temp = {60, 50, 20, 60, 70,  40, 30, 20,  10, 0};
                temp_unit.setGrowths(temp);
                temp_unit.setBaseExp(0);
                temp_unit.setArmy(UNIT::ARMY::ENEMY);
                in_units->erase(index);
                in_units->insert({index, temp_unit});
                break;

            case UNIT::NAME::VIKING:
                temp = {15,  4,  5,  7,  6,   8,  4,  6,  5, 5};
                temp_unit = Unit(UNIT::NAME::VIKING, UNIT::CLASS::VIKING, temp, UNIT::SEX::M);
                temp = {48, 14, 25, 32, 34,  28, 19, 40, 15};
                temp_unit.setCaps(temp);
                temp = {60, 50, 20, 60, 70,  40, 30, 20,  10, 0};
                temp_unit.setGrowths(temp);
                temp_unit.setBaseExp(0);
                temp_unit.setArmy(UNIT::ARMY::ENEMY);
                in_units->erase(index);
                in_units->insert({index, temp_unit});
                break;

            case UNIT::NAME::RAVAGER:
                temp = {15,  4,  5,  7,  6,   8,  4,  6,  5, 5};
                temp_unit = Unit(UNIT::NAME::RAVAGER, UNIT::CLASS::RAVAGER, temp, UNIT::SEX::M);
                temp = {48, 14, 25, 32, 34,  28, 19, 40, 15};
                temp_unit.setCaps(temp);
                temp = {60, 50, 20, 60, 70,  40, 30, 20,  10, 0};
                temp_unit.setGrowths(temp);
                temp_unit.setBaseExp(0);
                temp_unit.setArmy(UNIT::ARMY::ENEMY);
                in_units->erase(index);
                in_units->insert({index, temp_unit});
                break;

            case UNIT::NAME::CAVALIER:
                temp = {15,  4,  5,  7,  6,   8,  4,  6,  5, 5};
                temp_unit = Unit(UNIT::NAME::CAVALIER, UNIT::CLASS::CAVALIER, temp, UNIT::SEX::M);
                temp = {48, 14, 25, 32, 34,  28, 19, 40, 15};
                temp_unit.setCaps(temp);
                temp = {60, 50, 20, 60, 70,  40, 30, 20,  10, 0};
                temp_unit.setGrowths(temp);
                temp_unit.setBaseExp(0);
                temp_unit.setArmy(UNIT::ARMY::ENEMY);
                in_units->erase(index);
                in_units->insert({index, temp_unit});
                break;

            case UNIT::NAME::PALADIN:
                temp = {15,  4,  5,  7,  6,   8,  4,  6,  5, 5};
                temp_unit = Unit(UNIT::NAME::PALADIN, UNIT::CLASS::PALADIN, temp, UNIT::SEX::M);
                temp = {48, 14, 25, 32, 34,  28, 19, 40, 15};
                temp_unit.setCaps(temp);
                temp = {60, 50, 20, 60, 70,  40, 30, 20,  10, 0};
                temp_unit.setGrowths(temp);
                temp_unit.setBaseExp(0);
                temp_unit.setArmy(UNIT::ARMY::ENEMY);
                in_units->erase(index);
                in_units->insert({index, temp_unit});
                break;

            case UNIT::NAME::FENCER:
                temp = {15,  4,  5,  7,  6,   8,  4,  6,  5, 5};
                temp_unit = Unit(UNIT::NAME::FENCER, UNIT::CLASS::FENCER, temp, UNIT::SEX::M);
                temp = {48, 14, 25, 32, 34,  28, 19, 40, 15};
                temp_unit.setCaps(temp);
                temp = {60, 50, 20, 60, 70,  40, 30, 20,  10, 0};
                temp_unit.setGrowths(temp);
                temp_unit.setBaseExp(0);
                temp_unit.setArmy(UNIT::ARMY::ENEMY);
                in_units->erase(index);
                in_units->insert({index, temp_unit});
                break;

            case UNIT::NAME::MOUSQUETAIRE:
                temp = {15,  4,  5,  7,  6,   8,  4,  6,  5, 5};
                temp_unit = Unit(UNIT::NAME::MOUSQUETAIRE, UNIT::CLASS::MOUSQUETAIRE, temp, UNIT::SEX::M);
                temp = {48, 14, 25, 32, 34,  28, 19, 40, 15};
                temp_unit.setCaps(temp);
                temp = {60, 50, 20, 60, 70,  40, 30, 20,  10, 0};
                temp_unit.setGrowths(temp);
                temp_unit.setBaseExp(0);
                temp_unit.setArmy(UNIT::ARMY::ENEMY);
                in_units->erase(index);
                in_units->insert({index, temp_unit});
                break;
        }
    }
}

void baseUnits(std::unordered_map<int, Unit> * in_units) {
    SDL_Log("Making all base units \n");
    std::unordered_map<int, Unit> temp_units;
    std::vector<short int> toload = cpprange((short int)UNIT::NAME::ERWIN, (short int)(UNIT::NAME::PC_END - 1));
    baseUnits(in_units, toload);
}

void writeUnits_NPC(const char * filename) {
    Unit Utemp;
    std::unordered_map<int, Unit> units;
    std::vector<short int> temp_int = {0};

    for (int i = UNIT::NAME::PC_END; i < UNIT::NAME::NPC_END; i++) {
        temp_int[0] = i;
        baseUnits(&units, temp_int);
        Utemp = units[i];
        Utemp.writeXML(filename, true);
    }
}

void writeUnits_PC(const char * filename) {
    Unit Utemp;
    std::unordered_map<int, Unit> units;
    std::vector<short int> temp_int = {0};

    for (int i = UNIT::NAME::ERWIN; i < UNIT::NAME::PC_END; i++) {
        temp_int[0] = i;
        baseUnits(&units, temp_int);
        Utemp = units[i];
        Utemp.writeXML(filename, true);
    }
}

void writeAllUnits(const char * filename) {
    writeUnits_PC(filename);
    writeUnits_NPC(filename);
}
