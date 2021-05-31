
#include "convoy.h"
// #define STB_DS_IMPLEMENTATION
#include "stb_ds.h"

struct Convoy Convoy_default = {
    .JSONElement = {JSON_CONVOY},
    .bank = 0,
    .booksnum = 0,
    .full = false,
    .weapons = NULL,
    .quantity = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
    // .books,
    // .swords,
    // .lances,
    // .axes,
    // .bows,
    // .trinkets,
    // .offhands,
    // .elemental,
    // .demonic,
    // .angelic,
    // .shields,
    // .staffs,
    // .claws,
    // .items,
};

void Convoy_swap(int16_t arr[], int16_t ind1, int16_t ind2) {
    int16_t buffer;
    buffer = arr[ind1];
    arr[ind1] = arr[ind2];
    arr[ind2] = buffer;
}

void Convoy_Wpn_Swap(struct Inventory_item arr[], int16_t ind1, int16_t ind2) {
    struct Inventory_item buffer = Inventory_item_default;
    buffer = arr[ind1];
    arr[ind1] = arr[ind2];
    arr[ind2] = buffer;
}

void Convoy_Wpn_Swaps(struct Convoy * in_convoy, int16_t utilities_wpnType, int16_t ind1, int16_t ind2) {
    struct Inventory_item buffer = Inventory_item_default;
    switch (utilities_wpnType) {
        case ITEM_TYPE_SWORD:
            Convoy_Wpn_Swap(in_convoy->swords, ind1, ind2);
            break;
        case ITEM_TYPE_LANCE:
            Convoy_Wpn_Swap(in_convoy->lances, ind1, ind2);
            break;
        case ITEM_TYPE_AXE:
            Convoy_Wpn_Swap(in_convoy->axes, ind1, ind2);
            break;
        case ITEM_TYPE_BOW:
            Convoy_Wpn_Swap(in_convoy->bows, ind1, ind2);
            break;
        case ITEM_TYPE_TRINKET:
            Convoy_Wpn_Swap(in_convoy->trinkets, ind1, ind2);
            break;
        case ITEM_TYPE_OFFHAND:
            Convoy_Wpn_Swap(in_convoy->offhands, ind1, ind2);
            break;
        case ITEM_TYPE_ELEMENTAL:
            Convoy_Wpn_Swap(in_convoy->elemental, ind1, ind2);
            break;
        case ITEM_TYPE_DEMONIC:
            Convoy_Wpn_Swap(in_convoy->demonic, ind1, ind2);
            break;
        case ITEM_TYPE_ANGELIC:
            Convoy_Wpn_Swap(in_convoy->angelic, ind1, ind2);
            break;
        case ITEM_TYPE_SHIELD:
            Convoy_Wpn_Swap(in_convoy->shields, ind1, ind2);
            break;
        case ITEM_TYPE_STAFF:
            Convoy_Wpn_Swap(in_convoy->staffs, ind1, ind2);
            break;
        case ITEM_TYPE_CLAW:
            Convoy_Wpn_Swap(in_convoy->claws, ind1, ind2);
            break;
        case ITEM_TYPE_ITEM:
            Convoy_Wpn_Swap(in_convoy->items, ind1, ind2);
            break;
        case ITEM_TYPE_BOOK:
            Convoy_Wpn_Swap(in_convoy->books, ind1, ind2);
            break;
        default:
            SDL_Log("Convoy_Wpn_Swaps: utilities_wpnType is invalid");
    }
}

void Convoy_quicksort(struct Convoy * in_convoy, int16_t arr[], int16_t low, int16_t high, int16_t utilities_wpnType) {
    int16_t pi;
    if (low < high) {
        pi = Convoy_partition(in_convoy, arr, low, high, utilities_wpnType);
        Convoy_quicksort(in_convoy, arr, low, pi - 1, utilities_wpnType);
        Convoy_quicksort(in_convoy, arr, pi + 1, high, utilities_wpnType);
    }
}

