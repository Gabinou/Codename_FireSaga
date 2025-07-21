
#include "systems/slide.h"
#include "position.h"
#include "debug.h"
#include "slider.h"
#include "map/map.h"
#include "game/map.h"
#include "game/game.h"
#include "sprite.h"
#include "structs.h"
#include "actor.h"
#include "hover.h"

// TODO: mv non-systems into relevant files
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

void Hover_Any(tnecs_input *input) {
    /* --- PRELIMINARIES --- */
    /* -- Get components arrays -- */
    struct Hover    *hover_arr    = TNECS_COMPONENT_ARRAY(input, Hover_ID);
    struct Position *position_arr = TNECS_COMPONENT_ARRAY(input, Position_ID);
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
/* Slide_Update_FPS: Slider needs fps.instant
** to compute slide speed when game is lagging.
** Should:
** - Call AFTER Text_onUpdate_FPS -> Draw_Text_Timer
**      - Draw_Text_Timer is in a subsequent phase
** - Call BEFORE ALL OTHER SLIDE SYSTEMS. (OK)
*/
void Slide_Update_FPS(tnecs_input *input) {
    /* --- PRELIMINARIES --- */
    SDL_assert(input->data != NULL);
    /* -- Get components arrays -- */
    Slider *slider_arr = TNECS_COMPONENT_ARRAY(input, Slider_ID);
    SDL_assert(slider_arr   != NULL);

    /* -- Get Game -- */
    struct Game *IES = input->data;
    SDL_assert(IES != NULL);

    /* -- Check if entity is cursor -- */
    tnecs_component cursor_archetype = TNECS_COMPONENT_ID2TYPE(CursorFlag_ID);
    Map *map = Game_Map(IES);
    struct Camera camera = (map == NULL) ? Camera_default : map->render.camera;

    for (i32 o = 0; o < input->num_entities; o++) {
        Slider *slider = slider_arr + o;
        slider->fps_current = IES->fps.instant;
    }
}

/* Slide_Sprite: Compute next Popup position
** according to target position */
void Slide_Sprite(tnecs_input *input) {
    /* --- PRELIMINARIES --- */
    SDL_assert(input->data != NULL);
    /* -- Get components arrays -- */
    struct Sprite   *sprite_arr     = TNECS_COMPONENT_ARRAY(input, Sprite_ID);
    struct Slider   *slider_arr     = TNECS_COMPONENT_ARRAY(input, Slider_ID);
    struct Position *position_arr   = TNECS_COMPONENT_ARRAY(input, Position_ID);
    SDL_assert(sprite_arr   != NULL);
    SDL_assert(slider_arr   != NULL);
    SDL_assert(position_arr != NULL);

    /* -- Get Game -- */
    struct Game *IES = input->data;
    SDL_assert(IES != NULL);

    /* -- Check if entity is cursor -- */
    tnecs_component cursor_archetype = TNECS_COMPONENT_ID2TYPE(CursorFlag_ID);
    Map *map = Game_Map(IES);
    struct Camera camera = (map == NULL) ? Camera_default : map->render.camera;

    for (u16 order = 0; order < input->num_entities; order++) {
        struct Position *position   = position_arr  + order;
        struct Slider   *slider     = slider_arr    + order;
        struct Sprite   *sprite     = sprite_arr    + order;

        tnecs_component archetype_id   = input->entity_archetype_id;
        tnecs_entity current_ent      = input->world->bytype.entities[archetype_id][order];
        tnecs_component ent_archetype  = input->world->entities.archetypes[current_ent];
        int isCursor = TNECS_ARCHETYPE_HAS_TYPE(cursor_archetype, ent_archetype);

        if (!sprite->visible)
            continue;

        if (isCursor)
            Cursor_Target(slider, sprite, position);
        else
            Sprite_Target(slider, sprite, position);

        struct SliderInput input    = SliderInput_default;
        input.slider                = slider;
        input.pos                   = &position->pixel_pos;
        input.target                = slider->target;
        input.reverse          = false;

        Slider_Compute_Next(input);

        dstrect_func_t func = dstrect_funcs[!position->onTilemap][isCursor];
        func(sprite, &position->pixel_pos, &camera);
    }
}

void Slide_PopUp_Offscreen(tnecs_input *input) {
    /* --- PRELIMINARIES --- */
    SDL_assert(input->data != NULL);
    /* -- Get components arrays -- */
    struct Slider          *slider_arr      = TNECS_COMPONENT_ARRAY(input, Slider_ID);
    struct Position        *position_arr    = TNECS_COMPONENT_ARRAY(input, Position_ID);
    struct SliderOffscreen *offscreen_arr   = TNECS_COMPONENT_ARRAY(input, SliderOffscreen_ID);
    SDL_assert(slider_arr    != NULL);
    SDL_assert(position_arr  != NULL);
    SDL_assert(offscreen_arr != NULL);

    for (u16 order = 0; order < input->num_entities; order++) {
        struct Slider          *slider      = slider_arr    + order;
        struct Position        *position    = position_arr  + order;
        struct SliderOffscreen *offscreen   = offscreen_arr + order;

        /* offscreen slide / onscreen slide switch */
        struct SliderInput input    = SliderInput_default;
        input.slider                = slider;
        input.offscreen             = offscreen;
        input.pos                   = &position->pixel_pos;
        input.target                = slider->target;
        input.reverse          = offscreen->reverse;

#ifdef DEBUG_POPUP_TILE_OFFSCREEN
        SliderOffscreen_Compute_Next(input);
#else
        Slider_Compute_Next(input);
#endif
    }
}

void Slide_Actor(tnecs_input *input) {
    /* --- PRELIMINARIES --- */
    SDL_assert(input->data != NULL);
    /* -- Get components arrays -- */
    struct Actor   *actor_arr       = TNECS_COMPONENT_ARRAY(input, Actor_ID);
    struct Slider   *slider_arr     = TNECS_COMPONENT_ARRAY(input, Slider_ID);
    struct Position *position_arr   = TNECS_COMPONENT_ARRAY(input, Position_ID);
    SDL_assert(actor_arr    != NULL);
    SDL_assert(slider_arr   != NULL);
    SDL_assert(position_arr != NULL);

    /* -- Get Game -- */
    struct Game *IES = input->data;
    SDL_assert(IES != NULL);

    for (u16 order = 0; order < input->num_entities; order++) {
        struct Actor    *actor      = actor_arr     + order;
        struct Slider   *slider     = slider_arr    + order;
        struct Position *position   = position_arr  + order;

        if (!actor->visible) {
            continue;
        }

        if (!slider->active) {
            continue;
        }

        struct SliderInput input    = SliderInput_default;
        input.slider                = slider;
        input.pos                   = &position->pixel_pos;
        input.target                = slider->target;
        input.reverse               = false;

        Slider_Compute_Next(input);
    }
}
