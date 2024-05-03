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
tnecs_entity *data1_entity;
tnecs_entity *data2_entity;

void Events_Data_Malloc(void) {
    data1_entity = SDL_malloc(sizeof(tnecs_entity));
    data2_entity = SDL_malloc(sizeof(tnecs_entity));
}

void Events_Data_Free(void) {
    SDL_free(data1_entity);
    SDL_free(data2_entity);
}

tnecs_entity Events_Controllers_Check(struct Game *sota, i32 code) {
    tnecs_entity out_accepter_entity;
    struct controllerGamepad  *gamepad_ptr;
    struct controllerKeyboard *keyboard_ptr;
    switch (code) {
        case CONTROLLER_MOUSE:
            // SDL_Log("CONTROLLER_MOUSE");
            if (!sota->ismouse) {
                Event_Emit(__func__, SDL_USEREVENT, event_Mouse_Enable, NULL, NULL);
                // Event_Emit(__func__, SDL_USEREVENT, event_Cursor_Disable, NULL, NULL);
            }
            out_accepter_entity = sota->entity_mouse;
            break;
        case CONTROLLER_GAMEPAD:
            // SDL_Log("CONTROLLER_GAMEPAD");
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
            // SDL_Log("CONTROLLER_KEYBOARD");
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
            SDL_Log("CONTROLLER_TOUCHPAD");
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
    return (out_accepter_entity);
}

/* --- EVENT RECEIVERS --- */
void Event_Emit( const char *emitter, u32 type, i32 code, void *data1, void *data2) {
    SDL_assert(code > 0);
    s8 event_name = event_names[code - event_Start];
    SDL_Log("emitter -> %s, event -> %s", emitter, event_name.data);
    SDL_assert(type != ((UINT32_MAX) - 1));
    SDL_Event event;
    event.type          = type;
    event.user.code     = code;
    event.user.data1    = data1;
    event.user.data2    = data2;
    SDL_assert(SDL_PushEvent(&event));
}

void receive_event_Start(struct Game *sota, SDL_Event *userevent) {
}

void receive_event_End(struct Game *sota, SDL_Event *userevent) {
    sota->isrunning = false;
}

void receive_event_Load_Debug_Map(struct Game *sota, SDL_Event *userevent) {
    /* -- UNLOAD FirstMenu -- */
    Game_FirstMenu_Destroy(sota);
    Game_Title_Destroy(sota);

    /* -- LOAD Debug map -- */
    Game_debugMap_Load(sota);
    Game_Gameplay_Start(sota, GAME_STATE_Preparation, GAME_SUBSTATE_MENU);
    Utilities_DrawColor_Reset(sota->renderer);

    /* -- Music check -- */
    SDL_assert(sota->map->music_friendly != NULL);
}

void receive_event_Cursor_Moves(struct Game *sota, SDL_Event *userevent) {
    i32 controller_type = *(i32 *) userevent->user.data2;
    tnecs_entity mover_entity = Events_Controllers_Check(sota, controller_type);
    SDL_assert(mover_entity > 0);
    SDL_assert(userevent->user.data1 != NULL);

    /* Ignore event if cursor_move direction is wrong */
    sota->moved_direction = Ternary_Direction(sota->cursor_move);
    if (sota->moved_direction < 0)
        return;

    /* Play Cursor Soundfx */
#ifndef DEBUG_NO_SOUNDFX
    if (sota->soundfx_cursor) {
        Mix_PlayChannel(SOTA_SOUNDFX_CURSOR_CHANNEL, sota->soundfx_cursor, 0);
    }
#endif /* DEBUG_NO_SOUNDFX */

    if (fsm_eCrsMvs_s[sota->state] != NULL)
        fsm_eCrsMvs_s[sota->state](sota, mover_entity, &sota->cursor_move);

    if (fsm_eCrsMvs_ss[sota->substate] != NULL)
        fsm_eCrsMvs_ss[sota->substate](sota, mover_entity, &sota->cursor_move);

    Event_Emit(__func__, SDL_USEREVENT, event_Cursor_Moved, &sota->cursor_move,
               userevent->user.data2);
}

void receive_event_Cursor_Moved(struct Game *sota, SDL_Event *userevent) {
    SDL_assert(userevent->user.data2 != NULL);
    i32 controller_type = * (i32 *) userevent->user.data2;
    tnecs_entity mover_entity = Events_Controllers_Check(sota, controller_type);
    SDL_assert(mover_entity > 0);
    SDL_assert(userevent->user.data1 != NULL);
    struct Point *cursor_move = userevent->user.data1;

    SDL_assert(sota->entity_cursor != TNECS_NULL);

    if (fsm_eCrsMvd_s[sota->state] != NULL)
        fsm_eCrsMvd_s[sota->state](sota, mover_entity, cursor_move);

    // cursor_move->x = 0;
    // cursor_move->y = 0;
}

void receive_event_Input_CANCEL(struct Game *sota, SDL_Event *userevent) {
    i32 controller_type = * (i32 *) userevent->user.data1;
    tnecs_entity canceller_entity = Events_Controllers_Check(sota, controller_type);
    SDL_assert(canceller_entity > 0);
    if (fsm_eCncl_s[sota->state] != NULL)
        fsm_eCncl_s[sota->state](sota, canceller_entity);
}

void receive_event_Map_Win(struct Game *sota, SDL_Event *Map_Win) {
    SDL_Log("Map was won!");

    sota->map->win = true;
}

void receive_event_Map_Lose(struct Game *sota, SDL_Event *Map_Lose) {
    SDL_Log("Map was lost!");

    sota->map->loss = true;
}

void receive_event_Item_Get(struct Game *sota, SDL_Event *Map_Lose) {
}

void receive_event_Mouse_Disable(struct Game *sota, SDL_Event *Mouse_Disable) {
    Game_Mouse_Disable(sota);
}

void receive_event_Mouse_Enable(struct Game *sota, SDL_Event *Mouse_Enable) {
    Game_Mouse_Enable(sota);
}

void receive_event_Cursor_Enable(struct Game *sota, SDL_Event *Cursor_Enable) {
    Game_Cursor_Enable(sota);
}

void receive_event_Cursor_Disable(struct Game *sota, SDL_Event *Cursor_Disable) {
    Game_Cursor_Disable(sota);
}

void receive_event_Game_Control_Switch(struct Game *sota, SDL_Event *userevent) {
    i32 army = * (i32 *) userevent->user.data1;

    if (army == ARMY_FRIENDLY) {
        /* --- Control goes to player --- */
        Event_Emit(__func__, SDL_USEREVENT, event_Gameplay_Return2Standby, NULL, NULL);
        /* -- Turn only increments at the start of player turn -- */
        Map_Turn_Increment(sota->map);

        // Only if unit on tile
        struct Position *cursor_pos;
        cursor_pos = TNECS_GET_COMPONENT(sota->world, sota->entity_cursor, Position);
        struct Point pos = cursor_pos->tilemap_pos;
        int current_i  = pos.y * sota->map->col_len + pos.x;
        tnecs_entity ontile = sota->map->unitmap[current_i];

        /* unit hovering */
        if (ontile != TNECS_NULL) {
            *data2_entity = ontile;
            Event_Emit(__func__, SDL_USEREVENT, event_Cursor_Hovers_Unit, NULL, data2_entity);
        }
    } else {
        /* --- Control goes to AI --- */
        // TODO: Animate reinforcements
        if (sota->map->reinf_loaded < sota->map->turn) {
            Game_Map_Reinforcements_Load(sota);
        }

        /* -- Timer for reinforcements -- */
        sota->reinf_timer   = TNECS_ENTITY_CREATE_wCOMPONENTS(sota->world, Timer);
        struct Timer *timer = TNECS_GET_COMPONENT(sota->world, sota->reinf_timer, Timer);
        *timer = Timer_default;

#ifdef SOTA_PLAYER_CONTROLS_ENEMY
        /* --- Player control for enemy turn --- */
        Event_Emit(__func__, SDL_USEREVENT, event_Gameplay_Return2Standby, NULL, NULL);

#else /* SOTA_PLAYER_CONTROLS_ENEMY */
        /* --- AI control for enemy turn --- */
        /* -- Setting game substate -- */
        strncpy(sota->reason, "Ai control turn", sizeof(sota->reason));
        Game_subState_Set(sota, GAME_SUBSTATE_MAP_NPCTURN, sota->reason);
        AI_State_Turn_Start(&sota->ai_state);

#endif /* SOTA_PLAYER_CONTROLS_ENEMY */
    }
}

void receive_event_Input_STATS(struct Game *sota, SDL_Event *userevent) {
    SDL_assert(sota != NULL);
    i32 controller_type = * (i32 *) userevent->user.data1;
    tnecs_entity accepter_entity = Events_Controllers_Check(sota, controller_type);
    SDL_assert(accepter_entity > 0);
    *data1_entity = accepter_entity;
    if (fsm_eStats_ss[sota->substate] != NULL)
        fsm_eStats_ss[sota->substate](sota, accepter_entity);
}

void receive_event_Gameplay_Return2Standby(struct Game *sota, SDL_Event *userevent) {
    /* -- Popping all menus -- */
    b32 destroy = false;
    while (DARR_NUM(sota->menu_stack) > 0)
        Game_menuStack_Pop(sota, destroy);
    SDL_assert(DARR_NUM(sota->menu_stack) == 0);

    Game_cursorFocus_onMap(sota);

    /* --- Visible Popups --- */
    tnecs_entity popup_ent;
    struct PopUp *popup_ptr;
    /* -- Make Popup_Tile visible -- */
    if (popup_ent > TNECS_NULL) {
        popup_ent = sota->popups[POPUP_TYPE_HUD_TILE];
        popup_ptr = TNECS_GET_COMPONENT(sota->world, popup_ent, PopUp);
        SDL_assert(popup_ptr != NULL);
        popup_ptr->visible = true;
    }

    /* -- Make Popup_Unit visible -- */
    popup_ent = sota->popups[POPUP_TYPE_HUD_UNIT];
    if (popup_ent > TNECS_NULL) {
        popup_ptr = TNECS_GET_COMPONENT(sota->world, popup_ent, PopUp);
        SDL_assert(popup_ptr != NULL);
        popup_ptr->visible = true;
    }

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

    /* -- If map is won or loss, quit -- */
    if (sota->map->win)
        Event_Emit(__func__, SDL_USEREVENT, event_Scene_Play, NULL, NULL);

    if (sota->map->loss)
        Event_Emit(__func__, SDL_USEREVENT, event_Game_Over, NULL, NULL);
}

void receive_event_Scene_Play(struct Game *sota, SDL_Event *userevent) {
    /* --- Play scene --- */
    /* -- Removing unused stuff: menus, popups, map -- */
    /* - Hiding menus - */
    b32 destroy = false;
    while (DARR_NUM(sota->menu_stack) > 0)
        Game_menuStack_Pop(sota, destroy);

    /* - Hide Cursor - */
    struct Sprite *sprite = TNECS_GET_COMPONENT(sota->world, sota->entity_cursor, Sprite);
    SDL_assert(sprite != NULL);
    sprite->visible = false;

    /* - Hiding popups - */
    if (sota->popups[POPUP_TYPE_HUD_TILE] != TNECS_NULL) {
        Game_PopUp_Tile_Hide(sota);
    }
    if (sota->popups[POPUP_TYPE_HUD_UNIT] != TNECS_NULL) {
        Game_PopUp_Unit_Hide(sota);
    }

    /* - Remove unused components - */
    for (int i = 0; i < DARR_NUM(sota->map->units_onfield); i++) {
        tnecs_entity ent = sota->map->units_onfield[i];
        TNECS_REMOVE_COMPONENTS(sota->world, ent, MapHPBar);
        if (TNECS_ENTITY_HASCOMPONENT(sota->world, ent, RenderTop)) {
            TNECS_REMOVE_COMPONENTS(sota->world, ent, RenderTop);
        }

        if (TNECS_ENTITY_HASCOMPONENT(sota->world, ent, UnitMoveAnimation)) {
            TNECS_REMOVE_COMPONENTS(sota->world, ent, UnitMoveAnimation);
        }

        if (TNECS_ENTITY_HASCOMPONENT(sota->world, ent, Boss)) {
            TNECS_REMOVE_COMPONENTS(sota->world, ent, Boss);
        }
    }

    /* - Map_Free - */
    Game_Map_Free(sota);

    /* -- Creating scene to play -- */
    sota->scene = TNECS_ENTITY_CREATE_wCOMPONENTS(sota->world, Scene, Position,
                                                  Text, Timer);

    struct Scene *scene;
    scene  = TNECS_GET_COMPONENT(sota->world, sota->scene, Scene);
    *scene = Scene_default;
    scene->event = event_Quit;

    struct Timer *timer;
    timer  = TNECS_GET_COMPONENT(sota->world, sota->scene, Timer);
    *timer = Timer_default;

    /* TODO: remove when scene can actually play */
    struct Text *text;
    text  = TNECS_GET_COMPONENT(sota->world, sota->scene, Text);
    *text = Text_default;
    text->pixelfont         = sota->pixelnours_big;
    s8 line = s8_literal("You win!");
    Text_Set(text, line.data, PIXELNOURS_BIG_Y_OFFSET);
    SDL_assert((text->rect.w > 0) && (text->rect.h > 0));

    struct Position *position;
    position  = TNECS_GET_COMPONENT(sota->world, sota->scene, Position);
    *position = Position_default;
    position->onTilemap = false;
    position->scale[0] = 10.0f;
    position->scale[1] = 10.0f;
    position->pixel_pos.x = sota->settings.res.x / 2 - text->rect.w / 2 * position->scale[0];
    position->pixel_pos.y = sota->settings.res.y / 2;

    /* - Set state to scene - */
    strncpy(sota->reason, "Game plays scene", sizeof(sota->reason));
    Game_State_Set(sota, GAME_STATE_Scene_Talk, sota->reason);

    if (sota->substate != GAME_SUBSTATE_STANDBY) {
        strncpy(sota->reason, "Scene is playing", sizeof(sota->reason));
        Game_subState_Set(sota, GAME_SUBSTATE_STANDBY, sota->reason);
    }
}

void receive_event_Input_GLOBALRANGE(struct Game *sota, SDL_Event *userevent) {
    i32 controller_type = *(i32 *)userevent->user.data1;
    Events_Controllers_Check(sota, controller_type);

    if (fsm_eGlbRng_ss[sota->substate] != NULL)
        fsm_eGlbRng_ss[sota->substate](sota);
}

void receive_event_Input_ACCEPT(struct Game *sota, SDL_Event *userevent) {
    SDL_assert(sota);
    i32 controller_type = * (i32 *) userevent->user.data1;
    tnecs_entity accepter_entity = Events_Controllers_Check(sota, controller_type);
    SDL_assert(accepter_entity > 0);
    *data1_entity = accepter_entity;

    if (fsm_eAcpt_s[sota->state] != NULL)
        fsm_eAcpt_s[sota->state](sota, accepter_entity);
}

void receive_event_SDL_AUDIODEVICEADDED(struct Game *sota, SDL_Event *event) {
}

void receive_event_SDL_AUDIODEVICEREMOVED(struct Game *sota, SDL_Event *event) {
}

// SDL_CONTROLLERBUTTONDOWN emitted ONCE for the click
void receive_event_SDL_CONTROLLERBUTTONDOWN(struct Game *sota, SDL_Event *event) {
}

// SDL_KEYDOWN emitted unreliably/not every frame
void receive_event_SDL_KEYDOWN(struct Game *sota, SDL_Event *event) {
    // why is SDL_KEYDOWN unused?
    //  -> SDL_KEYDOWN is NOT emitted every frame
    // control system needs to run EVERY STEP
}

void receive_event_Quit(struct Game *sota, SDL_Event *event) {
    /* --- Quit gameplay, go back to start menu --- */
    strncpy(sota->reason, "Quitting gameplay", sizeof(sota->reason));
    Game_State_Set(sota, GAME_STATE_Title_Screen, sota->reason);
    if (sota->substate != GAME_SUBSTATE_MENU)
        Game_subState_Set(sota, GAME_SUBSTATE_MENU, sota->reason);

    /* - Show Cursor - */
    struct Sprite *sprite = TNECS_GET_COMPONENT(sota->world, sota->entity_cursor, Sprite);
    SDL_assert(sprite != NULL);
    sprite->visible = true;

    /* -- Destroying Scene -- */
    struct Scene *scene  = TNECS_GET_COMPONENT(sota->world, sota->scene, Scene);
    if (scene != NULL) {
        Scene_Free(scene);
        tnecs_entity_destroy(sota->world, sota->scene);
        sota->scene = TNECS_NULL;
    }

    /* -- Removing menus -- */
    b32 destroy = false;

    /* -- Hiding menus -- */
    while (DARR_NUM(sota->menu_stack) > 0)
        Game_menuStack_Pop(sota, destroy);

    /* -- Hiding popups -- */
    if (sota->popups[POPUP_TYPE_HUD_TILE] != TNECS_NULL) {
        Game_PopUp_Tile_Hide(sota);
    }
    if (sota->popups[POPUP_TYPE_HUD_UNIT] != TNECS_NULL) {
        Game_PopUp_Unit_Hide(sota);
    }

    /* -- Remove unused components -- */
    if (sota->map != NULL) {
        for (int i = 0; i < DARR_NUM(sota->map->units_onfield); i++) {
            tnecs_entity ent = sota->map->units_onfield[i];
            TNECS_REMOVE_COMPONENTS(sota->world, ent, MapHPBar);
            if (TNECS_ENTITY_HASCOMPONENT(sota->world, ent, RenderTop)) {
                TNECS_REMOVE_COMPONENTS(sota->world, ent, RenderTop);
            }

            if (TNECS_ENTITY_HASCOMPONENT(sota->world, ent, UnitMoveAnimation)) {
                TNECS_REMOVE_COMPONENTS(sota->world, ent, UnitMoveAnimation);
            }

            if (TNECS_ENTITY_HASCOMPONENT(sota->world, ent, Boss)) {
                TNECS_REMOVE_COMPONENTS(sota->world, ent, Boss);
            }
        }

        /* -- Map_Free -- */
        Game_Map_Free(sota);
    }

    /* -- Load TitleScreen -- */
    struct Input_Arguments args = Input_Arguments_default;
    Game_titleScreen_Load(sota, args);
    Game_cursorFocus_onMenu(sota);
}

void Reload_Entities_Archetype(struct Game *sota, entity_reload_f reload_func,
                               char *component) {
    tnecs_component component_flag;
    size_t flag_id;

    component_flag  = tnecs_component_names2typeflag(sota->world, 1, component);
    flag_id         = tnecs_typeflagid(sota->world, component_flag);

    Reload_Entities(sota, reload_func, flag_id, component);

    /* -- Reload entities for the all component archetypes -- */
    size_t num_archetypes = sota->world->num_archetype_ids[flag_id];
    for (size_t tsub = 0; tsub < num_archetypes; tsub++) {
        size_t archetype_id = sota->world->archetype_id_bytype[flag_id][tsub];
        Reload_Entities(sota, reload_func, archetype_id, component);
    }

}

void Reload_Entities(struct Game *sota, entity_reload_f reload_func, size_t flag_id,
                     char *component) {
    size_t num_entities = sota->world->num_entities_bytype[flag_id];

    for (size_t i = 0; i < num_entities; i++) {
        tnecs_entity entity = sota->world->entities_bytype[flag_id][i];
        size_t component_id   = tnecs_component_name2id(sota->world, component);
        void *struct_ptr      = tnecs_entity_get_component(sota->world, entity, component_id);
        reload_func(struct_ptr);
    }

}

void Reload_JSON(void *struct_ptr) {
    char **json_filename  = ((char **)struct_ptr + JSON_FILENAME_bOFFSET);
    jsonio_readJSON(s8_var(*json_filename), struct_ptr);
}

void Reload_Menu(void *struct_ptr) {
    struct Menu *mc     = struct_ptr;

    if (!mc->visible) {
        return;
    }

    void *menu          = mc->data;

    char *update_byte   = ((char *)menu + MENU_UPDATE_bOFFSET);
    b32  *update        = (b32 *)update_byte;

    *update      = true;
}

void Reload_Popup(void *struct_ptr) {
    struct PopUp *pc    = struct_ptr;

    if (!pc->visible) {
        return;
    }

    void *popup         = pc->data;

    char *update_byte   = ((char *)popup + MENU_UPDATE_bOFFSET);
    b32  *update        = (b32 *)update_byte;

    *update      = true;
}

void Reload_MapHpBar(void *struct_ptr) {
    struct MapHPBar *map_hp_bar = struct_ptr;
    map_hp_bar->update = true;
}

void receive_event_Music_Toggle(struct Game *sota, SDL_Event *event) {
    /* --- Toggle music --- */
    if (Mix_PlayingMusic() && !Mix_PausedMusic()) {
        Game_Music_Pause(sota);
    } else {
        Game_Music_Play(sota);
    }

    /* --- Blocking keyboard --- */
    struct controllerKeyboard *keyboard_ptr;
    keyboard_ptr = TNECS_GET_COMPONENT(sota->world, sota->entity_cursor, controllerKeyboard);
    keyboard_ptr->block_buttons = true;
}

void receive_event_Reload(struct Game *sota, SDL_Event *event) {
    /* --- Blocking keyboard --- */
    struct controllerKeyboard *keyboard_ptr;
    keyboard_ptr = TNECS_GET_COMPONENT(sota->world, sota->entity_cursor, controllerKeyboard);
    keyboard_ptr->block_buttons = true;

    /* --- Benchmarking reload time --- */
    u64 before_ns = tnecs_get_ns();

    /* --- Reload non-entities --- */
    Weapons_All_Reload(sota->weapons_dtab);
    Item_All_Reload(sota->items_dtab);

    /* --- Reload Unit --- */
    Reload_Entities_Archetype(sota, Reload_JSON, "Unit");

    /* --- Reload Unit entity's Sprite component --- */
    tnecs_component component_flag;
    size_t flag_id;

    component_flag  = tnecs_component_names2typeflag(sota->world, 2, "Sprite", "Unit");
    flag_id         = tnecs_typeflagid(sota->world, component_flag);

    Reload_Entities(sota, Reload_JSON, flag_id, "Sprite");

    /* --- Reload Popup and Menu --- */
    Reload_Entities_Archetype(sota, Reload_Popup, "PopUp");
    Reload_Entities_Archetype(sota, Reload_Menu,  "Menu");

    /* - Reload unit hpbars - */
    Reload_Entities_Archetype(sota, Reload_MapHpBar,  "MapHPBar");

    /* -- Reload Map -- */
    jsonio_readJSON(sota->map->json_filename, sota->map);
    sota->map->update = true;

    /* -- TODO: Reload Scenes -- */

    /* -- TODO: Reload Convoy -- */

    /* -- Benchmark reload time -- */
    u64 after_ns    = tnecs_get_ns();
    u64 elapsed_ms  = (after_ns - before_ns) / SOTA_us;
    float frame     = (float)(sota->instant_fps * elapsed_ms) / SOTA_ms;
    SDL_Log("Reload %d ms %f frames", elapsed_ms, frame);

}

void receive_event_SDL_QUIT(struct Game *sota, SDL_Event *event) {
    sota->isrunning = false;
}

void receive_event_SDL_CONTROLLERDEVICEREMOVED(struct Game *sota, SDL_Event *event) {
    // event->cdevice.which of DEVICEREMOVED is different from DEVICEADDED!
    // DEVICEREMOVED-> JoystickInstanceID which increments for every joystick

    SDL_assert(sota->entity_cursor != TNECS_NULL);

    struct controllerGamepad *gamepad_ptr;
    gamepad_ptr = TNECS_GET_COMPONENT(sota->world, sota->entity_cursor, controllerGamepad);
    if (gamepad_ptr != NULL)
        Gamepad_removeController(gamepad_ptr, event->cdevice.which);
    else
        SDL_Log("entity_cursor has no controllerGamepad component");

}

void receive_event_SDL_JOYDEVICEADDED(struct Game *sota, SDL_Event *event) {
}

void receive_event_SDL_JOYDEVICEREMOVED(struct Game *sota, SDL_Event *event) {
}

void receive_event_SDL_CONTROLLERDEVICEADDED(struct Game *sota, SDL_Event *event) {
    // event->cdevice.which of DEVICEREMOVED is different from DEVICEADDED!
    // DEVICEADDED-> JoystickDeviceID which is stable e.g. 0 for player 1, etc.

    if (sota->entity_cursor == TNECS_NULL) {
        SDL_Log("Entity_cursor is not valid");
        return;
    }

    struct controllerGamepad *gamepad_ptr;
    gamepad_ptr = TNECS_GET_COMPONENT(sota->world, sota->entity_cursor, controllerGamepad);
    if (gamepad_ptr != NULL)
        Gamepad_addController(gamepad_ptr, event->cdevice.which);
    else
        SDL_Log("entity_cursor has no controllerGamepad component");
}

void receive_event_SDL_MOUSEMOTION(struct Game *sota, SDL_Event *event) {
    SDL_assert(event);
    if (sota->runtime_ns <= CURSOR_FIRSTMENU_PAUSE_ns) {
        SDL_LogVerbose(SOTA_LOG_FPS, "Sota first init pause.");
        return;
    }
    if (event->motion.windowID != SDL_GetWindowID(sota->window)) {
        SDL_LogVerbose(SOTA_LOG_FPS, "Wrong window");
        return;
    }
    tnecs_entity mouse = sota->entity_mouse;
    if (sota->entity_mouse == TNECS_NULL) {
        SDL_LogVerbose(SOTA_LOG_FPS, "Mouse disabled");
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
}

// SDL_MOUSEBUTTON emitted ONCE for the CLICK
void receive_event_SDL_MOUSEBUTTON(struct Game *sota, SDL_Event *event) {
    SDL_assert(sota->window != NULL);
    SDL_assert(event        != NULL);
    SDL_assert(sota->entity_mouse > 0);

    if (event->motion.windowID != SDL_GetWindowID(sota->window)) {
        SDL_LogVerbose(SOTA_LOG_FPS, "Wrong window");
        return;
    }

    struct controllerMouse *mouse;
    if (event->button.state == SDL_PRESSED) {
        mouse = TNECS_GET_COMPONENT(sota->world, sota->entity_mouse, controllerMouse);
        Mouse_checkButton(mouse, event->button.button);
    }
}

void receive_event_Turn_Begin(struct Game *sota, SDL_Event *userevent) {
    struct Map *map = sota->map;
    SDL_assert(sota->state == GAME_STATE_Gameplay_Map);

    /* - Refresh all units - */
    // TODO: Move to turn end? Make units colored again
    for (int i = 0; i < DARR_NUM(map->units_onfield); i++) {
        if (map->units_onfield[i] != TNECS_NULL) {
            Game_Unit_Refresh(sota, map->units_onfield[i]);
        }
    }

    i32 *army = &sota->map->army_onfield[sota->map->army_i];

    /* Switch control to next army */
    Event_Emit(__func__, SDL_USEREVENT, event_Game_Control_Switch, army, NULL);
}

void receive_event_Turn_Transition(struct Game *sota, SDL_Event *userevent) {
    tnecs_entity turn_transition;
    turn_transition = TNECS_ENTITY_CREATE_wCOMPONENTS(sota->world, MapAnimation, Position, Text, Timer);

    struct Timer *timer;
    timer  = TNECS_GET_COMPONENT(sota->world, turn_transition, Timer);
    *timer = Timer_default;

    // TODO: How to do a fancy animation?
    // 1- draw animation pixel art
    // 2- Modify MapAnimation component
    // 3- Implement animation checker in Map_TurnTransition_Animate
    struct MapAnimation *map_anim;
    map_anim  = TNECS_GET_COMPONENT(sota->world, turn_transition, MapAnimation);
    *map_anim = MapAnimation_default;
    map_anim->time_ns = SOTA_SOUNDFX_NEXT_TURN_DURATION_ms * SOTA_us;
    map_anim->anim = &Map_TurnTransition_Animate;

    /* Get Army name */
    SDL_assert(DARR_NUM(sota->map->army_onfield) > 1);
    SDL_assert(sota->map->army_onfield[0] == ARMY_FRIENDLY);

    Map_Army_Next(sota->map);
    SDL_assert(sota->map->army_i >= 0);
    SDL_assert(sota->map->army_i < DARR_NUM(sota->map->army_onfield));
    i8 army      = sota->map->army_onfield[sota->map->army_i];
    SDL_assert(army >= 0);
    SDL_assert(army < ARMY_NUM);
    s8 army_name = armyNames[army];

    /* -- Change music -- */
    Game_Music_Stop(sota);
    if (SotA_isPC(army)) {
        sota->music = sota->map->music_friendly;
        SDL_assert(sota->music != NULL);
    } else {
        sota->music = sota->map->music_enemy;
    }
    Game_Music_Play(sota);

    /* -- Play Turn Transition -- */
#ifndef DEBUG_NO_SOUNDFX
    if (sota->soundfx_next_turn) {
        Mix_PlayChannel(-1, sota->soundfx_next_turn, 0);
    }
#endif /* DEBUG_NO_SOUNDFX */

    struct Text *text;
    text  = TNECS_GET_COMPONENT(sota->world, turn_transition, Text);
    *text = Text_default;
    text->pixelfont         = sota->pixelnours_big;
    s8 line = s8_mut(army_name.data);
    line = s8cat(line, s8_literal(" Turn"));
    Text_Set(text, line.data, PIXELNOURS_BIG_Y_OFFSET);
    SDL_assert((text->rect.w > 0) && (text->rect.h > 0));
    s8_free(&line);

    struct Position *position;
    position  = TNECS_GET_COMPONENT(sota->world, turn_transition, Position);
    *position = Position_default;
    position->onTilemap = false;
    position->scale[0] = 10.0f;
    position->scale[1] = 10.0f;
    position->pixel_pos.x = sota->settings.res.x / 2 - text->rect.w / 2 * position->scale[0];
    position->pixel_pos.y = sota->settings.res.y / 2;

    strncpy(sota->reason, "Turn transition is an animation", sizeof(sota->reason));
    Game_subState_Set(sota, GAME_SUBSTATE_MAP_ANIMATION, sota->reason);
}

void receive_event_Turn_End(struct Game *sota, SDL_Event *userevent) {
    /* - Pop all menus - */
    while (DARR_NUM(sota->menu_stack) > 0) {
        tnecs_entity menu_pop       = DARR_POP(sota->menu_stack);
        struct Menu *mc             = TNECS_GET_COMPONENT(sota->world, menu_pop, Menu);
        SDL_assert(mc           != NULL);
        SDL_assert(mc->elem_pos != NULL);
        mc->visible = false;
    }

    /* - Focus cursor on tilemap - */
    Game_cursorFocus_onMap(sota);

    /* - Destroy reinforcement timer - */
    if (sota->reinf_timer != TNECS_NULL) {
        tnecs_entity_destroy(sota->world, sota->reinf_timer);
        sota->reinf_timer = TNECS_NULL;
    }

    /* - Reset dangermap - */
    Map_Stacked_Dangermap_Reset(sota->map);
    Map_Danger_Reset(sota->map);
    Map_Palettemap_Reset(sota->map);

    /* - Decrement persistent auras for all units - */
    i32 army = sota->map->army_onfield[sota->map->army_i];
    Map_Bonus_Remove_Persistent(sota->map, army);

    Event_Emit(__func__, SDL_USEREVENT, event_Turn_Transition, NULL, NULL);
}

void receive_event_Unit_Enters_Shop(struct Game *sota, SDL_Event *userevent) {
}

void receive_event_Unit_Enters_Village(struct Game *sota, SDL_Event *userevent) {
}

void receive_event_Unit_Enters_Armory(struct Game *sota, SDL_Event *userevent) {
}

void receive_event_Unit_Select(struct Game *sota, SDL_Event *userevent) {
    sota->selected_unit_entity = *((tnecs_entity *) userevent->user.data2);
    sota->aggressor = sota->selected_unit_entity;
    SDL_assert(sota->selected_unit_entity > TNECS_NULL);
    SDL_assert(sota->state                == GAME_STATE_Gameplay_Map);
    if (fsm_eUnitSel_ss[sota->substate] != NULL)
        fsm_eUnitSel_ss[sota->substate](sota, sota->entity_cursor);
}

void receive_event_Unit_Deselect(struct Game *sota, SDL_Event *userevent) {
    SDL_assert(sota->entity_cursor != TNECS_NULL);
    sota->aggressor = TNECS_NULL;
    sota->defendant = TNECS_NULL;

    if (sota->selected_unit_entity == TNECS_NULL) {
        return;
    }

    tnecs_entity unit_ent     = sota->selected_unit_entity;
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
            tnecs_entity *ent_ptr = &sota->selected_unit_entity;
            Event_Emit(__func__, SDL_USEREVENT, event_Unit_Danger, NULL, ent_ptr);
        }
    }

    sota->selected_unit_entity = TNECS_NULL;
}

