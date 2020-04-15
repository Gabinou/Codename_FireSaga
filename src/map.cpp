#include "map.hpp"
#include "position.hpp"
#include "sprite.hpp"

// ECS_DEFINE_TYPE(Map);

Map::Map() {
    initVars();
    overlay_mode = MAP::OVERLAY::MOVE + MAP::OVERLAY::ATTACK;
    setXMLElement("Map");
}

Map::~Map() {
    
}

Map::Map(const short unsigned int width, const short unsigned int height) : Map() {
    setTilesize(width, height);
    srcrect.w = destrect.w = width;
    srcrect.h = destrect.h = height;
}

void Map::readXML(tinyxml2::XMLElement * in_pMap) {
    const char * buffer;
    int buffint;
    tinyxml2::XMLElement * ptemp;

    chapter = (unsigned short int)in_pMap->IntAttribute("chapter");
    tinyxml2::XMLElement * pTiles = in_pMap->FirstChildElement("Tiles");
    tinyxml2::XMLElement * pTile;
    tilenames.clear();
    tilesindex.clear();
    if (pTiles) {
        pTile = pTiles->FirstChildElement("Tile");
        while (pTile) {
            tilesindex.push_back(pTile->IntAttribute("id"));
            ptemp = pTile->FirstChildElement("Name");
            tilenames.push_back(ptemp->GetText());
            pTile = pTile->NextSiblingElement("Tile");
        }
    } else {
        SDL_Log("Cannot get Tiles element");
    }

    tinyxml2::XMLElement * pBounds = in_pMap->FirstChildElement("Bounds");
    if (!pBounds) {SDL_Log("Cannot get Bounds element");}
    ptemp = pBounds->FirstChildElement("row_max");
    ptemp->QueryIntText(&buffint);
    bounds[1] = buffint;
    ptemp = pBounds->FirstChildElement("col_max");
    ptemp->QueryIntText(&buffint);
    bounds[3] = buffint;

    tilemap.clear();

    tinyxml2::XMLElement * pTilemap = in_pMap->FirstChildElement("Tilemap");
    tinyxml2::XMLElement * pRow = pTilemap->FirstChildElement("Row");
    tinyxml2::XMLElement * pCol;
    int tempcol;
    int temprow;
    tilemap.clear();
    std::vector<short int> tempvec;
    while(pRow) {
        pCol = pRow->FirstChildElement("Col");
        temprow = pRow->IntAttribute("row");
        tempvec.clear();
        while(pCol) {
            pCol->QueryIntText(&buffint);
            tempcol = pCol->IntAttribute("col");
            tempvec.push_back(buffint);
            pCol = pCol->NextSiblingElement("Col");
            // SDL_Log("%d %d %d", temprow, tempcol, buffint);
        }
        tilemap.push_back(tempvec);
        pRow = pRow->NextSiblingElement("Row");
    }

    std::vector<std::vector<short int>> unitmap ((short int)bounds[3], std::vector<short int> ((short int)bounds[1]));
    tinyxml2::XMLElement * pArrivals = in_pMap->FirstChildElement("Arrivals");
    tinyxml2::XMLElement * pArrival = pArrivals->FirstChildElement("Arrival");
    Map_arrival temp_arrival;
    while (pArrival) {
        readXML_arrival(pArrival, &temp_arrival);
        map_arrivals.push_back(temp_arrival);
        pArrival = pArrival->NextSiblingElement("Arrival");
    }

}

