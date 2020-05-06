
#include "systems.hpp"

void RenderSystemx::setRenderer(SDL_Renderer * in_renderer) {
    if (in_renderer) {
        SDL_Log("Added renderer to renderSystemx");
        renderer = in_renderer;
    } else {
        SDL_Log("RenderSystemx: Renderer is invalid");
    }
}

RenderSystemx::RenderSystemx() {
    offset[0] = DEFAULT::TILEMAP_XOFFSET;
    offset[1] = DEFAULT::TILEMAP_YOFFSET;
}

RenderSystemx::RenderSystemx(SDL_Renderer * in_renderer) {
    setRenderer(in_renderer);
}

void RenderSystemx::setMap(entityx::ComponentHandle<Map> in_map) {
    if (in_map) {
        mapx = in_map;
        tilesize = mapx->getTilesize();
    } else {
        SDL_Log("RenderSystemx: Map Handle is invalid");
    }
}

void RenderSystemx::setTilesize(const short int unsigned width, const short int unsigned height) {
    tilesize[0] = width;
    tilesize[1] = height;
}

void RenderSystemx::setLinespace(const short int unsigned in_linespace) {
    linespace = in_linespace;
}

void RenderSystemx::slideSprites(entityx::Entity * in_ent, short int * slidepos, short int * objectivepos) {
    if (in_ent->valid()) {
        entityx::ComponentHandle<GamepadController> gamepad = in_ent->component<GamepadController>();
        entityx::ComponentHandle<KeyboardController> keyboard = in_ent->component<KeyboardController>();
        entityx::ComponentHandle<Sprite> sprite = in_ent->component<Sprite>();
        entityx::ComponentHandle<Position> position = in_ent->component<Position>();
        short int kb_held = 0;
        short int gp_held = 0;
        short int slideint = sprite->getSlideint();
        unsigned char slidetype = sprite->getSlidetype();
        float * slidefactors = sprite->getSlidefactors();
        short int scalefactor[2];
        SDL_Rect destrect = sprite->getDestrect();

        scalefactor[0] = tilesize[0];
        scalefactor[1] = tilesize[1];

        if (keyboard) {
            kb_held = keyboard->getHeldmove();
        }

        if (gamepad) {
            gp_held = gamepad->getHeldmove();
        }

        if ((!keyboard) && (!gamepad)) {
            if (!position->isonTilemap()) { //move on the menu space
                scalefactor[0] = 1;
                scalefactor[1] = 1 ;
            }

            slidepos[0] = (int)(position->getPos()[0] * scalefactor[0]);
            slidepos[1] = (int)(position->getPos()[1] * scalefactor[1]);
        } else {
            if (!position->isonTilemap()) { //move on the menu space
                scalefactor[0] = linespace;
                scalefactor[1] = linespace;
            }

            switch (slidetype) {
                case SLIDETYPE::GEOMETRIC: //for cursor mvt on mapx.
                    objectivepos[0] = (int)(position->getPos()[0]) * (scalefactor[0]) - destrect.w / 4;
                    objectivepos[1] = (int)(position->getPos()[1]) * (scalefactor[1]) - destrect.h / 4;

                    if ((gp_held > DEFAULT::CURSOR_FAST) || (kb_held > DEFAULT::CURSOR_FAST))  {
                        slideint = 1;
                    }

                    if (objectivepos[0] != slidepos[0]) {
                        slidepos[0] += geometricslide((objectivepos[0] - slidepos[0]), slidefactors[slideint]);
                    }

                    if (objectivepos[1] != slidepos[1]) {
                        slidepos[1] += geometricslide((objectivepos[1] - slidepos[1]), slidefactors[slideint]);
                    }

                    if ((objectivepos[0] == slidepos[0]) && (objectivepos[1] == slidepos[1]) && ((kb_held > DEFAULT::CURSOR_PAUSE) || (kb_held < 2)) && ((gp_held > DEFAULT::CURSOR_PAUSE) || (gp_held < 2))) {
                        position->setUpdatable(true);
                        slideint = 0;
                    } else {
                        position->setUpdatable(false);
                    }

                    break;
            }
        }
    } else {
        SDL_Log("slideSprites: Entity is invalid");
    }
}

SDL_Rect RenderSystemx::loopSprites(entityx::ComponentHandle<Sprite> in_sprite) {
    SDL_Rect srcrect;

    if (in_sprite) {
        short int frames = in_sprite->getFrames();
        short int speed = in_sprite->getSpeed();
        srcrect = in_sprite->getSrcrect();
        unsigned char looping = in_sprite->getSs_looping();

        switch (looping) {
            case LOOPING::PINGPONG:
                srcrect.x = srcrect.w * pingpong(static_cast<int>(SDL_GetTicks() / speed), frames, 0);
                break;

            case LOOPING::LINEAR:
            case LOOPING::DIRECT:
                srcrect.x = srcrect.w * static_cast<int>((SDL_GetTicks() / speed) % frames);
                break;

            case LOOPING::REVERSE:
                srcrect.x = srcrect.w * (frames - static_cast<int>((SDL_GetTicks() / speed) % frames));
                break;
        }
    }

    return (srcrect);
}

