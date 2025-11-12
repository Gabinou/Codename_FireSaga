
#include "map_end.h"

void System_Map_Repair(tnecs_In *input) {
    /* Repair items at the end of chapter. */
    InvItem *inv_item_arr = TNECS_C_ARRAY(input, InvItem_ID);
    for (size_t o = 0; o < input->num_Es; o++) {
        InvItem     *inv_item = (inv_item_arr + o);
        inv_item->used = 0;
    }
}
