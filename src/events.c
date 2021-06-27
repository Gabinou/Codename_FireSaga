#include "events.h"

struct Receivers_hash * receivers = NULL;

void (** receivers_arr)(struct Game *, SDL_Event *);

#define REGISTER_ENUM(x) uint32_t event_##x;
#include "names/events.h"
#undef REGISTER_ENUM

tnecs_entity_t * data1_entity;
tnecs_entity_t * data2_entity;

void Events_Data_Malloc() {
    data1_entity = SDL_malloc(sizeof(tnecs_entity_t));
    data2_entity = SDL_malloc(sizeof(tnecs_entity_t));

}

void Events_Data_Free() {
    free(data1_entity);
    free(data2_entity);
}

void Event_Emit(uint32_t in_event_type, int32_t in_event_code, void * in_data1, void * in_data2) {
    SDL_Log("Event_Emit");
    SDL_assert(in_event_type != ((UINT32_MAX) - 1));
    // SDL_Event * event = (SDL_Event *)SDL_calloc(1, sizeof(SDL_Event));
    SDL_Event event;
    event.type = in_event_type;
    event.user.code = in_event_code;
    event.user.data1 = in_data1;
    event.user.data2 = in_data2;
    SDL_PushEvent(&event);
}

void receive_Map_globalRange_Hide(struct Game * in_game, SDL_Event * in_userevent) {
    SDL_Log("receive_Map_globalRange_Hide");
    struct Menu * menu_top_ptr = TNECS_GET_COMPONENT(in_game->world, in_game->menu_stack[(in_game->menu_stack_num - 1)], Menu);
    struct controllerKeyboard * keyboard_ptr = TNECS_GET_COMPONENT(in_game->world, in_game->entity_cursor, controllerKeyboard);
    struct controllerGamepad * gamepad_ptr = TNECS_GET_COMPONENT(in_game->world, in_game->entity_cursor, controllerGamepad);
    menu_top_ptr->enabled = true;
    in_game->map_ptr->show_globalRange = false;
    in_game->map_ptr->mode_globalRange = 0;
    Game_Cursor_Enable(in_game);
    gamepad_ptr->block_move = false;
    keyboard_ptr->block_move = false;
    strncpy(in_game->reason, "global range was hidden", sizeof(in_game->reason));
    Game_subState_Set(in_game, GAME_SUBSTATE_MENU, in_game->reason);
}

void receive_Map_globalRange_Show(struct Game * in_game, SDL_Event * in_userevent) {
    SDL_Log("receive_Map_globalRange_Show");
    uint8_t alignment = ALIGNMENT_FRIENDLY;
    Map_globalRange(in_game->map_ptr, in_game->world, alignment);
    in_game->map_ptr->mode_globalRange = alignment;
    struct Menu * menu_top_ptr = TNECS_GET_COMPONENT(in_game->world, in_game->menu_stack[(in_game->menu_stack_num - 1)], Menu);
    struct controllerKeyboard * keyboard_ptr = TNECS_GET_COMPONENT(in_game->world, in_game->entity_cursor, controllerKeyboard);
    struct controllerGamepad * gamepad_ptr = TNECS_GET_COMPONENT(in_game->world, in_game->entity_cursor, controllerGamepad);
    menu_top_ptr->enabled = false;
    in_game->map_ptr->show_globalRange = true;
    gamepad_ptr->block_move = true;
    keyboard_ptr->block_move = true;
    // Game_Cursor_Disable(in_game);
    strncpy(in_game->reason, "global range is being shown", sizeof(in_game->reason));
    Game_subState_Set(in_game, GAME_SUBSTATE_MAP_GLOBAL_RANGE, in_game->reason);
}


void receive_Cursor_Moved(struct Game * in_game, SDL_Event * in_userevent) {
    SDL_Log("receive_Cursor_Moved");
    // struct Point * move  = (struct Point *) in_userevent->user.data1;
    tnecs_entity_t unit_entity_ontile;
    tnecs_entity_t unit_entity_previoustile;
    struct Position * cursor_position_ptr = TNECS_GET_COMPONENT(in_game->world, in_game->entity_cursor, Position);
    struct Position * selected_position_ptr;
    struct Point * cursor_move = (struct Point *) in_userevent->user.data1;
    // Position_Pos_Add(cursor_position_ptr, cursor_move->x, cursor_move->y);
    struct Point previous_pos;
    struct Point current_pos = cursor_position_ptr->tilemap_pos;
    struct Point offset = cursor_position_ptr->offset_px;
    struct Point boundsmin = cursor_position_ptr->boundsmin;
    struct Point boundsmax = cursor_position_ptr->boundsmax;
    // current_pos.x = cursor_pos.x - offset.x;
    // current_pos.y = cursor_pos.y - offset.y;

    switch (in_game->state) {
        case GAME_STATE_Gameplay_Map:
            previous_pos.x = current_pos.x - cursor_move->x;
            previous_pos.y = current_pos.y - cursor_move->y;
            unit_entity_previoustile = in_game->map_ptr->unitmap[previous_pos.y * in_game->map_ptr->col_len + previous_pos.x];
            unit_entity_ontile = in_game->map_ptr->unitmap[current_pos.y * in_game->map_ptr->col_len + current_pos.x];

            if (unit_entity_previoustile != 0) {
                Event_Emit(SDL_USEREVENT, event_Cursor_Dehovers_Unit, &unit_entity_previoustile, NULL);
            }

            if (unit_entity_ontile != 0) {
                Event_Emit(SDL_USEREVENT, event_Cursor_Hovers_Unit, &unit_entity_ontile, NULL);
            }
            switch (in_game->substate) {
                case GAME_SUBSTATE_MAP_UNIT_MOVES:
                    // Unit follows cursor movement.
                    selected_position_ptr = TNECS_GET_COMPONENT(in_game->world, in_game->selected_unit_entity, Position);
                    Position_Pos_Set(selected_position_ptr,  current_pos.x, current_pos.y);
                    break;
            }
            break;

        default:
            SDL_Log("receive_Cursor_Moved game state is invalid");
    }
}

