#include "events.h"

/* --- RECEIVERS DECLARATION --- */
struct dtab *receivers_dtab = NULL;

u32 event_Start;
#define REGISTER_ENUM(x, y) u32 event_##x;
#include "names/events.h"
#undef REGISTER_ENUM
#define REGISTER_ENUM(x, y) u32 event_Input_##x;
#include "names/input.h"
#undef REGISTER_ENUM
u32 event_End;

/* --- DATA ENTITIES DEFINITION --- */
tnecs_entity_t *data1_entity;
tnecs_entity_t *data2_entity;

void Events_Data_Malloc() {
    SOTA_Log_Func("%d\t%s\t" STRINGIZE(__LINE__), call_stack_depth++, __func__);
    data1_entity = SDL_malloc(sizeof(tnecs_entity_t));
    data2_entity = SDL_malloc(sizeof(tnecs_entity_t));
    SOTA_Log_Func("%d\t%s\t" STRINGIZE(__LINE__), --call_stack_depth, __func__);
}

void Events_Data_Free() {
    SOTA_Log_Func("%d\t%s\t" STRINGIZE(__LINE__), call_stack_depth++, __func__);
    free(data1_entity);
    free(data2_entity);
    SOTA_Log_Func("%d\t%s\t" STRINGIZE(__LINE__), --call_stack_depth, __func__);
}

tnecs_entity_t Events_Controllers_Check(struct Game *sota, i32 code) {
    SOTA_Log_Func("%d\t%s\t" STRINGIZE(__LINE__), call_stack_depth++, __func__);
    tnecs_entity_t out_accepter_entity;
    struct controllerGamepad  *gamepad_ptr;
    struct controllerKeyboard *keyboard_ptr;
    switch (code) {
        case CONTROLLER_MOUSE:
            SOTA_Log_Debug("CONTROLLER_MOUSE");
            if (!sota->ismouse) {
                Event_Emit(__func__, SDL_USEREVENT, event_Mouse_Enable, NULL, NULL);
                // Event_Emit(__func__, SDL_USEREVENT, event_Cursor_Disable, NULL, NULL);
            }
            out_accepter_entity = sota->entity_mouse;
            break;
        case CONTROLLER_GAMEPAD:
            SOTA_Log_Debug("CONTROLLER_GAMEPAD");
            if (!sota->iscursor) {
                Event_Emit(__func__, SDL_USEREVENT, event_Mouse_Disable, NULL, NULL);
                Event_Emit(__func__, SDL_USEREVENT, event_Cursor_Enable, NULL, NULL);
            }
            out_accepter_entity = sota->entity_cursor;
            gamepad_ptr  = TNECS_GET_COMPONENT(sota->world, sota->entity_cursor, controllerGamepad);
            keyboard_ptr = TNECS_GET_COMPONENT(sota->world, sota->entity_cursor, controllerKeyboard);
            gamepad_ptr->block_buttons        = true;
            keyboard_ptr->block_buttons       = true;
            gamepad_ptr->timeheld_button_ns   = SOTA_ns / sota->settings.FPS.cap;
            keyboard_ptr->timeheld_button_ns  = SOTA_ns / sota->settings.FPS.cap;
            break;
        case CONTROLLER_KEYBOARD:
            SOTA_Log_Debug("CONTROLLER_KEYBOARD");
            if (!sota->iscursor) {
                Event_Emit(__func__, SDL_USEREVENT, event_Mouse_Disable, NULL, NULL);
                Event_Emit(__func__, SDL_USEREVENT, event_Cursor_Enable, NULL, NULL);
            }
            out_accepter_entity = sota->entity_cursor;
            gamepad_ptr  = TNECS_GET_COMPONENT(sota->world, sota->entity_cursor, controllerGamepad);
            keyboard_ptr = TNECS_GET_COMPONENT(sota->world, sota->entity_cursor, controllerKeyboard);
            gamepad_ptr->block_buttons        = true;
            keyboard_ptr->block_buttons       = true;
            gamepad_ptr->timeheld_button_ns   = SOTA_ns / sota->settings.FPS.cap;
            keyboard_ptr->timeheld_button_ns  = SOTA_ns / sota->settings.FPS.cap;
            break;
        case CONTROLLER_TOUCHPAD:
            SOTA_Log_Debug("CONTROLLER_TOUCHPAD");
            if (!sota->iscursor) {
                Event_Emit(__func__, SDL_USEREVENT, event_Mouse_Disable, NULL, NULL);
                Event_Emit(__func__, SDL_USEREVENT, event_Cursor_Enable, NULL, NULL);
            }
            out_accepter_entity = sota->entity_cursor;
            gamepad_ptr  = TNECS_GET_COMPONENT(sota->world, sota->entity_cursor, controllerGamepad);
            keyboard_ptr = TNECS_GET_COMPONENT(sota->world, sota->entity_cursor, controllerKeyboard);
            gamepad_ptr->block_buttons        = true;
            keyboard_ptr->block_buttons       = true;
            gamepad_ptr->timeheld_button_ns   = SOTA_ns / sota->settings.FPS.cap;
            keyboard_ptr->timeheld_button_ns  = SOTA_ns / sota->settings.FPS.cap;
            break;
        default:
            printf("controller code is invalid.");
            exit;
    }
    SOTA_Log_Func("%d\t%s\t" STRINGIZE(__LINE__), --call_stack_depth, __func__);
    return (out_accepter_entity);
}

/* --- EVENT RECEIVERS --- */
void Event_Emit(const char *emitter, u32 type, i32 code, void *data1, void *data2) {
    SOTA_Log_Func("%d\t%s\t" STRINGIZE(__LINE__), call_stack_depth++, __func__);
    SDL_assert(code > 0);
    char *event_name = event_names[code - event_Start];
    SOTA_Log_Debug("emitter -> %s, event -> %s", emitter, event_name);
    SDL_assert(type != ((UINT32_MAX) - 1));
    SDL_Event event;
    event.type          = type;
    event.user.code     = code;
    event.user.data1    = data1;
    event.user.data2    = data2;
    SDL_PushEvent(&event);
    SOTA_Log_Func("%d\t%s\t" STRINGIZE(__LINE__), --call_stack_depth, __func__);
}

void receive_event_Start(struct Game *sota, SDL_Event *userevent) {
    SOTA_Log_Func("%d\t%s\t" STRINGIZE(__LINE__), call_stack_depth++, __func__);
    SOTA_Log_Func("%d\t%s\t" STRINGIZE(__LINE__), --call_stack_depth, __func__);
}

void receive_event_End(struct Game *sota, SDL_Event *userevent) {
    SOTA_Log_Func("%d\t%s\t" STRINGIZE(__LINE__), call_stack_depth++, __func__);
    sota->isrunning = false;
    SOTA_Log_Func("%d\t%s\t" STRINGIZE(__LINE__), --call_stack_depth, __func__);
}

void receive_event_Load_Debug_Map(struct Game *sota, SDL_Event *userevent) {
    SOTA_Log_Func("%d\t%s\t" STRINGIZE(__LINE__), call_stack_depth++, __func__);
    /* -- UNLOAD FirstMenu -- */
    Game_FirstMenu_Destroy(sota);
    Game_Title_Destroy(sota);
    /* -- LOAD Debug map -- */
    Game_debugMap_Load(sota);
    Utilities_DrawColor_Reset(sota->renderer);
    SOTA_Log_Func("%d\t%s\t" STRINGIZE(__LINE__), --call_stack_depth, __func__);
}

void receive_event_Cursor_Moves(struct Game *sota, SDL_Event *userevent) {
    SOTA_Log_Func("%d\t%s\t" STRINGIZE(__LINE__), call_stack_depth++, __func__);
    i32 controller_type = *(i32 *) userevent->user.data2;
    tnecs_entity_t mover_entity = Events_Controllers_Check(sota, controller_type);
    SDL_assert(mover_entity > 0);
    SDL_assert(userevent->user.data1 != NULL);
    // struct Point * cursor_move = (struct Point *)
    // userevent->user.data1;
    // Get movement direction
    sota->moved_direction = Ternary_Direction(sota->cursor_move);
    if (sota->moved_direction < 0)
        return;

    // if moving more than one tile, ignore movement.
    if (fsm_eCrsMvs_ss[sota->substate] != NULL)
        fsm_eCrsMvs_ss[sota->substate](sota, mover_entity, &sota->cursor_move);

    Event_Emit(__func__, SDL_USEREVENT, event_Cursor_Moved, &sota->cursor_move,
               userevent->user.data2);

    SOTA_Log_Func("%d\t%s\t" STRINGIZE(__LINE__), --call_stack_depth, __func__);
}

void receive_event_Cursor_Moved(struct Game *sota, SDL_Event *userevent) {
    SOTA_Log_Func("%d\t%s\t" STRINGIZE(__LINE__), call_stack_depth++, __func__);
    SDL_assert(userevent->user.data2 != NULL);
    i32 controller_type = * (i32 *) userevent->user.data2;
    tnecs_entity_t mover_entity = Events_Controllers_Check(sota, controller_type);
    SDL_assert(mover_entity > 0);
    SDL_assert(userevent->user.data1 != NULL);
    struct Point *cursor_move = userevent->user.data1;

    SDL_assert(sota->entity_cursor != TNECS_NULL);
    struct Position *cursor_pos;
    cursor_pos = TNECS_GET_COMPONENT(sota->world, sota->entity_cursor, Position);
    struct Point pos = cursor_pos->tilemap_pos;

    if (fsm_eCrsMvd_ss[sota->substate] != NULL)
        fsm_eCrsMvd_ss[sota->substate](sota, mover_entity, &pos);

    if (fsm_eCrsMvd_s[sota->state] != NULL)
        fsm_eCrsMvd_s[sota->state](sota, mover_entity, cursor_move);

    // cursor_move->x = 0;
    // cursor_move->y = 0;
    SOTA_Log_Func("%d\t%s\t" STRINGIZE(__LINE__), --call_stack_depth, __func__);
}

void receive_event_Input_CANCEL(struct Game *sota, SDL_Event *userevent) {
    SOTA_Log_Func("%d\t%s\t" STRINGIZE(__LINE__), call_stack_depth++, __func__);
    i32 controller_type = * (i32 *) userevent->user.data1;
    tnecs_entity_t canceller_entity = Events_Controllers_Check(sota, controller_type);
    SDL_assert(canceller_entity > 0);
    if (fsm_eCncl_s[sota->state] != NULL)
        fsm_eCncl_s[sota->state](sota, canceller_entity);

    SOTA_Log_Func("%d\t%s\t" STRINGIZE(__LINE__), --call_stack_depth, __func__);
}

