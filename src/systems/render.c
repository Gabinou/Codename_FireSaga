
#include "systems/render.h"
#include "popup/popup.h"
#include "map/map.h"
#include "sprite.h"
#include "debug.h"
#include "position.h"
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

void Scroll_Text(tnecs_system_input *input) {
    /* --- PRELIMINARIES --- */
    /* -- Get components arrays -- */
    struct Timer     *ut_arr = TNECS_COMPONENTS_LIST(input, Timer);
    struct PixelFont *pf_arr = TNECS_COMPONENTS_LIST(input, PixelFont);

    for (u16 order = 0; order < input->num_entities; order++) {
        struct Timer     *ut = (ut_arr + order);
        struct PixelFont *pf = (pf_arr + order);

        ut->reset = PixelFont_Scroll(pf, ut->time_ns);
    }
}

void Animate_Sprite(tnecs_system_input *input) {
    /* --- PRELIMINARIES --- */
    /* -- Get components arrays -- */
    struct Timer  *ut_arr     = TNECS_COMPONENTS_LIST(input, Timer);
    struct Sprite *sprite_arr = TNECS_COMPONENTS_LIST(input, Sprite);

    for (u16 order = 0; order < input->num_entities; order++) {
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

void Draw_Sprite(tnecs_system_input *input) {
    /* --- PRELIMINARIES --- */
    /* -- Get game -- */
    Game *IES = input->data;
    SDL_assert(IES != NULL);

    /* -- Get components arrays -- */
    struct Sprite   *sprite_arr   = TNECS_COMPONENTS_LIST(input, Sprite);
    struct Position *position_arr = TNECS_COMPONENTS_LIST(input, Position);
    SDL_assert(sprite_arr   != NULL);
    SDL_assert(position_arr != NULL);

    tnecs_world   *world       = input->world;
    size_t         typeflag_id = input->entity_typeflag_id;

    for (u16 order = 0; order < input->num_entities; order++) {
        struct Sprite   *sprite   = (sprite_arr   + order);

        if (!sprite->visible)
            continue;

        tnecs_entity entity     = world->entities_bytype[typeflag_id][order];

        MouseFlag *mouse_flag   = TNECS_GET_COMPONENT(IES->world, entity, MouseFlag);

        if (mouse_flag == NULL) {
            Position    *position       = (position_arr + order);
            CursorFlag  *cursor_flag    = TNECS_GET_COMPONENT(IES->world, entity, CursorFlag);
            b32 isCursor = (cursor_flag != NULL);
            dstrect_func_t func = dstrect_funcs[!position->onTilemap][isCursor];
            func(sprite, &position->pixel_pos, &IES->camera);
        }

        Sprite_Draw(sprite, IES->renderer);
    }
}

void Draw_Map_Boss_Icon(tnecs_system_input *input) {
    /* --- PRELIMINARIES --- */
    /* -- Get game -- */
    Game *IES = input->data;
    SDL_assert(IES != NULL);

    /* -- Get components arrays -- */
    struct Boss *boss_arr = TNECS_COMPONENTS_LIST(input, Boss);
    struct Sprite *sprite_arr = TNECS_COMPONENTS_LIST(input, Sprite);
    struct Position *position_arr = TNECS_COMPONENTS_LIST(input, Position);

    for (u16 order = 0; order < input->num_entities; order++) {
        struct Boss     *boss       = (boss_arr     + order);
        struct Sprite   *sprite     = (sprite_arr   + order);
        struct Position *position   = (position_arr + order);

        if (!sprite->visible)
            continue;

        Boss_Icon_Pos(boss, &IES->camera, position, IES->map);
        Boss_Icon_Draw(boss, position, IES->renderer);
    }
}

void Draw_Map_HPBar(tnecs_system_input *input) {
    /* --- PRELIMINARIES --- */
    /* -- Get game -- */
    Game *IES = input->data;
    SDL_assert(IES != NULL);

    /* -- Get components arrays -- */
    struct Position *pos_arr        = TNECS_COMPONENTS_LIST(input, Position);
    struct MapHPBar *map_hp_bar_arr = TNECS_COMPONENTS_LIST(input, MapHPBar);

    for (u16 order = 0; order < input->num_entities; order++) {
        struct MapHPBar *map_hp_bar = (map_hp_bar_arr + order);

        if ((!map_hp_bar->visible) && (!map_hp_bar->update))
            continue;

        map_hp_bar->tilemap_pos = pos_arr[order].tilemap_pos;
        MapHPBar_Draw(map_hp_bar, &IES->camera, IES->map->tilesize, IES->render_target,
                      IES->renderer, IES->world);
    }

}

void Draw_PopUp(tnecs_system_input *input) {
    /* --- PRELIMINARIES --- */
    /* -- Get game -- */
    Game *IES = input->data;
    SDL_assert(IES != NULL);

    /* -- Get components arrays -- */
    struct PopUp    *popup_arr    = TNECS_COMPONENTS_LIST(input, PopUp);
    struct Position *position_arr = TNECS_COMPONENTS_LIST(input, Position);
    SDL_assert(popup_arr    != NULL);
    SDL_assert(position_arr != NULL);

    for (u16 order = 0; order < input->num_entities; order++) {
        struct PopUp *popup = (popup_arr + order);

        if (!popup->visible) /* Skip drawing if not visible */
            continue;

        struct Position *position = (position_arr + order);
        popup->draw(popup, position->pixel_pos, IES->render_target, IES->renderer);
    }
}

void Draw_Menu(tnecs_system_input *input) {
    /* --- PRELIMINARIES --- */
    /* -- Get game -- */
    Game *IES = input->data;
    SDL_assert(IES != NULL);

    /* -- Get components arrays -- */
    struct Menu *mc_arr = TNECS_COMPONENTS_LIST(input, Menu);
    SDL_assert(mc_arr != NULL);

    /* --- DRAWING MENU ENTITIES --- */
    for (u16 order = 0; order < input->num_entities; order++) {
        struct Menu *mc = (mc_arr + order);

        if (!mc->visible)
            continue;

        mc->draw(mc, IES->render_target, IES->renderer);

#ifdef IES_DEBUG_MENU_BOXES
        Menu_Elem_Boxes_Draw(mc, IES->renderer);
#endif
    }
}

void Draw_Text(tnecs_system_input *input) {
    /* --- PRELIMINARIES --- */
    SDL_Rect dstrect;
    /* -- Get game -- */
    Game *IES = (Game *)input->data;
    SDL_assert(IES != NULL);

    /* -- Get components arrays -- */
    struct Text     *text_arr        = TNECS_COMPONENTS_LIST(input, Text);
    struct Position *position_arr    = TNECS_COMPONENTS_LIST(input, Position);
    SDL_assert(position_arr != NULL);

    /* --- DRAWING TEXT ENTITIES --- */
    for (u16 order = 0; order < input->num_entities; order++) {
        struct Position *pos  = (position_arr    + order);
        struct Text     *text = (text_arr        + order);

        dstrect.x = pos->pixel_pos.x;
        dstrect.y = pos->pixel_pos.y;
        dstrect.w = text->rect.w * pos->scale[0];
        dstrect.h = text->rect.h * pos->scale[1];

        Text_Draw(text, IES->renderer, &dstrect);
    }
}

void Draw_Text_Timer(tnecs_system_input *input) {
    /* --- PRELIMINARIES --- */
    SDL_Rect dstrect;
    /* -- Get game -- */
    Game *IES = (Game *)input->data;
    SDL_assert(IES != NULL);

    /* -- Get components arrays -- */
    struct Text     *text_arr        = TNECS_COMPONENTS_LIST(input, Text);
    struct Position *position_arr    = TNECS_COMPONENTS_LIST(input, Position);
    struct Timer    *updatetimer_arr = TNECS_COMPONENTS_LIST(input, Timer);
    SDL_assert(position_arr != NULL);

    /* --- DRAWING TEXT ENTITIES --- */
    for (u16 order = 0; order < input->num_entities; order++) {
        struct Position *pos  = (position_arr    + order);
        struct Text     *text = (text_arr        + order);
        struct Timer    *ut   = (updatetimer_arr + order);

        tnecs_world   *world       = input->world;
        size_t         typeflag_id = input->entity_typeflag_id;
        tnecs_entity   entity      = world->entities_bytype[typeflag_id][order];

        if ((text->onUpdate != NULL) && (ut->time_ns >= text->update_time_ns)) {
            (*text->onUpdate)(IES, entity, ut->frame_count, ut->time_ns, NULL);
            ut->reset = true;
        }

        dstrect.x = pos->pixel_pos.x;
        dstrect.y = pos->pixel_pos.y;
        dstrect.w = text->rect.w * pos->scale[0];
        dstrect.h = text->rect.h * pos->scale[1];

        Text_Draw(text, IES->renderer, &dstrect);
    }
}

void Animate_Combat_onMap(tnecs_system_input *input) {
    /* --- PRELIMINARIES --- */
    /* -- Get game -- */
    Game *IES = (Game *)input->data;
    SDL_assert(IES != NULL);

    /* -- Get components arrays -- */
    struct CombatAnimation *combatanim_arr;
    struct Timer *timer_arr      = TNECS_COMPONENTS_LIST(input, Timer);
    combatanim_arr = TNECS_COMPONENTS_LIST(input, CombatAnimation);

    /* --- DRAWING TEXT ENTITIES --- */
    for (u16 order = 0; order < input->num_entities; order++) {
        struct CombatAnimation *map_anim        = &combatanim_arr[order];
        struct Timer           *combat_timer    = &timer_arr[order];

        tnecs_world   *world       = input->world;
        size_t         typeflag_id = input->entity_typeflag_id;
        tnecs_entity   entity      = world->entities_bytype[typeflag_id][order];

        Map_Combat_Animate(IES, entity, map_anim, combat_timer);
    }
}

void Animate_Map_Animation(tnecs_system_input *input) {
    /* --- PRELIMINARIES --- */
    /* -- Get game -- */
    Game *IES = (Game *)input->data;
    SDL_assert(IES != NULL);

    /* -- Get components arrays -- */
    struct Timer        *timer_arr = TNECS_COMPONENTS_LIST(input, Timer);
    struct Text         *text_arr  = TNECS_COMPONENTS_LIST(input, Text);
    struct Position     *pos_arr   = TNECS_COMPONENTS_LIST(input, Position);
    struct MapAnimation *mapanim_arr;
    mapanim_arr = TNECS_COMPONENTS_LIST(input, MapAnimation);

    /* --- DRAWING TEXT ENTITIES --- */
    for (u16 order = 0; order < input->num_entities; order++) {
        struct MapAnimation *map_anim   = &mapanim_arr[order];
        struct Timer        *timer      = &timer_arr[order];
        struct Position     *pos        = &pos_arr[order];
        struct Text         *text       = &text_arr[order];

        tnecs_world   *world       = input->world;
        size_t         typeflag_id = input->entity_typeflag_id;
        tnecs_entity   entity      = world->entities_bytype[typeflag_id][order];

        map_anim->anim(IES, entity, map_anim, timer);
    }
}

void Animate_Unit_Move_onMap(tnecs_system_input *input) {
    /* --- PRELIMINARIES --- */
    /* -- Get game -- */
    Game *IES = (Game *)input->data;
    SDL_assert(IES != NULL);

    /* -- Get components arrays -- */
    struct Timer        *timer_arr = TNECS_COMPONENTS_LIST(input, Timer);
    struct Unit         *unit_arr  = TNECS_COMPONENTS_LIST(input, Unit);
    struct UnitMoveAnimation *unitmove_arr;
    unitmove_arr = TNECS_COMPONENTS_LIST(input, UnitMoveAnimation);

    /* --- DRAWING TEXT ENTITIES --- */
    for (u16 order = 0; order < input->num_entities; order++) {
        struct UnitMoveAnimation *unit_anim  = &unitmove_arr[order];
        struct Timer             *timer      = &timer_arr[order];
        struct Unit              *unit       = &unit_arr[order];

        tnecs_world   *world       = input->world;
        size_t         typeflag_id = input->entity_typeflag_id;
        tnecs_entity   entity      = world->entities_bytype[typeflag_id][order];

        Unit_Move_onMap_Animate(IES, entity, timer, unit_anim);
    }
}

void Animate_Cutscene(tnecs_system_input *input) {
    /* --- PRELIMINARIES --- */
    /* -- Get game -- */
    Game *IES = (Game *)input->data;
    SDL_assert(IES != NULL);

    /* -- Get components arrays -- */
    struct Timer    *timer_arr      = TNECS_COMPONENTS_LIST(input, Timer);
    struct Cutscene *cutscene_arr   = TNECS_COMPONENTS_LIST(input, Cutscene);

    /* --- DRAWING TEXT ENTITIES --- */
    for (u16 order = 0; order < input->num_entities; order++) {
        struct Timer    *timer      = &timer_arr[order];
        struct Cutscene *cutscene   = &cutscene_arr[order];

        tnecs_world     *world       = input->world;
        size_t           typeflag_id = input->entity_typeflag_id;
        tnecs_entity     entity      = world->entities_bytype[typeflag_id][order];

        if (timer->time_ns >= cutscene->time_ns) {
            Cutscene_Finish(cutscene, IES);
        }
        // Cutscene_Animate(IES, entity, scene, timer);
    }
}

void Animate_Scene(tnecs_system_input *input) {
    /* --- PRELIMINARIES --- */
    /* -- Get game -- */
    Game *IES = (Game *)input->data;
    SDL_assert(IES != NULL);

    /* -- Get components arrays -- */
    struct Timer    *timer_arr = TNECS_COMPONENTS_LIST(input, Timer);
    struct Scene    *scene_arr = TNECS_COMPONENTS_LIST(input, Scene);

    /* --- DRAWING TEXT ENTITIES --- */
    for (u16 order = 0; order < input->num_entities; order++) {
        struct Timer    *timer = &timer_arr[order];
        struct Scene    *scene = &scene_arr[order];

        tnecs_world     *world       = input->world;
        size_t           typeflag_id = input->entity_typeflag_id;
        tnecs_entity     entity      = world->entities_bytype[typeflag_id][order];

        Scene_Animate(IES, entity, scene, timer);
    }
}
