
#include "convoy.hpp"

Convoy::Convoy() {

}

void Convoy::swap(int arr[], int ind1, int ind2) {
    int buffer;
    buffer = arr[ind1];
    arr[ind1] = arr[ind2];
    arr[ind2] = buffer;
}

void Convoy::swapwpn(int wpn_type, int ind1, int ind2) {
    Inventory_item buffer;
    buffer = arr[ind1];
    arr[ind1] = arr[ind2];
    arr[ind2] = buffer;

    if ((type & ITEM::TYPE::SWORD) > 0) {
        swords[quantity.swords] = in_item;
    }
    if ((type & ITEM::TYPE::LANCE) > 0) {
        lances[quantity.lances] = in_item;
    }
    if ((type & ITEM::TYPE::AXE) > 0) {
        axes[quantity.axes] = in_item;
    }
    if ((type & ITEM::TYPE::BOW) > 0) {
        bows[quantity.bows] = in_item;
    }
    if ((type & ITEM::TYPE::TRINKET) > 0) {
        trinkets[quantity.trinkets] = in_item;
    }
    if ((type & ITEM::TYPE::OFFHAND) > 0) {
        offhands[quantity.offhands] = in_item;
    }
    if ((type & ITEM::TYPE::ELEMENTAL) > 0) {
        elemental[quantity.elemental] = in_item;
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

}


void Convoy::quicksort(int arr[], int low, int high, int wpntype) {
    int pi;
    if (low < high) {
        pi = partition(arr, low, high, wpntype);
        quicksort(arr, low, pi - 1, wpntype);
        quicksort(arr, pi + 1, high, wpntype);
    }
}

int Convoy::partition(int arr[], int low, int high, int wpntype) {
    int pivot = arr[high];
    int i = low - 1;
    for (int j = low; j < high; j++) {
        if (arr[j] < pivot) {
            i++;
            swap(arr, i, j);
            swapwpn(wpntype, i, j);
        }
    }
    swap(arr, i + 1, high);
    swapwpn(wpntype, i + 1, high);
    return(i + 1);
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
      

int * Convoy::getarr(int wpntype, int stattype) {
    static int temparr[DEFAULT::CONVOY_SIZE];
    Inventory_item temp[DEFAULT::CONVOY_SIZE];

    switch (wpntype) {
        case ITEM::TYPE::SWORD:
            memcpy(temp, swords, DEFAULT::CONVOY_SIZE);
            break;
        case ITEM::TYPE::LANCE:
            memcpy(temp, lances, DEFAULT::CONVOY_SIZE);
            break;
        case ITEM::TYPE::AXE:
            memcpy(temp, axes, DEFAULT::CONVOY_SIZE);
            break;
        case ITEM::TYPE::BOW:
            memcpy(temp, bows, DEFAULT::CONVOY_SIZE);
            break;
        case ITEM::TYPE::TRINKET:
            memcpy(temp, trinkets, DEFAULT::CONVOY_SIZE);
            break;
        case ITEM::TYPE::OFFHAND:
            memcpy(temp, offhands, DEFAULT::CONVOY_SIZE);
            break;
        case ITEM::TYPE::ELEMENTAL:
            memcpy(temp, elemental, DEFAULT::CONVOY_SIZE);
            break;
        case ITEM::TYPE::DEMONIC:
            memcpy(temp, demonic, DEFAULT::CONVOY_SIZE);
            break;
        case ITEM::TYPE::ANGELIC:
            memcpy(temp, angelic, DEFAULT::CONVOY_SIZE);
            break;
        case ITEM::TYPE::SHIELD:
            memcpy(temp, shields, DEFAULT::CONVOY_SIZE);
            break;
        case ITEM::TYPE::STAFF:
            memcpy(temp, staffs, DEFAULT::CONVOY_SIZE);
            break;
        case ITEM::TYPE::CLAW:
            memcpy(temp, claws, DEFAULT::CONVOY_SIZE);
            break;
        case ITEM::TYPE::ITEM:
            memcpy(temp, items, DEFAULT::CONVOY_SIZE);
            break;
    }

    for (int i = 0; i < DEFAULT::CONVOY_SIZE; i++) {
        switch(stattype) {
            case ITEM::STAT::PMIGHT:
                temparr[i] = all_weapons[temp[i].id].getStats().Pmight;
                break;
            case ITEM::STAT::MMIGHT:
                temparr[i] = all_weapons[temp[i].id].getStats().Mmight;
                break;
            case ITEM::STAT::HIT:
                temparr[i] = all_weapons[temp[i].id].getStats().combat.hit;
                break;
            case ITEM::STAT::DODGE:
                temparr[i] = all_weapons[temp[i].id].getStats().combat.dodge;
                break;
            case ITEM::STAT::CRIT:
                temparr[i] = all_weapons[temp[i].id].getStats().combat.crit;
                break;
            case ITEM::STAT::FAVOR:
                temparr[i] = all_weapons[temp[i].id].getStats().combat.favor;
                break;
            case ITEM::STAT::WGT:
                temparr[i] = all_weapons[temp[i].id].getStats().wgt;
                break;
            case ITEM::STAT::USES:
                temparr[i] = all_weapons[temp[i].id].getStats().uses;
                break;
            case ITEM::STAT::PROF:
                temparr[i] = all_weapons[temp[i].id].getStats().prof;
                break;
            case ITEM::STAT::RANGEMIN:
                temparr[i] = all_weapons[temp[i].id].getStats().range[0];
                break;
            case ITEM::STAT::RANGEMAX:
                temparr[i] = all_weapons[temp[i].id].getStats().range[1];
                break;
            case ITEM::STAT::HANDLEFT:
                temparr[i] = all_weapons[temp[i].id].getStats().hand[0];
                break;
            case ITEM::STAT::HANDRIGHT:
                temparr[i] = all_weapons[temp[i].id].getStats().hand[1];
                break;
            case ITEM::STAT::PRICE:
                temparr[i] = all_weapons[temp[i].id].getStats().price;
                break;
            case ITEM::STAT::HEAL:
                temparr[i] = all_weapons[temp[i].id].getStats().heal;
                break;
        }
    }
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
    switch (wpntype) {
        case ITEM::TYPE::SWORD:
            temp = swords[in_index];
            swords[in_index] = empty;
            quantity.swords -= 1;
            break;
        case ITEM::TYPE::LANCE:
            temp = lances[in_index];
            lances[in_index] = empty;
            quantity.lances -= 1;
            break;
        case ITEM::TYPE::AXE:
            temp = axes[in_index];
            axes[in_index] = empty;
            quantity.axes -= 1;
            break;
        case ITEM::TYPE::BOW:
            temp = bows[in_index];
            bows[in_index] = empty;
            quantity.bows -= 1;
            break;
        case ITEM::TYPE::TRINKET:
            temp = trinkets[in_index];
            trinkets[in_index] = empty;
            quantity.trinkets -= 1;
            break;
        case ITEM::TYPE::OFFHAND:
            temp = offhands[in_index];
            offhands[in_index] = empty;
            quantity.offhands -= 1;
            break;
        case ITEM::TYPE::ELEMENTAL:
            temp = elemental[in_index];
            elemental[in_index] = empty;
            quantity.elemental -= 1;
            break;
        case ITEM::TYPE::DEMONIC:
            temp = demonic[in_index];
            demonic[in_index] = empty;
            quantity.demonic -= 1;
            break;
        case ITEM::TYPE::ANGELIC:
            temp = angelic[in_index];
            angelic[in_index] = empty;
            quantity.angelic -= 1;
            break;
        case ITEM::TYPE::SHIELD:
            temp = shields[in_index];
            shields[in_index] = empty;
            quantity.shields -= 1;
            break;
        case ITEM::TYPE::STAFF:
            temp = staffs[in_index];
            staffs[in_index] = empty;
            quantity.staffs -= 1;
            break;
        case ITEM::TYPE::CLAW:
            temp = claws[in_index];
            claws[in_index] = empty;
            quantity.claws -= 1;
            break;
        case ITEM::TYPE::ITEM:
            temp = items[in_index];
            items[in_index] = empty;
            quantity.items -= 1;
            break;
    }
    return(temp);
}

void Convoy::earn(int in_money) {
    bank += in_money;
} 

void Convoy::spend(int out_money) {
    bank += out_money;
} 