void receive_event_Map_Win(struct Game *sota, SDL_Event *Map_Win) {
    SOTA_Log_Func("%d\t%s\t" STRINGIZE(__LINE__), call_stack_depth++, __func__);
    SOTA_Log_Func("%d\t%s\t" STRINGIZE(__LINE__), --call_stack_depth, __func__);
}

void receive_event_Map_Lose(struct Game *sota, SDL_Event *Map_Lose) {
    SOTA_Log_Func("%d\t%s\t" STRINGIZE(__LINE__), call_stack_depth++, __func__);
    SOTA_Log_Func("%d\t%s\t" STRINGIZE(__LINE__), --call_stack_depth, __func__);
}

void receive_event_Item_Get(struct Game *sota, SDL_Event *Map_Lose) {
    SOTA_Log_Func("%d\t%s\t" STRINGIZE(__LINE__), call_stack_depth++, __func__);
    SOTA_Log_Func("%d\t%s\t" STRINGIZE(__LINE__), --call_stack_depth, __func__);
}

void receive_event_Mouse_Disable(struct Game *sota, SDL_Event *Mouse_Disable) {
    SOTA_Log_Func("%d\t%s\t" STRINGIZE(__LINE__), call_stack_depth++, __func__);
    Game_Mouse_Disable(sota);
    SOTA_Log_Func("%d\t%s\t" STRINGIZE(__LINE__), --call_stack_depth, __func__);
}

void receive_event_Mouse_Enable(struct Game *sota, SDL_Event *Mouse_Enable) {
    SOTA_Log_Func("%d\t%s\t" STRINGIZE(__LINE__), call_stack_depth++, __func__);
    Game_Mouse_Enable(sota);
    SOTA_Log_Func("%d\t%s\t" STRINGIZE(__LINE__), --call_stack_depth, __func__);
}

void receive_event_Cursor_Enable(struct Game *sota, SDL_Event *Cursor_Enable) {
    SOTA_Log_Func("%d\t%s\t" STRINGIZE(__LINE__), call_stack_depth++, __func__);
    Game_Cursor_Enable(sota);
    SOTA_Log_Func("%d\t%s\t" STRINGIZE(__LINE__), --call_stack_depth, __func__);
}

void receive_event_Cursor_Disable(struct Game *sota, SDL_Event *Cursor_Disable) {
    SOTA_Log_Func("%d\t%s\t" STRINGIZE(__LINE__), call_stack_depth++, __func__);
    Game_Cursor_Disable(sota);
    SOTA_Log_Func("%d\t%s\t" STRINGIZE(__LINE__), --call_stack_depth, __func__);
}

void receive_event_Turn_Begin(struct Game *sota, SDL_Event *userevent) {
    SOTA_Log_Func("%d\t%s\t" STRINGIZE(__LINE__), call_stack_depth++, __func__);
    // u8 army = * (u8 *) userevent->user.data1;
    struct Map *map = sota->map;
    SDL_assert(sota->state == GAME_STATE_Gameplay_Map);

    /* - Refresh all units - */
    for (int i = 0; i < map->num_units_onfield; i++) {
        if (map->units_onfield[i] != TNECS_NULL) {
            Game_Unit_Refresh(sota, map->units_onfield[i]);
        }
    }

    // TODO:
    // - Perform turn begin animation
    Map_Turn_Increment(sota->map);

    Event_Emit(__func__, SDL_USEREVENT, event_Gameplay_Return2Standby, NULL, NULL);

    SOTA_Log_Func("%d\t%s\t" STRINGIZE(__LINE__), --call_stack_depth, __func__);
}

void receive_event_Game_Control_Switch(struct Game *sota, SDL_Event *userevent) {
    SOTA_Log_Func("%d\t%s\t" STRINGIZE(__LINE__), call_stack_depth++, __func__);
    u8 army = * (u8 *) userevent->user.data1;
    if (SotA_isPC(army)) {
        // Game_State_Set(sota, GAME_STATE_Gameplay_Map);
        if (sota->entity_cursor > TNECS_NULL)
            Game_Cursor_Create(sota);
        Game_Cursor_Enable(sota);
        // Game_setCursorstate(sota, MENU_MAP);
        // Game_Mouse_State_Set(sota, MENU_MAP);
    } else {
        strncpy(sota->reason, "control was switched to a non-player army", sizeof(sota->reason));
        Game_subState_Set(sota, GAME_SUBSTATE_MAP_NPCTURN, sota->reason);
        Game_Cursor_Free(sota);
    }
    SOTA_Log_Func("%d\t%s\t" STRINGIZE(__LINE__), --call_stack_depth, __func__);
}

void receive_event_Input_STATS(struct Game *sota, SDL_Event *userevent) {
    SOTA_Log_Func("%d\t%s\t" STRINGIZE(__LINE__), call_stack_depth++, __func__);
    SDL_assert(sota != NULL);
    i32 controller_type = * (i32 *) userevent->user.data1;
    tnecs_entity_t accepter_entity = Events_Controllers_Check(sota, controller_type);
    SDL_assert(accepter_entity > 0);
    *data1_entity = accepter_entity;
    SOTA_Log_Debug("sota->state %d", sota->state);
    // if (fsm_Input_Stats_state[sota->state] != NULL) {
    //     fsm_Input_Stats_state[sota->state](sota, accepter_entity);
    // }
    if (fsm_eStats_ss[sota->substate] != NULL)
        fsm_eStats_ss[sota->substate](sota, accepter_entity);
    SOTA_Log_Func("%d\t%s\t" STRINGIZE(__LINE__), --call_stack_depth, __func__);
}

void receive_event_Gameplay_Return2Standby(struct Game *sota, SDL_Event *userevent) {
    SOTA_Log_Func("%d\t%s\t" STRINGIZE(__LINE__), call_stack_depth++, __func__);

    /* -- Popping all menus -- */
    bool destroy = false;
    while (DARR_NUM(sota->menu_stack) > 0)
        Game_menuStack_Pop(sota, destroy);
    SDL_assert(DARR_NUM(sota->menu_stack) == 0);

    // BUG WITH MAP HERE
    Game_cursorFocus_onMap(sota);

    /* --- Visible Popups --- */
    tnecs_entity_t popup_ent;
    struct PopUp *popup_ptr;
    /* -- Popup_Tile -- */
    popup_ent = sota->popups[POPUP_TYPE_HUD_TILE];
    popup_ptr = TNECS_GET_COMPONENT(sota->world, popup_ent, PopUp);
    SDL_assert(popup_ptr != NULL);
    popup_ptr->visible = true;

    /* -- Make Popup_Unit visible -- */
    popup_ent = sota->popups[POPUP_TYPE_HUD_UNIT];
    popup_ptr = TNECS_GET_COMPONENT(sota->world, popup_ent, PopUp);
    SDL_assert(popup_ptr != NULL);
    popup_ptr->visible = true;

    /* --- Invisible Popups --- */
    /* -- Popup_Loadout_stats -- */
    popup_ent = sota->popups[POPUP_TYPE_HUD_LOADOUT_STATS];
    if (popup_ent > TNECS_NULL) {
        popup_ptr = TNECS_GET_COMPONENT(sota->world, popup_ent, PopUp);
        SDL_assert(popup_ptr != NULL);
        popup_ptr->visible = false;
    }

    /* -- FSM -- */
    if (fsm_eGmp2Stby_ss[sota->substate] != NULL)
        fsm_eGmp2Stby_ss[sota->substate](sota, TNECS_NULL);
    if (fsm_eGmp2Stby_s[sota->state] != NULL)
        fsm_eGmp2Stby_s[sota->state](sota, TNECS_NULL);

    /* -- Setting game substate -- */
    if (sota->substate != GAME_SUBSTATE_STANDBY) {
        strncpy(sota->reason, "sota->substate returns to Standby",
                sizeof(sota->reason));
        Game_subState_Set(sota, GAME_SUBSTATE_STANDBY, sota->reason);
    }

    SOTA_Log_Func("%d\t%s\t" STRINGIZE(__LINE__), --call_stack_depth, __func__);
}

void receive_event_Input_GLOBALRANGE(struct Game *sota, SDL_Event *userevent) {
    SOTA_Log_Func("%d\t%s\t" STRINGIZE(__LINE__), call_stack_depth++, __func__);
    i32 controller_type = *(i32 *)userevent->user.data1;
    Events_Controllers_Check(sota, controller_type);

    if (fsm_eGlbRng_ss[sota->substate] != NULL)
        fsm_eGlbRng_ss[sota->substate](sota);

    SOTA_Log_Func("%d\t%s\t" STRINGIZE(__LINE__), --call_stack_depth, __func__);
}

void receive_event_Input_ACCEPT(struct Game *sota, SDL_Event *userevent) {
    SOTA_Log_Func("%d\t%s\t" STRINGIZE(__LINE__), call_stack_depth++, __func__);

    SDL_assert(sota);
    i32 controller_type = * (i32 *) userevent->user.data1;
    tnecs_entity_t accepter_entity = Events_Controllers_Check(sota, controller_type);
    SDL_assert(accepter_entity > 0);
    *data1_entity = accepter_entity;

    if (fsm_eAcpt_s[sota->state] != NULL)
        fsm_eAcpt_s[sota->state](sota, accepter_entity);
    SOTA_Log_Func("%d\t%s\t" STRINGIZE(__LINE__), --call_stack_depth, __func__);
}

void receive_event_SDL_AUDIODEVICEADDED(struct Game *sota, SDL_Event *event) {
    SOTA_Log_Func("%d\t%s\t" STRINGIZE(__LINE__), call_stack_depth++, __func__);
    SOTA_Log_Func("%d\t%s\t" STRINGIZE(__LINE__), --call_stack_depth, __func__);
}

void receive_event_SDL_AUDIODEVICEREMOVED(struct Game *sota, SDL_Event *event) {
    SOTA_Log_Func("%d\t%s\t" STRINGIZE(__LINE__), call_stack_depth++, __func__);
    SOTA_Log_Func("%d\t%s\t" STRINGIZE(__LINE__), --call_stack_depth, __func__);
}

