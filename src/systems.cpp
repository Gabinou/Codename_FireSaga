
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
        map = in_map;
        tilesize = map->getTilesize();
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
                case SLIDETYPE::GEOMETRIC: //for cursor mvt on map.
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
        entityx::ComponentHandle<Map> map = ent.component<Map>();
        map->draw();
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

void RenderSystemx::configure(entityx::EventManager & event_manager) {
    event_manager.subscribe<turnBegin>(*this);
    event_manager.subscribe<turnEnd>(*this);
    event_manager.subscribe<unitHeal>(*this);
    event_manager.subscribe<unitWait>(*this);
    event_manager.subscribe<unitDie>(*this);
    event_manager.subscribe<unitRefresh>(*this);
}

void RenderSystemx::receive(const turnBegin & begin) {

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

UnitSystemx::UnitSystemx() {

}

UnitSystemx::UnitSystemx(Game * in_game) {
    SDL_Log("Adding Unitsystem.");
    game = in_game;
    settings = game->getSettings();
    unitmenux = game->getUnitmenu();
    updateMap();
}

void UnitSystemx::updateMap() {
    mapx = game->getMap();
}

void UnitSystemx::configure(entityx::EventManager & events) {
    event_manager = &events;
    events.subscribe<unitSelect>(*this);
    events.subscribe<unitDeselect>(*this);
    events.subscribe<unitDanger>(*this);
    events.subscribe<unitMove>(*this);
    events.subscribe<unitNomove>(*this);
    events.subscribe<unitMenu>(*this);
    events.subscribe<unitmenuSelect>(*this);
    events.subscribe<unitMap>(*this);
    events.subscribe<unitHover>(*this);
    events.subscribe<unitDehover>(*this);
    events.subscribe<unitWait>(*this);
    events.subscribe<unitTalk>(*this);
    events.subscribe<unitRescue>(*this);
    events.subscribe<unitAttack>(*this);
    events.subscribe<unitTrade>(*this);
    events.subscribe<unitEscape>(*this);
    events.subscribe<unitItems>(*this);
}

void UnitSystemx::receive(const unitDehover & dehover) {
    SDL_Log("unitDehover event received");
    short int newstate = -1;
    entityx::ComponentHandle<Unit> unit = dehover.unit;
    entityx::Entity cursor = dehover.cursor;

    if (isPC(unit->getArmy())) {

    } else {
        event_manager->emit<unitNomove>(cursor);
    }
}

void UnitSystemx::receive(const unitHover & hover) {
    SDL_Log("unitHover event received");

}

void UnitSystemx::receive(const unitWait & wait) {

}

void UnitSystemx::receive(const unitTalk & talk) {

}

void UnitSystemx::receive(const unitRescue & rescue) {

}

void UnitSystemx::receive(const unitAttack & attack) {

}

void UnitSystemx::receive(const unitTrade & trade) {

}

void UnitSystemx::receive(const unitEscape & Escape) {

}

void UnitSystemx::receive(const unitItems & Items) {

}



void UnitSystemx::receive(const unitDeselect & deselect) {
    SDL_Log("unitDeselect event received");
    short int newstate = -1;
    entityx::ComponentHandle<Unit> unit = deselect.unit;
    entityx::Entity cursor = deselect.cursor;


    if (isPC(unit->getArmy())) {
        // event_manager->emit<unitMove>(cursor, deselect.unit);
        // newstate = GAME::STATE::UNITMOVE;
    } else {
        std::vector<std::vector<short int>> costmapp;
        std::vector<std::vector<short int>> movemapp;
        std::vector<std::vector<short int>> attackmapp;
        std::vector<std::vector<short int>> dangermapp;
        short unsigned int unit_move;
        short unsigned int start[2];
        unsigned char * range;

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

        if (unit->isDanger()) {
            mapx->subDanger(dangermapp);
            unit->hideDanger();
        }
    }

    if (newstate > 0) {
        game->setState(newstate);
    }
}


void UnitSystemx::receive(const unitSelect & select) {
    SDL_Log("unitSelect event received");
    short int newstate = -1;
    entityx::ComponentHandle<Unit> unit = select.unit;

    if (isPC(unit->getArmy())) {
        event_manager->emit<unitMove>(select.cursor, select.unit);
        newstate = GAME::STATE::UNITMOVE;
    } else {
        if (game->getState() == GAME::STATE::UNITMOVE) {
            event_manager->emit<unitDanger>(select.cursor, select.unit);
        } else {
            event_manager->emit<unitMove>(select.cursor, select.unit);
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

    SDL_Log("menuind: %d ", menuind);
    unitmenuoptions = game->getMenuoptions(MENU::UNIT);

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
    short int old_position[2];

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

void UnitSystemx::receive(const unitNomove & nomove) {
    // SDL_Log("Received unitNomove event");
    mapx->hideOverlay();
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
}

void UnitSystemx::update(entityx::EntityManager & es, entityx::EventManager & events, entityx::TimeDelta dt) {

}

MapSystemx::MapSystemx() {
    SDL_Log("Adding Mapsystemx.");

}

void MapSystemx::addArmy(unsigned char in_army) {
    armies.push(in_army);
}

void MapSystemx::configure(entityx::EventManager & event_manager) {
    event_manager.subscribe<turnBegin>(*this);
    event_manager.subscribe<turnEnd>(*this);
    event_manager.subscribe<mapMenu>(*this);
}

void MapSystemx::receive(const mapMenu & menu) {
    SDL_Log("Received a mapMenu from...");
}

void MapSystemx::receive(const turnBegin & begin) {
    SDL_Log("Received a turnBegin from...");
}

void MapSystemx::receive(const turnEnd & end) {
    SDL_Log("Received a turnEnd from...");
    armies.push(armies.front());
    armies.pop();
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
    unsigned int frames_button = getHeldbutton(controllers);

    if (frames_button == 1) {
        short int cursor_pos[2];
        cursor_pos[0] = position->getPos()[0] - position->getOffset()[0];
        cursor_pos[1] = position->getPos()[1] - position->getOffset()[1];
        unitontile = unitmap[cursor_pos[1]][cursor_pos[0]];

        switch (game->getState()) {
            case GAME::STATE::UNITMENU:
            case GAME::STATE::OPTIONS:
            case GAME::STATE::MAPMENU:
            case GAME::STATE::UNITMOVE:
                event_manager->emit<unitMap>(canceller);
                game->setState(GAME::STATE::MAP);
                break;

            case GAME::STATE::MAP:
                SDL_Log("GameState is map");

                if (unitontile) {
                    event_manager->emit<unitDeselect>(canceller, unitontile);
                } else {
                    SDL_Log("No unit on tile.");
                }

                break;
        }
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
    SDL_Log("Received cursorMoved event");
    entityx::ComponentHandle<Unit> unitontile;
    entityx::ComponentHandle<Unit> unitprevioustile;
    entityx::Entity cursor = moved.cursor;
    entityx::ComponentHandle<Position> position = cursor.component<Position>();
    short int cursor_pos[2];

    switch (game->getState()) {
        case GAME::STATE::UNITMOVE:
            unitprevioustile = unitmap[cursor_pos[1] - moved.to_move.x][cursor_pos[0]  - moved.to_move.y];

            if (unitprevioustile) {
                event_manager->emit<unitDehover>(cursor, unitprevioustile);
            }

        case GAME::STATE::MAP:
            cursor_pos[0] = position->getPos()[0] - position->getOffset()[0];
            cursor_pos[1] = position->getPos()[1] - position->getOffset()[1];
            unitontile = unitmap[cursor_pos[1]][cursor_pos[0]];


            if (unitontile) {
                event_manager->emit<unitHover>(cursor, unitontile);
                // event_manager->emit<unitMove>(cursor, unitontile);
            } else {
                // event_manager->emit<unitNomove>(cursor);
            }

            break;
    }
}

void ControlSystemx::receive(const inputAccept & accept) {
    // SDL_Log("Received inputAccept event");
    entityx::ComponentHandle<KeyboardController> keyboard = accept.keyboard;
    entityx::ComponentHandle<GamepadController> gamepad = accept.gamepad;
    Controllers controllers = {keyboard, gamepad};
    entityx::Entity accepter = getInputent(controllers);
    unsigned int frames_button = getHeldbutton(controllers);

    if (frames_button == 1) {
        short int newstate = -1;
        short int cursor_pos[2];
        entityx::ComponentHandle<Position> position = accepter.component<Position>();
        cursor_pos[0] = position->getPos()[0] - position->getOffset()[0];
        cursor_pos[1] = position->getPos()[1] - position->getOffset()[1];
        entityx::ComponentHandle<Unit> unitontile;

        switch (game->getState()) {
            case GAME::STATE::MAP:
                SDL_Log("accepter Position, %d %d \n", cursor_pos[0], cursor_pos[1]);
                game->setCursorlastpos(cursor_pos[0], cursor_pos[1]);
                unitontile = unitmap[cursor_pos[1]][cursor_pos[0]];

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

                event_manager->emit<unitmenuSelect>(accepter, unitontile);
                break;
        }

        if (newstate != -1) {
            game->setState(newstate);
        }
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
            events.emit<inputAccept>(keyboard);
        }

        if (keyboard->is_pressed(kb_state, keyboardInputMap.cancel)) {
            pressed_button.push_back(keyboardInputMap.cancel);
            events.emit<inputCancel>(keyboard);
        }

        keyboard->check_move(pressed_move);
        keyboard->check_button(pressed_button);

        if (false) {
            events.emit<turnBegin>(ent);
        }

        if (false) {
            events.emit<turnEnd>(ent);
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
            events.emit<inputAccept>(gamepad);
        }


        if (gamepad->isPressed(gamepadInputMap.cancel)) {
            pressed_button.push_back(gamepadInputMap.cancel);
            events.emit<inputCancel>(gamepad);

        }

        gamepad->check_move(pressed_move);
        gamepad->check_button(pressed_button);

        if (false) {
            events.emit<turnBegin>(ent);
        }

        if (false) {
            events.emit<turnEnd>(ent);
        }

    }

    if ((to_move[0] != 0) || (to_move[1] != 0)) {
        position->addPos(to_move[0], to_move[1]);
        Point point;
        point.x = to_move[0];
        point.y = to_move[1];
        events.emit<cursorMoved>(cursorent, point);
    }
}

