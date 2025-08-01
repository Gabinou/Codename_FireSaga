
#include "systems/render.h"
#include "actor.h"
#include "game/game.h"
#include "popup/popup.h"
#include "map/map.h"
#include "game/map.h"
#include "map/render.h"
#include "sprite.h"
#include "structs.h"
#include "debug.h"
#include "position.h"
#include "globals.h"
#include "scene.h"
#include "map/animation.h"
#include "unit/anim.h"
#include "cutscene.h"
#include "menu/menu.h"
#include "bars/map_hp.h"
#include "popup/tile.h"
#include "AI.h"
#include "boss.h"
#include "text.h"
#include "pixelfonts.h"
#include "SDL.h"

void Scroll_Text(tnecs_input *input) {
    /* --- PRELIMINARIES --- */
    /* -- Get components arrays -- */
    struct Timer     *ut_arr = TNECS_COMPONENT_ARRAY(input, Timer_ID);
    struct PixelFont *pf_arr = TNECS_COMPONENT_ARRAY(input, PixelFont_ID);

    for (size_t order = 0; order < input->num_entities; order++) {
        struct Timer     *ut = (ut_arr + order);
        struct PixelFont *pf = (pf_arr + order);

        ut->reset = PixelFont_Scroll(pf, ut->time_ns);
    }
}

void Animate_Sprite(tnecs_input *input) {
    /* --- PRELIMINARIES --- */
    /* -- Get components arrays -- */
    struct Timer  *ut_arr     = TNECS_COMPONENT_ARRAY(input, Timer_ID);
    struct Sprite *sprite_arr = TNECS_COMPONENT_ARRAY(input, Sprite_ID);

    for (size_t order = 0; order < input->num_entities; order++) {
        struct Timer       *ut     = (ut_arr     + order);
        struct Sprite      *sprite = (sprite_arr + order);

        struct Spritesheet *ss     = sprite->spritesheet;
        SDL_assert(ss != NULL);

        int loop  = ss->current_loop;
        int frame = ss->current_frame;
        SDL_assert(loop  >= 0);
        SDL_assert(frame >= 0);

        /* Only draw next frame in animation loop if enough time has passed */
        int time_ms = (int)(ut->time_ns / IES_us);
        if (time_ms <= ss->speeds[loop][frame])
            continue;

        ut->reset = true;
        Sprite_Animation_Loop(sprite);
    }
}

void Draw_Actor(tnecs_input *input) {
    /* --- PRELIMINARIES --- */
    /* -- Get game -- */
    Game *IES = input->data;
    SDL_assert(IES != NULL);

    /* -- Get components arrays -- */
    struct Actor    *actor_arr   = TNECS_COMPONENT_ARRAY(input, Actor_ID);
    struct Position *position_arr = TNECS_COMPONENT_ARRAY(input, Position_ID);
    SDL_assert(actor_arr   != NULL);
    SDL_assert(position_arr != NULL);

    for (size_t order = 0; order < input->num_entities; order++) {
        struct Actor    *actor      = (actor_arr   + order);
        struct Position *position   = (position_arr   + order);

        if (!actor->visible)
            continue;

        // TODO: Scene camera?
        Actor_Draw(actor,
                   position,
                   &Camera_default,
                   IES->render.target,
                   IES->render.er
                  );
    }
}