void receive_event_Unit_Entity_Return(struct Game *sota, SDL_Event *userevent) {
}

void receive_event_Unit_Icon_Return(struct Game *sota, SDL_Event *userevent) {

    /* - Hide overlay/movemap - */
    sota->map->show_overlay = false;
    tnecs_entity returning = sota->selected_unit_entity;
    SDL_assert(returning > TNECS_NULL);

    struct Position *pos_ptr  = TNECS_GET_COMPONENT(sota->world, returning, Position);
    struct Sprite *sprite = TNECS_GET_COMPONENT(sota->world, sota->selected_unit_entity, Sprite);
    SDL_assert(pos_ptr != NULL);
    SDL_assert(sprite != NULL);

    /* - Resetting map animation loop - */
    struct Point initial = sota->selected_unit_initial_position;
    SDL_assert(sprite->spritesheet != NULL);
    if (sprite->spritesheet->loop_num == MAP_UNIT_LOOP_NUM) {
        if ((initial.x == pos_ptr->tilemap_pos.x) && (initial.y == pos_ptr->tilemap_pos.y))
            Spritesheet_Loop_Set(sprite->spritesheet, MAP_UNIT_LOOP_TAUNT, sprite->flip);
        else
            Spritesheet_Loop_Set(sprite->spritesheet, MAP_UNIT_LOOP_IDLE, sprite->flip);
        Sprite_Animation_Loop(sprite);
    }
    Sprite_Draw(sprite, sota->renderer);

    /* - Returning to initial position - */
    pos_ptr->tilemap_pos = initial;
    pos_ptr->pixel_pos.x = initial.x * pos_ptr->scale[0];
    pos_ptr->pixel_pos.y = initial.y * pos_ptr->scale[1];
}

