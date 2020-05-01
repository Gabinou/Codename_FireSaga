
#include "weapon.hpp"

Weapon::Weapon() {
    setXMLElement("Weapon");

}

Weapon::Weapon(short unsigned int in_type, Weapon_stats in_stats, unsigned char in_id) : Weapon() {
    stats = in_stats;
    name = wpnNames[in_id];
    type = in_type;
    id = in_id;
}

Weapon_stats Weapon::getStats() {
    return (stats);
}

void Weapon::setMalus(Unit_stats in_malus) {
    malus_stats = in_malus;
}

Unit_stats Weapon::getMalus() {
    return (malus_stats);
}

void Weapon::setStats(Weapon_stats in_stats) {
    stats = in_stats;
}

void Weapon::setBonus(Unit_stats in_bonus) {
    bonus_stats = in_bonus;
}
Unit_stats Weapon::getBonus() {
    return (bonus_stats);
}

bool Weapon::isSellable() {
    return (sellable);
}

void Weapon::setSellable(bool in_sellable) {
    sellable = in_sellable;
}

std::vector<unsigned short int> Weapon::getUsers() {
    return (users);
}

void Weapon::setUsers(std::vector<unsigned short int> in_users) {
    users = in_users;
}

void Weapon::infuse(unsigned char in_mag) {
    if (infused < 0) {
        infused = in_mag;
    } else {
        SDL_Log("Weapon is already infused/is a magic Weapon. Cannot infuse again.");
    }
}

char Weapon::getInfused() {
    return (infused);
}


bool Weapon::canInfuse() {
    bool out;

    if (infused < 0) {
        out = true;
    } else {
        out = false;
    }

    return (out);
}

void Weapon::setEffect(long unsigned int in_effect) {
    effect = in_effect;

    if ((in_effect & ITEM::EFFECT::USE_HEAL) > 0) {
        use_effect = ITEM::EFFECT::USE_HEAL;
    }

    if ((in_effect & ITEM::EFFECT::USE_BUFF) > 0) {
        use_effect = ITEM::EFFECT::USE_BUFF;
    }

    if ((in_effect & ITEM::EFFECT::USE_DIVINE) > 0) {
        use_effect = ITEM::EFFECT::USE_DIVINE;
    }


}
long unsigned int Weapon::getEffect() {
    return (effect);
}
void Weapon::setEffective(short unsigned int in_effective) {
    effective = in_effective;
}
short unsigned int Weapon::getEffective() {
    return (effective);
}
void Weapon::setDescription(std::string in_description) {
    description = in_description;
}
std::string Weapon::getDescription() {
    return (description);
}
std::string Weapon::getName() {
    return (name);
}
void Weapon::setName(std::string in_name) {
    name = in_name;
}
short unsigned int Weapon::getType() {
    return (type);
}
void Weapon::setType(short unsigned int in_type) {
    type = in_type;
}

void Weapon::readXML(tinyxml2::XMLElement * in_pWpn) {
    tinyxml2::XMLElement * ptemp;

    ptemp = in_pWpn->FirstChildElement("Name");

    if (!ptemp) {SDL_Log("Cannot get Name element");}

    name = ptemp->GetText();

    ptemp = in_pWpn->FirstChildElement("Sellable");

    if (!ptemp) {SDL_Log("Cannot get Sellable element");}

    sellable = ptemp->BoolText();

    id = (unsigned short int)in_pWpn->IntAttribute("id");
    ptemp = in_pWpn->FirstChildElement("Description");

    if (!ptemp) {SDL_Log("Cannot get Description element");}

    description = ptemp->GetText();
    ptemp = in_pWpn->FirstChildElement("Bonus");

    if (!ptemp) {SDL_Log("Cannot get Bonus element");}

    readXML_stats(ptemp, &bonus_stats);

    ptemp = in_pWpn->FirstChildElement("Malus");

    if (!ptemp) {SDL_Log("Cannot get Malus element");}

    readXML_stats(ptemp, &malus_stats);
    ptemp = in_pWpn->FirstChildElement("Types");

    if (!ptemp) {SDL_Log("Cannot get Types element");}

    type = (unsigned short int)ptemp->IntAttribute("id");
    ptemp = in_pWpn->FirstChildElement("Effectives");

    if (!ptemp) {SDL_Log("Cannot get Effectives element");}

    effective = (unsigned short int)ptemp->IntAttribute("id");
    ptemp = in_pWpn->FirstChildElement("Effects");

    if (!ptemp) {SDL_Log("Cannot get Effects element");}

    effect = (long unsigned int)ptemp->IntAttribute("id");
    ptemp = in_pWpn->FirstChildElement("Stats");

    if (!ptemp) {SDL_Log("Cannot get Stats element");}

    readXML_stats(ptemp, &stats);
}

void Weapon::writeXML(tinyxml2::XMLDocument * in_doc, tinyxml2::XMLElement * in_pWpn) {
    in_pWpn->SetAttribute("id", id);

    tinyxml2::XMLElement * pName = in_doc->NewElement("Name");
    in_pWpn->InsertEndChild(pName);
    pName->SetText(name.c_str());

    tinyxml2::XMLElement * pStats = in_doc->NewElement("Stats");
    in_pWpn->InsertEndChild(pStats);
    writeXML_stats(in_doc, pStats, &stats);

    tinyxml2::XMLElement * pDes = in_doc->NewElement("Description");
    in_pWpn->InsertEndChild(pDes);
    pDes->SetText(description.c_str());

    tinyxml2::XMLElement * pBonus = in_doc->NewElement("Bonus");
    in_pWpn->InsertEndChild(pBonus);
    writeXML_stats(in_doc, pBonus, &bonus_stats);

    tinyxml2::XMLElement * pMalus = in_doc->NewElement("Malus");
    in_pWpn->InsertEndChild(pMalus);
    writeXML_stats(in_doc, pMalus, &malus_stats);

    tinyxml2::XMLElement * pSellable = in_doc->NewElement("Sellable");
    in_pWpn->InsertEndChild(pSellable);
    pSellable->SetText(sellable);    

    tinyxml2::XMLElement * pInfused = in_doc->NewElement("Infused");
    in_pWpn->InsertEndChild(pInfused);
    pInfused->SetText(infused);

    tinyxml2::XMLElement * pEffectives = in_doc->NewElement("Effectives");
    in_pWpn->InsertEndChild(pEffectives);
    pEffectives->SetAttribute("id", effective);
    std::vector<std::string> effectives = unitTypes(effective);
    tinyxml2::XMLElement * pEffective;

    for (int i = 0; i < effectives.size(); i++) {
        pEffective = in_doc->NewElement("Effective");
        pEffectives->InsertEndChild(pEffective);
        pEffective->SetText(effectives[i].c_str());
    }

    tinyxml2::XMLElement * pTypes = in_doc->NewElement("Types");
    in_pWpn->InsertEndChild(pTypes);
    pTypes->SetAttribute("id", type);
    std::vector<std::string> types = wpnTypes(type);
    tinyxml2::XMLElement * pType;

    for (int i = 0; i < types.size(); i++) {
        pType = in_doc->NewElement("Type");
        pTypes->InsertEndChild(pType);
        pType->SetText(types[i].c_str());
    }

    tinyxml2::XMLElement * pEffects = in_doc->NewElement("Effects");
    in_pWpn->InsertEndChild(pEffects);
    tinyxml2::XMLElement * pEffect;
    std::vector<std::string> effects = wpnEffects(effect);

    for (int i = 0; i < effects.size(); i++) {
        pEffect = in_doc->NewElement("Effect");
        pEffects->InsertEndChild(pEffect);
        pEffect->SetText(effects[i].c_str());
    }

    pEffects->SetAttribute("id", (uint64_t) effect);
}

std::vector<Weapon> all_weapons(ITEM::NAME::END);
std::vector<Weapon> loaded_weapons;

void writeallXMLWeapons() {
    SDL_Log("Writinga all Weapon in an xml\n");
    PHYSFS_delete("all_weapons.xml");

    for (int i = 1; i < ITEM::NAME::END; i++) {
        all_weapons[i].writeXML("all_weapons.xml", true);
    }
}

