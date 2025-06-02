
#include "turn_end.h"

void System_Cooldown_Tick(tnecs_input *input) {
    Cooldown     *cd_arr = TNECS_COMPONENT_ARRAY(input, Timer_ID);
    for (size_t o = 0; o < input->num_entities; o++) {
        Cooldown     *cooldown = (cd_arr + o);
        Cooldown_Tick(cooldown);
    }
}
