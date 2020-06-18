#include "map.hpp"
#include "position.hpp"
#include "sprite.hpp"

Map::Map() {
    srcrect.x = srcrect.y = 0;
    destrect.x = destrect.y = 0;
    setTilesize(DEFAULT::TILESIZE, DEFAULT::TILESIZE);
    srcrect.w = destrect.w = DEFAULT::TILESIZE;
    srcrect.h = destrect.h = DEFAULT::TILESIZE;
    overlay_mode = MAP::OVERLAY::MOVE + MAP::OVERLAY::ATTACK;
    setJSONElement("Map");
    setXMLElement("Map");
}

Map::~Map() {

}

std::vector<Point> Map::getStartingpos() {
    return (starting_positions);
}

void Map::setStartingpos(std::vector<Point> in_positions) {
    starting_positions = in_positions;
}

void Map::addStartingpos(Point in_position) {
    starting_positions.push_back(in_position);
}

void Map::loadTilesJSON() {
    if (tilesindex.size() == tilenames.size()) {
        int16_t index;
        Tile temp_tile;
        std::string filename;

        for (int16_t i = 0; i < tilenames.size(); i++) {
            if (tilesindex[i] > DEFAULT::TILE_MAX) {
                index = tilesindex[i] / DEFAULT::TILE_DIVISOR;
            } else {
                index = tilesindex[i];
            }

            filename = "tiles//" + tilenames[i] + ".json";
            temp_tile.readJSON(filename.c_str());
            tiles[index] = temp_tile;
        }


    } else {
        SDL_Log("map: different number of tilenames to tileindex");
    }
}

void Map::readJSON(cJSON * in_jmap) {
    if (in_jmap != NULL) {

        cJSON * jchapter = cJSON_GetObjectItem(in_jmap, "chapter");
        cJSON * jarrivals = cJSON_GetObjectItem(in_jmap, "Arrivals");
        cJSON * jtiles = cJSON_GetObjectItem(in_jmap, "tiles");

        cJSON * jtile = cJSON_GetObjectItem(jtiles, "tile");
        cJSON * jid;
        cJSON * jtilename;
        tilenames.clear();
        tilesindex.clear();

        while (jtile != NULL) {
            jid = cJSON_GetObjectItem(jtile, "id");
            jtilename = cJSON_GetObjectItem(jtile, "name");
            tilesindex.push_back(cJSON_GetNumberValue(jid) * DEFAULT::TILE_DIVISOR);
            SDL_Log("Read Tilesindex %d", tilesindex[tilesindex.size() - 1]);
            tilenames.push_back(cJSON_GetStringValue(jtilename));
            jtile = jtile->next;
        }

        loadTilesJSON();
        cJSON * jarrival = cJSON_GetObjectItem(jarrivals, "arrival");
        cJSON * jequipment;
        cJSON * jitem;
        Map_arrival temp_arrival;
        map_arrivals.clear();
        arrival_equipments.clear();
        Inventory_item temp_item;
        std::vector<Inventory_item> arrival_equipment;

        while (jarrival != NULL) {
            temp_arrival = Map_arrival();
            readJSON_arrival(jarrival, &temp_arrival);
            map_arrivals.push_back(temp_arrival);

            jequipment = cJSON_GetObjectItem(jarrival, "Equipment");
            jitem = cJSON_GetObjectItem(jequipment, "Item");
            arrival_equipment.clear();

            while (jitem != NULL) {
                temp_item = Inventory_item();
                readJSON_item(jitem, &temp_item);
                arrival_equipment.push_back(temp_item);
                jitem = jitem->next;
            }

            arrival_equipments.push_back(arrival_equipment);
            jarrival = jarrival->next;
        }

        cJSON * jbounds = cJSON_GetObjectItem(in_jmap, "Bounds");
        cJSON * jrow_min = cJSON_GetObjectItem(jbounds, "row_min");
        cJSON * jrow_max = cJSON_GetObjectItem(jbounds, "row_max");
        cJSON * jcol_min = cJSON_GetObjectItem(jbounds, "col_min");
        cJSON * jcol_max = cJSON_GetObjectItem(jbounds, "col_max");
        bounds[0] = cJSON_GetNumberValue(jrow_min);
        bounds[1] = cJSON_GetNumberValue(jrow_max);
        bounds[2] = cJSON_GetNumberValue(jcol_min);
        bounds[3] = cJSON_GetNumberValue(jcol_max);

        cJSON * joffset = cJSON_GetObjectItem(in_jmap, "Offset");
        cJSON * joffset_row = cJSON_GetObjectItem(joffset, "offset_row");
        cJSON * joffset_col = cJSON_GetObjectItem(joffset, "offset_col");
        offset[0] = cJSON_GetNumberValue(jrow_min);
        offset[1] = cJSON_GetNumberValue(jrow_min);

        cJSON * jtilemap = cJSON_GetObjectItem(in_jmap, "tilemap");
        tilemap = readJSON_tilemap(jtilemap);
        postTilemap();

    } else {
        SDL_Log("in_jmap is NULL");
    }
}

