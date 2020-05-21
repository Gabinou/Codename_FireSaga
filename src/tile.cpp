#include "tile.hpp"
#include "stb_sprintf.h"

Tile::Tile() {
    setXMLElement("Tile");
    setJSONElement("Tile");
}

Tile::~Tile() {

}

Tile::Tile(const short int in_id, const char * in_name, const Movement_cost in_cost, const Tile_stats in_stats, const bool in_inside) : Tile() {
    name = in_name;
    id = in_id;
    cost_struct = in_cost;
    makeMvtCostarray();
    stats = in_stats;
    inside = in_inside;
}

void Tile::makeMvtCostarray() {
    cost_array[UNIT::MVT::FOOT_SLOW] = cost_struct.foot_slow;
    cost_array[UNIT::MVT::FOOT_FAST] = cost_struct.foot_fast;
    cost_array[UNIT::MVT::MAGES] = cost_struct.mages;
    cost_array[UNIT::MVT::RIDERS_SLOW] = cost_struct.riders_slow;
    cost_array[UNIT::MVT::RIDERS_FAST] = cost_struct.riders_fast;
    cost_array[UNIT::MVT::FLIERS] = cost_struct.fliers;
    cost_array[UNIT::MVT::ARMORS] = cost_struct.armors;
    cost_array[UNIT::MVT::PIRATES] = cost_struct.pirates;
    cost_array[UNIT::MVT::BANDITS] = cost_struct.bandits;
}

unsigned char * Tile::getCost() {
    return (cost_array);
}

Movement_cost Tile::getCoststruct() {
    return (cost_struct);
}

std::string Tile::getName() {
    return (name);
}

Tile_stats Tile::getStats() {
    return (stats);
}

bool Tile::isInside() {
    return (inside);
}

bool Tile::isOutside() {
    return (!inside);
}
short int Tile::getid() {
    return (id);
}

void Tile::setInside(const bool in_inside) {
    inside = in_inside;
}

void Tile::readXML(tinyxml2::XMLElement * in_pTile) {
    tinyxml2::XMLElement * ptemp;
    id = (unsigned short int)in_pTile->IntAttribute("id");
    ptemp = in_pTile->FirstChildElement("Name");

    if (!ptemp) {
        SDL_Log("Cannot get Name element");
    } else {
        name = ptemp->GetText();
    }

    ptemp = in_pTile->FirstChildElement("MvtCost");

    if (!ptemp) {
        SDL_Log("Cannot get Name element");
    } else {
        readXML_mvtcost(ptemp, &cost_struct);
        makeMvtCostarray();
    }

    ptemp = in_pTile->FirstChildElement("Stats");

    if (!ptemp) {
        SDL_Log("Cannot get Name element");
    } else {
        readXML_stats(ptemp, &stats);
    }

}

void Tile::readJSON(cJSON * in_jtile) {
    if (in_jtile != NULL) {
        cJSON * jname = cJSON_GetObjectItemCaseSensitive(in_jtile, "Name");
        cJSON * jid = cJSON_GetObjectItemCaseSensitive(in_jtile, "id");
        cJSON * jinside = cJSON_GetObjectItemCaseSensitive(in_jtile, "inside");
        cJSON * jstats = cJSON_GetObjectItemCaseSensitive(in_jtile, "Stats");
        cJSON * jmvtcost = cJSON_GetObjectItemCaseSensitive(in_jtile, "MvtCost");

        id = cJSON_GetNumberValue(jid);
        name = cJSON_GetStringValue(jname);
        readJSON_stats(jstats, &stats);
        inside = cJSON_IsTrue(jinside);
        readJSON_mvtcost(jmvtcost, &cost_struct);
        makeMvtCostarray();

    } else  {
        SDL_Log("in_jtile is NULL");
    }
}

void Tile::writeJSON(cJSON * in_jtile) {
    if (in_jtile != NULL) {
        cJSON * jtilestats = cJSON_CreateObject();
        cJSON * jcost = cJSON_CreateObject();
        cJSON * jname = cJSON_CreateString(name.c_str());
        cJSON * jid = cJSON_CreateNumber(id);

        writeJSON_mvtcost(jcost, &cost_struct);
        writeJSON_stats(jtilestats, &stats);

        cJSON_AddItemToObject(in_jtile, "Name", jname);
        cJSON_AddItemToObject(in_jtile, "id", jid);
        cJSON_AddBoolToObject(in_jtile, "inside", inside);
        cJSON_AddItemToObject(in_jtile, "Stats", jtilestats);
        cJSON_AddItemToObject(in_jtile, "MvtCost", jcost);
    } else  {
        SDL_Log("in_jtile is NULL");
    }
}

void Tile::writeXML(tinyxml2::XMLDocument * in_doc, tinyxml2::XMLElement * in_pTile) {
    in_pTile->SetAttribute("id", id);
    in_pTile->SetAttribute("inside", inside);

    tinyxml2::XMLElement * pName = in_doc->NewElement("Name");
    in_pTile->InsertEndChild(pName);
    pName->SetText(name.c_str());

    tinyxml2::XMLElement * pMvtCost = in_doc->NewElement("MvtCost");
    in_pTile->InsertEndChild(pMvtCost);
    writeXML_mvtcost(in_doc, pMvtCost, &cost_struct);

    tinyxml2::XMLElement * pStats = in_doc->NewElement("Stats");
    in_pTile->InsertEndChild(pStats);
    writeXML_stats(in_doc, pStats, &stats);
}