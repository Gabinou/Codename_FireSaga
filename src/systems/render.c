
#include "systems/render.h"

void scrollText(tnecs_system_input_t *in_input) {
    /* --- PRELIMINARIES --- */
    /* -- Get components arrays -- */
    struct Timer     *ut_arr = TNECS_COMPONENTS_LIST(in_input, Timer);
    struct PixelFont *pf_arr = TNECS_COMPONENTS_LIST(in_input, PixelFont);

    for (u16 order = 0; order < in_input->num_entities; order++) {
        struct Timer     *ut = (ut_arr + order);
        struct PixelFont *pf = (pf_arr + order);

        ut->reset = PixelFont_Scroll(pf, ut->time_ns);
    }
}

void animateSprite(tnecs_system_input_t *in_input) {
    /* --- PRELIMINARIES --- */
    /* -- Get components arrays -- */
    struct Timer  *ut_arr     = TNECS_COMPONENTS_LIST(in_input, Timer);
    struct Sprite *sprite_arr = TNECS_COMPONENTS_LIST(in_input, Sprite);

    for (u16 order = 0; order < in_input->num_entities; order++) {
        struct Timer       *ut     = (ut_arr     + order);
        struct Sprite      *sprite = (sprite_arr + order);

        struct Spritesheet *ss     = sprite->spritesheet;
        SDL_assert(ss != NULL);

        int loop  = ss->current_loop;
        int frame = ss->current_frame;
        SDL_assert(loop  >= 0);
        SDL_assert(frame >= 0);

        /* Only draw next frame in animation loop if enough time has passed */
        int time_ms = (int)(ut->time_ns / SOTA_us);
        if (time_ms <= ss->speeds[loop][frame])
            continue;

        ut->reset = true;
        Sprite_Animation_Loop(sprite);
    }
}

void drawCursor(tnecs_system_input_t *in_input) {
    /* --- PRELIMINARIES --- */
    /* -- Get game -- */
    struct Game *sota = in_input->user_data;
    SDL_assert(sota != NULL);

    /* -- Get components arrays -- */
    struct Sprite   *sprite_arr   = TNECS_COMPONENTS_LIST(in_input, Sprite);
    struct Position *position_arr = TNECS_COMPONENTS_LIST(in_input, Position);
    SDL_assert(sprite_arr   != NULL);
    SDL_assert(position_arr != NULL);

    int isCursor = true; /* drawCursor is exclusive to cursor */

    for (u16 order = 0; order < in_input->num_entities; order++) {
        struct Sprite   *sprite   = (sprite_arr   + order);
        struct Position *position = (position_arr + order);

        if (!sprite->visible)
            continue;

        dstrect_func_t func = dstrect_funcs[position->absolute][isCursor];
        func(sprite, &position->pixel_pos, &sota->camera);
        Sprite_Draw(sprite, sota->renderer);
    }

}

void drawMouse(tnecs_system_input_t *in_input) {
    /* --- PRELIMINARIES --- */
    /* -- Get game -- */
    struct Game *sota = in_input->user_data;
    SDL_assert(sota != NULL);

    /* -- Get components arrays -- */
    struct Sprite *sprite_arr = TNECS_COMPONENTS_LIST(in_input, Sprite);
    SDL_assert(sprite_arr != NULL);

    for (u16 order = 0; order < in_input->num_entities; order++) {
        struct Sprite *sprite = (sprite_arr + order);

        if (!sprite->visible)
            continue;

        Sprite_Draw(sprite, sota->renderer);
    }
}

void drawMapUnit(tnecs_system_input_t *in_input) {
    /* --- PRELIMINARIES --- */
    /* -- Get game -- */
    struct Game *sota = in_input->user_data;
    SDL_assert(sota != NULL);

    /* -- Get components arrays -- */
    struct Sprite   *sprite_arr   = TNECS_COMPONENTS_LIST(in_input, Sprite);
    struct Position *position_arr = TNECS_COMPONENTS_LIST(in_input, Position);

    int isCursor =  false; /* drawMapUnit is exclusive to units */

    for (u16 order = 0; order < in_input->num_entities; order++) {
        struct Sprite   *sprite   = (sprite_arr   + order);
        struct Position *position = (position_arr + order);

        if (!sprite->visible)
            continue;

        dstrect_func_t func = dstrect_funcs[position->absolute][isCursor];
        func(sprite, &position->pixel_pos, &sota->camera);
        Sprite_Draw(sprite, sota->renderer);
    }
}