void RenderSystemx::update(entityx::EntityManager & es, entityx::EventManager & events, entityx::TimeDelta dt) {
    SDL_RenderClear(renderer);

    for (entityx::Entity ent : es.entities_with_components<Map>()) {
        entityx::ComponentHandle<Map> mapx = ent.component<Map>();
        mapx->draw();
    }

    for (entityx::Entity ent : es.entities_with_components<Sprite, Position>()) {
        entityx::ComponentHandle<Sprite> sprite = ent.component<Sprite>();

        if (!ent.has_component<Text>()) {
            short int * slidepos = sprite->getSlidepos();
            short int * objectivepos = sprite->getObjpos();
            SDL_Rect srcrect = sprite->getSrcrect();
            SDL_Rect destrect = sprite->getDestrect();

            if (sprite->isAnimated()) {
                srcrect = loopSprites(sprite);
            }

            slideSprites(&ent, slidepos, objectivepos);

            destrect.x = slidepos[0];
            destrect.y = slidepos[1];

            sprite->setSlidepos(slidepos);
            sprite->setObjpos(objectivepos);
            sprite->setSrcrect(srcrect);
            sprite->setDestrect(destrect);
            sprite->draw();
        }
    }

    for (entityx::Entity ent : es.entities_with_components<Text, Position>()) {
        entityx::ComponentHandle<Position> position = ent.component<Position>();
        entityx::ComponentHandle<Text> text = ent.component<Text>();
        short int * pos = position->getPos();
        // SDL_Log("unit menu position: %d %d", position[0], position[1]);

        if (ent.has_component<Sprite>()) {
            ent.component<Sprite>()->setDestrectpos(pos);
            ent.component<Sprite>()->draw();
        }

        text->setRects(pos);
        text->draw();
    }

    for (entityx::Entity ent : es.entities_with_components<KeyboardController>()) {
        entityx::ComponentHandle<Sprite> sprite = ent.component<Sprite>();

        // SDL_Log("Rendering Cursor");

        short int * slidepos = sprite->getSlidepos();
        short int * objectivepos = sprite->getObjpos();
        SDL_Rect srcrect = sprite->getSrcrect();
        SDL_Rect destrect = sprite->getDestrect();

        if (sprite->isAnimated()) { //looping sprites.
            srcrect = loopSprites(sprite);
        }

        slideSprites(&ent, slidepos, objectivepos);

        destrect.x = slidepos[0];
        destrect.y = slidepos[1];

        sprite->setSlidepos(slidepos);
        sprite->setObjpos(objectivepos);
        sprite->setSrcrect(srcrect);
        sprite->setDestrect(destrect);
        sprite->draw();
    }

    SDL_RenderPresent(renderer);
}

void RenderSystemx::configure(entityx::EventManager & in_events) {
    event_manager = &in_events;
    event_manager->subscribe<turnBegin>(*this);
    event_manager->subscribe<turnEnd>(*this);
    event_manager->subscribe<unitHeal>(*this);
    event_manager->subscribe<unitWait>(*this);
    event_manager->subscribe<unitDie>(*this);
    event_manager->subscribe<unitRefresh>(*this);
}

void RenderSystemx::receive(const turnBegin & begin) {
    SDL_Log("Received turnBegin event");

}

void RenderSystemx::receive(const turnEnd & end) {

}

void RenderSystemx::receive(const unitHeal & heal) {

}

void RenderSystemx::receive(const unitDie & death) {

}

void RenderSystemx::receive(const unitWait & wait) {

}

void RenderSystemx::receive(const unitRefresh & refresh) {

}

MenuSystemx::MenuSystemx() {

}

MenuSystemx::MenuSystemx(Game * in_game) {
    SDL_Log("Adding Menusystem");
    game = in_game;
    unitmenux = game->getMenu(MENU::UNIT);
    settings = game->getSettings();
    updateMap();
}

void MenuSystemx::configure(entityx::EventManager & in_events) {
    event_manager = &in_events;
    event_manager->subscribe<unitSelect>(*this);
    event_manager->subscribe<unitMenu>(*this);
    event_manager->subscribe<mapMenu>(*this);
}

void MenuSystemx::updateMap() {
    mapx = game->getMap();
}

void MenuSystemx::setMap(entityx::ComponentHandle<Map> in_map) {
    if (in_map) {
        mapx = in_map;
    } else {
        SDL_Log("RenderSystemx: Map Handle is invalid");
    }
}

void MenuSystemx::receive(const unitSelect & select) {
    SDL_Log("unitSelect event received");
    short int newstate = -1;
    entityx::ComponentHandle<Unit> unit = select.unit;

    if (isPC(unit->getArmy())) {
        event_manager->emit<unitMove>(select.cursor, unit);
        newstate = GAME::STATE::UNITMOVE;
    } else {
        if (game->getState() == GAME::STATE::UNITMOVE) {
            event_manager->emit<unitDanger>(select.cursor, unit);
        } else {
            event_manager->emit<unitMove>(select.cursor, unit);
            newstate = GAME::STATE::UNITMOVE;
        }
    }

    if (newstate > 0) {
        game->setState(newstate);
    }
}

void MenuSystemx::receive(const mapMenu & menu) {

}