void receive_Input_Cancel(struct Game * in_game, SDL_Event * in_userevent) {
    SDL_Log("Received Input_Cancel event");
    int32_t controller_type = * (int32_t *) in_userevent->user.data1;
    tnecs_entity_t canceller_entity = Events_Controllers_Check(in_game, controller_type);

    tnecs_entity_t unit_entity_ontile = 0;
    tnecs_entity_t menu_popped = 0;

    struct Position * canceller_position_ptr, * selected_position_ptr;
    const struct Unit * unit_ontile_ptr;
    struct Menu * menu_popped_ptr = NULL;
    struct Point new_pos;
    bool destroy = false;

    if (canceller_entity != 0) {
        *data1_entity = canceller_entity;
        canceller_position_ptr = TNECS_GET_COMPONENT(in_game->world, canceller_entity, Position);
        struct Point cursor_pos = canceller_position_ptr->tilemap_pos;
        new_pos.x = cursor_pos.x;
        new_pos.y = cursor_pos.y;

        switch (in_game->state) {
            case GAME_STATE_Gameplay_Map:
                switch (in_game->substate) {
                    case GAME_SUBSTATE_STANDBY:
                        unit_entity_ontile = in_game->map_ptr->unitmap[new_pos.y * in_game->map_ptr->col_len + new_pos.x];

                        if (unit_entity_ontile != 0) {
                            unit_ontile_ptr = TNECS_GET_COMPONENT(in_game->world, canceller_entity, Unit);
                            *data2_entity = unit_entity_ontile;
                            if (utilities_isPC(unit_ontile_ptr->army)) {

                            } else {
                                if (unit_ontile_ptr->show_danger) {
                                    Event_Emit(SDL_USEREVENT, event_Unit_Danger, data1_entity, data2_entity);
                                }
                            }
                        }

                        break;

                    case GAME_SUBSTATE_MENU:
                        if (in_game->menu_stack_num > 0) {
                            Game_Menu_Disable_Entity(in_game, in_game->menu_stack[(in_game->menu_stack_num - 1)]);
                            menu_popped = Game_menuStack_Pop(in_game, destroy);
                            menu_popped_ptr = TNECS_GET_COMPONENT(in_game->world, menu_popped, Menu);
                            SDL_Log("Just before Game_subStateSwitch_onMenupop, in_game->menu_stack_num %d", in_game->menu_stack_num);
                            Game_subStateSwitch_onMenupop(in_game, menu_popped_ptr->id);
                            if (in_game->menu_stack_num > 0) {
                                Game_cursorFocus_onMenu(in_game);
                            } else {
                                Game_cursorFocus_onMap(in_game);
                            }

                        }
                        break;

                    case GAME_SUBSTATE_MAP_UNIT_MOVES:
                        if (in_game->selected_unit_entity != 0) {
                            *data2_entity = in_game->selected_unit_entity;
                            selected_position_ptr = TNECS_GET_COMPONENT(in_game->world, in_game->selected_unit_entity, Position);
                            selected_position_ptr->tilemap_moveable = NULL;
                            selected_position_ptr->tilemap_row = 0;
                            selected_position_ptr->tilemap_col = 0;
                            Event_Emit(SDL_USEREVENT, event_Unit_Return, data1_entity, data2_entity);
                            Event_Emit(SDL_USEREVENT, event_Unit_Deselect, data1_entity, data2_entity);
                        }
                        break;


                    case GAME_SUBSTATE_ANIMATION:
                        // Interrupt animation?
                        break;

                    case GAME_SUBSTATE_MAP_COMBAT:
                    case GAME_SUBSTATE_MAP_NPCTURN:
                        break;

                    case GAME_SUBSTATE_MAP_GLOBAL_RANGE:
                        strncpy(in_game->reason, "stops showing global Range", sizeof(in_game->reason));
                        Game_subState_Set(in_game, GAME_SUBSTATE_MENU, in_game->reason);
                        Event_Emit(SDL_USEREVENT, event_Map_globalRange_Hide, NULL, NULL);
                        break;

                    case GAME_SUBSTATE_MAP_MINIMAP:
                    case GAME_SUBSTATE_PAUSED:

                    default:
                        SDL_Log("receive_Input_Cancel game substate is invalid");
                        Event_Emit(SDL_USEREVENT, event_Gameplay_Return2Standby, NULL, NULL);
                        break;
                }
                break;
            default:
                SDL_Log("receive_Input_Cancel game state is invalid");
                break;
        }
    }
}

void receive_Mouse_Disable(struct Game * in_game, SDL_Event * in_Mouse_Disable) {
    SDL_Log("receive_Mouse_Disable");
    Game_Mouse_Disable(in_game);

}

void receive_Mouse_Enable(struct Game * in_game, SDL_Event * in_Mouse_Enable) {
    SDL_Log("receive_Mouse_Enable");
    Game_Mouse_Enable(in_game);
    in_game->mouse_idletime = 0.0f;
}

void receive_Cursor_Enable(struct Game * in_game, SDL_Event * in_Cursor_Enable) {
    SDL_Log("receive_Cursor_Enable");
    Game_Cursor_Enable(in_game);
}

void receive_Cursor_Disable(struct Game * in_game, SDL_Event * in_Cursor_Disable) {
    SDL_Log("receive_Cursor_Disable");
    Game_Cursor_Disable(in_game);
}

void receive_Turn_Begin(struct Game * in_game, SDL_Event * in_userevent) {
    SDL_Log("receive_Turn_Begin");
    uint8_t army = * (uint8_t *) in_userevent->user.data1;
}

void receive_Game_Control_Switch(struct Game * in_game, SDL_Event * in_userevent) {
    SDL_Log("receive_Game_Control_Switch");
    uint8_t army = * (uint8_t *) in_userevent->user.data1;

    if (utilities_isPC(army)) {
        // Game_State_Set(in_game, GAME_STATE_Gameplay_Map);
        if (in_game->entity_cursor == 0) {
            Game_Cursor_Create(in_game);
        }
        Game_Cursor_Enable(in_game);
        // Game_setCursorstate(in_game, MENU_MAP);
        // Game_Mouse_State_Set(in_game, MENU_MAP);

    } else {
        strncpy(in_game->reason, "control was switched to a non-player army", sizeof(in_game->reason));
        Game_subState_Set(in_game, GAME_SUBSTATE_MAP_NPCTURN, in_game->reason);
        Game_Cursor_Destroy(in_game);
    }
}

void receive_Input_Stats(struct Game * in_game, SDL_Event * in_userevent) {
    SDL_Log("receive_Input_Stats");

}

tnecs_entity_t Events_Controllers_Check(struct Game * in_game, int32_t in_code) {
    SDL_Log("Events_Controllers_Check");
    tnecs_entity_t out_accepter_entity;
    switch (in_code) {
        case CONTROLLER_MOUSE:
            if (!in_game->ismouse) {
                Event_Emit(SDL_USEREVENT, event_Mouse_Enable, NULL, NULL);
                // Event_Emit(SDL_USEREVENT, event_Cursor_Disable, NULL, NULL);
            }
            out_accepter_entity = in_game->entity_mouse;
            break;
        case CONTROLLER_GAMEPAD:
        case CONTROLLER_KEYBOARD:
        case CONTROLLER_TOUCHPAD:
            if (!in_game->iscursor) {
                Event_Emit(SDL_USEREVENT, event_Mouse_Disable, NULL, NULL);
                Event_Emit(SDL_USEREVENT, event_Cursor_Enable, NULL, NULL);
            }
            out_accepter_entity = in_game->entity_cursor;
            struct controllerGamepad * gamepad_ptr = TNECS_GET_COMPONENT(in_game->world, in_game->entity_cursor, controllerGamepad);
            struct controllerKeyboard * keyboard_ptr = TNECS_GET_COMPONENT(in_game->world, in_game->entity_cursor, controllerKeyboard);
            gamepad_ptr->block_buttons = true;
            keyboard_ptr->block_buttons = true;
            break;
        default:
            SDL_Log("controller code is invalid.");
    }
    return (out_accepter_entity);
}

