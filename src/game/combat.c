
#include "combat.h"
#include "globals.h"
#include "position.h"

#include "game/game.h"
#include "game/combat.h"

#include "unit/unit.h"

/* --- Combat --- */
/* Note: this should be called after units are moved? */
void Game_Combat_Outcome(Game *sota) {
    Unit        *aggressor      = NULL;
    Unit        *defendant      = NULL;
    Position    *agg_pos_cmp    = NULL;
    Position    *dft_pos_cmp    = NULL;
    defendant   = IES_GET_C(gl_world, sota->combat.defendant, Unit);
    aggressor   = IES_GET_C(gl_world, sota->combat.aggressor, Unit);
    dft_pos_cmp = IES_GET_C(gl_world, sota->combat.defendant,
                            Position);
    agg_pos_cmp = IES_GET_C(gl_world, sota->combat.aggressor,
                            Position);
    const s8 agg_name = Unit_Name(aggressor);
    const s8 dft_name = Unit_Name(defendant);
    SDL_Log("Combat outcome for %s -> %s",  agg_name.data,
            dft_name.data);

    Point *agg_pos = &agg_pos_cmp->tilemap_pos;
    Point *dft_pos = &dft_pos_cmp->tilemap_pos;
    SDL_Log("Pos %s -> %d,%d",  agg_name.data,
            agg_pos->x,
            agg_pos->y);

    SDL_Log("Pos %s -> %d,%d",  dft_name.data,
            dft_pos->x,
            dft_pos->y);

    sota->combat.forecast = Compute_Combat_Forecast(aggressor,
                                                    defendant,
                                                    agg_pos,
                                                    dft_pos);

    Compute_Combat_Outcome( &sota->combat.outcome,
                            &sota->combat.forecast,
                            aggressor, defendant);
}