void MenuSystemx::receive(const unitMenu & menu) {
    SDL_Log("unitMenu event received");
    mapx->hideOverlay();

    entityx::Entity cursor = menu.cursor;
    entityx::ComponentHandle<Position> cursorpos = cursor.component<Position>();

    short int new_position[2];

    entityx::ComponentHandle<Position> selectedpos;

    if (selected.valid()) {
        selectedpos = selected.component<Position>();

        if (selectedpos) {
            old_position[0] = selectedpos->getPos()[0] - selectedpos->getOffset()[0];
            old_position[1] = selectedpos->getPos()[1] - selectedpos->getOffset()[1];
            SDL_Log("Old position %d, %d \n", old_position[0], old_position[1]);
        } else {
            SDL_Log("Could not get selectedx unit component");
        }

    } else {
        SDL_Log("Could not get selected entity");
    }

    if (cursorpos) {
        new_position[0] = cursorpos->getPos()[0] - selectedpos->getOffset()[0];
        new_position[1] = cursorpos->getPos()[1] - selectedpos->getOffset()[1];
        SDL_Log("New position %d, %d \n", new_position[0], new_position[1]);
    } else {
        SDL_Log("Could not get setter(unit) position component");
    }

    mapx->moveUnit(old_position[0], old_position[1], new_position[0], new_position[1]);
    selectedpos->setPos(new_position);
    game->setCursorlastpos(new_position[0], new_position[1]);

    game->makeUnitmenuoptions();

    if (!unitmenux->valid()) {
        game->makeMenu(MENU::UNIT);
    } else {
        game->updateMenu(MENU::UNIT);
    }

    unitmenux->component<Position>()->setPos((new_position[0] + 1) * settings->tilesize[0], new_position[1] * settings->tilesize[1]);
    game->showMenu(MENU::UNIT);
    game->setCursorstate(GAME::STATE::UNITMENU);
}

void MenuSystemx::update(entityx::EntityManager & es, entityx::EventManager & events, entityx::TimeDelta dt) {

}

UnitSystemx::UnitSystemx() {

}

UnitSystemx::UnitSystemx(Game * in_game) {
    SDL_Log("Adding Unitsystem");
    game = in_game;
    settings = game->getSettings();
    unitmenux = game->getMenu(MENU::UNIT);
    updateMap();
}

void UnitSystemx::updateMap() {
    mapx = game->getMap();
}

void UnitSystemx::configure(entityx::EventManager & in_events) {
    event_manager = &in_events;
    event_manager->subscribe<unitSelect>(*this);
    event_manager->subscribe<unitDeselect>(*this);
    event_manager->subscribe<unitDanger>(*this);
    event_manager->subscribe<unitMove>(*this);
    event_manager->subscribe<unitMenu>(*this);
    event_manager->subscribe<unitmenuSelect>(*this);
    event_manager->subscribe<unitMap>(*this);
    event_manager->subscribe<unitHover>(*this);
    event_manager->subscribe<unitDehover>(*this);
    event_manager->subscribe<unitWait>(*this);
    event_manager->subscribe<unitReturn>(*this);
    event_manager->subscribe<unitTalk>(*this);
    event_manager->subscribe<unitRescue>(*this);
    event_manager->subscribe<unitAttack>(*this);
    event_manager->subscribe<unitTrade>(*this);
    event_manager->subscribe<unitEscape>(*this);
    event_manager->subscribe<unitItems>(*this);
}

void UnitSystemx::receive(const unitReturn & Return) {
    SDL_Log("Received unitReturn event");
    mapx->hideOverlay();

    entityx::Entity cursor = Return.cursor;
    entityx::ComponentHandle<Position> cursor_position = cursor.component<Position>();

    short int new_position[2];

    entityx::ComponentHandle<Position> toreturn_position;
    entityx::ComponentHandle<Unit> unit = Return.unit;
    entityx::Entity toreturn = unit.entity();

    if (toreturn.valid()) {
        toreturn_position = toreturn.component<Position>();

        if (toreturn_position) {
            new_position[0] = toreturn_position->getPos()[0] - toreturn_position->getOffset()[0];
            new_position[1] = toreturn_position->getPos()[1] - toreturn_position->getOffset()[1];
            SDL_Log("Old position %d, %d \n", old_position[0], old_position[1]);
        } else {
            SDL_Log("Could not get returning unit's position component");
        }
    } else {
        SDL_Log("Could not get selected entity");
    }

    mapx->moveUnit(new_position[0], new_position[1], old_position[0], old_position[1]);
    toreturn_position->setPos(old_position);
}

void UnitSystemx::receive(const unitDehover & dehover) {
    SDL_Log("Received unitDehover event");

    entityx::Entity cursor = dehover.cursor;
    entityx::ComponentHandle<Unit> unit = dehover.unit;

    if (isPC(unit->getArmy())) {

    } else {
        event_manager->emit<unitMap>(cursor);
    }
}

void UnitSystemx::receive(const unitHover & hover) {
    SDL_Log("Received unitHover event");
}

void UnitSystemx::receive(const unitWait & wait) {
    SDL_Log("Received unitWait event");
    entityx::ComponentHandle<Unit> unit = wait.unit;
    entityx::Entity ent = unit.entity();
    entityx::ComponentHandle<Sprite> sprite = ent.component<Sprite>();
    unit->wait();
    sprite->darken();
    event_manager->emit<unitMap>(wait.cursor);
}

void UnitSystemx::receive(const unitTalk & talk) {
    SDL_Log("unitTalk event received");
    entityx::ComponentHandle<Unit> unit = talk.unit;
    // event_manager->emit<talkMenu>(wait.cursor);
    unit->wait();
    event_manager->emit<unitMap>(talk.cursor);
}