void receive_Gameplay_Return2Standby(struct Game * in_game, SDL_Event * in_userevent) {
    SDL_Log("receive_Gameplay_Return2Standby");

    int32_t controller_type = * (int32_t *) in_userevent->user.data1;
    tnecs_entity_t controller_entity = Events_Controllers_Check(in_game, controller_type);

    *data1_entity = controller_entity;

    bool destroy = false;

    *data2_entity = in_game->selected_unit_entity;

    while (in_game->menu_stack_num > 0) {
        Game_Menu_Disable_Entity(in_game, in_game->menu_stack[in_game->menu_stack_num - 1]);
        Game_menuStack_Pop(in_game, destroy);
    }

    switch (in_game->state) {
        case GAME_STATE_Gameplay_Camp:
            break;
        case GAME_STATE_Gameplay_Map:
            if (in_game->selected_unit_entity != 0) {
                Event_Emit(SDL_USEREVENT, event_Unit_Return, data1_entity, data2_entity);
                Event_Emit(SDL_USEREVENT, event_Unit_Deselect, data1_entity, data2_entity);
                in_game->map_ptr->show_overlay = false;
            }
            break;
        default:
            SDL_Log("Game_state was invalid for event_Gameplay_Return2Standby");
            break;
    }
    strncpy(in_game->reason, "game->substate returns to Standby after error", sizeof(in_game->reason));
    Game_subState_Set(in_game, GAME_SUBSTATE_STANDBY, in_game->reason);
}

void receive_Input_Accept(struct Game * in_game, SDL_Event * in_userevent) {
    SDL_Log("receive_Input_Accept");
    int32_t controller_type = * (int32_t *) in_userevent->user.data1;
    tnecs_entity_t accepter_entity = Events_Controllers_Check(in_game, controller_type);

    tnecs_entity_t unit_entity_ontile = 0, menu_entity_top = 0;
    struct Position * accepter_position_ptr, * selected_position_ptr;
    const struct Unit * accepted_unit_ptr;
    struct Point current_pos;
    struct Point accepter_pos;
    struct Point offset;
    struct MenuOption menuOption_selected;
    uint16_t menuwidth = 7;
    struct Menu * current_menu_ptr;
    if (accepter_entity != 0) {
        *data1_entity = accepter_entity;
        switch (in_game->state) {
            case GAME_STATE_Gameplay_Map:
                switch (in_game->substate) {
                    case GAME_SUBSTATE_MENU:
                        Event_Emit(SDL_USEREVENT, event_Menu_Select, data1_entity, NULL);
                        break;
                    case GAME_SUBSTATE_MAP_COMBAT:
                    case GAME_SUBSTATE_MAP_NPCTURN:
                        // Go faster.
                        break;

                    case GAME_SUBSTATE_ANIMATION:
                        // Complete animation fast.
                        break;
                    case GAME_SUBSTATE_MAP_MINIMAP:
                    case GAME_SUBSTATE_PAUSED:
                        Event_Emit(SDL_USEREVENT, event_Gameplay_Return2Standby, in_userevent->user.data1, NULL);
                        break;
                    case GAME_SUBSTATE_STANDBY:
                        accepter_position_ptr = TNECS_GET_COMPONENT(in_game->world, accepter_entity, Position);
                        if (accepter_position_ptr != NULL) {
                            accepter_pos = accepter_position_ptr->tilemap_pos;
                            // offset = accepter_position_ptr->offset_px;
                            current_pos.x = accepter_pos.x;
                            current_pos.y = accepter_pos.y;
                            SDL_Log("current_pos: %d %d", current_pos.x, current_pos.y);
                            if (in_game->iscursor) {
                                in_game->cursor_lastpos.x = current_pos.x;
                                in_game->cursor_lastpos.y = current_pos.y;
                            }
                            if (accepter_entity == in_game->entity_mouse) {

                            }

                            unit_entity_ontile = in_game->map_ptr->unitmap[current_pos.y * in_game->map_ptr->col_len + current_pos.x];

                            if (unit_entity_ontile != 0) {
                                *data2_entity = unit_entity_ontile;
                                Event_Emit(SDL_USEREVENT, event_Unit_Select, data1_entity, data2_entity);
                            } else {
                                if (in_game->menu_loaded[MENU_MAP_ACTION] == 0) {
                                    Game_Menu_Create(in_game, MENU_MAP_ACTION);
                                }
                                Game_menuStack_Push(in_game, in_game->menu_loaded[MENU_MAP_ACTION]);
                                Game_Menu_Update(in_game, MENU_MAP_ACTION);
                                Game_cursorFocus_onMenu(in_game);
                                strncpy(in_game->reason, "no unit was selected", sizeof(in_game->reason));
                                Game_subState_Set(in_game, GAME_SUBSTATE_MENU, in_game->reason);
                            }
                        }
                        break;

                    case GAME_SUBSTATE_MAP_UNIT_MOVES:
                        if (in_game->selected_unit_entity != 0) {
                            accepted_unit_ptr = TNECS_GET_COMPONENT(in_game->world, in_game->selected_unit_entity, Unit);
                            accepter_position_ptr = TNECS_GET_COMPONENT(in_game->world, accepter_entity, Position);
                            selected_position_ptr = TNECS_GET_COMPONENT(in_game->world, in_game->selected_unit_entity, Position);
                            if (utilities_isPC(accepted_unit_ptr->army)) {
                                in_game->selected_unit_moved_position.x = accepter_position_ptr->tilemap_pos.x;
                                in_game->selected_unit_moved_position.y = accepter_position_ptr->tilemap_pos.y;

                                if (in_game->menu_loaded[MENU_UNIT_ACTION] == 0) {
                                    Game_Menu_Create(in_game, MENU_UNIT_ACTION);
                                }
                                Game_menuStack_Push(in_game, in_game->menu_loaded[MENU_UNIT_ACTION]);
                                Game_Menu_Update(in_game, MENU_UNIT_ACTION);
                                Game_cursorFocus_onMenu(in_game);
                                strncpy(in_game->reason, "friendly unit has moved, time to choose action", sizeof(in_game->reason));
                                Game_subState_Set(in_game, GAME_SUBSTATE_MENU, in_game->reason);

                                Map_Unit_Move(in_game->map_ptr, in_game->selected_unit_initial_position.x, in_game->selected_unit_initial_position.y, in_game->selected_unit_moved_position.x, in_game->selected_unit_moved_position.y);
                                Position_Pos_Set(selected_position_ptr, in_game->selected_unit_moved_position.x, in_game->selected_unit_moved_position.y);
                                SDL_Log("in_game->selected_unit_moved_position: %d %d", in_game->selected_unit_moved_position.x, in_game->selected_unit_moved_position.y);
                            }
                        } else {
                            Event_Emit(SDL_USEREVENT, event_Gameplay_Return2Standby, data1_entity, NULL);
                        }
                        break;
                    default:
                        SDL_Log("game substate is invalid.");
                        Event_Emit(SDL_USEREVENT, event_Gameplay_Return2Standby, NULL, NULL);
                }
        }
    }
}