void receive_event_Unit_Moves(struct Game *sota, SDL_Event *userevent) {
    /* Setup for MAP_UNIT_MOVES state */
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
    if (sprite->spritesheet->loop_num == MAP_UNIT_LOOP_NUM) {
        // TODO: check if arrow exists and is long, revert to correct orientation
        if (sprite->flip == SDL_FLIP_HORIZONTAL)
            Spritesheet_Loop_Set(sprite->spritesheet, MAP_UNIT_LOOP_MOVEL, sprite->flip);
        else
            Spritesheet_Loop_Set(sprite->spritesheet, MAP_UNIT_LOOP_MOVER, sprite->flip);
    }
    Sprite_Animation_Loop(sprite);
    Sprite_Draw(sprite, sota->renderer);
}

void receive_event_Cursor_Hovers_Unit(struct Game *sota, SDL_Event *userevent) {
    sota->hovered_unit_entity = *(tnecs_entity *)userevent->user.data2;
    SDL_assert(sota->hovered_unit_entity != TNECS_NULL);
    struct Unit *temp = TNECS_GET_COMPONENT(sota->world, sota->hovered_unit_entity, Unit);
    SDL_assert(temp->name.data != NULL);

    if (fsm_eCrsHvUnit_ss[sota->substate] != NULL)
        fsm_eCrsHvUnit_ss[sota->substate](sota, sota->hovered_unit_entity);
}

