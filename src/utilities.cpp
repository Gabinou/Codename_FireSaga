#include "utilities.hpp"
#include "game.hpp"


int pingpong(int current, int upper, int lower) {
    // Returns "pingpong" index.
    // modulo: x % 2 = 0,1,2,0,1,2,0...for x++
    // pingpong(x, 2, 0) = 0,1,2,1,0,1,2... for x++
    int mod_factor = (2 * (upper - lower) - 2);
    int term1 = mod_factor - (current % mod_factor);
    int term2 = current % mod_factor;
    return (std::min(term1, term2) + lower);
}

int geometricslide(int distance, float geo_factor) {
    // Returns geometrically decreasing indices.
    // Ex: distance/geo_factor -> distance/geo_factor**2 -> distance/geo_factor**3
    int sign = sgn(distance); 
    int out = sign * std::max(sign * (int)(distance / geo_factor), 1);
    return (out);
    // std::abs() possible instead of sign*distance,
    // but this is more elegant.
}

void printarr(int arr[], int size) {
    SDL_Log("Array:\n");
    for (int i = 0; i < size; i++) {
        SDL_Log("%d\n", arr[i]);
    }
}

void printvec(std::vector<int> vec) {
    SDL_Log("Vector:\n");
    for (int i = 0; i < vec.size(); i++) {
        SDL_Log("%d\n", vec[i]);
    }
}

std::vector<int> cppwhere(int tofind, std::vector<int> vec) {
    std::vector<int> found_inds;
    for (int i = 0; i < vec.size(); i++) {
        if (vec[i] == tofind) {
            found_inds.push_back(i);
        }
    }
    return(found_inds);
}

std::vector<int> cppuniques(std::vector<int> vec) {
    std::vector<int> uniques;
    std::vector<int> inuniques;
    uniques.push_back(vec[0]);    
    for (int i = 1; i < vec.size(); i++) {
        inuniques = cppwhere(vec[i], uniques);
        if (inuniques.size() == 0) {
            uniques.push_back(vec[i]);
        }
    }
    return(uniques);
}

int * cwhere(int tofind, int arr[], int size) {
    int * found_inds;
    found_inds = (int*) malloc(size * sizeof(*found_inds));
    int found_num = 1;
    for (int i = 0; i < size; i++) {
        if (arr[i] == tofind) {
            found_inds[found_num] = i;
            found_num++;
        }
    }
    found_inds = (int*) realloc(found_inds, found_num*sizeof((*found_inds) / size));
    found_inds[0] = found_num - 1;
    return(found_inds);
}

int * cuniques(int arr[], int size) {
    int * uniques;
    uniques = (int*) malloc((size + 1) * sizeof(*uniques));
    int * inunique;
    int found_uniques = 2;
    uniques[1] = arr[0];
    for (int i = 1; i < size; i++) {
        inunique = cwhere(arr[i], uniques, found_uniques);
        if (inunique[0] == 0) {
            uniques[found_uniques] = arr[i];
            found_uniques++;
        }
    }
    uniques = (int*) realloc(uniques, found_uniques*sizeof((*uniques) / size));
    uniques[0] = found_uniques - 1;
    return(uniques);
}

std::vector<std::string> unitType(unsigned short int in_typecode) {
    std::vector<std::string> names;
    if ((in_typecode & UNIT::TYPE::HUMAN) > 0) {
        names.push_back("Human");
    }
    if ((in_typecode & UNIT::TYPE::MOUNTED) > 0) {
        names.push_back("Mounted");
    }
    if ((in_typecode & UNIT::TYPE::FLYING) > 0) {
        names.push_back("Flying");
    }
    if ((in_typecode & UNIT::TYPE::ARMOR) > 0) {
        names.push_back("Armor");
    }
    if ((in_typecode & UNIT::TYPE::DEMON) > 0) {
        names.push_back("Demon");
    }
    if ((in_typecode & UNIT::TYPE::ANGEL) > 0) {
        names.push_back("Angel");
    }
    return(names);
}

std::vector<std::string> wpnEffects(long unsigned int in_effect) {
    std::vector<std::string> names;
    if ((in_effect & ITEM::EFFECT::KILL1P) > 0) {
        names.push_back("1% kill");
    }
    if ((in_effect & ITEM::EFFECT::BRAVE2X) > 0) {
        names.push_back("Double attack");
    }
    if ((in_effect & ITEM::EFFECT::BRAVE3X) > 0) {
        names.push_back("Triple attack");
    }
    if ((in_effect & ITEM::EFFECT::BRAVE4X) > 0) {
        names.push_back("Quadruple attack");
    }
    if ((in_effect & ITEM::EFFECT::NOCOUNTER) > 0) {
        names.push_back("No counter-attack");
    }
    if ((in_effect & ITEM::EFFECT::IGNORE_DEF) > 0) {
        names.push_back("Ignore defense");
    }
    if ((in_effect & ITEM::EFFECT::IGNORE_RES) > 0) {
        names.push_back("Ignore resistance");
    }
    if ((in_effect & ITEM::EFFECT::IGNORE_SHIELD) > 0) {
        names.push_back("Ignore shield");
    }
    if ((in_effect & ITEM::EFFECT::USE_HEAL) > 0) {
        names.push_back("Heal");
    }
    if ((in_effect & ITEM::EFFECT::USE_BUFF) > 0) {
        names.push_back("Buff");
    }
    if ((in_effect & ITEM::EFFECT::USE_DIVINE) > 0) {
        names.push_back("Divine shield");
    }
    if ((in_effect & ITEM::EFFECT::NO_CRIT) > 0) {
        names.push_back("Cannot be critted");
    }
    if ((in_effect & ITEM::EFFECT::NO_ATTACK) > 0) {
        names.push_back("Cannot be attacked");
    }
    if ((in_effect & ITEM::EFFECT::CHARM) > 0) {
        names.push_back("Charm");
    }
    if ((in_effect & ITEM::EFFECT::IMMUNE_MAGIC) > 0) {
        names.push_back("Immune to magic");
    }
    if ((in_effect & ITEM::EFFECT::IMMUNE_PHYS) > 0) {
        names.push_back("Immune to physical");
    }
    if ((in_effect & ITEM::EFFECT::COUP_DE_GRACE) > 0) {
        names.push_back("Coup de grâce");
    }
    if ((in_effect & ITEM::EFFECT::BREAK_WEAPON) > 0) {
        names.push_back("Break weapon");
    }
    if ((in_effect & ITEM::EFFECT::EXP2X) > 0) {
        names.push_back("Paragon");
    }
    if ((in_effect & ITEM::EFFECT::LIFESTEAL) > 0) {
        names.push_back("Lifesteal");
    }
    if ((in_effect & ITEM::EFFECT::POISON) > 0) {
        names.push_back("Poison");
    }
    if ((in_effect & ITEM::EFFECT::CURSED) > 0) {
        names.push_back("Cursed");
    }
    if ((in_effect & ITEM::EFFECT::MASOCHISM) > 0) {
        names.push_back("Masochism");
    }
    if ((in_effect & ITEM::EFFECT::STUN) > 0) {
        names.push_back("Stun");
    }
    if ((in_effect & ITEM::EFFECT::PASS) > 0) {
        names.push_back("Pass");
    }
    if ((in_effect & ITEM::EFFECT::JOUST) > 0) {
        names.push_back("Joust");
    }
    if ((in_effect & ITEM::EFFECT::SPARE) > 0) {
        names.push_back("Spare");
    }
    if ((in_effect & ITEM::EFFECT::CAPTURE) > 0) {
        names.push_back("Capture");
    }
    if ((in_effect & ITEM::EFFECT::HALF_DMG) > 0) {
        names.push_back("Half-damage");
    }
    if ((in_effect & ITEM::EFFECT::BREAK_SHIELD) > 0) {
        names.push_back("Shield breaker");
    }
    return(names);
}

