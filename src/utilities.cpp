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
    // Retunrs geometrically decreasing index.
    // distance/geo_factor + distance/geo_factor**2 + distance/geo_factor**3
    int sign = sgn(distance); 
    int out = sign * std::max(sign * (int)(distance / geo_factor), 1);
    return (out);
    // std::abs() possible instead of sign*distance,
    // but this is more elegant.
}

void sexName(char * name, bool in_sex) {
    if(in_sex) {
        strcpy(name, "M");
    } else {
        strcpy(name, "F");
    }
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