int16_t Convoy_partition(struct Convoy * in_convoy, int16_t arr[], int16_t low, int16_t high, int16_t utilities_wpnType) {
    int16_t pivot = arr[high];
    int16_t i = low - 1;
    for (int16_t j = low; j < high; j++) {
        if (arr[j] < pivot) {
            i++;
            Convoy_swap(arr, i, j);
            Convoy_Wpn_Swaps(in_convoy, utilities_wpnType, i, j);
        }
    }
    Convoy_swap(arr, i + 1, high);
    Convoy_Wpn_Swaps(in_convoy, utilities_wpnType, i + 1, high);
    return (i + 1);
}

void Convoy_deposit(struct Convoy * in_convoy, struct Inventory_item in_item) {
    if (!in_convoy->full) {
        SDL_Log("Depositing: %d", in_item.id);
        if (in_convoy->weapons != NULL) {
            Convoy_Weapon_Check(in_convoy, in_item.id);
            uint16_t wpntypecode = hmget(in_convoy->weapons, in_item.id).item->type;
            SDL_Log("wpntypecode=0? %d", wpntypecode == 0);
            if flagsum_isIn(wpntypecode, ITEM_TYPE_SWORD) {
                in_convoy->swords[in_convoy->quantity.swords] = in_item;
                in_convoy->quantity.swords += 1;
                // no GOTO cause item with multiple types get added to multiple banks.
                // no goto only for item types that have items with multiple types
            }
            if flagsum_isIn(wpntypecode, ITEM_TYPE_LANCE) {
                in_convoy->lances[in_convoy->quantity.lances] = in_item;
                in_convoy->quantity.lances += 1;
            }
            if flagsum_isIn(wpntypecode, ITEM_TYPE_AXE) {
                in_convoy->axes[in_convoy->quantity.axes] = in_item;
                in_convoy->quantity.axes += 1;
            }
            if flagsum_isIn(wpntypecode, ITEM_TYPE_BOW) {
                in_convoy->bows[in_convoy->quantity.bows] = in_item;
                in_convoy->quantity.bows += 1;
                goto DEPOSIT_END;
            }
            if flagsum_isIn(wpntypecode, ITEM_TYPE_TRINKET) {
                in_convoy->trinkets[in_convoy->quantity.trinkets] = in_item;
                in_convoy->quantity.trinkets += 1;
                goto DEPOSIT_END;
            }
            if flagsum_isIn(wpntypecode, ITEM_TYPE_OFFHAND) {
                in_convoy->offhands[in_convoy->quantity.offhands] = in_item;
                in_convoy->quantity.offhands += 1;
                goto DEPOSIT_END;
            }
            if flagsum_isIn(wpntypecode, ITEM_TYPE_ELEMENTAL) {
                in_convoy->elemental[in_convoy->quantity.elemental] = in_item;
                in_convoy->quantity.elemental += 1;
                goto DEPOSIT_END;
            }
            if flagsum_isIn(wpntypecode, ITEM_TYPE_DEMONIC) {
                in_convoy->demonic[in_convoy->quantity.demonic] = in_item;
                in_convoy->quantity.demonic += 1;
                goto DEPOSIT_END;
            }
            if flagsum_isIn(wpntypecode, ITEM_TYPE_ANGELIC) {
                in_convoy->angelic[in_convoy->quantity.angelic] = in_item;
                in_convoy->quantity.angelic += 1;
                goto DEPOSIT_END;
            }
            if flagsum_isIn(wpntypecode, ITEM_TYPE_SHIELD) {
                in_convoy->shields[in_convoy->quantity.shields] = in_item;
                in_convoy->quantity.shields += 1;
                goto DEPOSIT_END;
            }
            if flagsum_isIn(wpntypecode, ITEM_TYPE_STAFF) {
                in_convoy->staffs[in_convoy->quantity.staffs] = in_item;
                in_convoy->quantity.staffs += 1;
                goto DEPOSIT_END;
            }
            if flagsum_isIn(wpntypecode, ITEM_TYPE_CLAW) {
                in_convoy->claws[in_convoy->quantity.claws] = in_item;
                in_convoy->quantity.claws += 1;
                goto DEPOSIT_END;
            }
            if flagsum_isIn(wpntypecode, ITEM_TYPE_ITEM) {
                in_convoy->items[in_convoy->quantity.items] = in_item;
                in_convoy->quantity.items += 1;
                goto DEPOSIT_END;
            }
            if flagsum_isIn(wpntypecode, ITEM_TYPE_BOOK) {
                in_convoy->books[in_convoy->booksnum] = in_item;
                in_convoy->booksnum += 1;
                goto DEPOSIT_END;
            }
DEPOSIT_END:
            Convoy_isFull(in_convoy);
        } else {
            SDL_Log("weapons pointer is null");
        }
    } else {
        SDL_Log("in_convoy is full");
    }
}

