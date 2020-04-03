
#include "convoy.hpp"
// #ifndef STB_SPRINTF_IMPLEMENTATION //Why no need?
// #define STB_SPRINTF_IMPLEMENTATION
#include "stb_sprintf.h" 
//#endif /* STB_SPRINTF_IMPLEMENTATION */

Convoy::Convoy() {

}

void Convoy::swap(int arr[], int ind1, int ind2) {
    int buffer;
    buffer = arr[ind1];
    arr[ind1] = arr[ind2];
    arr[ind2] = buffer;
}

void Convoy::swapWpn(int wpntype, int ind1, int ind2) {
    Inventory_item buffer;
    switch (wpntype) {
        case ITEM::TYPE::SWORD:
            buffer = swords[ind1];
            swords[ind1] = swords[ind2];
            swords[ind2] = buffer;
            break;
        case ITEM::TYPE::LANCE:
            buffer = lances[ind1];
            lances[ind1] = lances[ind2];
            lances[ind2] = buffer;
            break;
        case ITEM::TYPE::AXE:
            buffer = axes[ind1];
            axes[ind1] = axes[ind2];
            axes[ind2] = buffer;
            break;
        case ITEM::TYPE::BOW:
            buffer = bows[ind1];
            bows[ind1] = bows[ind2];
            bows[ind2] = buffer;
            break;
        case ITEM::TYPE::TRINKET:
            buffer = trinkets[ind1];
            trinkets[ind1] = trinkets[ind2];
            trinkets[ind2] = buffer;
            break;
        case ITEM::TYPE::OFFHAND:
            buffer = offhands[ind1];
            offhands[ind1] = offhands[ind2];
            offhands[ind2] = buffer;
            break;
        case ITEM::TYPE::ELEMENTAL:
            buffer = elemental[ind1];
            elemental[ind1] = elemental[ind2];
            elemental[ind2] = buffer;
            break;
        case ITEM::TYPE::DEMONIC:
            buffer = demonic[ind1];
            demonic[ind1] = demonic[ind2];
            demonic[ind2] = buffer;
            break;
        case ITEM::TYPE::ANGELIC:
            buffer = angelic[ind1];
            angelic[ind1] = angelic[ind2];
            angelic[ind2] = buffer;
            break;
        case ITEM::TYPE::SHIELD:
            buffer = shields[ind1];
            shields[ind1] = shields[ind2];
            shields[ind2] = buffer;
            break;
        case ITEM::TYPE::STAFF:
            buffer = staffs[ind1];
            staffs[ind1] = staffs[ind2];
            staffs[ind2] = buffer;
            break;
        case ITEM::TYPE::CLAW:
            buffer = claws[ind1];
            claws[ind1] = claws[ind2];
            claws[ind2] = buffer;
            break;
        case ITEM::TYPE::ITEM:
            buffer = items[ind1];
            items[ind1] = items[ind2];
            items[ind2] = buffer;
            break;
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
            swapWpn(wpntype, i, j);
        }
    }
    swap(arr, i + 1, high);
    swapWpn(wpntype, i + 1, high);
    return(i + 1);
}