// std::string wpnType(short unsigned int in_type) {
//     std::string name;
//     switch (in_type) {
//         case ITEM::TYPE::SWORD:
//             name = "Sword";
//             break;
//         case ITEM::TYPE::LANCE:
//             name = "Lance";
//             break;
//         case ITEM::TYPE::AXE:
//             name = "Axe";
//             break;
//         case ITEM::TYPE::BOW:
//             name = "Bow";
//             break;
//         case ITEM::TYPE::TRINKET:
//             name = "Trinket";
//             break;
//         case ITEM::TYPE::OFFHAND:
//             name = "Offhand";
//             break;
//         case ITEM::TYPE::ELEMENTAL:
//             name = "Elemental";
//             break;
//         case ITEM::TYPE::DEMONIC:
//             name = "Demonic";
//             break;
//         case ITEM::TYPE::ANGELIC:
//             name = "Angelic";
//             break;
//         case ITEM::TYPE::SHIELD:
//             name = "Shield";
//             break;
//         case ITEM::TYPE::STAFF:
//             name = "Staff";
//             break;
//         case ITEM::TYPE::CLAW:
//             name = "Claw";
//             break;
//         case ITEM::TYPE::ITEM:
//             name = "Item";
//             break;
//     }
//     return(name);
// }

// std::string sexName(bool in_sex) {
//     std::string name;
//     if(in_sex) {
//         name = "M";
//     } else {
//         name = "F";
//     }
//     return(name);
// }

std::vector<std::string> wpnTypes(short unsigned int in_typecode) {
    std::vector<std::string> types;
    if ((in_typecode & ITEM::TYPE::SWORD) > 0) {
        types.push_back("Sword");
    }
    if ((in_typecode & ITEM::TYPE::LANCE) > 0) {
        types.push_back("Lance");
    }
    if ((in_typecode & ITEM::TYPE::AXE) > 0) {
        types.push_back("Axe");
    }
    if ((in_typecode & ITEM::TYPE::BOW) > 0) {
        types.push_back("Bow");
    }
    if ((in_typecode & ITEM::TYPE::TRINKET) > 0) {
        types.push_back("Trinket");
    }
    if ((in_typecode & ITEM::TYPE::OFFHAND) > 0) {
        types.push_back("Offhand");
    }
    if ((in_typecode & ITEM::TYPE::ELEMENTAL) > 0) {
        types.push_back("Magic");
        // types.push_back("Elemental");
    }
    if ((in_typecode & ITEM::TYPE::DEMONIC) > 0) {
        types.push_back("Demonic");
    }
    if ((in_typecode & ITEM::TYPE::ANGELIC) > 0) {
        types.push_back("Angelic");
    }
    if ((in_typecode & ITEM::TYPE::SHIELD) > 0) {
        types.push_back("Shield");
    }
    if ((in_typecode & ITEM::TYPE::STAFF) > 0) {
        types.push_back("Staff");
    }
    return(types);
}

std::vector<std::string> wpnNames;
void makewpnNames() {
    wpnNames.push_back("");
    wpnNames.push_back("Wooden sword");
    wpnNames.push_back("Kitchen knife");
    wpnNames.push_back("Wrath sword");
    wpnNames.push_back("Fleuret");
    wpnNames.push_back("Rapière");
    wpnNames.push_back("Glaive");
    wpnNames.push_back("Saber");
    wpnNames.push_back("Fauchon");
    wpnNames.push_back("Iron sword");
    wpnNames.push_back("Steel sword");
    wpnNames.push_back("Damas sword");
    wpnNames.push_back("Merciful blade");
    wpnNames.push_back("Broadsword");
    wpnNames.push_back("Espadon");
    wpnNames.push_back("Oodachi");
    wpnNames.push_back("Uchigatana");
    wpnNames.push_back("Exsangue");
    wpnNames.push_back("Hauteclaire");
    wpnNames.push_back("Secundus");
    wpnNames.push_back("Excalibur");
    wpnNames.push_back("Galatine");
    wpnNames.push_back("Raijintou");
    wpnNames.push_back("Fuujintou");
    wpnNames.push_back("Honjou masamune");
    wpnNames.push_back("Raw Iron Slab");
    wpnNames.push_back("Kirito");
    wpnNames.push_back("Killy");
    wpnNames.push_back("Pitchfork");
    wpnNames.push_back("Fighting stick");
    wpnNames.push_back("Iron lance");
    wpnNames.push_back("Wrath lance");
    wpnNames.push_back("Steel lance");
    wpnNames.push_back("Damas lance");
    wpnNames.push_back("Heavy spear");
    wpnNames.push_back("Trident");
    wpnNames.push_back("Merciful lance");
    wpnNames.push_back("Javelin");
    wpnNames.push_back("Pilum");
    wpnNames.push_back("Spear");
    wpnNames.push_back("Vel");
    wpnNames.push_back("Gungnir");
    wpnNames.push_back("Achilles");
    wpnNames.push_back("Tonbokiri");
    wpnNames.push_back("Iron axe");
    wpnNames.push_back("Steel axe");
    wpnNames.push_back("Damas axe");
    wpnNames.push_back("Wrath axe");
    wpnNames.push_back("Throwing axe");
    wpnNames.push_back("Tomahawk");
    wpnNames.push_back("Pick axe");
    wpnNames.push_back("Hoe");
    wpnNames.push_back("Club");
    wpnNames.push_back("Halberd");
    wpnNames.push_back("Hammer");
    wpnNames.push_back("War Pick");
    wpnNames.push_back("Battleaxe");
    wpnNames.push_back("Vidyu");
    wpnNames.push_back("Hephaestus");
    wpnNames.push_back("Adapted Crossbow");
    wpnNames.push_back("Repeatable Crossbow");
    wpnNames.push_back("Crossbow");
    wpnNames.push_back("Short bow");
    wpnNames.push_back("Long bow");
    wpnNames.push_back("Composite bow");
    wpnNames.push_back("Recurve Crossbow");
    wpnNames.push_back("Recurve bow");
    wpnNames.push_back("Apollo");
    wpnNames.push_back("Artemis");
    wpnNames.push_back("Pinaka");
    wpnNames.push_back("Adapted shield");
    wpnNames.push_back("Pot lid");
    wpnNames.push_back("Wooden shield");
    wpnNames.push_back("Cloak");
    wpnNames.push_back("Leather shield");
    wpnNames.push_back("Kite shield");
    wpnNames.push_back("Iron buckler");
    wpnNames.push_back("Steel buckler");
    wpnNames.push_back("Spiked buckler");
    wpnNames.push_back("Scutum");
    wpnNames.push_back("Master shield");
    wpnNames.push_back("Goddess's hand");
    wpnNames.push_back("Ancile");
    wpnNames.push_back("Rock plate");
    wpnNames.push_back("Obsidian plate");
    wpnNames.push_back("Marble plate shield");
    wpnNames.push_back("Divine protector");
    wpnNames.push_back("Wristblade");
    wpnNames.push_back("Shield hook");
    wpnNames.push_back("Retractable wristblade");
    wpnNames.push_back("Main Gauche");
    wpnNames.push_back("Swordbreaker");
    wpnNames.push_back("Trident Dagger");
    wpnNames.push_back("Tantou");
    wpnNames.push_back("Kodachi");
    wpnNames.push_back("Dague");
    wpnNames.push_back("Wakizashi");
    wpnNames.push_back("Braquemard");
    wpnNames.push_back("Miséricorde");
    wpnNames.push_back("Carnwennan");
    wpnNames.push_back("Flintlock");
    wpnNames.push_back("Cibo");
    wpnNames.push_back("Shrunken head");
    wpnNames.push_back("Crystal");
    wpnNames.push_back("Glass orb");
    wpnNames.push_back("Human femur");
    wpnNames.push_back("Void fragment");
    wpnNames.push_back("Demon fang");
    wpnNames.push_back("Angel feather");
    wpnNames.push_back("Angel hair");
    wpnNames.push_back("Vial of light");
    wpnNames.push_back("Maiden's handkerchief");
    wpnNames.push_back("War Horn");
    wpnNames.push_back("Ball lightning");
    wpnNames.push_back("Icicle");
    wpnNames.push_back("Volcano");
    wpnNames.push_back("Wind spear");
    wpnNames.push_back("Ember");
    wpnNames.push_back("Tsunami");
    wpnNames.push_back("God's forge fire");
    wpnNames.push_back("Fingolfin");
    wpnNames.push_back("Mithrandir");
    wpnNames.push_back("Void");
    wpnNames.push_back("Nether");
    wpnNames.push_back("Downfall");
    wpnNames.push_back("Consume");
    wpnNames.push_back("Putrify");
    wpnNames.push_back("Glaurung");
    wpnNames.push_back("Gothmog");
    wpnNames.push_back("Fatalis");
    wpnNames.push_back("Heal");
    wpnNames.push_back("Remedy");
    wpnNames.push_back("Renew");
    wpnNames.push_back("Wish");
    wpnNames.push_back("Pray");
    wpnNames.push_back("Salve");
    wpnNames.push_back("Panacea");
    wpnNames.push_back("Healing Potion");
    wpnNames.push_back("Telperion extract");
    wpnNames.push_back("Laurelin extract");
    wpnNames.push_back("Cross");
}