void receive_SDL_AUDIODEVICEADDED(struct Game * in_game, SDL_Event * in_event) {
    SDL_Log("receive_SDL_AUDIODEVICEADDED");

}

void receive_SDL_AUDIODEVICEREMOVED(struct Game * in_game, SDL_Event * in_event) {
    SDL_Log("receive_SDL_AUDIODEVICEREMOVED");

}

void receive_SDL_CONTROLLERBUTTONDOWN(struct Game * in_game, SDL_Event * in_event) {
    SDL_Log("receive_SDL_CONTROLLERBUTTONDOWN");

}

void receive_SDL_KEYDOWN(struct Game * in_game, SDL_Event * in_event) {
    SDL_Log("receive_SDL_KEYDOWN");

}

void receive_SDL_QUIT(struct Game * in_game, SDL_Event * in_event) {
    SDL_Log("Handling SDL_QUIT event");
    in_game->isrunning = false;
}

void receive_SDL_CONTROLLERDEVICEREMOVED(struct Game * in_game, SDL_Event * in_event) {
    SDL_Log("receive_SDL_CONTROLLERDEVICEREMOVED");

    if (in_game->entity_cursor != 0) {

        struct controllerGamepad * gamepad_ptr = TNECS_GET_COMPONENT(in_game->world, in_game->entity_cursor, controllerGamepad);

        if (gamepad_ptr != NULL) {
            Gamepad_removeController(gamepad_ptr, in_event->cdevice.which);

        } else {
            SDL_Log("entity_cursor has no controllerGamepad component");
        }
    } else {
        SDL_Log("entity_cursor is not valid");
    }
}


void receive_SDL_CONTROLLERDEVICEADDED(struct Game * in_game, SDL_Event * in_event) {
    SDL_Log("receive_SDL_CONTROLLERDEVICEADDED");

    if (in_game->entity_cursor != 0) {
        struct controllerGamepad * gamepad_ptr = TNECS_GET_COMPONENT(in_game->world, in_game->entity_cursor, controllerGamepad);

        if (gamepad_ptr != NULL) {
            Gamepad_addController(gamepad_ptr, in_event->cdevice.which);

        } else {
            SDL_Log("entity_cursor has no controllerGamepad component");
            SDL_Log("entity_cursor has no controllerGamepad component");
            SDL_Log("entity_cursor has no controllerGamepad component");
            SDL_Log("entity_cursor has no controllerGamepad component");
        }
    } else {
        SDL_Log("entity_cursor is not valid");
    }
}


void receive_SDL_MOUSEMOTION(struct Game * in_game, SDL_Event * in_event) {
    // SDL_Log("receive_SDL_MOUSEMOTION");
    if (in_event->motion.windowID == SDL_GetWindowID(in_game->window)) {

        // SDL_Log("WindowID: %d %d", in_event->motion.windowID, SDL_GetWindowID(in_game->window));
        struct Sprite * sprite_ptr = TNECS_GET_COMPONENT(in_game->world, in_game->entity_mouse, Sprite);
        struct Position * position_ptr = TNECS_GET_COMPONENT(in_game->world, in_game->entity_mouse, Position);
        // if (sprite_ptr != NULL) {
        // sprite_ptr->visible = true;
        // }

        // SDL_Log("Mouse motion event rel: %d %d", in_event->motion.xrel, in_event->motion.yrel);
        // SDL_Log("Mouse motion event pos: %d %d", in_event->motion.x, in_event->motion.y);
        // SDL_Log("Mouse last position: %d %d", in_game->mouse_lastpos.x, in_game->mouse_lastpos.y);

        // this is cause in_event->motion.xrel does not work
        if ((in_event->motion.x != position_ptr->pixel_pos.x) || (in_event->motion.y != position_ptr->pixel_pos.y)) {
            if (in_game->entity_mouse != 0) {
                if (!in_game->ismouse) {
                    Event_Emit(SDL_USEREVENT, event_Mouse_Enable, NULL, NULL);
                    // Event_Emit(SDL_USEREVENT, event_Cursor_Disable, NULL, NULL);
                }

                struct Position * position_ptr = TNECS_GET_COMPONENT(in_game->world, in_game->entity_mouse, Position);

                if (position_ptr != NULL) {
                    Position_PosPixel_Set(position_ptr, in_event->motion.x, in_event->motion.y);
                }

                if (in_game->map_ptr != NULL) {
                    struct Point tilemap_pos = Position_pixel2tilemap(position_ptr, in_event->motion.x, in_event->motion.y);
                    Position_PosTilemap_Set(position_ptr, tilemap_pos.x, tilemap_pos.y);
                }
                // SDL_Log("in_event->motion: %d %d", in_event->motion.x, in_event->motion.y);
                position_ptr->pixel_pos.x = in_event->motion.x;
                position_ptr->pixel_pos.y = in_event->motion.y;
            }
        }
    }
}

void receive_SDL_MOUSEBUTTON(struct Game * in_game, SDL_Event * in_event) {
    SDL_Log("receive_SDL_MOUSEBUTTON");
    if (in_event->button.windowID == SDL_GetWindowID(in_game->window)) {

        if (in_game->entity_mouse > 0) {
            struct controllerMouse * mouse_ptr = TNECS_GET_COMPONENT(in_game->world, in_game->entity_mouse, controllerMouse);
            struct Position * position_ptr = TNECS_GET_COMPONENT(in_game->world, in_game->entity_mouse, Position);
            SDL_assert(position_ptr);

            if (mouse_ptr != NULL) {
                // SDL_Log("event pos: %d %d", in_event->button.x, in_event->button.y);
                Position_PosPixel_Set(position_ptr, in_event->button.x, in_event->button.y);

                if (in_game->map_ptr != NULL) {
                    struct Point tilemap_pos = Position_pixel2tilemap(position_ptr, in_event->button.x, in_event->button.y);
                    Position_PosTilemap_Set(position_ptr, tilemap_pos.x, tilemap_pos.y);
                }

                if (in_event->button.state == SDL_PRESSED) {
                    if (array_isIn_uint8_t(in_game->mouseInputMap.cancel, in_event->button.button, DEFAULT_MAPPABLE_BUTTONS)) {
                        struct Position * cursor_pos = TNECS_GET_COMPONENT(in_game->world, in_game->entity_cursor, Position);
                        struct Point pixel_pos = Position_tilemap2pixel(cursor_pos, position_ptr->tilemap_pos.x, position_ptr->tilemap_pos.y);
                        cursor_pos->pixel_pos.x = pixel_pos.x;
                        cursor_pos->pixel_pos.y = pixel_pos.y;
                        Event_Emit(SDL_USEREVENT, event_Input_Cancel, &in_game->entity_mouse, NULL);
                    }

                    if (array_isIn_uint8_t(in_game->mouseInputMap.accept, in_event->button.button, DEFAULT_MAPPABLE_BUTTONS)) {
                        struct Position * cursor_pos = TNECS_GET_COMPONENT(in_game->world, in_game->entity_cursor, Position);
                        struct Point tilemap_pos = Position_pixel2tilemap(cursor_pos, position_ptr->pixel_pos.x, position_ptr->pixel_pos.y);
                        cursor_pos->tilemap_pos.x = tilemap_pos.x;
                        cursor_pos->tilemap_pos.y = tilemap_pos.y;
                        Event_Emit(SDL_USEREVENT, event_Input_Accept, &in_game->entity_mouse, NULL);
                    }
                }
            } else {
                SDL_Log("cursor has no controllerMouse component");
            }
        }
    } else {
        SDL_Log("entity_mouse is not valid");
    }
}

