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

std::string sexName(bool in_sex) {
    std::string name;
    if(in_sex) {
        name = "M";
    } else {
        name = "F";
    }
    return(name);
}

std::string unitName(unsigned short int in_id) {
    std::string name;
    switch(in_id) {
        case UNIT::NAME::ERWIN:
            name = "Erwin";
            break;
        case UNIT::NAME::KIARA:
            name = "Kiara";
            break;
        case UNIT::NAME::SILOU:
            name = "SILOU";
            break;
        case UNIT::NAME::SERVIL:
            name = "Servil";
            break;
        case UNIT::NAME::PERIGNON:
            name = "Perignon";
            break;
        case UNIT::NAME::COWARD:
            name = "Coward";
            break;
        case UNIT::NAME::POET:
            name = "Poet";
            break;
        case UNIT::NAME::RELIABLE:
            name = "Reliable";
            break;
        case UNIT::NAME::JAIGEN1H:
            name = "Jaigen1H";
            break;
        case UNIT::NAME::HOTTIE:
            name = "Hottie";
            break;
        case UNIT::NAME::BANDIT:
            name = "Bandit";
            break;
        case UNIT::NAME::DUELIST:
            name = "Duelist";
            break;
        case UNIT::NAME::PICKPOCKET:
            name = "Pickpocket";
            break;
        case UNIT::NAME::THIEF:
            name = "Thief";
            break;
        case UNIT::NAME::ASSASSIN:
            name = "Assassing";
            break;
        case UNIT::NAME::ARCHER:
            name = "Archer";
            break;
        case UNIT::NAME::MARKSMAN:
            name = "Marksman";
            break;
        case UNIT::NAME::MERCENARY:
            name = "Mercenary";
            break;
        case UNIT::NAME::HERO:
            name = "Hero";
            break;
        case UNIT::NAME::CORSAIR:
            name = "Corsair";
            break;
        case UNIT::NAME::VIKING:
            name = "Viking";
            break;
        case UNIT::NAME::RAVAGER:
            name = "Ravager";
            break;
        case UNIT::NAME::CAVALIER:
            name = "Cavalier";
            break;
        case UNIT::NAME::PALADIN:
            name = "Paladin";
            break;
        case UNIT::NAME::FENCER:
            name = "Fencer";
            break;
        case UNIT::NAME::MOUSQUETAIRE:
            name = "Mousquetaire";
            break;
    }
    return(name);
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


std::vector<std::string> skillNames(unsigned long long int in_skills) {
    std::vector<std::string> skill_names;
    if ((in_skills & UNIT::SKILL::CANTO) > 0) {
        skill_names.push_back("Canto");
    }
    if ((in_skills & UNIT::SKILL::SKILLED_RIDER) > 0) {
        skill_names.push_back("Skilled rider");
    }    
    if ((in_skills & UNIT::SKILL::SPRINT) > 0) {
        skill_names.push_back("Sprint");
    } 
    if ((in_skills & UNIT::SKILL::SWITCH) > 0) {
        skill_names.push_back("Switch");
    }   
    if ((in_skills & UNIT::SKILL::MOUNTAINWALK) > 0) {
        skill_names.push_back("Mountainwalk");
    }    
    if ((in_skills & UNIT::SKILL::WATERWALK) > 0) {
        skill_names.push_back("Waterwalk");
    }
    if ((in_skills & UNIT::SKILL::CRIT_KILLS) > 0) {
        skill_names.push_back("CritKill");
    }
    if ((in_skills & UNIT::SKILL::DISMEMBER) > 0) {
        skill_names.push_back("Dismember");
    }
    if ((in_skills & UNIT::SKILL::ATK_RANGE_P1) > 0) {
        skill_names.push_back("Range+1");
    }
    if ((in_skills & UNIT::SKILL::DIVINE_SHIELD) > 0) {
        skill_names.push_back("Divine Shield");
    }
    if ((in_skills & UNIT::SKILL::NO_CRIT) > 0) {
        skill_names.push_back("No crit");
    }
    if ((in_skills & UNIT::SKILL::NO_COUNTER) > 0) {
        skill_names.push_back("No counter");
    }
    if ((in_skills & UNIT::SKILL::MAX_DESPAIR) > 0) {
        skill_names.push_back("Maxima of Despair");
    }
    if ((in_skills & UNIT::SKILL::TUNNELING) > 0) {
        skill_names.push_back("Tunneling");
    }
    if ((in_skills & UNIT::SKILL::SCOUTING) > 0) {
        skill_names.push_back("Scouting");
    }
    if ((in_skills & UNIT::SKILL::ASSASSINATE) > 0) {
        skill_names.push_back("Assassinate");
    }
    if ((in_skills & UNIT::SKILL::LOCKPICK) > 0) {
        skill_names.push_back("Lockpick");
    }
    if ((in_skills & UNIT::SKILL::NO_LOCKPICK) > 0) {
        skill_names.push_back("Pick");
    }
    if ((in_skills & UNIT::SKILL::IMMUNE_MAGIC) > 0) {
        skill_names.push_back("Immune to magic");
    }
    if ((in_skills & UNIT::SKILL::IMMUNE_ELEMENTAL) > 0) {
        skill_names.push_back("Immune to elemental");
    }
    if ((in_skills & UNIT::SKILL::IMMUNE_DEMONIC) > 0) {
        skill_names.push_back("Immune to demonic");
    }
    if ((in_skills & UNIT::SKILL::IMMUNE_ANGELIC) > 0) {
        skill_names.push_back("Immune to angelic");
    }
    if ((in_skills & UNIT::SKILL::LIFESTEAL_RN) > 0) {
        skill_names.push_back("Sol");
    }
    if ((in_skills & UNIT::SKILL::INFUSE) > 0) {
        skill_names.push_back("Infuse");
    }
    if ((in_skills & UNIT::SKILL::DOUBLE_EXP) > 0) {
        skill_names.push_back("Double EXP");
    }
    if ((in_skills & UNIT::SKILL::AMBIDEXTRY) > 0) {
        skill_names.push_back("Ambidextry");
    }
    if ((in_skills & UNIT::SKILL::TWO_HAND_STYLE) > 0) {
        skill_names.push_back("Two-hand Style");
    }
    if ((in_skills & UNIT::SKILL::PIERCE_RN) > 0) {
        skill_names.push_back("Luna");
    }
    if ((in_skills & UNIT::SKILL::COUNTER) > 0) {
        skill_names.push_back("Counter");
    }
    if ((in_skills & UNIT::SKILL::THRUST_SWORD_BONUS) > 0) {
        skill_names.push_back("Thrust sword bonus");
    }
    if ((in_skills & UNIT::SKILL::SHIELD_BONUS) > 0) {
        skill_names.push_back("Shield bonus");
    }
    if ((in_skills & UNIT::SKILL::OFFHAND_BONUS) > 0) {
        skill_names.push_back("Offhand bonus");
    }
    if ((in_skills & UNIT::SKILL::BOW_BONUS) > 0) {
        skill_names.push_back("Bow bonus");
    }
    return(skill_names);
}

short unsigned int makeEquippable(unsigned char in_class_index) {
    short unsigned int equippable;
    switch(in_class_index) {
        case UNIT::CLASS::MERCENARY:
            equippable = WPN::TYPE::SHIELD + WPN::TYPE::SWORD + WPN::TYPE::OFFHAND;
            break;
        case UNIT::CLASS::LORD:
            equippable = WPN::TYPE::LANCE + WPN::TYPE::SHIELD;
            break;
        case UNIT::CLASS::DUELIST:
            equippable = WPN::TYPE::LANCE + WPN::TYPE::SHIELD;
            break;
        case UNIT::CLASS::THIEF:
            equippable = WPN::TYPE::LANCE + WPN::TYPE::SHIELD;
            break;
        case UNIT::CLASS::PEGASUS_KNIGHT:
            equippable = WPN::TYPE::LANCE + WPN::TYPE::SHIELD;
            break;
        case UNIT::CLASS::PIKEMAN:
            equippable = WPN::TYPE::LANCE + WPN::TYPE::SHIELD;
            break;
        case UNIT::CLASS::CAVALIER:
            equippable = WPN::TYPE::LANCE + WPN::TYPE::SHIELD;
            break;
        case UNIT::CLASS::KNIGHT:
            equippable = WPN::TYPE::LANCE + WPN::TYPE::SHIELD;
            break;
        case UNIT::CLASS::BANDIT:
            equippable = WPN::TYPE::SHIELD + WPN::TYPE::AXE;
            break;
        case UNIT::CLASS::CORSAIR:
            equippable = WPN::TYPE::SHIELD + WPN::TYPE::OFFHAND + WPN::TYPE::AXE;
            break;
        case UNIT::CLASS::VIKING:
            equippable = WPN::TYPE::SHIELD + WPN::TYPE::OFFHAND + WPN::TYPE::AXE;
            break;
        case UNIT::CLASS::PICKPOCKET:
            equippable = WPN::TYPE::OFFHAND + WPN::TYPE::SWORD;
            break;
        case UNIT::CLASS::FENCER:
            equippable = WPN::TYPE::OFFHAND + WPN::TYPE::SWORD;
            break;
        case UNIT::CLASS::MOUSQUETAIRE:
            equippable = WPN::TYPE::SWORD + WPN::TYPE::OFFHAND + WPN::TYPE::BOW; 
            break;
        case UNIT::CLASS::ASSASSIN:
            equippable = WPN::TYPE::SWORD + WPN::TYPE::OFFHAND + WPN::TYPE::BOW; 
            break;
        case UNIT::CLASS::MARKSMAN:
            equippable = WPN::TYPE::SWORD + WPN::TYPE::OFFHAND + WPN::TYPE::BOW; 
            break;
        case UNIT::CLASS::ARCHER:
            equippable = WPN::TYPE::BOW;
            break;
        case UNIT::CLASS::DUKE:
            equippable = WPN::TYPE::SWORD + WPN::TYPE::LANCE + WPN::TYPE::SHIELD + WPN::TYPE::OFFHAND;
            break;
        case UNIT::CLASS::PALADIN:
            equippable = WPN::TYPE::SWORD + WPN::TYPE::LANCE + WPN::TYPE::SHIELD + WPN::TYPE::OFFHAND;
            break;
        case UNIT::CLASS::GENERAL:
            equippable = WPN::TYPE::SWORD + WPN::TYPE::SHIELD + WPN::TYPE::AXE + WPN::TYPE::LANCE;
            break;
        case UNIT::CLASS::CLERIC:
            equippable = WPN::TYPE::STAFF;
            break;
        case UNIT::CLASS::PRIEST:
            equippable = WPN::TYPE::STAFF;
            break;
        case UNIT::CLASS::MAGE:
            equippable = WPN::TYPE::ELEMENTAL + WPN::TYPE::TRINKET;
            break;
        case UNIT::CLASS::HERO:
            equippable = WPN::TYPE::AXE + WPN::TYPE::SWORD + WPN::TYPE::SHIELD + WPN::TYPE::OFFHAND;
            break;
        case UNIT::CLASS::RAVAGER:
            equippable = WPN::TYPE::AXE + WPN::TYPE::SHIELD + WPN::TYPE::OFFHAND + WPN::TYPE::BOW;
            break;
        case UNIT::CLASS::BATTLEMAGE:
            equippable = WPN::TYPE::ELEMENTAL + WPN::TYPE::SHIELD + WPN::TYPE::SWORD + WPN::TYPE::TRINKET;
            break;
        case UNIT::CLASS::SAGE:
            equippable = WPN::TYPE::ELEMENTAL + WPN::TYPE::STAFF + WPN::TYPE::TRINKET;
            break;
        case UNIT::CLASS::TROUBADOUR:
            equippable = WPN::TYPE::ELEMENTAL + WPN::TYPE::STAFF + WPN::TYPE::TRINKET;
            break;
        case UNIT::CLASS::ORACLE:
            equippable = WPN::TYPE::STAFF + WPN::TYPE::ANGELIC + WPN::TYPE::TRINKET;
            break;
        case UNIT::CLASS::BISHOP:
            equippable = WPN::TYPE::STAFF + WPN::TYPE::ANGELIC + WPN::TYPE::TRINKET;
            break;
        case UNIT::CLASS::ANGEL:
            equippable = WPN::TYPE::ANGELIC + WPN::TYPE::SWORD + WPN::TYPE::LANCE + WPN::TYPE::SHIELD;
            break;
        case UNIT::CLASS::DEMON:
            equippable = WPN::TYPE::DEMONIC + WPN::TYPE::CLAW + WPN::TYPE::AXE;
            break;
        case UNIT::CLASS::DEMONIC_INCARNATE:
            equippable = WPN::TYPE::DEMONIC + WPN::TYPE::TRINKET;
            break;
        case UNIT::CLASS::ANGELIC_INCARNATE:
            equippable = WPN::TYPE::ANGELIC + WPN::TYPE::TRINKET;
            break;
    }
    return(equippable);
}