void receive_event_Cursor_Dehovers_Unit(struct Game *sota, SDL_Event *userevent) {
    tnecs_entity dehovered_unit_entity = sota->hovered_unit_entity;
    sota->hovered_unit_entity            = TNECS_NULL;

    if (fsm_eCrsDeHvUnit_ss[sota->substate] != NULL)
        fsm_eCrsDeHvUnit_ss[sota->substate](sota, dehovered_unit_entity);
}

void receive_event_Units_Refresh(struct Game *sota, SDL_Event *userevent) {
}

void receive_event_Unit_Danger(struct Game *sota, SDL_Event *userevent) {
    SDL_assert(sota->entity_cursor != TNECS_NULL);

    if (fsm_eUnitDng_ss[sota->substate] != NULL)
        fsm_eUnitDng_ss[sota->substate](sota, sota->entity_cursor);
}

void receive_event_Unit_Dance(struct Game *sota, SDL_Event *userevent) {
}

void receive_event_Menu_Select(struct Game *sota, SDL_Event *userevent) {

}

// Menu_Created event should be the ONLY EVENT that changes game substate to menu
void receive_event_Menu_Created(struct Game *sota, SDL_Event *userevent) {
    tnecs_entity menu_entity = *(tnecs_entity *)userevent->user.data1;

    /* - Set sprite to combat stance - */
    // Note: Map Action menu does not select unit
    if ((menu_entity == sota->item_select_menu) && (sota->selected_unit_entity)) {
        struct Sprite *sprite;
        sprite = TNECS_GET_COMPONENT(sota->world, sota->selected_unit_entity, Sprite);

        SDL_assert(sprite->spritesheet != NULL);
        SDL_assert(sprite->spritesheet->loop_num == MAP_UNIT_LOOP_NUM);
        Spritesheet_Loop_Set(sprite->spritesheet, MAP_UNIT_LOOP_STANCE, sprite->flip);
        Sprite_Animation_Loop(sprite);
        Sprite_Draw(sprite, sota->renderer);
    }

    // Note: reason set by event sender is kinda dumb.
    // reason might get overwritten...
    if (sota->substate != GAME_SUBSTATE_MENU)
        Game_subState_Set(sota, GAME_SUBSTATE_MENU, sota->reason);

}