void Map::writeJSON(cJSON * in_jmap) {
    if (in_jmap != NULL) {
        cJSON * jchapter = cJSON_CreateNumber(chapter);
        cJSON_AddItemToObject(in_jmap, "chapter", jchapter);
        cJSON * jstartingpositions = cJSON_CreateObject();
        cJSON_AddItemToObject(in_jmap, "StartingPositions", jstartingpositions);
        cJSON * jstartingpos;
        cJSON * jstartingposx;
        cJSON * jstartingposy;

        for (int16_t i = 0; i < starting_positions.size(); i++) {
            jstartingpos = cJSON_CreateObject();
            cJSON_AddItemToObject(jstartingpositions, "Position", jstartingpos);
            jstartingposx = cJSON_CreateNumber(starting_positions[i].x);
            jstartingposy = cJSON_CreateNumber(starting_positions[i].y);
            cJSON_AddItemToObject(jstartingpos, "x", jstartingposx);
            cJSON_AddItemToObject(jstartingpos, "y", jstartingposy);
        }

        cJSON * jtiles = cJSON_CreateObject();
        cJSON_AddItemToObject(in_jmap, "tiles", jtiles);
        cJSON * jtile;
        cJSON * jtilename;
        cJSON * jtileid;

        if (tilesindex.size() == tilenames.size()) {
            for (int i = 0; i < tilesindex.size(); i++) {
                jtile = cJSON_CreateObject();
                jtilename = cJSON_CreateString(tilenames[i].c_str());

                if (tilesindex[i] >= 100) {
                    jtileid = cJSON_CreateNumber(tilesindex[i] / DEFAULT::TILE_DIVISOR);
                } else {
                    jtileid = cJSON_CreateNumber(tilesindex[i]);
                }

                cJSON_AddItemToObject(jtile, "id", jtileid);
                cJSON_AddItemToObject(jtile, "name", jtilename);
                cJSON_AddItemToObject(jtiles, "tile", jtile);
            }
        } else {
            SDL_Log("Not the same number of tilenames as tileindex.");
        }

        if (map_arrivals.size() == arrival_equipments.size()) {
            cJSON * jarrival;
            cJSON * jarrivaleq;
            cJSON * jarrivals = cJSON_CreateObject();
            cJSON_AddItemToObject(in_jmap, "Arrivals", jarrivals);

            for (int i = 0; i < map_arrivals.size(); i++) {
                jarrival = cJSON_CreateObject();
                jarrivaleq = cJSON_CreateObject();
                writeJSON_arrival(jarrival, &map_arrivals[i]);
                writeJSON_items(jarrivaleq, arrival_equipments[i]);
                cJSON_AddItemToObject(jarrival, "Equipment", jarrivaleq);
                cJSON_AddItemToObject(jarrivals, "Arrival", jarrival);
            }

        } else {
            SDL_Log("Different numbers of arrivals to arrival_equipments");
        }

        cJSON * jbounds = cJSON_CreateObject();
        cJSON * jrow_min = cJSON_CreateNumber(bounds[0]);
        cJSON * jrow_max = cJSON_CreateNumber(bounds[1]);
        cJSON * jcol_min = cJSON_CreateNumber(bounds[2]);
        cJSON * jcol_max = cJSON_CreateNumber(bounds[3]);
        cJSON_AddItemToObject(jbounds, "row_min", jrow_min);
        cJSON_AddItemToObject(jbounds, "row_max", jrow_max);
        cJSON_AddItemToObject(jbounds, "col_min", jcol_min);
        cJSON_AddItemToObject(jbounds, "col_max", jcol_max);
        cJSON_AddItemToObject(in_jmap, "Bounds", jbounds);
        cJSON * joffset = cJSON_CreateObject();
        cJSON * joffset_row = cJSON_CreateNumber(offset[0]);
        cJSON * joffset_col = cJSON_CreateNumber(offset[1]);
        cJSON_AddItemToObject(joffset, "offset_row", joffset_row);
        cJSON_AddItemToObject(joffset, "offset_col", joffset_col);
        cJSON_AddItemToObject(in_jmap, "Offset", joffset);
        cJSON * jtilemap = cJSON_CreateObject();
        writeJSON_tilemap(jtilemap, tilemap);
        cJSON_AddItemToObject(in_jmap, "tilemap", jtilemap);
    } else {
        SDL_Log("in_jmap is NULL");
    }
}