void UnitSystemx::receive(const unitRescue & rescue) {
    SDL_Log("unitRescue event received");
    entityx::ComponentHandle<Unit> unit = rescue.unit;
    // event_manager->emit<rescueMenu>(wait.cursor);
    unit->wait();
    event_manager->emit<unitMap>(rescue.cursor);
}

void UnitSystemx::receive(const unitAttack & attack) {
    SDL_Log("unitAttack event received");
    entityx::ComponentHandle<Unit> unit = attack.unit;
    // event_manager->emit<attackMenu>(wait.cursor);
    unit->wait();
    event_manager->emit<unitMap>(attack.cursor);
}

void UnitSystemx::receive(const unitTrade & trade) {
    SDL_Log("unitTrade event received");
    entityx::ComponentHandle<Unit> unit = trade.unit;
    // event_manager->emit<tradeMenu>(wait.cursor);
    unit->wait();
    event_manager->emit<unitMap>(trade.cursor);
}

void UnitSystemx::receive(const unitEscape & escape) {
    SDL_Log("unitEscape event received");
    entityx::ComponentHandle<Unit> unit = escape.unit;
    unit->wait();
    event_manager->emit<unitMap>(escape.cursor);
}

void UnitSystemx::receive(const unitItems & items) {
    SDL_Log("unitItems event received");
    entityx::ComponentHandle<Unit> unit = items.unit;
    // event_manager->emit<itemMenu>(wait.cursor);
    unit->wait();
    event_manager->emit<unitMap>(items.cursor);
}

void UnitSystemx::receive(const unitDeselect & deselect) {
    SDL_Log("unitDeselect event received");
    entityx::ComponentHandle<Unit> unit = deselect.unit;
    entityx::Entity cursor = deselect.cursor;

    if (isPC(unit->getArmy())) {
        event_manager->emit<unitMap>(cursor);
    } else {
        switch (game->getState()) {
            case GAME::STATE::UNITMOVE:
                if (unit->isDanger()) {
                    event_manager->emit<unitDanger>(cursor, unit);
                } else {
                    event_manager->emit<unitMap>(cursor);
                }

                break;
        }
    }
}

void UnitSystemx::receive(const unitSelect & select) {
    SDL_Log("unitSelect event received");
    short int newstate = -1;
    entityx::ComponentHandle<Unit> unit = select.unit;

    if (isPC(unit->getArmy())) {
        event_manager->emit<unitMove>(select.cursor, unit);
        newstate = GAME::STATE::UNITMOVE;
    } else {
        if (game->getState() == GAME::STATE::UNITMOVE) {
            event_manager->emit<unitDanger>(select.cursor, unit);
        } else {
            event_manager->emit<unitMove>(select.cursor, unit);
            newstate = GAME::STATE::UNITMOVE;
        }
    }

    if (newstate > 0) {
        game->setState(newstate);
    }
}

void UnitSystemx::receive(const unitmenuSelect & select) {
    SDL_Log("unitmenuSelect event received");
    entityx::Entity cursor = select.cursor;
    entityx::ComponentHandle<Unit> unit = select.unit;
    entityx::ComponentHandle<Position> position = cursor.component<Position>();

    short int * cursorpos = position->getPos();
    short int * cursorbounds = position->getBounds();
    unsigned char menuind = cursorpos[1] - cursorbounds[2];

    unitmenuoptions = game->getMenuoptions(MENU::UNIT);
    SDL_Log("menuind: %d ", menuind);
    SDL_Log("unitmenuoptions[menuind]: %d ", unitmenuoptions[menuind]);

    if (unit) {
        switch (unitmenuoptions[menuind]) {
            case UNIT::MENU::ITEMS:
                event_manager->emit<unitItems>(cursor, unit);
                break;

            case UNIT::MENU::TALK:
                event_manager->emit<unitTalk>(cursor, unit);
                break;

            case UNIT::MENU::RESCUE:
                event_manager->emit<unitRescue>(cursor, unit);
                break;

            case UNIT::MENU::SEIZE:
                event_manager->emit<unitSeize>(unit);
                break;

            case UNIT::MENU::ESCAPE:
                event_manager->emit<unitEscape>(cursor, unit);
                break;

            case UNIT::MENU::ATTACK:
                event_manager->emit<unitAttack>(cursor, unit);
                break;

            case UNIT::MENU::TRADE:
                event_manager->emit<unitTrade>(cursor, unit);
                break;

            case UNIT::MENU::STAFF:
                event_manager->emit<unitStaff>(cursor, unit);
                break;

            case UNIT::MENU::WAIT:
                event_manager->emit<unitWait>(cursor, unit);
                break;
        }
    } else {
        SDL_Log("unitmenuSelect: could not get unit");
    }

}

