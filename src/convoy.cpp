
#include "convoy.hpp"

Convoy::Convoy() {

}

void Convoy::deposit(Inventory_item in_item) {
    short unsigned int type = all_weapons[in_item.id].getType();

  if ((type & ITEM::TYPE::SWORD) > 0) {
        sword[quantity] = in_item;
    }
    if ((in_type & ITEM::TYPE::LANCE) > 0) {
        lance[quantity] = in_item;
    }
    if ((in_type & ITEM::TYPE::AXE) > 0) {
        axe[quantity] = in_item;
    }
    if ((in_type & ITEM::TYPE::BOW) > 0) {
        bow[quantity] = in_item;
    }
    if ((in_type & ITEM::TYPE::TRINKET) > 0) {
        trinket[quantity] = in_item;
    }
    if ((in_type & ITEM::TYPE::OFFHAND) > 0) {
        offhand[quantity] = in_item;
    }
    if ((in_type & ITEM::TYPE::ELEMENTAL) > 0) {
        elemental[quantity] = in_item;
    }
    if ((in_type & ITEM::TYPE::DEMONIC) > 0) {
        demonic[quantity] = in_item;
    }
    if ((in_type & ITEM::TYPE::ANGELIC) > 0) {
        angelic[quantity] = in_item;
    }
    if ((in_type & ITEM::TYPE::SHIELD) > 0) {
        shield[quantity] = in_item;
    }
    if ((in_type & ITEM::TYPE::STAFF) > 0) {
        staff[quantity] = in_item;
    }
    if ((in_type & ITEM::TYPE::CLAW) > 0) {
        claw[quantity] = in_item;
    }
    if ((in_type & ITEM::TYPE::ITEM) > 0) {
        item[quantity] = in_item;
    }
    quantity += 1;
} 
      

void Convoy::withdraw(int index) {
    Inventory_item temp;
    sword[quantity] = temp;
    lance[quantity] = temp;
    axe[quantity] = temp;
    bow[quantity] = temp;
    offhand[quantity] = temp;
    trinket[quantity] = temp;
    elemental[quantity] = temp;
    demonic[quantity] = temp;
    angelic[quantity] = temp;
    shield[quantity] = temp;
    staff[quantity] = temp;
    claw[quantity] = temp;
    item[quantity] = temp;
    quantity -= 1;
}


void earn(int in_money) {
    bank += in_money;
} 

void spend(int out_money) {
    bank += out_money;
} 