Map::Map(const uint16_t width, const uint16_t height) : Map() {
    setTilesize(width, height);
    srcrect.w = destrect.w = width;
    srcrect.h = destrect.h = height;
}

void Map::readXML(tinyxml2::XMLElement * in_pMap) {
    const char * buffer;
    int buffint;
    tinyxml2::XMLElement * ptemp;

    SDL_Log("XMLElement: %s", getXMLElement().c_str());

    chapter = (uint16_t)in_pMap->IntAttribute("chapter");

    tinyxml2::XMLElement * pTiles = in_pMap->FirstChildElement("Tiles");
    tinyxml2::XMLElement * pTile;
    tilenames.clear();
    tilesindex.clear();

    if (pTiles) {
        pTile = pTiles->FirstChildElement("Tile");

        while (pTile) {
            tilesindex.push_back(pTile->IntAttribute("id") * DEFAULT::TILE_DIVISOR);
            ptemp = pTile->FirstChildElement("Name");
            tilenames.push_back(ptemp->GetText());
            pTile = pTile->NextSiblingElement("Tile");
        }
    } else {
        SDL_Log("Cannot get Tiles element");
    }

    tinyxml2::XMLElement * pBounds = in_pMap->FirstChildElement("Bounds");

    if (!pBounds) {
        SDL_Log("Cannot get Bounds element");
    } else {
        ptemp = pBounds->FirstChildElement("row_max");

        if (!ptemp) {
            SDL_Log("Cannot get row_max element");
        } else {
            ptemp->QueryIntText(&buffint);
            bounds[1] = buffint;
        }

        ptemp = pBounds->FirstChildElement("col_max");

        if (!ptemp) {
            SDL_Log("Cannot get col_max element");
        } else {
            ptemp->QueryIntText(&buffint);
            bounds[3] = buffint;
        }

    }

    tilemap.clear();

    tinyxml2::XMLElement * pTilemap = in_pMap->FirstChildElement("Tilemap");

    if (!pTilemap) {
        SDL_Log("Cannot get tilemap element");
    } else {
        tinyxml2::XMLElement * pRow = pTilemap->FirstChildElement("Row");
        tinyxml2::XMLElement * pCol;
        int16_t tempcol;
        int16_t temprow;
        std::vector<int16_t> tempvec;

        while (pRow) {
            pCol = pRow->FirstChildElement("Col");
            temprow = pRow->IntAttribute("row");
            tempvec.clear();

            while (pCol) {
                pCol->QueryIntText(&buffint);
                tempcol = pCol->IntAttribute("col");
                tempvec.push_back(buffint);
                pCol = pCol->NextSiblingElement("Col");
            }

            tilemap.push_back(tempvec);
            pRow = pRow->NextSiblingElement("Row");
        }
    }

    tinyxml2::XMLElement * pArrivals = in_pMap->FirstChildElement("Arrivals");

    if (!pArrivals) {
        SDL_Log("Cannot get Arrivals element");
    } else {
        tinyxml2::XMLElement * pArrival = pArrivals->FirstChildElement("Arrival");
        Map_arrival temp_arrival;

        while (pArrival) {
            readXML_arrival(pArrival, &temp_arrival);
            map_arrivals.push_back(temp_arrival);
            pArrival = pArrival->NextSiblingElement("Arrival");
        }
    }

    tinyxml2::XMLElement * pArrivalEqs = in_pMap->FirstChildElement("ArrivalEqs");

    if (!pArrivalEqs) {
        SDL_Log("Cannot get Arrivals element");
    } else {
        tinyxml2::XMLElement * pArrivalEq = pArrivalEqs->FirstChildElement("ArrivalEq");
        arrival_equipments.clear();
        std::vector<Inventory_item> tempeq;

        while (pArrivalEq) {
            readXML_items(pArrivalEq, &tempeq);
            arrival_equipments.push_back(tempeq);
            pArrivalEq = pArrivalEq->NextSiblingElement("ArrivalEq");
        }
    }

    clearUnitmap();
    std::vector<std::vector<entityx::ComponentHandle<Unit>>> tempunitmap(bounds[1], std::vector<entityx::ComponentHandle<Unit>>(bounds[3]));
    unitmap = tempunitmap;
    tinyxml2::XMLElement * pUnitmap = in_pMap->FirstChildElement("Unitmap");

    if (!pUnitmap) {
        SDL_Log("Cannot get Unitmap element");
    } else {
        tinyxml2::XMLElement * pOnMap = pUnitmap->FirstChildElement("OnMap");
        Unit tempunit;
        entityx::Entity tempUent;

        while (pOnMap) {
            tempunit.readXML(pOnMap);
            tempUent = manager->create();
            tempUent.assign<Unit>(tempunit);

            tempUent.assign<Position>(pOnMap->IntAttribute("row"), pOnMap->IntAttribute("col"));
            unitmap[pOnMap->IntAttribute("row")][pOnMap->IntAttribute("col")] = tempUent.component<Unit>();

            pOnMap = pOnMap->NextSiblingElement("OnMap");
        }
    }

}