void UnitSystemx::receive(const unitDanger & danger) {
    SDL_Log("unitDanger event received");
    std::vector<std::vector<short int>> costmapp;
    std::vector<std::vector<short int>> movemapp;
    std::vector<std::vector<short int>> attackmapp;
    std::vector<std::vector<short int>> dangermapp;
    short unsigned int unit_move;
    short unsigned int start[2];
    unsigned char * range;

    entityx::ComponentHandle<Unit> unit = danger.unit;
    entityx::Entity cursor = danger.cursor;
    entityx::ComponentHandle<Position> cursorpos = cursor.component<Position>();

    if (cursorpos) {
        start[0] = cursorpos->getPos()[0] - cursorpos->getOffset()[0];
        start[1] = cursorpos->getPos()[1] - cursorpos->getOffset()[1];
    } else {
        SDL_Log("Could not get cursor position component");
    }

    if (unit) {
        unit_move = unit->getStats().move;
        range = unit->getRange();
    } else {
        SDL_Log("Could not get unit component");
    }

    costmapp = mapx->makeMvtCostmap(unit);
    movemapp = movemap(costmapp, start, unit_move, "matrix");
    attackmapp = attackmap(movemapp, start, unit_move, range, "matrix");
    dangermapp = matrix_plus(attackmapp, movemapp);


    if (!unit->isDanger()) {
        if (!mapx->isDanger()) {
            mapx->setDanger(dangermapp);
            mapx->showDanger();
        } else {
            mapx->addDanger(dangermapp);
        }

        unit->showDanger();
    } else {
        mapx->subDanger(dangermapp);
        unit->hideDanger();
    }
}

void UnitSystemx::receive(const unitMenu & menu) {
    SDL_Log("unitMenu event received");
    mapx->hideOverlay();

    entityx::Entity cursor = menu.cursor;
    entityx::ComponentHandle<Position> cursorpos = cursor.component<Position>();

    short int new_position[2];

    entityx::ComponentHandle<Position> selectedpos;

    if (selected.valid()) {
        selectedpos = selected.component<Position>();

        if (selectedpos) {
            old_position[0] = selectedpos->getPos()[0] - selectedpos->getOffset()[0];
            old_position[1] = selectedpos->getPos()[1] - selectedpos->getOffset()[1];
            SDL_Log("Old position %d, %d \n", old_position[0], old_position[1]);
        } else {
            SDL_Log("Could not get selectedx unit component");
        }

    } else {
        SDL_Log("Could not get selected entity");
    }

    if (cursorpos) {
        new_position[0] = cursorpos->getPos()[0] - selectedpos->getOffset()[0];
        new_position[1] = cursorpos->getPos()[1] - selectedpos->getOffset()[1];
        SDL_Log("New position %d, %d \n", new_position[0], new_position[1]);
    } else {
        SDL_Log("Could not get setter(unit) position component");
    }

    mapx->moveUnit(old_position[0], old_position[1], new_position[0], new_position[1]);
    selectedpos->setPos(new_position);
    game->setCursorlastpos(new_position[0], new_position[1]);

    game->makeUnitmenuoptions();

    if (!unitmenux->valid()) {
        game->makeMenu(MENU::UNIT);
    } else {
        game->updateMenu(MENU::UNIT);
    }

    unitmenux->component<Position>()->setPos((new_position[0] + 1) * settings->tilesize[0], new_position[1] * settings->tilesize[1]);
    game->showMenu(MENU::UNIT);
    game->setCursorstate(GAME::STATE::UNITMENU);
}

void UnitSystemx::receive(const unitMove & move) {
    // SDL_Log("Received unitMove event");
    std::vector<std::vector<short int>> costmapp;
    std::vector<std::vector<short int>> movemapp;
    std::vector<std::vector<short int>> attackmapp;
    entityx::Entity cursor = move.cursor;
    entityx::ComponentHandle<Position> cursorpos = cursor.component<Position>();
    entityx::ComponentHandle<Unit> unit = move.unit;
    selected = unit.entity();
    short unsigned int * start;
    short unsigned int * offset;
    short unsigned int nooffset[2];
    short unsigned int unit_move;
    short unsigned int current_unit_id;
    unsigned char * range;

    if (cursorpos) {
        start = (short unsigned int *)cursorpos->getPos();
        offset = (short unsigned int *)cursorpos->getOffset();
    } else {
        SDL_Log("Could not get cursor position component");
    }

    if (unit) {
        unit_move = unit->getStats().move;
        range = unit->getRange();
    } else {
        SDL_Log("Could not get unit component");
    }

    nooffset[0] = start[0] - offset[0];
    nooffset[1] = start[1] - offset[1];
    SDL_Log("nooffset: %d %d", nooffset[0], nooffset[1]);
    SDL_Log("unitmove: %d", unit_move);

    costmapp = mapx->makeMvtCostmap(unit);

    movemapp = movemap(costmapp, nooffset, unit_move, "matrix");
    mapx->setOverlay(MAP::OVERLAY::MOVE, movemapp);

    attackmapp = attackmap(movemapp, nooffset, unit_move, range, "matrix");
    mapx->setOverlay(MAP::OVERLAY::ATTACK, attackmapp);

    mapx->showOverlay();
}

void UnitSystemx::receive(const unitMap & map) {
    SDL_Log("Received unitMap event");

    if ((game->getState() == GAME::STATE::UNITMOVE)) {
        mapx->hideOverlay();
    }

    if ((game->getState() == GAME::STATE::UNITMENU) ||
            (game->getState() == GAME::STATE::OPTIONS)) {
        game->hideMenu(MENU::UNIT);
        game->setCursorstate(GAME::STATE::MAP);
    }

    game->setState(GAME::STATE::MAP);
}

void UnitSystemx::update(entityx::EntityManager & es, entityx::EventManager & events, entityx::TimeDelta dt) {

}

MapSystemx::MapSystemx() {
}

MapSystemx::MapSystemx(Game * in_game) {
    SDL_Log("Adding Mapsystemx.");
    game = in_game;
    updateMap();
}