bool Convoy_isFull(struct Convoy * in_convoy) {
    int16_t sum = in_convoy->quantity.swords + in_convoy->quantity.lances + in_convoy->quantity.axes +
                  in_convoy->quantity.bows + in_convoy->quantity.trinkets + in_convoy->quantity.offhands + in_convoy->quantity.elemental +
                  in_convoy->quantity.demonic + in_convoy->quantity.angelic + in_convoy->quantity.shields +
                  in_convoy->quantity.staffs + in_convoy->quantity.claws + in_convoy->quantity.items;
    SDL_Log("sum: %d", sum);
    SDL_Log("swords: %d", in_convoy->quantity.swords);
    in_convoy->full = (sum >= DEFAULT_CONVOY_SIZE);
    return (in_convoy->full);
}

void Convoy_Contents_Print(struct Convoy * in_convoy, int16_t utilities_wpnType) {
    struct Inventory_item * tempitems = Convoy_Items_Get(in_convoy, utilities_wpnType);
    int16_t tempqty = Convoy_getQuantity(in_convoy, utilities_wpnType);
    // SDL_Log("Quantity: %d \nArray:\n", tempqty);
    if (in_convoy->weapons != NULL) {
        for (int16_t i = 0; i < tempqty; i++) {
            if (tempitems[i].id > 0) {
                SDL_Log("%d: %s", i, hmget(in_convoy->weapons, tempitems[i].id).item->name);
            }
        }
    } else {
        // SDL_Log("weapons pointer is NULL");
    }
}

void Convoy_Stats_Print(struct Convoy * in_convoy, int16_t utilities_wpnType, int16_t stattype) {
    struct Inventory_item * tempitems = Convoy_Items_Get(in_convoy, utilities_wpnType);
    int16_t tempqty = Convoy_getQuantity(in_convoy, utilities_wpnType);
    char * statname[DEFAULT_BUFFER_SIZE] = {statNames[stattype]};
    int16_t * arrstats = Convoy_Stats_Get(in_convoy, utilities_wpnType, stattype);
    SDL_Log("Quantity: %d \n%s \t Wpn \n", tempqty, statname);
    if (in_convoy->weapons != NULL) {
        for (int16_t i = 0; i < tempqty; i++) {
            if (tempitems[i].id > 0) {
                SDL_Log("%d \t %s", arrstats[i], hmget(in_convoy->weapons, tempitems[i].id).item->name);
            }
        }
    } else {
        // SDL_Log("weapons pointer is NULL");
    }
}

void Convoy_Used_Sort(struct Convoy * in_convoy, int16_t utilities_wpnType) {
    // SDL_Log("In Convoy_Used_Sort");
    int16_t * arrusesleft = NULL;
    arrusesleft = Convoy_Stats_Get(in_convoy, utilities_wpnType, ITEM_STAT_USES_LEFT);
    int16_t high = Convoy_getQuantity(in_convoy, utilities_wpnType);
    Convoy_quicksort(in_convoy, arrusesleft, 0, high - 1, utilities_wpnType);
}

void Convoy_Stats_Sort(struct Convoy * in_convoy, int16_t utilities_wpnType, int16_t stattype) {
    int16_t * arrid = Convoy_Stats_Get(in_convoy, utilities_wpnType, stattype);
    int16_t high = Convoy_getQuantity(in_convoy, utilities_wpnType);
    Convoy_quicksort(in_convoy, arrid, 0, high - 1, utilities_wpnType);
}