// std::string wpnName(unsigned short int in_id) {
//     std::string name;
//     switch(in_id) {
//         case ITEM::NAME::WOODEN_SWORD:
//             name = "Wooden sword";
//             break;
//         case ITEM::NAME::KITCHEN_KNIFE:
//             name = "Kitchen knife";
//             break;
//         case ITEM::NAME::WRATH_SWORD:
//             name = "Wrath sword";
//             break;
//         case ITEM::NAME::FLEURET:
//             name = "Fleuret";
//             break;
//         case ITEM::NAME::RAPIERE:
//             name = "Rapière";
//             break;
//         case ITEM::NAME::GLAIVE:
//             name = "Glaive";
//             break;
//         case ITEM::NAME::SABER:
//             name = "Saber";
//             break;
//         case ITEM::NAME::FAUCHON:
//             name = "Fauchon";
//             break;
//         case ITEM::NAME::IRON_SWORD:
//             name = "Iron sword";
//             break;
//         case ITEM::NAME::STEEL_SWORD:
//             name = "Steel sword";
//             break;
//         case ITEM::NAME::DAMAS_SWORD:
//             name = "Damas sword";
//             break;
//         case ITEM::NAME::MERCIFUL_BLADE:
//             name = "Merciful blade";
//             break;
//         case ITEM::NAME::BROADSWORD:
//             name = "Broadsword";
//             break;
//         case ITEM::NAME::ESPADON:
//             name = "Espadon";
//             break;
//         case ITEM::NAME::OODACHI:
//             name = "Oodachi";
//             break;
//         case ITEM::NAME::UCHIGATANA:
//             name = "Uchigatana";
//             break;
//         case ITEM::NAME::EXSANGUE:
//             name = "Exsangue";
//             break;
//         case ITEM::NAME::HAUTECLAIRE:
//             name = "Hauteclaire";
//             break;
//         case ITEM::NAME::SECUNDUS:
//             name = "Secundus";
//             break;
//         case ITEM::NAME::EXCALIBUR:
//             name = "Excalibur";
//             break;
//         case ITEM::NAME::GALATINE:
//             name = "Galatine";
//             break;
//         case ITEM::NAME::RAIJINTOU:
//             name = "Raijintou";
//             break;
//         case ITEM::NAME::FUUJINTOU:
//             name = "Fuujintou";
//             break;
//         case ITEM::NAME::HONJOU_MASAMUNE:
//             name = "Honjou masamune";
//             break;
//         case ITEM::NAME::RAW_IRON_SLAB:
//             name = "Raw iron slab";
//             break;
//         case ITEM::NAME::KIRITO:
//             name = "Kirito";
//             break;
//         case ITEM::NAME::KILLY:
//             name = "Killy";
//             break;

//         case ITEM::NAME::PITCHFORK:
//             name = "Pitchfork";
//             break;
//         case ITEM::NAME::FIGHTING_STICK:
//             name = "Fighting stick";
//             break;
//         case ITEM::NAME::IRON_LANCE:
//             name = "Iron lance";
//             break;
//         case ITEM::NAME::WRATH_LANCE:
//             name = "Wrath lance";
//             break;
//         case ITEM::NAME::STEEL_LANCE:
//             name = "Steel lance";
//             break;
//         case ITEM::NAME::DAMAS_LANCE:
//             name = "Damas lance";
//             break;
//         case ITEM::NAME::HEAVY_SPEAR:
//             name = "Heavy spear";
//             break;
//         case ITEM::NAME::TRIDENT:
//             name = "Trident";
//             break;
//         case ITEM::NAME::MERCIFUL_LANCE:
//             name = "Merciful lance";
//             break;
//         case ITEM::NAME::JAVELIN:
//             name = "Javelin";
//             break;
//         case ITEM::NAME::PILUM:
//             name = "Pilum";
//             break;
//         case ITEM::NAME::SPEAR:
//             name = "Spear";
//             break;
//         case ITEM::NAME::VEL:
//             name = "Vel";
//             break;
//         case ITEM::NAME::GUNGNIR:
//             name = "Gungnir";
//             break;
//         case ITEM::NAME::ACHILLES:
//             name = "Achilles";
//             break;
//         case ITEM::NAME::TONBOKIRI:
//             name = "Tonbokiri";
//             break;

//         case ITEM::NAME::IRON_AXE:
//             name = "Iron axe";
//             break;
//         case ITEM::NAME::STEEL_AXE:
//             name = "Steel axe";
//             break;
//         case ITEM::NAME::DAMAS_AXE:
//             name = "Damas axe";
//             break;
//         case ITEM::NAME::WRATH_AXE:
//             name = "Wrath axe";
//             break;
//         case ITEM::NAME::THROWING_AXE:
//             name = "Throwing axe";
//             break;
//         case ITEM::NAME::TOMAHAWK:
//             name = "Tomahawk";
//             break;
//         case ITEM::NAME::PICK_AXE:
//             name = "Pick axe";
//             break;
//         case ITEM::NAME::HOE:
//             name = "Hoe";
//             break;
//         case ITEM::NAME::CLUB:
//             name = "Club";
//             break;
//         case ITEM::NAME::HALBERD:
//             name = "Halberd";
//             break;
//         case ITEM::NAME::HAMMER:
//             name = "Hammer";
//             break;
//         case ITEM::NAME::WAR_PICK:
//             name = "War pick";
//             break;
//         case ITEM::NAME::BATTLEAXE:
//             name = "Battleaxe";
//             break;
//         case ITEM::NAME::VIDYU:
//             name = "Vidyu";
//             break;
//         case ITEM::NAME::HEPHAESTUS:
//             name = "Hephaestus";
//             break;
//         case ITEM::NAME::ADAPTED_CROSSBOW:
//             name = "Adapted crossbow";
//             break;
//         case ITEM::NAME::REPEATABLE_CROSSBOW:
//             name = "Repeatable crossbow";
//             break;
//         case ITEM::NAME::CROSSBOW:
//             name = "Crossbow";