void receive_Turn_End(struct Game * in_game, SDL_Event * in_userevent) {
    SDL_Log("receive_Turn_End");
    arrput(in_game->map_ptr->armies_onfield, in_game->map_ptr->armies_onfield[0]);
    arrdel(in_game->map_ptr->armies_onfield, 0);
    Event_Emit(SDL_USEREVENT, event_Turn_Begin, NULL, NULL);
}

void receive_Unit_Enters_Shop(struct Game * in_game, SDL_Event * in_userevent) {
    SDL_Log("receive_Unit_Enters_Shop");
}
void receive_Unit_Enters_Village(struct Game * in_game, SDL_Event * in_userevent) {
    SDL_Log("receive_Unit_Enters_Village");
}
void receive_Unit_Enters_Armory(struct Game * in_game, SDL_Event * in_userevent) {
    SDL_Log("receive_Unit_Enters_Armory");
}


void receive_Unit_Select(struct Game * in_game, SDL_Event * in_userevent) {
    SDL_Log("receive_Unit_Deselect");
    tnecs_entity_t * selector_entity = in_userevent->user.data1;
    // Only Unit_Select event puts non-zero in in_game->selected_unit_entity
    in_game->selected_unit_entity = *((tnecs_entity_t *) in_userevent->user.data2);

    struct Unit * selected_unit_ptr = TNECS_GET_COMPONENT(in_game->world, in_game->selected_unit_entity, Unit);
    struct Position * selected_position_ptr = TNECS_GET_COMPONENT(in_game->world, in_game->selected_unit_entity, Position);
    in_game->selected_unit_initial_position.x = selected_position_ptr->tilemap_pos.x;
    in_game->selected_unit_initial_position.y = selected_position_ptr->tilemap_pos.y;

    *data1_entity = *selector_entity;

    if (utilities_isPC(selected_unit_ptr->army)) {
        switch (in_game->substate) {
            case GAME_SUBSTATE_STANDBY:
                if (selected_unit_ptr->waits) {
                    if (in_game->menu_loaded[MENU_UNIT_ACTION] == 0) {
                        Game_Menu_Create(in_game, MENU_MAP_ACTION);
                    }
                    Game_menuStack_Push(in_game, in_game->menu_loaded[MENU_MAP_ACTION]);
                    Game_Menu_Update(in_game, MENU_MAP_ACTION);
                    Game_cursorFocus_onMenu(in_game);
                    strncpy(in_game->reason, "friendly unit was selected but cannot move", sizeof(in_game->reason));
                    Game_subState_Set(in_game, GAME_SUBSTATE_MENU, in_game->reason);
                } else {
                    selected_position_ptr->tilemap_moveable = in_game->map_ptr->overlay_move;
                    selected_position_ptr->tilemap_row = in_game->map_ptr->row_len;
                    selected_position_ptr->tilemap_col = in_game->map_ptr->col_len;
                    Event_Emit(SDL_USEREVENT, event_Unit_Moves, data1_entity, data2_entity);
                }
                break;
            default:
                SDL_Log("Unit_select, only during GAME_STATE_Gameplay_Map and GAME_SUBSTATE_STANDBY");
                break;
        }
    } else {
        switch (in_game->substate) {
            case GAME_SUBSTATE_STANDBY:
                Event_Emit(SDL_USEREVENT, event_Unit_Danger, data1_entity, data2_entity);
                strncpy(in_game->reason, "enemy unit was selected", sizeof(in_game->reason));
                break;
            default:
                SDL_Log("Unit_select, only during GAME_STATE_Gameplay_Map and GAME_SUBSTATE_STANDBY");
                break;
        }
    }
}

void receive_Unit_Deselect(struct Game * in_game, SDL_Event * in_userevent) {
    SDL_Log("receive_Unit_Deselect");
    tnecs_entity_t * selector_entity = (tnecs_entity_t *) in_userevent->user.data1;

    if (in_game->selected_unit_entity > 0) {
        struct Unit * unit_ptr = TNECS_GET_COMPONENT(in_game->world, in_game->selected_unit_entity, Unit);
        if (unit_ptr != NULL) {
            if (utilities_isPC(unit_ptr->army)) {
                in_game->map_ptr->show_overlay = false;
            } else {
                if (unit_ptr->show_danger) {
                    Event_Emit(SDL_USEREVENT, event_Unit_Danger, selector_entity, &in_game->selected_unit_entity);
                }
            }
            switch (in_game->substate) {
                case GAME_SUBSTATE_MENU:
                    strncpy(in_game->reason, "Unit was deselected during menu (I guess this means unit waits?)", sizeof(in_game->reason));
                    Game_subState_Set(in_game, GAME_SUBSTATE_STANDBY, in_game->reason);
                    break;
                case GAME_SUBSTATE_MAP_UNIT_MOVES:
                    strncpy(in_game->reason, "Unit was deselected during movement", sizeof(in_game->reason));
                    Game_subState_Set(in_game, GAME_SUBSTATE_STANDBY, in_game->reason);
                    break;
                default:
                    strncpy(in_game->reason, "Unit was deselected during invalid game substate", sizeof(in_game->reason));
                    Game_subState_Set(in_game, GAME_SUBSTATE_STANDBY, in_game->reason);
                    break;
            }
        }
        in_game->selected_unit_entity = 0;
    }

    in_game->selected_unit_initial_position.x = -1;
    in_game->selected_unit_initial_position.y = -1;
    in_game->selected_unit_moved_position.x = -1;
    in_game->selected_unit_moved_position.y = -1;
}