void receive_event_Loadout_Selected(struct Game *sota, SDL_Event *userevent) {
    /* Aggressor loadout was selected, time to select defendant. */

    /* - Turn menu_attack invisible - */
    int stack_top               = DARR_NUM(sota->menu_stack) - 1;
    tnecs_entity menu_top     = sota->menu_stack[stack_top];
    struct Menu *mc    = TNECS_GET_COMPONENT(sota->world, menu_top, Menu);
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
}

void receive_event_Input_ZOOM_IN(struct Game *sota, SDL_Event *userevent) {
    /* -- Check: Only Zoom_in on the map -- */
    SDL_assert(sota->state == GAME_STATE_Gameplay_Map);
    b32 correct_substate = (sota->substate == GAME_SUBSTATE_STANDBY);
    correct_substate |= (sota->substate == GAME_SUBSTATE_MAP_CANDIDATES);
    correct_substate |= (sota->substate == GAME_SUBSTATE_MAP_UNIT_MOVES);
    correct_substate |= (sota->substate == GAME_SUBSTATE_MAP_ANIMATION);
    SDL_assert(correct_substate);

    i32 controller_type = *(i32 *)userevent->user.data1;
    Events_Controllers_Check(sota, controller_type);

    /* -- Zoom in to sprite_atorigin -- */
    struct Sprite *sprite_atorigin;
    struct Sprite *cursor_sprite;
    struct Sprite *mouse_sprite;
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

}

