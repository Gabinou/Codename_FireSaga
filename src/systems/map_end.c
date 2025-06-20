
#include "map_end.h"

void System_Map_Repair(tnecs_input *input) {
    Inventory_item *inv_item_arr = TNECS_COMPONENT_ARRAY(input, Inventory_item_ID);
    for (size_t o = 0; o < input->num_entities; o++) {
        Inventory_item     *inv_item = (inv_item_arr + o);
        inv_item->used = 0;
    }
}