void Convoy::deposit(Inventory_item in_item) {
    if (!full) {
        // SDL_Log("Depositing: %d", in_item.id);
        short unsigned int wpntypecode = all_weapons[in_item.id].getType();
        if ((wpntypecode & ITEM::TYPE::SWORD) > 0) {
            swords[quantity.swords] = in_item;
            quantity.swords += 1;
            goto DEPOSIT_END;
        }
        if ((wpntypecode & ITEM::TYPE::LANCE) > 0) {
            lances[quantity.lances] = in_item;
            quantity.lances += 1;
            goto DEPOSIT_END;
        }
        if ((wpntypecode & ITEM::TYPE::AXE) > 0) {
            axes[quantity.axes] = in_item;
            quantity.axes += 1;
            goto DEPOSIT_END;
        }
        if ((wpntypecode & ITEM::TYPE::BOW) > 0) {
            bows[quantity.bows] = in_item;
            quantity.bows += 1;
            goto DEPOSIT_END;
        }
        if ((wpntypecode & ITEM::TYPE::TRINKET) > 0) {
            trinkets[quantity.trinkets] = in_item;
            quantity.trinkets += 1;
            goto DEPOSIT_END;
        }
        if ((wpntypecode & ITEM::TYPE::OFFHAND) > 0) {
            offhands[quantity.offhands] = in_item;
            quantity.offhands += 1;
            goto DEPOSIT_END;
        }
        if ((wpntypecode & ITEM::TYPE::ELEMENTAL) > 0) {
            elemental[quantity.elemental] = in_item;
            quantity.elemental += 1;
            goto DEPOSIT_END;
        }
        if ((wpntypecode & ITEM::TYPE::DEMONIC) > 0) {
            demonic[quantity.demonic] = in_item;
            quantity.demonic += 1;
            goto DEPOSIT_END;
        }
        if ((wpntypecode & ITEM::TYPE::ANGELIC) > 0) {
            angelic[quantity.angelic] = in_item;
            quantity.angelic += 1;
            goto DEPOSIT_END;
        }
        if ((wpntypecode & ITEM::TYPE::SHIELD) > 0) {
            shields[quantity.shields] = in_item;
            quantity.shields += 1;
            goto DEPOSIT_END;
        }
        if ((wpntypecode & ITEM::TYPE::STAFF) > 0) {
            staffs[quantity.staffs] = in_item;
            quantity.staffs += 1;
            goto DEPOSIT_END;
        }
        if ((wpntypecode & ITEM::TYPE::CLAW) > 0) {
            claws[quantity.claws] = in_item;
            quantity.claws += 1;
            goto DEPOSIT_END;
        }
        if ((wpntypecode & ITEM::TYPE::ITEM) > 0) {
            items[quantity.items] = in_item;
            quantity.items += 1;
            goto DEPOSIT_END;
        }
        DEPOSIT_END: isFull();
    } else {
        SDL_Log("Convoy is full");
    }
} 

void Convoy::isFull() {
    int sum = quantity.swords + quantity.lances + quantity.axes +
        quantity.bows + quantity.trinkets + quantity.offhands + quantity.elemental +
        quantity.demonic + quantity.angelic + quantity.shields + 
        quantity.staffs + quantity.claws + quantity.items;
    full = (sum >= DEFAULT::CONVOY_SIZE);
}

void Convoy::printContents(int wpntype) {
    Inventory_item * tempitems = getItems(wpntype);
    int tempqty = getQuantity(wpntype);
    SDL_Log("Quantity: %d \nArray:\n", tempqty);
    for (int i = 0; i < tempqty; i++) {
        if (tempitems[i].id > 0) {
            SDL_Log("%d: %s", i, all_weapons[tempitems[i].id].getName().c_str());
        }
    }
}

void Convoy::printStats(int wpntype, int stattype) {
    Inventory_item * tempitems = getItems(wpntype);
    int tempqty = getQuantity(wpntype);
    std::string statname = statNames[stattype];
    std::vector<int> vecstats = getStats(wpntype, stattype);
    SDL_Log("Quantity: %d \n%s \t Wpn \n", tempqty, statname.c_str());
    for (int i = 0; i < tempqty; i++) {
        if (tempitems[i].id > 0) {
            SDL_Log("%d \t %s", vecstats[i], all_weapons[tempitems[i].id].getName().c_str());
        }
    }
}

