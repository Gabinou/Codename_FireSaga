#include "utilities.hpp"
#include "game.hpp"

int pingpong(int current, int upper, int lower) {
    // Returns "pingpong" index. upper is excluded.
    // modulo: x % 2 = 0,1,2,0,1,2,0...for x++
    // pingpong(x, 2, 0) = 0,1,2,1,0,1,2... for x++
    int mod_factor = (2 * (upper - lower) - 2);
    int term1 = mod_factor - (current % mod_factor);
    int term2 = current % mod_factor;
    return (std::min(term1, term2) + lower);
}

bool fequal(const char * filename1, const char * filename2) {
    bool out = false;
    PHYSFS_file * fp1;
    fp1 = PHYSFS_openRead(filename1);

    if (!fp1) {
        SDL_Log("Failed to open %s.", filename1);
        return (out);
    }

    PHYSFS_file * fp2;
    fp2 = PHYSFS_openRead(filename2);

    if (!fp2) {
        SDL_Log("Failed to open %s.", filename2);
        return (out);
    }

    unsigned int filelen1 = PHYSFS_fileLength(fp1);
    unsigned int filelen2 = PHYSFS_fileLength(fp2);
    char filebuffer1[filelen1];
    char filebuffer2[filelen2];
    PHYSFS_readBytes(fp1, filebuffer1, filelen1);
    PHYSFS_readBytes(fp2, filebuffer2, filelen2);
    PHYSFS_close(fp1);
    PHYSFS_close(fp2);

    if (filelen1 == filelen2) {
        out = true;

        for (int i = 0; i < filelen1; i++) {
            if (filebuffer1[i] != filebuffer2[i]) {
                out = false;
            }
        }
    }

    return (out);
}

std::string stats2str(Unit_stats in_stats) {
    char buffer[DEFAULT::BUFFER_SIZE];
    stbsp_sprintf(buffer, "%02d %02d %02d %02d %02d %02d %02d %02d %02d %02d %02d", in_stats.hp, in_stats.str, in_stats.mag, in_stats.agi, in_stats.dex, in_stats.luck, in_stats.def, in_stats.res, in_stats.con, in_stats.move, in_stats.prof);
    std::string out(buffer);
    return (out);
}

std::string stats2str(Weapon_stats in_stats) {
    char buffer[DEFAULT::BUFFER_SIZE];
    stbsp_sprintf(buffer, "%02d %02d %02d %02d %02d %02d %02d %02d %02d %02d %02d %02d %02d %02d %06d %02d", in_stats.Pmight, in_stats.Mmight, in_stats.combat.hit, in_stats.combat.dodge, in_stats.combat.crit, in_stats.combat.favor, in_stats.wgt, in_stats.uses, in_stats.prof, in_stats.range[0], in_stats.range[1], in_stats.hand[0], in_stats.hand[1], in_stats.dmg_type, in_stats.price, in_stats.heal);
    std::string out(buffer);
    return (out);
}


int geometricslide(int distance, float geo_factor) {
    // Returns geometrically decreasing indices.
    // Ex: distance/geo_factor -> distance/geo_factor**2 -> distance/geo_factor**3
    int sign = sgn(distance);
    int out = sign * std::max(sign * (int)(distance / geo_factor), 1);
    return (out);
    // sign*distance more elegant than std::abs()
}

bool isFriendly(const unsigned char army1, const unsigned char army2) {
    bool out = false;

    if (army1 ==  army2) {
        out = true;
    }

    switch (army1) {
        case UNIT::ARMY::FRIENDLY:
        case UNIT::ARMY::NEUTRAL:
        case UNIT::ARMY::ERWIN:
        case UNIT::ARMY::FREE_MILITIA:
            switch (army2) {
                case UNIT::ARMY::FRIENDLY:
                case UNIT::ARMY::NEUTRAL:
                case UNIT::ARMY::ERWIN:
                case UNIT::ARMY::FREE_MILITIA:
                    out = true;
                    break;

                case UNIT::ARMY::ENEMY:
                case UNIT::ARMY::IMPERIAL:
                case UNIT::ARMY::THEOCRACY:
                case UNIT::ARMY::VOLDAN:
                    out = false;
                    break;
            }

            break;

        case UNIT::ARMY::ENEMY:
        case UNIT::ARMY::IMPERIAL:
        case UNIT::ARMY::THEOCRACY:
        case UNIT::ARMY::VOLDAN:
            switch (army2) {
                case UNIT::ARMY::FRIENDLY:
                case UNIT::ARMY::NEUTRAL:
                case UNIT::ARMY::ERWIN:
                case UNIT::ARMY::FREE_MILITIA:
                    out = false;
                    break;

                case UNIT::ARMY::ENEMY:
                case UNIT::ARMY::IMPERIAL:
                case UNIT::ARMY::THEOCRACY:
                case UNIT::ARMY::VOLDAN:
                    out = true;
                    break;
            }

            break;
    }

    return (out);
}