void drawMap_HPBar(tnecs_system_input_t *in_input) {
    /* --- PRELIMINARIES --- */
    /* -- Get game -- */
    struct Game *sota = in_input->user_data;
    SDL_assert(sota != NULL);

    /* -- Get components arrays -- */
    struct Position *pos_arr        = TNECS_COMPONENTS_LIST(in_input, Position);
    struct MapHPBar *map_hp_bar_arr = TNECS_COMPONENTS_LIST(in_input, MapHPBar);

    for (u16 order = 0; order < in_input->num_entities; order++) {
        struct MapHPBar *map_hp_bar = (map_hp_bar_arr + order);

        if ((!map_hp_bar->visible) && (!map_hp_bar->update))
            continue;

        map_hp_bar->tilemap_pos = pos_arr[order].tilemap_pos;
        MapHPBar_Draw(map_hp_bar, &sota->camera, sota->map->tilesize, sota->render_target,
                      sota->renderer, sota->world);
    }

}

void drawPopUp(tnecs_system_input_t *in_input) {
    /* --- PRELIMINARIES --- */
    /* -- Get game -- */
    struct Game *sota = in_input->user_data;
    SDL_assert(sota != NULL);

    /* -- Get components arrays -- */
    struct PopUp    *popup_arr    = TNECS_COMPONENTS_LIST(in_input, PopUp);
    struct Position *position_arr = TNECS_COMPONENTS_LIST(in_input, Position);
    SDL_assert(popup_arr    != NULL);
    SDL_assert(position_arr != NULL);

    for (u16 order = 0; order < in_input->num_entities; order++) {
        struct PopUp *popup = (popup_arr + order);

        if (!popup->visible) /* Skip drawing if not visible */
            continue;

        struct Position *position = (position_arr + order);
        popup->draw(popup, position->pixel_pos, sota->render_target, sota->renderer);
    }
}

void drawMenu(tnecs_system_input_t *in_input) {
    /* --- PRELIMINARIES --- */
    /* -- Get game -- */
    struct Game *sota = in_input->user_data;
    SDL_assert(sota != NULL);

    /* -- Get components arrays -- */
    struct Menu *mc_arr = TNECS_COMPONENTS_LIST(in_input, Menu);
    SDL_assert(mc_arr != NULL);

    /* --- DRAWING MENU ENTITIES --- */
    for (u16 order = 0; order < in_input->num_entities; order++) {
        struct Menu *mc = (mc_arr + order);

        if (!mc->visible)
            continue;

        mc->draw(mc, sota->render_target, sota->renderer);

        #ifdef SOTA_DEBUG_MENU_BOXES
        Menu_Elem_Boxes_Draw(mc, sota->renderer);
        #endif
    }
}

void drawText(tnecs_system_input_t *in_input) {
    /* --- PRELIMINARIES --- */
    SDL_Rect dstrect;
    /* -- Get game -- */
    struct Game *sota = (struct Game *)in_input->user_data;
    SDL_assert(sota != NULL);

    /* -- Get components arrays -- */
    struct Text     *text_arr        = TNECS_COMPONENTS_LIST(in_input, Text);
    struct Position *position_arr    = TNECS_COMPONENTS_LIST(in_input, Position);
    SDL_assert(position_arr != NULL);

    /* --- DRAWING TEXT ENTITIES --- */
    for (u16 order = 0; order < in_input->num_entities; order++) {
        struct Position *pos  = (position_arr    + order);
        struct Text     *text = (text_arr        + order);

        dstrect.x = pos->pixel_pos.x;
        dstrect.y = pos->pixel_pos.y;
        dstrect.w = text->rect.w * pos->scale[0];
        dstrect.h = text->rect.h * pos->scale[1];

        Text_Draw(text, sota->renderer, &dstrect);
    }
}

void drawTextTimer(tnecs_system_input_t *in_input) {
    /* --- PRELIMINARIES --- */
    SDL_Rect dstrect;
    /* -- Get game -- */
    struct Game *sota = (struct Game *)in_input->user_data;
    SDL_assert(sota != NULL);

    /* -- Get components arrays -- */
    struct Text     *text_arr        = TNECS_COMPONENTS_LIST(in_input, Text);
    struct Position *position_arr    = TNECS_COMPONENTS_LIST(in_input, Position);
    struct Timer    *updatetimer_arr = TNECS_COMPONENTS_LIST(in_input, Timer);
    SDL_assert(position_arr != NULL);

    /* --- DRAWING TEXT ENTITIES --- */
    for (u16 order = 0; order < in_input->num_entities; order++) {
        struct Position *pos  = (position_arr    + order);
        struct Text     *text = (text_arr        + order);
        struct Timer    *ut   = (updatetimer_arr + order);

        tnecs_world_t   *world       = in_input->world;
        size_t           typeflag_id = in_input->entity_typeflag_id;
        tnecs_entity_t   entity      = world->entities_bytype[typeflag_id][order];

        if ((text->onUpdate != NULL) && (ut->time_ns >= text->update_time_ns)) {
            (*text->onUpdate)(sota, entity, ut->frame_count, ut->time_ns, NULL);
            ut->reset = true;
        }

        dstrect.x = pos->pixel_pos.x;
        dstrect.y = pos->pixel_pos.y;
        dstrect.w = text->rect.w * pos->scale[0];
        dstrect.h = text->rect.h * pos->scale[1];

        Text_Draw(text, sota->renderer, &dstrect);
    }
}

