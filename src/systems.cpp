
#include "systems.hpp"

void RenderSystemx::setRenderer(SDL_Renderer * in_renderer) {
    if (in_renderer) {
        SDL_Log("Added renderer to renderSystemx");
        renderer = in_renderer;
    } else {
        SDL_Log("In renderer is null");
    }
}

RenderSystemx::RenderSystemx() {

}

RenderSystemx::RenderSystemx(SDL_Renderer * in_renderer) {
    setRenderer(in_renderer);
}

void RenderSystemx::setMap(entityx::ComponentHandle<Map> in_map) {
    //Make into Settilemap.
    map = in_map;
    tilesize = map->getTilesize();
}

void RenderSystemx::setTilesize(const short int unsigned width, const short int unsigned height) {
    tilesize[0] = width;
    tilesize[1] = height;
}

void RenderSystemx::setLinespace(const short int unsigned in_linespace) {
    linespace = in_linespace;
}


void RenderSystemx::update(entityx::EntityManager & es, entityx::EventManager & events, entityx::TimeDelta dt) {
    SDL_RenderClear(renderer);

    for (entityx::Entity ent : es.entities_with_components<Map>()) {
        entityx::ComponentHandle<Map> map = ent.component<Map>();
        map->draw();
    }

    // RENDERING NOTE: First loaded, last animated.
    // -> load cursor first to render it over everything else.
    for (entityx::Entity ent : es.entities_with_components<Sprite, Position>()) {
        entityx::ComponentHandle<Sprite> sprite = ent.component<Sprite>();
        entityx::ComponentHandle<Position> position = ent.component<Position>();

        if (!ent.has_component<Text>()) {
            int kb_held = 0;
            int gp_held = 0;
            short int frames = sprite->getFrames();
            short int speed = sprite->getSpeed();
            short int * slidepos = sprite->getSlidepos();
            short int * objectivepos = sprite->getObjpos();
            SDL_Rect srcrect = sprite->getSrcrect();
            SDL_Rect destrect = sprite->getDestrect();
            unsigned char slidetype = sprite->getSlidetype();
            short int slideint = sprite->getSlideint();
            float * slidefactors = sprite->getSlidefactors();
            short int scalefactor[2];
            scalefactor[0] = tilesize[0];
            scalefactor[1] = tilesize[1];

            if (sprite->isAnimated()) { //looping sprites.
                unsigned char looping = sprite->getSs_looping();

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

            if ((!ent.has_component<KeyboardController>()) && (!ent.has_component<GamepadController>())) {
                // This is for NOT CURSOR.
                if (!position->isonTilemap()) { //move on the menu space
                    scalefactor[0] = 1;
                    scalefactor[1] = 1 ;
                }

                slidepos[0] = (int)(position->getPos()[0] * scalefactor[0]);
                slidepos[1] = (int)(position->getPos()[1] * scalefactor[1]);
            }

            entityx::ComponentHandle<KeyboardController> keyboard = ent.component<KeyboardController>();

            if (keyboard) {
                kb_held = keyboard->getHeldmove();
            }

            entityx::ComponentHandle<GamepadController> gamepad = ent.component<GamepadController>();

            if (gamepad) {
                // SDL_Log("Rendering Gamepad Controller.");
                gp_held = gamepad->getHeldmove();
            }

            if ((gamepad) || (keyboard)) {
                if (!position->isonTilemap()) { //move on the menu space
                    scalefactor[0] = linespace;
                    scalefactor[1] = linespace;
                }

                if (slidetype == SLIDETYPE::GEOMETRIC) { //for cursor mvt on map.
                    objectivepos[0] = (int)position->getPos()[0] * (scalefactor[0]) - destrect.w / 4;
                    objectivepos[1] = (int)position->getPos()[1] * (scalefactor[1]) - destrect.h / 4;

                    if ((gp_held > 25) || (kb_held > 25))  {
                        slideint = 1;
                    }

                    if (objectivepos[0] != slidepos[0]) {
                        slidepos[0] += geometricslide((objectivepos[0] - slidepos[0]), slidefactors[slideint]);
                    }

                    if (objectivepos[1] != slidepos[1]) {
                        slidepos[1] += geometricslide((objectivepos[1] - slidepos[1]), slidefactors[slideint]);
                    }

                    if ((objectivepos[0] == slidepos[0]) && (objectivepos[1] == slidepos[1])) {
                        position->setUpdatable(true);
                        slideint = 0;
                    } else {
                        position->setUpdatable(false);
                    }
                }
            }

            // if (slidetype == "vector") { //for unit mvt on map.

            // }

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

    SDL_RenderPresent(renderer);
}

void RenderSystemx::configure(entityx::EventManager & event_manager) {
    event_manager.subscribe<turnBegin>(*this);
    event_manager.subscribe<turnEnd>(*this);
    event_manager.subscribe<unitHealEvent>(*this);
    event_manager.subscribe<unitWaitEvent>(*this);
    event_manager.subscribe<unitDieEvent>(*this);
    event_manager.subscribe<unitRefreshEvent>(*this);
}

void RenderSystemx::receive(const turnBegin & begin) {

}

void RenderSystemx::receive(const turnEnd & end) {

}

void RenderSystemx::receive(const unitHealEvent & heal) {

}

void RenderSystemx::receive(const unitDieEvent & death) {

}

void RenderSystemx::receive(const unitWaitEvent & wait) {

}

void RenderSystemx::receive(const unitRefreshEvent & refresh) {

}

UnitSystemx::UnitSystemx() {

}

UnitSystemx::UnitSystemx(Game * in_game, entityx::EntityManager * in_entity_manager) {
    game = in_game;
    entity_manager = in_entity_manager;
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
    events.subscribe<unitMove>(*this);
    events.subscribe<unitMenu>(*this);
    events.subscribe<unitMap>(*this);
}



void UnitSystemx::receive(const unitSelect & select) {
    entityx::ComponentHandle<Unit> unit = select.unit;

    switch (unit->getArmy()) {
        case UNIT::ARMY::FRIENDLY:
        case UNIT::ARMY::ERWIN:
        case UNIT::ARMY::FREE_MILITIA:
            event_manager->emit<unitMove>(select.cursor, select.unit);
            break;

        case UNIT::ARMY::ENEMY:
        case UNIT::ARMY::BANDITS:
        case UNIT::ARMY::KEWAC:
            break;

        case UNIT::ARMY::NEUTRAL:
        case UNIT::ARMY::IMPERIAL:
            break;
    }
}

void UnitSystemx::receive(const unitMenu & menu) {
    SDL_Log("Unitmenu event received.");
    mapx->hideOverlay();

    entityx::Entity cursor = menu.cursor;
    entityx::ComponentHandle<Position> cursorpos = cursor.component<Position>();

    if (unitmenux->valid()) {
        unitmenux->component<Sprite>()->show();
        unitmenux->component<Text>()->show();
    } else {
        game->makeUnitmenu();
    }

    short int * new_position;
    short int * old_position;

    entityx::ComponentHandle<Position> selectedpos;

    if (selected.valid()) {
        selectedpos = selected.component<Position>();

        if (selectedpos) {
            old_position = selectedpos->getPos();
            SDL_Log("Old position %d, %d \n", old_position[0], old_position[1]);
        } else {
            SDL_Log("Could not get selectedx unit component");
        }

    } else {
        SDL_Log("Could not get selected entity");
    }

    if (cursorpos) {
        new_position = cursorpos->getPos();
        SDL_Log("New position %d, %d \n", new_position[0], new_position[1]);
    } else {
        SDL_Log("Could not get setter(unit) position component");
    }

    mapx->moveUnit(old_position[0], old_position[1], new_position[0], new_position[1]);
    unitmenux->component<Position>()->setPos((new_position[0] + 1) * settings->tilesize[0], new_position[1] * settings->tilesize[1]);
    selectedpos->setPos(new_position); // move at the end, cause new and old_position are pointers!
    game->setCursorstate(GAME::STATE::UNITMENU);
}

void UnitSystemx::receive(const unitMove & move) {
    SDL_Log("Unitmove event received.");
    std::vector<std::vector<short int>> costmap;
    std::vector<std::vector<short int>> movemapp;
    std::vector<std::vector<short int>> attackmapp;
    entityx::Entity cursor = move.cursor;
    entityx::ComponentHandle<Position> cursorpos = cursor.component<Position>();
    entityx::ComponentHandle<Unit> unit = move.unit;
    selected = unit.entity();
    short unsigned int * start;
    short unsigned int unit_move;
    short unsigned int current_unit_id;
    unsigned char unitmvttype;
    unsigned char * range;

    if (cursorpos) {
        start = (short unsigned int *)cursorpos->getPos();
    } else {
        SDL_Log("Could not get cursor position component");
    }

    if (unit) {
        unit_move = unit->getStats().move;
        unitmvttype = unit->getMvttype();
        range = unit->getRange();
    } else {
        SDL_Log("Could not get unit component");
    }

    SDL_Log("start: %d %d", start[0], start[1]);
    SDL_Log("unitmove: %d", unit_move);

    costmap = mapx->makeMvtCostmap(unitmvttype);

    movemapp = movemap(costmap, start, unit_move, "matrix");
    mapx->setOverlay(MAP::OVERLAY::MOVE, movemapp);

    attackmapp = attackmap(movemapp, start, unit_move, range, "matrix");
    mapx->setOverlay(MAP::OVERLAY::ATTACK, attackmapp);

    mapx->showOverlay();
}

void UnitSystemx::receive(const unitMap & map) {

    if ((game->getState() == GAME::STATE::UNITMOVE)) {
        mapx->hideOverlay();
    }

    if ((game->getState() == GAME::STATE::UNITMENU) ||
            (game->getState() == GAME::STATE::OPTIONS)) {
        game->hideMenu(GAME::STATE::UNITMENU);
        game->setCursorstate(GAME::STATE::MAP);

    }
}

void UnitSystemx::update(entityx::EntityManager & es, entityx::EventManager & events, entityx::TimeDelta dt) {

}

MapSystemx::MapSystemx() {

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
    entityx::ComponentHandle<KeyboardController> keyboard = cancel.keyboard;
    entityx::ComponentHandle<GamepadController> gamepad = cancel.gamepad;
    entityx::Entity canceller;

    if (keyboard) {
        canceller = keyboard.entity();
    }

    if (gamepad) {
        canceller = gamepad.entity();
    }

    if ((game->getState() == GAME::STATE::UNITMENU) ||
            (game->getState() == GAME::STATE::OPTIONS) ||
            (game->getState() == GAME::STATE::UNITMOVE)) {
        event_manager->emit<unitMap>(canceller);
        game->setState(GAME::STATE::MAP);
    }
}


void ControlSystemx::receive(const inputAccept & accept) {
    short int toset = -1;
    entityx::ComponentHandle<Position> position;
    entityx::ComponentHandle<KeyboardController> keyboard = accept.keyboard;
    entityx::ComponentHandle<GamepadController> gamepad = accept.gamepad;
    entityx::Entity accepter;
    unsigned int frames_button = 0;

    if (keyboard) {
        frames_button = keyboard->getHeldbutton();
        accepter = keyboard.entity();
    }

    if (gamepad) {
        frames_button = gamepad->getHeldbutton();
        accepter = gamepad.entity();
    }

    position = accepter.component<Position>();

    if ((game->getState() == GAME::STATE::MAP) && (frames_button == 1)) {
        SDL_Log("accepter Position, %d %d \n", position->getPos()[0], position->getPos()[1]);
        entityx::ComponentHandle<Unit> unitontile = unitmap[position->getPos()[0]][position->getPos()[1]];

        if (unitontile) {
            toset = GAME::STATE::UNITMOVE;
            event_manager->emit<unitSelect>(accepter, unitontile);
        } else {
            toset = GAME::STATE::OPTIONS;
            event_manager->emit<mapMenu>(accepter);
        }
    } else if ((game->getState() == GAME::STATE::UNITMOVE) && (frames_button == 1)) {
        toset = GAME::STATE::UNITMENU;
        event_manager->emit<unitMenu>(accepter);
    }

    if (toset != -1) {
        game->setState(toset);
    }

}

void ControlSystemx::update(entityx::EntityManager & es, entityx::EventManager & events, entityx::TimeDelta dt) {

    for (entityx::Entity ent : es.entities_with_components<Map>()) {
        unitmap = ent.component<Map>()->getUnitmap();
    }

    for (entityx::Entity ent : es.entities_with_components<KeyboardController, Position>()) {
        entityx::ComponentHandle<Position> position = ent.component<Position>();
        entityx::ComponentHandle<KeyboardController> keyboard = ent.component<KeyboardController>();

        const Uint8 * kb_state = SDL_GetKeyboardState(NULL);
        std::vector<std::vector<SDL_Scancode>> pressed_move{};
        std::vector<std::vector<SDL_Scancode>> pressed_button{};

        if (keyboard->is_pressed(kb_state, keyboardInputMap.moveup) && !keyboard->is_pressed(kb_state, keyboardInputMap.movedown)) {
            position->addPos(0, -1);
            pressed_move.push_back(keyboardInputMap.moveup);
        } else if (!keyboard->is_pressed(kb_state, keyboardInputMap.moveup) && keyboard->is_pressed(kb_state, keyboardInputMap.movedown)) {
            position->addPos(0, 1);
            pressed_move.push_back(keyboardInputMap.movedown);
        }

        if (!keyboard->is_pressed(kb_state, keyboardInputMap.moveright) && keyboard->is_pressed(kb_state, keyboardInputMap.moveleft)) {
            position->addPos(-1, 0);
            pressed_move.push_back(keyboardInputMap.moveleft);
        } else if (keyboard->is_pressed(kb_state, keyboardInputMap.moveright) && !keyboard->is_pressed(kb_state, keyboardInputMap.moveleft)) {
            position->addPos(1, 0);
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

    for (entityx::Entity ent : es.entities_with_components<KeyboardController, Position>()) {
        entityx::ComponentHandle<Position> position = ent.component<Position>();
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
            position->addPos(1, 0);
            pressed_move.push_back(GAME::BUTTON::RIGHT);
        } else if ((mainxaxis < -joystick_dead_zone) || (secondxaxis < -joystick_dead_zone)) {
            position->addPos(-1, 0);
            pressed_move.push_back(GAME::BUTTON::LEFT);
        }

        if ((mainyaxis > joystick_dead_zone) || (secondyaxis > joystick_dead_zone)) {
            position->addPos(0, 1);
            pressed_move.push_back(GAME::BUTTON::UP);
        } else if ((mainyaxis < -joystick_dead_zone) || (secondyaxis < -joystick_dead_zone))  {
            position->addPos(0, -1);
            pressed_move.push_back(GAME::BUTTON::DOWN);
        }

        if (gamepad->isPressed(gamepadInputMap.moveright)) {
            position->addPos(1, 0);
            pressed_move.push_back(GAME::BUTTON::RIGHT);
        } else if (gamepad->isPressed(gamepadInputMap.moveleft)) {
            position->addPos(-1, 0);
            pressed_move.push_back(GAME::BUTTON::LEFT);
        }

        if (gamepad->isPressed(gamepadInputMap.moveup)) {
            position->addPos(0, -1);
            pressed_move.push_back(GAME::BUTTON::UP);
        } else if (gamepad->isPressed(gamepadInputMap.movedown)) {
            position->addPos(0, 1);
            pressed_move.push_back(GAME::BUTTON::DOWN);
        }

        short unsigned int toset = -1;
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
}