void Convoy::sortused(int wpntype) {
    SDL_Log("In sortused");
    std::vector<int> vecid;
    std::vector<int> vecusesleft;
    std::vector<int> vecwhere;
    std::vector<int> uniqueids;
    
    vecid = getStats(wpntype, ITEM::STAT::ID);    
    vecusesleft = getStats(wpntype, ITEM::STAT::USES_LEFT);    
    // int arrusesleft[DEFAULT::CONVOY_SIZE];
    // std::copy(vecusesleft.begin(), vecusesleft.end(), arrusesleft);
    int * arrusesleft = &vecusesleft[0];

    uniqueids = cppuniques(vecid);
    for (int i = 0; i < uniqueids.size(); i++) {
        vecwhere = cppwhere(uniqueids[i], vecid);
        if (vecwhere[0] != vecwhere.back()) {
            quicksort(arrusesleft, vecwhere[0], vecwhere.back(), wpntype);
        }
    }
}

void Convoy::sortStats(int wpntype, int stattype) {
    std::vector<int> vecid = getStats(wpntype, stattype);
    int* arr = &vecid[0];
    int high = getQuantity(wpntype);
    quicksort(arr, 0, high - 1, wpntype);
}

void Convoy::sort(int wpntype, int stattype) {
    sortStats(wpntype, stattype);
    sortused(wpntype);
}

Inventory_item * Convoy::getItems(int wpntype) {
    static Inventory_item temp[DEFAULT::CONVOY_SIZE];
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
    return(temp);
}

Quantity Convoy::getQuantity() {
    return(quantity);
}

int Convoy::getQuantity(int wpntype) {
    static int temp;
    switch (wpntype) {
        case ITEM::TYPE::SWORD:
            temp = quantity.swords;
            break;
        case ITEM::TYPE::LANCE:
            temp = quantity.lances;
            break;
        case ITEM::TYPE::AXE:
            temp = quantity.axes;
            break;
        case ITEM::TYPE::BOW:
            temp = quantity.bows;
            break;
        case ITEM::TYPE::TRINKET:
            temp = quantity.trinkets;
            break;
        case ITEM::TYPE::OFFHAND:
            temp = quantity.offhands;
            break;
        case ITEM::TYPE::ELEMENTAL:
            temp = quantity.elemental;
            break;
        case ITEM::TYPE::DEMONIC:
            temp = quantity.demonic;
            break;
        case ITEM::TYPE::ANGELIC:
            temp = quantity.angelic;
            break;
        case ITEM::TYPE::SHIELD:
            temp = quantity.shields;
            break;
        case ITEM::TYPE::STAFF:
            temp = quantity.staffs;
            break;
        case ITEM::TYPE::CLAW:
            temp = quantity.claws;
            break;
        case ITEM::TYPE::ITEM:
            temp = quantity.items;
            break;
    }
    return(temp);
}


