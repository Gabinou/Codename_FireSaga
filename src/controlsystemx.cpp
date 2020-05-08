
#include "controlsystemx.hpp"

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
    event_manager->subscribe<turnBegin>(*this);
    event_manager->subscribe<switchControl>(*this);
}

void ControlSystemx::receive(const turnBegin & begin) {
    SDL_Log("Received turnBegin event.");
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

            if (selected) {
                event_manager->emit<unitReturn>(canceller, selected);
            }

            event_manager->emit<return2Map>(canceller);
            break;

        case GAME::STATE::UNITMOVE:
            unitontile = unitmap[cursor_pos[1]][cursor_pos[0]];

            if (unitontile) {
                event_manager->emit<unitDeselect>(canceller, unitontile);
            } else {
                SDL_Log("No unit on tile.");
                event_manager->emit<return2Map>(canceller);
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

        case GAME::STATE::MAPMENU:
            event_manager->emit<mapmenuSelect>(accepter);
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