void receive_Unit_Return(struct Game * in_game, SDL_Event * in_userevent) {
    SDL_Log("receive_Unit_Return");
    in_game->map_ptr->show_overlay = false;

    tnecs_entity_t * selector_entity = (tnecs_entity_t *)in_userevent->user.data1;
    tnecs_entity_t * returning_unit_entity = (tnecs_entity_t *)in_userevent->user.data2;

    struct Point new_position;
    struct Position toreturn_position;

    if ((selector_entity != 0) && (returning_unit_entity != 0)) {
        struct Position * selector_position_ptr = TNECS_GET_COMPONENT(in_game->world, *selector_entity, Position);
        struct Unit * returning_unit_ptr  = TNECS_GET_COMPONENT(in_game->world, *returning_unit_entity, Unit);
        struct Position * returning_position_ptr  = TNECS_GET_COMPONENT(in_game->world, *returning_unit_entity, Position);

        if ((returning_unit_ptr != NULL) && (selector_position_ptr != NULL) && (returning_position_ptr != NULL)) {

            Map_Unit_Move(in_game->map_ptr, in_game->selected_unit_moved_position.x, in_game->selected_unit_moved_position.y, in_game->selected_unit_initial_position.x, in_game->selected_unit_initial_position.y);

            Position_Pos_Set(returning_position_ptr, in_game->selected_unit_initial_position.x, in_game->selected_unit_initial_position.y);

            in_game->selected_unit_initial_position.x = -1;
            in_game->selected_unit_initial_position.y = -1;
            in_game->selected_unit_moved_position.x = -1;
            in_game->selected_unit_moved_position.y = -1;

        } else {
            SDL_Log("Could not get selector_entity or selected_unit_entity position/unit components");
        }
    } else {
        SDL_Log("Could not get selected and selector entities");
    }
}

void receive_Unit_Moves(struct Game * in_game, SDL_Event * in_userevent) {
    SDL_Log("receive_Unit_Moves");
    // In this state, position of Silou should be equal to cursor position.
    // -> TO BECOME AN ARROW SOMEDAY.
    int16_t * costmapp = NULL;
    int16_t * movemapp = NULL;
    int16_t * attackmapp = NULL;
    tnecs_entity_t * selector_entity = in_userevent->user.data1;

    if ((*selector_entity != 0) && (in_game->selected_unit_entity != 0)) {
        struct Position * selector_position_ptr = TNECS_GET_COMPONENT(in_game->world, *selector_entity, Position);
        struct Unit * selected_unit_ptr = TNECS_GET_COMPONENT(in_game->world, in_game->selected_unit_entity, Unit);
        if ((selector_position_ptr != NULL) && (selected_unit_ptr != NULL)) {

            struct Point start;
            struct Point offset;
            struct Point nooffset;
            uint32_t unit_move;
            int_tile_t * range;

            start = selector_position_ptr->tilemap_pos;
            // offset = selector_position_ptr->offset_px;
            nooffset.x = start.x;
            nooffset.y = start.y;

            unit_move = Unit_effectiveStats(selected_unit_ptr).move;
            range = Unit_computeRange(selected_unit_ptr);

            costmapp = Map_Costmap_Movement_Compute(in_game->map_ptr, in_game->world, in_game->selected_unit_entity);
            matrix_print_int16_t(costmapp, in_game->map_ptr->row_len, in_game->map_ptr->col_len);
            movemapp = Pathfinding_Map_Move(costmapp, in_game->map_ptr->row_len, in_game->map_ptr->col_len,  start, unit_move, POINTS_MATRIX);
            matrix_print_int16_t(movemapp, in_game->map_ptr->row_len, in_game->map_ptr->col_len);
            attackmapp = Pathfinding_Map_Attack(movemapp, in_game->map_ptr->row_len, in_game->map_ptr->col_len, unit_move, range, POINTS_MATRIX, MOVETILE_EXCLUDE);
            matrix_print_int16_t(attackmapp, in_game->map_ptr->row_len, in_game->map_ptr->col_len);

            Map_Overlays_Set(in_game->map_ptr, MAP_OVERLAY_MOVE, movemapp);
            Map_Overlays_Set(in_game->map_ptr, MAP_OVERLAY_ATTACK, attackmapp);

            in_game->map_ptr->show_overlay = true;

            strncpy(in_game->reason, "friendly unit was selected and can move", sizeof(in_game->reason));
            Game_subState_Set(in_game, GAME_SUBSTATE_MAP_UNIT_MOVES, in_game->reason);
        } else {
            SDL_Log("Could not get selected unit component or selector component");
        }
    } else {
        SDL_Log("Could not get selected unit entity or selector entity");
    }
}

void receive_Cursor_Hovers_Unit(struct Game * in_game, SDL_Event * in_userevent) {
    SDL_Log("receive_Cursor_Hovers_Unit");

}

void receive_Cursor_Dehovers_Unit(struct Game * in_game, SDL_Event * in_userevent) {
    SDL_Log("receive_Cursor_Dehovers_Unit");
    arrput(in_game->map_ptr->armies_onfield, in_game->map_ptr->armies_onfield[0]);
    arrdel(in_game->map_ptr->armies_onfield, 0);
    // Event_Emit(SDL_USEREVENT, event_Turn_Begin, NULL, NULL);
    uint8_t army = * (uint8_t *)in_userevent->user.data1;

    tnecs_entity_t * unit_entitys = Map_Unit_Gets(in_game->map_ptr, in_game->world, army);

    // SDL_Log("units size: %d", units.size());
    switch (in_game->substate) {
        case GAME_SUBSTATE_STANDBY:
            for (uint16_t i = 0; i < arrlen(unit_entitys); i++) {
                struct Unit * unit_ptr = TNECS_GET_COMPONENT(in_game->world, unit_entitys[i], Unit);
                Unit_refresh(unit_ptr);
            }
            break;
    }
}

void receive_Units_Refresh(struct Game * in_game, SDL_Event * in_userevent) {
    SDL_Log("receive_Units_Refresh");

}