std::vector<int> Convoy::getStats(int wpntype, int stattype) {
    int tempqty = getQuantity(wpntype);
    std::vector<int> vecstats;
    Inventory_item * temp = getItems(wpntype);
    for (int i = 0; i < tempqty; i++) {
        switch(stattype) {
            case ITEM::STAT::ID:
                vecstats.push_back(temp[i].id);
                break;
            case ITEM::STAT::PMIGHT:
                vecstats.push_back(all_weapons[temp[i].id].getStats().Pmight);
                break;
            case ITEM::STAT::MMIGHT:
                vecstats.push_back(all_weapons[temp[i].id].getStats().Mmight);
                break;
            case ITEM::STAT::HIT:
                vecstats.push_back(all_weapons[temp[i].id].getStats().combat.hit);
                break;
            case ITEM::STAT::DODGE:
                vecstats.push_back(all_weapons[temp[i].id].getStats().combat.dodge);
                break;
            case ITEM::STAT::CRIT:
                vecstats.push_back(all_weapons[temp[i].id].getStats().combat.crit);
                break;
            case ITEM::STAT::FAVOR:
                vecstats.push_back(all_weapons[temp[i].id].getStats().combat.favor);
                break;
            case ITEM::STAT::WGT:
                vecstats.push_back(all_weapons[temp[i].id].getStats().wgt);
                break;
            case ITEM::STAT::USES:
                vecstats.push_back(all_weapons[temp[i].id].getStats().uses);
                break;            
            case ITEM::STAT::USES_LEFT:
                vecstats.push_back(all_weapons[temp[i].id].getStats().uses - temp[i].used);
                break;
            case ITEM::STAT::USED:
                vecstats.push_back(temp[i].used);
                break;            
            case ITEM::STAT::PROF:
                vecstats.push_back(all_weapons[temp[i].id].getStats().prof);
                break;
            case ITEM::STAT::RANGEMIN:
                vecstats.push_back(all_weapons[temp[i].id].getStats().range[0]);
                break;
            case ITEM::STAT::RANGEMAX:
                vecstats.push_back(all_weapons[temp[i].id].getStats().range[1]);
                break;
            case ITEM::STAT::HANDLEFT:
                vecstats.push_back(all_weapons[temp[i].id].getStats().hand[0]);
                break;
            case ITEM::STAT::HANDRIGHT:
                vecstats.push_back(all_weapons[temp[i].id].getStats().hand[1]);
                break;
            case ITEM::STAT::PRICE:
                vecstats.push_back(all_weapons[temp[i].id].getStats().price);
                break;
            case ITEM::STAT::HEAL:
                vecstats.push_back(all_weapons[temp[i].id].getStats().heal);
                break;
        }
    }
    return(vecstats);
}