void receive_event_Input_ZOOM_OUT(struct Game *sota, SDL_Event *userevent) {
    /* -- Check: Only Zoom_out on the map -- */
    SDL_assert(sota->state == GAME_STATE_Gameplay_Map);
    b32 correct_substate = (sota->substate == GAME_SUBSTATE_STANDBY);
    correct_substate |= (sota->substate == GAME_SUBSTATE_MAP_CANDIDATES);
    correct_substate |= (sota->substate == GAME_SUBSTATE_MAP_UNIT_MOVES);
    correct_substate |= (sota->substate == GAME_SUBSTATE_MAP_ANIMATION);
    SDL_assert(correct_substate);

    i32 controller_type = *(i32 *)userevent->user.data1;
    Events_Controllers_Check(sota, controller_type);

    /* -- Zoom out to sprite_atorigin -- */
    struct Sprite *cursor_sprite;
    struct Sprite *mouse_sprite;
    struct Sprite *sprite_atorigin;
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

}

void receive_event_Input_UP(    struct Game *sota, SDL_Event *userevent) {}
void receive_event_Input_LEFT(  struct Game *sota, SDL_Event *userevent) {}
void receive_event_Input_DOWN(  struct Game *sota, SDL_Event *userevent) {}
void receive_event_Input_RIGHT( struct Game *sota, SDL_Event *userevent) {}

void receive_event_Input_MENURIGHT(struct Game *sota, SDL_Event *userevent) {
    i32 controller_type = *(i32 *)userevent->user.data1;
    Events_Controllers_Check(sota, controller_type);

    if (fsm_eMenuRight_s[sota->state] != NULL)
        fsm_eMenuRight_s[sota->state](sota, controller_type);
}

void receive_event_Input_MENULEFT(struct Game *sota, SDL_Event *userevent) {
    i32 controller_type = *(i32 *)userevent->user.data1;
    Events_Controllers_Check(sota, controller_type);

    if (fsm_eMenuLeft_s[sota->state] != NULL)
        fsm_eMenuLeft_s[sota->state](sota, controller_type);
}

void receive_event_Input_MINIMAP(struct Game *sota, SDL_Event *userevent) {
    i32 controller_type = *(i32 *)userevent->user.data1;
    Events_Controllers_Check(sota, controller_type);

}

void receive_event_Input_FAST_FORWARD(struct Game *sota, SDL_Event *userevent) {
    i32 controller_type = *(i32 *)userevent->user.data1;
    Events_Controllers_Check(sota, controller_type);
    sota->fast_forward = !sota->fast_forward;
}

void receive_event_Input_PAUSE(struct Game *sota, SDL_Event *userevent) {
    /* TODO: REMOVE -> NO NEED FOR PAUSE */
    i32 controller_type = *(i32 *)userevent->user.data1;
    Events_Controllers_Check(sota, controller_type);

    if (fsm_eStart_s[sota->state] != NULL)
        fsm_eStart_s[sota->state](sota, controller_type);
}

void receive_event_Unit_Seize(struct Game *sota, SDL_Event *userevent) {

}

void receive_event_Game_Over(struct Game *sota, SDL_Event *userevent) {
    /* --- TODO: Start map Game over animation --- */
    tnecs_entity game_over;
    game_over = TNECS_ENTITY_CREATE_wCOMPONENTS(sota->world, MapAnimation, Position, Text, Timer);

    struct Timer *timer;
    timer  = TNECS_GET_COMPONENT(sota->world, game_over, Timer);
    *timer = Timer_default;

    // TODO: fancy game over screen with animation?
    struct MapAnimation *map_anim;
    map_anim  = TNECS_GET_COMPONENT(sota->world, game_over, MapAnimation);
    *map_anim = MapAnimation_default;
    map_anim->time_ns = SOTA_SOUNDFX_NEXT_TURN_DURATION_ms * SOTA_us;
    map_anim->anim = &Map_GameOver_Animate;

    /* -- Change music -- */
    // TODO: PLay game over music
    // Game_Music_Stop(sota);
    // sota->music = sota->map->music_friendly;
    // Game_Music_Play(sota);

    struct Text *text;
    text  = TNECS_GET_COMPONENT(sota->world, game_over, Text);
    *text = Text_default;
    text->pixelfont = sota->pixelnours_big;
    s8 line = s8_literal("Game Over");
    Text_Set(text, line.data, PIXELNOURS_BIG_Y_OFFSET);
    SDL_assert((text->rect.w > 0) && (text->rect.h > 0));

    struct Position *position;
    position  = TNECS_GET_COMPONENT(sota->world, game_over, Position);
    *position = Position_default;
    position->onTilemap = false;
    position->scale[0] = 10.0f;
    position->scale[1] = 10.0f;
    position->pixel_pos.x = sota->settings.res.x / 2 - text->rect.w / 2 * position->scale[0];
    position->pixel_pos.y = sota->settings.res.y / 2;

    strncpy(sota->reason, "Game Over is an animation", sizeof(sota->reason));
    Game_subState_Set(sota, GAME_SUBSTATE_MAP_ANIMATION, sota->reason);
}

void receive_event_Unit_Refresh(struct Game *sota, SDL_Event *userevent) {

}

void receive_event_Unit_Wait(struct Game *sota, SDL_Event *userevent) {

    /* -- Preliminaries -- */
    tnecs_entity unit_ent = sota->selected_unit_entity;
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

}

void receive_event_Unit_Talk(struct Game *sota, SDL_Event *userevent) {
    SDL_assert(sota->entity_cursor          > TNECS_NULL);
    SDL_assert(sota->selected_unit_entity   > TNECS_NULL);
    tnecs_entity unit_ent = sota->selected_unit_entity;
    Game_Unit_Wait(sota, unit_ent);

    Event_Emit(__func__, SDL_USEREVENT, event_Gameplay_Return2Standby, NULL, NULL);
}

void receive_event_Unit_Rescue(struct Game *sota, SDL_Event *userevent) {
    SDL_assert(sota->entity_cursor          > TNECS_NULL);
    SDL_assert(sota->selected_unit_entity   > TNECS_NULL);
    tnecs_entity unit_ent = sota->selected_unit_entity;
    Game_Unit_Wait(sota, unit_ent);

    Event_Emit(__func__, SDL_USEREVENT, event_Gameplay_Return2Standby, NULL, NULL);
}