void Draw_Sprite(tnecs_input *input) {
    /* --- PRELIMINARIES --- */
    /* -- Get game -- */
    Game *IES = input->data;
    SDL_assert(IES != NULL);

    Map *map = Game_Map(IES);
    struct Camera camera = (map == NULL) ? Camera_default : map->render.camera;

    /* -- Get components arrays -- */
    struct Sprite   *sprite_arr   = TNECS_COMPONENT_ARRAY(input, Sprite_ID);
    struct Position *position_arr = TNECS_COMPONENT_ARRAY(input, Position_ID);
    SDL_assert(sprite_arr   != NULL);
    SDL_assert(position_arr != NULL);

    size_t         archetype_id = input->entity_archetype_id;

    for (size_t order = 0; order < input->num_entities; order++) {
        struct Sprite   *sprite   = (sprite_arr   + order);

        if (!sprite->visible)
            continue;

        tnecs_entity entity     = gl_world->bytype.entities[archetype_id][order];

        MouseFlag *mouse_flag   = IES_GET_COMPONENT(gl_world, entity, MouseFlag);

        if (mouse_flag == NULL) {
            Position    *position       = (position_arr + order);
            CursorFlag  *cursor_flag    = IES_GET_COMPONENT(gl_world, entity, CursorFlag);
            b32 isCursor = (cursor_flag != NULL);
            dstrect_func_t func = dstrect_funcs[!position->onTilemap][isCursor];
            func(sprite, &position->pixel_pos, &camera);
        }

        Sprite_Draw(sprite, IES->render.er);
    }
}

void Draw_Map_Boss_Icon(tnecs_input *input) {
    /* --- PRELIMINARIES --- */
    /* -- Get game -- */
    Game *IES = input->data;
    SDL_assert(IES != NULL);

    /* -- Get components arrays -- */
    struct Boss     *boss_arr       = TNECS_COMPONENT_ARRAY(input, Boss_ID);
    struct Sprite   *sprite_arr     = TNECS_COMPONENT_ARRAY(input, Sprite_ID);
    struct Position *position_arr   = TNECS_COMPONENT_ARRAY(input, Position_ID);

    for (size_t order = 0; order < input->num_entities; order++) {
        struct Boss     *boss       = (boss_arr     + order);
        struct Sprite   *sprite     = (sprite_arr   + order);
        struct Position *position   = (position_arr + order);

        if (!sprite->visible)
            continue;

        Map *map = Game_Map(IES);
        Boss_Icon_Pos(boss, &map->render.camera, position, map);
        Boss_Icon_Draw(boss, position, IES->render.er);
    }
}

void Draw_Map_HPBar(tnecs_input *input) {
    /* --- PRELIMINARIES --- */
    /* -- Get game -- */
    Game *IES = input->data;
    SDL_assert(IES != NULL);

    /* -- Get components arrays -- */
    struct Position *pos_arr        = TNECS_COMPONENT_ARRAY(input, Position_ID);
    struct MapHPBar *map_hp_bar_arr = TNECS_COMPONENT_ARRAY(input, MapHPBar_ID);

    for (size_t order = 0; order < input->num_entities; order++) {
        struct MapHPBar *map_hp_bar = (map_hp_bar_arr + order);

        if ((!map_hp_bar->visible) && (!map_hp_bar->update))
            continue;

        map_hp_bar->tilemap_pos = pos_arr[order].tilemap_pos;
        Map *map = Game_Map(IES);
        i32 *tilesize = (i32 *)Map_Tilesize(map);
        MapHPBar_Draw(map_hp_bar, &map->render.camera,
                      tilesize, IES->render.target,
                      IES->render.er, gl_world);
    }

}

void Draw_PopUp(tnecs_input *input) {
    /* --- PRELIMINARIES --- */
    /* -- Get game -- */
    Game *IES = input->data;
    SDL_assert(IES != NULL);

    /* -- Get components arrays -- */
    struct PopUp    *popup_arr    = TNECS_COMPONENT_ARRAY(input, PopUp_ID);
    struct Position *position_arr = TNECS_COMPONENT_ARRAY(input, Position_ID);
    SDL_assert(popup_arr    != NULL);
    SDL_assert(position_arr != NULL);

    for (size_t order = 0; order < input->num_entities; order++) {
        struct PopUp *popup = (popup_arr + order);

        if (!popup->visible) /* Skip drawing if not visible */
            continue;

        struct Position *position = (position_arr + order);
        popup->draw(popup, position->pixel_pos, IES->render.target, IES->render.er);
    }
}

