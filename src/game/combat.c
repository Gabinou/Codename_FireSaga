
#include "game/combat.h"
#include "game/game.h"
#include "combat.h"
#include "globals.h"
#include "position.h"

/* --- Combat --- */
void Game_Combat_Outcome(struct Game *sota) {
    struct Unit *aggressor, *defendant;
    defendant   = IES_GET_COMPONENT(gl_world, sota->combat.defendant, Unit);
    aggressor   = IES_GET_COMPONENT(gl_world, sota->combat.aggressor, Unit);
    struct Position *agg_pos_cmp, *dft_pos_cmp;
    dft_pos_cmp = IES_GET_COMPONENT(gl_world, sota->combat.defendant, Position);
    agg_pos_cmp = IES_GET_COMPONENT(gl_world, sota->combat.aggressor, Position);

    struct Point *agg_pos = &agg_pos_cmp->tilemap_pos;
    struct Point *dft_pos = &dft_pos_cmp->tilemap_pos;
    sota->combat.forecast = Compute_Combat_Forecast(aggressor, defendant, agg_pos, dft_pos);

    Compute_Combat_Outcome(&sota->combat.outcome, &sota->combat.forecast, aggressor, defendant);
}