//             break;
//         case ITEM::NAME::SHORT_BOW:
//             name = "Short bow";
//             break;
//         case ITEM::NAME::LONG_BOW:
//             name = "Long bow";
//             break;
//         case ITEM::NAME::COMPOSITE_BOW:
//             name = "Composite bow";
//             break;
//         case ITEM::NAME::RECURVE_BOW:
//             name = "Recurve bow";
//             break;
//         case ITEM::NAME::APOLLO:
//             name = "Apollo";
//             break;
//         case ITEM::NAME::ARTEMIS:
//             name = "Artemis";
//             break;
//         case ITEM::NAME::PINAKA:
//             name = "Pinaka";
//             break;

//         case ITEM::NAME::ADAPTED_SHIELD:
//             name = "Adapted shield";
//             break;        
//         case ITEM::NAME::POT_LID:
//             name = "Pot lid";
//             break;
//         case ITEM::NAME::WOODEN_SHIELD:
//             name = "Wooden shield";
//             break;
//         case ITEM::NAME::CLOAK:
//             name = "Cloak";
//             break;
//         case ITEM::NAME::LEATHER_SHIELD:
//             name = "Leather shield";
//             break;
//         case ITEM::NAME::KITE_SHIELD:
//             name = "Kite shield";
//             break;
//         case ITEM::NAME::IRON_BUCKLER:
//             name = "Iron buckler";
//             break;
//         case ITEM::NAME::STEEL_BUCKLER:
//             name = "Steel buckler";
//             break;
//         case ITEM::NAME::SPIKED_BUCKLER:
//             name = "Spiked buckler";
//             break;
//         case ITEM::NAME::SCUTUM:
//             name = "Scutum";
//             break;
//         case ITEM::NAME::MASTER_SHIELD:
//             name = "Master shield";
//             break;
//         case ITEM::NAME::GODDESS_HAND:
//             name = "Goddess's hand";
//             break;
//         case ITEM::NAME::ANCILE:
//             name = "Ancile";
//             break;
//         case ITEM::NAME::ROCK_PLATE:
//             name = "Rock plate";
//             break;
//         case ITEM::NAME::OBSIDIAN_PLATE:
//             name = "Obisidan rock plate";
//             break;
//         case ITEM::NAME::MARBLE_PLATE_SHIELD:
//             name = "Marble plate shield";
//             break;
//         case ITEM::NAME::DIVINE_PROTECTOR:
//             name = "Divine protector";
//             break;
//         case ITEM::NAME::SHIELD_HOOK:
//             name = "Shield hook";
//             break;
//         case ITEM::NAME::RETRACTABLE_WRISTBLADE:
//             name = "Retractable wristblade";
//             break;
//         case ITEM::NAME::MAIN_GAUCHE:
//             name = "Main gauche";
//             break;
//         case ITEM::NAME::SWORDBREAKER:
//             name = "Swordbreaker";
//             break;
//         case ITEM::NAME::TRIDENT_DAGGER:
//             name = "Trident dagger";
//             break;
//         case ITEM::NAME::TANTOU:
//             name = "Tantou";
//             break;
//         case ITEM::NAME::KODACHI:
//             name = "Kodachi";
//             break;
//         case ITEM::NAME::DAGUE:
//             name = "Dague";
//             break;
//         case ITEM::NAME::WAKIZASHI:
//             name = "Wakizashi";
//             break;
//         case ITEM::NAME::BRAQUEMARD:
//             name = "Braquemard";
//             break;
//         case ITEM::NAME::MISERICORDE:
//             name = "Misericorde";
//             break;
//         case ITEM::NAME::CARNWENNAN:
//             name = "Carnwennan";
//             break;
//         case ITEM::NAME::FLINTLOCK:
//             name = "Flintlock";
//             break;
//         case ITEM::NAME::CIBO:
//             name = "Cibo";
//             break;

//         case ITEM::NAME::SHRUNKEN_HEAD:
//             name = "Shrunken head";
//             break;
//         case ITEM::NAME::CRYSTAL:
//             name = "Crystal";
//             break;
//         case ITEM::NAME::GLASS_ORB:
//             name = "Glass orb";
//             break;
//         case ITEM::NAME::HUMAN_FEMUR:
//             name = "Human femur";
//             break;
//         case ITEM::NAME::VOID_FRAGMENT:
//             name = "Void fragment";
//             break;
//         case ITEM::NAME::DEMON_FANG:
//             name = "Demon fang";
//             break;
//         case ITEM::NAME::ANGEL_FEATHER:
//             name = "Angel feather";
//             break;
//         case ITEM::NAME::ANGEL_HAIR:
//             name = "Angel hair";
//             break;
//         case ITEM::NAME::VIAL_OF_LIGHT:
//             name = "Vial of light";
//             break;
//         case ITEM::NAME::MAIDENS_HANDKERCHIEF:
//             name = "Maiden's handkerchief";
//             break;
//         case ITEM::NAME::WAR_HORN:
//             name = "War horn";
//             break;

//         case ITEM::NAME::BALL_LIGHTNING:
//             name = "Ball lightning";
//             break;
//         case ITEM::NAME::ICICLE:
//             name = "Icicle";
//             break;
//         case ITEM::NAME::VOLCANO:
//             name = "Volcano";
//             break;
//         case ITEM::NAME::WIND_SPEAR:
//             name = "Wind spear";
//             break;
//         case ITEM::NAME::EMBER:
//             name = "Ember";
//             break;
//         case ITEM::NAME::TSUNAMI:
//             name = "Tsunami";
//             break;
//         case ITEM::NAME::GOD_FORGE_FIRE:
//             name = "God's forge fire";
//             break;
//         case ITEM::NAME::FINGOLFIN:
//             name = "Fingolfin";
//             break;
//         case ITEM::NAME::MITHRANDIR:
//             name = "Mithrandir";
//             break;
//         case ITEM::NAME::VOIDD:
//             name = "Void";
//             break;
//         case ITEM::NAME::NETHER:
//             name = "Nether";
//             break;
//         case ITEM::NAME::DOWNFALL:
//             name = "Downfall";
//             break;
//         case ITEM::NAME::CONSUME:
//             name = "Consume";
//             break;
//         case ITEM::NAME::PUTRIFY:
//             name = "Putrify";
//             break;
//         case ITEM::NAME::GLAURUNG:
//             name = "Glaurung";
//             break;
//         case ITEM::NAME::MORGOTH:
//             name = "Morgoth";
//             break;
//         case ITEM::NAME::GOTHMOG:
//             name = "Gothmog";
//             break;
//         case ITEM::NAME::FATALIS:
//             name = "Fatalis";
//             break;
//     }
//     return(name);
// }


// (*)(Unit) makeheal(Weapon * in_weapon) {

//     void heal_func(Unit * in_unit) {
//         in_unit->getsHealed(weapon->stats.heal);
//     }
//     return(heal_func);
// }

