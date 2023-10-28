#include "game/combat.h"

/* --- Combat --- */
void Game_Combat_Outcome(struct Game *sota) {

    struct Unit *defendant       = TNECS_GET_COMPONENT(sota->world, sota->defendant, Unit);
    struct Unit *aggressor       = TNECS_GET_COMPONENT(sota->world, sota->aggressor, Unit);
    struct Position *dft_pos_cmp = TNECS_GET_COMPONENT(sota->world, sota->defendant, Position);
    struct Position *agg_pos_cmp = TNECS_GET_COMPONENT(sota->world, sota->aggressor, Position);

    const struct Point *agg_pos = (struct Point *)&agg_pos_cmp->tilemap_pos;
    const struct Point *dft_pos = (struct Point *)&dft_pos_cmp->tilemap_pos;
    sota->combat_forecast = Compute_Combat_Forecast(aggressor, defendant, agg_pos, dft_pos);

    Compute_Combat_Outcome(sota->combat_phases, sota->combat_attacks, &sota->combat_forecast,
                           &sota->tinymt32, aggressor, defendant);

}
