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
    sex = in_sex;
    sex_name = sexName(sex);
}

Unit::Unit(const std::string in_name, const unsigned char in_class_index, const Unit_stats in_bases) : Unit(in_name, in_bases) {
    class_index = in_class_index;
    mvt_type = mvtType(class_index);
    class_name = className(class_index);
    skill_names = skillNames(skills);
    equippable = makeEquippable(class_index);
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

unsigned char Unit::getMvttype() {
    printf("inside unit %d\n", mvt_type);
    return (mvt_type);
}

void Unit::setSupports(short int * in_names) {
    // supports.names = in_names;
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

// void Unit::readXML_equipment(tinyxml2::XMLElement * in_pEquipment) {
//     tinyxml2::XMLElement * pItem = in_pEquipment->FirstChildElement("Item");
//     tinyxml2::XMLElement * pId;
//     tinyxml2::XMLElement * pUsed;
//     int bufint;
//     for (int i = 1; i < DEFAULT::EQUIPMENT_SIZE; i++) {
//         pId = pItem->FirstChildElement("id");
//         pUsed = pItem->FirstChildElement("Used");
//         pId->QueryIntText(&bufint);
//         equipment[i].id = bufint;
//         pUsed->QueryIntText(&bufint);
//         equipment[i].used = bufint;
//         pItem = pItem->NextSiblingElement("Item");
//     }
// }

void Unit::readXML(const char * filename) {
    SDL_Log("readXML Unit file: %s", filename);    
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
        SDL_Log("XML file parsing failed");
    }
    tinyxml2::XMLElement * ptemp;
    tinyxml2::XMLElement * pUnit = xmlDoc.FirstChildElement("Unit");
    if (!pUnit) {SDL_Log("Cannot get Unit element");}   
    ptemp = pUnit->FirstChildElement("Name");
    if (!ptemp) {SDL_Log("Cannot get Name element");}   
    name = ptemp->GetText();
    ptemp = pUnit->FirstChildElement("Sex");
    if (!ptemp) {SDL_Log("Cannot get Sex element");}   
    ptemp->QueryBoolText(&sex);    
    ptemp = pUnit->FirstChildElement("SkillsCode");
    if (!ptemp) {SDL_Log("Cannot get SkillsCode element");}   
    buffer = ptemp->GetText();
    // skills = strtoull(buffer, NULL, 16);
    sscanf(buffer, "%llx", &skills);
    ptemp = pUnit->FirstChildElement("BaseExp");
    if (!ptemp) {SDL_Log("Cannot get BaseExp element");}   
    ptemp->QueryUnsignedText(&bufint);
    base_exp = (unsigned short int)bufint;
    ptemp = pUnit->FirstChildElement("Exp");
    if (!ptemp) {SDL_Log("Cannot get Exp element");}   
    ptemp->QueryUnsignedText(&bufint);
    exp = (unsigned short int)bufint;
    ptemp = pUnit->FirstChildElement("Classid");
    if (!ptemp) {SDL_Log("Cannot get Classid element");}   
    ptemp->QueryUnsignedText(&bufint);
    class_index = (unsigned char)bufint;
    ptemp = pUnit->FirstChildElement("Stats");
    if (!ptemp) {SDL_Log("Cannot get Stats element");}   
    readXML_stats(ptemp, &current_stats);
    ptemp = pUnit->FirstChildElement("Growths");
    if (!ptemp) {SDL_Log("Cannot get Growths element");}   
    readXML_stats(ptemp, &growths);
    ptemp = pUnit->FirstChildElement("Caps");
    if (!ptemp) {SDL_Log("Cannot get Caps element");}   
    readXML_stats(ptemp, &caps_stats);
    ptemp = pUnit->FirstChildElement("Bases");
    if (!ptemp) {SDL_Log("Cannot get Bases element");}   
    readXML_stats(ptemp, &base_stats);
    ptemp = pUnit->FirstChildElement("Equipment");
    if (!ptemp) {SDL_Log("Cannot get Equipment element");}   
    readXML_equipment(equipment, ptemp);
    
    tinyxml2::XMLElement * pLevelUps = pUnit->FirstChildElement("LevelUps");
    if (!pLevelUps) {SDL_Log("Cannot get levelUps element");
    } else {   
        ptemp = pLevelUps->FirstChildElement("LevelUp");
        Unit_stats temp_stats = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
        do {
            readXML_stats(ptemp, &temp_stats);
            grown_stats.push_back(temp_stats); 
            ptemp = ptemp->NextSiblingElement("LevelUp");
        } while(ptemp);
    }

    combatStats();
    mvt_type = mvtType(class_index);
    class_name = className(class_index);
    sex_name = sexName(sex);
    skill_names = skillNames(skills);
    speed();
} 

void Unit::writeXML(const char * filename, const bool append) {
    SDL_Log("writeXML Unit to: %s\n", filename);
    // How to write files so that it is modifiable by randos?
    PHYSFS_file * fp;
    char buffer[DEFAULT::BUFFER_SIZE];
    tinyxml2::XMLDocument xmlDoc;
    if (!PHYSFS_exists(filename)) {    
        xmlDoc.InsertFirstChild(xmlDoc.NewDeclaration());
    }
    if (append) {
        fp = PHYSFS_openAppend(filename);

    } else {
        fp = PHYSFS_openWrite(filename);
    }
    
    tinyxml2::XMLElement * pUnit = xmlDoc.NewElement("Unit");
    xmlDoc.InsertEndChild(pUnit);
    // pUnit.setAttribute();
    
    tinyxml2::XMLElement * pName = xmlDoc.NewElement("Name");
    pUnit->InsertEndChild(pName);
    pName->SetText(name.c_str());
    
    tinyxml2::XMLElement * pSex = xmlDoc.NewElement("Sex");
    pUnit->InsertEndChild(pSex);
    pSex->SetText(sex);
    
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
    writeXML_stats(&xmlDoc, pStats, &current_stats);
    
    tinyxml2::XMLElement * pGrowths = xmlDoc.NewElement("Growths");
    pUnit->InsertEndChild(pGrowths);
    writeXML_stats(&xmlDoc, pGrowths, &growths);
    
    tinyxml2::XMLElement * pCaps = xmlDoc.NewElement("Caps");
    pUnit->InsertEndChild(pCaps);
    writeXML_stats(&xmlDoc, pCaps, &caps_stats);
    
    tinyxml2::XMLElement * pBases = xmlDoc.NewElement("Bases");
    pUnit->InsertEndChild(pBases);
    writeXML_stats(&xmlDoc, pBases, &base_stats);

    if (grown_stats.size() > 0) {
        tinyxml2::XMLElement * pGrown = xmlDoc.NewElement("LevelUps");
        pUnit->InsertEndChild(pGrown);
        tinyxml2::XMLElement * pGrownLevel;
        for (int i = 0; i < grown_stats.size(); i++) {
            pGrownLevel = xmlDoc.NewElement("LevelUp");
            pGrown->InsertEndChild(pGrownLevel);
            writeXML_stats(&xmlDoc, pGrownLevel, &(grown_stats[i]));
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

void Unit::write(const char * filename, const bool append) {
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

std::vector<Unit> all_units(UNIT::NAME::END);
std::vector<Unit> loaded_units;

void testXMLUnits() { 
    SDL_Log("Testing Unit xml writing and reading\n");
    Unit temp_unit;
    Unit_stats temp;
    Inventory_item temp_wpn;

    temp = {17,  6,  2,  7,  7,   7,  4,  5,  6, 5};
    temp_unit = Unit("Erwin", UNIT::CLASS::MERCENARY, temp, UNIT::SEX::M);
    temp = {48, 14, 25, 32, 34,  28, 19, 40, 15, 0};
    temp_unit.setCaps(temp);
    temp = {60, 50, 20, 60, 70,  40, 30, 20,  10, 0};
    temp_unit.setGrowths(temp);
    temp_unit.setBaseExp(0);
    temp_unit.levelUp();
    temp_unit.levelUp();
    temp_wpn.id = WPN::NAME::FLEURET;
    temp_unit.addEquipment(temp_wpn);
    temp_wpn.id = WPN::NAME::KITCHEN_KNIFE;
    temp_unit.addEquipment(temp_wpn);
    temp_wpn.id = WPN::NAME::POT_LID;
    temp_unit.addEquipment(temp_wpn);
    temp_unit.writeXML("unit_test.xml");
    temp_unit.writeXML("unit_test.binou");
    temp_unit = Unit();
    temp_unit.readXML("unit_test.xml");
    temp_unit.writeXML("unit_rewrite.xml");
    temp_unit = Unit();
    temp_unit.readXML("unit_test.binou");
    temp_unit.writeXML("unit_rewrite.binou");
}

void baseUnits() {
    printf("Making base units \n");
    Unit temp_unit;
    Unit_stats temp;
    Inventory_item temp_wpn;
    short int temp_supports;
    //hp,str,mag,skl,spd,luck,def,res,con,move
    // printf("Made unit.\n");

    temp = {17,  6,  2,  7,  7,   7,  4,  5,  6, 5};
    temp_unit = Unit("Erwin", UNIT::CLASS::MERCENARY, temp, UNIT::SEX::M);
    temp = {48, 14, 25, 32, 34,  28, 19, 40, 15, 0};
    temp_unit.setCaps(temp);
    temp = {60, 50, 20, 60, 70,  40, 30, 20,  10, 0};
    temp_unit.setGrowths(temp);
    temp_unit.setBaseExp(0);
    temp_wpn.id = WPN::NAME::FLEURET;
    temp_unit.addEquipment(temp_wpn);
    temp_wpn.id = WPN::NAME::KITCHEN_KNIFE;
    temp_unit.addEquipment(temp_wpn);
    temp_wpn.id = WPN::NAME::POT_LID;
    temp_unit.addEquipment(temp_wpn);
    // temp_supports = {UNIT::NAME::KIARA}; 
    // temp_unit.setSupports(temp_supports);
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

    all_units[UNIT::NAME::ERWIN].writeXML("playables.xml", true); 
    all_units[UNIT::NAME::KIARA].writeXML("playables.xml", true); 
    all_units[UNIT::NAME::RELIABLE].writeXML("playables.xml", true); 
    all_units[UNIT::NAME::COWARD].writeXML("playables.xml", true); 
    all_units[UNIT::NAME::HOTTIE].writeXML("playables.xml", true); 
    all_units[UNIT::NAME::POET].writeXML("playables.xml", true); 
    all_units[UNIT::NAME::PERIGNON].writeXML("playables.xml", true); 
    all_units[UNIT::NAME::SILOU].writeXML("playables.xml", true); 
    all_units[UNIT::NAME::JAIGEN1H].writeXML("playables.xml", true); 

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