// SDL_CONTROLLERBUTTONDOWN emitted ONCE for the click
void receive_event_SDL_CONTROLLERBUTTONDOWN(struct Game *sota, SDL_Event *event) {
    SOTA_Log_Func("%d\t%s\t" STRINGIZE(__LINE__), call_stack_depth++, __func__);
    SOTA_Log_Func("%d\t%s\t" STRINGIZE(__LINE__), --call_stack_depth, __func__);
}

// SDL_KEYDOWN emitted unreliably/not every frame
void receive_event_SDL_KEYDOWN(struct Game *sota, SDL_Event *event) {
    SOTA_Log_Func("%d\t%s\t" STRINGIZE(__LINE__), call_stack_depth++, __func__);
    // why is SDL_KEYDOWN unused?
    //  -> SDL_KEYDOWN is NOT emitted every frame
    // control system needs to run EVERY STEP
    SOTA_Log_Func("%d\t%s\t" STRINGIZE(__LINE__), --call_stack_depth, __func__);
}

void receive_event_Quit(struct Game *sota, SDL_Event *event) {
    SOTA_Log_Func("%d\t%s\t" STRINGIZE(__LINE__), call_stack_depth++, __func__);
    // Quit gameplay, go back to start menu
    strncpy(sota->reason, "Quitting gameplay", sizeof(sota->reason));
    Game_State_Set(sota, GAME_STATE_Title_Screen, sota->reason);
    if (sota->substate != GAME_SUBSTATE_MENU)
        Game_subState_Set(sota, GAME_SUBSTATE_MENU, sota->reason);

    /* -- Removing menus -- */
    bool destroy = false;
    while (DARR_NUM(sota->menu_stack) > 0) {
        Game_menuStack_Pop(sota, destroy);
        /* -- Hiding menus -- */
    }
    /* -- Hiding popups -- */
    Game_PopUp_Tile_Hide(sota);

    /* -- UnloadMap -- */
    Game_Map_Unload(sota);
    /* -- Load TitleScreen -- */
    struct Input_Arguments args = Input_Arguments_default;
    Game_titleScreen_Load(sota, args);
    Game_cursorFocus_onMenu(sota);
    SOTA_Log_Func("%d\t%s\t" STRINGIZE(__LINE__), --call_stack_depth, __func__);
}

void Entities_Reload(struct Game *sota, size_t flag_id, const char *component) {
    SOTA_Log_Func("%d\t%s\t" STRINGIZE(__LINE__), call_stack_depth++, __func__);
    size_t num_entities = sota->world->num_entities_bytype[flag_id];
    for (size_t i = 0; i < num_entities; i++) {
        tnecs_entity_t entity = sota->world->entities_bytype[flag_id][i];
        size_t component_id = tnecs_component_name2id(sota->world, component);
        void *struct_ptr    = tnecs_entity_get_component(sota->world, entity, component_id);
        const char *filename = PATH_JOIN("");
        jsonio_readJSON(filename, struct_ptr);
    }

    SOTA_Log_Func("%d\t%s\t" STRINGIZE(__LINE__), --call_stack_depth, __func__);
}

void receive_event_Reload(struct Game *sota, SDL_Event *event) {
    SOTA_Log_Func("%d\t%s\t" STRINGIZE(__LINE__), call_stack_depth++, __func__);
    /* --- Reload all entities --- */
    tnecs_component_t component_flag;
    size_t num_archetypes, component_flag_id;
    /* -- Reload Units -- */
    /* -- Reload entities for the pure component typeflag -- */
    component_flag     = tnecs_component_names2typeflag(sota->world, 1, "Unit");
    component_flag_id  = tnecs_typeflagid(sota->world, component_flag);
    Entities_Reload(sota, component_flag_id, "Unit");

    /* -- Reload entities for the all component archetypes -- */
    num_archetypes = sota->world->num_archetype_ids[component_flag_id];
    for (size_t tsub = 0; tsub < num_archetypes; tsub++) {
        size_t archetype_id = sota->world->archetype_id_bytype[component_flag_id][tsub];
        Entities_Reload(sota, archetype_id, "Unit");
    }

    /* -- Reload Sprites -- */
    /* -- Reload entities for the pure component typeflag -- */
    component_flag = tnecs_component_names2typeflag(sota->world, 1, "Sprite");
    component_flag_id  = tnecs_typeflagid(sota->world, component_flag);
    Entities_Reload(sota, component_flag_id, "Sprite");

    /* -- Reload entities for the all component archetypes -- */
    num_archetypes = sota->world->num_archetype_ids[component_flag_id];
    for (size_t tsub = 0; tsub < num_archetypes; tsub++) {
        size_t archetype_id = sota->world->archetype_id_bytype[component_flag_id][tsub];
        Entities_Reload(sota, archetype_id, "Sprite");
    }


    // /* --- Reload non-entities --- */
    // /* -- Reload Weapons -- */
    // /* -- Reload Items -- */

    // /* -- Reload Map -- */
    // /* - Reload Map tiles - */

    // /* -- Reload Scenes -- */

    // /* -- Reload Convoy -- */

    SOTA_Log_Func("%d\t%s\t" STRINGIZE(__LINE__), --call_stack_depth, __func__);
}

void receive_event_SDL_QUIT(struct Game *sota, SDL_Event *event) {
    SOTA_Log_Func("%d\t%s\t" STRINGIZE(__LINE__), call_stack_depth++, __func__);
    sota->isrunning = false;
    SOTA_Log_Func("%d\t%s\t" STRINGIZE(__LINE__), --call_stack_depth, __func__);
}

void receive_event_SDL_CONTROLLERDEVICEREMOVED(struct Game *sota, SDL_Event *event) {
    SOTA_Log_Func("%d\t%s\t" STRINGIZE(__LINE__), call_stack_depth++, __func__);
    // event->cdevice.which of DEVICEREMOVED is different from DEVICEADDED!
    // DEVICEREMOVED-> JoystickInstanceID which increments for every joystick

    SDL_assert(sota->entity_cursor != TNECS_NULL);

    struct controllerGamepad *gamepad_ptr;
    gamepad_ptr = TNECS_GET_COMPONENT(sota->world, sota->entity_cursor, controllerGamepad);
    if (gamepad_ptr != NULL)
        Gamepad_removeController(gamepad_ptr, event->cdevice.which);
    else
        SOTA_Log_Debug("entity_cursor has no controllerGamepad component");

    SOTA_Log_Func("%d\t%s\t" STRINGIZE(__LINE__), --call_stack_depth, __func__);
}

void receive_event_SDL_JOYDEVICEADDED(struct Game *sota, SDL_Event *event) {
}

void receive_event_SDL_JOYDEVICEREMOVED(struct Game *sota, SDL_Event *event) {
}

void receive_event_SDL_CONTROLLERDEVICEADDED(struct Game *sota, SDL_Event *event) {
    SOTA_Log_Func("%d\t%s\t" STRINGIZE(__LINE__), call_stack_depth++, __func__);
    // event->cdevice.which of DEVICEREMOVED is different from DEVICEADDED!
    // DEVICEADDED-> JoystickDeviceID which is stable e.g. 0 for player 1, etc.

    if (sota->entity_cursor == TNECS_NULL) {
        SOTA_Log_Debug("Entity_cursor is not valid");
        SOTA_Log_Func("%d\t%s\t" STRINGIZE(__LINE__), --call_stack_depth, __func__);
        return;
    }

    struct controllerGamepad *gamepad_ptr;
    gamepad_ptr = TNECS_GET_COMPONENT(sota->world, sota->entity_cursor, controllerGamepad);
    if (gamepad_ptr != NULL)
        Gamepad_addController(gamepad_ptr, event->cdevice.which);
    else
        SOTA_Log_Debug("entity_cursor has no controllerGamepad component");

    SOTA_Log_Func("%d\t%s\t" STRINGIZE(__LINE__), --call_stack_depth, __func__);
}

void receive_event_SDL_MOUSEMOTION(struct Game *sota, SDL_Event *event) {
    SOTA_Log_FPS("%d\t%s\t" STRINGIZE(__LINE__), call_stack_depth++, __func__);
    SDL_assert(event);
    if (sota->runtime_ns <= CURSOR_FIRSTMENU_PAUSE_ns) {
        SOTA_Log_FPS("Sota first init pause.");
        SOTA_Log_FPS("%d\t%s\t" STRINGIZE(__LINE__), --call_stack_depth, __func__);
        return;
    }
    if (event->motion.windowID != SDL_GetWindowID(sota->window)) {
        SOTA_Log_FPS("Wrong window");
        SOTA_Log_FPS("%d\t%s\t" STRINGIZE(__LINE__), --call_stack_depth, __func__);
        return;
    }
    tnecs_entity_t mouse = sota->entity_mouse;
    if (sota->entity_mouse == TNECS_NULL) {
        SOTA_Log_FPS("Mouse disabled");
        SOTA_Log_FPS("%d\t%s\t" STRINGIZE(__LINE__), --call_stack_depth, __func__);
        return;
    }

    struct Sprite   *msprite  = TNECS_GET_COMPONENT(sota->world, mouse, Sprite);
    struct Position *mpos     = TNECS_GET_COMPONENT(sota->world, mouse, Position);
    SDL_assert(msprite);
    SDL_assert(mpos);
    SDL_Point motion = {event->motion.x, event->motion.y};
    if ((motion.x != mpos->pixel_pos.x) || (motion.y != mpos->pixel_pos.y)) {
        Event_Emit(__func__, SDL_USEREVENT, event_Mouse_Enable, NULL, NULL);
        Event_Emit(__func__, SDL_USEREVENT, event_Cursor_Disable, NULL, NULL);
        mpos->pixel_pos.x   = motion.x;
        mpos->pixel_pos.y   = motion.y;
        msprite->dstrect.x  = mpos->pixel_pos.x;
        msprite->dstrect.y  = mpos->pixel_pos.y;
    }
    SOTA_Log_FPS("%d\t%s\t" STRINGIZE(__LINE__), --call_stack_depth, __func__);
}