std::vector<int> cppwhere(int tofind, std::vector<int> vec) {
    std::vector<int> found_inds;

    for (int i = 0; i < vec.size(); i++) {
        if (vec[i] == tofind) {
            found_inds.push_back(i);
        }
    }

    return (found_inds);
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

    return (uniques);
}

std::vector<std::string> unitTypes(unsigned short int in_typecode) {
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

    return (names);
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

    return (names);
}

void loadUtilities() {
    makeunitNames();
    makestatNames();
    makesexNames();
    makeclassNames();
    makemvtTypes();
    makewpnNames();
    makeEquippableCodes();
    makeArmyNames();
}

std::vector<std::string> armyNames;
void makeArmyNames() {
    armyNames.push_back("");
    armyNames.push_back("Friendly");
    armyNames.push_back("Enemy");
    armyNames.push_back("Neutral");
    armyNames.push_back("Imperial");
    armyNames.push_back("Voldan");
    armyNames.push_back("Kewac");
    armyNames.push_back("Theocratic");
    armyNames.push_back("Free Milita");
    armyNames.push_back("Erwin");
    armyNames.push_back("");
}


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

    if ((in_typecode & ITEM::TYPE::CLAW) > 0) {
        types.push_back("Claw");
    }

    if ((in_typecode & ITEM::TYPE::ITEM) > 0) {
        types.push_back("Item");
    }

    if ((in_typecode & ITEM::TYPE::BOOK) > 0) {
        types.push_back("Book");
    }

    return (types);
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

std::vector<std::string> getTilenames(std::vector<short int> in_tilesindex) {
    std::vector<std::string> out;
    short int index;

    for (int i = 0; i < in_tilesindex.size(); i++) {
        index = in_tilesindex[i] / DEFAULT::TILE_DIVISOR;
        SDL_Log("In tile index: %d", index);

        switch (index) {
            case TILE::PLAIN:
                out.push_back("Plain");
                break;

            case TILE::BUSH:
                out.push_back("Bush");
                break;

            case TILE::FOREST:
                out.push_back("Forest");
                break;

            case TILE::THICKET:
                out.push_back("Thicket");
                break;

            case TILE::SNAG:
                out.push_back("Snag");
                break;

            case TILE::BRIDGE:
                out.push_back("Bridge");
                break;

            case TILE::HOLE:
                out.push_back("Hole");
                break;

            case TILE::SEA:
                out.push_back("Sea");
                break;

            case TILE::LAKE:
                out.push_back("Lake");
                break;

            case TILE::RIVER:
                out.push_back("River");
                break;

            case TILE::WATERFALL:
                out.push_back("Waterfall");
                break;

            case TILE::CLIFF:
                out.push_back("Cliff");
                break;

            case TILE::HILL:
                out.push_back("Hill");
                break;

            case TILE::MOUNTAIN:
                out.push_back("Mountain");
                break;

            case TILE::PEAK:
                out.push_back("Peak");
                break;

            case TILE::CAVE:
                out.push_back("Cave");
                break;

            case TILE::SAND:
                out.push_back("Sand");
                break;

            case TILE::WASTELAND:
                out.push_back("Wasteland");
                break;

            case TILE::ROCK:
                out.push_back("Rock");
                break;

            case TILE::BONES:
                out.push_back("Bones");
                break;

            case TILE::FLOOR:
                out.push_back("Floor");
                break;

            case TILE::PILLAR:
                out.push_back("Pillar");
                break;

            case TILE::WALL:
                out.push_back("Wall");
                break;

            case TILE::DOOR:
                out.push_back("Door");
                break;

            case TILE::THRONE:
                out.push_back("Throne");
                break;

            case TILE::GATE:
                out.push_back("Gate");
                break;

            case TILE::FENCE:
                out.push_back("Fence");
                break;

            case TILE::SHOP:
                out.push_back("Shop");
                break;

            case TILE::ARMORY:
                out.push_back("Armory");
                break;

            case TILE::VILLAGE:
                out.push_back("Village");
                break;

            case TILE::FORT:
                out.push_back("Fort");
                break;

            case TILE::CASTLE:
                out.push_back("Castle");
                break;

            case TILE::SNOW:
                out.push_back("Snow");
                break;

            case TILE::GLACIER:
                out.push_back("Glacier");
                break;

            case TILE::ICE:
                out.push_back("Ice");
                break;
        }
    }

    // SDL_Log("Tilenames size out:%d", out.size());
    return (out);
}

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

    switch (in_class_index) {
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

    return (mvt_type);
}

