
#include "item.hpp"

Item::Item() {

}

void Item::writeJSON(cJSON * in_json) {
    if (in_json != NULL) {
        cJSON * jitem = cJSON_CreateObject();
        cJSON_AddItemToObject(in_json, "item", jitem);

        cJSON * jname = cJSON_CreateString(name.c_str());
        cJSON * jid = cJSON_CreateNumber(id);
        cJSON * jbonus = cJSON_CreateObject();
        writeJSON_stats(jbonus, &bonus_stats);
        cJSON * jmalus = cJSON_CreateObject();
        writeJSON_stats(jmalus, &malus_stats);
        cJSON * jsellable = cJSON_CreateBool(sellable);

        cJSON * jusers = cJSON_CreateObject();
        cJSON * juserid = NULL;

        for (short int i = 0; i < users.size() ; i++) {
            juserid = cJSON_CreateNumber(users[i]);
            cJSON_AddItemToObject(jusers, "id", juserid);
        }

        cJSON * jeffects = cJSON_CreateObject();
        cJSON * jeffect = NULL;
        jeffect = cJSON_CreateNumber(effect);
        cJSON_AddItemToObject(jeffects, "id", jeffect);
        std::vector<std::string> effects = wpnEffects(effect);

        for (int i = 0; i < effects.size(); i++) {
            jeffect = cJSON_CreateString(effects[i].c_str());
            cJSON_AddItemToObject(jeffects, "Effect", jeffect);
        }

        cJSON * jtypes = cJSON_CreateObject();
        cJSON * jtype2 = NULL;
        jtype2 = cJSON_CreateNumber(type);
        cJSON_AddItemToObject(jtypes, "id", jtype2);
        std::vector<std::string> types = wpnTypes(type);

        for (int i = 0; i < types.size(); i++) {
            jtype2 = cJSON_CreateString(types[i].c_str());
            cJSON_AddItemToObject(jtypes, "Type", jtype2);
        }

        cJSON_AddItemToObject(jitem, "Name", jname);
        cJSON_AddItemToObject(jitem, "id", jid);
        cJSON_AddStringToObject(jitem, "Description", description.c_str());
        cJSON_AddItemToObject(jitem, "Bonus", jbonus);
        cJSON_AddItemToObject(jitem, "Malus", jmalus);
        cJSON_AddItemToObject(jitem, "Sellable", jsellable);
        cJSON_AddItemToObject(jitem, "Effects", jeffects);
        cJSON_AddItemToObject(jitem, "Types", jtypes);
    }
}

void Item::readJSON(cJSON * in_json) {

}

void Item::writeXML(tinyxml2::XMLDocument * in_doc, tinyxml2::XMLElement * in_pItem) {

}

void Item::readXML(tinyxml2::XMLElement * in_pItem) {

}

void Item::setMalus(Unit_stats in_malus) {
    malus_stats = in_malus;
}

Unit_stats Item::getMalus() {
    return (malus_stats);
}

void Item::setBonus(Unit_stats in_bonus) {
    bonus_stats = in_bonus;
}
Unit_stats Item::getBonus() {
    return (bonus_stats);
}

bool Item::isSellable() {
    return (sellable);
}

void Item::setSellable(bool in_sellable) {
    sellable = in_sellable;
}

std::vector<unsigned short int> Item::getUsers() {
    return (users);
}

void Item::setUsers(std::vector<unsigned short int> in_users) {
    users = in_users;
}

void Item::setEffect(long unsigned int in_effect) {
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

long unsigned int Item::getEffect() {
    return (effect);
}

void Item::setDescription(std::string in_description) {
    description = in_description;
}

std::string Item::getDescription() {
    return (description);
}

std::string Item::getName() {
    return (name);
}

void Item::setName(std::string in_name) {
    name = in_name;
}

short unsigned int Item::getType() {
    return (type);
}

void Item::setType(short unsigned int in_type) {
    type = in_type;
}