void Draw_Menu(tnecs_input *input) {
    /* --- PRELIMINARIES --- */
    /* -- Get game -- */
    Game *IES = input->data;
    SDL_assert(IES != NULL);

    /* -- Get components arrays -- */
    struct Menu *mc_arr = TNECS_COMPONENT_ARRAY(input, Menu_ID);
    SDL_assert(mc_arr != NULL);

    /* --- DRAWING MENU ENTITIES --- */
    for (size_t order = 0; order < input->num_entities; order++) {
        struct Menu *mc = (mc_arr + order);

        if (!mc->visible)
            continue;

        mc->draw(mc, IES->render.target, IES->render.er);

#ifdef IES_DEBUG_MENU_BOXES
        Menu_Elem_Boxes_Draw(mc, IES->render.er);
#endif
    }
}

void Draw_Text(tnecs_input *input) {
    /* --- PRELIMINARIES --- */
    /* -- Get game -- */
    Game *IES = (Game *)input->data;
    SDL_assert(IES != NULL);

    /* -- Get components arrays -- */
    struct Text     *text_arr        = TNECS_COMPONENT_ARRAY(input, Text_ID);
    struct Position *position_arr    = TNECS_COMPONENT_ARRAY(input, Position_ID);
    SDL_assert(position_arr != NULL);

    /* --- DRAWING TEXT ENTITIES --- */
    for (size_t order = 0; order < input->num_entities; order++) {
        struct Position *pos  = (position_arr    + order);
        struct Text     *text = (text_arr        + order);

        Text_Place(text, pos);
        Text_Draw(text);
    }
}

void Draw_Text_Timer(tnecs_input *input) {
    /* --- PRELIMINARIES --- */
    /* -- Get game -- */
    Game *IES = (Game *)input->data;
    SDL_assert(IES != NULL);

    /* -- Get components arrays -- */
    struct Text     *text_arr        = TNECS_COMPONENT_ARRAY(input, Text_ID);
    struct Position *position_arr    = TNECS_COMPONENT_ARRAY(input, Position_ID);
    struct Timer    *updatetimer_arr = TNECS_COMPONENT_ARRAY(input, Timer_ID);
    SDL_assert(position_arr != NULL);

    /* --- DRAWING TEXT ENTITIES --- */
    for (size_t order = 0; order < input->num_entities; order++) {
        struct Position *pos  = (position_arr    + order);
        struct Text     *text = (text_arr        + order);
        struct Timer    *ut   = (updatetimer_arr + order);

        size_t         archetype_id = input->entity_archetype_id;
        tnecs_entity   entity      = gl_world->bytype.entities[archetype_id][order];

        if ((text->onUpdate != NULL) &&
            (ut->time_ns >= text->update_time_ns)) {
            (*text->onUpdate)(IES, entity, NULL);
            // Note: this resets the FPS timer.
            ut->reset = true;
        }

        Text_Place(text, pos);
        Text_Draw(text);
    }
}

void Animate_Combat_onMap(tnecs_input *input) {
    /* --- PRELIMINARIES --- */
    /* -- Get game -- */
    Game *IES = (Game *)input->data;
    SDL_assert(IES != NULL);

    /* -- Get components arrays -- */
    struct CombatAnimation  *combatanim_arr = TNECS_COMPONENT_ARRAY(input, CombatAnimation_ID);
    struct Timer            *timer_arr      = TNECS_COMPONENT_ARRAY(input, Timer_ID);

    /* --- DRAWING TEXT ENTITIES --- */
    for (size_t order = 0; order < input->num_entities; order++) {
        struct CombatAnimation *map_anim        = &combatanim_arr[order];
        struct Timer           *combat_timer    = &timer_arr[order];

        size_t         archetype_id = input->entity_archetype_id;
        tnecs_entity   entity      = gl_world->bytype.entities[archetype_id][order];

        Map_Combat_Animate(IES, entity, map_anim, combat_timer);
    }
}