// std::string statName(unsigned short int in_id) {
//     std::string name;
//     switch(in_id) {
//         case ITEM::STAT::PMIGHT:
//             name = "PMight";
//             break;
//         case ITEM::STAT::MMIGHT:
//             name = "Mmight";
//             break;
//         case ITEM::STAT::HIT:
//             name = "Hit";
//             break;
//         case ITEM::STAT::DODGE:
//             name = "Dodge";
//             break;
//         case ITEM::STAT::CRIT:
//             name = "Crit";
//             break;
//         case ITEM::STAT::FAVOR:
//             name = "Favor";
//             break;
//         case ITEM::STAT::WGT:
//             name = "Weight";
//             break;
//         case ITEM::STAT::USES:
//             name = "Uses";
//             break;
//         case ITEM::STAT::USES_LEFT:
//             name = "Uses left";
//             break;
//         case ITEM::STAT::USED:
//             name = "Used";
//             break;
//         case ITEM::STAT::PROF:
//             name = "Prof";
//             break;
//         case ITEM::STAT::RANGEMIN:
//             name = "Rangemin";
//             break;
//         case ITEM::STAT::RANGEMAX:
//             name = "Rangemax";
//             break;
//         case ITEM::STAT::HANDLEFT:
//             name = "Left hand";
//             break;
//         case ITEM::STAT::HANDRIGHT:
//             name = "Right hand";
//             break;
//         case ITEM::STAT::PRICE:
//             name = "Price";
//             break;
//         case ITEM::STAT::HEAL:
//             name = "heal";
//             break;
//         case ITEM::STAT::ID:
//             name = "id";
//             break;
//     }
//     return(name);
// }

std::vector<std::string> unitNames;
void makeunitNames() {
    unitNames.push_back("");
    unitNames.push_back("Erwin");
    unitNames.push_back("Kiara");
    unitNames.push_back("Silou");
    unitNames.push_back("Servil");
    unitNames.push_back("Perignon");
    unitNames.push_back("Poet");
    unitNames.push_back("Reliable");
    unitNames.push_back("Coward");
    unitNames.push_back("Jaigen1H");
    unitNames.push_back("Pirou");
    unitNames.push_back("");
    unitNames.push_back("Zinedan");
    unitNames.push_back("Zidine");
    unitNames.push_back("");
    unitNames.push_back("Bandit");
    unitNames.push_back("Duelist");
    unitNames.push_back("Pickpocket");
    unitNames.push_back("Thief");
    unitNames.push_back("Assassin");
    unitNames.push_back("Archer");
    unitNames.push_back("Marksman");
    unitNames.push_back("Mercenary");
    unitNames.push_back("Hero");
    unitNames.push_back("Corsair");
    unitNames.push_back("Viking");
    unitNames.push_back("Ravager");
    unitNames.push_back("Cavalier");
    unitNames.push_back("Paladin");
    unitNames.push_back("Fencer");
    unitNames.push_back("Mousquetaire");
}

std::vector<std::string> statNames;
void makestatNames() {
    statNames.push_back("");
    statNames.push_back("Pmight");
    statNames.push_back("Mmight");
    statNames.push_back("Hit");
    statNames.push_back("Dodge");
    statNames.push_back("Crit");
    statNames.push_back("Favor");
    statNames.push_back("Wgt");
    statNames.push_back("Uses");
    statNames.push_back("Used");
    statNames.push_back("Uses left");
    statNames.push_back("Prof");
    statNames.push_back("Min range");
    statNames.push_back("Max range");
    statNames.push_back("Left hand");
    statNames.push_back("Right hand");
    statNames.push_back("Price");
    statNames.push_back("Heal");
    statNames.push_back("ID");
    statNames.push_back("");
}

std::vector<std::string> sexNames;
void makesexNames() {
    sexNames.push_back("F");
    sexNames.push_back("M");
}

// std::string unitName(unsigned short int in_id) {
//     std::string name;
//     switch(in_id) {
//         case UNIT::NAME::ERWIN:
//             name = "Erwin";
//             break;
//         case UNIT::NAME::KIARA:
//             name = "Kiara";
//             break;
//         case UNIT::NAME::SILOU:
//             name = "SILOU";
//             break;
//         case UNIT::NAME::SERVIL:
//             name = "Servil";
//             break;
//         case UNIT::NAME::PERIGNON:
//             name = "Perignon";
//             break;
//         case UNIT::NAME::COWARD:
//             name = "Coward";
//             break;
//         case UNIT::NAME::POET:
//             name = "Poet";
//             break;
//         case UNIT::NAME::RELIABLE:
//             name = "Reliable";
//             break;
//         case UNIT::NAME::JAIGEN1H:
//             name = "Jaigen1H";
//             break;
//         case UNIT::NAME::HOTTIE:
//             name = "Hottie";
//             break;
//         case UNIT::NAME::BANDIT:
//             name = "Bandit";
//             break;
//         case UNIT::NAME::DUELIST:
//             name = "Duelist";
//             break;
//         case UNIT::NAME::PICKPOCKET:
//             name = "Pickpocket";
//             break;
//         case UNIT::NAME::THIEF:
//             name = "Thief";
//             break;
//         case UNIT::NAME::ASSASSIN:
//             name = "Assassing";
//             break;
//         case UNIT::NAME::ARCHER:
//             name = "Archer";
//             break;
//         case UNIT::NAME::MARKSMAN:
//             name = "Marksman";
//             break;
//         case UNIT::NAME::MERCENARY:
//             name = "Mercenary";
//             break;
//         case UNIT::NAME::HERO:
//             name = "Hero";
//             break;
//         case UNIT::NAME::CORSAIR:
//             name = "Corsair";
//             break;
//         case UNIT::NAME::VIKING:
//             name = "Viking";
//             break;
//         case UNIT::NAME::RAVAGER:
//             name = "Ravager";
//             break;
//         case UNIT::NAME::CAVALIER:
//             name = "Cavalier";
//             break;
//         case UNIT::NAME::PALADIN:
//             name = "Paladin";
//             break;
//         case UNIT::NAME::FENCER:
//             name = "Fencer";
//             break;
//         case UNIT::NAME::MOUSQUETAIRE:
//             name = "Mousquetaire";
//             break;
//     }
//     return(name);
// }

std::vector<unsigned char> mvtTypes;
extern void makemvtTypes() {
    mvtTypes.push_back(0);
    mvtTypes.push_back(UNIT::MVT::FOOT_SLOW); // LORD
    mvtTypes.push_back(UNIT::MVT::RIDERS_SLOW); // LORD_RIDER
    mvtTypes.push_back(UNIT::MVT::FOOT_SLOW); // DUKE
    mvtTypes.push_back(UNIT::MVT::RIDERS_SLOW); // DUKE_RIDER
    mvtTypes.push_back(UNIT::MVT::FOOT_SLOW); // ARCHER
    mvtTypes.push_back(UNIT::MVT::RIDERS_SLOW); // ARCHER_RIDER
    mvtTypes.push_back(UNIT::MVT::FOOT_SLOW); // MARKSMAN
    mvtTypes.push_back(UNIT::MVT::RIDERS_SLOW); // MARKSMAN_RIDER
    mvtTypes.push_back(UNIT::MVT::FLIERS); // ANGEL
    mvtTypes.push_back(UNIT::MVT::FOOT_FAST); // DEMON
    mvtTypes.push_back(UNIT::MVT::FLIERS); // PEGASUS_KNIGHT
    mvtTypes.push_back(UNIT::MVT::MAGES); // DEMONIC_INCARNATE
    mvtTypes.push_back(UNIT::MVT::MAGES); // ANGELIC_INCARNATE
    mvtTypes.push_back(UNIT::MVT::FOOT_SLOW); // FENCER
    mvtTypes.push_back(UNIT::MVT::FOOT_SLOW); // DUELIST
    mvtTypes.push_back(UNIT::MVT::FOOT_SLOW); // MOUSQUETAIRE
    mvtTypes.push_back(UNIT::MVT::FOOT_FAST); // PICKPOCKET
    mvtTypes.push_back(UNIT::MVT::FOOT_FAST); // THIEF
    mvtTypes.push_back(UNIT::MVT::FOOT_SLOW); // ASSASSIN
    mvtTypes.push_back(UNIT::MVT::FOOT_SLOW); // MERCENARY
    mvtTypes.push_back(UNIT::MVT::FOOT_SLOW); // HERO
    mvtTypes.push_back(UNIT::MVT::PIRATES); // CORSAIR
    mvtTypes.push_back(UNIT::MVT::PIRATES); // VIKING
    mvtTypes.push_back(UNIT::MVT::BANDITS); // BANDIT
    mvtTypes.push_back(UNIT::MVT::BANDITS); // RAVAGER
    mvtTypes.push_back(UNIT::MVT::FOOT_SLOW); // PIKEMAN
    mvtTypes.push_back(UNIT::MVT::RIDERS_SLOW); // CAVALIER
    mvtTypes.push_back(UNIT::MVT::RIDERS_FAST); // PALADIN
    mvtTypes.push_back(UNIT::MVT::MAGES); // MAGE
    mvtTypes.push_back(UNIT::MVT::MAGES); // BATTLEMAGE
    mvtTypes.push_back(UNIT::MVT::RIDERS_SLOW); // TROUBADOUR
    mvtTypes.push_back(UNIT::MVT::MAGES); // SAGE
    mvtTypes.push_back(UNIT::MVT::MAGES); // PRIEST
    mvtTypes.push_back(UNIT::MVT::MAGES); // BISHOP
    mvtTypes.push_back(UNIT::MVT::MAGES); // ORACLE
    mvtTypes.push_back(UNIT::MVT::FOOT_SLOW); // GENERAL
    mvtTypes.push_back(UNIT::MVT::MAGES); // CLERIC
    mvtTypes.push_back(UNIT::MVT::FOOT_FAST); // DEMIGOD
    mvtTypes.push_back(UNIT::MVT::FOOT_FAST); // GOD
    mvtTypes.push_back(UNIT::MVT::FOOT_SLOW); // KNIGHT
    mvtTypes.push_back(UNIT::MVT::FOOT_FAST); // GODDESS
    mvtTypes.push_back(UNIT::MVT::FOOT_FAST); // TWINBORN

}


