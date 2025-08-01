
#include "events.h"
#include "reload.h"
#include "fsm.h"
#include "weapon.h"
#include "octant.h"
#include "names.h"
#include "log.h"
#include "position.h"
#include "utilities.h"
#include "combat.h"
#include "debug.h"
#include "game/popup.h"
#include "cutscene.h"
#include "noursclock.h"
#include "nmath.h"
#include "sprite.h"
#include "music.h"
#include "bars/map_hp.h"
#include "map/animation.h"
#include "map/render.h"
#include "map/ontile.h"
#include "unit/equipment.h"
#include "unit/loadout.h"
#include "popup/popup.h"
#include "popup/map_combat.h"
#include "menu/loadout_select.h"
#include "pixelfonts.h"
#include "game/combat.h"
#include "game/game.h"
#include "game/menu.h"
#include "game/map.h"
#include "game/cursor.h"
#include "game/unit.h"
#include "unit/unit.h"
#include "unit/flags.h"
#include "scene.h"
#include "convoy.h"
#include "arrow.h"
#include "menu/menu.h"
#include "map/conditions.h"
#include "macros.h"
#include "AI.h"
#include "controller/keyboard.h"
#include "controller/mouse.h"
#include "controller/gamepad.h"
#include "map/path.h"
#include "text.h"
#include "globals.h"

/* --- RECEIVERS DECLARATION --- */
struct dtab *receivers_dtab = NULL;

u32 event_Start = 0;
#define REGISTER_ENUM(x, y) u32 event_##x;
#include "names/events.h"
#undef REGISTER_ENUM
#define REGISTER_ENUM(x, y) u32 event_Input_##x;
#include "names/input.h"
#undef REGISTER_ENUM
u32 event_End = 0;

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
            if (!sota->flags.ismouse) {
                Event_Emit(__func__, SDL_USEREVENT, event_Mouse_Enable, NULL, NULL);
                // Event_Emit(__func__, SDL_USEREVENT, event_Cursor_Disable, NULL, NULL);
            }
            out_accepter_entity = sota->mouse.entity;
            break;
        case CONTROLLER_GAMEPAD:
            // SDL_Log("CONTROLLER_GAMEPAD");
            if (!sota->flags.iscursor) {
                Event_Emit(__func__, SDL_USEREVENT, event_Mouse_Disable, NULL, NULL);
                Event_Emit(__func__, SDL_USEREVENT, event_Cursor_Enable, NULL, NULL);
            }
            out_accepter_entity = sota->cursor.entity;
            gamepad_ptr  = IES_GET_COMPONENT(gl_world, sota->cursor.entity, controllerGamepad);
            keyboard_ptr = IES_GET_COMPONENT(gl_world, sota->cursor.entity, controllerKeyboard);
            gamepad_ptr->block_buttons        = true;
            keyboard_ptr->block_buttons       = true;
            gamepad_ptr->timeheld_button_ns   = SOTA_ns / sota->settings.FPS.cap;
            keyboard_ptr->timeheld_button_ns  = SOTA_ns / sota->settings.FPS.cap;
            break;
        case CONTROLLER_KEYBOARD:
            // SDL_Log("CONTROLLER_KEYBOARD");
            if (!sota->flags.iscursor) {
                Event_Emit(__func__, SDL_USEREVENT, event_Mouse_Disable, NULL, NULL);
                Event_Emit(__func__, SDL_USEREVENT, event_Cursor_Enable, NULL, NULL);
            }
            out_accepter_entity = sota->cursor.entity;
            gamepad_ptr  = IES_GET_COMPONENT(gl_world, sota->cursor.entity, controllerGamepad);
            keyboard_ptr = IES_GET_COMPONENT(gl_world, sota->cursor.entity, controllerKeyboard);
            gamepad_ptr->block_buttons        = true;
            keyboard_ptr->block_buttons       = true;
            gamepad_ptr->timeheld_button_ns   = SOTA_ns / sota->settings.FPS.cap;
            keyboard_ptr->timeheld_button_ns  = SOTA_ns / sota->settings.FPS.cap;
            break;
        case CONTROLLER_TOUCHPAD:
            SDL_Log("CONTROLLER_TOUCHPAD");
            if (!sota->flags.iscursor) {
                Event_Emit(__func__, SDL_USEREVENT, event_Mouse_Disable, NULL, NULL);
                Event_Emit(__func__, SDL_USEREVENT, event_Cursor_Enable, NULL, NULL);
            }
            out_accepter_entity = sota->cursor.entity;
            gamepad_ptr  = IES_GET_COMPONENT(gl_world, sota->cursor.entity, controllerGamepad);
            keyboard_ptr = IES_GET_COMPONENT(gl_world, sota->cursor.entity, controllerKeyboard);
            gamepad_ptr->block_buttons        = true;
            keyboard_ptr->block_buttons       = true;
            gamepad_ptr->timeheld_button_ns   = SOTA_ns / sota->settings.FPS.cap;
            keyboard_ptr->timeheld_button_ns  = SOTA_ns / sota->settings.FPS.cap;
            break;
        default: {
            printf("controller code is invalid.\n");
            // exit(1);
        }
    }
    return (out_accepter_entity);
}

/* --- EVENT RECEIVERS --- */
void Event_Emit( const char *emitter, u32 type, i32 code, void *data1, void *data2) {
    SDL_assert(code > 0);
    // s8 event_name = event_names[code - event_Start];
    // SDL_Log("emitter -> %s, event -> %s", emitter, event_name.data);
    SDL_assert(type != ((UINT32_MAX) - 1));
    SDL_Event event;
    SDL_zero(event);
    event.type          = type;
    event.user.code     = code;
    event.user.data1    = data1;
    event.user.data2    = data2;
    SDL_assert(SDL_PushEvent(&event));
}

void receive_event_Start(struct Game *sota, SDL_Event *userevent) {
}

void receive_event_End(struct Game *sota, SDL_Event *userevent) {
    /* --- Game stops running. --- */
    sota->flags.isrunning = false;
}

void receive_event_Load_Debug_Map(struct Game *sota, SDL_Event *userevent) {
    /* -- UNLOAD FirstMenu -- */
    Game_FirstMenu_Destroy(sota);
    Game_Title_Destroy(sota);

    /* -- LOAD Debug map -- */
    Game_Save_Load(sota, SOTA_SAVE_DEBUG_0);
    Game_Gameplay_Start(sota, GAME_STATE_Preparation, GAME_SUBSTATE_MENU);
    Utilities_DrawColor_Reset(sota->render.er);

    /* -- Music check -- */
    Map *map = Game_Map(sota);
    SDL_assert(map->music.friendly != NULL);
}

void receive_event_Cursor_Moves(struct Game *sota, SDL_Event *userevent) {
    i32 controller_type = *(i32 *) userevent->user.data2;
    SDL_assert(
            (controller_type == CONTROLLER_MOUSE)       ||
            (controller_type == CONTROLLER_GAMEPAD)     ||
            (controller_type == CONTROLLER_KEYBOARD)    ||
            (controller_type == CONTROLLER_TOUCHPAD)
    );
    tnecs_entity mover_entity = Events_Controllers_Check(sota, controller_type);
    SDL_assert(mover_entity > 0);
    SDL_assert(userevent->user.data1 != NULL);

    /* Ignore event if cursor_move direction is wrong */
    sota->cursor.moved_direction = Ternary_Direction(sota->cursor.move);
    if (sota->cursor.moved_direction < 0)
        return;

    /* Play Cursor Soundfx */
#ifndef DEBUG_NO_SOUNDFX
    if (sota->audio.soundfx_cursor) {
        Mix_PlayChannel(SOTA_SOUNDFX_CURSOR_CHANNEL, sota->audio.soundfx_cursor, 0);
    }
#endif /* DEBUG_NO_SOUNDFX */

    if (fsm_eCrsMvs_s[Game_State_Current(sota)] != NULL)
        fsm_eCrsMvs_s[Game_State_Current(sota)](sota, mover_entity, &sota->cursor.move);

    if (fsm_eCrsMvs_ss[Game_Substate_Current(sota)] != NULL)
        fsm_eCrsMvs_ss[Game_Substate_Current(sota)](sota, mover_entity, &sota->cursor.move);

    Event_Emit(__func__, SDL_USEREVENT, event_Cursor_Moved, &sota->cursor.move,
               userevent->user.data2);
}

void receive_event_Cursor_Moved(struct Game *sota, SDL_Event *userevent) {
    SDL_assert(userevent->user.data2 != NULL);
    i32 controller_type = * (i32 *) userevent->user.data2;
    SDL_assert(
            (controller_type == CONTROLLER_MOUSE)       ||
            (controller_type == CONTROLLER_GAMEPAD)     ||
            (controller_type == CONTROLLER_KEYBOARD)    ||
            (controller_type == CONTROLLER_TOUCHPAD)
    );

    tnecs_entity mover_entity = Events_Controllers_Check(sota, controller_type);
    SDL_assert(mover_entity > 0);
    SDL_assert(userevent->user.data1 != NULL);
    struct Point *cursor_move = userevent->user.data1;

    SDL_assert(sota->cursor.entity != TNECS_NULL);

    if (fsm_eCrsMvd_s[Game_State_Current(sota)] != NULL)
        fsm_eCrsMvd_s[Game_State_Current(sota)](sota, mover_entity, cursor_move);

    // cursor_move->x = 0;
    // cursor_move->y = 0;
}

void receive_event_Input_CANCEL(struct Game *sota, SDL_Event *userevent) {
    i32 controller_type = * (i32 *) userevent->user.data2;
    SDL_assert(
            (controller_type == CONTROLLER_MOUSE)       ||
            (controller_type == CONTROLLER_GAMEPAD)     ||
            (controller_type == CONTROLLER_KEYBOARD)    ||
            (controller_type == CONTROLLER_TOUCHPAD)
    );

    tnecs_entity canceller_entity = Events_Controllers_Check(sota, controller_type);
    SDL_assert(canceller_entity > 0);
    if (fsm_eCncl_s[Game_State_Current(sota)] != NULL)
        fsm_eCncl_s[Game_State_Current(sota)](sota, canceller_entity);
}