void receive_Unit_Danger(struct Game * in_game, SDL_Event * in_userevent) {
    SDL_Log("receive_Unit_Danger");
    int_tile_t * costmapp = NULL;
    int_tile_t * movemapp = NULL;
    int_tile_t * attackmapp = NULL;
    int_tile_t * dangermapp = NULL;
    uint32_t unit_move;
    struct Point start;
    int_tile_t * range;

    tnecs_entity_t * selector_entity = in_userevent->user.data1;

    if ((selector_entity != 0) && (in_game->selected_unit_entity != 0)) {
        struct Position * selector_position_ptr = TNECS_GET_COMPONENT(in_game->world, *selector_entity, Position);
        struct Unit * selected_unit_ptr = TNECS_GET_COMPONENT(in_game->world, in_game->selected_unit_entity, Unit);
        if ((selector_position_ptr != NULL) && (selected_unit_ptr != NULL)) {

            struct Point cursorpos = selector_position_ptr->tilemap_pos;
            // struct Point offset = selector_position_ptr->offset_px;
            start.x = cursorpos.x;
            start.y = cursorpos.y;

            unit_move = Unit_getStats(selected_unit_ptr).move;
            range = Unit_computeRange(selected_unit_ptr);

            costmapp = Map_Costmap_Movement_Compute(in_game->map_ptr, in_game->world, in_game->selected_unit_entity);
            movemapp = Pathfinding_Map_Move(costmapp, in_game->map_ptr->row_len, in_game->map_ptr->col_len, start, unit_move, POINTS_MATRIX);
            attackmapp = Pathfinding_Map_Attack(movemapp, in_game->map_ptr->row_len, in_game->map_ptr->col_len, unit_move, range, POINTS_MATRIX, MOVETILE_EXCLUDE);
            dangermapp = matrix_plus_int16_t(attackmapp, movemapp, in_game->map_ptr->row_len, in_game->map_ptr->col_len, 1);


            if (selected_unit_ptr != NULL) {
                if (!in_game->map_ptr->show_danger) {
                    in_game->map_ptr->overlay_danger = dangermapp;
                    in_game->map_ptr->show_danger = true;
                } else {
                    Map_Danger_Add(in_game->map_ptr, dangermapp);
                    Map_Danger_Sub(in_game->map_ptr, dangermapp);
                }

                in_game->map_ptr->show_danger = true;
            } else {
                Map_Danger_Sub(in_game->map_ptr, dangermapp);
                in_game->map_ptr->show_danger = false;
            }
        } else {
            SDL_Log("Could not get selected unit component or selector component");
        }
    } else {
        SDL_Log("Could not get selected unit entity or selector entity");
    }
}

void receive_Unit_Dance(struct Game * in_game, SDL_Event * in_userevent) {
    SDL_Log("receive_Unit_Dance");
}

void receive_Menu_Select(struct Game * in_game, SDL_Event * in_userevent) {
    SDL_Log("receive_Menu_Select");
    tnecs_entity_t selector_entity = *(tnecs_entity_t *)in_userevent->user.data1;

    tnecs_entity_t menu_entity_top = 0;
    struct Position * selector_position_ptr;
    struct MenuOption menuOption_selected;
    struct Menu * current_menu_ptr;

    if (in_game->menu_stack_num > 0) {
        menu_entity_top = in_game->menu_stack[in_game->menu_stack_num - 1];
        current_menu_ptr = TNECS_GET_COMPONENT(in_game->world, menu_entity_top, Menu);
        selector_position_ptr = TNECS_GET_COMPONENT(in_game->world, selector_entity, Position);
        menuOption_selected = current_menu_ptr->menuoptions[selector_position_ptr->tilemap_pos.y + current_menu_ptr->row_num * selector_position_ptr->tilemap_pos.x];
        if (menuOption_selected.childEvent > 0) {
            Event_Emit(SDL_USEREVENT, menuOption_selected.childEvent, menuOption_selected.data1_childEvent, menuOption_selected.data1_childEvent);
        }
        if (menuOption_selected.childMenu > 0) {
            if (in_game->menu_loaded[menuOption_selected.childMenu] == 0) {
                Game_Menu_Create(in_game, menuOption_selected.childMenu);
            }
            Game_menuStack_Push(in_game, in_game->menu_loaded[menuOption_selected.childMenu]);
            Game_Menu_Update(in_game, menuOption_selected.childMenu);
            Game_cursorFocus_onMenu(in_game);
            struct Menu * temp_menu_ptr = TNECS_GET_COMPONENT(in_game->world, in_game->menu_loaded[menuOption_selected.childMenu], Menu);
            SDL_Log("Menu_select: temp_menu_ptr->id %d", temp_menu_ptr->id);
            SDL_Log("Menu_select: temp_menu_ptr->enabled %d", temp_menu_ptr->enabled);
        }
    }

}

void receive_Input_menuRight(struct Game * in_game, SDL_Event * in_userevent) {
    SDL_Log("receive_Input_menuRight");

}

void receive_Input_menuLeft(struct Game * in_game, SDL_Event * in_userevent) {
    SDL_Log("receive_Input_menuLeft");

}

void receive_Input_Minimap(struct Game * in_game, SDL_Event * in_userevent) {
    SDL_Log("receive_Input_Minimap");

}

void receive_Input_Faster(struct Game * in_game, SDL_Event * in_userevent) {
    SDL_Log("receive_Input_Minimap");

}

void receive_Input_Pause(struct Game * in_game, SDL_Event * in_userevent) {
    SDL_Log("receive_Input_Pause");

}

void receive_Unit_Seize(struct Game * in_game, SDL_Event * in_userevent) {
    SDL_Log("receive_Input_Seize");

}

void receive_Game_Defeat(struct Game * in_game, SDL_Event * in_userevent) {
    SDL_Log("receive_Input_Pause");

}

void receive_Unit_Refresh(struct Game * in_game, SDL_Event * in_userevent) {
    SDL_Log("receive_Unit_Refresh");

}

void receive_Unit_Wait(struct Game * in_game, SDL_Event * in_userevent) {
    SDL_Log("receive_Input_Pause");

    tnecs_entity_t * selector_entity = (tnecs_entity_t *) in_userevent->user.data1;
    if (in_game->selected_unit_entity > 0) {
        struct Unit * waiting_unit_ptr = TNECS_GET_COMPONENT(in_game->world, in_game->selected_unit_entity, Unit);
        struct Sprite * waiting_sprite_ptr = TNECS_GET_COMPONENT(in_game->world, in_game->selected_unit_entity, Sprite);
        if (waiting_unit_ptr != NULL) {
            waiting_unit_ptr->waits = true;
        }
        if (waiting_sprite_ptr) {
            Sprite_darken(waiting_sprite_ptr);
        }
    }

    bool destroy = false;
    while (in_game->menu_stack_num > 0) {
        Game_Menu_Disable_Entity(in_game, in_game->menu_stack[in_game->menu_stack_num - 1]);
        Game_menuStack_Pop(in_game, destroy);
    }

    Event_Emit(SDL_USEREVENT, event_Unit_Deselect, selector_entity, NULL);
    Game_cursorFocus_onMap(in_game);
}

void receive_Unit_Talk(struct Game * in_game, SDL_Event * in_userevent) {
    SDL_Log("receive_Unit_Talk");
    tnecs_entity_t * selector_entity = (tnecs_entity_t *) in_userevent->user.data1;

    if (in_game->selected_unit_entity > 0) {
        struct Unit * unit_ptr = TNECS_GET_COMPONENT(in_game->world, in_game->selected_unit_entity, Unit);
        if (unit_ptr != NULL) {
            Unit_wait(unit_ptr);
        }
    }
    Event_Emit(SDL_USEREVENT, event_Gameplay_Return2Standby, selector_entity, NULL);
}

