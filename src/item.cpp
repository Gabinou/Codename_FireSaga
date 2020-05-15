
#include "item.hpp"

Item::Item() {

}

void Item::writeJSON(cJSON * in_json) {

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