void receive_event_Map_Win(struct Game *sota, SDL_Event *Map_Win) {
    // SDL_Log("Map was won!");
    Map *map = Game_Map(sota);
    map->flags.win = true;
}

void receive_event_Map_Lose(struct Game *sota, SDL_Event *Map_Lose) {
    // SDL_Log("Map was lost!");
    Map *map = Game_Map(sota);
    map->flags.loss = true;
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

    Map *map = Game_Map(sota);
    if (army == ARMY_FRIENDLY) {
        /* --- Control goes to player --- */
        Event_Emit(__func__, SDL_USEREVENT, event_Gameplay_Return2Standby, NULL, NULL);

        /* -- Remove AI entity -- */
        if (sota->ai.control != TNECS_NULL) {
            tnecs_entity_destroy(gl_world, sota->ai.control);
        }

        /* -- Turn only increments at the start of player turn -- */
        Map_Turn_Increment(map);

        /* Only if unit on tile */
        const struct Position *cursor_pos;
        cursor_pos = IES_GET_COMPONENT(gl_world, sota->cursor.entity, Position);
        struct Point pos    = cursor_pos->tilemap_pos;
        int current_i       = pos.y * Map_col_len(map) + pos.x;
        tnecs_entity ontile = map->darrs.unitmap[current_i];

        /* unit hovering */
        // Note: Cursor_Hovers should be sent after Return2Standby
        if (ontile != TNECS_NULL) {
            *data2_entity = ontile;
            Event_Emit(__func__, SDL_USEREVENT, event_Cursor_Hovers_Unit, NULL, data2_entity);
        }

    } else {
        /* --- Control goes to AI --- */
        SDL_assert(sota->ai.control == TNECS_NULL);
        sota->ai.control = TNECS_ENTITY_CREATE_wCOMPONENTS(gl_world, AI_Control_ID);

        // TODO: Animate reinforcements
        if (map->reinforcements.loaded < map->turn) {
            Game_Map_Reinforcements_Load(sota);
        }

        /* -- Timer for reinforcements -- */
        SDL_assert(sota->timers.reinf == TNECS_NULL);
        sota->timers.reinf   = TNECS_ENTITY_CREATE_wCOMPONENTS(gl_world, Timer_ID);
        struct Timer *timer = IES_GET_COMPONENT(gl_world, sota->timers.reinf, Timer);
        *timer = Timer_default;

#ifdef SOTA_PLAYER_CONTROLS_ENEMY
        /* --- Player control for enemy turn --- */
        Event_Emit(__func__, SDL_USEREVENT, event_Gameplay_Return2Standby, NULL, NULL);

#else /* SOTA_PLAYER_CONTROLS_ENEMY */
        /* --- AI control for enemy turn --- */
        /* -- Setting game substate -- */
        strncpy(sota->debug.reason, "Ai control turn", sizeof(sota->debug.reason));
        Game_subState_Set(sota, GAME_SUBSTATE_MAP_NPCTURN, sota->debug.reason);
        Game_AI_Turn_Start(&sota->ai);
        // TODO: create AI entity

#endif /* SOTA_PLAYER_CONTROLS_ENEMY */
    }
}

void receive_event_Input_STATS(struct Game *sota, SDL_Event *userevent) {
    SDL_assert(sota != NULL);

    i32 controller_type = * (i32 *) userevent->user.data2;
    SDL_assert(
            (controller_type == CONTROLLER_MOUSE)       ||
            (controller_type == CONTROLLER_GAMEPAD)     ||
            (controller_type == CONTROLLER_KEYBOARD)    ||
            (controller_type == CONTROLLER_TOUCHPAD)
    );

    tnecs_entity accepter_entity = Events_Controllers_Check(sota, controller_type);
    SDL_assert(accepter_entity > 0);
    *data1_entity = accepter_entity;
    if (fsm_eStats_s[Game_State_Current(sota)] != NULL)
        fsm_eStats_s[Game_State_Current(sota)](sota, accepter_entity);
}

void receive_event_Gameplay_Return2Standby(struct Game *sota, SDL_Event *userevent) {
    /* -- Popping all menus -- */
    b32 destroy = false;
    while (DARR_NUM(sota->menus.stack) > 0)
        Game_menuStack_Pop(sota, destroy);
    SDL_assert(DARR_NUM(sota->menus.stack) == 0);

    Game_cursorFocus_onMap(sota);

    /* --- Visible Popups --- */
    tnecs_entity popup_ent;
    struct PopUp *popup_ptr;
    /* -- Make Popup_Tile visible -- */
    popup_ent = sota->popups.arr[POPUP_TYPE_HUD_TILE];
    if (popup_ent > TNECS_NULL) {
        popup_ptr = IES_GET_COMPONENT(gl_world, popup_ent, PopUp);
        SDL_assert(popup_ptr != NULL);
        popup_ptr->visible = true;
    }

    /* -- Make Popup_Unit visible -- */
    popup_ent = sota->popups.arr[POPUP_TYPE_HUD_UNIT];
    if (popup_ent > TNECS_NULL) {
        popup_ptr = IES_GET_COMPONENT(gl_world, popup_ent, PopUp);
        SDL_assert(popup_ptr != NULL);
        popup_ptr->visible = true;
    }

    /* --- Invisible Popups --- */
    /* -- Popup_Loadout_stats -- */
    popup_ent = sota->popups.arr[POPUP_TYPE_HUD_LOADOUT_STATS];
    if (popup_ent > TNECS_NULL) {
        popup_ptr = IES_GET_COMPONENT(gl_world, popup_ent, PopUp);
        SDL_assert(popup_ptr != NULL);
        popup_ptr->visible = false;
    }

    /* -- FSM -- */
    if (fsm_eGmp2Stby_ss[Game_Substate_Current(sota)] != NULL)
        fsm_eGmp2Stby_ss[Game_Substate_Current(sota)](sota, TNECS_NULL);
    if (fsm_eGmp2Stby_s[Game_State_Current(sota)] != NULL)
        fsm_eGmp2Stby_s[Game_State_Current(sota)](sota, TNECS_NULL);

    /* -- Setting game substate -- */
    if (Game_Substate_Current(sota) != GAME_SUBSTATE_STANDBY) {
        strncpy(sota->debug.reason, "Game_Substate_Current(sota) returns to Standby",
                sizeof(sota->debug.reason));
        Game_subState_Set(sota, GAME_SUBSTATE_STANDBY, sota->debug.reason);
    }

    /* -- If map is won or loss, quit -- */
    Map *map = Game_Map(sota);
    if (Map_isWon(map)) {
        Event_Emit(__func__, SDL_USEREVENT, event_Scene_Play, NULL, NULL);
    }

    if (Map_isLost(map)) {
        Event_Emit(__func__, SDL_USEREVENT, event_Game_Over, NULL, NULL);
    }
}

void receive_event_Scene_Play(struct Game *sota, SDL_Event *userevent) {
    /* --- Play scene --- */
    /* -- Removing unused stuff: menus, popups, map -- */
    /* - Hiding menus - */
    b32 destroy = false;
    while (DARR_NUM(sota->menus.stack) > 0)
        Game_menuStack_Pop(sota, destroy);

    /* - Hide Cursor - */
    struct Sprite *sprite = IES_GET_COMPONENT(gl_world, sota->cursor.entity, Sprite);
    SDL_assert(sprite != NULL);
    sprite->visible = false;

    /* - Hiding popups - */
    if (sota->popups.arr[POPUP_TYPE_HUD_TILE] != TNECS_NULL) {
        Game_PopUp_Tile_Hide(sota);
    }
    if (sota->popups.arr[POPUP_TYPE_HUD_UNIT] != TNECS_NULL) {
        Game_PopUp_Unit_Hide(sota);
    }

    /* - Remove unused components - */
    Map *map = Game_Map(sota);
    for (int i = 0; i < DARR_NUM(map->units.onfield.arr); i++) {
        tnecs_entity ent = map->units.onfield.arr[i];
        TNECS_REMOVE_COMPONENTS(gl_world, ent, MapHPBar_ID);
        if (IES_ENTITY_HASCOMPONENT(gl_world, ent, RenderTop)) {
            TNECS_REMOVE_COMPONENTS(gl_world, ent, RenderTop_ID);
        }

        if (IES_ENTITY_HASCOMPONENT(gl_world, ent, UnitMoveAnimation)) {
            TNECS_REMOVE_COMPONENTS(gl_world, ent, UnitMoveAnimation_ID);
        }

        if (IES_ENTITY_HASCOMPONENT(gl_world, ent, Boss)) {
            TNECS_REMOVE_COMPONENTS(gl_world, ent, Boss_ID);
        }
    }

    /* - Map_Free - */
    Game_Map_Free(sota);

    /* -- Creating scene to play -- */
    // TODO: play Scene, OR cutscene?
    sota->narrative.scene = TNECS_ENTITY_CREATE_wCOMPONENTS(gl_world, Scene_ID,
                                                            Position_ID, Text_ID, Timer_ID);

    struct Scene *scene  = IES_GET_COMPONENT(gl_world, sota->narrative.scene, Scene);
    SDL_assert(scene != NULL);
    *scene = Scene_default;
    scene->event = event_Quit;

    struct Timer *timer;
    timer  = IES_GET_COMPONENT(gl_world, sota->narrative.scene, Timer);
    *timer = Timer_default;

    /* TODO: remove when scene can actually play */
    struct Text *text;
    text  = IES_GET_COMPONENT(gl_world, sota->narrative.scene, Text);
    Text_Init(text);
    P_Text_Init(text, sota->render.er);
    SDL_assert(text->plat != NULL);
    text->pixelfont         = sota->fonts.pixelnours_big;
    s8 line = s8_literal("You win!");
    Text_Set(text, line.data, PIXELNOURS_BIG_Y_OFFSET);
    SDL_assert((text->size.x > 0) && (text->size.y > 0));

    struct Position *position;
    position  = IES_GET_COMPONENT(gl_world, sota->narrative.scene, Position);
    *position = Position_default;
    position->onTilemap = false;
    position->scale[0] = 10.0f;
    position->scale[1] = 10.0f;
    position->pixel_pos.x = sota->settings.res.x / 2 - text->size.x / 2 * position->scale[0];
    position->pixel_pos.y = sota->settings.res.y / 2;

    /* - Set state to cutscene - */
    strncpy(sota->debug.reason, "Game plays scene", sizeof(sota->debug.reason));
    Game_State_Set(sota, GAME_STATE_Scene_Talk, sota->debug.reason);

    if (Game_Substate_Current(sota) != GAME_SUBSTATE_STANDBY) {
        strncpy(sota->debug.reason, "Scene is playing", sizeof(sota->debug.reason));
        Game_subState_Set(sota, GAME_SUBSTATE_STANDBY, sota->debug.reason);
    }
}

