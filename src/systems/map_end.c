
#include "map_end.h"

void System_Map_Repair(tnecs_In *input) {
    Inventory_item *inv_item_arr = TNECS_C_ARRAY(input, Inventory_item_ID);
    for (size_t o = 0; o < input->num_Es; o++) {
        Inventory_item     *inv_item = (inv_item_arr + o);
        inv_item->used = 0;
    }
}