void MapSystemx::updateMap() {
    mapx = game->getMap();
}

void MapSystemx::setMap(entityx::ComponentHandle<Map> in_map) {
    if (in_map) {
        mapx = in_map;
        // tilesize = mapx->getTilesize();
    } else {
        SDL_Log("RenderSystemx: Map Handle is invalid");
    }
}


void MapSystemx::addArmies(std::vector<unsigned char> in_armies) {
    for (short int i = 0; i < in_armies.size(); i++) {
        armies.push(in_armies[i]);
    }
}

void MapSystemx::addArmy(unsigned char in_army) {
    armies.push(in_army);
}

void MapSystemx::configure(entityx::EventManager & in_events) {
    event_manager = &in_events;
    event_manager->subscribe<turnBegin>(*this);
    event_manager->subscribe<turnEnd>(*this);
}

void MapSystemx::receive(const turnBegin & begin) {
    SDL_Log("Received turnBegin event");
    switchControl(armies.front());
    refreshUnits(armies.front());
}

void MapSystemx::receive(const turnEnd & end) {
    SDL_Log("Received turnEnd event");
    armies.push(armies.front());
    armies.pop();
    event_manager->emit<turnBegin>();
}

void MapSystemx::switchControl(unsigned char in_army) {
    SDL_Log("switching control to army: %d", in_army);

    if (isPC(in_army)) {
        game->setState(GAME::STATE::MAP);
    } else {
        game->setState(GAME::STATE::ENEMYTURN);
        // event_manager->emit<turnBegin>();
    }

}

void MapSystemx::refreshUnits(unsigned char in_army) {
    SDL_Log("Refreshing units");
    std::vector<entityx::ComponentHandle<Unit>> units = mapx->getUnits(in_army);

    SDL_Log("units size: %d", units.size());

    for (short i = 0; i < units.size(); i++) {
        units[i]->refresh();
    }
}


void MapSystemx::update(entityx::EntityManager & es, entityx::EventManager & events, entityx::TimeDelta dt) {
    entityx::ComponentHandle<Unit> unit;
}

ControlSystemx::ControlSystemx() {

}

ControlSystemx::ControlSystemx(Game * in_game) {
    SDL_Log("Adding Controlsystem.");
    game = in_game;
    keyboardInputMap = game->getKeyboardInputMap();
    gamepadInputMap = game->getGamepadInputMap();
}

void ControlSystemx::configure(entityx::EventManager & in_events) {
    event_manager = &in_events;
    event_manager->subscribe<inputAccept>(*this);
    event_manager->subscribe<inputCancel>(*this);
    event_manager->subscribe<inputMenuRight>(*this);
    event_manager->subscribe<inputMenuLeft>(*this);
    event_manager->subscribe<inputMinimap>(*this);
    event_manager->subscribe<cursorMoved>(*this);
    event_manager->subscribe<inputFaster>(*this);
    event_manager->subscribe<inputPause>(*this);
}

void ControlSystemx::receive(const inputMenuRight & menuright) {

}

void ControlSystemx::receive(const inputMenuLeft & menuleft) {

}

void ControlSystemx::receive(const inputMinimap & minimap) {

}

void ControlSystemx::receive(const inputFaster & faster) {

}

void ControlSystemx::receive(const inputPause & pause) {

}

void ControlSystemx::receive(const inputCancel & cancel) {
    SDL_Log("Received inputCancel event");
    entityx::ComponentHandle<KeyboardController> keyboard = cancel.keyboard;
    entityx::ComponentHandle<GamepadController> gamepad = cancel.gamepad;
    Controllers controllers = {keyboard, gamepad};
    entityx::Entity canceller = getInputent(controllers);
    entityx::ComponentHandle<Position> position = canceller.component<Position>();
    entityx::ComponentHandle<Unit> unitontile;
    short int cursor_pos[2];
    cursor_pos[0] = position->getPos()[0] - position->getOffset()[0];
    cursor_pos[1] = position->getPos()[1] - position->getOffset()[1];

    switch (game->getState()) {
        case GAME::STATE::UNITMENU:
        case GAME::STATE::MAPMENU:
        case GAME::STATE::OPTIONS:

            // mapx->moveUnit(old_position[0], old_position[1], new_position[0], new_position[1]);
            if (selected) {
                event_manager->emit<unitReturn>(canceller, selected);
            }

            event_manager->emit<unitMap>(canceller);
            break;

        case GAME::STATE::UNITMOVE:
            unitontile = unitmap[cursor_pos[1]][cursor_pos[0]];

            if (unitontile) {
                event_manager->emit<unitDeselect>(canceller, unitontile);
            } else {
                SDL_Log("No unit on tile.");
                event_manager->emit<unitMap>(canceller);
            }

            break;

        case GAME::STATE::MAP:
            unitontile = unitmap[cursor_pos[1]][cursor_pos[0]];

            if (unitontile) {
                if (unitontile->isDanger()) {
                    event_manager->emit<unitDanger>(canceller, unitontile);
                }

            }


            break;
    }
}

void ControlSystemx::AIturn(unsigned char in_army) {
    if (!isPC(in_army)) {
        SDL_Log("AIturn: Doing AI things...");

    } else {
        SDL_Log("AIturn: Army is Player Controlled!");
    }
}