// SDL_MOUSEBUTTON emitted ONCE for the CLICK
void receive_event_SDL_MOUSEBUTTON(struct Game *sota, SDL_Event *event) {
    SOTA_Log_Func("%d\t%s\t" STRINGIZE(__LINE__), call_stack_depth++, __func__);
    SDL_assert(sota->window != NULL);
    SDL_assert(event        != NULL);
    SDL_assert(sota->entity_mouse > 0);

    if (event->motion.windowID != SDL_GetWindowID(sota->window)) {
        SOTA_Log_FPS("Wrong window");
        SOTA_Log_FPS("%d\t%s\t" STRINGIZE(__LINE__), --call_stack_depth, __func__);
        return;
    }

    struct controllerMouse *mouse;
    if (event->button.state == SDL_PRESSED) {
        mouse = TNECS_GET_COMPONENT(sota->world, sota->entity_mouse, controllerMouse);
        Mouse_checkButton(mouse, event->button.button);
    }
    SOTA_Log_Func("%d\t%s\t" STRINGIZE(__LINE__), --call_stack_depth, __func__);
}

void receive_event_Turn_End(struct Game *sota, SDL_Event *userevent) {
    SOTA_Log_Func("%d\t%s\t" STRINGIZE(__LINE__), call_stack_depth++, __func__);
    /* - Pop all menus - */
    while (DARR_NUM(sota->menu_stack) > 0) {
        tnecs_entity_t menu_pop     = DARR_POP(sota->menu_stack);
        struct MenuComponent *mc    = TNECS_GET_COMPONENT(sota->world, menu_pop, MenuComponent);
        SDL_assert(mc           != NULL);
        SDL_assert(mc->elem_pos != NULL);
        mc->visible = false;
    }

    /* - focus cursor on tilemap - */
    Game_cursorFocus_onMap(sota);

    // TODO:
    // - Get next army
    // - Give control to Ai/Playerr
    // - Begin for next army Turn
    // - if AI:
    //      - Perform AI actions
    //      - End turn
    Event_Emit(__func__, SDL_USEREVENT, event_Turn_Begin, NULL, NULL);
    SOTA_Log_Func("%d\t%s\t" STRINGIZE(__LINE__), --call_stack_depth, __func__);
}

void receive_event_Unit_Enters_Shop(struct Game *sota, SDL_Event *userevent) {
    SOTA_Log_Func("%d\t%s\t" STRINGIZE(__LINE__), call_stack_depth++, __func__);
    SOTA_Log_Func("%d\t%s\t" STRINGIZE(__LINE__), --call_stack_depth, __func__);
}

void receive_event_Unit_Enters_Village(struct Game *sota, SDL_Event *userevent) {
    SOTA_Log_Func("%d\t%s\t" STRINGIZE(__LINE__), call_stack_depth++, __func__);
    SOTA_Log_Func("%d\t%s\t" STRINGIZE(__LINE__), --call_stack_depth, __func__);
}

void receive_event_Unit_Enters_Armory(struct Game *sota, SDL_Event *userevent) {
    SOTA_Log_Func("%d\t%s\t" STRINGIZE(__LINE__), call_stack_depth++, __func__);
    SOTA_Log_Func("%d\t%s\t" STRINGIZE(__LINE__), --call_stack_depth, __func__);
}

void receive_event_Unit_Select(struct Game *sota, SDL_Event *userevent) {
    SOTA_Log_Func("%d\t%s\t" STRINGIZE(__LINE__), call_stack_depth++, __func__);
    sota->selected_unit_entity = *((tnecs_entity_t *) userevent->user.data2);
    sota->aggressor = sota->selected_unit_entity;
    SDL_assert(sota->selected_unit_entity > TNECS_NULL);
    SDL_assert(sota->state                == GAME_STATE_Gameplay_Map);
    if (fsm_eUnitSel_ss[sota->substate] != NULL)
        fsm_eUnitSel_ss[sota->substate](sota, sota->entity_cursor);
    SOTA_Log_Func("%d\t%s\t" STRINGIZE(__LINE__), --call_stack_depth, __func__);
}

void receive_event_Unit_Deselect(struct Game *sota, SDL_Event *userevent) {
    SOTA_Log_Func("%d\t%s\t" STRINGIZE(__LINE__), call_stack_depth++, __func__);
    SDL_assert(sota->entity_cursor != TNECS_NULL);
    sota->aggressor = TNECS_NULL;
    sota->defendant = TNECS_NULL;

    if (sota->selected_unit_entity == TNECS_NULL) {
        SOTA_Log_Func("%d\t%s\t" STRINGIZE(__LINE__), --call_stack_depth, __func__);
        return;
    }

    tnecs_entity_t unit_ent     = sota->selected_unit_entity;
    struct Position *pos_ptr    = TNECS_GET_COMPONENT(sota->world, unit_ent, Position);
    struct Unit *unit_ptr       = TNECS_GET_COMPONENT(sota->world, unit_ent, Unit);
    SDL_assert(pos_ptr  != NULL);
    SDL_assert(unit_ptr != NULL);

    sota->selected_unit_moved_position.x = pos_ptr->tilemap_pos.x;
    sota->selected_unit_moved_position.y = pos_ptr->tilemap_pos.y;

    /* - Reset overlay modes and/or go back to standby - */
    if (fsm_eUnitDsel_ss[sota->substate] != NULL)
        fsm_eUnitDsel_ss[sota->substate](sota, sota->entity_cursor);

    /* - New overlays - */
    if (SotA_isPC(unit_ptr->army)) {
        /* - Hide PC overlay - */
        sota->map->show_overlay = false;
    } else {
        /* - Show NPC danger - */
        if (unit_ptr->show_danger) {
            tnecs_entity_t *ent_ptr = &sota->selected_unit_entity;
            Event_Emit(__func__, SDL_USEREVENT, event_Unit_Danger, NULL, ent_ptr);
        }
    }

    sota->selected_unit_entity = TNECS_NULL;
    // sota->hovered_unit_entity  = TNECS_NULL;
    SOTA_Log_Func("%d\t%s\t" STRINGIZE(__LINE__), --call_stack_depth, __func__);
}

void receive_event_Unit_Entity_Return(struct Game *sota, SDL_Event *userevent) {
    SOTA_Log_Func("%d\t%s\t" STRINGIZE(__LINE__), call_stack_depth++, __func__);
    SOTA_Log_Func("%d\t%s\t" STRINGIZE(__LINE__), --call_stack_depth, __func__);
}

void receive_event_Unit_Icon_Return(struct Game *sota, SDL_Event *userevent) {
    SOTA_Log_Func("%d\t%s\t" STRINGIZE(__LINE__), call_stack_depth++, __func__);

    /* - Hide overlay/movemap - */
    sota->map->show_overlay = false;
    tnecs_entity_t returning = sota->selected_unit_entity;
    SDL_assert(returning > TNECS_NULL);

    struct Position *pos_ptr  = TNECS_GET_COMPONENT(sota->world, returning, Position);
    struct Sprite *sprite = TNECS_GET_COMPONENT(sota->world, sota->selected_unit_entity, Sprite);
    SDL_assert(pos_ptr != NULL);
    SDL_assert(sprite != NULL);

    /* - Resetting map animation loop - */
    struct Point initial = sota->selected_unit_initial_position;
    SDL_assert(sprite->spritesheet != NULL);
    SDL_assert(sprite->spritesheet->loop_num == MAP_UNIT_SPRITE_LOOP_NUM);
    if ((initial.x == pos_ptr->tilemap_pos.x) && (initial.y == pos_ptr->tilemap_pos.y))
        Spritesheet_Loop_Set(sprite->spritesheet, MAP_UNIT_SPRITE_LOOP_TAUNT, sprite->flip);
    else
        Spritesheet_Loop_Set(sprite->spritesheet, MAP_UNIT_SPRITE_LOOP_IDLE, sprite->flip);
    Sprite_Animation_Loop(sprite);
    Sprite_Draw(sprite, sota->renderer);

    /* - Returning to initial position - */
    pos_ptr->tilemap_pos = initial;
    pos_ptr->pixel_pos.x = initial.x * pos_ptr->scale[0];
    pos_ptr->pixel_pos.y = initial.y * pos_ptr->scale[1];

    SOTA_Log_Func("%d\t%s\t" STRINGIZE(__LINE__), --call_stack_depth, __func__);
}

void receive_event_Unit_Moves(struct Game *sota, SDL_Event *userevent) {
    SOTA_Log_Func("%d\t%s\t" STRINGIZE(__LINE__), call_stack_depth++, __func__);
    SDL_assert(userevent->user.data1 != NULL);
    SDL_assert(sota->entity_cursor          != TNECS_NULL);
    SDL_assert(sota->selected_unit_entity   != TNECS_NULL);

    /* -- Initialize Arrow -- */
    struct Position *cpos;
    struct Unit *selected;
    cpos     = TNECS_GET_COMPONENT(sota->world, sota->entity_cursor,        Position);
    selected = TNECS_GET_COMPONENT(sota->world, sota->selected_unit_entity, Unit);
    SDL_assert(cpos                 != NULL);
    SDL_assert(selected             != NULL);
    SDL_assert(sota->map->costmap   != NULL);
    struct Map *map = sota->map;
    Arrow_Path_Init(map->arrow, map->costmap, Unit_computeMove(selected), cpos->tilemap_pos);
    strncpy(sota->reason, "friendly unit was selected and can move", sizeof(sota->reason));
    Game_subState_Set(sota, GAME_SUBSTATE_MAP_UNIT_MOVES, sota->reason);

    /* -- Sprite walking animation -- */
    struct Sprite *sprite = TNECS_GET_COMPONENT(sota->world, sota->selected_unit_entity, Sprite);
    SDL_assert(sprite->spritesheet != NULL);
    SDL_assert(sprite->spritesheet->loop_num == MAP_UNIT_SPRITE_LOOP_NUM);
    // TODO: check if arrow exists and is long, revert to correct orientation
    if (sprite->flip == SDL_FLIP_HORIZONTAL)
        Spritesheet_Loop_Set(sprite->spritesheet, MAP_UNIT_SPRITE_LOOP_MOVEL, sprite->flip);
    else
        Spritesheet_Loop_Set(sprite->spritesheet, MAP_UNIT_SPRITE_LOOP_MOVER, sprite->flip);
    Sprite_Animation_Loop(sprite);
    Sprite_Draw(sprite, sota->renderer);

    SOTA_Log_Func("%d\t%s\t" STRINGIZE(__LINE__), --call_stack_depth, __func__);
}

