#include "tile.hpp"
#include "stb_sprintf.h"

Tile::Tile() {
    setXMLElement("Tile");
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

void Tile::readJSON(cJSON * in_json) {
    cJSON * jitem = cJSON_GetObjectItemCaseSensitive(in_json, "item");
    cJSON * jstats = cJSON_GetObjectItemCaseSensitive(in_json, "Stats");

    description = cJSON_GetStringValue(jdescription);
    readJSON_stats(jstats, &stats);
}

void Tile::writeJSON(cJSON * in_json) {
    if (in_json != NULL) {
        cJSON * jtile = cJSON_CreateObject();
        cJSON * jtilestats = cJSON_CreateObject();
        cJSON * jcost = cJSON_CreateObject();
        cJSON * jname = cJSON_CreateString(name.c_str());
        cJSON * jid = cJSON_CreateNumber(id);

        writeJSON_mvtcost(jcost, &cost_struct);
        writeJSON_stats(jtilestats, &stats);

        cJSON_AddItemToObject(jtile, "Name", jname);
        cJSON_AddItemToObject(jtile, "id", jid);
        cJSON_AddBoolToObject(jtile, "inside", inside);
        cJSON_AddItemToObject(jtile, "Stats", jtilestats);
        cJSON_AddItemToObject(jtile, "MvtCost", jcost);
        cJSON_AddItemToObject(in_json, "tile", jtile);
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

void baseTiles(std::unordered_map<int, Tile> * in_tiles, const std::vector<short int> toload) {
    SDL_Log("Loading base tiles\n");
    Tile temp_tile;
    Movement_cost temp_cost;
    Tile_stats temp_stats;
    int index;

    for (short unsigned int i = 0; i < toload.size(); i++) {
        index = toload[i] / DEFAULT::TILE_DIVISOR;

        switch (index) {
            case TILE::PLAIN: {
                temp_cost = {1, 1, 1, 1, 1, 1, 1, 1, 1};
                temp_stats = {10, 0, 1, 0};
                temp_tile = Tile(TILE::PLAIN, "Plain", temp_cost, temp_stats, false);
                in_tiles->emplace(index, temp_tile);
                break;
            }

            case TILE::BUSH: {
                temp_cost = {2, 1, 2, 3, 2, 1, 3, 2, 2};
                temp_stats = {15, 0, 2, 0};
                temp_tile = Tile(TILE::BUSH, "Bush", temp_cost, temp_stats, false);
                in_tiles->emplace(index, temp_tile);
                break;
            }

            case TILE::FOREST: {
                temp_cost = {3, 2, 3, 4, 3, 1, 3, 3, 3};
                temp_stats = {15, 0, 2, 0};
                temp_tile = Tile(TILE::FOREST, "Forest", temp_cost, temp_stats, false);
                in_tiles->emplace(index, temp_tile);
                break;
            }

            case TILE::THICKET: {
                temp_cost = {0, 0, 0, 0, 0, 1, 0, 0, 0};
                temp_stats = {20, 0, 3, 0};
                temp_tile = Tile(TILE::THICKET, "Thicket", temp_cost, temp_stats, false);
                in_tiles->emplace(index, temp_tile);
                break;
            }

            case TILE::SNAG: {
                temp_cost = {0, 0, 0, 0, 0, 1, 0, 0, 0};
                temp_stats = {15, 0, 2, 0};
                temp_tile = Tile(TILE::SNAG, "Snag", temp_cost, temp_stats, false);
                in_tiles->emplace(index, temp_tile);
                break;
            }

            case TILE::BRIDGE: {
                temp_cost = {1, 1, 1, 1, 1, 1, 1, 1, 1};
                temp_stats = {15, 0, 2, 0};
                temp_tile = Tile(TILE::BRIDGE, "Bridge", temp_cost, temp_stats, false);
                in_tiles->emplace(index, temp_tile);
                break;
            }

            case TILE::HOLE: {
                temp_cost = {1, 1, 1, 1, 1, 1, 1, 1, 1};
                temp_stats = {15, 0, 2, 0};
                temp_tile = Tile(TILE::HOLE, "Hole", temp_cost, temp_stats, false);
                in_tiles->emplace(index, temp_tile);
                break;
            }

            case TILE::SEA: {
                temp_cost = {0, 0, 0, 0, 0, 1, 0, 2, 0};
                temp_stats = {15, 0, 2, 0};
                temp_tile = Tile(TILE::SEA, "Sea", temp_cost, temp_stats, false);
                in_tiles->emplace(index, temp_tile);
                break;
            }

            case TILE::LAKE: {
                temp_cost = {0, 0, 0, 0, 0, 1, 0, 3, 0};
                temp_stats = {15, 0, 2, 0};
                temp_tile = Tile(TILE::LAKE, "Lake", temp_cost, temp_stats, false);
                in_tiles->emplace(index, temp_tile);
                break;
            }

            case TILE::RIVER: {
                temp_cost = {5, 4, 5, 0, 0, 1, 0, 2, 4};
                temp_stats = {15, 0, 2, 0};
                temp_tile = Tile(TILE::RIVER, "River", temp_cost, temp_stats, false);
                in_tiles->emplace(index, temp_tile);
                break;
            }

            case TILE::WATERFALL: {
                temp_cost = {0, 0, 0, 0, 0, 1, 0, 0, 0};
                temp_stats = {15, 0, 2, 0};
                temp_tile = Tile(TILE::WATERFALL, "Waterfall", temp_cost, temp_stats, false);
                in_tiles->emplace(index, temp_tile);
                break;
            }

            case TILE::CLIFF: {
                temp_cost = {0, 0, 0, 0, 0, 1, 0, 0, 0};
                temp_stats = {15, 0, 2, 0};
                temp_tile = Tile(TILE::CLIFF, "cliff", temp_cost, temp_stats, false);
                in_tiles->emplace(index, temp_tile);
                break;
            }

            case TILE::HILL: {
                temp_cost = {3, 2, 3, 4, 3, 1, 4, 2, 2};
                temp_stats = {15, 0, 2, 0};
                temp_tile = Tile(TILE::HILL, "Hill", temp_cost, temp_stats, false);
                in_tiles->emplace(index, temp_tile);
                break;
            }

            case TILE::MOUNTAIN: {
                temp_cost = {4, 3, 4, 0, 4, 1, 0, 3, 3};
                temp_stats = {25, 0, 3, 0};
                temp_tile = Tile(TILE::MOUNTAIN, "Mountain", temp_cost, temp_stats, false);
                in_tiles->emplace(index, temp_tile);
                break;
            }

            case TILE::PEAK: {
                temp_cost = {0, 5, 0, 0, 0, 1, 0, 5, 5};
                temp_stats = {30, 0, 4, 0};
                temp_tile = Tile(TILE::PEAK, "Peak", temp_cost, temp_stats, false);
                in_tiles->emplace(index, temp_tile);
                break;
            }

            case TILE::CAVE: {
                temp_cost = {4, 3, 4, 0, 4, 1, 0, 3, 3};
                temp_stats = {20, 0, 2, 0};
                temp_tile = Tile(TILE::CAVE, "Cave", temp_cost, temp_stats, false);
                in_tiles->emplace(index, temp_tile);
                break;
            }

            case TILE::SAND: {
                temp_cost = {3, 2, 1, 4, 3, 1, 3, 2, 2};
                temp_stats = {0, 0, 0, 0};
                temp_tile = Tile(TILE::SAND, "Sand", temp_cost, temp_stats, false);
                in_tiles->emplace(index, temp_tile);
                break;
            }

            case TILE::WASTELAND: {
                temp_cost = {3, 2, 1, 4, 3, 1, 3, 2, 2};
                temp_stats = {0, 0, 0, 0};
                temp_tile = Tile(TILE::WASTELAND, "Wasteland", temp_cost, temp_stats, false);
                in_tiles->emplace(index, temp_tile);
                break;
            }

            case TILE::ROCK: {
                temp_cost = {0, 0, 0, 0, 0, 1, 0, 3, 3};
                temp_stats = {5, 0, 2, 0};
                temp_tile = Tile(TILE::ROCK, "Rock", temp_cost, temp_stats, false);
                in_tiles->emplace(index, temp_tile);
                break;
            }

            case TILE::BONES: {
                temp_cost = {0, 0, 0, 0, 0, 1, 0, 0, 0};
                temp_stats = {0, 0, 0, 0};
                temp_tile = Tile(TILE::BONES, "Bones", temp_cost, temp_stats, false);
                in_tiles->emplace(index, temp_tile);
                break;
            }

            case TILE::FLOOR: {
                temp_cost = {1, 1, 1, 1, 1, 1, 1, 1, 1};
                temp_stats = {0, 0, 0, 0};
                temp_tile = Tile(TILE::FLOOR, "Floor", temp_cost, temp_stats, true);
                in_tiles->emplace(index, temp_tile);
                break;
            }

            case TILE::PILLAR: {
                temp_cost = {2, 2, 2, 3, 3, 1, 3, 2, 2};
                temp_stats = {10, 1, 10, 0};
                temp_tile = Tile(TILE::PILLAR, "Pillar", temp_cost, temp_stats, true);
                in_tiles->emplace(index, temp_tile);
                break;
            }

            case TILE::WALL: {
                temp_cost = {0, 0, 0, 0, 0, 1, 0, 0, 0};
                temp_stats = {0, 0, 0, 0};
                temp_tile = Tile(TILE::WALL, "Wall", temp_cost, temp_stats, true);
                in_tiles->emplace(index, temp_tile);
                break;
            }

            case TILE::DOOR: {
                temp_cost = {0, 0, 0, 0, 0, 1, 0, 0, 0};
                temp_stats = {0, 0, 0, 0};
                temp_tile = Tile(TILE::DOOR, "Door", temp_cost, temp_stats, true);
                in_tiles->emplace(index, temp_tile);
                break;
            }

            case TILE::THRONE: {
                temp_cost = {0, 0, 0, 0, 0, 1, 0, 0, 0};
                temp_stats = {20, 2, 15, 0};
                temp_tile = Tile(TILE::THRONE, "Throne", temp_cost, temp_stats, true);
                in_tiles->emplace(index, temp_tile);
                break;
            }

            case TILE::GATE: {
                temp_cost = {0, 0, 0, 0, 0, 1, 0, 0, 0};
                temp_stats = {10, 1, 10, 0};
                temp_tile = Tile(TILE::GATE, "Gate", temp_cost, temp_stats, false);
                in_tiles->emplace(index, temp_tile);
                break;
            }

            case TILE::FENCE: {
                temp_cost = {0, 0, 0, 0, 0, 1, 0, 0, 0};
                temp_stats = {0, 0, 0, 0};
                temp_tile = Tile(TILE::FENCE, "Fence", temp_cost, temp_stats, false);
                in_tiles->emplace(index, temp_tile);
                break;
            }

            case TILE::SHOP: {
                temp_cost = {1, 1, 1, 1, 1, 1, 1, 1, 1};
                temp_stats = {10, 1, 10, 0};
                temp_tile = Tile(TILE::SHOP, "Shop", temp_cost, temp_stats, false);
                in_tiles->emplace(index, temp_tile);
                break;
            }

            case TILE::ARMORY: {
                temp_cost = {1, 1, 1, 1, 1, 1, 1, 1, 1};
                temp_stats = {15, 0, 2, 0};
                temp_tile = Tile(TILE::ARMORY, "Armory", temp_cost, temp_stats, false);
                in_tiles->emplace(index, temp_tile);
                break;
            }

            case TILE::VILLAGE: {
                temp_cost = {1, 1, 1, 1, 1, 1, 1, 1, 1};
                temp_stats = {15, 0, 2, 0};
                temp_tile = Tile(TILE::VILLAGE, "Village", temp_cost, temp_stats, false);
                in_tiles->emplace(index, temp_tile);
                break;
            }

            case TILE::FORT: {
                temp_cost = {2, 2, 2, 2, 2, 1, 2, 2, 2};
                temp_stats = {10, 2, 10, 0};
                temp_tile = Tile(TILE::FORT, "Fort", temp_cost, temp_stats, false);
                in_tiles->emplace(index, temp_tile);
                break;
            }

            case TILE::CASTLE: {
                temp_cost = {0, 0, 0, 0, 0, 1, 0, 0, 0};
                temp_stats = {0, 0, 0, 0};
                temp_tile = Tile(TILE::CASTLE, "Castle", temp_cost, temp_stats, false);
                in_tiles->emplace(index, temp_tile);
                break;
            }

            case TILE::SNOW: {
                temp_cost = {2, 2, 2, 3, 2, 1, 2, 2, 2};
                temp_stats = {0, 0, 0, 0};
                temp_tile = Tile(TILE::SNOW, "Snow", temp_cost, temp_stats, false);
                in_tiles->emplace(index, temp_tile);
                break;
            }

            case TILE::GLACIER: {
                temp_cost = {4, 3, 4, 0, 0, 1, 0, 3, 3};
                temp_stats = {0, 0, 0, 0};
                temp_tile = Tile(TILE::GLACIER, "Glacier", temp_cost, temp_stats, false);
                in_tiles->emplace(index, temp_tile);
                break;
            }

            default: {
                break;
            }
        }

        SDL_Log("loaded tile: %s, %d", temp_tile.getName().c_str(), temp_tile.getid());
    }
}

// void baseTiles(std::unordered_map<int, Tile> * in_tiles) {
//  SDL_Log("Loading all base tiles\n");
//     // Fliers always have 1 movement because they dismount inside.
//     Tile temp_tile;
//     // Mvt_types: foot_slow, foot_fast, mages, riders_slow, riders_fast, fliers, armors, pirates, bandits;
//     Movement_cost temp_cost = {1, 1, 1, 1, 1, 1, 1, 1, 1};
//     // Tile_stats:  dodge, Pprot, Mprot, heal
//     Tile_stats temp_stats = {10, 0, 1, 0};
//     temp_tile = Tile(TILE::PLAIN, "Plain", temp_cost, temp_stats, false);
//     in_tiles->emplace(index, temp_tile);
//     temp_cost = {2, 1, 2, 3, 2, 1, 3, 2, 2};
//     temp_stats = {15, 0, 2, 0};
//     temp_tile = Tile(TILE::BUSH, "Bush", temp_cost, temp_stats, false);
//     in_tiles->emplace(index, temp_tile);

//     temp_cost = {3, 2, 3, 4, 3, 1, 3, 3, 3};
//     temp_stats = {15, 0, 2, 0};
//     temp_tile = Tile(TILE::FOREST, "Forest", temp_cost, temp_stats, false);
//     in_tiles->emplace(index, temp_tile);
//     temp_cost = {0, 0, 0, 0, 0, 1, 0, 0, 0};
//     temp_stats = {20, 0, 3, 0};
//     temp_tile = Tile(TILE::THICKET, "Thicket", temp_cost, temp_stats, false);
//     in_tiles->emplace(index, temp_tile);
//     temp_cost = {0, 0, 0, 0, 0, 1, 0, 0, 0};
//     temp_stats = {15, 0, 2, 0};
//     temp_tile = Tile(TILE::SNAG, "Snag", temp_cost, temp_stats, false);
//     in_tiles->emplace(index, temp_tile);
//     temp_cost = {1, 1, 1, 1, 1, 1, 1, 1, 1};
//     temp_stats = {15, 0, 2, 0};
//     temp_tile = Tile(TILE::BRIDGE, "Bridge", temp_cost, temp_stats, false);
//     in_tiles->emplace(index, temp_tile);
//     temp_cost = {1, 1, 1, 1, 1, 1, 1, 1, 1};
//     temp_stats = {15, 0, 2, 0};
//     temp_tile = Tile(TILE::HOLE, "Hole", temp_cost, temp_stats, false);
//     in_tiles->emplace(index, temp_tile);

//     temp_cost = {0, 0, 0, 0, 0, 1, 0, 2, 0};
//     temp_stats = {15, 0, 2, 0};
//     temp_tile = Tile(TILE::SEA, "Sea", temp_cost, temp_stats, false);
//     in_tiles->emplace(index, temp_tile);
//     temp_cost = {0, 0, 0, 0, 0, 1, 0, 3, 0};
//     temp_stats = {15, 0, 2, 0};
//     temp_tile = Tile(TILE::LAKE, "Lake", temp_cost, temp_stats, false);
//     in_tiles->emplace(index, temp_tile);
//     temp_cost = {5, 4, 5, 0, 0, 1, 0, 2, 4};
//     temp_stats = {15, 0, 2, 0};
//     temp_tile = Tile(TILE::RIVER, "River", temp_cost, temp_stats, false);
//     in_tiles->emplace(index, temp_tile);
//     temp_cost = {0, 0, 0, 0, 0, 1, 0, 0, 0};
//     temp_stats = {15, 0, 2, 0};
//     temp_tile = Tile(TILE::WATERFALL, "Waterfall", temp_cost, temp_stats, false);
//     in_tiles->emplace(index, temp_tile);
//     temp_cost = {0, 0, 0, 0, 0, 1, 0, 0, 0};
//     temp_stats = {15, 0, 2, 0};
//     temp_tile = Tile(TILE::CLIFF, "Cliff", temp_cost, temp_stats, false);
//     in_tiles->emplace(index, temp_tile);

//     temp_cost = {3, 2, 3, 4, 3, 1, 4, 2, 2};
//     temp_stats = {15, 0, 2, 0};
//     temp_tile = Tile(TILE::HILL, "Hill", temp_cost, temp_stats, false);
//     in_tiles->emplace(index, temp_tile);
//     temp_cost = {4, 3, 4, 0, 4, 1, 0, 3, 3};
//     temp_stats = {25, 0, 3, 0};
//     temp_tile = Tile(TILE::MOUNTAIN, "Mountain", temp_cost, temp_stats, false);
//     in_tiles->emplace(index, temp_tile);
//     temp_cost = {0, 5, 0, 0, 0, 1, 0, 5, 5};
//     temp_stats = {30, 0, 4, 0};
//     temp_tile = Tile(TILE::PEAK, "Peak", temp_cost, temp_stats, false);
//     in_tiles->emplace(index, temp_tile);
//     temp_cost = {4, 3, 4, 0, 4, 1, 0, 3, 3};
//     temp_stats = {20, 0, 2, 0};
//     temp_tile = Tile(TILE::CAVE, "Cave", temp_cost, temp_stats, false);
//     in_tiles->emplace(index, temp_tile);

//     temp_cost = {3, 2, 1, 4, 3, 1, 3, 2, 2};
//     temp_stats = {0, 0, 0, 0};
//     temp_tile = Tile(TILE::SAND, "Sand", temp_cost, temp_stats, false);
//     in_tiles->emplace(index, temp_tile);
//     temp_cost = {3, 2, 1, 4, 3, 1, 3, 2, 2};
//     temp_stats = {0, 0, 0, 0};
//     temp_tile = Tile(TILE::WASTELAND, "Wasteland", temp_cost, temp_stats, false);
//     in_tiles->emplace(index, temp_tile);
//     temp_cost = {0, 0, 0, 0, 0, 1, 0, 3, 3};
//     temp_stats = {5, 0, 2, 0};
//     temp_tile = Tile(TILE::ROCK, "Rock", temp_cost, temp_stats, false);
//     in_tiles->emplace(index, temp_tile);
//     temp_cost = {0, 0, 0, 0, 0, 1, 0, 0, 0};
//     temp_stats = {0, 0, 0, 0};
//     temp_tile = Tile(TILE::BONES, "Bones", temp_cost, temp_stats, false);
//     in_tiles->emplace(index, temp_tile);

//     temp_cost = {1, 1, 1, 1, 1, 1, 1, 1, 1};
//     temp_stats = {0, 0, 0, 0};
//     temp_tile = Tile(TILE::FLOOR, "Floor", temp_cost, temp_stats, true);
//     in_tiles->emplace(index, temp_tile);
//     temp_cost = {2, 2, 2, 3, 3, 1, 3, 2, 2};
//     temp_stats = {10, 1, 10, 0};
//     temp_tile = Tile(TILE::PILLAR, "Pillar", temp_cost, temp_stats, true);
//     in_tiles->emplace(index, temp_tile);
//     temp_cost = {0, 0, 0, 0, 0, 1, 0, 0, 0};
//     temp_stats = {0, 0, 0, 0};
//     temp_tile = Tile(TILE::WALL, "Wall", temp_cost, temp_stats, true);
//     in_tiles->emplace(index, temp_tile);
//     temp_cost = {0, 0, 0, 0, 0, 1, 0, 0, 0};
//     temp_stats = {0, 0, 0, 0};
//     temp_tile = Tile(TILE::DOOR, "Door", temp_cost, temp_stats, true);
//     in_tiles->emplace(index, temp_tile);
//     temp_cost = {0, 0, 0, 0, 0, 1, 0, 0, 0};
//     temp_stats = {20, 2, 15, 0};
//     temp_tile = Tile(TILE::THRONE, "Throne", temp_cost, temp_stats, true);
//     in_tiles->emplace(index, temp_tile);

//     temp_cost = {0, 0, 0, 0, 0, 1, 0, 0, 0};
//     temp_stats = {10, 1, 10, 0};
//     temp_tile = Tile(TILE::GATE, "Gate", temp_cost, temp_stats, false);
//     in_tiles->emplace(index, temp_tile);
//     temp_cost = {0, 0, 0, 0, 0, 1, 0, 0, 0};
//     temp_stats = {0, 0, 0, 0};
//     temp_tile = Tile(TILE::FENCE, "Fence", temp_cost, temp_stats, false);
//     in_tiles->emplace(index, temp_tile);
//     temp_cost = {1, 1, 1, 1, 1, 1, 1, 1, 1};
//     temp_stats = {10, 1, 10, 0};
//     temp_tile = Tile(TILE::SHOP, "Shop", temp_cost, temp_stats, false);
//     in_tiles->emplace(index, temp_tile);
//     temp_cost = {1, 1, 1, 1, 1, 1, 1, 1, 1};
//     temp_stats = {15, 0, 2, 0};
//     temp_tile = Tile(TILE::ARMORY, "Armory", temp_cost, temp_stats, false);
//     in_tiles->emplace(index, temp_tile);
//     temp_cost = {1, 1, 1, 1, 1, 1, 1, 1, 1};
//     temp_stats = {15, 0, 2, 0};
//     temp_tile = Tile(TILE::VILLAGE, "Village", temp_cost, temp_stats, false);
//     in_tiles->emplace(index, temp_tile);
//     temp_cost = {2, 2, 2, 2, 2, 1, 2, 2, 2};
//     temp_stats = {10, 2, 10, 0};
//     temp_tile = Tile(TILE::FORT, "Fort", temp_cost, temp_stats, false);
//     in_tiles->emplace(index, temp_tile);
//     temp_cost = {0, 0, 0, 0, 0, 1, 0, 0, 0};
//     temp_stats = {0, 0, 0, 0};
//     temp_tile = Tile(TILE::CASTLE, "Castle", temp_cost, temp_stats, false);
//     in_tiles->emplace(index, temp_tile);

//     temp_cost = {2, 2, 2, 3, 2, 1, 2, 2, 2};
//     temp_stats = {0, 0, 0, 0};
//     temp_tile = Tile(TILE::SNOW, "Snow", temp_cost, temp_stats, false);
//     in_tiles->emplace(index, temp_tile);
//     temp_cost = {4, 3, 4, 0, 0, 1, 0, 3, 3};
//     temp_stats = {0, 0, 0, 0};
//     temp_tile = Tile(TILE::GLACIER, "Glacier", temp_cost, temp_stats, false);
//     in_tiles->emplace(index, temp_tile);
// }

std::vector<short int> testTiles() {
    std::vector<short int> temp_vector = {TILE::PLAIN * DEFAULT::TILE_DIVISOR + 0, TILE::FOREST * DEFAULT::TILE_DIVISOR + 0, TILE::SEA * DEFAULT::TILE_DIVISOR + 0, TILE::HILL * DEFAULT::TILE_DIVISOR + 0};
    return (temp_vector);
}

std::vector<short int> chap1Tiles() {
    std::vector<short int> temp_vector = {TILE::PLAIN * DEFAULT::TILE_DIVISOR + 0, TILE::FOREST * DEFAULT::TILE_DIVISOR + 0, TILE::SEA * DEFAULT::TILE_DIVISOR + 0, TILE::HILL * DEFAULT::TILE_DIVISOR + 0};
    return (temp_vector);
}

std::vector<short int> chap2Tiles() {
    std::vector<short int> temp_vector = {TILE::PLAIN * DEFAULT::TILE_DIVISOR + 0, TILE::FOREST * DEFAULT::TILE_DIVISOR + 0, TILE::SEA * DEFAULT::TILE_DIVISOR + 0, TILE::HILL * DEFAULT::TILE_DIVISOR + 0};
    return (temp_vector);
}

std::vector<short int> chap3Tiles() {
    std::vector<short int> temp_vector = {TILE::PLAIN * DEFAULT::TILE_DIVISOR + 0, TILE::FOREST * DEFAULT::TILE_DIVISOR + 0, TILE::SEA * DEFAULT::TILE_DIVISOR + 0, TILE::HILL * DEFAULT::TILE_DIVISOR + 0};
    return (temp_vector);
}

std::vector<short int> chap4Tiles() {
    std::vector<short int> temp_vector = {TILE::PLAIN * DEFAULT::TILE_DIVISOR + 0, TILE::FOREST * DEFAULT::TILE_DIVISOR + 0, TILE::SEA * DEFAULT::TILE_DIVISOR + 0, TILE::HILL * DEFAULT::TILE_DIVISOR + 0};
    return (temp_vector);
}

std::vector<short int> chap5Tiles() {
    std::vector<short int> temp_vector = {TILE::PLAIN * DEFAULT::TILE_DIVISOR + 0, TILE::FOREST * DEFAULT::TILE_DIVISOR + 0, TILE::SEA * DEFAULT::TILE_DIVISOR + 0, TILE::HILL * DEFAULT::TILE_DIVISOR + 0};
    return (temp_vector);
}

std::vector<short int> chap6Tiles() {
    std::vector<short int> temp_vector = {TILE::PLAIN * DEFAULT::TILE_DIVISOR + 0, TILE::FOREST * DEFAULT::TILE_DIVISOR + 0, TILE::SEA * DEFAULT::TILE_DIVISOR + 0, TILE::HILL * DEFAULT::TILE_DIVISOR + 0};
    return (temp_vector);
}

std::vector<short int> chap7Tiles() {
    std::vector<short int> temp_vector = {TILE::PLAIN * DEFAULT::TILE_DIVISOR + 0, TILE::FOREST * DEFAULT::TILE_DIVISOR + 0, TILE::SEA * DEFAULT::TILE_DIVISOR + 0, TILE::HILL * DEFAULT::TILE_DIVISOR + 0};
    return (temp_vector);
}

std::vector<short int> chap8Tiles() {
    std::vector<short int> temp_vector = {TILE::PLAIN * DEFAULT::TILE_DIVISOR + 0, TILE::FOREST * DEFAULT::TILE_DIVISOR + 0, TILE::SEA * DEFAULT::TILE_DIVISOR + 0, TILE::HILL * DEFAULT::TILE_DIVISOR + 0};
    return (temp_vector);
}

std::vector<short int> chap9Tiles() {
    std::vector<short int> temp_vector = {TILE::PLAIN * DEFAULT::TILE_DIVISOR + 0, TILE::FOREST * DEFAULT::TILE_DIVISOR + 0, TILE::SEA * DEFAULT::TILE_DIVISOR + 0, TILE::HILL * DEFAULT::TILE_DIVISOR + 0};
    return (temp_vector);
}

std::vector<short int> chap10Tiles() {
    std::vector<short int> temp_vector = {TILE::PLAIN * DEFAULT::TILE_DIVISOR + 0, TILE::FOREST * DEFAULT::TILE_DIVISOR + 0, TILE::SEA * DEFAULT::TILE_DIVISOR + 0, TILE::HILL * DEFAULT::TILE_DIVISOR + 0};
    return (temp_vector);
}

std::vector<short int> chap11Tiles() {
    std::vector<short int> temp_vector = {TILE::PLAIN * DEFAULT::TILE_DIVISOR + 0, TILE::FOREST * DEFAULT::TILE_DIVISOR + 0, TILE::SEA * DEFAULT::TILE_DIVISOR + 0, TILE::HILL * DEFAULT::TILE_DIVISOR + 0};
    return (temp_vector);
}

std::vector<short int> chap12Tiles() {
    std::vector<short int> temp_vector = {TILE::PLAIN * DEFAULT::TILE_DIVISOR + 0, TILE::FOREST * DEFAULT::TILE_DIVISOR + 0, TILE::SEA * DEFAULT::TILE_DIVISOR + 0, TILE::HILL * DEFAULT::TILE_DIVISOR + 0};
    return (temp_vector);
}

std::vector<short int> chap13Tiles() {
    std::vector<short int> temp_vector = {TILE::PLAIN * DEFAULT::TILE_DIVISOR + 0, TILE::FOREST * DEFAULT::TILE_DIVISOR + 0, TILE::SEA * DEFAULT::TILE_DIVISOR + 0, TILE::HILL * DEFAULT::TILE_DIVISOR + 0};
    return (temp_vector);
}

std::vector<short int> chap14Tiles() {
    std::vector<short int> temp_vector = {TILE::PLAIN * DEFAULT::TILE_DIVISOR + 0, TILE::FOREST * DEFAULT::TILE_DIVISOR + 0, TILE::SEA * DEFAULT::TILE_DIVISOR + 0, TILE::HILL * DEFAULT::TILE_DIVISOR + 0};
    return (temp_vector);
}

std::vector<short int> chap15Tiles() {
    std::vector<short int> temp_vector = {TILE::PLAIN * DEFAULT::TILE_DIVISOR + 0, TILE::FOREST * DEFAULT::TILE_DIVISOR + 0, TILE::SEA * DEFAULT::TILE_DIVISOR + 0, TILE::HILL * DEFAULT::TILE_DIVISOR + 0};
    return (temp_vector);
}

std::vector<short int> chap16Tiles() {
    std::vector<short int> temp_vector = {TILE::PLAIN * DEFAULT::TILE_DIVISOR + 0, TILE::FOREST * DEFAULT::TILE_DIVISOR + 0, TILE::SEA * DEFAULT::TILE_DIVISOR + 0, TILE::HILL * DEFAULT::TILE_DIVISOR + 0};
    return (temp_vector);
}

std::vector<short int> chap17Tiles() {
    std::vector<short int> temp_vector = {TILE::PLAIN * DEFAULT::TILE_DIVISOR + 0, TILE::FOREST * DEFAULT::TILE_DIVISOR + 0, TILE::SEA * DEFAULT::TILE_DIVISOR + 0, TILE::HILL * DEFAULT::TILE_DIVISOR + 0};
    return (temp_vector);
}

std::vector<short int> chap18Tiles() {
    std::vector<short int> temp_vector = {TILE::PLAIN * DEFAULT::TILE_DIVISOR + 0, TILE::FOREST * DEFAULT::TILE_DIVISOR + 0, TILE::SEA * DEFAULT::TILE_DIVISOR + 0, TILE::HILL * DEFAULT::TILE_DIVISOR + 0};
    return (temp_vector);
}

std::vector<short int> chap19Tiles() {
    std::vector<short int> temp_vector = {TILE::PLAIN * DEFAULT::TILE_DIVISOR + 0, TILE::FOREST * DEFAULT::TILE_DIVISOR + 0, TILE::SEA * DEFAULT::TILE_DIVISOR + 0, TILE::HILL * DEFAULT::TILE_DIVISOR + 0};
    return (temp_vector);
}

std::vector<short int> chap20Tiles() {
    std::vector<short int> temp_vector = {TILE::PLAIN * DEFAULT::TILE_DIVISOR + 0, TILE::FOREST * DEFAULT::TILE_DIVISOR + 0, TILE::SEA * DEFAULT::TILE_DIVISOR + 0, TILE::HILL * DEFAULT::TILE_DIVISOR + 0};
    return (temp_vector);
}

std::vector<short int> chap21Tiles() {
    std::vector<short int> temp_vector = {TILE::PLAIN * DEFAULT::TILE_DIVISOR + 0, TILE::FOREST * DEFAULT::TILE_DIVISOR + 0, TILE::SEA * DEFAULT::TILE_DIVISOR + 0, TILE::HILL * DEFAULT::TILE_DIVISOR + 0};
    return (temp_vector);
}

std::vector<short int> chap22Tiles() {
    std::vector<short int> temp_vector = {TILE::PLAIN * DEFAULT::TILE_DIVISOR + 0, TILE::FOREST * DEFAULT::TILE_DIVISOR + 0, TILE::SEA * DEFAULT::TILE_DIVISOR + 0, TILE::HILL * DEFAULT::TILE_DIVISOR + 0};
    return (temp_vector);
}

std::vector<short int> chap23Tiles() {
    std::vector<short int> temp_vector = {TILE::PLAIN * DEFAULT::TILE_DIVISOR + 0, TILE::FOREST * DEFAULT::TILE_DIVISOR + 0, TILE::SEA * DEFAULT::TILE_DIVISOR + 0, TILE::HILL * DEFAULT::TILE_DIVISOR + 0};
    return (temp_vector);
}

std::vector<short int> chap24Tiles() {
    std::vector<short int> temp_vector = {TILE::PLAIN * DEFAULT::TILE_DIVISOR + 0, TILE::FOREST * DEFAULT::TILE_DIVISOR + 0, TILE::SEA * DEFAULT::TILE_DIVISOR + 0, TILE::HILL * DEFAULT::TILE_DIVISOR + 0};
    return (temp_vector);
}

std::vector<short int> chap25Tiles() {
    std::vector<short int> temp_vector = {TILE::PLAIN * DEFAULT::TILE_DIVISOR + 0, TILE::FOREST * DEFAULT::TILE_DIVISOR + 0, TILE::SEA * DEFAULT::TILE_DIVISOR + 0, TILE::HILL * DEFAULT::TILE_DIVISOR + 0};
    return (temp_vector);
}

std::vector<short int> gaiden1Tiles() {
    std::vector<short int> temp_vector = {TILE::PLAIN * DEFAULT::TILE_DIVISOR + 0, TILE::FOREST * DEFAULT::TILE_DIVISOR + 0, TILE::SEA * DEFAULT::TILE_DIVISOR + 0, TILE::HILL * DEFAULT::TILE_DIVISOR + 0};
    return (temp_vector);
}

std::vector<short int> (*chapTiles[40])() = {testTiles, chap1Tiles, chap2Tiles, chap3Tiles, chap4Tiles,
                                             chap5Tiles, chap6Tiles, chap7Tiles, chap8Tiles, chap9Tiles, chap10Tiles,
                                             chap11Tiles, chap12Tiles, chap13Tiles, chap14Tiles, chap15Tiles, chap16Tiles,
                                             chap17Tiles, chap18Tiles, chap19Tiles, chap20Tiles, chap21Tiles, chap22Tiles,
                                             chap23Tiles, chap24Tiles, chap25Tiles, gaiden1Tiles
                                            };