entityx::Entity ControlSystemx::getInputent(Controllers in_controllers) {
    entityx::Entity inputter;

    if (in_controllers.gamepad) {
        inputter = in_controllers.gamepad.entity();
    }

    if (in_controllers.keyboard) {
        inputter = in_controllers.keyboard.entity();
    }

    return (inputter);
}

unsigned int ControlSystemx::getHeldbutton(Controllers in_controllers) {
    unsigned int frames_button = 0;

    if (in_controllers.keyboard) {
        frames_button = in_controllers.keyboard->getHeldbutton();
    }

    if (in_controllers.gamepad) {
        frames_button = in_controllers.gamepad->getHeldbutton();
    }

    return (frames_button);
}

void ControlSystemx::receive(const cursorMoved & moved) {
    // SDL_Log("Received cursorMoved event");
    entityx::ComponentHandle<Unit> unitontile;
    entityx::ComponentHandle<Unit> unitprevioustile;
    entityx::Entity cursor = moved.cursor;
    Point move = moved.move;
    entityx::ComponentHandle<Position> position = cursor.component<Position>();
    short int cursor_pos[2];
    short int previous_pos[2];
    short int newstate = -1;
    cursor_pos[0] = position->getPos()[0] - position->getOffset()[0];
    cursor_pos[1] = position->getPos()[1] - position->getOffset()[1];

    switch (game->getState()) {
        case GAME::STATE::UNITMOVE:
            previous_pos[0] = cursor_pos[0] - move.x;
            previous_pos[1] = cursor_pos[1] - move.y;
            unitprevioustile = unitmap[previous_pos[1]][previous_pos[0]];

            if (!isPC(selected->getArmy())) {
                if (unitprevioustile) {
                    event_manager->emit<unitDehover>(cursor, unitprevioustile);
                }
            }

            break;

        case GAME::STATE::MAP:
            previous_pos[0] = cursor_pos[0] - move.x;
            previous_pos[1] = cursor_pos[1] - move.y;
            unitprevioustile = unitmap[previous_pos[1]][previous_pos[0]];
            unitontile = unitmap[cursor_pos[1]][cursor_pos[0]];

            if (unitprevioustile) {
                event_manager->emit<unitDehover>(cursor, unitprevioustile);
            }

            if (unitontile) {
                event_manager->emit<unitHover>(cursor, unitontile);
            }

            break;
    }

    if (newstate != -1) {
        game->setState(newstate);
    }
}

void ControlSystemx::receive(const inputAccept & accept) {
    SDL_Log("Received inputAccept event");
    entityx::ComponentHandle<KeyboardController> keyboard = accept.keyboard;
    entityx::ComponentHandle<GamepadController> gamepad = accept.gamepad;
    Controllers controllers = {keyboard, gamepad};
    entityx::Entity accepter = getInputent(controllers);
    entityx::ComponentHandle<Unit> unitontile;
    short int newstate = -1;
    short int cursor_pos[2];
    entityx::ComponentHandle<Position> position = accepter.component<Position>();
    cursor_pos[0] = position->getPos()[0] - position->getOffset()[0];
    cursor_pos[1] = position->getPos()[1] - position->getOffset()[1];

    switch (game->getState()) {
        case GAME::STATE::MAP:
            SDL_Log("accepter Position, %d %d \n", cursor_pos[0], cursor_pos[1]);
            unitontile = unitmap[cursor_pos[1]][cursor_pos[0]];
            game->setCursorlastpos(cursor_pos[0], cursor_pos[1]);

            if (unitontile) {
                selected = unitontile;
                event_manager->emit<unitSelect>(accepter, unitontile);
            } else {
                newstate = GAME::STATE::MAPMENU;
                event_manager->emit<mapMenu>(accepter);
            }

            break;

        case GAME::STATE::UNITMOVE:
            if (isPC(selected->getArmy())) {
                newstate = GAME::STATE::UNITMENU;
                event_manager->emit<unitMenu>(accepter);
            } else {
                event_manager->emit<unitDanger>(accepter, selected);
            }

            break;

        case GAME::STATE::UNITMENU:
            event_manager->emit<unitmenuSelect>(accepter, selected);
            break;
    }

    if (newstate != -1) {
        game->setState(newstate);
    }
}

