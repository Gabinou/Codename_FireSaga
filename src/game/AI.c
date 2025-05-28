
void Game_AI_Enemy_Turn(struct Game *sota) {
    SDL_Log(__func__);
    /* --- AI CONTROL --- */
    // TODO: ai_control entity
    // TODO: Don't check for loss every frame
    if (Map_isLost(sota->map)) {
        // SDL_Log("AI CONTROL -> LOSS");
        Event_Emit(__func__, SDL_USEREVENT, event_Game_Over, NULL, NULL);
        return;
    }

    /* -- Reinforcements timer: pause before moving units -- */
    /* -- TODO: Rename to AI timer? */
    // TODO: Animate reinforcements
    if (sota->timers.reinf != TNECS_NULL) {
        struct Timer *timer = IES_GET_COMPONENT(sota->ecs.world, sota->timers.reinf, Timer);
        SDL_assert(timer != NULL);
        // u64 limit = sota->settings.enemy_turn_settings.pause_post_reinforcement;
        if (timer->time_ns <= timer->limit_ns)
            return;

        tnecs_entity_destroy(sota->ecs.world, sota->timers.reinf);
        sota->timers.reinf = TNECS_NULL;
    }

    /* -- Skip if turn is over -- */
    if (sota->state.ai.turn_over)
        return;

    /* -- Build list of npcs to control -- */
    if (sota->state.ai.init == false) {
        SDL_LogDebug(SOTA_LOG_AI, "Building NPC list");
        AI_State_Init(&sota->state.ai, sota->ecs.world, sota->map);
    }
    SDL_assert(sota->state.ai.npcs != NULL);

    /* -- If no more NPCs, end NPC turn. -- */
    if (sota->state.ai.npcs && (DARR_NUM(sota->state.ai.npcs) < 1)) {
        // SDL_Log("AI Turn Finished");
        AI_State_Turn_Finish(&sota->state.ai);
        Event_Emit(__func__, SDL_USEREVENT, event_Turn_End, NULL, NULL);
        return;
    }

    /* -- Decide next NPC to act -- */
    if (sota->state.ai.npc_i < 0) {
        tnecs_entity debug = AI_Decide_Next(sota);
        SDL_LogDebug(SOTA_LOG_AI, "Next npc entity: %lld", debug);
    }
    tnecs_entity npc_ent = sota->state.ai.npcs[sota->state.ai.npc_i];

    SDL_assert(npc_ent != TNECS_NULL);

    /* -- AI decides what to do with unit -- */
    // If not previously decided for npc_ent, decide
    b32 decided     = sota->state.ai.decided;
    if (!decided) {
        SDL_LogDebug(SOTA_LOG_AI, "AI_Decide");
        AI_Decide_Action(sota, npc_ent, &sota->state.ai.action);
        AI_Decide_Move(  sota, npc_ent, &sota->state.ai.action);
        sota->state.ai.decided = true;
    }

    decided         = sota->state.ai.decided;
    b32 act_anim    = sota->state.ai.act_anim;
    b32 move_anim   = sota->state.ai.move_anim;

    /* -- AI moves unit -- */
    // TODO: wait on combat to finish!
    if (decided && !move_anim && !act_anim) {
        SDL_LogDebug(SOTA_LOG_AI, "AI_Move");
        SDL_assert(!act_anim);
        AI_Move(sota, npc_ent, &sota->state.ai.action);
        // TODO: Move animation
        sota->state.ai.move_anim = true;
    }

    /* Check if move_anim updated during frame */
    move_anim   = sota->state.ai.move_anim;
    act_anim    = sota->state.ai.act_anim;

    /* -- AI acts unit -- */
    if (decided && move_anim && !act_anim) {
        SDL_LogDebug(SOTA_LOG_AI, "AI_Act");
        AI_Act(sota, npc_ent, &sota->state.ai.action);
        // TODO: Act animation
        sota->state.ai.act_anim = true;
    }

    /* Check if act_anim updated during frame */
    act_anim    = sota->state.ai.act_anim;

    /* -- Pop unit from list in AI_State -- */
    if ((act_anim) && ((DARR_NUM(sota->state.ai.npcs) > 0))) {
        SDL_LogDebug(SOTA_LOG_AI, "AI_Pop");
        AI_State_Pop(&sota->state.ai, sota->ecs.world);

        SDL_LogDebug(SOTA_LOG_AI, "AI: Pause AFTER AI_act");
        /* Pause AFTER AI action */
        sota->timers.reinf   = TNECS_ENTITY_CREATE_wCOMPONENTS(sota->ecs.world, Timer_ID);
        struct Timer *timer = IES_GET_COMPONENT(sota->ecs.world, sota->timers.reinf, Timer);
        *timer = Timer_default;
        timer->limit_ns = sota->settings.enemy_turn_settings.pause_post_move;
    }
}