struct Inventory_item * Convoy_Items_Get(struct Convoy * in_convoy, int16_t utilities_wpnType) {
    static struct Inventory_item temp[DEFAULT_CONVOY_SIZE];
    switch (utilities_wpnType) {
        case ITEM_TYPE_SWORD:
            memcpy(temp, in_convoy->swords, sizeof(temp));
            break;
        case ITEM_TYPE_LANCE:
            memcpy(temp, in_convoy->lances, sizeof(temp));
            break;
        case ITEM_TYPE_AXE:
            memcpy(temp, in_convoy->axes, sizeof(temp));
            break;
        case ITEM_TYPE_BOW:
            memcpy(temp, in_convoy->bows, sizeof(temp));
            break;
        case ITEM_TYPE_TRINKET:
            memcpy(temp, in_convoy->trinkets, sizeof(temp));
            break;
        case ITEM_TYPE_OFFHAND:
            memcpy(temp, in_convoy->offhands, sizeof(temp));
            break;
        case ITEM_TYPE_ELEMENTAL:
            memcpy(temp, in_convoy->elemental, sizeof(temp));
            break;
        case ITEM_TYPE_DEMONIC:
            memcpy(temp, in_convoy->demonic, sizeof(temp));
            break;
        case ITEM_TYPE_ANGELIC:
            memcpy(temp, in_convoy->angelic, sizeof(temp));
            break;
        case ITEM_TYPE_SHIELD:
            memcpy(temp, in_convoy->shields, sizeof(temp));
            break;
        case ITEM_TYPE_STAFF:
            memcpy(temp, in_convoy->staffs, sizeof(temp));
            break;
        case ITEM_TYPE_CLAW:
            memcpy(temp, in_convoy->claws, sizeof(temp));
            break;
        case ITEM_TYPE_ITEM:
            memcpy(temp, in_convoy->items, sizeof(temp));
            break;
        case ITEM_TYPE_BOOK:
            memcpy(temp, in_convoy->books, sizeof(temp));
            break;
        default:
            SDL_Log("Convoy_Items_Get: utilities_wpnType is invalid");
    }
    return (temp);
}

void Convoy_Weapons_Set(struct Convoy * in_convoy, struct Weapons_hash * in_weapons) {
    in_convoy->weapons =  in_weapons;
}

void Convoy_clear(struct Convoy * in_convoy) {
    hmfree(in_convoy->weapons);
    struct Inventory_item empty = Inventory_item_default;
    struct Weapon empty_weapon = Weapon_default;
    hmdefault(in_convoy->weapons, empty_weapon);
    for (int16_t i = 0; i < in_convoy->quantity.swords; i++) {
        in_convoy->swords[i] = empty;
    }
    for (int16_t i = 0; i < in_convoy->quantity.lances; i++) {
        in_convoy->lances[i] = empty;
    }
    for (int16_t i = 0; i < in_convoy->quantity.axes; i++) {
        in_convoy->axes[i] = empty;
    }
    for (int16_t i = 0; i < in_convoy->quantity.bows; i++) {
        in_convoy->bows[i] = empty;
    }
    for (int16_t i = 0; i < in_convoy->quantity.trinkets; i++) {
        in_convoy->trinkets[i] = empty;
    }
    for (int16_t i = 0; i < in_convoy->quantity.offhands; i++) {
        in_convoy->offhands[i] = empty;
    }
    for (int16_t i = 0; i < in_convoy->quantity.elemental; i++) {
        in_convoy->elemental[i] = empty;
    }
    for (int16_t i = 0; i < in_convoy->quantity.demonic; i++) {
        in_convoy->demonic[i] = empty;
    }
    for (int16_t i = 0; i < in_convoy->quantity.angelic; i++) {
        in_convoy->angelic[i] = empty;
    }
    for (int16_t i = 0; i < in_convoy->quantity.shields; i++) {
        in_convoy->shields[i] = empty;
    }
    for (int16_t i = 0; i < in_convoy->quantity.staffs; i++) {
        in_convoy->staffs[i] = empty;
    }
    for (int16_t i = 0; i < in_convoy->quantity.claws; i++) {
        in_convoy->claws[i] = empty;
    }
    for (int16_t i = 0; i < in_convoy->quantity.items; i++) {
        in_convoy->items[i] = empty;
    }
    for (int16_t i = 0; i < in_convoy->booksnum; i++) {
        in_convoy->books[i] = empty;
    }
    in_convoy->quantity.swords = 0;
    in_convoy->quantity.lances = 0;
    in_convoy->quantity.axes = 0;
    in_convoy->quantity.bows = 0;
    in_convoy->quantity.trinkets = 0;
    in_convoy->quantity.offhands = 0;
    in_convoy->quantity.elemental = 0;
    in_convoy->quantity.demonic = 0;
    in_convoy->quantity.angelic = 0;
    in_convoy->quantity.shields = 0;
    in_convoy->quantity.staffs = 0;
    in_convoy->quantity.claws = 0;
    in_convoy->quantity.items = 0;
    in_convoy->booksnum = 0;
}