void receive_event_Cursor_Hovers_Unit(struct Game *sota, SDL_Event *userevent) {
    SOTA_Log_Func("%d\t%s\t" STRINGIZE(__LINE__), call_stack_depth++, __func__);
    sota->hovered_unit_entity = *(tnecs_entity_t *)userevent->user.data2;
    SDL_assert(sota->hovered_unit_entity != TNECS_NULL);

    if (fsm_eCrsHvUnit_ss[sota->substate] != NULL)
        fsm_eCrsHvUnit_ss[sota->substate](sota, sota->hovered_unit_entity);
    SOTA_Log_Func("%d\t%s\t" STRINGIZE(__LINE__), --call_stack_depth, __func__);
}

void receive_event_Cursor_Dehovers_Unit(struct Game *sota, SDL_Event *userevent) {
    SOTA_Log_Func("%d\t%s\t" STRINGIZE(__LINE__), call_stack_depth++, __func__);
    tnecs_entity_t dehovered_unit_entity = sota->hovered_unit_entity;
    sota->hovered_unit_entity            = TNECS_NULL;

    if (fsm_eCrsDeHvUnit_ss[sota->substate] != NULL)
        fsm_eCrsDeHvUnit_ss[sota->substate](sota, dehovered_unit_entity);
    SOTA_Log_Func("%d\t%s\t" STRINGIZE(__LINE__), --call_stack_depth, __func__);
}

void receive_event_Units_Refresh(struct Game *sota, SDL_Event *userevent) {
    SOTA_Log_Func("%d\t%s\t" STRINGIZE(__LINE__), call_stack_depth++, __func__);
    SOTA_Log_Func("%d\t%s\t" STRINGIZE(__LINE__), --call_stack_depth, __func__);
}

void receive_event_Unit_Danger(struct Game *sota, SDL_Event *userevent) {
    SOTA_Log_Func("%d\t%s\t" STRINGIZE(__LINE__), call_stack_depth++, __func__);
    SDL_assert(sota->entity_cursor != TNECS_NULL);

    if (fsm_eUnitDng_ss[sota->substate] != NULL)
        fsm_eUnitDng_ss[sota->substate](sota, sota->entity_cursor);
    SOTA_Log_Func("%d\t%s\t" STRINGIZE(__LINE__), --call_stack_depth, __func__);
}

void receive_event_Unit_Dance(struct Game *sota, SDL_Event *userevent) {
    SOTA_Log_Func("%d\t%s\t" STRINGIZE(__LINE__), call_stack_depth++, __func__);
    SOTA_Log_Func("%d\t%s\t" STRINGIZE(__LINE__), --call_stack_depth, __func__);
}

void receive_event_Menu_Select(struct Game *sota, SDL_Event *userevent) {
    SOTA_Log_Func("%d\t%s\t" STRINGIZE(__LINE__), call_stack_depth++, __func__);

    SOTA_Log_Func("%d\t%s\t" STRINGIZE(__LINE__), --call_stack_depth, __func__);
}

// Menu_Created event should be the ONLY EVENT that changes game substate to menu
void receive_event_Menu_Created(struct Game *sota, SDL_Event *userevent) {
    SOTA_Log_Func("%d\t%s\t" STRINGIZE(__LINE__), call_stack_depth++, __func__);
    tnecs_entity_t menu_entity = *(tnecs_entity_t *)userevent->user.data1;

    /* - Set sprite to combat stance - */
    // Note: Map Action menu does not select unit
    if ((menu_entity == sota->item_select_menu) && (sota->selected_unit_entity)) {
        struct Sprite *sprite;
        sprite = TNECS_GET_COMPONENT(sota->world, sota->selected_unit_entity, Sprite);

        SDL_assert(sprite->spritesheet != NULL);
        SDL_assert(sprite->spritesheet->loop_num == MAP_UNIT_SPRITE_LOOP_NUM);
        Spritesheet_Loop_Set(sprite->spritesheet, MAP_UNIT_SPRITE_LOOP_STANCE, sprite->flip);
        Sprite_Animation_Loop(sprite);
        Sprite_Draw(sprite, sota->renderer);
    }

    // Note: reason set by event sender is kinda dumb.
    // reason might get overwritten...
    if (sota->substate != GAME_SUBSTATE_MENU)
        Game_subState_Set(sota, GAME_SUBSTATE_MENU, sota->reason);

    SOTA_Log_Func("%d\t%s\t" STRINGIZE(__LINE__), --call_stack_depth, __func__);
}

void receive_event_Loadout_Selected(struct Game *sota, SDL_Event *userevent) {
    SOTA_Log_Func("%d\t%s\t" STRINGIZE(__LINE__), call_stack_depth++, __func__);
    /* Aggressor loadout was selected, time to select defendant. */

    /* - Turn menu_attack invisible - */
    int stack_top               = DARR_NUM(sota->menu_stack) - 1;
    tnecs_entity_t menu_top     = sota->menu_stack[stack_top];
    struct MenuComponent *mc    = TNECS_GET_COMPONENT(sota->world, menu_top, MenuComponent);
    SDL_assert(mc              != NULL);
    SDL_assert(mc->type        == MENU_TYPE_WEAPON_SELECT);
    SDL_assert(mc->elem_pos    != NULL);
    mc->visible = false;

    /* - Turn PopUp_Loadout_Stats invisible - */
    Game_PopUp_Loadout_Stats_Hide(sota);

    // Setting: Select loadout first
    /* - Switch to Map_Candidates substate - */
    SDL_assert(sota->state    == GAME_STATE_Gameplay_Map);
    SDL_assert(sota->substate == GAME_SUBSTATE_MENU);
    strncpy(sota->reason, "loadout was selected, time to select defendant", sizeof(sota->reason));
    Game_Switch_toCandidates(sota, sota->defendants);

    SOTA_Log_Func("%d\t%s\t" STRINGIZE(__LINE__), --call_stack_depth, __func__);
}

void receive_event_Input_ZOOM_IN(struct Game *sota, SDL_Event *userevent) {
    SOTA_Log_Func("%d\t%s\t" STRINGIZE(__LINE__), call_stack_depth++, __func__);
    /* -- Check: Only Zoom_in on the map -- */
    SDL_assert(sota->state == GAME_STATE_Gameplay_Map);
    bool correct_substate = (sota->substate == GAME_SUBSTATE_STANDBY);
    correct_substate |= (sota->substate == GAME_SUBSTATE_MAP_CANDIDATES);
    correct_substate |= (sota->substate == GAME_SUBSTATE_MAP_UNIT_MOVES);
    correct_substate |= (sota->substate == GAME_SUBSTATE_MAP_ANIMATION);
    SDL_assert(correct_substate);

    i32 controller_type = *(i32 *)userevent->user.data1;
    Events_Controllers_Check(sota, controller_type);

    /* -- Zoom in to sprite_atorigin -- */
    const struct Sprite *sprite_atorigin;
    const struct Sprite *cursor_sprite;
    const struct Sprite *mouse_sprite;
    cursor_sprite   = TNECS_GET_COMPONENT(sota->world, sota->entity_cursor, Sprite);
    mouse_sprite    = TNECS_GET_COMPONENT(sota->world, sota->entity_mouse, Sprite);
    SDL_assert(cursor_sprite != NULL);
    SDL_assert(mouse_sprite != NULL);

    /* - Choose which sprite to zoom into - */
    switch (controller_type) {
        case CONTROLLER_GAMEPAD:
            sprite_atorigin = cursor_sprite;
        case CONTROLLER_MOUSE:
            sprite_atorigin = mouse_sprite;
        default:
            sprite_atorigin = cursor_sprite;
    }
    /* - Zoom in - */
    struct Camera *cam = &sota->camera;
    struct SDL_Rect dstrect = sprite_atorigin->dstrect;
    float previous_zoom = cam->zoom;
    float new_zoom = cam->zoom + CAMERA_INCREMENT;
    cam->zoom = new_zoom < MAX_CAMERA_ZOOM ? new_zoom : cam->zoom;
    cam->offset.x = SOTA_ZOOMTOPOINT(cam->zoom / previous_zoom, cam->offset.x, dstrect.x);
    cam->offset.y = SOTA_ZOOMTOPOINT(cam->zoom / previous_zoom, cam->offset.y, dstrect.y);
    sota->map->camera_moved = true;


    SOTA_Log_Func("%d\t%s\t" STRINGIZE(__LINE__), --call_stack_depth, __func__);
}

void receive_event_Input_ZOOM_OUT(struct Game *sota, SDL_Event *userevent) {
    SOTA_Log_Func("%d\t%s\t" STRINGIZE(__LINE__), call_stack_depth++, __func__);
    /* -- Check: Only Zoom_out on the map -- */
    SDL_assert(sota->state == GAME_STATE_Gameplay_Map);
    bool correct_substate = (sota->substate == GAME_SUBSTATE_STANDBY);
    correct_substate |= (sota->substate == GAME_SUBSTATE_MAP_CANDIDATES);
    correct_substate |= (sota->substate == GAME_SUBSTATE_MAP_UNIT_MOVES);
    correct_substate |= (sota->substate == GAME_SUBSTATE_MAP_ANIMATION);
    SDL_assert(correct_substate);

    i32 controller_type = *(i32 *)userevent->user.data1;
    Events_Controllers_Check(sota, controller_type);

    /* -- Zoom out to sprite_atorigin -- */
    const struct Sprite *cursor_sprite;
    const struct Sprite *mouse_sprite;
    const struct Sprite *sprite_atorigin;
    cursor_sprite = TNECS_GET_COMPONENT(sota->world, sota->entity_cursor, Sprite);
    mouse_sprite = TNECS_GET_COMPONENT(sota->world, sota->entity_mouse, Sprite);
    SDL_assert(cursor_sprite != NULL);
    SDL_assert(mouse_sprite != NULL);

    /* - Choose which sprite to zoom into - */
    switch (controller_type) {
        case CONTROLLER_GAMEPAD:
            sprite_atorigin = cursor_sprite;
        case CONTROLLER_MOUSE:
            sprite_atorigin = mouse_sprite;
        default:
            sprite_atorigin = cursor_sprite;
    }
    /* - Zoom out - */
    struct Camera *cam = &sota->camera;
    struct SDL_Rect dstrect = sprite_atorigin->dstrect;
    float previous_zoom = cam->zoom;
    float new_zoom = cam->zoom - CAMERA_INCREMENT;
    cam->zoom = new_zoom > MIN_CAMERA_ZOOM ? new_zoom : cam->zoom;
    cam->offset.x = SOTA_ZOOMTOPOINT(cam->zoom / previous_zoom, cam->offset.x, dstrect.x);
    cam->offset.y = SOTA_ZOOMTOPOINT(cam->zoom / previous_zoom, cam->offset.y, dstrect.y);
    sota->map->camera_moved = true;


    SOTA_Log_Func("%d\t%s\t" STRINGIZE(__LINE__), --call_stack_depth, __func__);
}

