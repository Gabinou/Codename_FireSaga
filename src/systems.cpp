
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
                if (!position->isonTilemap()) { //move on the pixelspace
                    slidepos[0] = (int)position->getPos()[0];
                    slidepos[1] = (int)position->getPos()[1];
                } else { //move on the map.
                    slidepos[0] = (int)(position->getPos()[0] * tilesize[0]);
                    slidepos[1] = (int)(position->getPos()[1] * tilesize[1]);
                }
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

            if (slidetype == SLIDETYPE::GEOMETRIC) { //for cursor mvt on map.
                objectivepos[0] = (int)position->getPos()[0] * (tilesize[0]) - destrect.w / 4;
                objectivepos[1] = (int)position->getPos()[1] * (tilesize[1]) - destrect.h / 4;

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
    event_manager.subscribe<beginTurnEvent>(*this);
    event_manager.subscribe<endTurnEvent>(*this);
    event_manager.subscribe<unitHealEvent>(*this);
    event_manager.subscribe<unitWaitEvent>(*this);
    event_manager.subscribe<unitDieEvent>(*this);
    event_manager.subscribe<unitRefreshEvent>(*this);
}

void RenderSystemx::receive(const beginTurnEvent & begin) {

}

void RenderSystemx::receive(const endTurnEvent & end) {

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

UnitSystemx::UnitSystemx(Game * in_game) {
    game = in_game;
    settings = game->getSettings();
    updateMap();
}

void UnitSystemx::updateMap() {
    mapx = game->getMap();
}


void UnitSystemx::configure(entityx::EventManager & event_manager) {
    event_manager.subscribe<unitMove>(*this);
    event_manager.subscribe<unitMenu>(*this);
}

void UnitSystemx::makeUnitmenu(entityx::Entity & setter) {
    SDL_Log("Making unit menu\n");
    // unitmenux = entities.create();
    unitmenux.assign<Position>();
    unitmenux.component<Position>()->setBounds(0, 2000, 0, 2000);
    // unitmenux.component<Position>()->setPos(
    //     (int)(setter.component<Position>()->getPos()[0] * settings.tilesize[0]),
    //     (int)(setter.component<Position>()->getPos()[1] * settings.tilesize[1])
    // );
    SDL_Log("Unitmenu setter position %d %d\n", setter.component<Position>()->getPos()[0], setter.component<Position>()->getPos()[1]);
    SDL_Log("Unitmenu position %d %d\n", unitmenux.component<Position>()->getPos()[0], unitmenux.component<Position>()->getPos()[1]);

    SDL_Color black = {255, 255, 255};
    unitmenux.assign<Sprite>("..//assets//textbox.png", (int []) {128, 128});
    // I think the menu textures should be loaded elsewhere when initted or first called. Then, should be only unloaded after a while.
    //Not loaded and unloaded after EACH CALL.
    // unitmenux.assign<Text>(settings.fontsize, std::vector<std::string> {"Attack", "Wait"}, black);
}

void UnitSystemx::receive(const unitMenu & menu) {
    SDL_Log("Unitmenu event received.");
    mapx->hideOverlay();

    entityx::Entity cursor = menu.cursor;
    entityx::ComponentHandle<Position> cursorpos = cursor.component<Position>();
    entityx::ComponentHandle<Unit> unit = menu.unit;
    entityx::Entity selected = unit.entity();

    if (unitmenux.valid()) {
        unitmenux.component<Sprite>()->show();
        unitmenux.component<Text>()->show();
    } else {
        makeUnitmenu(cursor);
    }

    // short int * new_position;
    // short int * old_position;

    // entityx::ComponentHandle<Position> setterpos;
    // entityx::ComponentHandle<Position> selectedpos;
    // entityx::ComponentHandle<Unit> unitcomp;
    // setterpos = setter.component<Position>();

    // if (selected.valid()) {
    //     selectedpos = selected.component<Position>();

    //     if (selectedpos) {
    //         old_position = selectedpos->getPos();
    //         // SDL_Log("Old position %d, %d \n", old_position[0], old_position[1]);
    //     } else {
    //         SDL_Log("Could not get selectedx unit component");
    //     }
    // } else {
    //     SDL_Log("Could not get selected entity");
    // }

    // if (setterpos) {
    //     new_position = setterpos->getPos();
    //     // SDL_Log("New position %d, %d \n", new_position[0], new_position[1]);
    // } else {
    //     SDL_Log("Could not get setter(unit) position component");
    // }

    // mapx->moveUnit(old_position[0], old_position[1], new_position[0], new_position[1]);
    // unitmenux.component<Position>()->setPos((new_position[0] + 1) * settings.tilesize[0], new_position[1] * settings.tilesize[1]);
    // selectedpos->setPos(new_position); // move at the end, cause new and old_position are pointers!
    // setCursorstate(new_state);
}

void UnitSystemx::receive(const unitMove & move) {
    SDL_Log("Unitmove event received.");
    std::vector<std::vector<short int>> costmap;
    std::vector<std::vector<short int>> movemapp;
    std::vector<std::vector<short int>> attackmapp;
    entityx::Entity cursor = move.cursor;
    entityx::ComponentHandle<Position> cursorpos = cursor.component<Position>();
    entityx::ComponentHandle<Unit> unit = move.unit;
    entityx::Entity selected = unit.entity();
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

void UnitSystemx::update(entityx::EntityManager & es, entityx::EventManager & events, entityx::TimeDelta dt) {

}

TurnSystemx::TurnSystemx() {

}

void TurnSystemx::addArmy(unsigned char in_army) {
    armies.push(in_army);
}

void TurnSystemx::configure(entityx::EventManager & event_manager) {
    event_manager.subscribe<beginTurnEvent>(*this);
    event_manager.subscribe<endTurnEvent>(*this);
}

void TurnSystemx::receive(const beginTurnEvent & begin) {
    SDL_Log("Received a beginTurnEvent from...");
}

void TurnSystemx::receive(const endTurnEvent & end) {
    SDL_Log("Received a endTurnEvent from...");
    armies.push(armies.front());
    armies.pop();
}

void TurnSystemx::update(entityx::EntityManager & es, entityx::EventManager & events, entityx::TimeDelta dt) {
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
        game->setState(canceller, GAME::STATE::MAP);
    }
}

void ControlSystemx::receive(const inputAccept & accept) {
    short int toset = -1;
    entityx::Entity setter;
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
            setter = unitontile.entity();
            event_manager->emit<unitMove>(accepter, unitontile);
        } else {
            toset = GAME::STATE::OPTIONS;
            setter = accepter;
        }
    } else if ((game->getState() == GAME::STATE::UNITMOVE) && (frames_button == 1)) {
        toset = GAME::STATE::UNITMENU;
        setter = accepter;
    }

    if (toset != -1) {
        game->setState(setter, toset);
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
            events.emit<beginTurnEvent>(ent);
        }

        if (false) {
            events.emit<endTurnEvent>(ent);
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
            events.emit<beginTurnEvent>(ent);
        }

        if (false) {
            events.emit<endTurnEvent>(ent);
        }
    }
}

