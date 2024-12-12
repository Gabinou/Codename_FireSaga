
#include "systems/slide.h"
#include "position.h"
#include "slider.h"
#include "sprite.h"
#include "hover.h"

void Cursor_Target(struct Slider *slider, struct Sprite *sprite, Position *position) {
    if (position->onTilemap) {
        slider->target.x  = position->tilemap_pos.x * sprite->tilesize[0];
        slider->target.x -= sprite->tilesize[0] / 2;
        slider->target.y  = position->tilemap_pos.y * sprite->tilesize[1];
        slider->target.y -= sprite->tilesize[1] / 2;
    } else {
        slider->target.x = position->pixel_pos.x;
        slider->target.y = position->pixel_pos.y;
    }
}

void Sprite_Target(struct Slider *slider, struct Sprite *sprite, Position *position) {
    slider->target.x = (i32)lround(position->tilemap_pos.x * position->scale[0]);
    slider->target.y = (i32)lround(position->tilemap_pos.y * position->scale[1]);
}

void Hover_Any(tnecs_system_input *input) {
    /* --- PRELIMINARIES --- */
    /* -- Get components arrays -- */
    struct Hover    *hover_arr    = TNECS_COMPONENTS_LIST(input, Hover);
    struct Position *position_arr = TNECS_COMPONENTS_LIST(input, Position);
    SDL_assert(hover_arr    != NULL);
    SDL_assert(position_arr != NULL);

    for (u16 order = 0; order < input->num_entities; order++) {
        struct Hover    *hover    = hover_arr    + order;
        struct Position *position = position_arr + order;

        hover->timer_ns += input->deltat;
        if (hover->timer_ns <= hover->update_wait_ns)
            continue;

        Hover_Compute_Next(hover, &position->pixel_pos);
        hover->timer_ns = 0;
    }
}

/* Slide systems */
/* Compute next Popup position according to target position */
void Slide_Sprite(tnecs_system_input *input) {
    /* --- PRELIMINARIES --- */
    SDL_assert(input->user_data != NULL);
    /* -- Get components arrays -- */
    struct Sprite   *sprite_arr     = TNECS_COMPONENTS_LIST(input, Sprite);
    struct Slider   *slider_arr     = TNECS_COMPONENTS_LIST(input, Slider);
    struct Position *position_arr   = TNECS_COMPONENTS_LIST(input, Position);
    SDL_assert(sprite_arr   != NULL);
    SDL_assert(slider_arr   != NULL);
    SDL_assert(position_arr != NULL);

    /* -- Get Game -- */
    struct Game *sota = input->user_data;
    SDL_assert(sota != NULL);

    /* -- Check if entity is cursor -- */
    tnecs_component cursor_Component_Flag = TNECS_COMPONENT_NAME2TYPE(input->world, CursorFlag);

    for (u16 order = 0; order < input->num_entities; order++) {
        struct Position *position   = position_arr  + order;
        struct Slider   *slider     = slider_arr    + order;
        struct Sprite   *sprite     = sprite_arr    + order;

        tnecs_component typeflag_id   = input->entity_typeflag_id;
        tnecs_entity current_ent      = input->world->entities_bytype[typeflag_id][order];
        tnecs_component ent_typeflag  = input->world->entity_typeflags[current_ent];
        int isCursor = TNECS_TYPEFLAG_HAS_TYPE(cursor_Component_Flag, ent_typeflag);

        if (!sprite->visible)
            continue;

        if (isCursor)
            Cursor_Target(slider, sprite, position);
        else
            Sprite_Target(slider, sprite, position);

        slider->timer_ns += input->deltat;
        if (slider->timer_ns > slider->update_wait_ns) {
            Slider_Compute_Next(slider, &position->pixel_pos, &slider->target, false);
            slider->timer_ns = 0;
        }

        dstrect_func_t func = dstrect_funcs[!position->onTilemap][isCursor];
        func(sprite, &position->pixel_pos, &sota->camera);
    }
}

/* slidePopUp MAY RUN TWICE on popups WITH slidePopUpOffscreen */
void Slide_PopUp(tnecs_system_input *input) {
    /* --- PRELIMINARIES --- */
    SDL_assert(input->user_data != NULL);
    /* -- Get components arrays -- */
    struct Slider   *slider_arr     = TNECS_COMPONENTS_LIST(input, Slider);
    struct Position *position_arr   = TNECS_COMPONENTS_LIST(input, Position);
    SDL_assert(slider_arr   != NULL);
    SDL_assert(position_arr != NULL);

    for (u16 order = 0; order < input->num_entities; order++) {
        struct Slider   *slider     = slider_arr   + order;
        struct Position *position   = position_arr + order;
        slider->timer_ns += input->deltat;

        if (slider->timer_ns <= slider->update_wait_ns)
            continue;

        Slider_Compute_Next(slider, &position->pixel_pos, &slider->target, false);
        slider->timer_ns = 0;
    }
}

void Slide_PopUp_Offscreen(tnecs_system_input *input) {
    /* --- PRELIMINARIES --- */
    SDL_assert(input->user_data != NULL);
    /* -- Get components arrays -- */
    struct Slider          *slider_arr      = TNECS_COMPONENTS_LIST(input, Slider);
    struct Position        *position_arr    = TNECS_COMPONENTS_LIST(input, Position);
    struct SliderOffscreen *offscreen_arr   = TNECS_COMPONENTS_LIST(input, SliderOffscreen);
    SDL_assert(slider_arr    != NULL);
    SDL_assert(position_arr  != NULL);
    SDL_assert(offscreen_arr != NULL);

    for (u16 order = 0; order < input->num_entities; order++) {
        struct Slider          *slider        = slider_arr    + order;
        struct Position        *position      = position_arr  + order;
        struct SliderOffscreen *offscreen_ptr = offscreen_arr + order;
        slider->timer_ns += input->deltat;

        if (slider->timer_ns <= slider->update_wait_ns)
            continue;

        /* offscreen slide / onscreen slide switch */
#ifdef DEBUG_POPUP_TILE_OFFSCREEN
        SliderOffscreen_Compute_Next(slider, offscreen_ptr, &position->pixel_pos);
#else
        Slider_Compute_Next(slider, &position->pixel_pos, &slider->target, false);
#endif
        slider->timer_ns = 0;
    }
}