unsigned char mvtType(unsigned char in_class_index) {
    unsigned char mvt_type;
    switch(in_class_index) {
        case UNIT::CLASS::MERCENARY:
            mvt_type = UNIT::MVT::FOOT_SLOW;
            break;
        case UNIT::CLASS::LORD:
            mvt_type = UNIT::MVT::FOOT_SLOW;
            break;
        case UNIT::CLASS::DUELIST:
            mvt_type = UNIT::MVT::FOOT_SLOW;
            break;
        case UNIT::CLASS::THIEF:
            mvt_type = UNIT::MVT::FOOT_FAST;
            break;
        case UNIT::CLASS::PEGASUS_KNIGHT:
            mvt_type = UNIT::MVT::FLIERS;
            break;
        case UNIT::CLASS::PIKEMAN:
            mvt_type = UNIT::MVT::FOOT_SLOW;
            break;
        case UNIT::CLASS::CAVALIER:
            mvt_type = UNIT::MVT::RIDERS_SLOW;
            break;
        case UNIT::CLASS::KNIGHT:
            mvt_type = UNIT::MVT::ARMORS;
            break;
        case UNIT::CLASS::BANDIT:
            mvt_type = UNIT::MVT::BANDITS;
            break;
        case UNIT::CLASS::CORSAIR:
            mvt_type = UNIT::MVT::PIRATES;
            break;
        case UNIT::CLASS::VIKING:
            mvt_type = UNIT::MVT::PIRATES;
            break;
        case UNIT::CLASS::PICKPOCKET:
            mvt_type = UNIT::MVT::FOOT_FAST;
            break;
        case UNIT::CLASS::FENCER:
            mvt_type = UNIT::MVT::FOOT_SLOW;
            break;
        case UNIT::CLASS::MOUSQUETAIRE:
            mvt_type = UNIT::MVT::FOOT_SLOW;
            break;
        case UNIT::CLASS::ASSASSIN:
            mvt_type = UNIT::MVT::FOOT_FAST;
            break;
        case UNIT::CLASS::MARKSMAN:
            mvt_type = UNIT::MVT::FOOT_SLOW;
            break;        
        case UNIT::CLASS::MARKSMAN_RIDER:
            mvt_type = UNIT::MVT::RIDERS_SLOW;
            break;
        case UNIT::CLASS::ARCHER:
            mvt_type = UNIT::MVT::FOOT_SLOW;
            break;        
        case UNIT::CLASS::ARCHER_RIDER:
            mvt_type = UNIT::MVT::RIDERS_SLOW;
            break;
        case UNIT::CLASS::DUKE:
            mvt_type = UNIT::MVT::FOOT_SLOW;
            break;
        case UNIT::CLASS::PALADIN:
            mvt_type = UNIT::MVT::RIDERS_FAST;
            break;
        case UNIT::CLASS::GENERAL:
            mvt_type = UNIT::MVT::ARMORS;
            break;
        case UNIT::CLASS::CLERIC:
            mvt_type = UNIT::MVT::MAGES;
            break;
        case UNIT::CLASS::PRIEST:
            mvt_type = UNIT::MVT::MAGES;
            break;
        case UNIT::CLASS::MAGE:
            mvt_type = UNIT::MVT::MAGES;
            break;
        case UNIT::CLASS::HERO:
            mvt_type = UNIT::MVT::FOOT_SLOW;
            break;
        case UNIT::CLASS::RAVAGER:
            mvt_type = UNIT::MVT::BANDITS;
            break;
        case UNIT::CLASS::BATTLEMAGE:
            mvt_type = UNIT::MVT::MAGES;
            break;
        case UNIT::CLASS::SAGE:
            mvt_type = UNIT::MVT::MAGES;
            break;
        case UNIT::CLASS::TROUBADOUR:
            mvt_type = UNIT::MVT::RIDERS_FAST;
            break;
        case UNIT::CLASS::ORACLE:
            mvt_type = UNIT::MVT::MAGES;
            break;
        case UNIT::CLASS::BISHOP:
            mvt_type = UNIT::MVT::MAGES;
            break;
        case UNIT::CLASS::ANGEL:
            mvt_type = UNIT::MVT::FLIERS;
            break;
        case UNIT::CLASS::DEMON:
            mvt_type = UNIT::MVT::FOOT_FAST;
            break;
        case UNIT::CLASS::DEMONIC_INCARNATE:
            mvt_type = UNIT::MVT::MAGES;
            break;
        case UNIT::CLASS::ANGELIC_INCARNATE:
            mvt_type = UNIT::MVT::MAGES;
            break;
    }
    return(mvt_type);
}

std::vector<std::string> classNames;
void makeclassNames() {
    classNames.push_back("");
    classNames.push_back("Lord");
    classNames.push_back("Lord Rider");
    classNames.push_back("Duke");
    classNames.push_back("Duke Rider");
    classNames.push_back("Archer");
    classNames.push_back("Archer Rider");
    classNames.push_back("Marksman");
    classNames.push_back("Marksman Rider");
    classNames.push_back("Angel");
    classNames.push_back("Demon");
    classNames.push_back("Pegasus Knight");
    classNames.push_back("Demonic Incarnate");
    classNames.push_back("Angelic Incarnate");
    classNames.push_back("Fencer");
    classNames.push_back("Duelist");
    classNames.push_back("Mousquetaire");
    classNames.push_back("Pickpocket");
    classNames.push_back("Thief");
    classNames.push_back("Assassin");
    classNames.push_back("Mercenary");
    classNames.push_back("Hero");
    classNames.push_back("Corsair");
    classNames.push_back("Viking");
    classNames.push_back("Bandit");
    classNames.push_back("Ravager");
    classNames.push_back("Pikeman");
    classNames.push_back("Cavalier");
    classNames.push_back("Paladin");
    classNames.push_back("Mage");
    classNames.push_back("Battlemage");
    classNames.push_back("Troubadour");
    classNames.push_back("Sage");
    classNames.push_back("Priest");
    classNames.push_back("Bishop");
    classNames.push_back("Oracle");
    classNames.push_back("General");
    classNames.push_back("Cleric");
    classNames.push_back("Demigod");
    classNames.push_back("God");
    classNames.push_back("Knight");
    classNames.push_back("Goddess");
    classNames.push_back("Twinborn");
}