void receive_event_Input_UP(    struct Game *sota, SDL_Event *userevent) {}
void receive_event_Input_LEFT(  struct Game *sota, SDL_Event *userevent) {}
void receive_event_Input_DOWN(  struct Game *sota, SDL_Event *userevent) {}
void receive_event_Input_RIGHT( struct Game *sota, SDL_Event *userevent) {}

void receive_event_Input_MENURIGHT(struct Game *sota, SDL_Event *userevent) {
    SOTA_Log_Func("%d\t%s\t" STRINGIZE(__LINE__), call_stack_depth++, __func__);
    i32 controller_type = *(i32 *)userevent->user.data1;
    Events_Controllers_Check(sota, controller_type);

    if (fsm_eMenuRight_s[sota->state] != NULL)
        fsm_eMenuRight_s[sota->state](sota, controller_type);
    SOTA_Log_Func("%d\t%s\t" STRINGIZE(__LINE__), --call_stack_depth, __func__);
}


void receive_event_Input_MENULEFT(struct Game *sota, SDL_Event *userevent) {
    SOTA_Log_Func("%d\t%s\t" STRINGIZE(__LINE__), call_stack_depth++, __func__);
    i32 controller_type = *(i32 *)userevent->user.data1;
    Events_Controllers_Check(sota, controller_type);

    if (fsm_eMenuLeft_s[sota->state] != NULL)
        fsm_eMenuLeft_s[sota->state](sota, controller_type);
    SOTA_Log_Func("%d\t%s\t" STRINGIZE(__LINE__), --call_stack_depth, __func__);
}

void receive_event_Input_MINIMAP(struct Game *sota, SDL_Event *userevent) {
    SOTA_Log_Func("%d\t%s\t" STRINGIZE(__LINE__), call_stack_depth++, __func__);
    i32 controller_type = *(i32 *)userevent->user.data1;
    Events_Controllers_Check(sota, controller_type);

    SOTA_Log_Func("%d\t%s\t" STRINGIZE(__LINE__), --call_stack_depth, __func__);
}

void receive_event_Input_FASTER(struct Game *sota, SDL_Event *userevent) {
    SOTA_Log_Func("%d\t%s\t" STRINGIZE(__LINE__), call_stack_depth++, __func__);
    i32 controller_type = *(i32 *)userevent->user.data1;
    Events_Controllers_Check(sota, controller_type);

    SOTA_Log_Func("%d\t%s\t" STRINGIZE(__LINE__), --call_stack_depth, __func__);
}

void receive_event_Input_PAUSE(struct Game *sota, SDL_Event *userevent) {
    SOTA_Log_Func("%d\t%s\t" STRINGIZE(__LINE__), call_stack_depth++, __func__);
    /* Pausing opens options */
    i32 controller_type = *(i32 *)userevent->user.data1;
    Events_Controllers_Check(sota, controller_type);

    SOTA_Log_Func("%d\t%s\t" STRINGIZE(__LINE__), --call_stack_depth, __func__);
}

void receive_event_Unit_Seize(struct Game *sota, SDL_Event *userevent) {
    SOTA_Log_Func("%d\t%s\t" STRINGIZE(__LINE__), call_stack_depth++, __func__);
    SOTA_Log_Func("%d\t%s\t" STRINGIZE(__LINE__), --call_stack_depth, __func__);
}

void receive_event_Game_Defeat(struct Game *sota, SDL_Event *userevent) {
    SOTA_Log_Func("%d\t%s\t" STRINGIZE(__LINE__), call_stack_depth++, __func__);
    SOTA_Log_Func("%d\t%s\t" STRINGIZE(__LINE__), --call_stack_depth, __func__);
}

void receive_event_Unit_Refresh(struct Game *sota, SDL_Event *userevent) {
    SOTA_Log_Func("%d\t%s\t" STRINGIZE(__LINE__), call_stack_depth++, __func__);
    SOTA_Log_Func("%d\t%s\t" STRINGIZE(__LINE__), --call_stack_depth, __func__);
}

void receive_event_Unit_Wait(struct Game *sota, SDL_Event *userevent) {
    SOTA_Log_Func("%d\t%s\t" STRINGIZE(__LINE__), call_stack_depth++, __func__);

    /* -- Preliminaries -- */
    tnecs_entity_t unit_ent = sota->selected_unit_entity;
    SDL_assert(unit_ent > TNECS_NULL);
    Game_Unit_Wait(sota, unit_ent);

    /* - hide arrow - */
    sota->map->arrow->show = false;

    /* - hide movemap - */
    Map_Stacked_Dangermap_Reset(sota->map);
    Map_Palettemap_Reset(sota->map);

    /* -- Deselect unit and go back to map -- */
    Event_Emit(__func__, SDL_USEREVENT, event_Unit_Deselect, NULL, &sota->selected_unit_entity);

    if (DARR_NUM(sota->menu_stack) == 0)
        Game_cursorFocus_onMap(sota);

    SOTA_Log_Func("%d\t%s\t" STRINGIZE(__LINE__), --call_stack_depth, __func__);
}

void receive_event_Unit_Talk(struct Game *sota, SDL_Event *userevent) {
    SOTA_Log_Func("%d\t%s\t" STRINGIZE(__LINE__), call_stack_depth++, __func__);
    SDL_assert(sota->entity_cursor          > TNECS_NULL);
    SDL_assert(sota->selected_unit_entity   > TNECS_NULL);
    tnecs_entity_t unit_ent = sota->selected_unit_entity;
    Game_Unit_Wait(sota, unit_ent);

    Event_Emit(__func__, SDL_USEREVENT, event_Gameplay_Return2Standby, NULL, NULL);
    SOTA_Log_Func("%d\t%s\t" STRINGIZE(__LINE__), --call_stack_depth, __func__);
}

void receive_event_Unit_Rescue(struct Game *sota, SDL_Event *userevent) {
    SOTA_Log_Func("%d\t%s\t" STRINGIZE(__LINE__), call_stack_depth++, __func__);
    SDL_assert(sota->entity_cursor          > TNECS_NULL);
    SDL_assert(sota->selected_unit_entity   > TNECS_NULL);
    tnecs_entity_t unit_ent = sota->selected_unit_entity;
    Game_Unit_Wait(sota, unit_ent);

    Event_Emit(__func__, SDL_USEREVENT, event_Gameplay_Return2Standby, NULL, NULL);
    SOTA_Log_Func("%d\t%s\t" STRINGIZE(__LINE__), --call_stack_depth, __func__);
}

void receive_event_Combat_Start(struct Game *sota, SDL_Event *userevent) {
    SOTA_Log_Func("%d\t%s\t" STRINGIZE(__LINE__), call_stack_depth++, __func__);

    struct Sprite *agg_sprite = TNECS_GET_COMPONENT(sota->world, sota->aggressor, Sprite);
    struct Sprite *dft_sprite = TNECS_GET_COMPONENT(sota->world, sota->defendant, Sprite);

    // 1. Make cursor visible
    struct Sprite *sprite     = TNECS_GET_COMPONENT(sota->world, sota->entity_cursor, Sprite);
    sprite->visible = true;

    // 3. Animate Combat
    /* -- TODO choose map animation or combat animation -- */
    /* -- change game substate -- */
    strncpy(sota->reason, "Quitting gameplay", sizeof(sota->reason));
    Game_subState_Set(sota, GAME_SUBSTATE_MAP_ANIMATION, sota->reason);
    /* -- find attack direction -- */
    struct Position *agg_posc = TNECS_GET_COMPONENT(sota->world, sota->aggressor, Position);
    struct Position *dft_posc = TNECS_GET_COMPONENT(sota->world, sota->defendant, Position);
    const struct Point *agg_pos = &agg_posc->tilemap_pos;
    const struct Point *dft_pos = &dft_posc->tilemap_pos;

    struct Point move = {dft_pos->x - agg_pos->x, dft_pos->y - agg_pos->y};
    move.x = move.x < -1 ? -1 : move.x;
    move.x = move.x >  1 ?  1 : move.x;
    move.y = move.y < -1 ? -1 : move.y;
    move.y = move.y >  1 ?  1 : move.y;
    if8 direction = Ternary_Direction(move);
    /* -- reset animation states -- */

    switch (direction) {
        case SOTA_DIRECTION_BOTTOM:
        case SOTA_DIRECTION_DIAGONAL_BL:
        case SOTA_DIRECTION_DIAGONAL_BR:
            Sprite_Animation_Restart(agg_sprite, MAP_UNIT_SPRITE_LOOP_ATTACKB);
            Sprite_Animation_Restart(dft_sprite, MAP_UNIT_SPRITE_LOOP_ATTACKB);
            break;
        case SOTA_DIRECTION_TOP:
            Sprite_Animation_Restart(agg_sprite, MAP_UNIT_SPRITE_LOOP_ATTACKT);
            Sprite_Animation_Restart(dft_sprite, MAP_UNIT_SPRITE_LOOP_ATTACKT);
            break;
        case SOTA_DIRECTION_RIGHT:
        case SOTA_DIRECTION_DIAGONAL_TR:
            Sprite_Animation_Restart(agg_sprite, MAP_UNIT_SPRITE_LOOP_ATTACKR);
            Sprite_Animation_Restart(dft_sprite, MAP_UNIT_SPRITE_LOOP_ATTACKL);
            break;
        case SOTA_DIRECTION_LEFT:
        case SOTA_DIRECTION_DIAGONAL_TL:
            Sprite_Animation_Restart(agg_sprite, MAP_UNIT_SPRITE_LOOP_ATTACKL);
            Sprite_Animation_Restart(dft_sprite, MAP_UNIT_SPRITE_LOOP_ATTACKR);
            break;
    }

    struct Timer *agg_timer = TNECS_GET_COMPONENT(sota->world, sota->aggressor, Timer);
    struct Timer *dft_timer = TNECS_GET_COMPONENT(sota->world, sota->defendant, Timer);
    agg_timer->paused = true;
    dft_timer->paused = true;

    /* -- Create combat animation entity -- */
    sota->map_animation = TNECS_ENTITY_CREATE_wCOMPONENTS(sota->world, Timer, CombatAnimation);

    struct CombatAnimation *map_anim;
    map_anim = TNECS_GET_COMPONENT(sota->world, sota->map_animation, CombatAnimation);
    *map_anim = CombatAnimation_default;

    /* - Hide pre combat popup - */
    struct MenuComponent *mc;
    mc = TNECS_GET_COMPONENT(sota->world, sota->pre_combat_menu, MenuComponent);
    mc->visible = false;

    /* - Show map combat popup - */
    if (sota->popups[POPUP_TYPE_MAP_COMBAT] == TNECS_NULL)
        Game_PopUp_Map_Combat_Create(sota);

    struct Unit *aggressor = TNECS_GET_COMPONENT(sota->world, sota->aggressor, Unit);
    struct Unit *defendant = TNECS_GET_COMPONENT(sota->world, sota->defendant, Unit);

    struct PopUp *popup    = TNECS_GET_COMPONENT(sota->world, sota->popups[POPUP_TYPE_MAP_COMBAT],
                                                 PopUp);
    struct PopUp_Map_Combat *pmc = popup->data;

    PopUp_Map_Combat_Units(pmc, sota, aggressor, defendant, agg_pos, dft_pos);

    /* - Deselect weapons in LoadoutSelectMenu - */
    mc = TNECS_GET_COMPONENT(sota->world, sota->weapon_select_menu, MenuComponent);
    struct LoadoutSelectMenu *wsm = mc->data;
    LoadoutSelectMenu_Select_Reset(wsm);

    /* - Set previous candidate to NULL - */
    sota->previous_candidate = -1;

    SOTA_Log_Func("%d\t%s\t" STRINGIZE(__LINE__), --call_stack_depth, __func__);
}