void Convoy_Weapon_Check(struct Convoy * in_convoy, int16_t in_id) {
    SDL_Log("Convoy_Weapon_Check");
    if (in_convoy->weapons != NULL) {
        char * token;
        char buffer[DEFAULT_BUFFER_SIZE];
        if (hmgetp_null(in_convoy->weapons, in_id) == NULL) {
            char filename[DEFAULT_BUFFER_SIZE] = "items"PHYSFS_SEPARATOR;
            strncpy(buffer, hmget(itemNames, in_id), DEFAULT_BUFFER_SIZE);
            token = strtok(buffer, " \t");
            while (token != NULL) {
                strcat(filename, token);
                token = strtok(NULL, " \t");
            }
            strcat(filename, ".json");
            SDL_Log("Loading weapon %d %s", in_id, filename);
            // SDL_Log("Loading weapon: %s", filename);
            struct Weapon temp_weapon = Weapon_default;
            readJSON(filename, &temp_weapon);
            hmput(in_convoy->weapons, in_id, temp_weapon);
            SDL_Log("temp_weapon.type==0? %d", (0 == temp_weapon.item->type));
        }
    } else {
        SDL_Log("Convoy_Weapon_Check: weapons pointer is NULL");
    }
}

void Convoy_Items_Set(struct Convoy * in_convoy, int16_t utilities_wpnType, struct Inventory_item * in_items) {
    switch (utilities_wpnType) {
        case ITEM_TYPE_SWORD:
            memcpy(in_convoy->swords, in_items, sizeof(in_convoy->swords));
            break;
        case ITEM_TYPE_LANCE:
            memcpy(in_convoy->lances, in_items, sizeof(in_convoy->lances));
            break;
        case ITEM_TYPE_AXE:
            memcpy(in_convoy->axes, in_items, sizeof(in_convoy->axes));
            break;
        case ITEM_TYPE_BOW:
            memcpy(in_convoy->bows, in_items, sizeof(in_convoy->bows));
            break;
        case ITEM_TYPE_TRINKET:
            memcpy(in_convoy->trinkets, in_items, sizeof(in_convoy->trinkets));
            break;
        case ITEM_TYPE_OFFHAND:
            memcpy(in_convoy->offhands, in_items, sizeof(in_convoy->offhands));
            break;
        case ITEM_TYPE_ELEMENTAL:
            memcpy(in_convoy->elemental, in_items, sizeof(in_convoy->elemental));
            break;
        case ITEM_TYPE_DEMONIC:
            memcpy(in_convoy->demonic, in_items, sizeof(in_convoy->demonic));
            break;
        case ITEM_TYPE_ANGELIC:
            memcpy(in_convoy->angelic, in_items, sizeof(in_convoy->angelic));
            break;
        case ITEM_TYPE_SHIELD:
            memcpy(in_convoy->shields, in_items, sizeof(in_convoy->shields));
            break;
        case ITEM_TYPE_STAFF:
            memcpy(in_convoy->staffs, in_items, sizeof(in_convoy->staffs));
            break;
        case ITEM_TYPE_CLAW:
            memcpy(in_convoy->claws, in_items, sizeof(in_convoy->claws));
            break;
        case ITEM_TYPE_ITEM:
            memcpy(in_convoy->items, in_items, sizeof(in_convoy->items));
            break;
        case ITEM_TYPE_BOOK:
            memcpy(in_convoy->books, in_items, sizeof(in_convoy->books));
            break;
        default:
            SDL_Log("Convoy_Items_Set: utilities_wpnType is invalid");
    }
}

