
#include "controlsystemx.hpp"

ControlSystemx::ControlSystemx() {

}

ControlSystemx::ControlSystemx(Game * in_game) {
    SDL_Log("Adding Controlsystem.");
    setGame(in_game);
}

void ControlSystemx::setGame(Game * in_game) {
    game = in_game;
    keyboardInputMap = game->getKeyboardInputMap();
    gamepadInputMap = game->getGamepadInputMap();
    mouseInputMap = game->getMouseInputMap();
    cursorx = game->getMousex();
    mousex = game->getCursorx();
    window = game->getWindow();
    updateSettings();
    updateMap();
}

void ControlSystemx::updateSettings() {
    if (game != nullptr) {
        settings = game->getSettings();
    }
}

void ControlSystemx::updateMap() {
    if (game != nullptr) {
        mapx = game->getMap();
    }
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
    event_manager->subscribe<turnBegin>(*this);
    event_manager->subscribe<switchControl>(*this);
    event_manager->subscribe<enableMouse>(*this);
    event_manager->subscribe<disableMouse>(*this);
    event_manager->subscribe<enableCursor>(*this);
    event_manager->subscribe<disableCursor>(*this);
}

void ControlSystemx::receive(const disableMouse & disableM) {
    SDL_Log("Received disableMouse event");
    game->disableMousex();
}

void ControlSystemx::receive(const enableMouse & enableM) {
    SDL_Log("Received enableMouse event");
    game->enableMousex();
    mouse_idletime = 0.0f;
}

void ControlSystemx::receive(const enableCursor & enableC) {
    SDL_Log("Received enableCursor event");
    game->enableCursorx();
}

void ControlSystemx::receive(const disableCursor & disableC) {
    SDL_Log("Received disableCursor event");
    game->disableCursorx();
}

void ControlSystemx::receive(const turnBegin & begin) {
    SDL_Log("Received turnBegin event");
    unsigned char army = begin.army;

    if (game->getState() == GAME::STATE::NPCTURN) {
        AIturn(army);
    }
}

void ControlSystemx::receive(const switchControl & Switch) {
    SDL_Log("Received switchControl event.");
    unsigned char army = Switch.army;

    if (isPC(army)) {
        game->setState(GAME::STATE::MAP);
        game->loadCursor();
        game->setCursorstate(MENU::MAP);

    } else {
        game->setState(GAME::STATE::NPCTURN);
        game->unloadCursor();
    }
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
    entityx::ComponentHandle<MouseController> mouse = cancel.mouse;

    if ((cancel.mouse) && (!game->isMouse())) {
        event_manager->emit<enableMouse>();
        event_manager->emit<disableCursor>();
    }

    if ((cancel.keyboard) || (cancel.gamepad) && (!game->isCursor())) {
        event_manager->emit<disableMouse>();
        event_manager->emit<enableCursor>();
    }

    Controllers controllers = {keyboard, gamepad, mouse};
    entityx::Entity canceller = getInputent(controllers);
    entityx::ComponentHandle<Position> position = canceller.component<Position>();
    entityx::ComponentHandle<Unit> unitontile;
    Point cursor_pos = position->getTilemapPos();
    Point offset = position->getOffset();
    Point new_pos;
    new_pos.x = cursor_pos.x - offset.x;
    new_pos.y = cursor_pos.y - offset.y;

    switch (game->getState()) {
        case GAME::STATE::UNITMENU:
        case GAME::STATE::MAPMENU:
        case GAME::STATE::OPTIONS:

            if (selected) {
                event_manager->emit<unitReturn>(canceller, selected);
            }

            event_manager->emit<return2Map>(canceller);
            break;

        case GAME::STATE::UNITMOVE:
            unitontile = unitmap[new_pos.y][new_pos.x];

            if (unitontile) {
                event_manager->emit<unitDeselect>(canceller, unitontile);
            } else {
                SDL_Log("No unit on tile.");
                event_manager->emit<return2Map>(canceller);
            }

            break;

        case GAME::STATE::MAP:
            unitontile = unitmap[new_pos.y][new_pos.x];

            if (unitontile) {
                if (unitontile->isDanger()) {
                    event_manager->emit<unitDanger>(canceller, unitontile);
                }

            }

            break;
    }

    blockInput = true;
}