void ControlSystemx::update(entityx::EntityManager & es, entityx::EventManager & events, entityx::TimeDelta dt) {
    char to_move[2] = {0, 0};
    entityx::ComponentHandle<Position> position;
    entityx::Entity cursorent;

    for (entityx::Entity ent : es.entities_with_components<Map>()) {
        unitmap = ent.component<Map>()->getUnitmap();
    }

    for (entityx::Entity ent : es.entities_with_components<KeyboardController, Position>()) {
        if (!cursorent.valid()) {
            cursorent = ent;
        }

        if (!position) {
            position = ent.component<Position>();
        }

        entityx::ComponentHandle<KeyboardController> keyboard = ent.component<KeyboardController>();

        const Uint8 * kb_state = SDL_GetKeyboardState(NULL);
        std::vector<std::vector<SDL_Scancode>> pressed_move{};
        std::vector<std::vector<SDL_Scancode>> pressed_button{};

        if (keyboard->is_pressed(kb_state, keyboardInputMap.moveup) && !keyboard->is_pressed(kb_state, keyboardInputMap.movedown)) {
            to_move[1] = -1;
            pressed_move.push_back(keyboardInputMap.moveup);
        } else if (!keyboard->is_pressed(kb_state, keyboardInputMap.moveup) && keyboard->is_pressed(kb_state, keyboardInputMap.movedown)) {
            to_move[1] = 1;
            pressed_move.push_back(keyboardInputMap.movedown);
        }

        if (!keyboard->is_pressed(kb_state, keyboardInputMap.moveright) && keyboard->is_pressed(kb_state, keyboardInputMap.moveleft)) {
            to_move[0] = -1;
            pressed_move.push_back(keyboardInputMap.moveleft);
        } else if (keyboard->is_pressed(kb_state, keyboardInputMap.moveright) && !keyboard->is_pressed(kb_state, keyboardInputMap.moveleft)) {
            to_move[0] = 1;
            pressed_move.push_back(keyboardInputMap.moveright);
        }

        if (keyboard->is_pressed(kb_state, keyboardInputMap.accept)) {
            pressed_button.push_back(keyboardInputMap.accept);

            if (keyboard->getHeldbutton() == 1) {
                events.emit<inputAccept>(keyboard);
            }
        }

        if (keyboard->is_pressed(kb_state, keyboardInputMap.cancel)) {
            pressed_button.push_back(keyboardInputMap.cancel);

            if (keyboard->getHeldbutton() == 1) {
                events.emit<inputCancel>(keyboard);
            }
        }

        keyboard->check_move(pressed_move);
        keyboard->check_button(pressed_button);

        if (false) {
            events.emit<turnBegin>();
        }

        if (false) {
            events.emit<turnEnd>();
        }
    }

    for (entityx::Entity ent : es.entities_with_components<GamepadController, Position>()) {
        if (!cursorent.valid()) {
            cursorent = ent;
        }

        if (!position) {
            position = ent.component<Position>();
        }

        entityx::ComponentHandle<GamepadController> gamepad = ent.component<GamepadController>();
        SDL_GameController * controller = gamepad->getController();
        Sint16 mainxaxis = SDL_GameControllerGetAxis(controller, gamepadInputMap.mainxaxis[0]);
        Sint16 mainyaxis = SDL_GameControllerGetAxis(controller, gamepadInputMap.mainyaxis[0]);
        Sint16 secondxaxis = SDL_GameControllerGetAxis(controller, gamepadInputMap.secondxaxis[0]);
        Sint16 secondyaxis = SDL_GameControllerGetAxis(controller, gamepadInputMap.secondyaxis[0]);
        std::vector<short unsigned int> pressed_move{};
        std::vector<std::vector<SDL_GameControllerButton>> pressed_button{};
        int joystick_dead_zone = gamepad->getDeadzone();
        unsigned int frames_button = gamepad->getHeldbutton();

        if ((mainxaxis > joystick_dead_zone) || (secondxaxis > joystick_dead_zone)) {
            to_move[0] = 1;
            pressed_move.push_back(GAME::BUTTON::RIGHT);
        } else if ((mainxaxis < -joystick_dead_zone) || (secondxaxis < -joystick_dead_zone)) {
            to_move[0] = -1;
            pressed_move.push_back(GAME::BUTTON::LEFT);
        }

        if ((mainyaxis > joystick_dead_zone) || (secondyaxis > joystick_dead_zone)) {
            to_move[1] = 1;
            pressed_move.push_back(GAME::BUTTON::UP);
        } else if ((mainyaxis < -joystick_dead_zone) || (secondyaxis < -joystick_dead_zone))  {
            to_move[1] = -1;
            pressed_move.push_back(GAME::BUTTON::DOWN);
        }

        if (gamepad->isPressed(gamepadInputMap.moveright)) {
            to_move[0] = 1;
            pressed_move.push_back(GAME::BUTTON::RIGHT);
        } else if (gamepad->isPressed(gamepadInputMap.moveleft)) {
            to_move[0] = -1;
            pressed_move.push_back(GAME::BUTTON::LEFT);
        }

        if (gamepad->isPressed(gamepadInputMap.moveup)) {
            to_move[1] = -1;
            pressed_move.push_back(GAME::BUTTON::UP);
        } else if (gamepad->isPressed(gamepadInputMap.movedown)) {
            to_move[1] = 1;
            pressed_move.push_back(GAME::BUTTON::DOWN);
        }

        short unsigned int newstate = -1;
        entityx::Entity setter;

        if (gamepad->isPressed(gamepadInputMap.accept)) {
            // SDL_Log("Gamepad pressed accept.");
            pressed_button.push_back(gamepadInputMap.accept);

            if (gamepad->getHeldbutton() == 1) {
                events.emit<inputAccept>(gamepad);
            }
        }


        if (gamepad->isPressed(gamepadInputMap.cancel)) {
            pressed_button.push_back(gamepadInputMap.cancel);

            if (gamepad->getHeldbutton() == 1) {
                events.emit<inputCancel>(gamepad);
            }
        }

        gamepad->check_move(pressed_move);
        gamepad->check_button(pressed_button);

        if (false) {
            events.emit<turnBegin>();
        }

        if (false) {
            events.emit<turnEnd>();
        }

    }

    if (((to_move[0] != 0) || (to_move[1] != 0)) && (position->isUpdatable())) {
        Point move;
        move.x = to_move[0];
        move.y = to_move[1];

        if (position->addPos(to_move[0], to_move[1])) {
            events.emit<cursorMoved>(cursorent, move);
        }
    }
}