struct Quantity Convoy_Quantities_Get(struct Convoy * in_convoy) {
    return (in_convoy->quantity);
}

int16_t Convoy_getQuantity(struct Convoy * in_convoy, int16_t utilities_wpnType) {
    SDL_Log("Convoy_getQuantity. Type:%d", utilities_wpnType);

    static int16_t temp;
    switch (utilities_wpnType) {
        case ITEM_TYPE_SWORD:
            temp = in_convoy->quantity.swords;
            break;
        case ITEM_TYPE_LANCE:
            temp = in_convoy->quantity.lances;
            break;
        case ITEM_TYPE_AXE:
            temp = in_convoy->quantity.axes;
            break;
        case ITEM_TYPE_BOW:
            temp = in_convoy->quantity.bows;
            break;
        case ITEM_TYPE_TRINKET:
            temp = in_convoy->quantity.trinkets;
            break;
        case ITEM_TYPE_OFFHAND:
            temp = in_convoy->quantity.offhands;
            break;
        case ITEM_TYPE_ELEMENTAL:
            temp = in_convoy->quantity.elemental;
            break;
        case ITEM_TYPE_DEMONIC:
            temp = in_convoy->quantity.demonic;
            break;
        case ITEM_TYPE_ANGELIC:
            temp = in_convoy->quantity.angelic;
            break;
        case ITEM_TYPE_SHIELD:
            temp = in_convoy->quantity.shields;
            break;
        case ITEM_TYPE_STAFF:
            temp = in_convoy->quantity.staffs;
            break;
        case ITEM_TYPE_CLAW:
            temp = in_convoy->quantity.claws;
            break;
        case ITEM_TYPE_ITEM:
            temp = in_convoy->quantity.items;
            break;
        case ITEM_TYPE_BOOK:
            temp = in_convoy->booksnum;
            break;
        default:
            SDL_Log("Convoy_getQuantity: utilities_wpnType is invalid");
    }
    return (temp);
}