std::vector<Weapon> baseWeapons(std::vector<short int> toload) {
    SDL_Log("Making some base weapons");
    Weapon temp_wpn;
    Weapon_stats temp_wpn_stats;
    std::vector<Weapon> wpn_out;
    wpn_out.resize(toload[toload.size() - 1] + 1);
    short unsigned int index;

    for (short unsigned int i = 0; i < toload.size(); i++) {
        index = toload[i];

        switch (index) {
            case ITEM::NAME::WOODEN_SWORD:
                // Pmight, Mmight, hit, dodge, crit, favor, wgt, uses, wpnlvl, range, hand, dmg_type, price
                temp_wpn_stats = {3, 0, 80, 0, 0, 0, 3, 30, 2, {1, 1}, {1, 2}, 0, 200, 0};
                temp_wpn = Weapon(ITEM::TYPE::SWORD, temp_wpn_stats, ITEM::NAME::WOODEN_SWORD);
                temp_wpn.setDescription("Practice sword, made of wood. It's crushing blows are still deadly.");
                wpn_out[ITEM::NAME::WOODEN_SWORD] = temp_wpn;
                break;
            case ITEM::NAME::KITCHEN_KNIFE:
                temp_wpn_stats = {1, 0, 25, 0, 0, 0, 2, 10, 1, {1, 1}, {1}, 0, 100, 0};
                temp_wpn = Weapon(ITEM::TYPE::SWORD, temp_wpn_stats, ITEM::NAME::KITCHEN_KNIFE);
                temp_wpn.setDescription("The poor, desperate swordman's dagger.");
                wpn_out[ITEM::NAME::KITCHEN_KNIFE] = temp_wpn;
                break;
            case ITEM::NAME::WRATH_SWORD:
                temp_wpn_stats = {7, 0, 70, 0, 25, 0, 7, 30, 9, {1, 1}, {1, 2}, 0, 800, 0};
                temp_wpn = Weapon(ITEM::TYPE::SWORD, temp_wpn_stats, ITEM::NAME::WRATH_SWORD);
                temp_wpn.setDescription("Increases critical hit chance.");
                wpn_out[ITEM::NAME::WRATH_SWORD] = temp_wpn;
                break;
            case ITEM::NAME::FLEURET:
                temp_wpn_stats = {4, 0, 90, 0, 0, 0, 4, 20, 6, {1, 1}, {1}, 0, 370, 0};
                temp_wpn = Weapon(ITEM::TYPE::SWORD, temp_wpn_stats, ITEM::NAME::FLEURET);
                temp_wpn.setEffective(UNIT::TYPE::ARMOR);
                temp_wpn.setDescription("Simple and light thrust swords. Effective against armor. Usually used with off-hand parry daggers.");
                wpn_out[ITEM::NAME::FLEURET] = temp_wpn;
                break;
            case ITEM::NAME::RAPIERE:
                temp_wpn_stats = {8, 0, 85, 5, 0, 0, 5, 25, 8, {1, 1}, {1}, 0, 470, 0};
                temp_wpn = Weapon(ITEM::TYPE::SWORD, temp_wpn_stats, ITEM::NAME::RAPIERE);
                temp_wpn.setEffective(UNIT::TYPE::ARMOR);
                temp_wpn.setDescription("Strong thrust swords. Effective against armor. Usually used with off-hand parry daggers.");
                wpn_out[ITEM::NAME::RAPIERE] = temp_wpn;
                break;
            case ITEM::NAME::GLAIVE:
                temp_wpn_stats = {5, 0, 55, 0, 0, 0, 7, 30, 3, {1, 1}, {1}, 0, 450, 0};
                temp_wpn = Weapon(ITEM::TYPE::SWORD, temp_wpn_stats, ITEM::NAME::GLAIVE);
                temp_wpn.setDescription("Old-world style short sword.");
                wpn_out[ITEM::NAME::GLAIVE] = temp_wpn;
                break;
            case ITEM::NAME::SABER:
                temp_wpn_stats = {7, 0, 60, 10, 0, 0, 6, 33, 8, {1, 1}, {1}, 0, 600, 0};
                temp_wpn = Weapon(ITEM::TYPE::SWORD, temp_wpn_stats, ITEM::NAME::SABER);
                temp_wpn.setDescription("Curved sword made for slashing.");
                wpn_out[ITEM::NAME::SABER] = temp_wpn;
                break;
            case ITEM::NAME::FAUCHON:
                temp_wpn_stats = {8, 0, 60, 15, 0, 0, 8, 27, 10, {1, 1}, {1}, 0, 700, 0};
                temp_wpn = Weapon(ITEM::TYPE::SWORD, temp_wpn_stats, ITEM::NAME::FAUCHON);
                temp_wpn.setDescription("Curved sword made for slashing.");
                wpn_out[ITEM::NAME::FAUCHON] = temp_wpn;
                break;
            case ITEM::NAME::IRON_SWORD:
                temp_wpn_stats = {6, 0, 70, 0, 0, 0, 6, 45, 6, {1, 1}, {1, 2}, 0, 500, 0};
                temp_wpn = Weapon(ITEM::TYPE::SWORD, temp_wpn_stats, ITEM::NAME::IRON_SWORD);
                temp_wpn.setDescription("Simple straight sword made of iron. Cheap and reliable.");
                wpn_out[ITEM::NAME::IRON_SWORD] = temp_wpn;
                break;
            case ITEM::NAME::STEEL_SWORD:
                temp_wpn_stats = {9, 0, 60, 0, 0, 0, 9, 30, 10, {1, 1}, {1, 2}, 0, 650, 0};
                temp_wpn = Weapon(ITEM::TYPE::SWORD, temp_wpn_stats, ITEM::NAME::STEEL_SWORD);
                temp_wpn.setDescription("Simple straight sword made of steel. Strong and durable.");
                wpn_out[ITEM::NAME::STEEL_SWORD] = temp_wpn;
                break;
            case ITEM::NAME::DAMAS_SWORD:
                temp_wpn_stats = {13, 0, 65, 0, 0, 0, 10, 25, 12, {1, 1}, {1, 2}, 0, 1020, 0};
                temp_wpn = Weapon(ITEM::TYPE::SWORD, temp_wpn_stats, ITEM::NAME::DAMAS_SWORD);
                temp_wpn.setDescription("Simple straight sword made of damascus steel. Beautifully rippled, deadly and expensive.");
                wpn_out[ITEM::NAME::DAMAS_SWORD] = temp_wpn;
                break;
            case ITEM::NAME::MERCIFUL_BLADE:
                temp_wpn_stats = {4, 0, 70, 0, 0, 10, 6, 30, 9, {1, 1}, {1, 2}, 0, 800, 0};
                temp_wpn = Weapon(ITEM::TYPE::SWORD, temp_wpn_stats, ITEM::NAME::MERCIFUL_BLADE);
                temp_wpn.setEffect(ITEM::EFFECT::SPARE);
                temp_wpn.setDescription("Infused with the life-giving tears of the goddess. Cannot reduce enemy HP below 1.");
                wpn_out[ITEM::NAME::MERCIFUL_BLADE] = temp_wpn;
                break;
            case ITEM::NAME::BROADSWORD:
                temp_wpn_stats = {10, 0, 40, 0, 0, 0, 10, 25, 6, {1, 1}, {1, 2}, 0, 940, 0};
                temp_wpn = Weapon(ITEM::TYPE::SWORD, temp_wpn_stats, ITEM::NAME::BROADSWORD);
                temp_wpn.setDescription("Blade of notable girth. Heavy and strong.");
                wpn_out[ITEM::NAME::BROADSWORD] = temp_wpn;
                break;
            case ITEM::NAME::ESPADON:
                temp_wpn_stats = {12, 0, 40, 0, 0, 0, 13, 20, 7, {1, 1}, {1, 2}, 0, 1100, 0};
                temp_wpn = Weapon(ITEM::TYPE::SWORD, temp_wpn_stats, ITEM::NAME::ESPADON);
                temp_wpn.setDescription("Blade of considerable girth and length. Heavier and stronger.");
                wpn_out[ITEM::NAME::ESPADON] = temp_wpn;
                break;
            case ITEM::NAME::OODACHI:
                temp_wpn_stats = {9, 0, 60, 10, 5, 0, 8, 25, 10, {1, 1}, {1, 2}, 0, 660, 0};
                temp_wpn = Weapon(ITEM::TYPE::SWORD, temp_wpn_stats, ITEM::NAME::OODACHI);
                temp_wpn.setDescription("Long, curved sword of the eastern lands.");
                wpn_out[ITEM::NAME::OODACHI] = temp_wpn;
                break;
            case ITEM::NAME::UCHIGATANA:
                temp_wpn_stats = {7, 0, 75, 5, 0, 0, 5, 30, 8, {1, 1}, {1, 2}, 0, 880, 0};
                temp_wpn = Weapon(ITEM::TYPE::SWORD, temp_wpn_stats, ITEM::NAME::UCHIGATANA);
                temp_wpn.setDescription("Curved sword in the style of the eastern lands. Very sharp.");
                wpn_out[ITEM::NAME::UCHIGATANA] = temp_wpn;
                break;
            case ITEM::NAME::EXSANGUE:
                temp_wpn_stats = {1, 0, 50, 0, 0, 0, 1, 20, 10, {1, 1}, {1, 2}, 0, 666, 0};
                temp_wpn = Weapon(ITEM::TYPE::SWORD, temp_wpn_stats, ITEM::NAME::EXSANGUE);
                temp_wpn.setEffect(ITEM::EFFECT::LIFESTEAL);
                temp_wpn.setDescription("Drains enemies of their blood. Heals HP equal to damage dealt.");
                wpn_out[ITEM::NAME::EXSANGUE] = temp_wpn;
                break;
            case ITEM::NAME::HAUTECLAIRE:
                temp_wpn_stats = {14, 0, 90, 10, 0, 0, 7, 25, 14, {1, 1}, {1, 2}, 0, 777, 0};
                temp_wpn = Weapon(ITEM::TYPE::SWORD, temp_wpn_stats, ITEM::NAME::HAUTECLAIRE);
                temp_wpn.setDescription("Crystal encrusted sword, as described in the Franc legends of old.");
                wpn_out[ITEM::NAME::HAUTECLAIRE] = temp_wpn;
                break;
            case ITEM::NAME::SECUNDUS:
                temp_wpn_stats = {16, 0, 100, 0, 0, 10, 9, 20, 12, {1, 1}, {1, 2}, 0, 69, 0};
                temp_wpn = Weapon(ITEM::TYPE::SWORD, temp_wpn_stats, ITEM::NAME::SECUNDUS);
                temp_wpn.setDescription("The first sword made by the God. The first weapon really only the second weapon of man, after his fists.");
                wpn_out[ITEM::NAME::SECUNDUS] = temp_wpn;
                break;
            case ITEM::NAME::EXCALIBUR:
                temp_wpn_stats = {15, 0, 90, 10, 0, 0, 10, 30, 15, {1, 1}, {1, 2}, 0, 1, 0};
                temp_wpn = Weapon(ITEM::TYPE::SWORD, temp_wpn_stats, ITEM::NAME::EXCALIBUR);
                temp_wpn.setEffective(UNIT::TYPE::DEMON);
                temp_wpn.setDescription("The king's sword. Found in a rock.");
                wpn_out[ITEM::NAME::EXCALIBUR] = temp_wpn;
                break;
            case ITEM::NAME::GALATINE:
                temp_wpn_stats = {12, 0, 100, 0, 0, 0, 6, 20, 18, {1, 1}, {1}, 0, 2000, 0};
                temp_wpn = Weapon(ITEM::TYPE::SWORD, temp_wpn_stats, ITEM::NAME::GALATINE);
                temp_wpn.setEffective(UNIT::TYPE::ARMOR);
                temp_wpn.setDescription("A pragmatic and honorable knight's thrusting sword. Usually used with off-hand parry daggers.");
                wpn_out[ITEM::NAME::GALATINE] = temp_wpn;
                break;
            case ITEM::NAME::RAIJINTOU:
                temp_wpn_stats = {5, 0, 50, 0, 0, 0, 7, 25, 11, {1, 2}, {1, 2}, 1, 1400, 0};
                temp_wpn = Weapon(ITEM::TYPE::SWORD, temp_wpn_stats, ITEM::NAME::RAIJINTOU);
                temp_wpn.setDescription("The thunder god's sword. Rains lightning upon enemies at a distance.");
                wpn_out[ITEM::NAME::RAIJINTOU] = temp_wpn;
                break;
            case ITEM::NAME::FUUJINTOU:
                temp_wpn_stats = {7, 0, 45, 0, 0, 0, 1, 25, 11, {1, 2}, {1, 2}, 1, 1640, 0};
                temp_wpn = Weapon(ITEM::TYPE::SWORD, temp_wpn_stats, ITEM::NAME::FUUJINTOU);
                temp_wpn.setDescription("The wind god's sword. Cuts enemies with wind at a distance.");
                wpn_out[ITEM::NAME::FUUJINTOU] = temp_wpn;
                break;
            case ITEM::NAME::HONJOU_MASAMUNE:
                temp_wpn_stats = {16, 0, 60, 5, 5, 0, 7, 35, 18, {1, 1}, {1, 2}, 0, 2200, 0};
                temp_wpn = Weapon(ITEM::TYPE::SWORD, temp_wpn_stats, ITEM::NAME::HONJOU_MASAMUNE);
                temp_wpn.setDescription("Lost sword made by the most skilled swordsmith of the eastern lands.");
                wpn_out[ITEM::NAME::HONJOU_MASAMUNE] = temp_wpn;
                break;
            case ITEM::NAME::RAW_IRON_SLAB:
                temp_wpn_stats = {20, 0, 30, 0, 0, 0, 20, 50, 20, {1, 1}, {1, 2}, 0, 9001, 0};
                temp_wpn = Weapon(ITEM::TYPE::SWORD, temp_wpn_stats, ITEM::NAME::RAW_IRON_SLAB); //Should be found inside a dragon skull?
                temp_wpn.setDescription("Barely a sword. Too big, too thick, too massive, too rough. Tempered by the malevolence of countless slain demons. Effective against demons and angels.");
                temp_wpn.setEffective(UNIT::TYPE::DEMON + UNIT::TYPE::ANGEL);
                wpn_out[ITEM::NAME::RAW_IRON_SLAB] = temp_wpn;
                // Should have deeps chinks. This isn't the first time demons roamed the earth. Has deep holes in it. Must be repaired before being usable. Who is willing to repair it?
                break;
            case ITEM::NAME::KIRITO:
                temp_wpn_stats = {0, 0, 0, 0, 0, 0, 0, 10, 30, {1, 1}, {1, 2}, 0, 0};
                temp_wpn = Weapon(ITEM::TYPE::SWORD, temp_wpn_stats, ITEM::NAME::KIRITO); //Should be found inside a dragon skull?
                temp_wpn.setDescription("Useless sword of a fake black swordsman.");
                wpn_out[ITEM::NAME::KIRITO] = temp_wpn;
                break;
            case ITEM::NAME::KILLY:
                break;

            case ITEM::NAME::PITCHFORK:
                temp_wpn_stats = {2, 0, 60, 0, 0, 0, 4, 15, 1, {1, 1}, {1, 2}, 0, 200, 0};
                temp_wpn = Weapon(ITEM::TYPE::LANCE, temp_wpn_stats, ITEM::NAME::PITCHFORK);
                temp_wpn.setDescription("Farming implement. Makeshift lance for desperate times.");
                wpn_out[ITEM::NAME::PITCHFORK] = temp_wpn;
                break;
            case ITEM::NAME::FIGHTING_STICK:
                temp_wpn_stats = {3, 0, 70, 0, 0, 0, 3, 30, 5, {1, 1}, {2}, 0, 240, 0};
                temp_wpn = Weapon(ITEM::TYPE::LANCE, temp_wpn_stats, ITEM::NAME::FIGHTING_STICK);
                temp_wpn.setDescription("Simple two meter long wooden stick. Deadly in competent hands.");
                wpn_out[ITEM::NAME::FIGHTING_STICK] = temp_wpn;
                break;
            case ITEM::NAME::IRON_LANCE:
                temp_wpn_stats = {5, 0, 95, 0, 0, 0, 5, 40, 4, {1, 1}, {1, 2}, 0, 520, 0};
                temp_wpn = Weapon(ITEM::TYPE::LANCE, temp_wpn_stats, ITEM::NAME::IRON_LANCE);
                temp_wpn.setDescription("Sharp iron tip on a wooden stick. Standard military fare.");
                wpn_out[ITEM::NAME::IRON_LANCE] = temp_wpn;
                break;
            case ITEM::NAME::WRATH_LANCE:
                temp_wpn_stats = {7, 0, 80, 0, 30, 0, 3, 30, 9, {1, 1}, {1, 2}, 0, 700, 0};
                temp_wpn = Weapon(ITEM::TYPE::LANCE, temp_wpn_stats, ITEM::NAME::WRATH_LANCE);
                temp_wpn.setDescription("Increases critical hit chance.");
                wpn_out[ITEM::NAME::WRATH_LANCE] = temp_wpn;
                break;
            case ITEM::NAME::STEEL_LANCE:
                temp_wpn_stats = {8, 0, 90, 0, 0, 0, 9, 35, 7, {1, 1}, {1, 2}, 0, 888, 0};
                temp_wpn = Weapon(ITEM::TYPE::LANCE, temp_wpn_stats, ITEM::NAME::STEEL_LANCE);
                temp_wpn.setDescription("Stronger and deadlier.");
                wpn_out[ITEM::NAME::STEEL_LANCE] = temp_wpn;
                break;
            case ITEM::NAME::DAMAS_LANCE:
                temp_wpn_stats = {12, 0, 85, 0, 0, 0, 12, 25, 13, {1, 1}, {1, 2}, 0, 1100, 0};
                temp_wpn = Weapon(ITEM::TYPE::LANCE, temp_wpn_stats, ITEM::NAME::DAMAS_LANCE);
                temp_wpn.setDescription("Fancy and fatal.");
                wpn_out[ITEM::NAME::DAMAS_LANCE] = temp_wpn;
                break;
            case ITEM::NAME::HEAVY_SPEAR:
                temp_wpn_stats = {6, 0, 60, 0, 0, 0, 15, 10, 8, {1, 1}, {1, 2}, 0, 650, 0};
                temp_wpn = Weapon(ITEM::TYPE::LANCE, temp_wpn_stats, ITEM::NAME::HEAVY_SPEAR);
                temp_wpn.setEffective(UNIT::TYPE::ARMOR);
                temp_wpn.setDescription("Heavy enough to pierce armor.");
                wpn_out[ITEM::NAME::HEAVY_SPEAR] = temp_wpn;
                break;
            case ITEM::NAME::TRIDENT:
                temp_wpn_stats = {8, 0, 90, 0, 0, 0, 8, 10, 10, {1, 1}, {1, 2}, 0, 1040, 0};
                temp_wpn = Weapon(ITEM::TYPE::LANCE, temp_wpn_stats, ITEM::NAME::TRIDENT);
                temp_wpn.setDescription("Fancy pitchfork");
                wpn_out[ITEM::NAME::TRIDENT] = temp_wpn;
                break;
            case ITEM::NAME::MERCIFUL_LANCE:
                temp_wpn_stats = {6, 0, 70, 0, 0, 0, 7, 20, 7, {1, 1}, {1, 2}, 0, 910, 0};
                temp_wpn = Weapon(ITEM::TYPE::LANCE, temp_wpn_stats, ITEM::NAME::MERCIFUL_LANCE);
                temp_wpn.setDescription("Infused with the life-giving tears of the goddess. Cannot reduce enemy HP below 1.");
                wpn_out[ITEM::NAME::MERCIFUL_LANCE] = temp_wpn;
                break;
            case ITEM::NAME::JAVELIN:
                temp_wpn_stats = {4, 0, 40, 0, 0, 0, 6, 25, 9, {1, 2}, {1}, 0, 750, 0};
                temp_wpn = Weapon(ITEM::TYPE::LANCE, temp_wpn_stats, ITEM::NAME::JAVELIN);
                temp_wpn.setDescription("Simple throwing lance.");
                wpn_out[ITEM::NAME::JAVELIN] = temp_wpn;
                break;
            case ITEM::NAME::PILUM:
                temp_wpn_stats = {6, 0, 45, 0, 0, 0, 8, 23, 10, {1, 2}, {1}, 0, 990, 0};
                temp_wpn = Weapon(ITEM::TYPE::LANCE, temp_wpn_stats, ITEM::NAME::PILUM);
                temp_wpn.setDescription("Reliable throwing lance.");
                wpn_out[ITEM::NAME::PILUM] = temp_wpn;
                break;
            case ITEM::NAME::SPEAR:
                temp_wpn_stats = {9, 0, 50, 0, 0, 0, 11, 20, 11, {1, 2}, {1}, 0, 1300, 0};
                temp_wpn = Weapon(ITEM::TYPE::LANCE, temp_wpn_stats, ITEM::NAME::SPEAR);
                temp_wpn.setDescription("Strong throwing Lance. Strikes true.");
                wpn_out[ITEM::NAME::SPEAR] = temp_wpn;
                break;
            case ITEM::NAME::VEL:
                temp_wpn_stats = {14, 0, 60, 0, 0, 0, 9, 25, 15, {1, 2}, {1, 2}, 0, 2000, 0};
                temp_wpn = Weapon(ITEM::TYPE::LANCE, temp_wpn_stats, ITEM::NAME::VEL);
                temp_wpn.setDescription("Legendary throwing lance. Said to be able to home in on the heart of your enemies.");
                wpn_out[ITEM::NAME::VEL] = temp_wpn;
                break;
            case ITEM::NAME::GUNGNIR:
                temp_wpn_stats = {8, 0, 90, 0, 0, 0, 6, 20, 9, {1, 2}, {1, 2}, 1, 3000, 0};
                temp_wpn = Weapon(ITEM::TYPE::LANCE, temp_wpn_stats, ITEM::NAME::GUNGNIR);
                temp_wpn.setDescription("A lightning bolt resides in its tip. Can shoot lightning at enemies at a distance.");
                wpn_out[ITEM::NAME::GUNGNIR] = temp_wpn;
                break;
            case ITEM::NAME::ACHILLES:
                temp_wpn_stats = {12, 0, 70, 0, 0, 0, 10, 32, 18, {1, 2}, {1, 2}, 0, 4000, 0};
                temp_wpn = Weapon(ITEM::TYPE::LANCE, temp_wpn_stats, ITEM::NAME::ACHILLES);
                temp_wpn.setEffect(ITEM::EFFECT::KILL1P);
                temp_wpn.setDescription("Legendary throwing spear named after a fearless, immortal hero. Kills instantly if hits enemy's heel (1%).");
                wpn_out[ITEM::NAME::ACHILLES] = temp_wpn;
                break;
            case ITEM::NAME::TONBOKIRI:
                temp_wpn_stats = {16, 0, 50, 0, 0, 0, 14, 24, 17, {1, 1}, {1, 2}, 0, 5000, 0};
                temp_wpn = Weapon(ITEM::TYPE::LANCE, temp_wpn_stats, ITEM::NAME::TONBOKIRI);
                temp_wpn.setDescription("Cut a dragonfly that tried to land on its tip. Excessively sharp and dangerous.");
                wpn_out[ITEM::NAME::TONBOKIRI] = temp_wpn;
                break;

            case ITEM::NAME::IRON_AXE:
                temp_wpn_stats = {8, 0, 40, 0, 0, 0, 8, 40, 4, {1, 1}, {1, 2}, 0, 360, 0};
                temp_wpn = Weapon(ITEM::TYPE::AXE, temp_wpn_stats, ITEM::NAME::IRON_AXE);
                temp_wpn.setDescription("Iron cutting head on a stick. Cheap, simple, easy to repair. Bandits' favorite.");
                wpn_out[ITEM::NAME::IRON_AXE] = temp_wpn;
                break;
            case ITEM::NAME::STEEL_AXE:
                temp_wpn_stats = {11, 0, 50, 0, 0, 0, 10, 35, 7, {1, 1}, {1, 2}, 0, 720, 0};
                temp_wpn = Weapon(ITEM::TYPE::AXE, temp_wpn_stats, ITEM::NAME::STEEL_AXE);
                temp_wpn.setDescription("Steel cutting head on a stick. Stronger. Smart bandits like it.");
                wpn_out[ITEM::NAME::STEEL_AXE] = temp_wpn;
                break;
            case ITEM::NAME::DAMAS_AXE:
                temp_wpn_stats = {14, 0, 55, 0, 0, 0, 13, 25, 11, {1, 1}, {1, 2}, 0, 1080, 0};
                temp_wpn = Weapon(ITEM::TYPE::AXE, temp_wpn_stats, ITEM::NAME::DAMAS_AXE);
                temp_wpn.setDescription("Damas cutting head on a stick. Fancy and deadly. Scorned by dumb bandits.");
                wpn_out[ITEM::NAME::DAMAS_AXE] = temp_wpn;
                break;
            case ITEM::NAME::WRATH_AXE:
                temp_wpn_stats = {10, 0, 65, 0, 20, 0, 11, 30, 10, {1, 1}, {1, 2}, 0, 920, 0};
                temp_wpn = Weapon(ITEM::TYPE::AXE, temp_wpn_stats, ITEM::NAME::WRATH_AXE);
                temp_wpn.setDescription("Increases critical hit chance.");
                wpn_out[ITEM::NAME::WRATH_AXE] = temp_wpn;
                break;
            case ITEM::NAME::THROWING_AXE:
                temp_wpn_stats = {6, 0, 30, 0, 0, 0, 7, 25, 5, {1, 2}, {1, 2}, 0, 450, 0};
                temp_wpn = Weapon(ITEM::TYPE::AXE, temp_wpn_stats, ITEM::NAME::THROWING_AXE);
                temp_wpn.setDescription("Smaller, balanced axe, made for throwing. Cheapest option for ranged attacks.");
                wpn_out[ITEM::NAME::THROWING_AXE] = temp_wpn;
                break;
            case ITEM::NAME::TOMAHAWK:
                temp_wpn_stats = {10, 0, 40, 0, 0, 0, 7, 25, 10, {1, 2}, {1, 2}, 0, 790, 0};
                temp_wpn = Weapon(ITEM::TYPE::AXE, temp_wpn_stats, ITEM::NAME::TOMAHAWK);
                temp_wpn.setDescription("Smaller, balanced axe, made for throwing. Cheapest option for ranged attacks.");
                wpn_out[ITEM::NAME::TOMAHAWK] = temp_wpn;
                break;
            case ITEM::NAME::PICK_AXE:
                temp_wpn_stats = {7, 0, 60, 0, 0, 0, 9, 20, 3, {1, 1}, {1, 2}, 0, 650, 0};
                temp_wpn = Weapon(ITEM::TYPE::AXE, temp_wpn_stats, ITEM::NAME::PICK_AXE);
                temp_wpn.setDescription("Mining implement. Weapon in time of need.");
                wpn_out[ITEM::NAME::PICK_AXE] = temp_wpn;
                break;
            case ITEM::NAME::HOE:
                temp_wpn_stats = {4, 0, 40, 0, 0, 0, 5, 25, 2, {1, 1}, {1, 2}, 0, 200, 0};
                temp_wpn = Weapon(ITEM::TYPE::AXE, temp_wpn_stats, ITEM::NAME::HOE);
                temp_wpn.setDescription("Farming implement. Can be used as an axe if need be.");
                wpn_out[ITEM::NAME::HOE] = temp_wpn;
                break;
            case ITEM::NAME::CLUB:
                temp_wpn_stats = {9, 0, 50, 5, 0, 0, 6, 15, 1, {1, 1}, {1, 2}, 0, 100, 0};
                temp_wpn = Weapon(ITEM::TYPE::AXE, temp_wpn_stats, ITEM::NAME::CLUB);
                temp_wpn.setDescription("Big and wooden. For when the old-world weapons just aren't quite old enough.");
                wpn_out[ITEM::NAME::CLUB] = temp_wpn;
                break;
            case ITEM::NAME::HALBERD:
                temp_wpn_stats = {10, 0, 50, 0, 0, 0, 11, 22, 12, {1, 1}, {1, 2}, 0, 1110, 0};
                temp_wpn = Weapon(ITEM::TYPE::AXE + ITEM::TYPE::LANCE, temp_wpn_stats, ITEM::NAME::HALBERD);
                temp_wpn.setEffective(UNIT::TYPE::MOUNTED);
                temp_wpn.setDescription("Polearm specially designed to deal with cavalry. Is both a lance and an axe.");
                wpn_out[ITEM::NAME::HALBERD] = temp_wpn;
                break;
            case ITEM::NAME::HAMMER:
                temp_wpn_stats = {8, 0, 40, 0, 0, 0, 9, 30, 8, {1, 1}, {1, 2}, 0, 690, 0};
                temp_wpn = Weapon(ITEM::TYPE::AXE, temp_wpn_stats, ITEM::NAME::HAMMER);
                temp_wpn.setEffective(UNIT::TYPE::ARMOR);
                temp_wpn.setDescription("Armor cannot stand the hammer's heavy crushing blows.");
                wpn_out[ITEM::NAME::HAMMER] = temp_wpn;
                break;
            case ITEM::NAME::WAR_PICK:
                temp_wpn_stats = {12, 0, 50, 0, 0, 0, 12, 25, 11, {1, 1}, {1, 2}, 0, 1310, 0};
                temp_wpn = Weapon(ITEM::TYPE::AXE, temp_wpn_stats, ITEM::NAME::WAR_PICK);
                temp_wpn.setEffective(UNIT::TYPE::ARMOR);
                temp_wpn.setDescription("In trained hands, the pick can penatrate armor.");
                wpn_out[ITEM::NAME::WAR_PICK] = temp_wpn;
                break;
            case ITEM::NAME::BATTLEAXE:
                temp_wpn_stats = {15, 0, 45, 0, 0, 0, 14, 30, 14, {1, 1}, {1, 2}, 0, 1800, 0};
                temp_wpn = Weapon(ITEM::TYPE::AXE, temp_wpn_stats, ITEM::NAME::BATTLEAXE);
                temp_wpn.setDescription("Double-bladed, comically oversized axe.");
                wpn_out[ITEM::NAME::BATTLEAXE] = temp_wpn;
                break;
            case ITEM::NAME::VIDYU:
                temp_wpn_stats = {17, 0, 60, 0, 0, 0, 17, 20, 17, {1, 1}, {1, 2}, 0, 2700, 0};
                temp_wpn = Weapon(ITEM::TYPE::AXE, temp_wpn_stats, ITEM::NAME::VIDYU);
                temp_wpn.setDescription("Battleaxe of the many-headed and armed gods of the east.");
                wpn_out[ITEM::NAME::VIDYU] = temp_wpn;
                break;
            case ITEM::NAME::HEPHAESTUS:
                temp_wpn_stats = {19, 0, 70, 0, 0, 0, 5, 60, 20, {1, 1}, {1, 2}, 0, 10000, 0};
                temp_wpn = Weapon(ITEM::TYPE::AXE, temp_wpn_stats, ITEM::NAME::HEPHAESTUS);
                temp_wpn.setDescription("The god's hammer. Used to forge Secundus, the first sword. Basically a vaguely square stone fixed to a wooden handle.");
                wpn_out[ITEM::NAME::HEPHAESTUS] = temp_wpn;
                break;

            case ITEM::NAME::ADAPTED_CROSSBOW:
                temp_wpn_stats = {12, 0, 70, 0, 0, 0, 14, 35, 9, {1, 1}, {1}, 0, 2500, 0};
                temp_wpn = Weapon(ITEM::TYPE::BOW, temp_wpn_stats, ITEM::NAME::ADAPTED_CROSSBOW);
                temp_wpn.setDescription("Crossbow adapted to be fired, reloaded and drawn using one hand and one stump. 1H assassin only.");
                wpn_out[ITEM::NAME::ADAPTED_CROSSBOW] = temp_wpn;
                break;
            case ITEM::NAME::REPEATABLE_CROSSBOW:
                temp_wpn_stats = {8, 0, 60, 0, 0, 0, 18, 28, 14, {1, 1}, {1}, 0, 2010, 0};
                temp_wpn = Weapon(ITEM::TYPE::BOW, temp_wpn_stats, ITEM::NAME::REPEATABLE_CROSSBOW);
                temp_wpn.setDescription("Crossbow that fires many bolts every attack.");
                wpn_out[ITEM::NAME::REPEATABLE_CROSSBOW] = temp_wpn;
                break;
            case ITEM::NAME::CROSSBOW:
                temp_wpn_stats = {10, 0, 85, 0, 0, 0, 12, 15, 7, {1, 1}, {1}, 0, 1055, 0};
                temp_wpn = Weapon(ITEM::TYPE::BOW, temp_wpn_stats, ITEM::NAME::CROSSBOW);
                temp_wpn.setDescription("Powerful and easier to use than a bow, although quite heavy.");
                wpn_out[ITEM::NAME::CROSSBOW] = temp_wpn;
                break;
            case ITEM::NAME::SHORT_BOW:
                temp_wpn_stats = {4, 0, 90, 0, 0, 0, 4, 35, 5, {1, 2}, {1}, 0, 400, 0};
                temp_wpn = Weapon(ITEM::TYPE::BOW, temp_wpn_stats, ITEM::NAME::SHORT_BOW);
                temp_wpn.setDescription("Simple and light bow.");
                wpn_out[ITEM::NAME::SHORT_BOW] = temp_wpn;
                break;
            case ITEM::NAME::LONG_BOW:
                temp_wpn_stats = {8, 0, 70, 0, 0, 0, 10, 25, 11, {1, 3}, {1}, 0, 800, 0};
                temp_wpn = Weapon(ITEM::TYPE::BOW, temp_wpn_stats, ITEM::NAME::LONG_BOW);
                temp_wpn.setDescription("Long and heavy bow. Has increased range.");
                wpn_out[ITEM::NAME::LONG_BOW] = temp_wpn;
                break;
            case ITEM::NAME::COMPOSITE_BOW:
                temp_wpn_stats = {7, 0, 80, 0, 0, 0, 5, 30, 10, {1, 1}, {1}, 0, 620, 0};
                temp_wpn = Weapon(ITEM::TYPE::BOW, temp_wpn_stats, ITEM::NAME::COMPOSITE_BOW);
                temp_wpn.setDescription("Bow made of laminated wood and animal horn. Very useful for horseback shooting.");
                wpn_out[ITEM::NAME::COMPOSITE_BOW] = temp_wpn;
                break;
            case ITEM::NAME::RECURVE_CROSSBOW:
                temp_wpn_stats = {18, 0, 50, 0, 0, 0, 19, 20, 11, {1, 1}, {1}, 0, 760, 0};
                temp_wpn = Weapon(ITEM::TYPE::BOW, temp_wpn_stats, ITEM::NAME::RECURVE_CROSSBOW);
                temp_wpn.setDescription("Crossbow with recurve limbs. Heavy and powerful.");
                wpn_out[ITEM::NAME::RECURVE_CROSSBOW] = temp_wpn;
                break;
            case ITEM::NAME::RECURVE_BOW:
                temp_wpn_stats = {13, 0, 75, 0, 0, 0, 10, 24, 14, {1, 1}, {1}, 0, 901, 0};
                temp_wpn = Weapon(ITEM::TYPE::BOW, temp_wpn_stats, ITEM::NAME::RECURVE_BOW);
                temp_wpn.setDescription("Bow whose limbs curve away when unstrung. Shoots faster and deadlier arrows.");
                wpn_out[ITEM::NAME::RECURVE_BOW] = temp_wpn;
                break;
            case ITEM::NAME::APOLLO:
                temp_wpn_stats = {19, 0, 69, 0, 0, 0, 9, 25, 17, {1, 1}, {1}, 0, 8000, 0};
                temp_wpn = Weapon(ITEM::TYPE::BOW, temp_wpn_stats, ITEM::NAME::APOLLO);
                temp_wpn.setDescription("Bow crafted of sun rays, used by the most handsome ancient god.");
                wpn_out[ITEM::NAME::APOLLO] = temp_wpn;
                break;
            case ITEM::NAME::ARTEMIS:
                temp_wpn_stats = {18, 0, 96, 0, 0, 0, 4, 26, 19, {1, 1}, {1}, 0, 7007, 0};
                temp_wpn = Weapon(ITEM::TYPE::BOW, temp_wpn_stats, ITEM::NAME::ARTEMIS);
                temp_wpn.setDescription("Bow of the ancient goddess of hunting.");
                wpn_out[ITEM::NAME::ARTEMIS] = temp_wpn;
                break;
            case ITEM::NAME::PINAKA:
                temp_wpn_stats = {17, 0, 100, 0, 0, 0, 7, 35, 16, {1, 1}, {1}, 0, 8008, 0};
                temp_wpn = Weapon(ITEM::TYPE::BOW, temp_wpn_stats, ITEM::NAME::PINAKA);
                temp_wpn.setDescription("Bow of the god of death.");
                wpn_out[ITEM::NAME::PINAKA] = temp_wpn;
                break;

            case ITEM::NAME::ADAPTED_SHIELD:
                temp_wpn_stats = {5, 0, 0, 10, 0, 5, 4, 45, 8, {1, 1}, {1}, 0, 3003, 0};
                temp_wpn = Weapon(ITEM::TYPE::SHIELD, temp_wpn_stats, ITEM::NAME::ADAPTED_SHIELD);
                temp_wpn.setDescription("Shield adapted to be used without one\'s weak hand. Straps on the forearm. 1H assassin only.");
                wpn_out[ITEM::NAME::ADAPTED_SHIELD] = temp_wpn;
                break;
            case ITEM::NAME::POT_LID:
                temp_wpn_stats = {1, 0, 0, 5, 0, 1, 2, 15, 2, {1, 1}, {1}, 0, 111, 0};
                temp_wpn = Weapon(ITEM::TYPE::SHIELD, temp_wpn_stats, ITEM::NAME::POT_LID);
                temp_wpn.setDescription("Useful in the kitchen. For the poor militia man: life saving device.");
                wpn_out[ITEM::NAME::POT_LID] = temp_wpn;
                break;
            case ITEM::NAME::WOODEN_SHIELD:
                temp_wpn_stats = {2, 0, 0, 8, 0, 0, 3, 20, 7, {1, 1}, {1}, 0, 222, 0};
                temp_wpn = Weapon(ITEM::TYPE::SHIELD, temp_wpn_stats, ITEM::NAME::WOODEN_SHIELD);
                temp_wpn.setDescription("Simple wood plank with a strap. Will probably save your life.");
                wpn_out[ITEM::NAME::WOODEN_SHIELD] = temp_wpn;
                break;
            case ITEM::NAME::CLOAK:
                temp_wpn_stats = {4, 0, 0, 10, 0, 4, 5, 0, 13, {1, 1}, {1}, 0, 10, 0};
                temp_wpn = Weapon(ITEM::TYPE::SHIELD, temp_wpn_stats, ITEM::NAME::CLOAK);
                temp_wpn.setDescription("The Mousquetaire\"s offhand shield in time of need.");
                wpn_out[ITEM::NAME::CLOAK] = temp_wpn;
                break;
            case ITEM::NAME::LEATHER_SHIELD:
                temp_wpn_stats = {3, 0, 0, 9, 0, 2, 4, 25, 9, {1, 1}, {1}, 0, 440, 0};
                temp_wpn = Weapon(ITEM::TYPE::SHIELD, temp_wpn_stats, ITEM::NAME::LEATHER_SHIELD);
                temp_wpn.setDescription("Wooden plank lined with leather. Smooth and slippery. Cheap way to redirect weapons away from the body.");
                wpn_out[ITEM::NAME::LEATHER_SHIELD] = temp_wpn;
                break;
            case ITEM::NAME::KITE_SHIELD:
                temp_wpn_stats = {8, 0, 0, 15, 0, 4, 4, 35, 12, {1, 1}, {1}, 0, 810, 0};
                temp_wpn = Weapon(ITEM::TYPE::SHIELD, temp_wpn_stats, ITEM::NAME::KITE_SHIELD);
                temp_wpn.setDescription("Large wooden shield of recognizable shape, to increase protection.");
                wpn_out[ITEM::NAME::KITE_SHIELD] = temp_wpn;
                break;
            case ITEM::NAME::IRON_BUCKLER:
                temp_wpn_stats = {6, 0, 0, 6, 0, 6, 6, 36, 6, {1, 1}, {1}, 0, 630, 0};
                temp_wpn = Weapon(ITEM::TYPE::SHIELD, temp_wpn_stats, ITEM::NAME::IRON_BUCKLER);
                temp_wpn.setDescription("Small round iron shield. Light, resistant.");
                wpn_out[ITEM::NAME::IRON_BUCKLER] = temp_wpn;
                break;
            case ITEM::NAME::STEEL_BUCKLER:
                temp_wpn_stats = {7, 0, 0, 7, 0, 7, 7, 37, 7, {1, 1}, {1}, 0, 940, 0};
                temp_wpn = Weapon(ITEM::TYPE::SHIELD, temp_wpn_stats, ITEM::NAME::STEEL_BUCKLER);
                temp_wpn.setDescription("Small round steel shield. Heavier and more resistant.");
                wpn_out[ITEM::NAME::STEEL_BUCKLER] = temp_wpn;
                break;
            case ITEM::NAME::SPIKED_BUCKLER:
                temp_wpn_stats = {5, 0, 0, 5, 0, 5, 9, 30, 10, {1, 1}, {1}, 0, 1100, 0};
                temp_wpn = Weapon(ITEM::TYPE::SHIELD, temp_wpn_stats, ITEM::NAME::SPIKED_BUCKLER);
                temp_wpn.setDescription("Iron buckler with a prickly spike at its center. Damages attackers upon their attacks.");
                wpn_out[ITEM::NAME::SPIKED_BUCKLER] = temp_wpn;
                break;
            case ITEM::NAME::SCUTUM:
                temp_wpn_stats = {11, 0, 0, 15, 0, 5, 13, 25, 7, {1, 1}, {1}, 0, 1440, 0};
                temp_wpn = Weapon(ITEM::TYPE::SHIELD, temp_wpn_stats, ITEM::NAME::SCUTUM);
                temp_wpn.setDescription("Very large and heavy shield.");
                wpn_out[ITEM::NAME::SCUTUM] = temp_wpn;
                break;
            case ITEM::NAME::MASTER_SHIELD:
                temp_wpn_stats = {12, 0, 0, 20, 0, 10, 10, 0, 14, {1, 1}, {1}, 0, 80, 0};
                temp_wpn = Weapon(ITEM::TYPE::SHIELD, temp_wpn_stats, ITEM::NAME::MASTER_SHIELD);
                temp_wpn.setDescription("Shield of the princess of time.");
                wpn_out[ITEM::NAME::MASTER_SHIELD] = temp_wpn;
                break;
            case ITEM::NAME::GODDESS_HAND:
                temp_wpn_stats = {9, 0, 0, 5, 0, 200, 9, 22, 13, {1, 1}, {1}, 0, 420, 0};
                temp_wpn = Weapon(ITEM::TYPE::SHIELD, temp_wpn_stats, ITEM::NAME::GODDESS_HAND);
                temp_wpn.setDescription("Offers the protection of the goddess. Negates enemy critical hits.");
                wpn_out[ITEM::NAME::GODDESS_HAND] = temp_wpn;
                break;
            case ITEM::NAME::ANCILE:
                temp_wpn_stats = {15, 0, 0, 10, 0, 2, 13, 19, 12, {1, 1}, {1}, 0, 30000, 0};
                temp_wpn = Weapon(ITEM::TYPE::SHIELD, temp_wpn_stats, ITEM::NAME::ANCILE);
                temp_wpn.setDescription("Shield of the ancient god of war.");
                wpn_out[ITEM::NAME::ANCILE] = temp_wpn;
                break;
            case ITEM::NAME::ROCK_PLATE:
                temp_wpn_stats = {15, 0, 0, 10, 0, 2, 13, 19, 12, {1, 1}, {1}, 0, 0, 0};
                temp_wpn = Weapon(ITEM::TYPE::SHIELD, temp_wpn_stats, ITEM::NAME::ROCK_PLATE);
                temp_wpn.setDescription("Almost a raw boulder. The powerful and dumb demon's favorite.");
                wpn_out[ITEM::NAME::ROCK_PLATE] = temp_wpn;
                break;
            case ITEM::NAME::OBSIDIAN_PLATE:
                temp_wpn_stats = {15, 0, 0, 10, 0, 2, 13, 19, 12, {1, 1}, {1}, 0, 1, 0};
                temp_wpn = Weapon(ITEM::TYPE::SHIELD, temp_wpn_stats, ITEM::NAME::OBSIDIAN_PLATE);
                temp_wpn.setDescription("Rock melted by demon-breath, then recystallized into a more demon-claw friendly shape. Even harder than rock, but more brittle.");
                wpn_out[ITEM::NAME::OBSIDIAN_PLATE] = temp_wpn;
                break;
            case ITEM::NAME::MARBLE_PLATE_SHIELD:
                temp_wpn_stats = {15, 0, 0, 10, 0, 2, 13, 19, 12, {1, 1}, {1}, 0, 3000, 0};
                temp_wpn = Weapon(ITEM::TYPE::SHIELD, temp_wpn_stats, ITEM::NAME::MARBLE_PLATE_SHIELD);
                temp_wpn.setDescription("Kite shield lined in an ornate layer of sculpted marble. Magical.");
                wpn_out[ITEM::NAME::MARBLE_PLATE_SHIELD] = temp_wpn;
                break;
            case ITEM::NAME::DIVINE_PROTECTOR:
                temp_wpn_stats = {8, 8, 0, 10, 0, 2, 11, 22, 12, {1, 1}, {1}, 0, 4000, 0};
                temp_wpn = Weapon(ITEM::TYPE::SHIELD, temp_wpn_stats, ITEM::NAME::DIVINE_PROTECTOR);
                temp_wpn.setDescription("Large tower shield imbued with angelic properties. Surprinsgly light and effective against magic attacks.");
                wpn_out[ITEM::NAME::DIVINE_PROTECTOR] = temp_wpn;
                break;

            case ITEM::NAME::WRISTBLADE:
                temp_wpn_stats = {2, 0, 10, 0, 10, 0, 5, 21, 16, {1, 1}, {1}, 0, 925, 0};
                temp_wpn = Weapon(ITEM::TYPE::OFFHAND, temp_wpn_stats, ITEM::NAME::WRISTBLADE);
                temp_wpn.setDescription("Blade generally strapped to the weak forearm. Used by assassins to increase crit potential.");
                wpn_out[ITEM::NAME::WRISTBLADE] = temp_wpn;
                break;
            case ITEM::NAME::SHIELD_HOOK:
                temp_wpn_stats = {2, 0, 10, 0, 10, 0, 5, 21, 16, {1, 1}, {1}, 0, 830, 0};
                temp_wpn = Weapon(ITEM::TYPE::OFFHAND, temp_wpn_stats, ITEM::NAME::SHIELD_HOOK);
                temp_wpn.setDescription("Offhand weapon designed to grab on shields to drag them aside. Ignores shield bonuses.");
                temp_wpn.setEffect(ITEM::EFFECT::IGNORE_SHIELD);
                wpn_out[ITEM::NAME::SHIELD_HOOK] = temp_wpn;
                break;
            case ITEM::NAME::RETRACTABLE_WRISTBLADE:
                temp_wpn_stats = {4, 0, 8, 0, 14, 0, 7, 16, 19, {1, 1}, {1}, 0, 1025, 0};
                temp_wpn = Weapon(ITEM::TYPE::OFFHAND, temp_wpn_stats, ITEM::NAME::RETRACTABLE_WRISTBLADE);
                temp_wpn.setDescription("Blade generally strapped to the weak forearm. Controlled by hand movements. Used by assasins to stealthily increase crit potential.");
                wpn_out[ITEM::NAME::RETRACTABLE_WRISTBLADE] = temp_wpn;
                break;
            case ITEM::NAME::MAIN_GAUCHE:
                temp_wpn_stats = {1, 0, 0, 10, 0, 5, 4, 35, 7, {1, 1}, {1}, 0, 825, 0};
                temp_wpn = Weapon(ITEM::TYPE::OFFHAND, temp_wpn_stats, ITEM::NAME::MAIN_GAUCHE);
                temp_wpn.setDescription("Dagger designed to parry using the left/weak/off-hand. Increases avoir and favor.");
                wpn_out[ITEM::NAME::MAIN_GAUCHE] = temp_wpn;
                break;
            case ITEM::NAME::SWORDBREAKER:
                temp_wpn_stats = {2, 0, 0, 22, 0, 4, 6, 28, 12, {1, 1}, {1}, 0, 612, 0};
                temp_wpn = Weapon(ITEM::TYPE::OFFHAND, temp_wpn_stats, ITEM::NAME::SWORDBREAKER);
                temp_wpn.setDescription("Dagger with slots designed to catch swords. Increases avoid and favor. Rarely does it actually break swords...");
                wpn_out[ITEM::NAME::SWORDBREAKER] = temp_wpn;
                break;
            case ITEM::NAME::TRIDENT_DAGGER:
                temp_wpn_stats = {3, 0, 14, 0, 8, 0, 4, 20, 10, {1, 1}, {1}, 0, 724, 0};
                temp_wpn = Weapon(ITEM::TYPE::OFFHAND, temp_wpn_stats, ITEM::NAME::TRIDENT_DAGGER);
                temp_wpn.setDescription("Dagger whose blade splits into three prongs. Designed to catch opposing swords. Increases avoid and favor.");
                wpn_out[ITEM::NAME::TRIDENT_DAGGER] = temp_wpn;
                break;
            case ITEM::NAME::TANTOU:
                temp_wpn_stats = {3, 0, 5, 0, 0, 0, 2, 26, 8, {1, 1}, {1}, 0, 852, 0};
                temp_wpn = Weapon(ITEM::TYPE::OFFHAND, temp_wpn_stats, ITEM::NAME::TANTOU);
                temp_wpn.setDescription("Eastern style short dagger. Very sharp and dangerous. Increases might.");
                wpn_out[ITEM::NAME::TANTOU] = temp_wpn;
                break;
            case ITEM::NAME::KODACHI:
                temp_wpn_stats = {5, 0, 8, 0, 0, 0, 4, 24, 11, {1, 1}, {1}, 0, 992, 0};
                temp_wpn = Weapon(ITEM::TYPE::OFFHAND, temp_wpn_stats, ITEM::NAME::KODACHI);
                temp_wpn.setDescription("Dagger used by the legendary assasins of the East, the Ninja. Increases might.");
                wpn_out[ITEM::NAME::KODACHI] = temp_wpn;
                break;
            case ITEM::NAME::DAGUE:
                temp_wpn_stats = {4, 0, 10, 0, 0, 0, 3, 20, 10, {1, 1}, {1}, 0, 880, 0};
                temp_wpn = Weapon(ITEM::TYPE::OFFHAND, temp_wpn_stats, ITEM::NAME::DAGUE);
                temp_wpn.setDescription("Messenic dagger. Simple and efficient. Beautiful engravings adorn the blade surface. ");
                wpn_out[ITEM::NAME::DAGUE] = temp_wpn;
                break;
            case ITEM::NAME::WAKIZASHI:
                temp_wpn_stats = {7, 0, 0, 0, 0, 0, 8, 10, 15, {1, 1}, {1}, 0, 1300, 0};
                temp_wpn = Weapon(ITEM::TYPE::OFFHAND, temp_wpn_stats, ITEM::NAME::WAKIZASHI);
                temp_wpn.setDescription("Dagger of the warriors of the East. Companion of the uchigatana. Very deadly.");
                wpn_out[ITEM::NAME::WAKIZASHI] = temp_wpn;
                break;
            case ITEM::NAME::BRAQUEMARD:
                temp_wpn_stats = {2, 0, 0, 0, 0, 0, 4, 15, 7, {1, 1}, {1}, 0, 2400, 0};
                temp_wpn = Weapon(ITEM::TYPE::OFFHAND, temp_wpn_stats, ITEM::NAME::BRAQUEMARD);
                temp_wpn.setDescription("Legendary Dagger.");
                wpn_out[ITEM::NAME::BRAQUEMARD] = temp_wpn;
                break;
            case ITEM::NAME::MISERICORDE:
                temp_wpn_stats = {0, 0, 0, 0, 0, 0, 7, 30, 10, {1, 1}, {1}, 0, 1500, 0};
                temp_wpn = Weapon(ITEM::TYPE::OFFHAND, temp_wpn_stats, ITEM::NAME::MISERICORDE);
                temp_wpn.setDescription("Used to delivery the Coup de grace to wounded knights. Might of this dagger increases by one for each missing 3 health.");
                wpn_out[ITEM::NAME::MISERICORDE] = temp_wpn;
                break;
            case ITEM::NAME::CARNWENNAN:
                temp_wpn_stats = {8, 0, 10, 10, 0, 2, 9, 24, 17, {1, 1}, {1}, 0, 3312, 0};
                temp_wpn = Weapon(ITEM::TYPE::OFFHAND, temp_wpn_stats, ITEM::NAME::CARNWENNAN);
                temp_wpn.setDescription("The king\'s dagger. Has the power to shroud its user in shadow.");
                wpn_out[ITEM::NAME::CARNWENNAN] = temp_wpn;
                break;
            case ITEM::NAME::FLINTLOCK:
                temp_wpn_stats = {7, 0, 0, 0, 0, 0, 7, 8, 5, {1, 1}, {1}, 0, 10000, 0};
                temp_wpn = Weapon(ITEM::TYPE::OFFHAND, temp_wpn_stats, ITEM::NAME::FLINTLOCK);
                temp_wpn.setDescription("A gun!");
                wpn_out[ITEM::NAME::FLINTLOCK] = temp_wpn;
                break;
            case ITEM::NAME::CIBO:
                temp_wpn_stats = {7, 0, 0, 0, 0, 0, 7, 8, 5, {1, 1}, {1}, 0, 10000, 0};
                temp_wpn = Weapon(ITEM::TYPE::OFFHAND, temp_wpn_stats, ITEM::NAME::FLINTLOCK);
                temp_wpn.setDescription("A gun!");
                wpn_out[ITEM::NAME::FLINTLOCK] = temp_wpn;
                break;

            case ITEM::NAME::SHRUNKEN_HEAD:
                temp_wpn_stats = {7, 0, 0, 0, 0, 0, 7, 8, 5, {1, 1}, {1}, 0, 1000, 0};
                temp_wpn = Weapon(ITEM::TYPE::TRINKET, temp_wpn_stats, ITEM::NAME::SHRUNKEN_HEAD);
                temp_wpn.setDescription("Horrible. Infused with demonic energies.");
                wpn_out[ITEM::NAME::SHRUNKEN_HEAD] = temp_wpn;
                break;
            case ITEM::NAME::CRYSTAL:
                temp_wpn_stats = {7, 0, 0, 0, 0, 0, 7, 8, 5, {1, 1}, {1}, 0, 1000, 0};
                temp_wpn = Weapon(ITEM::TYPE::TRINKET, temp_wpn_stats, ITEM::NAME::CRYSTAL);
                temp_wpn.setDescription("");
                wpn_out[ITEM::NAME::CRYSTAL] = temp_wpn;
                break;
            case ITEM::NAME::GLASS_ORB:
                temp_wpn_stats = {7, 0, 0, 0, 0, 0, 7, 8, 5, {1, 1}, {1}, 0, 1000, 0};
                temp_wpn = Weapon(ITEM::TYPE::TRINKET, temp_wpn_stats, ITEM::NAME::GLASS_ORB);
                temp_wpn.setDescription("Orb used to focus magical power.");
                wpn_out[ITEM::NAME::GLASS_ORB] = temp_wpn;
                break;
            case ITEM::NAME::HUMAN_FEMUR:
                temp_wpn_stats = {7, 0, 0, 0, 0, 0, 7, 8, 5, {1, 1}, {1}, 0, 1000, 0};
                temp_wpn = Weapon(ITEM::TYPE::TRINKET, temp_wpn_stats, ITEM::NAME::HUMAN_FEMUR);
                temp_wpn.setDescription("Taken from an ancient tomb. Used by demons to increase their magic power.");
                wpn_out[ITEM::NAME::HUMAN_FEMUR] = temp_wpn;
                break;
            case ITEM::NAME::VOID_FRAGMENT:
                temp_wpn_stats = {7, 0, 0, 0, 0, 0, 7, 8, 5, {1, 1}, {1}, 0, 1000, 0};
                temp_wpn = Weapon(ITEM::TYPE::TRINKET, temp_wpn_stats, ITEM::NAME::VOID_FRAGMENT);
                temp_wpn.setDescription("");
                wpn_out[ITEM::NAME::VOID_FRAGMENT] = temp_wpn;
                break;
            case ITEM::NAME::DEMON_FANG:
                temp_wpn_stats = {7, 0, 0, 0, 0, 0, 7, 8, 5, {1, 1}, {1}, 0, 1000, 0};
                temp_wpn = Weapon(ITEM::TYPE::TRINKET, temp_wpn_stats, ITEM::NAME::DEMON_FANG);
                temp_wpn.setDescription("Ripped from the mouth of a dead demon. Unusable by demons.");
                wpn_out[ITEM::NAME::DEMON_FANG] = temp_wpn;
                break;
            case ITEM::NAME::ANGEL_FEATHER:
                temp_wpn_stats = {7, 0, 0, 0, 0, 0, 7, 8, 5, {1, 1}, {1}, 0, 1000, 0};
                temp_wpn = Weapon(ITEM::TYPE::TRINKET, temp_wpn_stats, ITEM::NAME::ANGEL_FEATHER);
                temp_wpn.setDescription("Beautiful and fluffy. Given by incarnates and possessed to the worthy. Increases magical ability. Unusable by demons and demon incarnates.");
                wpn_out[ITEM::NAME::ANGEL_FEATHER] = temp_wpn;
                break;
            case ITEM::NAME::ANGEL_HAIR:
                temp_wpn_stats = {7, 0, 0, 0, 0, 0, 7, 8, 5, {1, 1}, {1}, 0, 1000, 0};
                temp_wpn = Weapon(ITEM::TYPE::TRINKET, temp_wpn_stats, ITEM::NAME::ANGEL_HAIR);
                temp_wpn.setDescription("Golden strands of angel hair held up by a silver thread. Increases magical ability. Unusable by demons and demon incarnates.");
                wpn_out[ITEM::NAME::ANGEL_HAIR] = temp_wpn;
                break;
            case ITEM::NAME::VIAL_OF_LIGHT:
                temp_wpn_stats = {7, 0, 0, 0, 0, 0, 7, 8, 5, {1, 1}, {1}, 0, 1000, 0};
                temp_wpn = Weapon(ITEM::TYPE::TRINKET, temp_wpn_stats, ITEM::NAME::VIAL_OF_LIGHT);
                temp_wpn.setDescription("Light magically contained in glass. Shines brightest in darkness.");
                wpn_out[ITEM::NAME::VIAL_OF_LIGHT] = temp_wpn;
                break;
            case ITEM::NAME::MAIDENS_HANDKERCHIEF:
                break;

            case ITEM::NAME::WAR_HORN:
                temp_wpn_stats = {7, 0, 0, 0, 0, 0, 7, 8, 5, {1, 1}, {1}, 0, 1000, 0};
                temp_wpn = Weapon(ITEM::TYPE::TRINKET, temp_wpn_stats, ITEM::NAME::WAR_HORN);
                temp_wpn.setDescription("Used to call for battle. Raise's close allies' combat abilities.");
                wpn_out[ITEM::NAME::WAR_HORN] = temp_wpn;
                break;

            case ITEM::NAME::BALL_LIGHTNING:
                temp_wpn_stats = {0, 12, 76, 0, 0, 0, 11, 25, 7, {1, 2}, {1, 2}, 1, 1000, 0};
                temp_wpn = Weapon(ITEM::TYPE::ELEMENTAL, temp_wpn_stats, ITEM::NAME::BALL_LIGHTNING);
                temp_wpn.setDescription("Lightning, compressed and explosive.");
                wpn_out[ITEM::NAME::BALL_LIGHTNING] = temp_wpn;
                break;
            case ITEM::NAME::ICICLE:
                break;
                temp_wpn_stats = {0, 6, 70, 0, 0, 0, 5, 34, 7, {1, 2}, {1, 2}, 1, 1000, 0};
                temp_wpn = Weapon(ITEM::TYPE::ELEMENTAL, temp_wpn_stats, ITEM::NAME::ICICLE);
                temp_wpn.setDescription("Summons sharp and cold death spikes.");
                wpn_out[ITEM::NAME::ICICLE] = temp_wpn;

            case ITEM::NAME::VOLCANO:
                temp_wpn_stats = {0, 14, 50, 0, 0, 0, 13, 20, 7, {1, 2}, {1, 2}, 1, 1000, 0};
                temp_wpn = Weapon(ITEM::TYPE::ELEMENTAL, temp_wpn_stats, ITEM::NAME::VOLCANO);
                temp_wpn.setDescription("Summons magma spewing mountains from under enemies' feets.");
                wpn_out[ITEM::NAME::VOLCANO] = temp_wpn;
                break;
            case ITEM::NAME::WIND_SPEAR:
                temp_wpn_stats = {0, 4, 80, 0, 0, 0, 3, 40, 7, {1, 2}, {1, 2}, 1, 1000, 0};
                temp_wpn = Weapon(ITEM::TYPE::ELEMENTAL, temp_wpn_stats, ITEM::NAME::WIND_SPEAR);
                temp_wpn.setDescription("Invisible, swift and minuscule. Can pierce armor.");
                wpn_out[ITEM::NAME::WIND_SPEAR] = temp_wpn;
                break;
            case ITEM::NAME::EMBER:
                temp_wpn_stats = {0, 6, 60, 0, 0, 0, 8, 30, 7, {1, 2}, {1, 2}, 1, 1000, 0};
                temp_wpn = Weapon(ITEM::TYPE::ELEMENTAL, temp_wpn_stats, ITEM::NAME::EMBER);
                temp_wpn.setDescription("Summon smoldering embers.");
                wpn_out[ITEM::NAME::EMBER] = temp_wpn;
                break;
            case ITEM::NAME::TSUNAMI:
                temp_wpn_stats = {0, 13, 40, 0, 0, 0, 11, 25, 7, {1, 2}, {1, 2}, 1, 1000, 0};
                temp_wpn = Weapon(ITEM::TYPE::ELEMENTAL, temp_wpn_stats, ITEM::NAME::TSUNAMI);
                temp_wpn.setDescription("");
                wpn_out[ITEM::NAME::TSUNAMI] = temp_wpn;
                break;
            case ITEM::NAME::GOD_FORGE_FIRE:
                temp_wpn_stats = {0, 18, 50, 0, 0, 0, 11, 15, 7, {1, 2}, {1, 2}, 1, 1000, 0};
                temp_wpn = Weapon(ITEM::TYPE::ELEMENTAL, temp_wpn_stats, ITEM::NAME::GOD_FORGE_FIRE);
                temp_wpn.setDescription("The fire used to forge Secundus, contained magically. Can melt any and all metals.");
                wpn_out[ITEM::NAME::GOD_FORGE_FIRE] = temp_wpn;
                break;
            case ITEM::NAME::FINGOLFIN:
                temp_wpn_stats = {0, 11, 50, 0, 0, 0, 9, 20, 7, {1, 2}, {1, 2}, 1, 1000, 0};
                temp_wpn = Weapon(ITEM::TYPE::ELEMENTAL, temp_wpn_stats, ITEM::NAME::FINGOLFIN);
                temp_wpn.setDescription("Distilled courage. Strikes at 7 times at darkness's heart. Effective against demons.");
                temp_wpn.setEffective(UNIT::TYPE::DEMON);
                wpn_out[ITEM::NAME::FINGOLFIN] = temp_wpn;
                break;
            case ITEM::NAME::MITHRANDIR:
                temp_wpn_stats = {0, 6, 65, 0, 0, 0, 5, 28, 7, {1, 2}, {1, 2}, 1, 1000, 0};
                temp_wpn = Weapon(ITEM::TYPE::ELEMENTAL, temp_wpn_stats, ITEM::NAME::MITHRANDIR);
                temp_wpn.setDescription("Inspires courage in the hearts of men. Effective against demons.");
                temp_wpn.setEffective(UNIT::TYPE::DEMON);
                wpn_out[ITEM::NAME::MITHRANDIR] = temp_wpn;
                break;

            case ITEM::NAME::VOIDD:
                temp_wpn_stats = {0, 5, 70, 0, 0, 0, 5, 60, 7, {1, 2}, {1, 2}, 1, 1000, 0};
                temp_wpn = Weapon(ITEM::TYPE::DEMONIC, temp_wpn_stats, ITEM::NAME::VOIDD);
                temp_wpn.setDescription("");
                wpn_out[ITEM::NAME::VOIDD] = temp_wpn;
                break;
            case ITEM::NAME::NETHER:
                temp_wpn_stats = {0, 5, 70, 0, 0, 0, 5, 60, 7, {1, 2}, {1, 2}, 1, 1000, 0};
                temp_wpn = Weapon(ITEM::TYPE::DEMONIC, temp_wpn_stats, ITEM::NAME::NETHER);
                temp_wpn.setDescription("");
                wpn_out[ITEM::NAME::NETHER] = temp_wpn;
                break;
            case ITEM::NAME::DOWNFALL:
                temp_wpn = Weapon(ITEM::TYPE::DEMONIC, temp_wpn_stats, ITEM::NAME::DOWNFALL);
                temp_wpn.setDescription("");
                temp_wpn.setDescription("");
                wpn_out[ITEM::NAME::DOWNFALL] = temp_wpn;
                break;

            case ITEM::NAME::CONSUME:
                temp_wpn_stats = {0, 5, 70, 0, 0, 0, 5, 60, 7, {1, 2}, {1, 2}, 1, 1000, 0};
                temp_wpn = Weapon(ITEM::TYPE::DEMONIC, temp_wpn_stats, ITEM::NAME::CONSUME);
                temp_wpn.setDescription("");
                wpn_out[ITEM::NAME::CONSUME] = temp_wpn;
                break;
            case ITEM::NAME::PUTRIFY:
                temp_wpn_stats = {0, 5, 70, 0, 0, 0, 5, 60, 7, {1, 2}, {1, 2}, 1, 1000, 0};
                temp_wpn = Weapon(ITEM::TYPE::DEMONIC, temp_wpn_stats, ITEM::NAME::PUTRIFY);
                temp_wpn.setDescription("Brings out the inner rot.");
                wpn_out[ITEM::NAME::PUTRIFY] = temp_wpn;
                break;
            case ITEM::NAME::GLAURUNG:
                temp_wpn_stats = {0, 5, 70, 0, 0, 0, 5, 60, 7, {1, 2}, {1, 2}, 1, 1000, 0};
                temp_wpn = Weapon(ITEM::TYPE::DEMONIC, temp_wpn_stats, ITEM::NAME::GLAURUNG);
                temp_wpn.setDescription("");
                wpn_out[ITEM::NAME::GLAURUNG] = temp_wpn;
                break;
            case ITEM::NAME::MORGOTH:
                temp_wpn_stats = {0, 5, 70, 0, 0, 0, 5, 60, 7, {1, 2}, {1, 2}, 1, 1000, 0};
                temp_wpn = Weapon(ITEM::TYPE::DEMONIC, temp_wpn_stats, ITEM::NAME::MORGOTH);
                temp_wpn.setDescription("Dark power, in the shape of a massive dark hammer. Makes craters wherever it strikes.");
                wpn_out[ITEM::NAME::MORGOTH] = temp_wpn;
                temp_wpn_stats = {0, 5, 70, 0, 0, 0, 5, 60, 7, {1, 2}, {1, 2}, 1, 1000, 0};
                break;
            case ITEM::NAME::GOTHMOG:
                temp_wpn = Weapon(ITEM::TYPE::DEMONIC, temp_wpn_stats, ITEM::NAME::GOTHMOG);
                temp_wpn.setDescription("Infernal whip of flame and shadows.");
                wpn_out[ITEM::NAME::GOTHMOG] = temp_wpn;
                break;

            case ITEM::NAME::FATALIS:
                temp_wpn_stats = {0, 5, 70, 0, 0, 0, 5, 60, 7, {1, 2}, {1, 2}, 1, 1000, 0};
                temp_wpn = Weapon(ITEM::TYPE::DEMONIC, temp_wpn_stats, ITEM::NAME::FATALIS);
                temp_wpn.setDescription("");
                wpn_out[ITEM::NAME::FATALIS] = temp_wpn;
                break;
        }
    }

    return (wpn_out);
}

void baseWeapons() {
    SDL_Log("Making all base weapons");
    std::vector<short int> toload;

    for (short int i = ITEM::NAME::WOODEN_SWORD; i < ITEM::NAME::FATALIS; i++) {
        toload.push_back(i);
    }

    all_weapons = baseWeapons(toload);
}
