
#include "turn_end.h"

void System_Cooldown_Tick(tnecs_In *input) {
    Cooldown *cd_arr = TNECS_C_ARRAY(input, Cooldown_ID);
    for (size_t o = 0; o < input->num_Es; o++) {
        Cooldown     *cooldown = (cd_arr + o);
        Cooldown_Tick(cooldown);
    }
}
