
#include "map/animation.h"

struct CombatAnimation CombatAnimation_default = {
    .attack_ind         =    0,
    .pause_before_ms    =   10,
    .pause_after_ms     = 1000,
    .frame_count        =    0,
};

struct MapAnimation MapAnimation_default = {
    .time_ns         = SOTA_ns,
};

void Map_Combat_Animate(struct Game *sota, tnecs_entity entity,
                        struct CombatAnimation *combat_anim, struct Timer *combat_timer) {
    /* --- Animate fight on the map: Units take turn hitting each other --- */
    SDL_assert(combat_anim      != NULL);
    SDL_assert(combat_timer     != NULL);
    SDL_assert(sota->aggressor  > TNECS_NULL);
    SDL_assert(sota->defendant  > TNECS_NULL);

    b32 no_more_attacks = (combat_anim->attack_ind >= sota->combat_forecast.attack_num);
    if (no_more_attacks || sota->combat_outcome.ended) {
        /* - Check for remaining attack, ending combat after pause - */
        SDL_Log("COMBATOVER");
        SDL_Log("COMBATOVER");
        SDL_Log("COMBATOVER");
        b32 paused = ((combat_timer->time_ns / SOTA_us) < combat_anim->pause_after_ms);
        SDL_Log("paused %d", paused);
        if (!paused) {
            SDL_Log("tnecs_entity_destroy");
            tnecs_entity_destroy(sota->world, entity);
            receive_event_Combat_End(sota, NULL);
            // Event_Emit(__func__, SDL_USEREVENT, event_Combat_End, NULL, NULL);
        }
        // getchar();
        /* - Skip if no more attacks to animate - */
        return;
    }

    /* - pausing attacker for constant time - */
    int attacker_i = sota->combat_outcome.phases[combat_anim->attack_ind].attacker;
    tnecs_entity attacker = attacker_i ? sota->aggressor : sota->defendant;
    SDL_assert(TNECS_ENTITY_HASCOMPONENT(sota->world, attacker, Timer));
    struct Timer *att_timer = TNECS_GET_COMPONENT(sota->world, attacker, Timer);
    SDL_assert(att_timer != NULL);
    att_timer->paused = ((combat_timer->time_ns / SOTA_us) < combat_anim->pause_before_ms);

    if (att_timer->paused) {
        return;
    }

    /* - Add RenderTop component to attacker - */
    if (!TNECS_ENTITY_HASCOMPONENT(sota->world, attacker, RenderTop)) {
        TNECS_ADD_COMPONENT(sota->world, attacker, RenderTop);
    }

    /* - combat_anim's frame count only grows - */
    struct Sprite *att_sprite = TNECS_GET_COMPONENT(sota->world, attacker, Sprite);
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
    SDL_assert(sota->aggressor  > TNECS_NULL);
    SDL_assert(sota->defendant  > TNECS_NULL);
    SDL_Event *userevent = malloc(sizeof(userevent));
    receive_event_Increment_Attack(sota, userevent);
    free(userevent);
    // Event_Emit(__func__, SDL_USEREVENT, event_Increment_Attack, NULL, NULL);

    /* - reset combat timer for next attack - */
    combat_timer->time_ns       = 0;
    /* - reset frame_count for next attack - */
    combat_anim->frame_count    = 0;
    att_timer->paused           = true;

    /* - pause defender - */
    tnecs_entity defender = attacker_i ? sota->defendant : sota->aggressor;
    SDL_assert(TNECS_ENTITY_HASCOMPONENT(sota->world, defender, Timer));
    struct Timer *def_timer = TNECS_GET_COMPONENT(sota->world, defender, Timer);
    SDL_assert(def_timer != NULL);
    def_timer->paused = true;

    /* - Remove RenderTop component from attacker - */
    if (TNECS_ENTITY_HASCOMPONENT(sota->world, attacker, RenderTop))
        TNECS_REMOVE_COMPONENTS(sota->world, attacker, RenderTop);
    if (TNECS_ENTITY_HASCOMPONENT(sota->world, defender, RenderTop))
        TNECS_REMOVE_COMPONENTS(sota->world, defender, RenderTop);
}

void Map_TurnTransition_Animate(struct Game *sota, tnecs_entity entity,
                                struct MapAnimation *map_anim, struct Timer *timer) {
    if (timer->time_ns >= map_anim->time_ns) {
        /* - Animation is complete, begin a turn - */
        SDL_LogDebug(SDL_LOG_CATEGORY_SYSTEM, "Turn Transition Finished");
        tnecs_entity_destroy(sota->world, entity);
        Event_Emit(__func__, SDL_USEREVENT, event_Turn_Begin, NULL, NULL);
    }
}

void Map_GameOver_Animate(struct Game *sota, tnecs_entity entity,
                          struct MapAnimation *map_anim, struct Timer *timer) {
    if (timer->time_ns >= map_anim->time_ns) {
        /* - Animation is complete, quit to start menu - */
        SDL_LogDebug(SDL_LOG_CATEGORY_SYSTEM, "Game Over Animation Finished");
        tnecs_entity_destroy(sota->world, entity);
        Event_Emit(__func__, SDL_USEREVENT, event_Quit, NULL, NULL);
    }
}