void receive_event_Combat_End(struct Game *sota, SDL_Event *userevent) {
    SOTA_Log_Func("%d\t%s\t" STRINGIZE(__LINE__), call_stack_depth++, __func__);

    Event_Emit(__func__, SDL_USEREVENT, event_Unit_Wait, NULL, NULL);

    // 1. Resolve Combat
    struct Unit *aggressor = TNECS_GET_COMPONENT(sota->world, sota->aggressor, Unit);
    struct Unit *defendant = TNECS_GET_COMPONENT(sota->world, sota->defendant, Unit);
    Combat_Resolve(sota->combat_attacks, sota->combat_forecast.attack_num, aggressor, defendant);

    tnecs_entity_t popup_ent     = sota->popups[POPUP_TYPE_MAP_COMBAT];
    struct PopUp *popup_ptr      = TNECS_GET_COMPONENT(sota->world, popup_ent, PopUp);
    struct PopUp_Map_Combat *pmc = popup_ptr->data;

    SDL_assert(aggressor->current_hp == pmc->agg_current_hp);
    SDL_assert(defendant->current_hp == pmc->dft_current_hp);

    // 2. Update maphpbars.
    struct MapHPBar *map_hp_bar = TNECS_GET_COMPONENT(sota->world, sota->aggressor, MapHPBar);
    struct Unit *unit           = TNECS_GET_COMPONENT(sota->world, sota->aggressor, Unit);
    SDL_assert(map_hp_bar != NULL);
    SDL_assert(map_hp_bar->unit_ent == sota->aggressor);
    map_hp_bar->update  = true;
    map_hp_bar->visible = true;
    map_hp_bar = TNECS_GET_COMPONENT(sota->world, sota->defendant, MapHPBar);
    unit = TNECS_GET_COMPONENT(sota->world, sota->defendant, Unit);
    map_hp_bar->update  = true;
    map_hp_bar->visible = true;

    // 3. reset animation states
    struct Sprite *dft_sprite = TNECS_GET_COMPONENT(sota->world, sota->defendant, Sprite);
    struct Sprite *agg_sprite = TNECS_GET_COMPONENT(sota->world, sota->aggressor, Sprite);
    Sprite_Animation_Restart(agg_sprite, MAP_UNIT_SPRITE_LOOP_IDLE);
    Sprite_Animation_Restart(dft_sprite, MAP_UNIT_SPRITE_LOOP_IDLE);

    // 4. Restart Defendant animation
    struct Timer *dft_timer = TNECS_GET_COMPONENT(sota->world, sota->defendant, Timer);
    dft_timer->paused = false;

    // 5. Hide PopUp_Map_Combat
    // Game_PopUp_Map_Combat_Hide(sota);

    // 6. Return to standby
    *data1_entity = sota->entity_cursor;
    Event_Emit(__func__, SDL_USEREVENT, event_Gameplay_Return2Standby, NULL, NULL);

    SDL_assert(map_hp_bar->update);
    SDL_assert(map_hp_bar->visible);

    SOTA_Log_Func("%d\t%s\t" STRINGIZE(__LINE__), --call_stack_depth, __func__);
}

void receive_event_Defendant_Select(struct Game *sota, SDL_Event *userevent) {
    SOTA_Log_Func("%d\t%s\t" STRINGIZE(__LINE__), call_stack_depth++, __func__);

    // 1. Compute Combat stuff
    Game_Combat_Outcome(sota);

    // 2. Enable Pre-combat menu
    Game_PopUp_Pre_Combat_Enable(sota);

    struct MenuComponent *mc;
    mc = TNECS_GET_COMPONENT(sota->world, sota->pre_combat_menu, MenuComponent);
    mc->visible = true;

    // 3. Hide other popups
    // TODO: place at appropriate places.

    // 4. Switch substate to Menu
    strncpy(sota->reason, "defendant was selected, need to select combat", sizeof(sota->reason));
    Game_subState_Set(sota, GAME_SUBSTATE_MENU, sota->reason);

    // 5. Focus on Menu
    Game_cursorFocus_onMenu(sota);

    // 6. Attackmap only defendant.
    struct Map *map = sota->map;
    struct Position *pos  = TNECS_GET_COMPONENT(sota->world, sota->defendant, Position);
    memset(map->attacktomap, 0, map->row_len * map->col_len * sizeof(*map->attacktomap));
    map->attacktomap[(pos->tilemap_pos.y * map->col_len + pos->tilemap_pos.x)] = 1;
    Map_Palettemap_Autoset(sota->map, MAP_OVERLAY_ATTACK);
    Map_Stacked_Dangermap_Compute(sota->map);

    SOTA_Log_Func("%d\t%s\t" STRINGIZE(__LINE__), --call_stack_depth, __func__);
}

void receive_event_Unit_Trade(struct Game *sota, SDL_Event *userevent) {
    SOTA_Log_Func("%d\t%s\t" STRINGIZE(__LINE__), call_stack_depth++, __func__);
    SDL_assert(sota->entity_cursor > TNECS_NULL);
    if (sota->selected_unit_entity == TNECS_NULL)
        return;

    Game_Unit_Wait(sota, sota->selected_unit_entity);

    Event_Emit(__func__, SDL_USEREVENT, event_Gameplay_Return2Standby, NULL, NULL);
    SOTA_Log_Func("%d\t%s\t" STRINGIZE(__LINE__), --call_stack_depth, __func__);
}

void receive_event_Unit_Escape(struct Game *sota, SDL_Event *userevent) {
    SOTA_Log_Func("%d\t%s\t" STRINGIZE(__LINE__), call_stack_depth++, __func__);
    SDL_assert(sota->entity_cursor > TNECS_NULL);
    if (sota->selected_unit_entity == TNECS_NULL)
        return;

    Game_Unit_Wait(sota, sota->selected_unit_entity);

    Event_Emit(__func__, SDL_USEREVENT, event_Gameplay_Return2Standby, NULL, NULL);
    SOTA_Log_Func("%d\t%s\t" STRINGIZE(__LINE__), --call_stack_depth, __func__);
}

void receive_event_Unit_Staff(struct Game *sota, SDL_Event *userevent) {
    SOTA_Log_Func("%d\t%s\t" STRINGIZE(__LINE__), call_stack_depth++, __func__);
    SOTA_Log_Func("%d\t%s\t" STRINGIZE(__LINE__), --call_stack_depth, __func__);
}

void receive_event_Unit_Items(struct Game *sota, SDL_Event *userevent) {
    SOTA_Log_Func("%d\t%s\t" STRINGIZE(__LINE__), call_stack_depth++, __func__);
    SDL_assert(sota->entity_cursor > TNECS_NULL);
    SDL_assert(sota->selected_unit_entity != TNECS_NULL);

    Game_Unit_Wait(sota, sota->selected_unit_entity);

    Event_Emit(__func__, SDL_USEREVENT, event_Gameplay_Return2Standby, NULL, NULL);
    SOTA_Log_Func("%d\t%s\t" STRINGIZE(__LINE__), --call_stack_depth, __func__);
}

void receive_event_Unit_Heals(struct Game *sota, SDL_Event *userevent) {
    SOTA_Log_Func("%d\t%s\t" STRINGIZE(__LINE__), call_stack_depth++, __func__);
    SOTA_Log_Func("%d\t%s\t" STRINGIZE(__LINE__), --call_stack_depth, __func__);
}

void receive_event_Convoy_Check(struct Game *sota, SDL_Event *userevent) {
    SOTA_Log_Func("%d\t%s\t" STRINGIZE(__LINE__), call_stack_depth++, __func__);
    SOTA_Log_Func("%d\t%s\t" STRINGIZE(__LINE__), --call_stack_depth, __func__);
}

void receive_event_Convoy_Map(struct Game *sota, SDL_Event *userevent) {
    SOTA_Log_Func("%d\t%s\t" STRINGIZE(__LINE__), call_stack_depth++, __func__);
    SOTA_Log_Func("%d\t%s\t" STRINGIZE(__LINE__), --call_stack_depth, __func__);
}