void Animate_Map_Animation(tnecs_input *input) {
    /* --- PRELIMINARIES --- */
    /* -- Get game -- */
    Game *IES = (Game *)input->data;
    SDL_assert(IES != NULL);

    /* -- Get components arrays -- */
    struct Timer        *timer_arr      = TNECS_COMPONENT_ARRAY(input, Timer_ID);
    struct MapAnimation *mapanim_arr    = TNECS_COMPONENT_ARRAY(input, MapAnimation_ID);

    /* --- DRAWING TEXT ENTITIES --- */
    for (size_t order = 0; order < input->num_entities; order++) {
        struct MapAnimation *map_anim   = &mapanim_arr[order];
        struct Timer        *timer      = &timer_arr[order];

        size_t         archetype_id = input->entity_archetype_id;
        tnecs_entity   entity      = gl_world->bytype.entities[archetype_id][order];

        map_anim->anim(IES, entity, map_anim, timer);
    }
}

void Animate_Unit_Move_onMap(tnecs_input *input) {
    /* --- PRELIMINARIES --- */
    /* -- Get game -- */
    Game *IES = (Game *)input->data;
    SDL_assert(IES != NULL);

    /* -- Get components arrays -- */
    struct Timer                *timer_arr      = TNECS_COMPONENT_ARRAY(input, Timer_ID);
    struct UnitMoveAnimation    *unitmove_arr   = TNECS_COMPONENT_ARRAY(input, UnitMoveAnimation_ID);

    /* --- DRAWING TEXT ENTITIES --- */
    for (size_t order = 0; order < input->num_entities; order++) {
        struct UnitMoveAnimation *unit_anim  = &unitmove_arr[order];
        struct Timer             *timer      = &timer_arr[order];

        size_t         archetype_id = input->entity_archetype_id;
        tnecs_entity   entity      = gl_world->bytype.entities[archetype_id][order];

        Unit_Move_onMap_Animate(IES, entity, timer, unit_anim);
    }
}

void Animate_Cutscene(tnecs_input *input) {
    /* --- PRELIMINARIES --- */
    /* -- Get game -- */
    Game *IES = (Game *)input->data;
    SDL_assert(IES != NULL);

    /* -- Get components arrays -- */
    struct Timer    *timer_arr      = TNECS_COMPONENT_ARRAY(input, Timer_ID);
    struct Cutscene *cutscene_arr   = TNECS_COMPONENT_ARRAY(input, Cutscene_ID);

    /* --- DRAWING TEXT ENTITIES --- */
    for (size_t order = 0; order < input->num_entities; order++) {
        struct Timer    *timer      = &timer_arr[order];
        struct Cutscene *cutscene   = &cutscene_arr[order];

        if (timer->time_ns >= cutscene->time_ns) {
            Cutscene_Finish(cutscene, IES);
        }
        // Cutscene_Animate(IES, entity, scene, timer);
    }
}

void Draw_Scene(        tnecs_input *input) {
    Game *IES = input->data;
    SDL_assert(IES != NULL);

    Scene *scene_arr = TNECS_COMPONENT_ARRAY(input, Scene_ID);

    /* There should only be one scene */
    SDL_assert(input->num_entities == 1);
    Scene *scene = scene_arr;
    Scene_Draw(scene, &IES->settings, IES->render.target, IES->render.er);
}

void Draw_Map(          tnecs_input *input) {
    Game *IES = input->data;
    SDL_assert(IES != NULL);

    Map *map_arr = TNECS_COMPONENT_ARRAY(input, Map_ID);

    /* There should only be one map */
    if (input->num_entities != 1);
    Map *map = map_arr;

    Map_Draw(map, &IES->settings, IES->render.target);
    Map_Grid_Draw(map, &IES->settings);
    Map_Perimeter_Draw_Danger(map, &IES->settings);

    // TODO: Draw weapon auras.
    // void Map_Perimeter_Draw_Aura(struct Map     *map,    struct Settings *settings,
    //                          struct Camera  *camera, struct Point pos,
    //                          struct Range    range,  sota->map->perimiter_aura_color) {

#ifdef DEBUG_SUPPORT_PERIMITER
    /* Draw support auras perimiters for all friendlies */
    Map_Perimeter_Draw_Support(map, settings, gl_gl_world);
#endif /* DEBUG_SUPPORT_PERIMITER */
}
