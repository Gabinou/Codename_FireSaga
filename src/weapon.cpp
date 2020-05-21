
#include "weapon.hpp"

Weapon::Weapon() {
    setJSONElement("Item");
    setXMLElement("Item");
}

Weapon::Weapon(short unsigned int in_type, Weapon_stats in_stats, unsigned char in_id) : Weapon() {
    stats = in_stats;
    name = itemNames[in_id];
    type = in_type;
    id = in_id;
}

Weapon_stats Weapon::getStats() {
    return (stats);
}

void Weapon::setStats(Weapon_stats in_stats) {
    stats = in_stats;
}

void Weapon::infuse(unsigned char in_mag, unsigned short int in_type) {
    if (infused.power < 0) {
        if (((in_type & ITEM::TYPE::ELEMENTAL) > 0) || ((in_type & ITEM::TYPE::ANGELIC) > 0) || ((in_type & ITEM::TYPE::DEMONIC) > 0)) {
            infused.power = in_mag;
            infused.type = in_type;
        } else {
            SDL_Log("Cannot infuse weapon with weapon type otehr than Elemental, Demonic or Angelic.");
        }
    } else {
        SDL_Log("Weapon is already infused/is a magic Weapon. Cannot infuse again.");
    }
}

Infusion Weapon::getInfused() {
    return (infused);
}

bool Weapon::canInfuse() {
    bool out;

    if (infused.power < 0) {
        out = true;
    } else {
        out = false;
    }

    return (out);
}

void Weapon::setEffective(short unsigned int in_effective) {
    effective = in_effective;
}

short unsigned int Weapon::getEffective() {
    return (effective);
}

void Weapon::readJSON(cJSON * in_jwpn) {
    if (in_jwpn != NULL) {
        Item::readJSON(in_jwpn);

        cJSON * jstats = cJSON_GetObjectItemCaseSensitive(in_jwpn, "Stats");
        cJSON * jinfused = cJSON_GetObjectItemCaseSensitive(in_jwpn, "Infused");
        cJSON * jpower = cJSON_GetObjectItemCaseSensitive(jinfused, "Power");
        cJSON * jitype = cJSON_GetObjectItemCaseSensitive(jinfused, "Type");
        cJSON * jeffective = cJSON_GetObjectItemCaseSensitive(in_jwpn, "Effective");

        readJSON_stats(jstats, &stats);
        infused.power = cJSON_GetNumberValue(jpower);
        infused.type = cJSON_GetNumberValue(jitype);
        effective = cJSON_GetNumberValue(jeffective);
    } else  {
        SDL_Log("in_jwpn is NULL");
    }
}

void Weapon::writeJSON(cJSON * in_jwpn) {
    if (in_jwpn != NULL) {
        Item::writeJSON(in_jwpn);

        cJSON * jitemstats = cJSON_CreateObject();
        cJSON * jinfused = cJSON_CreateObject();
        cJSON * jpower = cJSON_CreateNumber(infused.power);
        cJSON * jtype = cJSON_CreateNumber(infused.type);
        cJSON * jeffective = cJSON_CreateNumber(effective);

        writeJSON_stats(jitemstats, &stats);
        cJSON_AddItemToObject(jinfused, "Power", jpower);
        cJSON_AddItemToObject(jinfused, "Type", jtype);
        cJSON_AddItemToObject(in_jwpn, "Stats", jitemstats);
        cJSON_AddItemToObject(in_jwpn, "Infused", jinfused);
    } else  {
        SDL_Log("in_jwpn is NULL");
    }
}

void Weapon::readXML(tinyxml2::XMLElement * in_pWpn) {
    tinyxml2::XMLElement * ptemp;
    tinyxml2::XMLElement * ptemp2;

    ptemp = in_pWpn->FirstChildElement("Name");

    if (!ptemp) {
        SDL_Log("Cannot get Name element");
    } else {
        name = ptemp->GetText();
    }

    ptemp = in_pWpn->FirstChildElement("Sellable");

    if (!ptemp) {
        SDL_Log("Cannot get Sellable element");
    } else {
        sellable = ptemp->BoolText();
    }

    ptemp = in_pWpn->FirstChildElement("Infused");

    if (!ptemp) {
        SDL_Log("Cannot get Infused element");
    } else {
        ptemp2 = ptemp->FirstChildElement("Power");
        infused.power = ptemp2->IntText();
    }

    if (!ptemp) {
        SDL_Log("Cannot get Power element");
    } else {
        ptemp2 = ptemp->FirstChildElement("Type");
        infused.type = ptemp2->IntText();
    }

    id = (unsigned short int)in_pWpn->IntAttribute("id");
    ptemp = in_pWpn->FirstChildElement("Description");

    if (!ptemp) {
        SDL_Log("Cannot get Description element");
    } else {
        description = ptemp->GetText();
    }

    ptemp = in_pWpn->FirstChildElement("Bonus");

    if (!ptemp) {
        SDL_Log("Cannot get Bonus element");
    } else {
        readXML_stats(ptemp, &bonus_stats);
    }

    ptemp = in_pWpn->FirstChildElement("Malus");

    if (!ptemp) {
        SDL_Log("Cannot get Malus element");
    } else {
        readXML_stats(ptemp, &malus_stats);
    }

    ptemp = in_pWpn->FirstChildElement("Types");

    if (!ptemp) {
        SDL_Log("Cannot get Types element");
    } else {
        type = (unsigned short int)ptemp->IntAttribute("id");
    }

    ptemp = in_pWpn->FirstChildElement("Effectives");

    if (!ptemp) {
        SDL_Log("Cannot get Effectives element");
    } else {
        effective = (unsigned short int)ptemp->IntAttribute("id");
    }

    ptemp = in_pWpn->FirstChildElement("Effects");

    if (!ptemp) {
        SDL_Log("Cannot get Effects element");
    } else {
        effect = (long unsigned int)ptemp->IntAttribute("id");
    }

    ptemp = in_pWpn->FirstChildElement("Stats");

    if (!ptemp) {
        SDL_Log("Cannot get Stats element");
    } else {
        readXML_stats(ptemp, &stats);
    }
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
    tinyxml2::XMLElement * pPower = in_doc->NewElement("Power");
    pInfused->InsertEndChild(pPower);
    pPower->SetText(infused.power);
    tinyxml2::XMLElement * piType = in_doc->NewElement("Type");
    pInfused->InsertEndChild(piType);
    piType->SetText(infused.type);

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