void Map::clearUnitmap() {
    entityx::ComponentHandle<Unit> tempunit;

    for (uint16_t row = 0; row < unitmap.size(); row++) {// This loop cache friendly.
        for (uint16_t col = 0; col < unitmap[row].size(); col++) {
            tempunit = unitmap[row][col];

            if (tempunit) {
                tempunit.entity().destroy();
            }
        }
    }

    unitmap.clear();
}

void Map::writeXML(tinyxml2::XMLDocument * in_doc, tinyxml2::XMLElement * in_pMap) {
    char buffer[DEFAULT::BUFFER_SIZE];
    in_pMap->SetAttribute("chapter", chapter);

    tinyxml2::XMLElement * pTiles = in_doc->NewElement("Tiles");
    in_pMap->InsertEndChild(pTiles);
    tinyxml2::XMLElement * pTile;
    tinyxml2::XMLElement * pName;

    if (tilesindex.size() == tilenames.size()) {
        for (uint16_t i = 0; i < tilesindex.size(); i++) {
            pTile = in_doc->NewElement("Tile");
            pName = in_doc->NewElement("Name");

            if (tilesindex[i] >= 100) {
                pTile->SetAttribute("id", tilesindex[i] / DEFAULT::TILE_DIVISOR);
            } else {
                pTile->SetAttribute("id", tilesindex[i]);
            }

            pTiles->InsertEndChild(pTile);
            pTile->InsertEndChild(pName);
            pName->SetText(tilenames[i].c_str());
        }
    } else {
        SDL_Log("Not the same number of tilenames as tileindex.");
    }

    tinyxml2::XMLElement * pBounds = in_doc->NewElement("Bounds");
    tinyxml2::XMLElement * pRowmax = in_doc->NewElement("row_max");
    tinyxml2::XMLElement * pColmax = in_doc->NewElement("col_max");
    in_pMap->InsertEndChild(pBounds);
    pBounds->InsertEndChild(pRowmax);
    pBounds->InsertEndChild(pColmax);
    pRowmax->SetText(bounds[1]);
    pColmax->SetText(bounds[3]);

    tinyxml2::XMLElement * pTilemap = in_doc->NewElement("Tilemap");
    tinyxml2::XMLElement * pRow;
    tinyxml2::XMLElement * pCol;
    tinyxml2::XMLElement * pIndex;
    in_pMap->InsertEndChild(pTilemap);

    if ((tilemap.size() == bounds[1]) && (tilemap[0].size() == bounds[3])) {
        for (uint16_t row = 0; row < tilemap.size(); row++) {// This loop cache friendly.
            pRow = in_doc->NewElement("Row");
            pRow->SetAttribute("row", row);
            pTilemap->InsertEndChild(pRow);

            for (uint16_t col = 0; col < tilemap[row].size(); col++) {
                pCol = in_doc->NewElement("Col");
                pCol->SetAttribute("col", col);
                pRow->InsertEndChild(pCol);
                pCol->SetText(tilemap[row][col]);
            }
        }
    } else {
        SDL_Log("Problem with tilemap size");
    }

    tinyxml2::XMLElement * pArrivals = in_doc->NewElement("Arrivals");
    tinyxml2::XMLElement * pArrival;
    in_pMap->InsertEndChild(pArrivals);

    for (uint16_t i = 0; i < map_arrivals.size(); i++) {
        pArrival = in_doc->NewElement("Arrival");
        pArrivals->InsertEndChild(pArrival);
        writeXML_arrival(in_doc, pArrival, &map_arrivals[i]);
    }

    tinyxml2::XMLElement * pArrivalEqs = in_doc->NewElement("ArrivalEqs");
    tinyxml2::XMLElement * pArrivalEq = in_doc->NewElement("ArrivalEq");
    in_pMap->InsertEndChild(pArrivalEqs);

    for (uint16_t i = 0; i < arrival_equipments.size(); i++) {
        pArrivalEqs->InsertEndChild(pArrivalEq);
        pArrivalEq->SetAttribute("unitid", map_arrivals[i].id);
        writeXML_items(in_doc, pArrivalEq, arrival_equipments[i]);
    }

    tinyxml2::XMLElement * pUnitmap = in_doc->NewElement("Unitmap");
    tinyxml2::XMLElement * pOnmap;
    in_pMap->InsertEndChild(pUnitmap);
    entityx::ComponentHandle<Unit> tempunit;

    if ((unitmap.size() == bounds[1]) && (unitmap[0].size() == bounds[3])) {
        for (uint16_t row = 0; row < unitmap.size(); row++) {// This loop cache friendly.
            for (uint16_t col = 0; col < unitmap[row].size(); col++) {
                if (unitmap[row][col]) {
                    pOnmap = in_doc->NewElement("OnMap");
                    pOnmap->SetAttribute("row", row);
                    pOnmap->SetAttribute("col", col);
                    pUnitmap->InsertEndChild(pOnmap);
                    unitmap[row][col]->writeXML(in_doc, pOnmap);
                }
            }
        }
    } else {
        SDL_Log("Problem with unitmap bounds");
    }
}