void receive_event_Input_GLOBALRANGE(struct Game *sota, SDL_Event *userevent) {
    i32 controller_type = * (i32 *) userevent->user.data2;
    SDL_assert(
            (controller_type == CONTROLLER_MOUSE)       ||
            (controller_type == CONTROLLER_GAMEPAD)     ||
            (controller_type == CONTROLLER_KEYBOARD)    ||
            (controller_type == CONTROLLER_TOUCHPAD)
    );

    Events_Controllers_Check(sota, controller_type);

    if (fsm_eGlbRng_ss[Game_Substate_Current(sota)] != NULL)
        fsm_eGlbRng_ss[Game_Substate_Current(sota)](sota);
}

void receive_event_Input_ACCEPT(struct Game *sota, SDL_Event *userevent) {
    SDL_assert(sota);
    SDL_assert(userevent->user.data2 != NULL);
    i32 controller_type = * (i32 *) userevent->user.data2;
    SDL_assert(
            (controller_type == CONTROLLER_MOUSE)       ||
            (controller_type == CONTROLLER_GAMEPAD)     ||
            (controller_type == CONTROLLER_KEYBOARD)    ||
            (controller_type == CONTROLLER_TOUCHPAD)
    );

    tnecs_entity accepter_entity = Events_Controllers_Check(sota, controller_type);
    SDL_assert(accepter_entity > 0);
    *data1_entity = accepter_entity;

    if (fsm_eAcpt_s[Game_State_Current(sota)] != NULL)
        fsm_eAcpt_s[Game_State_Current(sota)](sota, accepter_entity);
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
    strncpy(sota->debug.reason, "Quitting gameplay", sizeof(sota->debug.reason));
    Game_State_Set(sota, GAME_STATE_Title_Screen, sota->debug.reason);
    if (Game_Substate_Current(sota) != GAME_SUBSTATE_MENU)
        Game_subState_Set(sota, GAME_SUBSTATE_MENU, sota->debug.reason);

    /* - Show Cursor - */
    struct Sprite *sprite = IES_GET_COMPONENT(gl_world, sota->cursor.entity, Sprite);
    SDL_assert(sprite != NULL);
    sprite->visible = true;

    /* -- Destroying Cutscene -- */
    struct Cutscene *cutscene  = IES_GET_COMPONENT(gl_world, sota->narrative.cutscene, Cutscene);
    if (cutscene != NULL) {
        Cutscene_Free(cutscene);
        tnecs_entity_destroy(gl_world, sota->narrative.cutscene);
        sota->narrative.cutscene = TNECS_NULL;
    }

    /* -- Destroying scene -- */
    struct Scene *scene  = IES_GET_COMPONENT(gl_world, sota->narrative.scene, Scene);
    if (scene != NULL) {
        Scene_Free(scene);
        tnecs_entity_destroy(gl_world, sota->narrative.scene);
        sota->narrative.scene = TNECS_NULL;
    }

    /* -- Removing menus -- */
    b32 destroy = false;

    /* -- Hiding menus -- */
    while (DARR_NUM(sota->menus.stack) > 0)
        Game_menuStack_Pop(sota, destroy);

    /* -- Hiding popups -- */
    if (sota->popups.arr[POPUP_TYPE_HUD_TILE] != TNECS_NULL) {
        Game_PopUp_Tile_Hide(sota);
    }
    if (sota->popups.arr[POPUP_TYPE_HUD_UNIT] != TNECS_NULL) {
        Game_PopUp_Unit_Hide(sota);
    }

    /* -- Remove unused components -- */
    Map *map = Game_Map(sota);
    if (map != NULL) {
        for (int i = 0; i < DARR_NUM(map->units.onfield.arr); i++) {
            tnecs_entity ent = map->units.onfield.arr[i];
            TNECS_REMOVE_COMPONENTS(gl_world, ent, MapHPBar_ID);
            if (IES_ENTITY_HASCOMPONENT(gl_world, ent, RenderTop)) {
                TNECS_REMOVE_COMPONENTS(gl_world, ent, RenderTop_ID);
            }

            if (IES_ENTITY_HASCOMPONENT(gl_world, ent, UnitMoveAnimation)) {
                TNECS_REMOVE_COMPONENTS(gl_world, ent, UnitMoveAnimation_ID);
            }

            if (IES_ENTITY_HASCOMPONENT(gl_world, ent, Boss)) {
                TNECS_REMOVE_COMPONENTS(gl_world, ent, Boss_ID);
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

void receive_event_Music_Toggle(struct Game *sota, SDL_Event *event) {
    /* --- Toggle music --- */
    if (Mix_PlayingMusic() && !Mix_PausedMusic()) {
        Game_Music_Pause(sota);
    } else {
        Game_Music_Play(sota);
    }

    /* --- Blocking keyboard --- */
    struct controllerKeyboard *keyboard_ptr;
    keyboard_ptr = IES_GET_COMPONENT(gl_world, sota->cursor.entity, controllerKeyboard);
    keyboard_ptr->block_buttons = true;
}

void receive_event_Reload(struct Game *sota, SDL_Event *event) {
    /* --- Blocking keyboard --- */
    SDL_Log("receive_event_Reload");
    struct controllerKeyboard *keyboard_ptr;
    keyboard_ptr = IES_GET_COMPONENT(gl_world, sota->cursor.entity, controllerKeyboard);
    keyboard_ptr->block_buttons = true;

    /* --- Benchmarking reload time --- */
    u64 before_ns = nours_get_ns();

    /* --- Reload non-entities --- */
    Weapons_All_Reload(gl_weapons_dtab);
    Item_All_Reload(gl_items_dtab);

    /* --- Reload Unit --- */
    Reload_Entities_Archetype(sota, Reload_JSON, "Unit");

    /* --- Reload Unit entity's Sprite component --- */
    tnecs_component component_flag;
    size_t flag_id;

    component_flag  = tnecs_component_ids2archetype(2, Sprite_ID, Unit_ID);
    flag_id         = tnecs_archetypeid(gl_world, component_flag);

    Reload_Entities(sota, Reload_JSON, flag_id, "Sprite");

    /* --- Reload Popup and Menu --- */
    Reload_Entities_Archetype(sota, Reload_Popup, "PopUp");
    Reload_Entities_Archetype(sota, Reload_Menu,  "Menu");

    /* - Reload unit hpbars - */
    Reload_Entities_Archetype(sota, Reload_MapHpBar,  "MapHPBar");

    /* -- Reload Map -- */
    Map *map = Game_Map(sota);
    jsonio_readJSON(map->jsonio_header.json_filename, map);
    map->flags.update = true;

    /* -- TODO: Reload Scenes -- */

    /* -- TODO: Reload Convoy -- */

    /* -- Benchmark reload time -- */
    u64 after_ns    = nours_get_ns();
    u64 elapsed_ms  = (after_ns - before_ns) / SOTA_us;
    float frame     = (float)(sota->fps.instant * elapsed_ms) / SOTA_ms;
    SDL_Log("Reload %d ms %f frames", elapsed_ms, frame);
}

void receive_event_SDL_QUIT(struct Game *sota, SDL_Event *event) {
    sota->flags.isrunning = false;
}

void receive_event_SDL_CONTROLLERDEVICEREMOVED(struct Game *sota, SDL_Event *event) {
    // event->cdevice.which of DEVICEREMOVED is different from DEVICEADDED!
    // DEVICEREMOVED-> JoystickInstanceID which increments for every joystick

    SDL_assert(sota->cursor.entity != TNECS_NULL);

    struct controllerGamepad *gamepad_ptr;
    gamepad_ptr = IES_GET_COMPONENT(gl_world, sota->cursor.entity, controllerGamepad);
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

    if (sota->cursor.entity == TNECS_NULL) {
        SDL_Log("Entity_cursor is not valid");
        return;
    }

    struct controllerGamepad *gamepad_ptr;
    gamepad_ptr = IES_GET_COMPONENT(gl_world, sota->cursor.entity, controllerGamepad);
    if (gamepad_ptr != NULL)
        Gamepad_addController(gamepad_ptr, event->cdevice.which);
    else
        SDL_Log("entity_cursor has no controllerGamepad component");
}

void receive_event_SDL_MOUSEMOTION(struct Game *sota, SDL_Event *event) {
    SDL_assert(event);
    if (sota->timers.runtime_ns <= CURSOR_FIRSTMENU_PAUSE_ns) {
        SDL_LogVerbose(SOTA_LOG_FPS, "Sota first init pause.");
        return;
    }
    if (event->motion.windowID != SDL_GetWindowID(sota->render.window)) {
        SDL_LogVerbose(SOTA_LOG_FPS, "Wrong window");
        return;
    }
    tnecs_entity mouse = sota->mouse.entity;
    if (sota->mouse.entity == TNECS_NULL) {
        SDL_LogVerbose(SOTA_LOG_FPS, "Mouse disabled");
        return;
    }

    struct Sprite   *msprite  = IES_GET_COMPONENT(gl_world, mouse, Sprite);
    struct Position *mpos     = IES_GET_COMPONENT(gl_world, mouse, Position);
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
    SDL_assert(sota->render.window != NULL);
    SDL_assert(event        != NULL);
    SDL_assert(sota->mouse.entity > 0);

    if (event->motion.windowID != SDL_GetWindowID(sota->render.window)) {
        SDL_LogVerbose(SOTA_LOG_FPS, "Wrong window");
        return;
    }

    struct controllerMouse *mouse;
    if (event->button.state == SDL_PRESSED) {
        mouse = IES_GET_COMPONENT(gl_world, sota->mouse.entity, controllerMouse);
        Mouse_checkButton(mouse, event->button.button);
    }
}

// TODO: rn to _Turn_Start
void receive_event_Turn_Start(struct Game *sota, SDL_Event *userevent) {
    Map *map = Game_Map(sota);
    SDL_assert(Game_State_Current(sota) == GAME_STATE_Gameplay_Map);

    /* - Refresh all units - */
    // TODO: make into a system?
    size_t num = DARR_NUM(map->units.onfield.arr);
    for (int i = 0; i < num; i++) {
        tnecs_entity entity = map->units.onfield.arr[i];
        if (entity != TNECS_NULL) {
            Game_Unit_Refresh(sota, entity);
        }
    }

    i32 *army = &map->armies.onfield[map->armies.current];

    /* Switch control to next army */
    Event_Emit(__func__, SDL_USEREVENT,
               event_Game_Control_Switch,
               army, NULL);
}

void receive_event_Turn_Transition(struct Game *sota, SDL_Event *userevent) {
    tnecs_entity turn_transition;
    turn_transition = TNECS_ENTITY_CREATE_wCOMPONENTS(gl_world,
                                                      MapAnimation_ID,
                                                      Position_ID,
                                                      Text_ID,
                                                      Timer_ID);

    struct Timer *timer;
    timer  = IES_GET_COMPONENT(gl_world,
                               turn_transition, Timer);
    *timer = Timer_default;

    // TODO: How to do a fancy animation?
    // 1- draw animation pixel art
    // 2- Modify MapAnimation component
    // 3- Implement animation checker in Map_TurnTransition_Animate
    struct MapAnimation *map_anim;
    map_anim  = IES_GET_COMPONENT(gl_world, turn_transition, MapAnimation);
    *map_anim = MapAnimation_default;
    map_anim->time_ns = SOTA_SOUNDFX_NEXT_TURN_DURATION_ms * SOTA_us;
    map_anim->anim = &Map_TurnTransition_Animate;

    /* Get Army name */
    Map *map = Game_Map(sota);
    SDL_assert(DARR_NUM(map->armies.onfield) > 1);
    SDL_assert(map->armies.onfield[0] == ARMY_FRIENDLY);

    Map_Army_Next(map);
    SDL_assert(map->armies.current >= 0);
    SDL_assert(map->armies.current < DARR_NUM(map->armies.onfield));

    i8 army      = map->armies.onfield[map->armies.current];
    SDL_assert(army >  ARMY_NULL);
    SDL_assert(army <= ARMY_NUM);
    s8 army_name = armyNames[army];

    /* -- Change music -- */
    Game_Music_Stop(sota);
    if (SotA_isPC(army)) {
        sota->audio.music = map->music.friendly;
        SDL_assert(sota->audio.music != NULL);
    } else {
        sota->audio.music = map->music.enemy;
    }
    if (sota->audio.music != NULL)
        Game_Music_Play(sota);

    /* -- Play Turn Transition -- */
#ifndef DEBUG_NO_SOUNDFX
    if (sota->audio.soundfx_next_turn) {
        Mix_PlayChannel(-1, sota->audio.soundfx_next_turn, 0);
    }
#endif /* DEBUG_NO_SOUNDFX */

    struct Text *text;
    text  = IES_GET_COMPONENT(gl_world, turn_transition, Text);
    Text_Init(text);
    P_Text_Init(text, sota->render.er);
    SDL_assert(text->plat != NULL);
    text->pixelfont         = sota->fonts.pixelnours_big;
    s8 line = s8_mut(army_name.data);
    line = s8cat(line, s8_literal(" Turn"));
    Text_Set(text, line.data, PIXELNOURS_BIG_Y_OFFSET);
    SDL_assert((text->size.x > 0) && (text->size.y > 0));
    s8_free(&line);

    struct Position *position;
    position  = IES_GET_COMPONENT(gl_world, turn_transition, Position);
    *position = Position_default;
    position->onTilemap = false;
    position->scale[0] = 10.0f;
    position->scale[1] = 10.0f;
    position->pixel_pos.x = sota->settings.res.x / 2 - text->size.x / 2 * position->scale[0];
    position->pixel_pos.y = sota->settings.res.y / 2;

    strncpy(sota->debug.reason, "Turn transition is an animation", sizeof(sota->debug.reason));
    Game_subState_Set(sota, GAME_SUBSTATE_MAP_ANIMATION, sota->debug.reason);
}

void receive_event_Turn_End(struct Game *sota, SDL_Event *userevent) {
    /* - Pop all menus - */
    while (DARR_NUM(sota->menus.stack) > 0) {
        tnecs_entity menu_pop       = DARR_POP(sota->menus.stack);
        struct Menu *mc             = IES_GET_COMPONENT(gl_world, menu_pop, Menu);
        SDL_assert(mc           != NULL);
        SDL_assert(mc->elem_pos != NULL);
        mc->visible = false;
    }

    /* - Focus cursor on tilemap - */
    Game_cursorFocus_onMap(sota);

    /* - Destroy reinforcement timer - */
    if (sota->timers.reinf != TNECS_NULL) {
        tnecs_entity_destroy(gl_world, sota->timers.reinf);
        sota->timers.reinf = TNECS_NULL;
    }

    /* - Reset dangermap - */
    Map *map = Game_Map(sota);
    Map_Stacked_Dangermap_Reset(map);
    Map_Danger_Reset(map);
    Map_Palettemap_Reset(map);

    // TODO
    // u64 updateTime_ns = 0ul;
    // b32 success = tnecs_pipeline_step(gl_world, updateTime_ns, sota, TNECS_PIPELINE_TURN_END);
    // if (!success) {
    //     SDL_Log("Pipeline %d failed", TNECS_PIPELINE_TURN_END);
    //     SDL_assert(false);
    //     exit(ERROR_Generic);
    // }

    // TODO make into a system
    /* - Decrement persistent auras for all units - */
    i32 army = map->armies.onfield[map->armies.current];
    Map_Bonus_Remove_Persistent(map, army);

    Event_Emit(__func__, SDL_USEREVENT, event_Turn_Transition, NULL, NULL);
}

void receive_event_Unit_Enters_Shop(struct Game *sota, SDL_Event *userevent) {
}

void receive_event_Unit_Enters_Village(struct Game *sota, SDL_Event *userevent) {
}

void receive_event_Unit_Enters_Armory(struct Game *sota, SDL_Event *userevent) {
}

void receive_event_Unit_Select(struct Game *sota, SDL_Event *userevent) {
    sota->selected.unit_entity = *((tnecs_entity *) userevent->user.data2);
    sota->combat.aggressor = sota->selected.unit_entity;
    SDL_assert(sota->selected.unit_entity > TNECS_NULL);
    SDL_assert(Game_State_Current(sota)                == GAME_STATE_Gameplay_Map);
    if (fsm_eUnitSel_ss[Game_Substate_Current(sota)] != NULL)
        fsm_eUnitSel_ss[Game_Substate_Current(sota)](sota, sota->cursor.entity);
}

void receive_event_Unit_Deselect(struct Game *sota, SDL_Event *userevent) {
    SDL_assert(sota->cursor.entity != TNECS_NULL);
    sota->combat.aggressor = TNECS_NULL;
    sota->combat.defendant = TNECS_NULL;

    if (sota->selected.unit_entity == TNECS_NULL) {
        return;
    }

    tnecs_entity unit_ent       = sota->selected.unit_entity;
    struct Position *pos_ptr    = IES_GET_COMPONENT(gl_world, unit_ent, Position);
    struct Unit     *unit_ptr   = IES_GET_COMPONENT(gl_world, unit_ent, Unit);
    SDL_assert(pos_ptr  != NULL);
    SDL_assert(unit_ptr != NULL);

    sota->selected.unit_moved_position.x = pos_ptr->tilemap_pos.x;
    sota->selected.unit_moved_position.y = pos_ptr->tilemap_pos.y;

    /* - Reset overlay modes and/or go back to standby - */
    if (fsm_eUnitDsel_ss[Game_Substate_Current(sota)] != NULL)
        fsm_eUnitDsel_ss[Game_Substate_Current(sota)](sota, sota->cursor.entity);

    /* - New overlays - */
    Map *map = Game_Map(sota);
    if (SotA_isPC(Unit_Army(unit_ptr))) {
        /* - Hide PC overlay - */
        map->flags.show_overlay = false;
    } else {
        /* - Show NPC danger - */
        if (Unit_showsDanger(unit_ptr)) {
            tnecs_entity *ent_ptr = &sota->selected.unit_entity;
            Event_Emit(__func__, SDL_USEREVENT, event_Unit_Danger, NULL, ent_ptr);
        }
    }

    sota->selected.unit_entity = TNECS_NULL;
}

void receive_event_Unit_Entity_Return(struct Game *sota, SDL_Event *userevent) {
}

void receive_event_Unit_Icon_Return(struct Game *sota, SDL_Event *userevent) {

    /* - Hide overlay/movemap - */
    Map *map = Game_Map(sota);
    map->flags.show_overlay = false;
    tnecs_entity returning = sota->selected.unit_entity;
    SDL_assert(returning > TNECS_NULL);

    struct Position *pos_ptr  = IES_GET_COMPONENT(gl_world, returning, Position);
    struct Sprite *sprite = IES_GET_COMPONENT(gl_world, sota->selected.unit_entity, Sprite);
    SDL_assert(pos_ptr != NULL);
    SDL_assert(sprite != NULL);

    /* - Resetting map animation loop - */
    struct Point initial = sota->selected.unit_initial_position;
    SDL_assert(sprite->spritesheet != NULL);
    if (sprite->spritesheet->loop_num == MAP_UNIT_LOOP_NUM) {
        if ((initial.x == pos_ptr->tilemap_pos.x) && (initial.y == pos_ptr->tilemap_pos.y))
            Spritesheet_Loop_Set(sprite->spritesheet, MAP_UNIT_LOOP_TAUNT, sprite->flip);
        else
            Spritesheet_Loop_Set(sprite->spritesheet, MAP_UNIT_LOOP_IDLE, sprite->flip);
        Sprite_Animation_Loop(sprite);
    }

    /* - Returning to initial position - */
    pos_ptr->tilemap_pos = initial;
    pos_ptr->pixel_pos.x = initial.x * pos_ptr->scale[0];
    pos_ptr->pixel_pos.y = initial.y * pos_ptr->scale[1];
}

void receive_event_Unit_Moves(struct Game *sota, SDL_Event *userevent) {
    /* Setup for MAP_UNIT_MOVES state */
    SDL_assert(userevent->user.data1 != NULL);
    SDL_assert(sota->cursor.entity          != TNECS_NULL);
    SDL_assert(sota->selected.unit_entity   != TNECS_NULL);

    /* -- Initialize Arrow -- */
    Position *cpos;
    Unit     *selected;
    cpos     = IES_GET_COMPONENT(gl_world, sota->cursor.entity,        Position);
    selected = IES_GET_COMPONENT(gl_world, sota->selected.unit_entity, Unit);
    SDL_assert(cpos                 != NULL);
    SDL_assert(selected             != NULL);
    Map *map = Game_Map(sota);
    SDL_assert(map->darrs.costmap   != NULL);
    i32 move = 0;
    Unit_computeMove(selected, &move);
    Arrow_Path_Init(map->arrow, map->darrs.costmap, move, cpos->tilemap_pos);
    strncpy(sota->debug.reason, "friendly unit was selected and can move", sizeof(sota->debug.reason));
    Game_subState_Set(sota, GAME_SUBSTATE_MAP_UNIT_MOVES, sota->debug.reason);

    /* -- Sprite walking animation -- */
    struct Sprite *sprite = IES_GET_COMPONENT(gl_world, sota->selected.unit_entity, Sprite);
    SDL_assert(sprite->spritesheet != NULL);
    if (sprite->spritesheet->loop_num == MAP_UNIT_LOOP_NUM) {
        // TODO: check if arrow exists and is long, revert to correct orientation
        if (sprite->flip == SDL_FLIP_HORIZONTAL)
            Spritesheet_Loop_Set(sprite->spritesheet, MAP_UNIT_LOOP_MOVEL, sprite->flip);
        else
            Spritesheet_Loop_Set(sprite->spritesheet, MAP_UNIT_LOOP_MOVER, sprite->flip);
    }
    Sprite_Animation_Loop(sprite);
}

void receive_event_Cursor_Hovers_Unit(struct Game *sota, SDL_Event *userevent) {
    sota->hovered.unit_entity = *(tnecs_entity *)userevent->user.data2;
    SDL_assert(sota->hovered.unit_entity != TNECS_NULL);
    struct Unit *temp = IES_GET_COMPONENT(gl_world, sota->hovered.unit_entity, Unit);
    SDL_assert(Unit_Name(temp).data != NULL);

    if (fsm_eCrsHvUnit_ss[Game_Substate_Current(sota)] != NULL)
        fsm_eCrsHvUnit_ss[Game_Substate_Current(sota)](sota, sota->hovered.unit_entity);
}

void receive_event_Cursor_Dehovers_Unit(struct Game *sota, SDL_Event *userevent) {
    tnecs_entity dehovered_unit_entity = sota->hovered.unit_entity;
    sota->hovered.unit_entity            = TNECS_NULL;

    if (fsm_eCrsDeHvUnit_ss[Game_Substate_Current(sota)] != NULL)
        fsm_eCrsDeHvUnit_ss[Game_Substate_Current(sota)](sota, dehovered_unit_entity);
}

void receive_event_Units_Refresh(struct Game *sota, SDL_Event *userevent) {
}

void receive_event_Unit_Danger(struct Game *sota, SDL_Event *userevent) {
    SDL_assert(sota->cursor.entity != TNECS_NULL);

    if (fsm_eUnitDng_ss[Game_Substate_Current(sota)] != NULL)
        fsm_eUnitDng_ss[Game_Substate_Current(sota)](sota, sota->cursor.entity);
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
    if ((menu_entity == sota->menus.item_select) && (sota->selected.unit_entity)) {
        struct Sprite *sprite;
        sprite = IES_GET_COMPONENT(gl_world, sota->selected.unit_entity, Sprite);

        SDL_assert(sprite->spritesheet != NULL);
        SDL_assert(sprite->spritesheet->loop_num == MAP_UNIT_LOOP_NUM);
        Spritesheet_Loop_Set(sprite->spritesheet, MAP_UNIT_LOOP_STANCE, sprite->flip);
        Sprite_Animation_Loop(sprite);
    }

    // Note: reason set by event sender is kinda dumb.
    // reason might get overwritten...
    if (Game_Substate_Current(sota) != GAME_SUBSTATE_MENU)
        Game_subState_Set(sota, GAME_SUBSTATE_MENU, sota->debug.reason);

}

void receive_event_Loadout_Selected(struct Game *sota, SDL_Event *userevent) {
    /* Aggressor loadout was selected, time to select defendant. */

    /* - Turn menu_attack invisible - */
    int stack_top           = DARR_NUM(sota->menus.stack) - 1;
    tnecs_entity menu_top   = sota->menus.stack[stack_top];
    struct Menu *mc         = IES_GET_COMPONENT(gl_world, menu_top, Menu);
    SDL_assert(mc              != NULL);
    SDL_assert(mc->type        == MENU_TYPE_WEAPON_SELECT);
    SDL_assert(mc->type        == MENU_TYPE_WEAPON_SELECT);
    SDL_assert(mc->elem_pos    != NULL);
    mc->visible = false;

    /* - Turn PopUp_Loadout_Stats invisible - */
    Game_PopUp_Loadout_Stats_Hide(sota);

    // Setting: Select loadout first
    /* - Switch to Map_Candidates substate - */
    SDL_assert(Game_State_Current(sota)    == GAME_STATE_Gameplay_Map);
    SDL_assert(Game_Substate_Current(sota) == GAME_SUBSTATE_MENU);
    strncpy(sota->debug.reason, "loadout was selected, time to select defendant",
            sizeof(sota->debug.reason));
    SDL_Log("Game_Switch_toCandidates -> defendants");
    Game_Switch_toCandidates(sota, sota->targets.defendants); // sends event_Cursor_Hovers_Unit

    // 1. Compute Combat stuff -> Move to cursor hovers new defendant
    sota->combat.defendant = sota->targets.candidates[sota->targets.order];
    Game_Combat_Outcome(sota);

    // 2. Enable Pre-combat menu -> Move to cursor hovers new defendant
    Game_PopUp_Pre_Combat_Enable(sota);

    /* -- Start unit combat stance loop -- */
    // NOTE: ONLY FOR ATTACK
    struct Sprite *sprite = IES_GET_COMPONENT(gl_world, sota->combat.defendant, Sprite);
    if (MAP_UNIT_LOOP_STANCE < sprite->spritesheet->loop_num) {
        Spritesheet_Loop_Set(sprite->spritesheet, MAP_UNIT_LOOP_STANCE, sprite->flip);
        Sprite_Animation_Loop(sprite);
    }

    // 3. Attackmap only defendant. -> Move to cursor hovers new defendant
    Map *map = Game_Map(sota);
    struct Position *pos  = IES_GET_COMPONENT(gl_world, sota->combat.defendant, Position);
    memset(map->darrs.attacktomap, 0, Map_area(map) * sizeof(*map->darrs.attacktomap));
    map->darrs.attacktomap[(pos->tilemap_pos.y * Map_col_len(map) + pos->tilemap_pos.x)] = 1;
    Map_Palettemap_Autoset(map, MAP_OVERLAY_ATTACK, TNECS_NULL);
    Map_Stacked_Dangermap_Compute(map, map->darrs.dangermap);
}

void receive_event_Input_ZOOM_IN(struct Game *sota, SDL_Event *userevent) {
    /* -- Check: Only Zoom_in on the map -- */
    SDL_assert(Game_State_Current(sota) == GAME_STATE_Gameplay_Map);
    b32 correct_substate = (Game_Substate_Current(sota) == GAME_SUBSTATE_STANDBY);
    correct_substate |= (Game_Substate_Current(sota) == GAME_SUBSTATE_MAP_CANDIDATES);
    correct_substate |= (Game_Substate_Current(sota) == GAME_SUBSTATE_MAP_UNIT_MOVES);
    correct_substate |= (Game_Substate_Current(sota) == GAME_SUBSTATE_MAP_ANIMATION);
    SDL_assert(correct_substate);

    i32 controller_type = * (i32 *) userevent->user.data2;
    SDL_assert(
            (controller_type == CONTROLLER_MOUSE)       ||
            (controller_type == CONTROLLER_GAMEPAD)     ||
            (controller_type == CONTROLLER_KEYBOARD)    ||
            (controller_type == CONTROLLER_TOUCHPAD)
    );

    Events_Controllers_Check(sota, controller_type);

    /* -- Zoom in to sprite_atorigin -- */
    struct Sprite *sprite_atorigin;
    struct Sprite *cursor_sprite;
    struct Sprite *mouse_sprite;
    cursor_sprite   = IES_GET_COMPONENT(gl_world, sota->cursor.entity, Sprite);
    mouse_sprite    = IES_GET_COMPONENT(gl_world, sota->mouse.entity, Sprite);
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
    Map *map = Game_Map(sota);
    struct Camera *cam = &map->render.camera;
    struct SDL_Rect dstrect = sprite_atorigin->dstrect;
    float previous_zoom = cam->zoom;
    float new_zoom = cam->zoom + CAMERA_INCREMENT;
    cam->zoom = new_zoom < MAX_CAMERA_ZOOM ? new_zoom : cam->zoom;
    cam->offset.x = SOTA_ZOOMTOPOINT(cam->zoom / previous_zoom, cam->offset.x, dstrect.x);
    cam->offset.y = SOTA_ZOOMTOPOINT(cam->zoom / previous_zoom, cam->offset.y, dstrect.y);
    map->flags.camera_moved = true;
}

void receive_event_Input_ZOOM_OUT(struct Game *sota, SDL_Event *userevent) {
    /* -- Check: Only Zoom_out on the map -- */
    SDL_assert(Game_State_Current(sota) == GAME_STATE_Gameplay_Map);
    b32 correct_substate = (Game_Substate_Current(sota) == GAME_SUBSTATE_STANDBY);
    correct_substate |= (Game_Substate_Current(sota) == GAME_SUBSTATE_MAP_CANDIDATES);
    correct_substate |= (Game_Substate_Current(sota) == GAME_SUBSTATE_MAP_UNIT_MOVES);
    correct_substate |= (Game_Substate_Current(sota) == GAME_SUBSTATE_MAP_ANIMATION);
    SDL_assert(correct_substate);

    i32 controller_type = * (i32 *) userevent->user.data2;
    SDL_assert(
            (controller_type == CONTROLLER_MOUSE)       ||
            (controller_type == CONTROLLER_GAMEPAD)     ||
            (controller_type == CONTROLLER_KEYBOARD)    ||
            (controller_type == CONTROLLER_TOUCHPAD)
    );

    Events_Controllers_Check(sota, controller_type);

    /* -- Zoom out to sprite_atorigin -- */
    struct Sprite *cursor_sprite;
    struct Sprite *mouse_sprite;
    struct Sprite *sprite_atorigin;
    cursor_sprite   = IES_GET_COMPONENT(gl_world, sota->cursor.entity, Sprite);
    mouse_sprite    = IES_GET_COMPONENT(gl_world, sota->mouse.entity, Sprite);
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
    Map *map = Game_Map(sota);
    struct Camera *cam = &map->render.camera;
    struct SDL_Rect dstrect = sprite_atorigin->dstrect;
    float previous_zoom = cam->zoom;
    float new_zoom = cam->zoom - CAMERA_INCREMENT;
    cam->zoom = new_zoom > MIN_CAMERA_ZOOM ? new_zoom : cam->zoom;
    cam->offset.x = SOTA_ZOOMTOPOINT(cam->zoom / previous_zoom, cam->offset.x, dstrect.x);
    cam->offset.y = SOTA_ZOOMTOPOINT(cam->zoom / previous_zoom, cam->offset.y, dstrect.y);
    map->flags.camera_moved = true;

}

void receive_event_Input_UP(    struct Game *sota, SDL_Event *userevent) {}
void receive_event_Input_LEFT(  struct Game *sota, SDL_Event *userevent) {}
void receive_event_Input_DOWN(  struct Game *sota, SDL_Event *userevent) {}
void receive_event_Input_RIGHT( struct Game *sota, SDL_Event *userevent) {}

void receive_event_Input_MENURIGHT(struct Game *sota, SDL_Event *userevent) {
    i32 controller_type = * (i32 *) userevent->user.data2;
    SDL_assert(
            (controller_type == CONTROLLER_MOUSE)       ||
            (controller_type == CONTROLLER_GAMEPAD)     ||
            (controller_type == CONTROLLER_KEYBOARD)    ||
            (controller_type == CONTROLLER_TOUCHPAD)
    );

    Events_Controllers_Check(sota, controller_type);

    if (fsm_eMenuRight_s[Game_State_Current(sota)] != NULL)
        fsm_eMenuRight_s[Game_State_Current(sota)](sota, controller_type);
}

void receive_event_Input_MENULEFT(struct Game *sota, SDL_Event *userevent) {
    i32 controller_type = * (i32 *) userevent->user.data2;
    SDL_assert(
            (controller_type == CONTROLLER_MOUSE)       ||
            (controller_type == CONTROLLER_GAMEPAD)     ||
            (controller_type == CONTROLLER_KEYBOARD)    ||
            (controller_type == CONTROLLER_TOUCHPAD)
    );

    Events_Controllers_Check(sota, controller_type);

    if (fsm_eMenuLeft_s[Game_State_Current(sota)] != NULL)
        fsm_eMenuLeft_s[Game_State_Current(sota)](sota, controller_type);
}

void receive_event_Input_MINIMAP(struct Game *sota, SDL_Event *userevent) {
    i32 controller_type = * (i32 *) userevent->user.data2;
    SDL_assert(
            (controller_type == CONTROLLER_MOUSE)       ||
            (controller_type == CONTROLLER_GAMEPAD)     ||
            (controller_type == CONTROLLER_KEYBOARD)    ||
            (controller_type == CONTROLLER_TOUCHPAD)
    );

    Events_Controllers_Check(sota, controller_type);

}

void receive_event_Input_FAST_FORWARD(struct Game *sota, SDL_Event *userevent) {
    i32 controller_type = * (i32 *) userevent->user.data2;
    SDL_assert(
            (controller_type == CONTROLLER_MOUSE)       ||
            (controller_type == CONTROLLER_GAMEPAD)     ||
            (controller_type == CONTROLLER_KEYBOARD)    ||
            (controller_type == CONTROLLER_TOUCHPAD)
    );

    Events_Controllers_Check(sota, controller_type);
    sota->flags.fast_forward = !sota->flags.fast_forward;
}

void receive_event_Input_PAUSE(struct Game *sota, SDL_Event *userevent) {
    /* TODO: REMOVE -> NO NEED FOR PAUSE */
    i32 controller_type = * (i32 *) userevent->user.data2;
    SDL_assert(
            (controller_type == CONTROLLER_MOUSE)       ||
            (controller_type == CONTROLLER_GAMEPAD)     ||
            (controller_type == CONTROLLER_KEYBOARD)    ||
            (controller_type == CONTROLLER_TOUCHPAD)
    );
    Events_Controllers_Check(sota, controller_type);

    if (fsm_eStart_s[Game_State_Current(sota)] != NULL)
        fsm_eStart_s[Game_State_Current(sota)](sota, controller_type);
}

void receive_event_Unit_Seize(struct Game *sota, SDL_Event *userevent) {

}

void receive_event_Game_Over(struct Game *sota, SDL_Event *userevent) {
    /* - Hiding menus - */
    b32 destroy = false;
    while (DARR_NUM(sota->menus.stack) > 0)
        Game_menuStack_Pop(sota, destroy);

    /* - Hide Cursor - */
    struct Sprite *sprite = IES_GET_COMPONENT(gl_world, sota->cursor.entity, Sprite);
    SDL_assert(sprite != NULL);
    sprite->visible = false;

    /* - Hiding popups - */
    if (sota->popups.arr[POPUP_TYPE_HUD_TILE] != TNECS_NULL) {
        Game_PopUp_Tile_Hide(sota);
    }
    if (sota->popups.arr[POPUP_TYPE_HUD_UNIT] != TNECS_NULL) {
        Game_PopUp_Unit_Hide(sota);
    }

    /* - Remove unused components - */
    Map *map = Game_Map(sota);
    for (int i = 0; i < DARR_NUM(map->units.onfield.arr); i++) {
        tnecs_entity ent = map->units.onfield.arr[i];
        TNECS_REMOVE_COMPONENTS(gl_world, ent, MapHPBar_ID);
        if (IES_ENTITY_HASCOMPONENT(gl_world, ent, RenderTop)) {
            TNECS_REMOVE_COMPONENTS(gl_world, ent, RenderTop_ID);
        }

        if (IES_ENTITY_HASCOMPONENT(gl_world, ent, UnitMoveAnimation)) {
            TNECS_REMOVE_COMPONENTS(gl_world, ent, UnitMoveAnimation_ID);
        }

        if (IES_ENTITY_HASCOMPONENT(gl_world, ent, Boss)) {
            TNECS_REMOVE_COMPONENTS(gl_world, ent, Boss_ID);
        }
    }

    /* - Map_Free - */
    Game_Map_Free(sota);

    /* -- Creating cutscene to play -- */
    sota->narrative.cutscene = TNECS_ENTITY_CREATE_wCOMPONENTS(gl_world, Cutscene_ID,
                                                               Position_ID, Text_ID, Timer_ID);

    struct Cutscene *cutscene;
    cutscene  = IES_GET_COMPONENT(gl_world, sota->narrative.cutscene, Cutscene);
    *cutscene = Cutscene_default;
    cutscene->event = event_Quit;

    struct Timer *timer;
    timer  = IES_GET_COMPONENT(gl_world, sota->narrative.cutscene, Timer);
    *timer = Timer_default;

    /* TODO: remove Text when cutscene can actually play */
    struct Text *text;
    text  = IES_GET_COMPONENT(gl_world, sota->narrative.cutscene, Text);
    Text_Init(text);
    P_Text_Init(text, sota->render.er);
    SDL_assert(text->plat != NULL);
    text->pixelfont         = sota->fonts.pixelnours_big;
    s8 line = s8_literal("Tragedy.");
    Text_Set(text, line.data, PIXELNOURS_BIG_Y_OFFSET);
    SDL_assert((text->size.x > 0) && (text->size.y > 0));

    /* TODO: remove Position when cutscene can actually play */
    struct Position *position;
    position  = IES_GET_COMPONENT(gl_world, sota->narrative.cutscene, Position);
    *position = Position_default;
    position->onTilemap = false;
    position->scale[0] = 10.0f;
    position->scale[1] = 10.0f;
    position->pixel_pos.x = sota->settings.res.x / 2 - text->size.x / 2 * position->scale[0];
    position->pixel_pos.y = sota->settings.res.y / 2;

    /* - Set state to cutscene - */
    strncpy(sota->debug.reason, "Game plays cutscene", sizeof(sota->debug.reason));
    Game_State_Set(sota, GAME_STATE_Cutscene, sota->debug.reason);

    if (Game_Substate_Current(sota) != GAME_SUBSTATE_STANDBY) {
        strncpy(sota->debug.reason, "Cutscene is playing", sizeof(sota->debug.reason));
        Game_subState_Set(sota, GAME_SUBSTATE_STANDBY, sota->debug.reason);
    }
}

void receive_event_Unit_Refresh(struct Game *sota, SDL_Event *userevent) {

}

void receive_event_Unit_Wait(struct Game *sota, SDL_Event *userevent) {
    /* -- Preliminaries -- */

    /* - Make aggressor wait - */
    SDL_assert(sota->combat.aggressor == sota->selected.unit_entity);
    tnecs_entity unit_ent = sota->combat.aggressor;
    SDL_assert(unit_ent > TNECS_NULL);
    Game_Unit_Wait(sota, unit_ent);

    /* - hide arrow - */
    Map *map = Game_Map(sota);
    map->arrow->show = false;

    /* - hide movemap - */
    Map_Stacked_Dangermap_Reset(map);
    Map_Palettemap_Reset(map);

    if (DARR_NUM(sota->menus.stack) == 0)
        Game_cursorFocus_onMap(sota);
    /* -- Deselect unit and go back to map -- */

    // Event_Emit(__func__, SDL_USEREVENT, event_Unit_Deselect, NULL, &sota->selected.unit_entity);
    receive_event_Unit_Deselect(sota, userevent);
}

void receive_event_Unit_Talk(struct Game *sota, SDL_Event *userevent) {
    SDL_assert(sota->cursor.entity          > TNECS_NULL);
    SDL_assert(sota->selected.unit_entity   > TNECS_NULL);
    tnecs_entity unit_ent = sota->selected.unit_entity;
    Game_Unit_Wait(sota, unit_ent);

    Event_Emit(__func__, SDL_USEREVENT, event_Gameplay_Return2Standby, NULL, NULL);
}

void receive_event_Unit_Rescue(struct Game *sota, SDL_Event *userevent) {
    SDL_assert(sota->cursor.entity          > TNECS_NULL);
    SDL_assert(sota->selected.unit_entity   > TNECS_NULL);
    tnecs_entity unit_ent = sota->selected.unit_entity;
    Game_Unit_Wait(sota, unit_ent);

    Event_Emit(__func__, SDL_USEREVENT, event_Gameplay_Return2Standby, NULL, NULL);
}

void receive_event_Combat_Start(struct Game *sota, SDL_Event *userevent) {
    SDL_assert(sota->combat.aggressor > TNECS_NULL);
    SDL_assert(sota->combat.defendant > TNECS_NULL);
    struct Sprite *agg_sprite = IES_GET_COMPONENT(gl_world, sota->combat.aggressor, Sprite);
    struct Sprite *dft_sprite = IES_GET_COMPONENT(gl_world, sota->combat.defendant, Sprite);

    SDL_assert(IES_ENTITY_HASCOMPONENT(gl_world, sota->combat.defendant, Timer));
    SDL_assert(IES_ENTITY_HASCOMPONENT(gl_world, sota->combat.aggressor, Timer));

    // 1. Make cursor visible
    struct Sprite *sprite     = IES_GET_COMPONENT(gl_world, sota->cursor.entity, Sprite);
    sprite->visible = true;

    // 3. Animate Combat
    /* -- TODO choose map animation or combat animation -- */
    /* -- change game substate -- */
    strncpy(sota->debug.reason, "Quitting gameplay", sizeof(sota->debug.reason));
    Game_subState_Set(sota, GAME_SUBSTATE_MAP_ANIMATION, sota->debug.reason);
    /* -- find attack direction -- */
    struct Position *agg_posc   = IES_GET_COMPONENT(gl_world, sota->combat.aggressor, Position);
    struct Position *dft_posc   = IES_GET_COMPONENT(gl_world, sota->combat.defendant, Position);
    struct Point    *dft_pos    = &dft_posc->tilemap_pos;
    struct Point    *agg_pos    = &agg_posc->tilemap_pos;

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

    struct Timer *agg_timer = IES_GET_COMPONENT(gl_world, sota->combat.aggressor, Timer);
    struct Timer *dft_timer = IES_GET_COMPONENT(gl_world, sota->combat.defendant, Timer);
    SDL_assert(agg_timer != NULL);
    SDL_assert(dft_timer != NULL);
    dft_timer->paused = true;
    agg_timer->paused = true;

    /* -- Create combat animation entity -- */
    // TODO change to MapAnimation component
    tnecs_entity combat_animation;
    combat_animation = TNECS_ENTITY_CREATE_wCOMPONENTS(gl_world, Timer_ID, CombatAnimation_ID);

    struct CombatAnimation *combat_anim;
    combat_anim  = IES_GET_COMPONENT(gl_world, combat_animation, CombatAnimation);
    SDL_assert(combat_anim != NULL);
    *combat_anim = CombatAnimation_default;

    struct Timer *timer = IES_GET_COMPONENT(gl_world, combat_animation, Timer);
    SDL_assert(combat_anim != NULL);
    *timer = Timer_default;
    timer->time_ns = 0;

    /* - Hide pre combat popup, if it exists - */
    struct Menu *mc;
    mc = IES_GET_COMPONENT(gl_world, sota->popups.pre_combat, Menu);
    if (mc != NULL)
        mc->visible = false;

    /* - Show map combat popup - */
    if (sota->popups.arr[POPUP_TYPE_MAP_COMBAT] == TNECS_NULL)
        Game_PopUp_Map_Combat_Create(sota);
    Game_PopUp_Map_Combat_Update(sota);

    // struct Unit *aggressor = IES_GET_COMPONENT(gl_world, sota->combat.aggressor, Unit);
    // struct Unit *defendant = IES_GET_COMPONENT(gl_world, sota->combat.defendant, Unit);
    struct PopUp *popup    = IES_GET_COMPONENT(gl_world, sota->popups.arr[POPUP_TYPE_MAP_COMBAT],
                                               PopUp);
    struct PopUp_Map_Combat *pmc = popup->data;

    PopUp_Map_Combat_Units(pmc, sota, sota->combat.aggressor, sota->combat.defendant);
    popup->visible = true;
    /* - Deselect weapons in LoadoutSelectMenu, if it exists - */
    mc = IES_GET_COMPONENT(gl_world, sota->menus.weapon_select, Menu);
    if (mc != NULL) {
        struct LoadoutSelectMenu *wsm = mc->data;
        LoadoutSelectMenu_Select_Reset(wsm);
    }

    /* - Set previous candidate to NULL - */
    sota->targets.previous_order = -1;

    SDL_assert(IES_ENTITY_HASCOMPONENT(gl_world, sota->combat.defendant, Timer));
    SDL_assert(IES_ENTITY_HASCOMPONENT(gl_world, sota->combat.aggressor, Timer));
}

void receive_event_Combat_End(struct Game *sota, SDL_Event *userevent) {
    // Event_Emit(__func__, SDL_USEREVENT, event_Unit_Wait, NULL, NULL);

    // 1. Resolve Combat
    struct Unit *aggressor = IES_GET_COMPONENT(gl_world, sota->combat.aggressor, Unit);
    Unit_Waiting_set(aggressor, true);
    // struct Unit *defendant = IES_GET_COMPONENT(gl_world, sota->combat.defendant, Unit);

    SDL_assert(IES_ENTITY_HASCOMPONENT(gl_world, sota->combat.defendant, Timer));
    SDL_assert(IES_ENTITY_HASCOMPONENT(gl_world, sota->combat.aggressor, Timer));

    // tnecs_entity  popup_ent      = sota->popups.arr[POPUP_TYPE_MAP_COMBAT];
    // struct PopUp *popup_ptr      = IES_GET_COMPONENT(gl_world, popup_ent, PopUp);
    // struct PopUp_Map_Combat *pmc = popup_ptr->data;

    /* 2. Update maphpbars. */
    // Unit might be dead, only update if maphpbar exists
    struct MapHPBar *map_hp_bar = IES_GET_COMPONENT(gl_world, sota->combat.aggressor, MapHPBar);
    if (map_hp_bar != NULL) {
        map_hp_bar->update  = true;
        map_hp_bar->visible = true;
    }

    map_hp_bar = IES_GET_COMPONENT(gl_world, sota->combat.defendant, MapHPBar);
    if (map_hp_bar != NULL) {
        map_hp_bar->update  = true;
        map_hp_bar->visible = true;
    }

    // 3. reset animation states
    struct Sprite *dft_sprite = IES_GET_COMPONENT(gl_world, sota->combat.defendant, Sprite);
    struct Sprite *agg_sprite = IES_GET_COMPONENT(gl_world, sota->combat.aggressor, Sprite);
    Sprite_Animation_Restart(agg_sprite, MAP_UNIT_LOOP_IDLE);
    Sprite_Animation_Restart(dft_sprite, MAP_UNIT_LOOP_IDLE);

    // 4. Restart Defendant animation
    struct Timer *dft_timer = IES_GET_COMPONENT(gl_world, sota->combat.defendant, Timer);
    dft_timer->paused = false;

    // 5. Hide PopUp_Map_Combat
    Game_PopUp_Map_Combat_Hide(sota);

    // 6. Revert hovered entity to aggressor
    sota->hovered.unit_entity = sota->combat.aggressor;

    if (fsm_eCmbtEnd_ss[Game_Substate_Previous(sota)] != NULL)
        fsm_eCmbtEnd_ss[Game_Substate_Previous(sota)](sota);

    SDL_assert(IES_ENTITY_HASCOMPONENT(gl_world, sota->combat.defendant, Timer));
    SDL_assert(IES_ENTITY_HASCOMPONENT(gl_world, sota->combat.aggressor, Timer));

    receive_event_Unit_Wait(sota, userevent);
}

void receive_event_Defendant_Select(struct Game *sota, SDL_Event *userevent) {
    Game_PopUp_Pre_Combat_Hide(sota);
    /* --- Start Combat --- */
    // Necessary criteria:
    //  - sota->combat.aggressor
    //  - sota->combat.defendant
    Event_Emit(__func__, SDL_USEREVENT, event_Combat_Start, data1_entity, data2_entity);
}

void receive_event_Unit_Trade(struct Game *sota, SDL_Event *userevent) {
    SDL_assert(sota->cursor.entity > TNECS_NULL);
    if (sota->selected.unit_entity == TNECS_NULL)
        return;

    Game_Unit_Wait(sota, sota->selected.unit_entity);

    Event_Emit(__func__, SDL_USEREVENT, event_Gameplay_Return2Standby, NULL, NULL);
}

void receive_event_Unit_Escape(struct Game *sota, SDL_Event *userevent) {
    SDL_assert(sota->cursor.entity > TNECS_NULL);
    if (sota->selected.unit_entity == TNECS_NULL)
        return;

    Game_Unit_Wait(sota, sota->selected.unit_entity);

    Event_Emit(__func__, SDL_USEREVENT, event_Gameplay_Return2Standby, NULL, NULL);
}

void receive_event_Unit_Staff(struct Game *sota, SDL_Event *userevent) {
}

void receive_event_Unit_Items(struct Game *sota, SDL_Event *userevent) {
    SDL_assert(sota->cursor.entity > TNECS_NULL);
    SDL_assert(sota->selected.unit_entity != TNECS_NULL);

    Game_Unit_Wait(sota, sota->selected.unit_entity);

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
    struct Unit *killer = IES_GET_COMPONENT(gl_world, killer_entity, Unit);
    struct Unit *victim = IES_GET_COMPONENT(gl_world, victim_entity, Unit);
    struct Boss *boss   = IES_GET_COMPONENT(gl_world, victim_entity, Boss);

    SDL_assert(killer != NULL);
    SDL_assert(victim != NULL);

    // b32 died_boss = (boss != NULL);
    Map *map = Game_Map(sota);

    /* --- Increasing Killer's regrets --- */
    int regrets = Unit_Current_Regrets(killer);
    killer->counters.regrets = regrets > UINT8_MAX - REGRET_KILL ? UINT8_MAX : regrets + REGRET_KILL;

    /* --- Removing unit from map --- */
    SDL_assert(map->world == gl_world);
    SDL_assert(victim_entity > TNECS_NULL);
    Map_Unit_Remove(map, victim_entity);

    /* --- Making unit sprite invisible --- */
    struct Sprite *sprite = IES_GET_COMPONENT(gl_world, victim_entity, Sprite);
    sprite->visible = false;

    /* --- Remove MapHPbar --- */
    if (IES_ENTITY_HASCOMPONENT(gl_world, victim_entity, MapHPBar))
        TNECS_REMOVE_COMPONENTS(gl_world, victim_entity, MapHPBar_ID);

    /* Components changed place, need to reload */
    victim = IES_GET_COMPONENT(gl_world, victim_entity, Unit);
    boss   = IES_GET_COMPONENT(gl_world, victim_entity, Boss);

    /* --- Deleting entity? --- */
    // - Put unit entity in list of killed units

    /* --- Check Map conditions --- */
    Map_Conditions_Check_Death(map->conditions.death_enemy,      map,
                               victim,                      boss, sota);
    Map_Conditions_Check_Death(map->conditions.death_friendly,   map,
                               victim,                      boss, sota);
}

void receive_event_Unit_Loots(struct Game *sota, SDL_Event *userevent) {
    tnecs_entity looter_entity = *(tnecs_entity *) userevent->user.data1;
    // tnecs_entity victim_entity = *(tnecs_entity *) userevent->user.data2;
    struct Unit *looter = IES_GET_COMPONENT(gl_world, looter_entity, Unit);
    // struct Unit *victim = IES_GET_COMPONENT(gl_world, victim_entity, Unit);
    int regrets = Unit_Current_Regrets(looter);
    looter->counters.regrets = regrets > UINT8_MAX - REGRET_LOOT ? UINT8_MAX : regrets + REGRET_LOOT;
}

void receive_event_Increment_Attack(struct Game *sota, SDL_Event *userevent) {
    /* -- Popup_Map_Combat -- */
    // tnecs_entity popup_ent = sota->popups.arr[POPUP_TYPE_MAP_COMBAT];
    // struct PopUp *popup_ptr  = IES_GET_COMPONENT(gl_world, popup_ent, PopUp);
    // struct PopUp_Map_Combat *pmc = popup_ptr->data;
    // SDL_assert(pmc->defendant == sota->combat.defendant);
    // SDL_assert(pmc->aggressor == sota->combat.aggressor);
    // SDL_assert(pmc->defendant > TNECS_NULL);
    // SDL_assert(pmc->aggressor > TNECS_NULL);
    SDL_assert(sota->combat.aggressor > TNECS_NULL);
    SDL_assert(sota->combat.defendant > TNECS_NULL);
    Unit *aggressor  = IES_GET_COMPONENT(gl_world, sota->combat.aggressor, Unit);
    Unit *defendant  = IES_GET_COMPONENT(gl_world, sota->combat.defendant, Unit);
    SDL_assert(aggressor != NULL);
    SDL_assert(defendant != NULL);

    // 1. Get next HP of attacked unit
    struct Combat_Attack *attacks = sota->combat.outcome.attacks;
    SDL_assert(sota->combat.outcome.current_attack < DARR_NUM(attacks));
    struct Combat_Attack attack = sota->combat.outcome.attacks[sota->combat.outcome.current_attack++];

    // pmc->current_attack = sota->combat.outcome.current_attack;
    struct Unit *attacker = NULL, *defender = NULL;

    attacker = attack.attacker ? aggressor : defendant;
    defender = attack.attacker ? defendant : aggressor;
    Combat_Resolve_Attack(attack, attacker, defender);
    // int id_L          = Unit_Id_Equipped(attacker, UNIT_HAND_LEFT);
    // int id_R          = Unit_Id_Equipped(attacker, UNIT_HAND_RIGHT);

    // 2. Check for unit agony/death
    b32 agg_death = (!Unit_isAlive(aggressor)) || (Unit_Current_Agony(aggressor) > AGONY_NULL);
    if (agg_death) {
        userevent->user.data1 = &sota->combat.aggressor;
        userevent->user.data2 = &sota->combat.defendant;
        receive_event_Unit_Dies(sota, userevent);
        // Event_Emit(__func__, SDL_USEREVENT, event_Unit_Dies, &sota->combat.aggressor, &sota->combat.defendant);
    }

    b32 dft_death = (!Unit_isAlive(defendant)) || (Unit_Current_Agony(defendant) > AGONY_NULL);
    if (dft_death) {
        userevent->user.data1 = &sota->combat.defendant;
        userevent->user.data2 = &sota->combat.aggressor;
        receive_event_Unit_Dies(sota, userevent);
        // Event_Emit(__func__, SDL_USEREVENT, event_Unit_Dies, &sota->combat.defendant, &sota->combat.aggressor);
    }
    /* Only one of combatants can die */
    SDL_assert(!(dft_death && agg_death));

    sota->combat.outcome.ended = (agg_death || dft_death);

    // SDL_assert(popup_ptr != NULL);
    // SDL_assert(pmc != NULL);
    SDL_assert(sota->combat.aggressor != TNECS_NULL);
    SDL_assert(sota->combat.defendant != TNECS_NULL);
    // pmc->update = true;
}

void receive_event_Unit_Agonizes(struct Game *sota, SDL_Event *userevent) {
    tnecs_entity victor_entity = *(tnecs_entity *) userevent->user.data1;
    // tnecs_entity victim_entity = *(tnecs_entity *) userevent->user.data2;
    struct Unit *victor = IES_GET_COMPONENT(gl_world, victor_entity, Unit);
    // struct Unit *victim = IES_GET_COMPONENT(gl_world, victim_entity, Unit);
    int regrets = Unit_Current_Regrets(victor);
    victor->counters.regrets = regrets > UINT8_MAX - REGRET_LOOT ? UINT8_MAX : regrets + REGRET_LOOT;
}

/* --- EVENT UTILITIES --- */
void receive_event_SDL_WINDOWEVENT(struct Game *sota, SDL_Event *event) {
    if (event->window.windowID != SDL_GetWindowID(sota->render.window))
        return;

    switch (event->window.event) {
        case SDL_WINDOWEVENT_CLOSE:
            Event_Emit(__func__, SDL_QUIT, event_SDL_QUIT, NULL, NULL);
            break;
    }

}

void Events_Names_Declare(void) {
    if (event_Start != 0)
        return;
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
    event_names = NULL;
}

s8 *event_names = NULL;
void Events_Names_Alloc(void) {
    if (event_names != NULL) {
        return;
    }
    event_names = SDL_calloc((event_End - event_Start) + 1, sizeof(*event_names));
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

    /* Notes:
    **  1. events emitted during SDL_PollEvent
    **     loop don't get polled by SDL_PollEvent.
    **      -> Prevents infinite event loops.
    **  2. How to ensure events occur on same frame
    **     without triggering infinite loop?
    **      -> Run function, don't emit event.
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
