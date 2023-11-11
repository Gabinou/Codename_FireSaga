
#include "map/animation.h"

void CombatAnimation_Play(struct Game *sota, tnecs_entity_t entity,
                          struct CombatAnimation *map_anim, struct Timer *combat_timer) {
    SDL_assert(map_anim     != NULL);
    SDL_assert(combat_timer != NULL);

    if (map_anim->attack_ind >= sota->combat_forecast.attack_num) {
        /* - Check for remaining attack, ending combat after pause - */
        bool paused = ((combat_timer->time_ns / SOTA_ms) < map_anim->pause_after_ms);
        if (!paused) {
            tnecs_entity_destroy(sota->world, entity);
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