void Map::setTilesize(const uint16_t width, const uint16_t height) {
    tilesize[0] = width;
    tilesize[1] = height;
}

uint16_t * Map::getTilesize() const {
    return ((uint16_t *)tilesize);
}

std::vector<entityx::ComponentHandle<Unit>> Map::getUnits(int8_t in_army) {
    SDL_Log("Map in getUnits");
    return (units_onfield[in_army]);
}

std::vector<int8_t> Map::getArmies() {
    return (armies_onfield);
}

void Map::addArmy(const int8_t in_army) {
    if (!cppisin(in_army, armies_onfield)) {
        armies_onfield.push_back(in_army);
    }

}
void Map::putUnit(const uint16_t x, const uint16_t y, entityx::ComponentHandle<Unit> in_unit) {
    unitmap[y][x] = in_unit;// unitmap[row][col]
    uint8_t army = in_unit->getArmy();
    addArmy(army);
    units_onfield[army].push_back(in_unit);
}

entityx::ComponentHandle<Unit> Map::getUnit(const uint16_t x, const uint16_t y) {
    return (unitmap[y][x]);
}

void Map::moveUnit(const uint16_t x, const uint16_t y, const uint16_t new_x, const uint16_t new_y) {
    SDL_Log("Move Unit %d %d %d %d", x, y, new_x, new_y);
    entityx::ComponentHandle<Unit> buffer = unitmap[new_y][new_x];
    unitmap[new_y][new_x] = unitmap[y][x];
    unitmap[y][x] = buffer;
}

void Map::setArrivalEquipments(const std::vector<std::vector<Inventory_item>> in_arrival_equipments) {
    arrival_equipments = in_arrival_equipments;
}

std::vector<std::vector<Inventory_item>> Map::getArrivalEquipments() {
    return (arrival_equipments);
}

void Map::setArrivals(const std::vector<Map_arrival> in_arrivals) {
    map_arrivals = in_arrivals;
}

void Map::addArrival(const Map_arrival in_arrival) {
    map_arrivals.push_back(in_arrival);
}

void Map::removeMapArrival(const uint8_t index) {
    loaded_map_arrivals.push_back(map_arrivals[index]);
    map_arrivals.erase(map_arrivals.begin() + index);
}

std::vector<Map_arrival> Map::getArrivals() {
    return (map_arrivals);
}