void ControlSystemx::AIturn(unsigned char in_army) {
    if (!isPC(in_army)) {
        SDL_Log("AIturn: Doing AI things...");
        event_manager->emit<turnEnd>();

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

    if (in_controllers.mouse) {
        inputter = in_controllers.mouse.entity();
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
    Point move = moved.move;
    entityx::ComponentHandle<Position> position = cursor.component<Position>();
    short int newstate = -1;
    Point previous_pos;
    Point current_pos;
    Point cursor_pos = position->getPos();
    Point offset = position->getOffset();
    short int * bounds = position->getBounds();
    current_pos.x = cursor_pos.x - offset.x;
    current_pos.y = cursor_pos.y - offset.y;
    // SDL_Log("cursor_pos: %d %d", cursor_pos.x, cursor_pos.y);
    // SDL_Log("current_pos: %d %d", current_pos.x, current_pos.y);
    // SDL_Log("move: %d %d", move.x, move.y);
    // SDL_Log("offset: %d %d", offset.x, offset.y);
    // SDL_Log("bounds: %d %d %d %d", bounds[0], bounds[1], bounds[2], bounds[3]);

    switch (game->getState()) {
        case GAME::STATE::UNITMOVE:
            previous_pos.x = current_pos.x - move.x;
            previous_pos.y = current_pos.y - move.y;
            unitprevioustile = unitmap[previous_pos.y][previous_pos.x];

            if (!isPC(selected->getArmy())) {
                if (unitprevioustile) {
                    event_manager->emit<unitDehover>(cursor, unitprevioustile);
                }
            }

            break;

        case GAME::STATE::MAP:
            previous_pos.x = current_pos.x - move.x;
            previous_pos.y = current_pos.y - move.y;
            unitprevioustile = unitmap[previous_pos.y][previous_pos.x];
            unitontile = unitmap[current_pos.y][current_pos.x];

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
    entityx::ComponentHandle<MouseController> mouse = accept.mouse;
    Controllers controllers = {keyboard, gamepad, mouse};
    entityx::Entity accepter = getInputent(controllers);

    if ((accept.mouse) && (!game->isMouse())) {
        event_manager->emit<enableMouse>();
        event_manager->emit<disableCursor>();
    }

    if ((accept.keyboard) || (accept.gamepad) && (!game->isCursor())) {
        event_manager->emit<disableMouse>();
        event_manager->emit<enableCursor>();
    }

    entityx::ComponentHandle<Unit> unitontile;
    entityx::ComponentHandle<Position> position = accepter.component<Position>();
    short int newstate = -1;

    Point current_pos;
    Point cursor_pos = position->getTilemapPos();
    Point offset = position->getOffset();
    current_pos.x = cursor_pos.x - offset.x;
    current_pos.y = cursor_pos.y - offset.y;

    switch (game->getState()) {
        case GAME::STATE::MAP:
            // SDL_Log("accepter Position, %d %d \n", cursor_pos[0], cursor_pos[1]);
            unitontile = unitmap[current_pos.y][current_pos.x];
            game->setCursorlastpos(current_pos.x, current_pos.y);

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

        case GAME::STATE::MAPMENU:
            event_manager->emit<mapmenuSelect>(accepter);
            break;
    }

    if (newstate != -1) {
        game->setState(newstate);
    }

    blockInput = true;
}

void ControlSystemx::SDL_update() {
    SDL_Event event;

    while (SDL_PollEvent(&event)) {

        switch (event.type) {

            case SDL_MOUSEBUTTONDOWN:
            case SDL_MOUSEBUTTONUP:
                if (event.button.windowID == SDL_GetWindowID(window)) {

                    if (mousex->valid()) {
                        entityx::ComponentHandle<MouseController> mouse;
                        entityx::ComponentHandle<Position> position;
                        mouse = mousex->component<MouseController>();
                        position = mousex->component<Position>();

                        if (mouse) {
                            // SDL_Log("event pos: %d %d", event.button.x, event.button.y);
                            position->setPixelPos(event.button.x, event.button.y);

                            if (mapx) {
                                position->setTilemapPos(mapx->pixel2tile(event.button.x, event.button.y));
                            }

                            if (event.type != previous_mouse) {
                                if (event.button.state == SDL_PRESSED) {
                                    mouse->addHeld(event.button.button);
                                } else {
                                    mouse->removeHeld(event.button.button);
                                }
                            }
                        } else {
                            SDL_Log("cursor has no MouseController component");
                        }

                        previous_mouse = event.type;
                    }

                    // } else {
                    //     SDL_Log("mousex is not valid");
                    // }

                }

                break;

            case SDL_MOUSEMOTION: // received even if there is no motion.
                if (event.motion.windowID == SDL_GetWindowID(window)) {
                    // this is cause event.motion.xrel does not work

                    // SDL_Log("Mouse motion event rel: %d %d", event.motion.xrel, event.motion.yrel);
                    // SDL_Log("Mouse motion event pos: %d %d", event.motion.x, event.motion.y);
                    // SDL_Log("Mouse last position: %d %d", mouse_lastpos.x, mouse_lastpos.y);

                    if ((event.motion.x != mouse_lastpos.x) || (event.motion.y != mouse_lastpos.y)) {
                        if (!game->isMouse()) {
                            event_manager->emit<enableMouse>();
                            event_manager->emit<disableCursor>();
                        }

                        if (mousex->valid()) {
                            // SDL_Log("until here");
                            entityx::ComponentHandle<Position> position;
                            position = mousex->component<Position>();

                            if (position) {
                                position->setPixelPos(event.motion.x, event.motion.y);
                            }

                            if (mapx) {
                                Point tilemap_pos = mapx->pixel2tile(event.motion.x, event.motion.y);
                                position->setTilemapPos(tilemap_pos);
                            }

                            // } else {
                            //     SDL_Log("cursor has no MouseController component");
                            // }
                        }

                        // } else {
                        //     event_manager->emit<disableMouse>();
                    }

                    mouse_lastpos.x = event.motion.x;
                    mouse_lastpos.y = event.motion.y;
                    // SDL_Log("until here");
                }

                break;

            case SDL_AUDIODEVICEADDED:
                break;

            case SDL_AUDIODEVICEREMOVED:
                break;

            case SDL_CONTROLLERDEVICEADDED:
                SDL_Log("Handling SDL_CONTROLLERDEVICEADDED event");

                if (cursorx->valid()) {
                    entityx::ComponentHandle<GamepadController> gamepad = cursorx->component<GamepadController>();

                    if (gamepad) {
                        gamepad->addController(event.cdevice.which);
                    } else {
                        SDL_Log("cursorx has no GamepadController component");
                    }
                } else {
                    SDL_Log("cursorx is not valid");
                }

                break;

            case SDL_CONTROLLERDEVICEREMOVED:
                SDL_Log("Handling SDL_CONTROLLERDEVICEREMOVED event");

                if (cursorx->valid()) {
                    entityx::ComponentHandle<GamepadController> gamepad = cursorx->component<GamepadController>();

                    if (gamepad) {
                        gamepad->removeController();
                    } else {
                        SDL_Log("cursorx has no GamepadController component");
                    }
                } else {
                    SDL_Log("cursorx is not valid");
                }

                break;

            case SDL_QUIT:
                SDL_Log("Handling SDL_QUIT event");
                game->stopRunning();
                break;

            default:
                break;
        }
    }
}


void ControlSystemx::update(entityx::EntityManager & es, entityx::EventManager & events, entityx::TimeDelta dt) {
    // SDL_Log("updating control system");
    SDL_update();
    char to_move[2] = {0, 0};
    Point cursor_move = {0, 0};
    Point mouse_move = {0, 0};
    entityx::ComponentHandle<Position> cursor_position;
    entityx::ComponentHandle<Position> mouse_position;
    entityx::Entity cursor_ent;
    bool cursor_toenable = false;

    for (entityx::Entity ent : es.entities_with_components<Map>()) {
        unitmap = ent.component<Map>()->getUnitmap();
    }

    for (entityx::Entity ent : es.entities_with_components<KeyboardController, Position>()) {
        if (!cursor_ent.valid()) {
            cursor_ent = ent;
        }

        if (!cursor_position) {
            cursor_position = ent.component<Position>();
        }

        entityx::ComponentHandle<KeyboardController> keyboard = ent.component<KeyboardController>();

        const Uint8 * kb_state = SDL_GetKeyboardState(NULL);
        std::vector<std::vector<SDL_Scancode>> pressed_move{};
        std::vector<std::vector<SDL_Scancode>> pressed_button{};

        if (keyboard->is_pressed(kb_state, keyboardInputMap.moveup) && !keyboard->is_pressed(kb_state, keyboardInputMap.movedown)) {
            cursor_move.y = -1;
            pressed_move.push_back(keyboardInputMap.moveup);
        } else if (!keyboard->is_pressed(kb_state, keyboardInputMap.moveup) && keyboard->is_pressed(kb_state, keyboardInputMap.movedown)) {
            cursor_move.y = 1;
            pressed_move.push_back(keyboardInputMap.movedown);
        }

        if (!keyboard->is_pressed(kb_state, keyboardInputMap.moveright) && keyboard->is_pressed(kb_state, keyboardInputMap.moveleft)) {
            cursor_move.x = -1;
            pressed_move.push_back(keyboardInputMap.moveleft);
        } else if (keyboard->is_pressed(kb_state, keyboardInputMap.moveright) && !keyboard->is_pressed(kb_state, keyboardInputMap.moveleft)) {
            cursor_move.x = 1;
            pressed_move.push_back(keyboardInputMap.moveright);
        }

        if (keyboard->is_pressed(kb_state, keyboardInputMap.accept)) {
            pressed_button.push_back(keyboardInputMap.accept);

            if (keyboard->getHeldbutton() > min_held) {
                if (!blockInput) {
                    event_manager->emit<inputAccept>(keyboard);
                }
            }
        }

        if (keyboard->is_pressed(kb_state, keyboardInputMap.cancel)) {
            pressed_button.push_back(keyboardInputMap.cancel);

            if (keyboard->getHeldbutton() > min_held) {
                if (!blockInput) {
                    event_manager->emit<inputCancel>(keyboard);
                }
            }
        }

        keyboard->check_move(pressed_move, dt);
        keyboard->check_button(pressed_button, dt);
        kb_held = keyboard->getHeldbutton();
    }

    for (entityx::Entity ent : es.entities_with_components<GamepadController, Position>()) {
        if (!cursor_ent.valid()) {
            cursor_ent = ent;
        }

        if (!cursor_position) {
            cursor_position = ent.component<Position>();
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
            cursor_move.x = 1;
            pressed_move.push_back(GAME::BUTTON::RIGHT);
        } else if ((mainxaxis < -joystick_dead_zone) || (secondxaxis < -joystick_dead_zone)) {
            cursor_move.x = -1;
            pressed_move.push_back(GAME::BUTTON::LEFT);
        }

        if ((mainyaxis > joystick_dead_zone) || (secondyaxis > joystick_dead_zone)) {
            cursor_move.y = 1;
            pressed_move.push_back(GAME::BUTTON::UP);
        } else if ((mainyaxis < -joystick_dead_zone) || (secondyaxis < -joystick_dead_zone))  {
            cursor_move.y = -1;
            pressed_move.push_back(GAME::BUTTON::DOWN);
        }

        if (gamepad->isPressed(gamepadInputMap.moveright)) {
            cursor_move.x = 1;
            pressed_move.push_back(GAME::BUTTON::RIGHT);
        } else if (gamepad->isPressed(gamepadInputMap.moveleft)) {
            cursor_move.x = -1;
            pressed_move.push_back(GAME::BUTTON::LEFT);
        }

        if (gamepad->isPressed(gamepadInputMap.moveup)) {
            cursor_move.y = -1;
            pressed_move.push_back(GAME::BUTTON::UP);
        } else if (gamepad->isPressed(gamepadInputMap.movedown)) {
            cursor_move.y = 1;
            pressed_move.push_back(GAME::BUTTON::DOWN);
        }

        short unsigned int newstate = -1;
        entityx::Entity setter;

        if (gamepad->isPressed(gamepadInputMap.accept)) {
            pressed_button.push_back(gamepadInputMap.accept);

            if (gamepad->getHeldbutton() > min_held) {
                if (!blockInput) {
                    event_manager->emit<inputAccept>(gamepad);
                }
            }
        }

        if (gamepad->isPressed(gamepadInputMap.cancel)) {
            pressed_button.push_back(gamepadInputMap.cancel);

            if (gamepad->getHeldbutton() > min_held) {
                if (!blockInput) {
                    event_manager->emit<inputCancel>(gamepad);
                }
            }
        }

        gamepad->check_move(pressed_move, dt);
        gamepad->check_button(pressed_button, dt);
        gp_held = gamepad->getHeldbutton();
    }

    if (game->isMouse()) {
        for (entityx::Entity ent : es.entities_with_components<MouseController>()) {
            if (!mouse_position) {
                mouse_position = ent.component<Position>();
            }

            entityx::ComponentHandle<MouseController> mouse = ent.component<MouseController>();

            if (mouse->isPressed(mouseInputMap.cancel)) {
                if (mouse->getHeldbutton() > min_held) {
                    if (!blockInput) {
                        event_manager->emit<inputCancel>(mouse);
                    }
                }
            }

            if (mouse->isPressed(mouseInputMap.accept)) {
                if (mouse->getHeldbutton() > min_held) {
                    if (!blockInput) {
                        event_manager->emit<inputAccept>(mouse);
                    }
                }
            }

            mouse->check_button(dt);
            mouse_held = mouse->getHeldbutton();

        }
    }

    if ((kb_held == 0.) && (gp_held == 0.)  && (mouse_held == 0.)) {
        blockInput = false;
    }

    if (cursor_position) {
        if (((cursor_move.x != 0) || (cursor_move.y != 0)) && (cursor_position->isUpdatable())) {
            if (!game->isCursor()) {
                event_manager->emit<enableCursor>();
                event_manager->emit<disableMouse>();
            } else {
                if (cursor_position->addPos(cursor_move.x, cursor_move.y)) {
                    event_manager->emit<cursorMoved>(cursor_ent, cursor_move);
                }
            }
        }
    }
}