std::string className(unsigned char in_class_index) {
    std::string class_name;
    switch(in_class_index) {
        case UNIT::CLASS::MERCENARY:
            class_name = "Mercenary";
            break;
        case UNIT::CLASS::LORD:
            class_name = "Lord";
            break;        
        case UNIT::CLASS::LORD_RIDER:
            class_name = "Lord rider";
            break;
        case UNIT::CLASS::DUELIST:
            class_name = "Duelist";
            break;
        case UNIT::CLASS::THIEF:
            class_name = "Thief";
            break;
        case UNIT::CLASS::PEGASUS_KNIGHT:
            class_name = "Pegasus knight";
            break;
        case UNIT::CLASS::PIKEMAN:
            class_name = "Pikeman";
            break;
        case UNIT::CLASS::CAVALIER:
            class_name = "Cavalier";
            break;
        case UNIT::CLASS::KNIGHT:
            class_name = "Knight";
            break;
        case UNIT::CLASS::BANDIT:
            class_name = "Bandit";
            break;
        case UNIT::CLASS::CORSAIR:
            class_name = "Corsair";
            break;
        case UNIT::CLASS::VIKING:
            class_name = "Viking";
            break;
        case UNIT::CLASS::PICKPOCKET:
            class_name = "Pickpocket";
            break;
        case UNIT::CLASS::FENCER:
            class_name = "Fencer";
            break;
        case UNIT::CLASS::MOUSQUETAIRE:
            class_name = "Mousquetaire";
            break;
        case UNIT::CLASS::ASSASSIN:
            class_name = "Assassin";
            break;
        case UNIT::CLASS::MARKSMAN:
            class_name = "Marksman";
            break;        
        case UNIT::CLASS::MARKSMAN_RIDER:
            class_name = "Marksman rider";
            break;
        case UNIT::CLASS::ARCHER:
            class_name = "Archer";
            break;        
        case UNIT::CLASS::ARCHER_RIDER:
            class_name = "Archer rider";
            break;
        case UNIT::CLASS::DUKE:
            class_name = "Duke";
            break;
        case UNIT::CLASS::DUKE_RIDER:
            class_name = "Duke rider";
            break;
        case UNIT::CLASS::PALADIN:
            class_name = "Paladin";
            break;
        case UNIT::CLASS::GENERAL:
            class_name = "General";
            break;
        case UNIT::CLASS::CLERIC:
            class_name = "Cleric";
            break;
        case UNIT::CLASS::PRIEST:
            class_name = "Priest";
            break;
        case UNIT::CLASS::MAGE:
            class_name = "Mage";
            break;
        case UNIT::CLASS::HERO:
            class_name = "Hero";
            break;
        case UNIT::CLASS::RAVAGER:
            class_name = "Ravager";
            break;
        case UNIT::CLASS::BATTLEMAGE:
            class_name = "Battlemage";
            break;
        case UNIT::CLASS::SAGE:
            class_name = "Sage";
            break;
        case UNIT::CLASS::TROUBADOUR:
            class_name = "Troubadour";
            break;
        case UNIT::CLASS::ORACLE:
            class_name = "Oracle";
            break;
        case UNIT::CLASS::BISHOP:
            class_name = "Bishop";
            break;
        case UNIT::CLASS::ANGEL:
            class_name = "Angel";
            break;
        case UNIT::CLASS::DEMON:
            class_name = "Demon";
            break;
        case UNIT::CLASS::DEMONIC_INCARNATE:
            class_name = "Demonic Incarnate";
            break;
        case UNIT::CLASS::ANGELIC_INCARNATE:
            class_name = "Angelic Incarnate";
            break;
    }
    return(class_name);
}


std::vector<std::string> skillNames(unsigned long long int in_skillscode) {
    std::vector<std::string> skill_names;
    if ((in_skillscode & UNIT::SKILL::CANTO) > 0) {
        skill_names.push_back("Canto");
    }
    if ((in_skillscode & UNIT::SKILL::SKILLED_RIDER) > 0) {
        skill_names.push_back("Skilled rider");
    }    
    if ((in_skillscode & UNIT::SKILL::SPRINT) > 0) {
        skill_names.push_back("Sprint");
    } 
    if ((in_skillscode & UNIT::SKILL::SWITCH) > 0) {
        skill_names.push_back("Switch");
    }   
    if ((in_skillscode & UNIT::SKILL::MOUNTAINWALK) > 0) {
        skill_names.push_back("Mountainwalk");
    }    
    if ((in_skillscode & UNIT::SKILL::WATERWALK) > 0) {
        skill_names.push_back("Waterwalk");
    }
    if ((in_skillscode & UNIT::SKILL::CRIT_KILLS) > 0) {
        skill_names.push_back("CritKill");
    }
    if ((in_skillscode & UNIT::SKILL::DISMEMBER) > 0) {
        skill_names.push_back("Dismember");
    }
    if ((in_skillscode & UNIT::SKILL::ATK_RANGE_P1) > 0) {
        skill_names.push_back("Range+1");
    }
    if ((in_skillscode & UNIT::SKILL::DIVINE_SHIELD) > 0) {
        skill_names.push_back("Divine Shield");
    }
    if ((in_skillscode & UNIT::SKILL::NO_CRIT) > 0) {
        skill_names.push_back("No crit");
    }
    if ((in_skillscode & UNIT::SKILL::NO_COUNTER) > 0) {
        skill_names.push_back("No counter");
    }
    if ((in_skillscode & UNIT::SKILL::MAX_DESPAIR) > 0) {
        skill_names.push_back("Maxima of Despair");
    }
    if ((in_skillscode & UNIT::SKILL::TUNNELING) > 0) {
        skill_names.push_back("Tunneling");
    }
    if ((in_skillscode & UNIT::SKILL::SCOUTING) > 0) {
        skill_names.push_back("Scouting");
    }
    if ((in_skillscode & UNIT::SKILL::ASSASSINATE) > 0) {
        skill_names.push_back("Assassinate");
    }
    if ((in_skillscode & UNIT::SKILL::LOCKPICK) > 0) {
        skill_names.push_back("Lockpick");
    }
    if ((in_skillscode & UNIT::SKILL::NO_LOCKPICK) > 0) {
        skill_names.push_back("Pick");
    }
    if ((in_skillscode & UNIT::SKILL::IMMUNE_MAGIC) > 0) {
        skill_names.push_back("Immune to magic");
    }
    if ((in_skillscode & UNIT::SKILL::IMMUNE_ELEMENTAL) > 0) {
        skill_names.push_back("Immune to elemental");
    }
    if ((in_skillscode & UNIT::SKILL::IMMUNE_DEMONIC) > 0) {
        skill_names.push_back("Immune to demonic");
    }
    if ((in_skillscode & UNIT::SKILL::IMMUNE_ANGELIC) > 0) {
        skill_names.push_back("Immune to angelic");
    }
    if ((in_skillscode & UNIT::SKILL::LIFESTEAL_RN) > 0) {
        skill_names.push_back("Sol");
    }
    if ((in_skillscode & UNIT::SKILL::INFUSE) > 0) {
        skill_names.push_back("Infuse");
    }
    if ((in_skillscode & UNIT::SKILL::DOUBLE_EXP) > 0) {
        skill_names.push_back("Double EXP");
    }
    if ((in_skillscode & UNIT::SKILL::AMBIDEXTRY) > 0) {
        skill_names.push_back("Ambidextry");
    }
    if ((in_skillscode & UNIT::SKILL::TWO_HAND_STYLE) > 0) {
        skill_names.push_back("Two-hand Style");
    }
    if ((in_skillscode & UNIT::SKILL::PIERCE_RN) > 0) {
        skill_names.push_back("Luna");
    }
    if ((in_skillscode & UNIT::SKILL::COUNTER) > 0) {
        skill_names.push_back("Counter");
    }
    if ((in_skillscode & UNIT::SKILL::THRUST_SWORD_BONUS) > 0) {
        skill_names.push_back("Thrust sword bonus");
    }
    if ((in_skillscode & UNIT::SKILL::SHIELD_BONUS) > 0) {
        skill_names.push_back("Shield bonus");
    }
    if ((in_skillscode & UNIT::SKILL::OFFHAND_BONUS) > 0) {
        skill_names.push_back("Offhand bonus");
    }
    if ((in_skillscode & UNIT::SKILL::BOW_BONUS) > 0) {
        skill_names.push_back("Bow bonus");
    }
    return(skill_names);
}