Inventory_item Convoy::withdraw(int in_index, int wpntype) {
    Inventory_item temp;
    Inventory_item empty;
    SDL_Log("Withdraw. Index: %d", in_index);
    switch (wpntype) {
        case ITEM::TYPE::SWORD:
            SDL_Log("Withdrawing a sword");
            temp = swords[in_index];
            swapWpn(wpntype, in_index, quantity.swords);
            swords[quantity.swords] = empty;
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
    isFull();
    return(temp);
}

void Convoy::earn(int in_money) {
    bank += in_money;
} 

void Convoy::spend(int out_money) {
    bank += out_money;
}

void Convoy::writeXML(const char * filename, const bool append) {
    SDL_Log("writeXML Convoy to: %s\n", filename);
    // How to write files so that it is modifiable by randos?
    PHYSFS_file * fp;
    char buffer[DEFAULT::BUFFER_SIZE];
    if (append) {
        fp = PHYSFS_openAppend(filename);
    } else {
        fp = PHYSFS_openWrite(filename);
    }
    tinyxml2::XMLDocument xmlDoc;
    xmlDoc.InsertFirstChild(xmlDoc.NewDeclaration());

    tinyxml2::XMLElement * pConvoy = xmlDoc.NewElement("Convoy");
    xmlDoc.InsertEndChild(pConvoy);    

    tinyxml2::XMLElement * ptemp;
    std::vector<std::string> names;
    Inventory_item * tempitem;
    int quantity;
    int i = 1;
    while (i < ITEM::TYPE::END) {
        SDL_Log("i: %d", i);
        names = wpnTypes(i);
        quantity = getQuantity(i);
        SDL_Log("name: %s", names[0].c_str());
        tempitem = getItems(i);
        ptemp = xmlDoc.NewElement(names[0].c_str());
        pConvoy->InsertEndChild(ptemp);
        writeXML_items(&xmlDoc, ptemp, tempitem, quantity);
        i*=2;
    }

    tinyxml2::XMLPrinter printer;

    xmlDoc.Print(&printer);
    char longbuffer[printer.CStrSize()];
    stbsp_sprintf(longbuffer, printer.CStr());
    PHYSFS_writeBytes(fp, longbuffer, printer.CStrSize());

    PHYSFS_close(fp);
}

void testConvoy() {
    // testConvoyfull();
    // testConvoysortStats();
    testConvoyWriteXML();
    // testConvoysortused();
}

void testConvoyfull() {
    Convoy test_convoy;
    Inventory_item temp;
    temp.id = ITEM::NAME::WOODEN_SWORD;
    test_convoy.deposit(temp);
    for (int i = 0; i < (DEFAULT::CONVOY_SIZE + 2); i++) {
        test_convoy.deposit(temp);
    }
    test_convoy.withdraw(0, ITEM::TYPE::SWORD);
    test_convoy.withdraw(1, ITEM::TYPE::SWORD);
    temp.id = ITEM::NAME::DAMAS_LANCE;
    test_convoy.deposit(temp);
    test_convoy.deposit(temp);
}

void testConvoysortused() {
    SDL_Log("Testing Convoy sorting for stats and uses abilities");
    Convoy test_convoy;
    Inventory_item temp;
    temp.id = ITEM::NAME::WOODEN_SWORD;
    test_convoy.deposit(temp);
    temp.id = ITEM::NAME::KITCHEN_KNIFE;
    test_convoy.deposit(temp);
    temp.id = ITEM::NAME::WRATH_SWORD;
    test_convoy.deposit(temp);
    temp.id = ITEM::NAME::FLEURET;
    test_convoy.deposit(temp);
    temp.id = ITEM::NAME::DAMAS_SWORD;
    temp.used = 0;
    test_convoy.deposit(temp);
    temp.id = ITEM::NAME::DAMAS_SWORD;
    temp.used = 1;
    test_convoy.deposit(temp);
    temp.id = ITEM::NAME::WOODEN_SWORD;
    temp.used = 1;
    test_convoy.deposit(temp);
    temp.id = ITEM::NAME::WOODEN_SWORD;
    temp.used = 2;
    test_convoy.deposit(temp);
    temp.id = ITEM::NAME::WOODEN_SWORD;
    temp.used = 20;
    test_convoy.deposit(temp);
    temp.id = ITEM::NAME::WOODEN_SWORD;
    temp.used = 21;
    test_convoy.deposit(temp);
    temp.id = ITEM::NAME::WOODEN_SWORD;
    temp.used = 10;
    test_convoy.deposit(temp);
    temp.id = ITEM::NAME::WOODEN_SWORD;
    temp.used = 11;
    test_convoy.deposit(temp);
    temp.id = ITEM::NAME::WOODEN_SWORD;
    temp.used = 12;
    test_convoy.deposit(temp);
    temp.id = ITEM::NAME::WOODEN_SWORD;
    test_convoy.deposit(temp);
    temp.id = ITEM::NAME::IRON_SWORD;
    test_convoy.deposit(temp);
    temp.id = ITEM::NAME::STEEL_SWORD;
    test_convoy.deposit(temp);
    SDL_Log("SWORD: Base Convoy Order.");
    test_convoy.printContents(ITEM::TYPE::SWORD);
    test_convoy.printStats(ITEM::TYPE::SWORD, ITEM::STAT::USES_LEFT);
    SDL_Log("Sorting swords according to Pmight");
    test_convoy.sortStats(ITEM::TYPE::SWORD, ITEM::STAT::PMIGHT);
    test_convoy.sort(ITEM::TYPE::SWORD, ITEM::STAT::PMIGHT);
    test_convoy.printStats(ITEM::TYPE::SWORD, ITEM::STAT::USES_LEFT);
}

void testConvoyWriteXML() {
    SDL_Log("Testing Convoy for writeXML abilities.");
    Convoy test_convoy;
    Inventory_item temp;
    temp.id = ITEM::NAME::WOODEN_SWORD;
    test_convoy.deposit(temp);
    temp.id = ITEM::NAME::KITCHEN_KNIFE;
    test_convoy.deposit(temp);
    temp.id = ITEM::NAME::WRATH_SWORD;
    test_convoy.deposit(temp);
    temp.id = ITEM::NAME::FLEURET;
    test_convoy.deposit(temp);
    temp.id = ITEM::NAME::DAMAS_SWORD;
    temp.used = 0;
    test_convoy.deposit(temp);
    temp.id = ITEM::NAME::DAMAS_SWORD;
    temp.used = 1;
    test_convoy.deposit(temp);
    temp.id = ITEM::NAME::WOODEN_SWORD;
    temp.used = 1;
    test_convoy.deposit(temp);
    temp.id = ITEM::NAME::WOODEN_SWORD;
    temp.used = 2;
    test_convoy.deposit(temp);
    temp.id = ITEM::NAME::WOODEN_SWORD;
    temp.used = 20;
    test_convoy.deposit(temp);
    temp.id = ITEM::NAME::WOODEN_SWORD;
    temp.used = 21;
    test_convoy.deposit(temp);
    temp.id = ITEM::NAME::WOODEN_SWORD;
    temp.used = 10;
    test_convoy.deposit(temp);
    temp.id = ITEM::NAME::WOODEN_SWORD;
    temp.used = 11;
    test_convoy.deposit(temp);
    temp.id = ITEM::NAME::WOODEN_SWORD;
    temp.used = 12;
    test_convoy.deposit(temp);
    temp.id = ITEM::NAME::WOODEN_SWORD;
    test_convoy.deposit(temp);
    temp.id = ITEM::NAME::IRON_SWORD;
    test_convoy.deposit(temp);
    temp.id = ITEM::NAME::STEEL_SWORD;

    temp.id = ITEM::NAME::DAMAS_LANCE;
    test_convoy.deposit(temp);
    temp.id = ITEM::NAME::ACHILLES;
    test_convoy.deposit(temp);
    temp.id = ITEM::NAME::VEL;
    test_convoy.deposit(temp);
    temp.id = ITEM::NAME::IRON_LANCE;
    test_convoy.deposit(temp);
    temp.id = ITEM::NAME::TONBOKIRI;
    test_convoy.deposit(temp);
    temp.id = ITEM::NAME::DAMAS_LANCE;
    test_convoy.deposit(temp);
    temp.id = ITEM::NAME::WRATH_LANCE;
    test_convoy.deposit(temp);
    temp.id = ITEM::NAME::WRATH_LANCE;
    test_convoy.deposit(temp);
    temp.id = ITEM::NAME::TRIDENT;
    test_convoy.deposit(temp);
    temp.id = ITEM::NAME::SPEAR;
    test_convoy.deposit(temp);
    test_convoy.writeXML("convoy_test.xml");
}

void testConvoysortStats() {
    SDL_Log("Testing Convoy for stat sorting abilities");
    Convoy test_convoy;
    Inventory_item temp;
    temp.id = ITEM::NAME::WOODEN_SWORD;
    test_convoy.deposit(temp);
    temp.id = ITEM::NAME::KITCHEN_KNIFE;
    test_convoy.deposit(temp);
    temp.id = ITEM::NAME::WRATH_SWORD;
    test_convoy.deposit(temp);
    temp.id = ITEM::NAME::FLEURET;
    test_convoy.deposit(temp);
    temp.id = ITEM::NAME::DAMAS_SWORD;
    test_convoy.deposit(temp);
    temp.id = ITEM::NAME::DAMAS_SWORD;
    test_convoy.deposit(temp);
    temp.id = ITEM::NAME::WOODEN_SWORD;
    test_convoy.deposit(temp);
    temp.id = ITEM::NAME::WOODEN_SWORD;
    test_convoy.deposit(temp);
    temp.id = ITEM::NAME::IRON_SWORD;
    test_convoy.deposit(temp);
    temp.id = ITEM::NAME::STEEL_SWORD;
    test_convoy.deposit(temp);
    temp.id = ITEM::NAME::DAMAS_SWORD;
    test_convoy.deposit(temp);
    temp.id = ITEM::NAME::ESPADON;
    test_convoy.deposit(temp);
    temp.id = ITEM::NAME::EXSANGUE;
    test_convoy.deposit(temp);
    temp.id = ITEM::NAME::SECUNDUS;
    test_convoy.deposit(temp);
    temp.id = ITEM::NAME::FAUCHON;
    test_convoy.deposit(temp);
    temp.id = ITEM::NAME::SABER;
    test_convoy.deposit(temp);
    temp.id = ITEM::NAME::GALATINE;
    test_convoy.deposit(temp);
    temp.id = ITEM::NAME::HAUTECLAIRE;
    test_convoy.deposit(temp);
    temp.id = ITEM::NAME::BROADSWORD;
    test_convoy.deposit(temp);
    temp.id = ITEM::NAME::MERCIFUL_BLADE;
    test_convoy.deposit(temp);

    temp.id = ITEM::NAME::DAMAS_LANCE;
    test_convoy.deposit(temp);
    temp.id = ITEM::NAME::ACHILLES;
    test_convoy.deposit(temp);
    temp.id = ITEM::NAME::VEL;
    test_convoy.deposit(temp);
    temp.id = ITEM::NAME::IRON_LANCE;
    test_convoy.deposit(temp);
    temp.id = ITEM::NAME::TONBOKIRI;
    test_convoy.deposit(temp);
    temp.id = ITEM::NAME::DAMAS_LANCE;
    test_convoy.deposit(temp);
    temp.id = ITEM::NAME::WRATH_LANCE;
    test_convoy.deposit(temp);
    temp.id = ITEM::NAME::WRATH_LANCE;
    test_convoy.deposit(temp);
    temp.id = ITEM::NAME::TRIDENT;
    test_convoy.deposit(temp);
    temp.id = ITEM::NAME::SPEAR;
    test_convoy.deposit(temp);

    SDL_Log("SWORD: Base Convoy Order.");
    test_convoy.printContents(ITEM::TYPE::SWORD);
    SDL_Log("Sorting swords according to Pmight");
    test_convoy.sortStats(ITEM::TYPE::SWORD, ITEM::STAT::PMIGHT);
    test_convoy.printStats(ITEM::TYPE::SWORD, ITEM::STAT::PMIGHT);
    SDL_Log("Sorting swords according to Mmight");
    test_convoy.sortStats(ITEM::TYPE::SWORD, ITEM::STAT::MMIGHT);
    test_convoy.printStats(ITEM::TYPE::SWORD, ITEM::STAT::MMIGHT);
    SDL_Log("Sorting swords according to hit");
    test_convoy.sortStats(ITEM::TYPE::SWORD, ITEM::STAT::HIT);
    test_convoy.printStats(ITEM::TYPE::SWORD, ITEM::STAT::HIT);
    SDL_Log("Sorting swords according to dodge");
    test_convoy.sortStats(ITEM::TYPE::SWORD, ITEM::STAT::DODGE);
    test_convoy.printStats(ITEM::TYPE::SWORD, ITEM::STAT::DODGE);
    SDL_Log("Sorting swords according to crit");
    test_convoy.sortStats(ITEM::TYPE::SWORD, ITEM::STAT::CRIT);
    test_convoy.printStats(ITEM::TYPE::SWORD, ITEM::STAT::CRIT);
    SDL_Log("Sorting swords according to favor");
    test_convoy.sortStats(ITEM::TYPE::SWORD, ITEM::STAT::FAVOR);
    test_convoy.printStats(ITEM::TYPE::SWORD, ITEM::STAT::FAVOR);
    SDL_Log("Sorting swords according to weight");
    test_convoy.sortStats(ITEM::TYPE::SWORD, ITEM::STAT::WGT);
    test_convoy.printStats(ITEM::TYPE::SWORD, ITEM::STAT::WGT);
    SDL_Log("Sorting swords according to uses");
    test_convoy.sortStats(ITEM::TYPE::SWORD, ITEM::STAT::USES);
    test_convoy.printStats(ITEM::TYPE::SWORD, ITEM::STAT::USES);
    SDL_Log("Sorting swords according to proficiency");
    test_convoy.sortStats(ITEM::TYPE::SWORD, ITEM::STAT::PROF);
    test_convoy.printStats(ITEM::TYPE::SWORD, ITEM::STAT::PROF);
    SDL_Log("Sorting swords according to price");
    test_convoy.sortStats(ITEM::TYPE::SWORD, ITEM::STAT::PRICE);
    test_convoy.printStats(ITEM::TYPE::SWORD, ITEM::STAT::PRICE);

    SDL_Log("LANCE: Base Convoy Order.");
    test_convoy.printContents(ITEM::TYPE::LANCE);
    SDL_Log("Sorting lances according to Pmight");
    test_convoy.sortStats(ITEM::TYPE::LANCE, ITEM::STAT::PMIGHT);
    test_convoy.printStats(ITEM::TYPE::LANCE, ITEM::STAT::PMIGHT);
    SDL_Log("Sorting lances according to Mmight");
    test_convoy.sortStats(ITEM::TYPE::LANCE, ITEM::STAT::MMIGHT);
    test_convoy.printStats(ITEM::TYPE::LANCE, ITEM::STAT::MMIGHT);
    SDL_Log("Sorting lances according to hit");
    test_convoy.sortStats(ITEM::TYPE::LANCE, ITEM::STAT::HIT);
    test_convoy.printStats(ITEM::TYPE::LANCE, ITEM::STAT::HIT);
    SDL_Log("Sorting lances according to dodge");
    test_convoy.sortStats(ITEM::TYPE::LANCE, ITEM::STAT::DODGE);
    test_convoy.printStats(ITEM::TYPE::LANCE, ITEM::STAT::DODGE);
    SDL_Log("Sorting lances according to crit");
    test_convoy.sortStats(ITEM::TYPE::LANCE, ITEM::STAT::CRIT);
    test_convoy.printStats(ITEM::TYPE::LANCE, ITEM::STAT::CRIT);
    SDL_Log("Sorting lances according to favor");
    test_convoy.sortStats(ITEM::TYPE::LANCE, ITEM::STAT::FAVOR);
    test_convoy.printStats(ITEM::TYPE::LANCE, ITEM::STAT::FAVOR);
    SDL_Log("Sorting lances according to weight");
    test_convoy.sortStats(ITEM::TYPE::LANCE, ITEM::STAT::WGT);
    test_convoy.printStats(ITEM::TYPE::LANCE, ITEM::STAT::WGT);
    SDL_Log("Sorting lances according to uses");
    test_convoy.sortStats(ITEM::TYPE::LANCE, ITEM::STAT::USES);
    test_convoy.printStats(ITEM::TYPE::LANCE, ITEM::STAT::USES);
    SDL_Log("Sorting lances according to proficiency");
    test_convoy.sortStats(ITEM::TYPE::LANCE, ITEM::STAT::PROF);
    test_convoy.printStats(ITEM::TYPE::LANCE, ITEM::STAT::PROF);
    SDL_Log("Sorting lances according to minimum range");
    test_convoy.sortStats(ITEM::TYPE::LANCE, ITEM::STAT::RANGEMIN);
    test_convoy.printStats(ITEM::TYPE::LANCE, ITEM::STAT::RANGEMIN);
    SDL_Log("Sorting lances according to maximum range");
    test_convoy.sortStats(ITEM::TYPE::LANCE, ITEM::STAT::RANGEMAX);
    test_convoy.printStats(ITEM::TYPE::LANCE, ITEM::STAT::RANGEMAX);
    SDL_Log("Sorting lances according to price");
    test_convoy.sortStats(ITEM::TYPE::LANCE, ITEM::STAT::PRICE);
    test_convoy.printStats(ITEM::TYPE::LANCE, ITEM::STAT::PRICE);    
}