std::vector<std::vector<int16_t>> Map::makeMvtCostmap(entityx::ComponentHandle<Unit> in_unit) {
    SDL_Log("Making MvtCostmap");
    int16_t tile_ind = 0;
    std::vector<std::vector<int16_t>> costmap((int16_t)tilemap.size(), std::vector<int16_t> ((int16_t)tilemap[0].size()));
    char unitmovetype = in_unit->getMvttype();
    uint8_t army = in_unit->getArmy();
    uint8_t ontile_army;
    entityx::ComponentHandle<Unit> unitontile;

    if (unitmovetype > 0) {
        for (int16_t row = 0; row < tilemap.size(); row++) {
            for (int16_t col = 0; col < tilemap[row].size(); col++) {
                tile_ind = tilemap[row][col] / DEFAULT::TILE_DIVISOR;
                // SDL_Log("tile_ind: %d", tile_ind);

                unitontile = unitmap[row][col];
                costmap[row][col] = tiles[tile_ind].getCost()[unitmovetype];

                if (unitontile) {
                    // SDL_Log("Unit on tile: %d %d", col, row);
                    ontile_army = unitontile->getArmy();

                    if (!isFriendly(ontile_army, army)) {
                        costmap[row][col] = 0;
                    }
                }
            }
        }
    }

    return (costmap);
}

std::unordered_map<int16_t, Tile> Map::getTiles() {
    return (tiles);
}
std::vector<int16_t> Map::getTilesindex() {
    return (tilesindex);
}

void Map::loadTiles(std::vector<int16_t> to_load) {
    tilesindex = to_load;
    tilenames = getTilenames(tilesindex);
    loadTilesJSON();
}

void Map::unloadTiles(std::vector<int16_t> to_unload) {
    for (uint16_t i = 0; i < to_unload.size(); i++) {
        tiles.erase(to_unload[i]);
    }
}

std::vector<std::vector<int16_t>> Map::getTilemap() {
    return (tilemap);
}

void Map::setTilemap(const std::vector<std::vector<int16_t>> in_tilemap) {
    tilemap = in_tilemap;
    postTilemap();
}

uint8_t Map::getTurn() {
    return (turn);
}

void Map::setRenderer(SDL_Renderer * in_renderer) {
    SDL_Log("Setting Map renderer");

    if (in_renderer) {
        renderer = in_renderer;
    } else {
        SDL_Log("Could not set map renderer");
    }

    loadOverlays();
    loadDanger();
}

void Map::setManager(entityx::EntityManager * in_manager) {
    manager = in_manager;
}

void Map::loadTiletextures() {
    uint16_t tile_ind;
    std::string texturename;

    for (uint16_t i = 0; i < tilesindex.size(); i++) {
        tile_ind = (tilesindex[i] / DEFAULT::TILE_DIVISOR);
        texturename = "..//assets//Tiles//" + tiles[tile_ind].getName() + "_" + std::to_string(tilesindex[i]) + ".png";
        textures[tilesindex[i]] = loadTexture(renderer, texturename.c_str());
    }
}

void Map::loadOverlays() {
    SDL_Log("Loading Map overlays");
    overlays[0] = loadTexture(renderer, "tile_overlay_move.png", true);
    overlays[1] = loadTexture(renderer, "tile_overlay_attack.png", true);
    overlays[2] = loadTexture(renderer, "tile_overlay_heal.png", true);
}

void Map::loadDanger() {
    SDL_Log("Loading Map dangerzone");
    dangers[0] = loadTexture(renderer, "..//assets//Tiles//tile_overlay_danger.png", false);
}

void Map::loadGrid() {
}

void Map::addDanger(const std::vector<std::vector<int16_t>> in_danger) {
    dangeroverlay = matrix_plus(dangeroverlay, in_danger);
}

void Map::subDanger(const std::vector<std::vector<int16_t>> in_danger) {
    dangeroverlay = matrix_plus(dangeroverlay, in_danger, -1);
}

void Map::setDanger(const std::vector<std::vector<int16_t>> in_danger) {
    dangeroverlay = in_danger;
}

void Map::setOverlaymode(const uint8_t in_mode) {
    char message[DEFAULT::BUFFER_SIZE];
    sprintf(message, "Set Map ovelay mode: %d", in_mode);
    SDL_Log(message);
    overlay_mode = in_mode;
}

void Map::setDangermode(const uint8_t in_mode) {
    danger_mode = in_mode;
}

void Map::showDanger() {
    show_danger = true;
}
bool Map::isDanger() {
    return (show_danger);
}