void receive_event_Unit_Dies(struct Game *sota, SDL_Event *userevent) {
    SOTA_Log_Func("%d\t%s\t" STRINGIZE(__LINE__), call_stack_depth++, __func__);
    tnecs_entity_t killer_entity = *(tnecs_entity_t *) userevent->user.data1;
    tnecs_entity_t victim_entity = *(tnecs_entity_t *) userevent->user.data2;
    struct Unit *killer = TNECS_GET_COMPONENT(sota->world, killer_entity, Unit);
    struct Unit *victim = TNECS_GET_COMPONENT(sota->world, victim_entity, Unit);
    int regrets = killer->regrets;
    killer->regrets = regrets > UINT8_MAX - REGRET_KILL ? UINT8_MAX : regrets + REGRET_KILL;
    SOTA_Log_Func("%d\t%s\t" STRINGIZE(__LINE__), --call_stack_depth, __func__);
}

void receive_event_Unit_Loots(struct Game *sota, SDL_Event *userevent) {
    SOTA_Log_Func("%d\t%s\t" STRINGIZE(__LINE__), call_stack_depth++, __func__);
    tnecs_entity_t looter_entity = *(tnecs_entity_t *) userevent->user.data1;
    tnecs_entity_t victim_entity = *(tnecs_entity_t *) userevent->user.data2;
    struct Unit *looter = TNECS_GET_COMPONENT(sota->world, looter_entity, Unit);
    struct Unit *victim = TNECS_GET_COMPONENT(sota->world, victim_entity, Unit);
    int regrets = looter->regrets;
    looter->regrets = regrets > UINT8_MAX - REGRET_LOOT ? UINT8_MAX : regrets + REGRET_LOOT;
    SOTA_Log_Func("%d\t%s\t" STRINGIZE(__LINE__), --call_stack_depth, __func__);
}

void receive_event_Increment_Attack(struct Game *sota, SDL_Event *userevent) {
    SOTA_Log_Func("%d\t%s\t" STRINGIZE(__LINE__), call_stack_depth++, __func__);
    // Should be called only during map combat

    tnecs_entity_t popup_ent;
    /* -- Popup_Map_Combat -- */
    popup_ent = sota->popups[POPUP_TYPE_MAP_COMBAT];
    struct PopUp *popup_ptr = TNECS_GET_COMPONENT(sota->world, popup_ent, PopUp);
    struct PopUp_Map_Combat *pmc = popup_ptr->data;
    pmc->aggressor = TNECS_GET_COMPONENT(sota->world, sota->aggressor, Unit);
    pmc->defendant = TNECS_GET_COMPONENT(sota->world, sota->defendant, Unit);

    // 1. Get next HP of attacked unit
    struct Combat_Attack attack = sota->combat_attacks[pmc->current_attack++];
    u8 next_hp = attack.attacker ? pmc->dft_current_hp : pmc->agg_current_hp;
    next_hp = Combat_Next_HP(attack, next_hp);

    // 2. Update popup_map_combat
    if (attack.attacker)
        pmc->dft_current_hp = next_hp;
    else
        pmc->agg_current_hp = next_hp;

    SDL_assert(popup_ptr != NULL);
    SDL_assert(pmc != NULL);
    SDL_assert(sota->aggressor != TNECS_NULL);
    SDL_assert(sota->defendant != TNECS_NULL);
    pmc->update = true;

    SOTA_Log_Func("%d\t%s\t" STRINGIZE(__LINE__), --call_stack_depth, __func__);
}

void receive_event_Unit_Agonizes(struct Game *sota, SDL_Event *userevent) {
    SOTA_Log_Func("%d\t%s\t" STRINGIZE(__LINE__), call_stack_depth++, __func__);
    tnecs_entity_t victor_entity = *(tnecs_entity_t *) userevent->user.data1;
    tnecs_entity_t victim_entity = *(tnecs_entity_t *) userevent->user.data2;
    struct Unit *victor = TNECS_GET_COMPONENT(sota->world, victor_entity, Unit);
    struct Unit *victim = TNECS_GET_COMPONENT(sota->world, victim_entity, Unit);
    int regrets = victor->regrets;
    victor->regrets = regrets > UINT8_MAX - REGRET_LOOT ? UINT8_MAX : regrets + REGRET_LOOT;
    SOTA_Log_Func("%d\t%s\t" STRINGIZE(__LINE__), --call_stack_depth, __func__);
}

/* --- EVENT UTILITIES --- */
void receive_event_SDL_WINDOWEVENT(struct Game *sota, SDL_Event *event) {
    SOTA_Log_Func("%d\t%s\t" STRINGIZE(__LINE__), call_stack_depth++, __func__);
    if (event->window.windowID != SDL_GetWindowID(sota->window))
        return;

    switch (event->window.event) {
        case SDL_WINDOWEVENT_CLOSE:
            SOTA_Log_Debug("SDL_WINDOWEVENT_CLOSE");
            Event_Emit(__func__, SDL_QUIT, event_SDL_QUIT, NULL, NULL);
            break;
    }

    SOTA_Log_Func("%d\t%s\t" STRINGIZE(__LINE__), --call_stack_depth, __func__);
}

void Events_Names_Declare() {
    SOTA_Log_Func("%d\t%s\t" STRINGIZE(__LINE__), call_stack_depth++, __func__);
    event_Start = SDL_RegisterEvents(1);
#define REGISTER_ENUM(x, y) event_##x = SDL_RegisterEvents(1);
#include "names/events.h"
#undef REGISTER_ENUM
#define REGISTER_ENUM(x, y)  event_Input_##x = SDL_RegisterEvents(1);
#include "names/input.h"
#undef REGISTER_ENUM
    event_End = SDL_RegisterEvents(1);
    SOTA_Log_Func("%d\t%s\t" STRINGIZE(__LINE__), --call_stack_depth, __func__);
}

extern void Events_Names_Free() {
    SOTA_Log_Func("%d\t%s\t" STRINGIZE(__LINE__), call_stack_depth++, __func__);
    if (event_names == NULL)
        return;

    for (size_t i = 0; i < ((event_End - event_Start) + 1); i++) {
        if (event_names[i] != NULL) {
            free(event_names[i]);
            event_names[i] = NULL;
        }
    }
    free(event_names);
    SOTA_Log_Func("%d\t%s\t" STRINGIZE(__LINE__), --call_stack_depth, __func__);
}

char **event_names = NULL;
extern void Events_Names_Alloc() {
    SOTA_Log_Func("%d\t%s\t" STRINGIZE(__LINE__), call_stack_depth++, __func__);
    SOTA_Log_Debug("event_End event_Start %d %d", event_End, event_Start);
    SOTA_Log_Debug("event_End - event_Start %d", event_End - event_Start);
    event_names = (char **)calloc((event_End - event_Start) + 1, sizeof(*event_names));
    SDL_assert(event_names != NULL);
    char *temp_str;

#define REGISTER_ENUM(x, y) temp_str = (char *) malloc(DEFAULT_BUFFER_SIZE);\
    strncpy(temp_str, #x, sizeof(#x));\
    temp_str[sizeof(#x)] = '\0';\
    event_names[(event_##x - event_Start)] = nstr_toUpper(temp_str);
#include "names/events.h"
#undef REGISTER_ENUM

#define REGISTER_ENUM(x, y) temp_str = (char *) malloc(DEFAULT_BUFFER_SIZE);\
    strncpy(temp_str, #x, sizeof(#x));\
    temp_str[sizeof(#x)] = '\0';\
    event_names[(event_Input_##x - event_Start)] = nstr_toUpper(temp_str);
#include "names/input.h"
#undef REGISTER_ENUM

    SOTA_Log_Func("%d\t%s\t" STRINGIZE(__LINE__), --call_stack_depth, __func__);
}

void Events_Receivers_Free() {
    SOTA_Log_Func("%d\t%s\t" STRINGIZE(__LINE__), call_stack_depth++, __func__);
    if (receivers_dtab != NULL)
        DTAB_FREE(receivers_dtab);
    SOTA_Log_Func("%d\t%s\t" STRINGIZE(__LINE__), --call_stack_depth, __func__);
}

void Events_Receivers_Declare() {
    SOTA_Log_Func("%d\t%s\t" STRINGIZE(__LINE__), call_stack_depth++, __func__);
    u32 temp_index;
    Events_Receivers_Free();
    receiver_t temp_receiver_p;
    DTAB_INIT(receivers_dtab, receiver_t);

#define REGISTER_ENUM(x, y) temp_receiver_p = &receive_event_##x;\
    DTAB_ADD(receivers_dtab, &temp_receiver_p, event_##x);
#include "names/events.h"
#undef REGISTER_ENUM

#define REGISTER_ENUM(x, y) temp_receiver_p = &receive_event_Input_##x;\
    DTAB_ADD(receivers_dtab, &temp_receiver_p, event_Input_##x);
#include "names/input.h"
#undef REGISTER_ENUM

#define REGISTER_ENUM(x) temp_index = x;\
    temp_receiver_p     = &receive_event_##x;\
    DTAB_ADD(receivers_dtab, &temp_receiver_p, temp_index);
#include "names/SDL_events.h"
#undef REGISTER_ENUM
    temp_index          = SDL_MOUSEBUTTONDOWN;
    temp_receiver_p     = &receive_event_SDL_MOUSEBUTTON;
    DTAB_ADD(receivers_dtab, &temp_receiver_p, temp_index);
    temp_index          = SDL_WINDOWEVENT;
    temp_receiver_p     = &receive_event_SDL_WINDOWEVENT;
    DTAB_ADD(receivers_dtab, &temp_receiver_p, temp_index);
    temp_index          = SDL_QUIT;
    temp_receiver_p     = &receive_event_SDL_QUIT;
    DTAB_ADD(receivers_dtab, &temp_receiver_p, temp_index);
    SOTA_Log_Func("%d\t%s\t" STRINGIZE(__LINE__), --call_stack_depth, __func__);
}

void Events_Manage(struct Game *sota) {
    SOTA_Log_FPS("%d\t%s\t" STRINGIZE(__LINE__), call_stack_depth++, __func__);
    SDL_assert(sota != NULL);
    SDL_Event event;

    while (SDL_PollEvent(&event)) {
        /* -- Getting receiver -- */
        u32 receiver_key = (event.type == SDL_USEREVENT) ? event.user.code : event.type;
        receiver_t *receiver = dtab_get(receivers_dtab, receiver_key);

        /* -- Calling receiver -- */
        if (receiver != NULL)
            (*receiver)(sota, &event);
    }
    SOTA_Log_FPS("%d\t%s\t" STRINGIZE(__LINE__), --call_stack_depth, __func__);
}
