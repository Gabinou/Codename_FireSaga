
#include "convoy.hpp"

Convoy::Convoy() {

}

void Convoy::deposit(Inventory_item in_item) {
    if (!full) {
        short unsigned int type = all_weapons[in_item.id].getType();

        if ((type & ITEM::TYPE::SWORD) > 0) {
            swords[quantity.swords] = in_item;
            quantity.swords += 1;
        }
        if ((type & ITEM::TYPE::LANCE) > 0) {
            lances[quantity.lances] = in_item;
            quantity.lances += 1;
        }
        if ((type & ITEM::TYPE::AXE) > 0) {
            axes[quantity.axes] = in_item;
            quantity.axes += 1;
        }
        if ((type & ITEM::TYPE::BOW) > 0) {
            bows[quantity.bows] = in_item;
            quantity.bows += 1;
        }
        if ((type & ITEM::TYPE::TRINKET) > 0) {
            trinkets[quantity.trinkets] = in_item;
            quantity.trinkets += 1;
        }
        if ((type & ITEM::TYPE::OFFHAND) > 0) {
            offhands[quantity.offhands] = in_item;
            quantity.offhands += 1;
        }
        if ((type & ITEM::TYPE::ELEMENTAL) > 0) {
            elemental[quantity.elemental] = in_item;
            quantity.elemental += 1;
        }
        if ((type & ITEM::TYPE::DEMONIC) > 0) {
            demonic[quantity.demonic] = in_item;
            quantity.demonic += 1;
        }
        if ((type & ITEM::TYPE::ANGELIC) > 0) {
            angelic[quantity.angelic] = in_item;
            quantity.angelic += 1;
        }
        if ((type & ITEM::TYPE::SHIELD) > 0) {
            shields[quantity.shields] = in_item;
            quantity.shields += 1;
        }
        if ((type & ITEM::TYPE::STAFF) > 0) {
            staffs[quantity.staffs] = in_item;
            quantity.staffs += 1;
        }
        if ((type & ITEM::TYPE::CLAW) > 0) {
            claws[quantity.claws] = in_item;
            quantity.claws += 1;
        }
        if ((type & ITEM::TYPE::ITEM) > 0) {
            items[quantity.items] = in_item;
            quantity.items += 1;
        }
        int sum = quantity.swords + quantity.lances + quantity.axes +
            quantity.bows + quantity.trinkets + quantity.offhands + quantity.elemental +
            quantity.demonic + quantity.angelic + quantity.shields + 
            quantity.staffs + quantity.claws + quantity.items;
        if (sum >= DEFAULT::CONVOY_SIZE) {
            full = true;
        }
    }
} 
      

int * Convoy::getarr(int type) {
    static int temparr[DEFAULT::CONVOY_SIZE];
    Inventory_item temp[DEFAULT::CONVOY_SIZE];
    if ((type & ITEM::TYPE::SWORD) > 0) {
        memcpy(temp, swords, DEFAULT::CONVOY_SIZE);
    }
    // if ((type & ITEM::TYPE::LANCE) > 0) {
    //     temp = lances;
    // }
    // if ((type & ITEM::TYPE::AXE) > 0) {
    //     temp = axes;
    // }
    // if ((type & ITEM::TYPE::BOW) > 0) {
    //     temp = bows;
    // }
    // if ((type & ITEM::TYPE::TRINKET) > 0) {
    //     temp = trinkets;
    // }
    // if ((type & ITEM::TYPE::OFFHAND) > 0) {
    //     temp = offhands;
    // }
    // if ((type & ITEM::TYPE::ELEMENTAL) > 0) {
    //     temp = elemental;
    // }
    // if ((type & ITEM::TYPE::DEMONIC) > 0) {
    //     temp = demonic;
    // }
    // if ((type & ITEM::TYPE::ANGELIC) > 0) {
    //     temp = angelic;
    // }
    // if ((type & ITEM::TYPE::SHIELD) > 0) {
    //     temp = shields;
    // }
    // if ((type & ITEM::TYPE::STAFF) > 0) {
    //     temp = staffs;
    // }
    // if ((type & ITEM::TYPE::CLAW) > 0) {
    //     temp = claws;
    // }
    // if ((type & ITEM::TYPE::ITEM) > 0) {
    //     temp = items;
    // }

    return(temparr);
}

// void Convoy::exch(int ind1, int ind2, Inventory_item * in_item) {
//     Inventory_item buffer[2];
//     buffer[0] = in_item[ind1];
//     buffer[1] = in_item[ind2];
//     in_item[ind1] = buffer[1];
//     in_item[ind2] = buffer[0];
// }

Inventory_item Convoy::withdraw(int in_index, int in_type) {
    Inventory_item temp;
    Inventory_item empty;

        if (in_type == ITEM::TYPE::SWORD) {
            temp = swords[in_index];
            swords[in_index] = empty;
            quantity.swords -= 1;
        }
        if (in_type == ITEM::TYPE::LANCE) {
            temp = lances[in_index];
            lances[in_index] = empty;
            quantity.lances -= 1;
        }
        if (in_type == ITEM::TYPE::AXE) {
            temp = axes[in_index];
            axes[in_index] = empty;
            quantity.axes -= 1;
        }
        if (in_type == ITEM::TYPE::BOW) {
            temp = bows[in_index];
            bows[in_index] = empty;
            quantity.bows -= 1;
        }
        if (in_type == ITEM::TYPE::TRINKET) {
            temp = trinkets[in_index];
            trinkets[in_index] = empty;
            quantity.trinkets -= 1;
        }
        if (in_type == ITEM::TYPE::OFFHAND) {
            temp = offhands[in_index];
            offhands[in_index] = empty;
            quantity.offhands -= 1;
        }
        if (in_type == ITEM::TYPE::ELEMENTAL) {
            temp = elemental[in_index];
            elemental[in_index] = empty;
            quantity.elemental -= 1;
        }
        if (in_type == ITEM::TYPE::DEMONIC) {
            temp = demonic[in_index];
            demonic[in_index] = empty;
            quantity.demonic -= 1;
        }
        if (in_type == ITEM::TYPE::ANGELIC) {
            temp = angelic[in_index];
            angelic[in_index] = empty;
            quantity.angelic -= 1;
        }
        if (in_type == ITEM::TYPE::SHIELD) {
            temp = shields[in_index];
            shields[in_index] = empty;
            quantity.shields -= 1;
        }
        if (in_type == ITEM::TYPE::STAFF) {
            temp = staffs[in_index];
            staffs[in_index] = empty;
            quantity.staffs -= 1;
        }
        if (in_type == ITEM::TYPE::CLAW) {
            temp = claws[in_index];
            claws[in_index] = empty;
            quantity.claws -= 1;
        }
        if (in_type == ITEM::TYPE::ITEM) {
            temp = items[in_index];
            items[in_index] = empty;
            quantity.items -= 1;
        }
    return(temp);
}

void Convoy::earn(int in_money) {
    bank += in_money;
} 

void Convoy::spend(int out_money) {
    bank += out_money;
} 