void Map::switchDanger() {
    show_danger = !show_danger;
}

void Map::hideDanger() {
    show_danger = false;
}

void Map::showOverlay() {
    show_overlay = true;
}

void Map::hideOverlay() {
    show_overlay = false;
}

std::vector<std::vector<entityx::ComponentHandle<Unit>>> Map::getUnitmap() {
    return (unitmap);
}

void Map::setOffset(int16_t xoffset, int16_t yoffset) {
    offset[0] = xoffset;
    offset[1] = yoffset;
}

void Map::setOffset(int16_t in_offset[2]) {
    offset[0] = in_offset[0];
    offset[1] = in_offset[1];
}

int16_t * Map::getOffset() {
    return (offset);
}

void Map::postTilemap() {
    loadTiletextures();
    bounds[0] = offset[0];
    bounds[1] = tilemap[0].size() - 1 + offset[0];
    bounds[2] = offset[1];
    bounds[3] = tilemap.size() - 1 + offset[1];
    std::vector<std::vector<entityx::ComponentHandle<Unit>>> tempunit(tilemap.size(), std::vector<entityx::ComponentHandle<Unit>>(tilemap[0].size()));
    unitmap = tempunit;
    SDL_Log("unitmap size: %d %d", unitmap[0].size(), unitmap.size());
    SDL_Log("tilemap size: %d %d", tilemap[0].size(), tilemap.size());
    SDL_Log("bounds: %d %d %d %d", bounds[0], bounds[1], bounds[2], bounds[3]);
}

int16_t * Map::getBounds() {
    return (bounds);
}

uint8_t Map::getnumArrivals() {
    return (num_enemies);
}

uint16_t Map::getBoss() {
    return (boss);
}

bool Map::getBossDeath() {
    return (bossdied);
}

bool Map::getSeized() {
    return (bossdied);
}

std::vector<uint16_t> Map::getEssentials() {
    return (essentials);
}

void Map::setOverlay(const uint8_t in_mode, const std::vector<std::vector<int16_t>> in_map) {
    if ((in_mode & MAP::OVERLAY::HEAL) > 0) {
        healoverlay = in_map;
    }

    if ((in_mode & MAP::OVERLAY::ATTACK) > 0) {
        attackoverlay = in_map;
    }

    if ((in_mode & MAP::OVERLAY::MOVE) > 0) {
        moveoverlay = in_map;
    }
}

void Map::clearOverlays() {
    attackoverlay.clear();
    moveoverlay.clear();
    healoverlay.clear();
    overlay_mode = 0;
}

void Map::draw() {
    // SDL_Log("Drawing Map");
    uint16_t tile_ind = 0;

    for (uint16_t row = 0; row < tilemap.size(); row++) {// This loop cache friendly.
        for (uint16_t col = 0; col < tilemap[row].size(); col++) {
            tile_ind = tilemap[row][col];
            destrect.x = (col + offset[0]) * tilesize[0];
            destrect.y = (row + offset[1]) * tilesize[1];
            SDL_RenderCopy(renderer, textures[tile_ind], &srcrect, &destrect);

            if (show_overlay) {
                if (((overlay_mode & MAP::OVERLAY::MOVE) > 0) && (overlays[0] != NULL)) {
                    if (moveoverlay[row][col] == 1) {
                        SDL_RenderCopy(renderer, overlays[0], &srcrect, &destrect);
                    }
                }

                if (((overlay_mode & MAP::OVERLAY::ATTACK) > 0)  && (overlays[1] != NULL)) {
                    if (attackoverlay[row][col] == 1) {
                        SDL_RenderCopy(renderer, overlays[1], &srcrect, &destrect);
                    }
                }

                if (((overlay_mode & MAP::OVERLAY::HEAL) > 0) && (overlays[2] != NULL)) {
                    if (healoverlay[row][col] == 1) {
                        SDL_RenderCopy(renderer, overlays[2], &srcrect, &destrect);
                    }
                }
            }

            if (show_danger) {
                if (dangeroverlay[row][col] > 0) {
                    SDL_RenderCopy(renderer, dangers[0], &srcrect, &destrect);
                }
            }
        }
    }
}

std::vector<std::string> mapNames = {"map_test.json"};
std::vector<int16_t> baseParty() {
    std::vector<int16_t> out = {UNIT::NAME::ERWIN, UNIT::NAME::KIARA};
    return (out);
}