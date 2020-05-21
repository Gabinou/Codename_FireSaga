#include "unit.hpp"
#ifndef STB_SPRINTF_IMPLEMENTATION
#define STB_SPRINTF_IMPLEMENTATION
#include "stb_sprintf.h"
#endif /* STB_SPRINTF_IMPLEMENTATION */

Unit::Unit() {
    init();
}

Unit::Unit(const short int in_id, const char in_class_index,  const Unit_stats in_bases, const bool in_sex) : Unit() {
    setBases(in_bases);
    setStats(in_bases);
    setid(in_id);
    setClassind(in_class_index);
    setSex(in_sex);
}

Unit::Unit(const Unit & obj) : Unit()  {
    copyUnit(obj);
}

void Unit::init() {
    setXMLElement("Unit");
    setJSONElement("Unit");
    equipsR(0);
    equipsL(1);
}

void Unit::copyUnit(const Unit & obj) {
    SDL_Log("Copying Unit");
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

void Unit::setid(const short int in_id) {
    id = in_id;
    name = unitNames[in_id];
    SDL_Log("id: %d, name: %s", id, name.c_str());
    // setArmy(unitid2army(id));
}

short int Unit::getid() {
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


void Unit::removeEquipment(char in_index) {
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

short int Unit::getEquippable() {
    return (equippable);
}

char Unit::getMvttype() {
    return (mvt_type);
}

char Unit::getClassind() {
    return (class_index);
}

void Unit::setClassind(char in_class_index) {
    if ((in_class_index > 0) && (in_class_index < UNIT::CLASS::END)) {
        class_index = in_class_index;
        mvt_type = mvtTypes[class_index];
        class_name = classNames[class_index];
        equippable = equippableCodes[class_index];
    } else {
        SDL_Log("Unit class_index is invalid");
    }
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

void Unit::equips(const short int index, const bool hand) {
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

short int Unit::getLvl() const {
    return (ceil(current_hp / 100));
}

short int Unit::getExp() const {
    return (exp);
}

unsigned char * Unit::getRange() {
    // DESIGN QUESTION: what about equipping only an offhand? Should offhand have ranges?
    // Can you attack with only offhand weapons? how to treat their hit rate?
    SDL_Log("Computing unit range\n");
    static unsigned char range[2] = {0, 0};

    if (weapons != NULL) {
        if (equipped.left >= 0) {
            if (equipment[equipped.left].id > 0) {
                checkWeapon(equipment[equipped.left].id);
                unsigned char * temp = weapons->at(equipment[equipped.left].id).getStats().range;
                range[0] = temp[0];
                range[1] = temp[1];
            }
        }

        if (equipped.right >= 0) {
            if (equipment[equipped.right].id > 0) {
                checkWeapon(equipment[equipped.right].id);
                unsigned char * temp = weapons->at(equipment[equipped.right].id).getStats().range;
                range[0] = std::min(temp[0], range[0]);
                range[1] = std::max(temp[1], range[1]);
            }
        }
    } else {
        SDL_Log("weapons pointer is NULL");
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
    return (current_stats);
}

void Unit::setBases(const Unit_stats in_stats) {
    base_stats = in_stats;
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

void Unit::checkWeapon(short int in_id) {
    if (weapons != NULL) {
        if (weapons->find(in_id) == weapons->end()) {
            std::string filename;
            filename = "items//" + itemNames[in_id] + ".json";
            SDL_Log("Loading weapon %d %s", in_id, filename.c_str());
            Weapon temp_weapon;
            temp_weapon.readJSON(filename.c_str());
            SDL_Log("weapon loaded");
            // weapons->at(in_id).readJSON(filename.c_str()); // this line does not work.
            weapons->emplace(in_id, temp_weapon); // this line does not work.
            SDL_Log("weapon set");
        }
    } else {
        SDL_Log("weapons pointer is NULL");
    }
}

bool Unit::canEquip(short int in_id) {
    checkWeapon(in_id);
    short unsigned int wpntypecode;
    bool out = false;

    if (weapons != NULL) {
        wpntypecode = weapons->at(in_id).getType();
        out = ((equippable & wpntypecode) > 0);
    } else {
        SDL_Log("weapons pointer is NULL");
    }

    return (out);
}

bool Unit::canAttack() {
    // THIS FUNCTION needs to be updated with the fact that some weapons can have multiple types. -> typecodes
    bool out;
    struct wpntypecodes {
        short int left;
        short int right;
    } wpntypecodes;

    if (weapons != NULL) {
        if (equipped.left > 0) {
            checkWeapon(equipment[equipped.left].id);
            wpntypecodes.left = weapons->at(equipment[equipped.left].id).getType();

            if ((wpntypecodes.left != ITEM::TYPE::SHIELD)  & (wpntypecodes.left != ITEM::TYPE::TRINKET) & (wpntypecodes.left != ITEM::TYPE::STAFF)) {
                out = true;
            }
        }

        if (equipped.right > 0) {
            checkWeapon(equipment[equipped.right].id);
            wpntypecodes.right = weapons->at(equipment[equipped.right].id).getType();

            if ((wpntypecodes.right != ITEM::TYPE::SHIELD)  & (wpntypecodes.right != ITEM::TYPE::TRINKET) & (wpntypecodes.right != ITEM::TYPE::STAFF)) {
                out = true;
            }
        }
    } else {
        SDL_Log("weapons pointer is NULL");
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

//     wpntypes.left = weapons->at(equipment[equipped.left].name).getType();
//     wpntypes.right = weapons->at(equipment[equipped.right].name).getType();


//     if ((wpntypes.right.empty()) & (wpntypes.right.empty()) ) {

//     }
//     if ((wpntypes.left != "shield") & (wpntypes.left != "trinket")  & (wpntypes.left != "staff")) {
//         dmg_types.left = weapons->at(equipment[equipped.left].name).getStats().dmg_type;
//     }

//     if (wpntypes.right != "shield") & (wpntypes.left != "trinket")  & (wpntypes.left != "staff") {
//         dmg_types.right = weapons->at(equipment[equipped.right].name).getStats().dmg_type;
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

void Unit::setWeapons(std::unordered_map<short int, Weapon> * in_weapons) {
    weapons = in_weapons;
}


unsigned char Unit::totalDef(bool dmg_type) {
    unsigned char total_def = 0;
    // if (dmg_type){
    //     total_def += current_stats.res;
    //     total_def += weapons->at(equipment[equipped.left].name].getBonus().res;
    //     total_def += weapons->at(equipment[equipped.right].name].getBonus().res;
    //     if (weapons->at(equipment[equipped.right].name).getType() == "shield") {
    //         total_def += weapons->at(equipment[equipped.right].name).getStats().Mmight;
    //     }
    //     if (weapons->at(equipment[equipped.left].name).getType() == "shield") {
    //         total_def += weapons->at(equipment[equipped.left].name).getStats().Mmight;
    //     }
    // } else {
    //     total_def += current_stats.def;
    //     total_def += weapons->at(equipment[equipped.left].name].getBonus().def;
    //     total_def += weapons->at(equipment[equipped.right].name].getBonus().def;
    //     if (weapons->at(equipment[equipped.right].name).getType() == "shield") {
    //         total_def += weapons->at(equipment[equipped.right].name).getStats().Pmight;
    //     }
    //     if (weapons->at(equipment[equipped.left].name).getType() == "shield") {
    //         total_def += weapons->at(equipment[equipped.left].name).getStats().Pmight;
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

void Unit::readJSON(cJSON * in_junit) {
    cJSON * jid = cJSON_GetObjectItemCaseSensitive(in_junit, "id");
    cJSON * jname = cJSON_GetObjectItemCaseSensitive(in_junit, "Name");
    cJSON * jsex = cJSON_GetObjectItemCaseSensitive(in_junit, "Sex");
    cJSON * jbase_exp = cJSON_GetObjectItemCaseSensitive(in_junit, "BaseExp");
    cJSON * jexp = cJSON_GetObjectItemCaseSensitive(in_junit, "Exp");
    cJSON * jcurrent_hp = cJSON_GetObjectItemCaseSensitive(in_junit, "CurrentHP");
    cJSON * jclass_index = cJSON_GetObjectItemCaseSensitive(in_junit, "Class Index");
    cJSON * jcurrent_stats = cJSON_GetObjectItemCaseSensitive(in_junit, "Stats");
    cJSON * jbase_stats = cJSON_GetObjectItemCaseSensitive(in_junit, "Bases");
    cJSON * jcaps_stats = cJSON_GetObjectItemCaseSensitive(in_junit, "Caps");
    cJSON * jgrowths = cJSON_GetObjectItemCaseSensitive(in_junit, "Growths");
    cJSON * jlevelups = cJSON_GetObjectItemCaseSensitive(in_junit, "Level-ups");
    cJSON * jitems = cJSON_GetObjectItemCaseSensitive(in_junit, "Items");

    id = cJSON_GetNumberValue(jid); //returns 0 if junit is NULL
    name = cJSON_GetStringValue(jname);
    sex = cJSON_IsTrue(jsex);
    base_exp = cJSON_GetNumberValue(jbase_exp);
    exp = cJSON_GetNumberValue(jexp);
    current_hp = cJSON_GetNumberValue(jcurrent_hp);
    setClassind(cJSON_GetNumberValue(jclass_index));
    readJSON_stats(jcurrent_stats, &current_stats);
    readJSON_stats(jcaps_stats, &caps_stats);
    readJSON_stats(jbase_stats, &base_stats);
    readJSON_stats(jgrowths, &growths);

    cJSON * jlevelup = cJSON_GetObjectItemCaseSensitive(jlevelups, "Level-up");
    Unit_stats temp_ustats;

    while (jlevelup != NULL) {
        readJSON_stats(jlevelup, &temp_ustats);
        grown_stats.push_back(temp_ustats);
        jlevelup = jlevelup->next;
    };

    cJSON * jitem = cJSON_GetObjectItemCaseSensitive(jitems, "Item");

    Inventory_item temp_item;

    int i = 0;

    while ((jitem != NULL) && (i < DEFAULT::EQUIPMENT_SIZE)) {
        readJSON_item(jitem, &temp_item);
        equipment[i] = temp_item;
        jitem = jitem->next;
        i++;
    };
}

void Unit::writeJSON(cJSON * in_junit) {
    if (in_junit != NULL) {

        cJSON * jid = cJSON_CreateNumber(id);
        cJSON * jexp = cJSON_CreateNumber(base_exp);
        cJSON * jbase_exp = cJSON_CreateNumber(exp);
        cJSON * jcurrent_hp = cJSON_CreateNumber(current_hp);
        cJSON * jsex = cJSON_CreateBool(sex);
        cJSON * jname = cJSON_CreateString(name.c_str());
        cJSON * jclass = cJSON_CreateString(class_name.c_str());
        cJSON * jclass_index = cJSON_CreateNumber(class_index);
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

        cJSON_AddItemToObject(in_junit, "level", jlevel);
        cJSON_AddItemToObject(in_junit, "id", jid);
        cJSON_AddItemToObject(in_junit, "Name", jname);
        cJSON_AddItemToObject(in_junit, "Sex", jsex);
        cJSON_AddItemToObject(in_junit, "BaseExp", jbase_exp);
        cJSON_AddItemToObject(in_junit, "Exp", jexp);
        cJSON_AddItemToObject(in_junit, "CurrentHP", jcurrent_hp);
        cJSON_AddItemToObject(in_junit, "Class", jclass);
        cJSON_AddItemToObject(in_junit, "Class Index", jclass_index);
        cJSON_AddItemToObject(in_junit, "Stats", jcurrent_stats);
        cJSON_AddItemToObject(in_junit, "Caps", jcaps_stats);
        cJSON_AddItemToObject(in_junit, "Bases", jbase_stats);
        cJSON_AddItemToObject(in_junit, "Growths", jgrowths);
        cJSON_AddItemToObject(in_junit, "Level-ups", jgrown);

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

        cJSON_AddItemToObject(in_junit, "Items", jitems);

    } else {
        SDL_Log("in_json is not NULL");
    }
}

bool Unit::isLiterate() {
    return (literate);
}

void Unit::setLiteracy(bool in_literacy) {
    literate = in_literacy;
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

std::vector<short int> init_party = {UNIT::NAME::ERWIN, UNIT::NAME::KIARA};