void receive_Unit_Rescue(struct Game * in_game, SDL_Event * in_userevent) {
    SDL_Log("receive_Unit_Rescue");
    tnecs_entity_t * selector_entity = (tnecs_entity_t *) in_userevent->user.data1;

    if (in_game->selected_unit_entity > 0) {
        struct Unit * unit_ptr = TNECS_GET_COMPONENT(in_game->world, in_game->selected_unit_entity, Unit);
        if (unit_ptr != NULL) {
            Unit_wait(unit_ptr);
        }
    }
    Event_Emit(SDL_USEREVENT, event_Gameplay_Return2Standby, selector_entity, NULL);
}

void receive_Defender_Select(struct Game * in_game, SDL_Event * in_userevent) {
    SDL_Log("receive_Defender_Select");
    tnecs_entity_t * selector_entity = (tnecs_entity_t *) in_userevent->user.data1;
    if (in_game->selected_unit_entity > 0) {
        struct Unit * unit_ptr = TNECS_GET_COMPONENT(in_game->world, in_game->selected_unit_entity, Unit);
        if (unit_ptr != NULL) {
            Unit_wait(unit_ptr);
        }
    }
    Event_Emit(SDL_USEREVENT, event_Gameplay_Return2Standby, selector_entity, NULL);
}

void receive_Unit_Attack(struct Game * in_game, SDL_Event * in_userevent) {
    SDL_Log("receive_Unit_Attack");
    tnecs_entity_t * selector_entity = (tnecs_entity_t *) in_userevent->user.data1;
    tnecs_entity_t * unit_entity_attacker = (tnecs_entity_t *) in_userevent->user.data2;

    if (*unit_entity_attacker > 0) {
        struct Unit * attacker_unit_ptr = TNECS_GET_COMPONENT(in_game->world, *unit_entity_attacker, Unit);
        if (attacker_unit_ptr != NULL) {
            Unit_computedStats(attacker_unit_ptr);
        }
    }
    // Option for order? defender->weapon OR weapon->defender?
}

void receive_Unit_Trade(struct Game * in_game, SDL_Event * in_userevent) {
    SDL_Log("receive_Unit_Trade");
    tnecs_entity_t * selector_entity = (tnecs_entity_t *) in_userevent->user.data1;

    if (in_game->selected_unit_entity > 0) {
        struct Unit * unit_ptr = TNECS_GET_COMPONENT(in_game->world, in_game->selected_unit_entity, Unit);
        if (unit_ptr != NULL) {
            Unit_wait(unit_ptr);
        }
    }
    Event_Emit(SDL_USEREVENT, event_Gameplay_Return2Standby, selector_entity, NULL);
}
void receive_Unit_Escape(struct Game * in_game, SDL_Event * in_userevent) {
    SDL_Log("receive_Unit_Escape");
    tnecs_entity_t * selector_entity = (tnecs_entity_t *) in_userevent->user.data1;

    if (in_game->selected_unit_entity > 0) {
        struct Unit * unit_ptr = TNECS_GET_COMPONENT(in_game->world, in_game->selected_unit_entity, Unit);
        if (unit_ptr != NULL) {
            Unit_wait(unit_ptr);
        }
    }
    Event_Emit(SDL_USEREVENT, event_Gameplay_Return2Standby, selector_entity, NULL);
}

void receive_Unit_Staff(struct Game * in_game, SDL_Event * in_userevent) {
    SDL_Log("receive_Unit_Staff");

}

void receive_Unit_Items(struct Game * in_game, SDL_Event * in_userevent) {
    SDL_Log("receive_Unit_Items");
    tnecs_entity_t * selector_entity = (tnecs_entity_t *) in_userevent->user.data1;

    if (in_game->selected_unit_entity > 0) {
        struct Unit * unit_ptr = TNECS_GET_COMPONENT(in_game->world, in_game->selected_unit_entity, Unit);
        if (unit_ptr != NULL) {
            Unit_wait(unit_ptr);
        }
    }
    Event_Emit(SDL_USEREVENT, event_Gameplay_Return2Standby, selector_entity, NULL);
}

void receive_Unit_Heals(struct Game * in_game, SDL_Event * in_userevent) {
    SDL_Log("receive_Unit_Heals");

}

void receive_Convoy_Check(struct Game * in_game, SDL_Event * in_userevent) {
    SDL_Log("receive_Convoy_Check");

}

void receive_Convoy_Map(struct Game * in_game, SDL_Event * in_userevent) {
    SDL_Log("receive_Convoy_Map");

}

void receive_Unit_Dies(struct Game * in_game, SDL_Event * in_userevent) {
    SDL_Log("receive_Unit_Dies");

}

void receive_Unit_Agonizes(struct Game * in_game, SDL_Event * in_userevent) {
    SDL_Log("receive_Unit_Agonizes");

}

void receive_SDL_WINDOWEVENT(struct Game * in_game, SDL_Event * in_event) {
    if (in_event->window.windowID == SDL_GetWindowID(in_game->window)) {
        switch (in_event->window.event) {
            case SDL_WINDOWEVENT_CLOSE:
                SDL_Log("receive_SDL_WINDOWEVENT_CLOSE");
                Event_Emit(SDL_QUIT, 0, NULL, NULL);

                break;
        }
    }
}


void Events_Names_Declare() {
#define REGISTER_ENUM(x) event_##x = SDL_RegisterEvents(1);
#include "names/events.h"
}
#undef REGISTER_ENUM

void Events_Receivers_Declare_Arr() {
    receivers_arr = DARR_INIT(receiver_t, 100);
}

void Events_Receivers_Declare() {
    uint32_t temp_index;
#define REGISTER_ENUM(x) hmput(receivers, event_##x, &receive_##x);
#include "names/events.h"
#undef REGISTER_ENUM

#define REGISTER_ENUM(x) temp_index = x;\
hmput(receivers, temp_index, &receive_##x);
#include "names/SDL_events.h"
#undef REGISTER_ENUM
    temp_index = SDL_MOUSEBUTTONDOWN;
    hmput(receivers, temp_index, &receive_SDL_MOUSEBUTTON);
    temp_index = SDL_MOUSEBUTTONUP;
    hmput(receivers, temp_index, &receive_SDL_MOUSEBUTTON);
    temp_index = SDL_WINDOWEVENT;
    hmput(receivers, temp_index, &receive_SDL_WINDOWEVENT);
    temp_index = SDL_QUIT;
    hmput(receivers, temp_index, &receive_SDL_QUIT);
    // hmdefault(receivers, NULL);
}

void Events_Manage(struct Game * in_game) {
    // SDL_Log("Events_Manage");
    SDL_Event current_event;
    uint32_t receiver_key;
    void * receiver;
    while (SDL_PollEvent(&current_event)) {
        // Think about event ordering
        //-> Inputs first? System things first?
        receiver_key = 0;
        if (current_event.type == SDL_USEREVENT) {
            receiver_key = current_event.user.code;
        } else {
            receiver_key = current_event.type;
        }
        void (*receiver)(struct Game *, SDL_Event *) = hmget(receivers, receiver_key);

        if (*receiver != NULL) {
            receiver(in_game, &current_event);
        }
    }
}
