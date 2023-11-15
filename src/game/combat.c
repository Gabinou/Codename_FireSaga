#include "game/combat.h"

/* --- Combat --- */
void Game_Combat_Outcome(struct Game *sota) {
    struct Unit *aggressor, *defendant;
    defendant   = TNECS_GET_COMPONENT(sota->world, sota->defendant, Unit);
    aggressor   = TNECS_GET_COMPONENT(sota->world, sota->aggressor, Unit);
    struct Position *agg_pos_cmp, *dft_pos_cmp;
    dft_pos_cmp = TNECS_GET_COMPONENT(sota->world, sota->defendant, Position);
    agg_pos_cmp = TNECS_GET_COMPONENT(sota->world, sota->aggressor, Position);

    struct Point *agg_pos = &agg_pos_cmp->tilemap_pos;
    struct Point *dft_pos = &dft_pos_cmp->tilemap_pos;
    sota->combat_forecast = Compute_Combat_Forecast(aggressor, defendant, agg_pos, dft_pos);

    Compute_Combat_Outcome(&sota->combat_outcome, &sota->combat_forecast, aggressor, defendant);
}