void Map::writeXML(tinyxml2::XMLDocument * in_doc, tinyxml2::XMLElement * in_pMap) {
    char buffer[DEFAULT::BUFFER_SIZE];
    in_pMap->SetAttribute("chapter", chapter);
    
    tinyxml2::XMLElement * pTiles = in_doc->NewElement("Tiles");
    in_pMap->InsertEndChild(pTiles);
    tinyxml2::XMLElement * pTile;
    tinyxml2::XMLElement * pName;

    if (tilesindex.size() == tilenames.size()) {
        for (int i = 0; i < tilesindex.size(); i++) {
            pTile = in_doc->NewElement("Tile");
            pName = in_doc->NewElement("Name");
            if (tilesindex[i] >= 100) {
                pTile->SetAttribute("id", tilesindex[i]/DEFAULT::TILE_DIVISOR);
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

    if ((tilemap.size() == bounds[3]) && (tilemap[0].size() == bounds[1])) {
        for (int row = 0; row < tilemap.size(); row++) {// This loop cache friendly.
            pRow = in_doc->NewElement("Row");
            pRow->SetAttribute("row", row);
            pTilemap->InsertEndChild(pRow);
            for (int col = 0; col < tilemap[row].size(); col++) {
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
    tinyxml2::XMLElement * pArrival = in_doc->NewElement("Arrival");
    in_pMap->InsertEndChild(pArrivals);
    for (int i = 0; i < map_arrivals.size(); i++) {
        pArrivals->InsertEndChild(pArrival);
        writeXML_arrival(in_doc, pArrival, &map_arrivals[i]);
    }
    tinyxml2::XMLElement * pArrivalEqs = in_doc->NewElement("ArrivalEqs");    
    tinyxml2::XMLElement * pArrivalEq = in_doc->NewElement("ArrivalEq");    
    in_pMap->InsertEndChild(pArrivalEqs);
    for (int i = 0; i < arrival_equipments.size(); i++) {
        pArrivalEqs->InsertEndChild(pArrivalEq);
        pArrivalEq->SetAttribute("unitid", map_arrivals[i].id);
        writeXML_items(in_doc, pArrivalEq, arrival_equipments[i]);
    }

    SDL_Log("until here");
    tinyxml2::XMLElement * pUnitMap = in_doc->NewElement("UnitMap");
    tinyxml2::XMLElement * pOnmap;
    in_pMap->InsertEndChild(pUnitMap);
    entityx::ComponentHandle<Unit> tempunit;
    if ((unitmap.size() == bounds[3]) && (unitmap[0].size() == bounds[1])) {
        for (int row = 0; row < unitmap.size(); row++) {// This loop cache friendly.
            for (int col = 0; col < unitmap[row].size(); col++) {
                // tempunit = unitmap[row][col];
                if (unitmap[row][col]) {
                    SDL_Log("Found unit on Map");
                    pOnmap = in_doc->NewElement("OnMap");
                    pOnmap->SetAttribute("row", row);
                    pOnmap->SetAttribute("col", col);
                    pUnitMap->InsertEndChild(pOnmap);
                    unitmap[row][col]->writeXML(in_doc, pOnmap); 
                }
            }
        }
    } else {
        SDL_Log("Problem with unitmap bounds");
    }
}

void Map::setTilesize(const short int unsigned width, const short int unsigned height) {
    tilesize[0] = width;
    tilesize[1] = height;
}

short unsigned int * Map::getTilesize() const {
    return ((short unsigned int *)tilesize);
}

void Map::putUnit(const short unsigned int x, const short unsigned int y, entityx::ComponentHandle<Unit> in_unit) {
    unitmap[x][y] = in_unit; 
}

entityx::ComponentHandle<Unit> Map::getUnit(const short unsigned int x, const short unsigned int y) {
    return(unitmap[x][y]);
}

void Map::moveUnit(const short unsigned int x, const short unsigned int y, const short unsigned int new_x, const short unsigned int new_y) {
    SDL_Log("Move Unit %d %d %d %d", x, y, new_x, new_y);
    entityx::ComponentHandle<Unit> buffer = unitmap[new_x][new_y];
    unitmap[new_x][new_y] = unitmap[x][y];
    unitmap[x][y] = buffer;
}

void Map::setArrivalEquipments(const std::vector<std::vector<Inventory_item>> in_arrival_equipments) {
    arrival_equipments = in_arrival_equipments;
}

std::vector<std::vector<Inventory_item>> Map::getArrivalEquipments() {
    return(arrival_equipments);
}

void Map::setArrivals(const std::vector<Map_arrival> in_arrivals) {
    map_arrivals = in_arrivals;
}

void Map::addArrival(const Map_arrival in_arrival) {
    map_arrivals.push_back(in_arrival);
}

void Map::removeMapArrival(const unsigned char index) {
    loaded_map_arrivals.push_back(map_arrivals[index]);
    map_arrivals.erase(map_arrivals.begin() + index);
}

std::vector<Map_arrival> Map::getArrivals() {
    return(map_arrivals);
}

std::vector<std::vector<short int>> Map::makeMvtCostmap(const unsigned char unitmovetype) {
    // SDL_Log("Making MvtCostmap");
    short int tile_ind = 0;
    std::vector<std::vector<short int>> costmap((short int)tilemap.size(), std::vector<short int> ((short int)tilemap[0].size()));
    for (short int row = 0; row < tilemap.size(); row++) {
        for (short int col = 0; col < tilemap[row].size(); col++) {
            tile_ind = tilemap[row][col]/DEFAULT::TILE_DIVISOR;
            costmap[row][col] = tiles[tile_ind].getCost()[unitmovetype];
        }
    }
    return(costmap);
}
void Map::loadTiles(const int in_map_index) {
    tilesindex = chapTiles[in_map_index]();
    tilenames = getTilenames(tilesindex);
    baseTiles(&tiles, tilesindex);
}

void Map::loadTiles(std::vector<short int> to_load) {
    tilesindex = to_load;
    tilenames = getTilenames(tilesindex);
    baseTiles(&tiles, tilesindex);
}

void Map::unloadTiles(std::vector<short int> to_unload) {
    for (int i = 0; i < to_unload.size(); i++) {
        tiles.erase(to_unload[i]);
    }
}

std::vector<std::vector<short int>> Map::getTilemap(){
    return(tilemap);
}

void Map::setTilemap(const std::vector<std::vector<short int>> in_tilemap){
    tilemap = in_tilemap;
}

unsigned char Map::getTurn() {
    return(turn);
}

void Map::setRenderer(SDL_Renderer * in_renderer){
    SDL_Log("Setting Map renderer");
    if (in_renderer) {
        renderer = in_renderer;
    } else {
        SDL_Log("Could not set map renderer");
    }
    loadOverlays();
}

void Map::loadTiletextures() {
    short unsigned int tile_ind;
    std::string texturename;
    for (short unsigned int i = 0; i < tilesindex.size(); i++) {
        tile_ind = (tilesindex[i]/DEFAULT::TILE_DIVISOR);
        texturename = "..//assets//" + tiles[tile_ind].getName() + "_" + std::to_string(tilesindex[i]) + ".png";
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
    dangers[0] = loadTexture(renderer, "..//assets//danger.png", false);
    // dangers[1] = loadTexture("..//assets//danger_grid.png");
}

void Map::loadGrid() {
}

void Map::addDanger(const std::vector<std::vector<short int>> in_danger) {
    dangeroverlay = matrix_plus(dangeroverlay, in_danger);
}

void Map::subDanger(const std::vector<std::vector<short int>> in_danger) {
    dangeroverlay = matrix_plus(dangeroverlay, in_danger, -1);
}

void Map::setOverlaymode(const unsigned char in_mode) {
    char message[DEFAULT::BUFFER_SIZE];
    sprintf(message, "Set Map ovelay mode: %d", in_mode);
    SDL_Log(message);
    overlay_mode = in_mode;
}

void Map::setDangermode(const unsigned char in_mode) {
    danger_mode = in_mode;
}

void Map::showDanger(){
    show_danger = true;
}

void Map::hideDanger(){
    show_overlay = false;
}

void Map::showOverlay(){
    show_overlay = true;
}

void Map::hideOverlay(){
    show_overlay = false;
}

void Map::initVars() {
    srcrect.x = srcrect.y = 0;
    destrect.x = destrect.y = 0;
    setTilesize(DEFAULT::TILESIZE, DEFAULT::TILESIZE);
    srcrect.w = destrect.w = DEFAULT::TILESIZE;
    srcrect.h = destrect.h = DEFAULT::TILESIZE;
}

// std::vector<std::vector<entityx::Entity *>> Map::getEntitymap() {
//     return(entitymap);
// }

std::vector<std::vector<entityx::ComponentHandle<Unit>>> Map::getUnitmap() {
    return(unitmap);
}

void Map::loadTilemap(const short unsigned int in_map_index) {
    tilemap = chapTilemaps[in_map_index]();
    postTilemap();
}

void Map::postTilemap() {
    loadTiletextures();
    short unsigned int col_size = tilemap.size();
    short unsigned int row_size = tilemap[0].size();
    bounds[1] = row_size;
    bounds[3] = col_size;
    std::vector<std::vector<entityx::ComponentHandle<Unit>>> tempunit(row_size, std::vector<entityx::ComponentHandle<Unit>>(col_size));
    unitmap = tempunit; 
}

short int * Map::getBounds() {
    return(bounds);
}

unsigned char Map::getnumArrivals() {
    return(num_enemies);
}

unsigned short int Map::getBoss() {
    return(boss);
}

bool Map::getBossDeath() {
    return(bossdied);
}

bool Map::getSeized() {
    return(bossdied);
}

std::vector<unsigned short int> Map::getEssentials() {
    return(essentials);
}

void Map::setOverlay(const unsigned char in_mode, const std::vector<std::vector<short int>> in_map) {
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
    int tile_ind = 0;
    for (int row = 0; row < tilemap.size(); row++) {// This loop cache friendly.
        for (int col = 0; col < tilemap[row].size(); col++) {
            tile_ind = tilemap[row][col];
            destrect.x = (col + 1) * tilesize[0];
            destrect.y = (row + 1) * tilesize[1];
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
                if (dangeroverlay[row][col] > 1) {
                    SDL_RenderCopy(renderer, dangers[0], &srcrect, &destrect);
                }
            }
        }
    }
}

std::vector<std::vector<short int>> testTilemap(){
    std::vector<std::vector<short int>> tilemap = { 
        {100, 100, 100, 100, 100, 100, 200, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 300, 300, 300, 300, 300, 100, 100, 100},
        {100, 100, 100, 100, 100, 100, 200, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 300, 300, 300, 300, 300, 100, 100, 100},
        {100, 100, 100, 100, 100, 100, 200, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 300, 300, 300, 300, 300, 100, 100, 100},
        {100, 100, 100, 100, 100, 100, 200, 200, 100, 100, 100, 100, 100, 100, 100, 100, 100, 300, 300, 300, 300, 300, 100, 100, 100},
        {100, 100, 100, 100, 100, 100, 100, 200, 100, 100, 100, 100, 100, 100, 100, 100, 100, 300, 300, 300, 300, 300, 100, 100, 100},
        {100, 100, 100, 100, 100, 100, 100, 200, 200, 100, 100, 100, 100, 100, 100, 100, 100, 300, 300, 300, 300, 100, 100, 100, 100},
        {100, 100, 100, 100, 100, 100, 100, 100, 200, 100, 100, 100, 100, 100, 100, 100, 100, 300, 300, 300, 100, 300, 100, 100, 100},
        {100, 100, 100, 100, 100, 100, 100, 100, 200, 100, 100, 100, 100, 100, 100, 100, 100, 300, 300, 100, 100, 100, 100, 100, 100},
        {100, 100, 100, 100, 100, 100, 100, 100, 200, 200, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100},
        {100, 100, 100, 100, 100, 100, 100, 100, 200, 200, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 120, 100, 100, 100, 100},
        {100, 100, 100, 100, 100, 100, 100, 100, 100, 200, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 120, 120, 100, 100, 100},
        {100, 100, 100, 100, 100, 100, 100, 100, 100, 200, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100},
        {100, 100, 100, 100, 100, 100, 100, 100, 100, 200, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 120, 100, 100, 100},
        {100, 100, 100, 100, 100, 100, 100, 100, 100, 300, 100, 100, 100, 100, 100, 100, 100, 100, 100, 120, 120, 120, 100, 100, 100},
        {100, 100, 100, 100, 100, 100, 100, 100, 100, 200, 100, 100, 100, 100, 100, 100, 100, 100, 100, 120, 100, 100, 100, 100, 100},
        {100, 100, 100, 100, 100, 100, 100, 100, 100, 200, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100},
        {100, 100, 100, 100, 100, 100, 100, 100, 100, 200, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100},
        {100, 100, 100, 100, 100, 100, 100, 100, 100, 200, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100},
        {100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100},
        {100, 100, 100, 100, 100, 100, 100, 100, 100, 200, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100},
        {100, 100, 100, 100, 100, 100, 100, 100, 100, 200, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100}
    };
    return(tilemap);
}

std::vector<short unsigned int> testArrivalinds(){
    std::vector<short unsigned int> enemies = {UNIT::NAME::SILOU};
    return(enemies);
}

std::vector<std::vector<short int>> (*chapTilemaps[40])() = {testTilemap,};
std::vector<short unsigned int> (*chapArrivalinds[40])() = {testArrivalinds,};


void testXMLMap() {
    SDL_Log("Testing Map xml writing and reading\n");
    // Must be run when units are in memory.
    Map map(32, 32); // mapx is a pointer
    map.loadTiles(0);
    map.loadTilemap(0);
    map.setArrivals(mapArrivals[0]());
    map.setArrivalEquipments(arrivalEquipments[0]());
    std::string asset_name;
    entityx::EntityX ex;
    entityx::Entity Uent = ex.entities.create();;
    Unit temp_unit;
    Unit_stats temp;
    Inventory_item temp_wpn;
    std::vector<short int> temp_supports;
    Equipped temp_equipped;
    temp = {15,  4,  5,  7,  6,   8,  4,  6,  5,  5,  6};
    temp_unit = Unit(UNIT::NAME::SILOU, UNIT::CLASS::MAGE, temp, UNIT::SEX::F);
    temp = {48, 14, 25, 32, 34,  28, 19, 40, 15};
    temp_unit.setCaps(temp);
    temp = {60, 50, 20, 60, 70,  40, 30, 20,  10, 0};
    temp_unit.setGrowths(temp);
    temp_unit.setBaseExp(400);
    temp_wpn.id = ITEM::NAME::BALL_LIGHTNING;
    temp_unit.addEquipment(temp_wpn);

    asset_name = "..//assets//" +  temp_unit.getName() + ".png";
    Uent.assign<Unit>(temp_unit);
    Uent.assign<Position>(6, 6);
    Uent.assign<Sprite>(asset_name.c_str());
    map.putUnit(6, 6, Uent.component<Unit>());
    map.writeXML("map_test.xml");

    Map test;
    test.readXML("map_test.xml");
    test.writeXML("map_rewrite.xml");

}