void receive_event_Combat_Start(struct Game *sota, SDL_Event *userevent) {

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
    struct Point *agg_pos     = &agg_posc->tilemap_pos;
    struct Point *dft_pos     = &dft_posc->tilemap_pos;

    struct Point move = {dft_pos->x - agg_pos->x, dft_pos->y - agg_pos->y};
    move.x = move.x < -1 ? -1 : move.x;
    move.x = move.x >  1 ?  1 : move.x;
    move.y = move.y < -1 ? -1 : move.y;
    move.y = move.y >  1 ?  1 : move.y;
    i8 direction = Ternary_Direction(move);
    /* -- reset animation states -- */

    switch (direction) {
        case SOTA_DIRECTION_BOTTOM:
        case SOTA_DIRECTION_BOTLEFT:
        case SOTA_DIRECTION_BOTRIGHT:
            Sprite_Animation_Restart(agg_sprite, MAP_UNIT_LOOP_ATTACKB);
            Sprite_Animation_Restart(dft_sprite, MAP_UNIT_LOOP_ATTACKB);
            break;
        case SOTA_DIRECTION_TOP:
            Sprite_Animation_Restart(agg_sprite, MAP_UNIT_LOOP_ATTACKT);
            Sprite_Animation_Restart(dft_sprite, MAP_UNIT_LOOP_ATTACKT);
            break;
        case SOTA_DIRECTION_RIGHT:
        case SOTA_DIRECTION_TOPRIGHT:
            Sprite_Animation_Restart(agg_sprite, MAP_UNIT_LOOP_ATTACKR);
            Sprite_Animation_Restart(dft_sprite, MAP_UNIT_LOOP_ATTACKL);
            break;
        case SOTA_DIRECTION_LEFT:
        case SOTA_DIRECTION_TOPLEFT:
            Sprite_Animation_Restart(agg_sprite, MAP_UNIT_LOOP_ATTACKL);
            Sprite_Animation_Restart(dft_sprite, MAP_UNIT_LOOP_ATTACKR);
            break;
    }

    struct Timer *agg_timer = TNECS_GET_COMPONENT(sota->world, sota->aggressor, Timer);
    struct Timer *dft_timer = TNECS_GET_COMPONENT(sota->world, sota->defendant, Timer);
    agg_timer->paused = true;
    dft_timer->paused = true;

    /* -- Create combat animation entity -- */
    // TODO change to MapAnimation component
    tnecs_entity combat_animation;
    combat_animation = TNECS_ENTITY_CREATE_wCOMPONENTS(sota->world, Timer, CombatAnimation);

    struct CombatAnimation *combat_anim;
    combat_anim  = TNECS_GET_COMPONENT(sota->world, combat_animation, CombatAnimation);
    SDL_assert(combat_anim != NULL);
    *combat_anim = CombatAnimation_default;

    struct Timer *timer = TNECS_GET_COMPONENT(sota->world, combat_animation, Timer);
    SDL_assert(combat_anim != NULL);
    *timer = Timer_default;
    timer->time_ns = 0;

    /* - Hide pre combat popup, if it exists - */
    struct Menu *mc;
    mc = TNECS_GET_COMPONENT(sota->world, sota->pre_combat_popup, Menu);
    if (mc != NULL)
        mc->visible = false;

    /* - Show map combat popup - */
    if (sota->popups[POPUP_TYPE_MAP_COMBAT] == TNECS_NULL)
        Game_PopUp_Map_Combat_Create(sota);
    Game_PopUp_Map_Combat_Update(sota);

    struct Unit *aggressor = TNECS_GET_COMPONENT(sota->world, sota->aggressor, Unit);
    struct Unit *defendant = TNECS_GET_COMPONENT(sota->world, sota->defendant, Unit);
    struct PopUp *popup    = TNECS_GET_COMPONENT(sota->world, sota->popups[POPUP_TYPE_MAP_COMBAT],
                                                 PopUp);
    struct PopUp_Map_Combat *pmc = popup->data;

    PopUp_Map_Combat_Units(pmc, sota, aggressor, defendant, agg_pos, dft_pos);
    popup->visible = true;
    /* - Deselect weapons in LoadoutSelectMenu, if it exists - */
    mc = TNECS_GET_COMPONENT(sota->world, sota->weapon_select_menu, Menu);
    if (mc != NULL) {
        struct LoadoutSelectMenu *wsm = mc->data;
        LoadoutSelectMenu_Select_Reset(wsm);
    }

    /* - Set previous candidate to NULL - */
    sota->previous_candidate = -1;
}

void receive_event_Combat_End(struct Game *sota, SDL_Event *userevent) {
    Event_Emit(__func__, SDL_USEREVENT, event_Unit_Wait, NULL, NULL);

    // 1. Resolve Combat
    struct Unit *aggressor = TNECS_GET_COMPONENT(sota->world, sota->aggressor, Unit);
    aggressor->waits = true;
    aggressor->waits = true;
    struct Unit *defendant = TNECS_GET_COMPONENT(sota->world, sota->defendant, Unit);

    tnecs_entity popup_ent     = sota->popups[POPUP_TYPE_MAP_COMBAT];
    struct PopUp *popup_ptr      = TNECS_GET_COMPONENT(sota->world, popup_ent, PopUp);
    struct PopUp_Map_Combat *pmc = popup_ptr->data;

    /* 2. Update maphpbars. */
    // Unit might be dead, only update if maphpbar exists
    struct MapHPBar *map_hp_bar = TNECS_GET_COMPONENT(sota->world, sota->aggressor, MapHPBar);
    if (map_hp_bar != NULL) {
        map_hp_bar->update  = true;
        map_hp_bar->visible = true;
    }

    map_hp_bar = TNECS_GET_COMPONENT(sota->world, sota->defendant, MapHPBar);
    if (map_hp_bar != NULL) {
        map_hp_bar->update  = true;
        map_hp_bar->visible = true;
    }

    // 3. reset animation states
    struct Sprite *dft_sprite = TNECS_GET_COMPONENT(sota->world, sota->defendant, Sprite);
    struct Sprite *agg_sprite = TNECS_GET_COMPONENT(sota->world, sota->aggressor, Sprite);
    Sprite_Animation_Restart(agg_sprite, MAP_UNIT_LOOP_IDLE);
    Sprite_Animation_Restart(dft_sprite, MAP_UNIT_LOOP_IDLE);

    // 4. Restart Defendant animation
    struct Timer *dft_timer = TNECS_GET_COMPONENT(sota->world, sota->defendant, Timer);
    dft_timer->paused = false;

    // 5. Hide PopUp_Map_Combat
    Game_PopUp_Map_Combat_Hide(sota);

    // 6. Revert hovered entity to aggressor
    sota->hovered_unit_entity = sota->aggressor;

    // 7. Return to standby
    *data1_entity = sota->entity_cursor;
    Event_Emit(__func__, SDL_USEREVENT, event_Gameplay_Return2Standby, NULL, NULL);
}

void receive_event_Defendant_Select(struct Game *sota, SDL_Event *userevent) {
    Game_PopUp_Pre_Combat_Hide(sota);
    /* --- Start Combat --- */
    // Necessary criteria:
    //  - sota->aggressor
    //  - sota->defendant
    Event_Emit(__func__, SDL_USEREVENT, event_Combat_Start, data1_entity, data2_entity);
}

void receive_event_Unit_Trade(struct Game *sota, SDL_Event *userevent) {
    SDL_assert(sota->entity_cursor > TNECS_NULL);
    if (sota->selected_unit_entity == TNECS_NULL)
        return;

    Game_Unit_Wait(sota, sota->selected_unit_entity);

    Event_Emit(__func__, SDL_USEREVENT, event_Gameplay_Return2Standby, NULL, NULL);
}

void receive_event_Unit_Escape(struct Game *sota, SDL_Event *userevent) {
    SDL_assert(sota->entity_cursor > TNECS_NULL);
    if (sota->selected_unit_entity == TNECS_NULL)
        return;

    Game_Unit_Wait(sota, sota->selected_unit_entity);

    Event_Emit(__func__, SDL_USEREVENT, event_Gameplay_Return2Standby, NULL, NULL);
}

void receive_event_Unit_Staff(struct Game *sota, SDL_Event *userevent) {
}

void receive_event_Unit_Items(struct Game *sota, SDL_Event *userevent) {
    SDL_assert(sota->entity_cursor > TNECS_NULL);
    SDL_assert(sota->selected_unit_entity != TNECS_NULL);

    Game_Unit_Wait(sota, sota->selected_unit_entity);

    Event_Emit(__func__, SDL_USEREVENT, event_Gameplay_Return2Standby, NULL, NULL);
}

void receive_event_Unit_Heals(struct Game *sota, SDL_Event *userevent) {
}

void receive_event_Convoy_Check(struct Game *sota, SDL_Event *userevent) {
}

void receive_event_Convoy_Map(struct Game *sota, SDL_Event *userevent) {
}

