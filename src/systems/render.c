
#include "systems/render.h"

void scrollText(tnecs_system_input_t *in_input) {
    SOTA_Log_FPS("%d\t%s\t" STRINGIZE(__LINE__), call_stack_depth++, __func__);
    /* --- PRELIMINARIES --- */
    /* -- Get components arrays -- */
    struct Timer     *ut_arr = TNECS_COMPONENTS_LIST(in_input, Timer);
    struct PixelFont *pf_arr = TNECS_COMPONENTS_LIST(in_input, PixelFont);

    for (u16 order = 0; order < in_input->num_entities; order++) {
        struct Timer     *ut = (ut_arr + order);
        struct PixelFont *pf = (pf_arr + order);

        ut->reset = PixelFont_Scroll(pf, ut->time_ns);
    }
    SOTA_Log_FPS("%d\t%s\t" STRINGIZE(__LINE__), --call_stack_depth, __func__);
}

void animateSprite(tnecs_system_input_t *in_input) {
    SOTA_Log_FPS("%d\t%s\t" STRINGIZE(__LINE__), call_stack_depth++, __func__);
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
    SOTA_Log_FPS("%d\t%s\t" STRINGIZE(__LINE__), --call_stack_depth, __func__);
}

void drawCursor(tnecs_system_input_t *in_input) {
    SOTA_Log_FPS("%d\t%s\t" STRINGIZE(__LINE__), call_stack_depth++, __func__);
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

    SOTA_Log_FPS("%d\t%s\t" STRINGIZE(__LINE__), --call_stack_depth, __func__);
}

void drawMouse(tnecs_system_input_t *in_input) {
    SOTA_Log_FPS("%d\t%s\t" STRINGIZE(__LINE__), call_stack_depth++, __func__);
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
    SOTA_Log_FPS("%d\t%s\t" STRINGIZE(__LINE__), --call_stack_depth, __func__);
}

void drawMapUnit(tnecs_system_input_t *in_input) {
    SOTA_Log_FPS("%d\t%s\t" STRINGIZE(__LINE__), call_stack_depth++, __func__);
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
    SOTA_Log_FPS("%d\t%s\t" STRINGIZE(__LINE__), --call_stack_depth, __func__);
}

void drawMap_HPBar(tnecs_system_input_t *in_input) {
    SOTA_Log_FPS("%d\t%s\t" STRINGIZE(__LINE__), call_stack_depth++, __func__);
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

    SOTA_Log_FPS("%d\t%s\t" STRINGIZE(__LINE__), --call_stack_depth, __func__);
}

void drawPopUp(tnecs_system_input_t *in_input) {
    SOTA_Log_FPS("%d\t%s\t" STRINGIZE(__LINE__), call_stack_depth++, __func__);
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
    SOTA_Log_FPS("%d\t%s\t" STRINGIZE(__LINE__), --call_stack_depth, __func__);
}

void drawMenu(tnecs_system_input_t *in_input) {
    SOTA_Log_FPS("%d\t%s\t" STRINGIZE(__LINE__), call_stack_depth++, __func__);
    /* --- PRELIMINARIES --- */
    /* -- Get game -- */
    struct Game *sota = in_input->user_data;
    SDL_assert(sota != NULL);

    /* -- Get components arrays -- */
    struct MenuComponent *mc_arr = TNECS_COMPONENTS_LIST(in_input, MenuComponent);
    SDL_assert(mc_arr != NULL);

    /* --- DRAWING MENU ENTITIES --- */
    for (u16 order = 0; order < in_input->num_entities; order++) {
        struct MenuComponent *mc = (mc_arr + order);

        if (!mc->visible)
            continue;

        mc->draw(mc, sota->render_target, sota->renderer);

        #ifdef SOTA_DEBUG_MENU_BOXES
        MenuComponent_Elem_Boxes_Draw(mc, sota->renderer);
        #endif
    }
    SOTA_Log_FPS("%d\t%s\t" STRINGIZE(__LINE__), --call_stack_depth, __func__);
}

void drawText(tnecs_system_input_t *in_input) {
    SOTA_Log_FPS("%d\t%s\t" STRINGIZE(__LINE__), call_stack_depth++, __func__);
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
    SOTA_Log_FPS("%d\t%s\t" STRINGIZE(__LINE__), --call_stack_depth, __func__);
}

void drawTextTimer(tnecs_system_input_t *in_input) {
    SOTA_Log_FPS("%d\t%s\t" STRINGIZE(__LINE__), call_stack_depth++, __func__);
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
    SOTA_Log_FPS("%d\t%s\t" STRINGIZE(__LINE__), --call_stack_depth, __func__);
}