int16_t * Convoy_Stats_Get(struct Convoy * in_convoy, int16_t utilities_wpnType, int16_t stattype) {
    int16_t tempqty = Convoy_getQuantity(in_convoy, utilities_wpnType);
    int16_t * arrstats = NULL;
    struct Inventory_item * temp = Convoy_Items_Get(in_convoy, utilities_wpnType);
    if (in_convoy->weapons != NULL) {
        for (int16_t i = 0; i < tempqty; i++) {
            switch (stattype) {
                case ITEM_STAT_ID:
                    arrput(arrstats, temp[i].id);
                    break;
                case ITEM_STAT_PMIGHT:
                    arrput(arrstats, hmget(in_convoy->weapons, temp[i].id).stats.Pmight);
                    break;
                case ITEM_STAT_MMIGHT:
                    arrput(arrstats, hmget(in_convoy->weapons, temp[i].id).stats.Mmight);
                    break;
                case ITEM_STAT_HIT:
                    arrput(arrstats, hmget(in_convoy->weapons, temp[i].id).stats.combat.hit);
                    break;
                case ITEM_STAT_DODGE:
                    arrput(arrstats, hmget(in_convoy->weapons, temp[i].id).stats.combat.dodge);
                    break;
                case ITEM_STAT_CRIT:
                    arrput(arrstats, hmget(in_convoy->weapons, temp[i].id).stats.combat.crit);
                    break;
                case ITEM_STAT_FAVOR:
                    arrput(arrstats, hmget(in_convoy->weapons, temp[i].id).stats.combat.favor);
                    break;
                case ITEM_STAT_WGT:
                    arrput(arrstats, hmget(in_convoy->weapons, temp[i].id).stats.wgt);
                    break;
                case ITEM_STAT_USES:
                    arrput(arrstats, hmget(in_convoy->weapons, temp[i].id).stats.uses);
                    break;
                case ITEM_STAT_USES_LEFT:
                    arrput(arrstats, hmget(in_convoy->weapons, temp[i].id).stats.uses - temp[i].used);
                    break;
                case ITEM_STAT_USED:
                    arrput(arrstats, temp[i].used);
                    break;
                case ITEM_STAT_PROF:
                    arrput(arrstats, hmget(in_convoy->weapons, temp[i].id).stats.prof);
                    break;
                case ITEM_STAT_RANGEMIN:
                    arrput(arrstats, hmget(in_convoy->weapons, temp[i].id).stats.range[0]);
                    break;
                case ITEM_STAT_RANGEMAX:
                    arrput(arrstats, hmget(in_convoy->weapons, temp[i].id).stats.range[1]);
                    break;
                case ITEM_STAT_HANDLEFT:
                    arrput(arrstats, hmget(in_convoy->weapons, temp[i].id).stats.hand[0]);
                    break;
                case ITEM_STAT_HANDRIGHT:
                    arrput(arrstats, hmget(in_convoy->weapons, temp[i].id).stats.hand[1]);
                    break;
                case ITEM_STAT_PRICE:
                    arrput(arrstats, hmget(in_convoy->weapons, temp[i].id).item->price);
                    break;
                case ITEM_STAT_HEAL:
                    arrput(arrstats, hmget(in_convoy->weapons, temp[i].id).item->use_power);
                    break;
                default:
                    SDL_Log("Convoy_Stats_Get: stat type is invalid");
            }
        }
    } else {
        SDL_Log("Convoy_Stats_Get: weapons pointer is NULL");
    }
    return (arrstats);
}

struct Inventory_item Convoy_withdraw(struct Convoy * in_convoy, int16_t in_index, int16_t utilities_wpnType) {
    struct Inventory_item temp;
    struct Inventory_item empty = Inventory_item_default;
    // SDL_Log("Convoy_withdraw. Type:%d Index: %d", utilities_wpnType, in_index);
    switch (utilities_wpnType) {
        case ITEM_TYPE_SWORD:
            // SDL_Log("Withdrawing a sword");
            temp = in_convoy->swords[in_index];
            Convoy_Wpn_Swaps(in_convoy, utilities_wpnType, in_index, in_convoy->quantity.swords);
            in_convoy->swords[in_convoy->quantity.swords] = empty;
            in_convoy->quantity.swords -= 1;
            break;
        case ITEM_TYPE_LANCE:
            temp = in_convoy->lances[in_index];
            in_convoy->lances[in_index] = empty;
            in_convoy->quantity.lances -= 1;
            break;
        case ITEM_TYPE_AXE:
            temp = in_convoy->axes[in_index];
            in_convoy->axes[in_index] = empty;
            in_convoy->quantity.axes -= 1;
            break;
        case ITEM_TYPE_BOW:
            temp = in_convoy->bows[in_index];
            in_convoy->bows[in_index] = empty;
            in_convoy->quantity.bows -= 1;
            break;
        case ITEM_TYPE_TRINKET:
            temp = in_convoy->trinkets[in_index];
            in_convoy->trinkets[in_index] = empty;
            in_convoy->quantity.trinkets -= 1;
            break;
        case ITEM_TYPE_OFFHAND:
            temp = in_convoy->offhands[in_index];
            in_convoy->offhands[in_index] = empty;
            in_convoy->quantity.offhands -= 1;
            break;
        case ITEM_TYPE_ELEMENTAL:
            temp = in_convoy->elemental[in_index];
            in_convoy->elemental[in_index] = empty;
            in_convoy->quantity.elemental -= 1;
            break;
        case ITEM_TYPE_DEMONIC:
            temp = in_convoy->demonic[in_index];
            in_convoy->demonic[in_index] = empty;
            in_convoy->quantity.demonic -= 1;
            break;
        case ITEM_TYPE_ANGELIC:
            temp = in_convoy->angelic[in_index];
            in_convoy->angelic[in_index] = empty;
            in_convoy->quantity.angelic -= 1;
            break;
        case ITEM_TYPE_SHIELD:
            temp = in_convoy->shields[in_index];
            in_convoy->shields[in_index] = empty;
            in_convoy->quantity.shields -= 1;
            break;
        case ITEM_TYPE_STAFF:
            temp = in_convoy->staffs[in_index];
            in_convoy->staffs[in_index] = empty;
            in_convoy->quantity.staffs -= 1;
            break;
        case ITEM_TYPE_CLAW:
            temp = in_convoy->claws[in_index];
            in_convoy->claws[in_index] = empty;
            in_convoy->quantity.claws -= 1;
            break;
        case ITEM_TYPE_ITEM:
            temp = in_convoy->items[in_index];
            in_convoy->items[in_index] = empty;
            in_convoy->quantity.items -= 1;
            break;
        case ITEM_TYPE_BOOK:
            temp = in_convoy->books[in_index];
            in_convoy->books[in_index] = empty;
            in_convoy->booksnum -= 1;
            break;
        default:
            SDL_Log("Convoy_withdraw: utilities_wpnType is invalid");
    }
    Convoy_isFull(in_convoy);
    return (temp);
}