void receive_event_Unit_Dies(struct Game *sota, SDL_Event *userevent) {
    /* --- PRELIMINARIES --- */
    tnecs_entity victim_entity = *(tnecs_entity *) userevent->user.data1;
    tnecs_entity killer_entity = *(tnecs_entity *) userevent->user.data2;
    SDL_assert(victim_entity > TNECS_NULL);
    SDL_assert(killer_entity > TNECS_NULL);
    struct Unit *killer = TNECS_GET_COMPONENT(sota->world, killer_entity, Unit);
    struct Unit *victim = TNECS_GET_COMPONENT(sota->world, victim_entity, Unit);
    struct Boss *boss   = TNECS_GET_COMPONENT(sota->world, victim_entity, Boss);
    SDL_assert(killer != NULL);
    SDL_assert(victim != NULL);

    /* --- Increasing Killer's regrets --- */
    int regrets = killer->regrets;
    killer->regrets = regrets > UINT8_MAX - REGRET_KILL ? UINT8_MAX : regrets + REGRET_KILL;

    /* --- Removing unit from map --- */
    SDL_assert(sota->map->world == sota->world);
    Map_Unit_Remove(sota->map, victim_entity);

    /* --- Making unit sprite invisible --- */
    struct Sprite *sprite = TNECS_GET_COMPONENT(sota->world, victim_entity, Sprite);
    sprite->visible = false;

    /* --- Remove MapHPbar --- */
    if (TNECS_ENTITY_HASCOMPONENT(sota->world, victim_entity, MapHPBar))
        TNECS_REMOVE_COMPONENTS(sota->world, victim_entity, MapHPBar);

    /* Components changed place, need to reload */
    victim = TNECS_GET_COMPONENT(sota->world, victim_entity, Unit);
    boss   = TNECS_GET_COMPONENT(sota->world, victim_entity, Boss);

    /* --- Deleting entity? --- */
    // - Put unit entity in list of killed units
    /* --- Check Map conditions --- */
    Map_Conditions_Check_Death(sota->map->death_enemy,      sota->map,
                               victim,                      boss, sota);
    Map_Conditions_Check_Death(sota->map->death_friendly,   sota->map,
                               victim,                      boss, sota);
}

void receive_event_Unit_Loots(struct Game *sota, SDL_Event *userevent) {
    tnecs_entity looter_entity = *(tnecs_entity *) userevent->user.data1;
    tnecs_entity victim_entity = *(tnecs_entity *) userevent->user.data2;
    struct Unit *looter = TNECS_GET_COMPONENT(sota->world, looter_entity, Unit);
    struct Unit *victim = TNECS_GET_COMPONENT(sota->world, victim_entity, Unit);
    int regrets = looter->regrets;
    looter->regrets = regrets > UINT8_MAX - REGRET_LOOT ? UINT8_MAX : regrets + REGRET_LOOT;
}

void receive_event_Increment_Attack(struct Game *sota, SDL_Event *userevent) {
    /* -- Popup_Map_Combat -- */
    tnecs_entity popup_ent = sota->popups[POPUP_TYPE_MAP_COMBAT];
    struct PopUp *popup_ptr  = TNECS_GET_COMPONENT(sota->world, popup_ent, PopUp);
    struct PopUp_Map_Combat *pmc = popup_ptr->data;
    pmc->aggressor = TNECS_GET_COMPONENT(sota->world, sota->aggressor, Unit);
    pmc->defendant = TNECS_GET_COMPONENT(sota->world, sota->defendant, Unit);

    // 1. Get next HP of attacked unit
    struct Combat_Attack attack = sota->combat_outcome.attacks[pmc->current_attack++];
    struct Unit *attacker, *defender;
    attacker = attack.attacker ? pmc->aggressor : pmc->defendant;
    defender = attack.attacker ? pmc->defendant : pmc->aggressor;
    Combat_Resolve_Attack(attack, attacker, defender);

    // 2. Check for unit agony/death
    b32 agg_death = (!pmc->aggressor->alive) || (pmc->aggressor->agony >= 0);
    if (agg_death) {
        Event_Emit(__func__, SDL_USEREVENT, event_Unit_Dies, &sota->aggressor, &sota->defendant);
    }

    b32 dft_death = (!pmc->defendant->alive) || (pmc->defendant->agony >= 0);
    if (dft_death) {
        Event_Emit(__func__, SDL_USEREVENT, event_Unit_Dies, &sota->defendant, &sota->aggressor);
    }
    /* Only one of combatants can die */
    SDL_assert(!(dft_death && agg_death));

    sota->combat_outcome.ended = (agg_death || dft_death);

    SDL_assert(popup_ptr != NULL);
    SDL_assert(pmc != NULL);
    SDL_assert(sota->aggressor != TNECS_NULL);
    SDL_assert(sota->defendant != TNECS_NULL);
    pmc->update = true;
}

void receive_event_Unit_Agonizes(struct Game *sota, SDL_Event *userevent) {
    tnecs_entity victor_entity = *(tnecs_entity *) userevent->user.data1;
    tnecs_entity victim_entity = *(tnecs_entity *) userevent->user.data2;
    struct Unit *victor = TNECS_GET_COMPONENT(sota->world, victor_entity, Unit);
    struct Unit *victim = TNECS_GET_COMPONENT(sota->world, victim_entity, Unit);
    int regrets = victor->regrets;
    victor->regrets = regrets > UINT8_MAX - REGRET_LOOT ? UINT8_MAX : regrets + REGRET_LOOT;
}

/* --- EVENT UTILITIES --- */
void receive_event_SDL_WINDOWEVENT(struct Game *sota, SDL_Event *event) {
    if (event->window.windowID != SDL_GetWindowID(sota->window))
        return;

    switch (event->window.event) {
        case SDL_WINDOWEVENT_CLOSE:
            SDL_Log("SDL_WINDOWEVENT_CLOSE");
            Event_Emit(__func__, SDL_QUIT, event_SDL_QUIT, NULL, NULL);
            break;
    }

}

void Events_Names_Declare(void) {
    event_Start = SDL_RegisterEvents(1);
#define REGISTER_ENUM(x, y) event_##x = SDL_RegisterEvents(1);
#include "names/events.h"
#undef REGISTER_ENUM
#define REGISTER_ENUM(x, y)  event_Input_##x = SDL_RegisterEvents(1);
#include "names/input.h"
#undef REGISTER_ENUM
    event_End = SDL_RegisterEvents(1);
}

void Events_Names_Free(void) {
    if (event_names == NULL)
        return;

    for (size_t i = 0; i < ((event_End - event_Start) + 1); i++) {
        s8_free(&event_names[i]);
    }
    SDL_free(event_names);
}

s8 *event_names = NULL;
void Events_Names_Alloc(void) {
    SDL_Log("event_End event_Start %d %d", event_End, event_Start);
    SDL_Log("event_End - event_Start %d", event_End - event_Start);
    event_names = calloc((event_End - event_Start) + 1, sizeof(*event_names));
    SDL_assert(event_names != NULL);

#define REGISTER_ENUM(x, y)event_names[(event_##x - event_Start)] = s8_toUpper(s8_mut(#x));
#include "names/events.h"
#undef REGISTER_ENUM

#define REGISTER_ENUM(x, y) event_names[(event_Input_##x - event_Start)] = s8_toUpper(s8_mut(#x));
#include "names/input.h"
#undef REGISTER_ENUM

}

void Events_Receivers_Free(void) {
    if (receivers_dtab != NULL) {
        DTAB_FREE(receivers_dtab);
        receivers_dtab = NULL;
    }
}

void Events_Receivers_Declare(void) {
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
}

void Events_Manage(struct Game *sota) {
    SDL_assert(sota != NULL);
    SDL_Event event;

    /* Note: events emitted during SDL_PollEvent loop don't get polled.
        -> Prevents infinite event loops.
    How to ensure events occur on same frame without triggering infinite loop?
        -> Run receiver directly instead of emitting event.
    */
    while (SDL_PollEvent(&event)) {
        /* -- Getting receiver -- */
        u32 receiver_key = (event.type == SDL_USEREVENT) ? event.user.code : event.type;
        receiver_t *receiver = dtab_get(receivers_dtab, receiver_key);

        /* -- Calling receiver -- */
        if (receiver != NULL)
            (*receiver)(sota, &event);
    }
}