short unsigned int makeEquippable(unsigned char in_class_index) {
    short unsigned int equippable;
    switch(in_class_index) {
        case UNIT::CLASS::MERCENARY:
            equippable = ITEM::TYPE::SHIELD + ITEM::TYPE::SWORD + ITEM::TYPE::OFFHAND;
            break;
        case UNIT::CLASS::LORD:
            equippable = ITEM::TYPE::LANCE + ITEM::TYPE::SHIELD;
            break;
        case UNIT::CLASS::DUELIST:
            equippable = ITEM::TYPE::LANCE + ITEM::TYPE::SHIELD;
            break;
        case UNIT::CLASS::THIEF:
            equippable = ITEM::TYPE::LANCE + ITEM::TYPE::SHIELD;
            break;
        case UNIT::CLASS::PEGASUS_KNIGHT:
            equippable = ITEM::TYPE::LANCE + ITEM::TYPE::SHIELD;
            break;
        case UNIT::CLASS::PIKEMAN:
            equippable = ITEM::TYPE::LANCE + ITEM::TYPE::SHIELD;
            break;
        case UNIT::CLASS::CAVALIER:
            equippable = ITEM::TYPE::LANCE + ITEM::TYPE::SHIELD;
            break;
        case UNIT::CLASS::KNIGHT:
            equippable = ITEM::TYPE::LANCE + ITEM::TYPE::SHIELD;
            break;
        case UNIT::CLASS::BANDIT:
            equippable = ITEM::TYPE::SHIELD + ITEM::TYPE::AXE;
            break;
        case UNIT::CLASS::CORSAIR:
            equippable = ITEM::TYPE::SHIELD + ITEM::TYPE::OFFHAND + ITEM::TYPE::AXE;
            break;
        case UNIT::CLASS::VIKING:
            equippable = ITEM::TYPE::SHIELD + ITEM::TYPE::OFFHAND + ITEM::TYPE::AXE;
            break;
        case UNIT::CLASS::PICKPOCKET:
            equippable = ITEM::TYPE::OFFHAND + ITEM::TYPE::SWORD;
            break;
        case UNIT::CLASS::FENCER:
            equippable = ITEM::TYPE::OFFHAND + ITEM::TYPE::SWORD;
            break;
        case UNIT::CLASS::MOUSQUETAIRE:
            equippable = ITEM::TYPE::SWORD + ITEM::TYPE::OFFHAND + ITEM::TYPE::BOW; 
            break;
        case UNIT::CLASS::ASSASSIN:
            equippable = ITEM::TYPE::SWORD + ITEM::TYPE::OFFHAND + ITEM::TYPE::BOW; 
            break;
        case UNIT::CLASS::MARKSMAN:
            equippable = ITEM::TYPE::SWORD + ITEM::TYPE::OFFHAND + ITEM::TYPE::BOW; 
            break;
        case UNIT::CLASS::ARCHER:
            equippable = ITEM::TYPE::BOW;
            break;
        case UNIT::CLASS::DUKE:
            equippable = ITEM::TYPE::SWORD + ITEM::TYPE::LANCE + ITEM::TYPE::SHIELD + ITEM::TYPE::OFFHAND;
            break;
        case UNIT::CLASS::PALADIN:
            equippable = ITEM::TYPE::SWORD + ITEM::TYPE::LANCE + ITEM::TYPE::SHIELD + ITEM::TYPE::OFFHAND;
            break;
        case UNIT::CLASS::GENERAL:
            equippable = ITEM::TYPE::SWORD + ITEM::TYPE::SHIELD + ITEM::TYPE::AXE + ITEM::TYPE::LANCE;
            break;
        case UNIT::CLASS::CLERIC:
            equippable = ITEM::TYPE::STAFF;
            break;
        case UNIT::CLASS::PRIEST:
            equippable = ITEM::TYPE::STAFF;
            break;
        case UNIT::CLASS::MAGE:
            equippable = ITEM::TYPE::ELEMENTAL + ITEM::TYPE::TRINKET;
            break;
        case UNIT::CLASS::HERO:
            equippable = ITEM::TYPE::AXE + ITEM::TYPE::SWORD + ITEM::TYPE::SHIELD + ITEM::TYPE::OFFHAND;
            break;
        case UNIT::CLASS::RAVAGER:
            equippable = ITEM::TYPE::AXE + ITEM::TYPE::SHIELD + ITEM::TYPE::OFFHAND + ITEM::TYPE::BOW;
            break;
        case UNIT::CLASS::BATTLEMAGE:
            equippable = ITEM::TYPE::ELEMENTAL + ITEM::TYPE::SHIELD + ITEM::TYPE::SWORD + ITEM::TYPE::TRINKET;
            break;
        case UNIT::CLASS::SAGE:
            equippable = ITEM::TYPE::ELEMENTAL + ITEM::TYPE::STAFF + ITEM::TYPE::TRINKET;
            break;
        case UNIT::CLASS::TROUBADOUR:
            equippable = ITEM::TYPE::ELEMENTAL + ITEM::TYPE::STAFF + ITEM::TYPE::TRINKET;
            break;
        case UNIT::CLASS::ORACLE:
            equippable = ITEM::TYPE::STAFF + ITEM::TYPE::ANGELIC + ITEM::TYPE::TRINKET;
            break;
        case UNIT::CLASS::BISHOP:
            equippable = ITEM::TYPE::STAFF + ITEM::TYPE::ANGELIC + ITEM::TYPE::TRINKET;
            break;
        case UNIT::CLASS::ANGEL:
            equippable = ITEM::TYPE::ANGELIC + ITEM::TYPE::SWORD + ITEM::TYPE::LANCE + ITEM::TYPE::SHIELD;
            break;
        case UNIT::CLASS::DEMON:
            equippable = ITEM::TYPE::DEMONIC + ITEM::TYPE::CLAW + ITEM::TYPE::AXE;
            break;
        case UNIT::CLASS::DEMONIC_INCARNATE:
            equippable = ITEM::TYPE::DEMONIC + ITEM::TYPE::TRINKET;
            break;
        case UNIT::CLASS::ANGELIC_INCARNATE:
            equippable = ITEM::TYPE::ANGELIC + ITEM::TYPE::TRINKET;
            break;
    }
    return(equippable);
}