unsigned char unitid2army(short unsigned int in_unitid) {
    // only used for unit init. baseArmy
    unsigned char armyid = 0;

    switch (in_unitid) {
        case UNIT::NAME::ERWIN:
            armyid = UNIT::ARMY::ERWIN;
            break;

        case UNIT::NAME::KIARA:
            armyid = UNIT::ARMY::ERWIN;
            break;

        case UNIT::NAME::SILOU:
            armyid = UNIT::ARMY::NEUTRAL;
            break;

        case UNIT::NAME::SERVIL:
            armyid = UNIT::ARMY::IMPERIAL;
            break;

        case UNIT::NAME::PERIGNON:
            armyid = UNIT::ARMY::ERWIN;
            break;

        case UNIT::NAME::POET:
            armyid = UNIT::ARMY::NEUTRAL;
            break;

        case UNIT::NAME::RELIABLE:
            armyid = UNIT::ARMY::ERWIN;
            break;

        case UNIT::NAME::COWARD:
            armyid = UNIT::ARMY::ERWIN;
            break;

        case UNIT::NAME::JAIGEN1H:
            armyid = UNIT::ARMY::NEUTRAL;
            break;

        case UNIT::NAME::HOTTIE:
            armyid = UNIT::ARMY::ERWIN;
            break;

        case UNIT::NAME::ZINEDAN:
            armyid = UNIT::ARMY::BANDITS;
            break;

        case UNIT::NAME::ZIDINE:
            armyid = UNIT::ARMY::BANDITS;
            break;

        case UNIT::NAME::BANDIT:
            armyid = UNIT::ARMY::BANDITS;
            break;

        case UNIT::NAME::DUELIST:
            armyid = UNIT::ARMY::ENEMY;
            break;

        case UNIT::NAME::PICKPOCKET:
            armyid = UNIT::ARMY::ENEMY;
            break;

        case UNIT::NAME::THIEF:
            armyid = UNIT::ARMY::ENEMY;
            break;

        case UNIT::NAME::ASSASSIN:
            armyid = UNIT::ARMY::ENEMY;
            break;

        case UNIT::NAME::ARCHER:
            armyid = UNIT::ARMY::ENEMY;
            break;

        case UNIT::NAME::MARKSMAN:
            armyid = UNIT::ARMY::ENEMY;
            break;

        case UNIT::NAME::MERCENARY:
            armyid = UNIT::ARMY::ENEMY;
            break;

        case UNIT::NAME::HERO:
            armyid = UNIT::ARMY::ENEMY;
            break;

        case UNIT::NAME::CORSAIR:
            armyid = UNIT::ARMY::ENEMY;
            break;

        case UNIT::NAME::VIKING:
            armyid = UNIT::ARMY::ENEMY;
            break;

        case UNIT::NAME::RAVAGER:
            armyid = UNIT::ARMY::ENEMY;
            break;

        case UNIT::NAME::CAVALIER:
            armyid = UNIT::ARMY::ENEMY;
            break;

        case UNIT::NAME::PALADIN:
            armyid = UNIT::ARMY::ENEMY;
            break;

        case UNIT::NAME::FENCER:
            armyid = UNIT::ARMY::ENEMY;
            break;

        case UNIT::NAME::MOUSQUETAIRE:
            armyid = UNIT::ARMY::ENEMY;
            break;
    }

    return (armyid);
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

    switch (in_class_index) {
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

    return (class_name);
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

    return (skill_names);
}

std::vector<short unsigned int> equippableCodes;
void makeEquippableCodes() {
    equippableCodes.push_back(0);
    equippableCodes.push_back(ITEM::TYPE::LANCE + ITEM::TYPE::SHIELD);
    equippableCodes.push_back(ITEM::TYPE::LANCE + ITEM::TYPE::SHIELD); //LORD_RIDER
    equippableCodes.push_back(ITEM::TYPE::SWORD + ITEM::TYPE::LANCE + ITEM::TYPE::SHIELD + ITEM::TYPE::OFFHAND); //DUKE
    equippableCodes.push_back(ITEM::TYPE::SWORD + ITEM::TYPE::LANCE + ITEM::TYPE::SHIELD + ITEM::TYPE::OFFHAND); //DUKE_RIDER
    equippableCodes.push_back(ITEM::TYPE::BOW); //ARCHER
    equippableCodes.push_back(ITEM::TYPE::BOW); //ARCHER_RIDER
    equippableCodes.push_back(ITEM::TYPE::SWORD + ITEM::TYPE::OFFHAND + ITEM::TYPE::BOW);
    equippableCodes.push_back(ITEM::TYPE::SWORD + ITEM::TYPE::OFFHAND + ITEM::TYPE::BOW);
    equippableCodes.push_back(ITEM::TYPE::ANGELIC + ITEM::TYPE::SWORD + ITEM::TYPE::LANCE + ITEM::TYPE::SHIELD);
    equippableCodes.push_back(ITEM::TYPE::DEMONIC + ITEM::TYPE::CLAW + ITEM::TYPE::AXE);
    equippableCodes.push_back(ITEM::TYPE::LANCE + ITEM::TYPE::SHIELD);
    equippableCodes.push_back(ITEM::TYPE::ANGELIC + ITEM::TYPE::TRINKET); //ANGELIC_INCARNATE
    equippableCodes.push_back(ITEM::TYPE::DEMONIC + ITEM::TYPE::TRINKET); //DEMONIC_INCARNATE
    equippableCodes.push_back(ITEM::TYPE::OFFHAND + ITEM::TYPE::SWORD); //FENCER
    equippableCodes.push_back(ITEM::TYPE::LANCE + ITEM::TYPE::SHIELD); //DUELIST
    equippableCodes.push_back(ITEM::TYPE::SWORD + ITEM::TYPE::OFFHAND + ITEM::TYPE::BOW); //MOUSQUETAIRE
    equippableCodes.push_back(ITEM::TYPE::OFFHAND + ITEM::TYPE::SWORD); //PICKPOCKET
    equippableCodes.push_back(ITEM::TYPE::LANCE + ITEM::TYPE::SHIELD); // THIEF
    equippableCodes.push_back(ITEM::TYPE::SWORD + ITEM::TYPE::OFFHAND + ITEM::TYPE::BOW); //ASSASSIN
    equippableCodes.push_back(ITEM::TYPE::SHIELD + ITEM::TYPE::SWORD + ITEM::TYPE::OFFHAND); //MERCERNARY
    equippableCodes.push_back(ITEM::TYPE::AXE + ITEM::TYPE::SWORD + ITEM::TYPE::SHIELD + ITEM::TYPE::OFFHAND); //HERO
    equippableCodes.push_back(ITEM::TYPE::SHIELD + ITEM::TYPE::OFFHAND + ITEM::TYPE::AXE); // CORSAIR
    equippableCodes.push_back(ITEM::TYPE::SHIELD + ITEM::TYPE::OFFHAND + ITEM::TYPE::AXE); // VIKING
    equippableCodes.push_back(ITEM::TYPE::SHIELD + ITEM::TYPE::AXE); //BANDIT
    equippableCodes.push_back(ITEM::TYPE::AXE + ITEM::TYPE::SHIELD + ITEM::TYPE::OFFHAND + ITEM::TYPE::BOW); // RAVAGER
    equippableCodes.push_back(ITEM::TYPE::LANCE + ITEM::TYPE::SHIELD); // PIKEMAN
    equippableCodes.push_back(ITEM::TYPE::LANCE + ITEM::TYPE::SHIELD); // CAVALIER
    equippableCodes.push_back(ITEM::TYPE::SWORD + ITEM::TYPE::LANCE + ITEM::TYPE::SHIELD + ITEM::TYPE::OFFHAND); //PALADIN
    equippableCodes.push_back(ITEM::TYPE::ELEMENTAL + ITEM::TYPE::TRINKET); // MAGE
    equippableCodes.push_back(ITEM::TYPE::ELEMENTAL + ITEM::TYPE::SHIELD + ITEM::TYPE::SWORD + ITEM::TYPE::TRINKET); // BATTLEMAGE
    equippableCodes.push_back(ITEM::TYPE::ELEMENTAL + ITEM::TYPE::STAFF + ITEM::TYPE::TRINKET); //TROUBADOUR
    equippableCodes.push_back(ITEM::TYPE::ELEMENTAL + ITEM::TYPE::STAFF + ITEM::TYPE::TRINKET); //SAGE
    equippableCodes.push_back(ITEM::TYPE::STAFF); //PRIEST
    equippableCodes.push_back(ITEM::TYPE::STAFF + ITEM::TYPE::ANGELIC + ITEM::TYPE::TRINKET); //BISHOP
    equippableCodes.push_back(ITEM::TYPE::STAFF + ITEM::TYPE::ANGELIC + ITEM::TYPE::TRINKET); //ORACLE
    equippableCodes.push_back(ITEM::TYPE::SWORD + ITEM::TYPE::SHIELD + ITEM::TYPE::AXE + ITEM::TYPE::LANCE); // GENERAL
    equippableCodes.push_back(ITEM::TYPE::STAFF); //CLERIC
    equippableCodes.push_back(ITEM::TYPE::STAFF); //DEMIGOD
    equippableCodes.push_back(ITEM::TYPE::STAFF); //GOD
    equippableCodes.push_back(ITEM::TYPE::LANCE + ITEM::TYPE::SHIELD); //KNIGHT
    equippableCodes.push_back(ITEM::TYPE::LANCE + ITEM::TYPE::SHIELD); //GODDESS
    equippableCodes.push_back(ITEM::TYPE::LANCE + ITEM::TYPE::SHIELD); //TWINBORN
}