void Animate_Combat_onMap(tnecs_system_input_t *in_input) {
    /* -- Should only be ONE MapAnimation at a time -- */
    SDL_assert(in_input->num_entities <= 1);
    if (in_input->num_entities < 1)
        return;


    /* -- Get game -- */
    struct Game *sota = (struct Game *)in_input->user_data;
    SDL_assert(sota != NULL);
    SDL_assert(sota->combat_forecast.attack_num > 0);

    /* -- Get components arrays -- */
    // struct Text     *text_arr        = TNECS_COMPONENTS_LIST(in_input, Text);
    // Timer, CombatAnimation
    // struct Position *position_arr    = TNECS_COMPONENTS_LIST(in_input, Position);
    struct Timer *timer_arr = TNECS_COMPONENTS_LIST(in_input, Timer);
    struct CombatAnimation *combatanim_arr = TNECS_COMPONENTS_LIST(in_input, CombatAnimation);
    SDL_assert(timer_arr != NULL);
    SDL_assert(combatanim_arr != NULL);

    // for (u16 order = 0; order < in_input->num_entities; order++) {
    // }
    /* - getting attacker - */
    struct CombatAnimation *map_anim        = &combatanim_arr[0];
    struct Timer           *combat_timer    = &timer_arr[0];
    SDL_assert(combat_timer != NULL);
    SDL_assert(map_anim != NULL);

    if (map_anim->attack_ind >= sota->combat_forecast.attack_num) {
        /* - Check for remaining attack, ending combat after pause - */
        bool paused = ((combat_timer->time_ns / SOTA_ms) < map_anim->pause_after_ms);
        if (!paused) {
            tnecs_entity_destroy(sota->world, sota->map_animation);
            sota->map_animation = TNECS_NULL;
            Event_Emit(__func__, SDL_USEREVENT, event_Combat_End, NULL, NULL);
        }
        /* - Skip if no more attacks to animate - */
        return;
    }

    /* - pausing attacker - */
    int attacker_i = sota->combat_phases[map_anim->attack_ind].attacker;
    tnecs_entity_t attacker = attacker_i ? sota->aggressor : sota->defendant;

    struct Timer *att_timer = TNECS_GET_COMPONENT(sota->world, attacker, Timer);
    SDL_assert(att_timer != NULL);
    att_timer->paused = ((combat_timer->time_ns / SOTA_us) < map_anim->pause_before_ms);
    if (att_timer->paused) {
        return;
    }

    /* - Add RenderTop component to attacker - */
    if (!TNECS_ENTITY_HASCOMPONENT(sota->world, attacker, RenderTop)) {
        TNECS_ADD_COMPONENT(sota->world, attacker, RenderTop);
    }

    /* - map_anim's frame count only grows - */
    struct Sprite *att_sprite = TNECS_GET_COMPONENT(sota->world, attacker, Sprite);
    SDL_assert(att_sprite != NULL);
    int current_frame = att_sprite->spritesheet->current_frame;
    int frame_count = map_anim->frame_count;
    map_anim->frame_count = current_frame > frame_count ? current_frame : frame_count;

    /* - Checking if map_anim->attack_ind should be incremented - */
    if (att_timer->paused || (map_anim->frame_count == 0) || (current_frame > 0)) {
        return;
    }

    map_anim->attack_ind++;
    Event_Emit(__func__, SDL_USEREVENT, event_Increment_Attack, NULL, NULL);

    /* - reset combat timer for next attack - */
    combat_timer->time_ns = 0;
    /* - reset frame_count for next attack - */
    map_anim->frame_count = 0;
    att_timer->paused = true;

    /* - pause defender - */
    tnecs_entity_t defender = attacker_i ? sota->defendant : sota->aggressor;
    struct Timer *def_timer = TNECS_GET_COMPONENT(sota->world, defender, Timer);
    SDL_assert(def_timer != NULL);
    def_timer->paused = true;

    /* - Remove RenderTop component from attacker - */
    if (TNECS_ENTITY_HASCOMPONENT(sota->world, attacker, RenderTop))
        TNECS_REMOVE_COMPONENTS(sota->world, attacker, RenderTop);
    if (TNECS_ENTITY_HASCOMPONENT(sota->world, defender, RenderTop))
        TNECS_REMOVE_COMPONENTS(sota->world, defender, RenderTop);
}

