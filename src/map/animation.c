/*
**  Copyright 2025 Gabriel Taillon
**  Licensed under GPLv3
**
**      Éloigne de moi l'esprit d'oisiveté, de
**          découragement, de domination et de
**          vaines paroles.
**      Accorde-moi l'esprit d'intégrité,
**          d'humilité, de patience et de charité.
**      Donne-moi de voir mes fautes.
**
***************************************************
**
** Animation for units on the map
**
*/

#include "map/animation.h"
#include "game/game.h"
#include "sprite.h"
#include "globals.h"
#include "events.h"

const struct CombatAnimation CombatAnimation_default = {
    .pause_before_ms    =   10,
    .pause_after_ms     = 1000,
};

const struct MapAnimation MapAnimation_default = {
    .time_ns         = SOTA_ns,
};

void MapAnimation_Init(struct MapAnimation *manim) {
    *manim = MapAnimation_default;
}
void MapAnimation_Init_tnecs(void *voidmanim) {
    MapAnimation_Init(voidmanim);
}
void CombatAnimation_Init(struct CombatAnimation *canim) {
    *canim = CombatAnimation_default;
}
void CombatAnimation_Init_tnecs(void *voidcanim) {
    CombatAnimation_Init(voidcanim);
}


void Map_Combat_Animate(struct Game *sota, tnecs_entity entity,
                        struct CombatAnimation *combat_anim, struct Timer *combat_timer) {
    /* --- Animate fight on the map: Units take turn hitting each other --- */
    SDL_assert(combat_anim      != NULL);
    SDL_assert(combat_timer     != NULL);
    SDL_assert(sota->combat.aggressor  > TNECS_NULL);
    SDL_assert(sota->combat.defendant  > TNECS_NULL);

    b32 no_more_attacks = (combat_anim->attack_ind >= sota->combat.forecast.attack_num);
    if (no_more_attacks || sota->combat.outcome.ended) {
        /* - Check for remaining attack, ending combat after pause - */
        b32 paused = ((combat_timer->time_ns / SOTA_us) < combat_anim->pause_after_ms);
        if (!paused) {
            tnecs_entity_destroy(gl_world, entity);
            receive_event_Combat_End(sota, NULL);
            // Event_Emit(__func__, SDL_USEREVENT, event_Combat_End, NULL, NULL);
        }
        /* - Skip if no more attacks to animate - */
        return;
    }

    /* - pausing attacker for constant time - */
    int attacker_i = sota->combat.outcome.phases[combat_anim->attack_ind].attacker;
    tnecs_entity attacker = attacker_i ? sota->combat.aggressor : sota->combat.defendant;
    SDL_assert(IES_ENTITY_HASCOMPONENT(gl_world, attacker, Timer));
    struct Timer *att_timer = IES_GET_COMPONENT(gl_world, attacker, Timer);
    SDL_assert(att_timer != NULL);
    att_timer->paused = ((combat_timer->time_ns / SOTA_us) < combat_anim->pause_before_ms);

    if (att_timer->paused) {
        return;
    }

    /* - Add RenderTop component to attacker - */
    if (!IES_ENTITY_HASCOMPONENT(gl_world, attacker, RenderTop)) {
        TNECS_ADD_COMPONENT(gl_world, attacker, RenderTop_ID);
    }

    /* - combat_anim's frame count only grows - */
    struct Sprite *att_sprite = IES_GET_COMPONENT(gl_world, attacker, Sprite);
    SDL_assert(att_sprite != NULL);
    int current_frame   = att_sprite->spritesheet->current_frame;
    int current_loop    = att_sprite->spritesheet->current_loop;
    int frame_num       = att_sprite->spritesheet->frames[current_loop];
    int frame_count     = combat_anim->frame_count;
    combat_anim->frame_count = current_frame > frame_count ? current_frame : frame_count;

    b32 attack_anim_done    = (combat_anim->frame_count >= (frame_num - 1));
    b32 paused              = att_timer->paused;

    /* - Checking if combat_anim->attack_ind should be incremented - */
    if (paused || !attack_anim_done) {
        /* Don't increment, keep animating */
        return;
    }

    combat_anim->attack_ind++;
    SDL_assert(sota->combat.aggressor  > TNECS_NULL);
    SDL_assert(sota->combat.defendant  > TNECS_NULL);
    SDL_Event *userevent = SDL_malloc(sizeof(*userevent));
    receive_event_Increment_Attack(sota, userevent);
    SDL_free(userevent);
    // Event_Emit(__func__, SDL_USEREVENT, event_Increment_Attack, NULL, NULL);

    /* - reset combat timer for next attack - */
    combat_timer->time_ns       = 0;
    /* - reset frame_count for next attack - */
    combat_anim->frame_count    = 0;
    att_timer->paused           = true;

    /* - pause defender - */
    tnecs_entity defender = attacker_i ? sota->combat.defendant : sota->combat.aggressor;
    SDL_assert(IES_ENTITY_HASCOMPONENT(gl_world, defender, Timer));
    struct Timer *def_timer = IES_GET_COMPONENT(gl_world, defender, Timer);
    SDL_assert(def_timer != NULL);
    def_timer->paused = true;

    /* - Remove RenderTop component from attacker - */
    if (IES_ENTITY_HASCOMPONENT(gl_world, attacker, RenderTop))
        TNECS_REMOVE_COMPONENTS(gl_world, attacker, RenderTop_ID);
    if (IES_ENTITY_HASCOMPONENT(gl_world, defender, RenderTop))
        TNECS_REMOVE_COMPONENTS(gl_world, defender, RenderTop_ID);
}

void Map_TurnTransition_Animate(struct Game *sota, tnecs_entity entity,
                                struct MapAnimation *map_anim, struct Timer *timer) {
    if (timer->time_ns >= map_anim->time_ns) {
        /* - Animation is complete, begin a turn - */
        SDL_LogDebug(SDL_LOG_CATEGORY_SYSTEM, "Turn Transition Finished");
        tnecs_entity_destroy(gl_world, entity);
        Event_Emit(__func__, SDL_USEREVENT, event_Turn_Start, NULL, NULL);
    }
}

void Map_GameOver_Animate(struct Game *sota, tnecs_entity entity,
                          struct MapAnimation *map_anim, struct Timer *timer) {
    if (timer->time_ns >= map_anim->time_ns) {
        /* - Animation is complete, quit to start menu - */
        SDL_LogDebug(SDL_LOG_CATEGORY_SYSTEM, "Game Over Animation Finished");
        tnecs_entity_destroy(gl_world, entity);
        Event_Emit(__func__, SDL_USEREVENT, event_Quit, NULL, NULL);
    }
}