void Convoy_earn(struct Convoy * in_convoy, int16_t in_money) {
    in_convoy->bank += in_money;
}

void Convoy_spend(struct Convoy * in_convoy, int16_t out_money) {
    in_convoy->bank += out_money;
}

void Convoy_readJSON(struct Convoy * in_convoy, cJSON * in_jconvoy) {
    if (in_jconvoy != NULL) {
        char ** names;
        struct Inventory_item tempitem = Inventory_item_default;
        struct Inventory_item empty = Inventory_item_default;
        cJSON * jitem = NULL;
        cJSON * jtype = NULL;
        int16_t i = 0x0001;
        while (i < ITEM_TYPE_END) {
            names = utilities_wpnType(i);
            jtype = cJSON_GetObjectItem(in_jconvoy, names[0]);
            if (jtype ==  NULL) {
                SDL_Log("Convoy_readJSON: Cannot get %s element", names[0]);
            } else {
                jitem = cJSON_GetObjectItem(jtype, "Item");
                while (jitem != NULL) {
                    readJSON_item(jitem, &tempitem);
                    if (tempitem.id > 0) {
                        Convoy_deposit(in_convoy, tempitem);
                    }
                    tempitem = empty;
                    jitem = jitem->next;
                }
            }
            i *= 2;
        }
    } else {
        SDL_Log("Convoy_readJSON: in_jconvoy is NULL");
    }
}

void Convoy_writeJSON(struct Convoy * in_convoy, cJSON * in_jconvoy) {
    if (in_jconvoy != NULL) {
        cJSON * jitems = NULL;
        struct Inventory_item * tempitem = NULL;
        char ** names;
        int16_t quantity;
        int16_t i = 1;
        while (i < ITEM_TYPE_END) {
            jitems = cJSON_CreateObject();
            names = utilities_wpnType(i);
            quantity = Convoy_getQuantity(in_convoy, i);
            tempitem = Convoy_Items_Get(in_convoy, i);
            for (uint8_t item_num = 0; item_num < quantity; item_num ++) {
                writeJSON_item(jitems, &tempitem[item_num]);
            }
            cJSON_AddItemToObject(in_jconvoy, names[0], jitems);
            i *= 2;
        }
    } else {
        SDL_Log("Convoy_writeJSON: in_jconvoy is NULL");
    }
}