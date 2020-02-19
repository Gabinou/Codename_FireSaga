/*! \page handle weapon.cpp
* \file weapon.cpp
* \brief weapon class implementation.
*/
#include "weapon.hpp"
#include "shared.hpp"

Weapon::Weapon() {

}

Weapon::Weapon(Weapon_stats in_stats) {
    stats = in_stats;
}

Weapon::Weapon(std::string in_name, Weapon_stats in_stats) : Weapon(in_stats) {
    name = in_name;
}

Weapon::Weapon(std::string in_name, std::string in_type, Weapon_stats in_stats ) : Weapon(in_name, in_stats) {
    type = in_type;
}

Weapon_stats Weapon::getStats() {
    return(stats);
}

void Weapon::setMalus(Unit_stats in_malus) {
    malus = in_malus;
}

Unit_stats Weapon::getMalus() {
    return(malus);
}

void Weapon::setStats(Weapon_stats in_stats) {
    stats = in_stats;
}

void Weapon::setBonus(Unit_stats in_bonus) {
    bonus = in_bonus;
}
Unit_stats Weapon::getBonus() {
    return(bonus);
}

void Weapon::setEffect(std::string in_effect) {
    effect = in_effect;
}
std::string Weapon::getEffect() {
    return(effect);
}
void Weapon::setEffective(std::string in_effective) {
	effective = in_effective;
}
std::string Weapon::getEffective() {
    return(effective);
}
void Weapon::setDescription(std::string in_description) {
	description = in_description;
}
std::string Weapon::getDescription() {
    return(description);
}
std::string Weapon::getName() {
    return(name);
}
std::string Weapon::getType() {
    return(type);
}

void Weapon::write(const char * filename, const char * mode){
    FILE * fp;
    fp = fopen(filename, mode);
    fprintf(fp, "%s \n", name.c_str());
    fprintf(fp, "%s \n", type.c_str());
    fprintf(fp, "%s \n", description.c_str());
    fprintf(fp, "%s \n", effect.c_str());
    if (stats.dmg_type) {
        fprintf(fp, "Damage type: %s \n", "Magic");
    } else {
        fprintf(fp, "Damage type: %s \n", "Physical");
    }
    // Handedness variable sucks and is hard to print.
    fprintf(fp, "Stats, PMight, MMight, Hit, Dodge, Crit, Favor, Weight, Proficiency, Range min, Range max, Price\n");
    fprintf(fp, "Stats,\t\t%d,\t%d,\t%d,\t%d,\t%d,\t%d,\t%d,\t%d,\t%d,\t%d,\t%d\n", stats.Pmight, stats.Mmight, stats.combat.hit, stats.combat.dodge, stats.combat.crit, stats.combat.favor, stats.prof, stats.uses, stats.wgt, stats.price);
    fprintf(fp, "Units Stats, HP, Str, Mag, Skl, Spd, Luck, Def, Res, Con, Move\n");
    fprintf(fp, "Unit Malus,\t\t%d,\t%d,\t%d,\t%d,\t%d,\t%d,\t%d,\t%d,\t%d,\t%d,\t%d\n", malus.hp, malus.str, malus.mag, malus.dex, malus.agi, malus.luck, malus.def, malus.res, malus.con, malus.move, malus.prof);
    fprintf(fp, "Unit Bonus,\t\t%d,\t%d,\t%d,\t%d,\t%d,\t%d,\t%d,\t%d,\t%d,\t%d,\t%d\n", bonus.hp, bonus.str, bonus.mag, bonus.dex, bonus.agi, bonus.luck, bonus.def, bonus.res, bonus.con, bonus.move, bonus.prof);
    fprintf(fp, "\n");
    fclose(fp);
}

std::unordered_map<std::string, Weapon> all_weapons;

void baseWeapons(){
    printf("Making base weapons\n");
    Weapon_stats temp_wpn;
    Unit_stats temp_unit;
    // Pmight, Mmight, hit, dodge, crit, favor, wgt, uses, wpnlvl, range, hand, dmg_type, cost
    temp_wpn = {3, 0, 80, 0, 0, 0, 3, 30, 2, {1,1}, {1,2}, 0, 1000};
    Weapon wooden_sword("Wooden sword", "sword", temp_wpn);
    wooden_sword.setDescription("Practice sword, made of wood. It's crushing blows are still deadly.");
    all_weapons["wooden_sword"] = wooden_sword;
    temp_wpn = {1, 0, 25, 0, 0, 0, 2, 10, 1, {1,1}, {1}, 0, 1000};
    Weapon kitchen_knife("Kitchen knife", "sword", temp_wpn);
    kitchen_knife.setDescription("The poor, desperate swordman's dagger.");
    all_weapons["kitchen_knife"] = kitchen_knife;
    temp_wpn = {7, 0, 70, 0, 25, 0, 7, 30, 9, {1,1}, {1,2}, 0, 1000};
    Weapon wrath_sword("Wrath sword", "sword", temp_wpn);
    wrath_sword.setDescription("Increases critical hit chance.");
    all_weapons["wrath_sword"] = wrath_sword;
    temp_wpn = {4, 0, 90, 0, 0, 0, 4, 20, 6, {1,1}, {1}, 0, 1000};
    Weapon fleuret("Fleuret", "sword", temp_wpn);
    fleuret.setEffective("Armor");
    fleuret.setDescription("Simple and light thrust swords. Effective against armor. Usually used with off-hand parry daggers.");
    all_weapons["fleuret"] = fleuret;
    temp_wpn = {8, 0, 85, 5, 0, 0, 5, 25, 8, {1,1}, {1}, 0, 1000};
    Weapon rapiere("Rapière", "sword", temp_wpn);
    rapiere.setEffective("Armor");
    rapiere.setDescription("Strong thrust swords. Effective against armor. Usually used with off-hand parry daggers.");
    all_weapons["rapiere"] = rapiere;
    temp_wpn = {5, 0, 55, 0, 0, 0, 7, 30, 3, {1,1}, {1}, 0, 1000};
    Weapon glaive("Glaive", "sword", temp_wpn);
    glaive.setDescription("Old-world style short sword.");
    all_weapons["glaive"] = glaive;
    temp_wpn = {7, 0, 60, 10, 0, 0, 6, 33, 8, {1,1}, {1}, 0, 1000};
    Weapon saber("Saber", "sword", temp_wpn);
    saber.setDescription("Curved sword made for slashing.");
    all_weapons["saber"] = saber;
    temp_wpn = {8, 0, 60, 15, 0, 0, 8, 27, 10, {1,1}, {1}, 0, 1000};
    Weapon fauchon("Fauchon", "sword", temp_wpn);
    fauchon.setDescription("Curved sword made for slashing.");
    all_weapons["fauchon"] = fauchon;
    temp_wpn = {6, 0, 70, 0, 0, 0, 6, 45, 6, {1,1}, {1,2}, 0, 1000};
    Weapon iron_sword("Iron sword", "sword", temp_wpn);
    iron_sword.setDescription("Simple straight sword made of iron. Cheap and reliable.");
    all_weapons["iron_sword"] = iron_sword;
    temp_wpn = {9, 0, 60, 0, 0, 0, 9, 30, 10, {1,1}, {1,2}, 0, 1000};
    Weapon steel_sword("Steel sword", "sword", temp_wpn);
    steel_sword.setDescription("Simple straight sword made of steel. Strong and durable.");
    all_weapons["steel_sword"] = steel_sword;
    temp_wpn = {13, 0, 65, 0, 0, 0, 10, 25, 12, {1,1}, {1,2}, 0, 1000};
    Weapon damas_sword("Damas sword", "sword", temp_wpn);
    damas_sword.setDescription("Simple straight sword made of damascus steel. Beautifully rippled, deadly and expensive.");
    all_weapons["damas_sword"] = damas_sword;
    temp_wpn = {4, 0, 70, 0, 0, 10, 6, 30, 9, {1,1}, {1,2}, 0, 1000};
    Weapon merciful_blade("Merciful blade", "sword", temp_wpn);
    merciful_blade.setEffect("spare");
    merciful_blade.setDescription("Infused with the life-giving tears of the goddess. Cannot reduce enemy HP below 1.");
    all_weapons["merciful_blade"] = merciful_blade;
    temp_wpn = {10, 0, 40, 0, 0, 0, 10, 25, 6, {1,1}, {1,2}, 0, 1000};
    Weapon broadsword("Broadsword", "sword", temp_wpn);
    broadsword.setDescription("Blade of notable girth. Heavy and strong.");
    temp_wpn = {12, 0, 40, 0, 0, 0, 13, 20, 7, {1,1}, {1,2}, 0, 1000};
    all_weapons["broadsword"] = broadsword;
    Weapon espadon("Espadon", "sword", temp_wpn);
    espadon.setDescription("Blade of considerable girth and length. Heavier and stronger.");
    all_weapons["espadon"] = espadon;
    temp_wpn = {9, 0, 60, 10, 5, 0, 8, 25, 10, {1,1}, {1,2}, 0, 1000};
    Weapon oodachi("Oodachi", "sword", temp_wpn);
    oodachi.setDescription("Long, curved sword of the eastern lands.");
    all_weapons["oodachi"] = oodachi;
    temp_wpn = {7, 0, 75, 5, 0, 0, 5, 30, 8, {1,1}, {1,2}, 0, 1000};
    Weapon uchigatana("Uchigatana", "sword", temp_wpn);
    uchigatana.setDescription("Curved sword in the style of the eastern lands. Very sharp.");
    all_weapons["uchigatana"] = uchigatana;
    temp_wpn = {1, 0, 50, 0, 0, 0, 1, 20, 10, {1,1}, {1,2}, 0, 1000};
    Weapon exsangue("Exsangue", "sword", temp_wpn);
    exsangue.setEffect("lifesteal");
    exsangue.setDescription("Drains enemies of their blood. Heals HP equal to damage dealt.");
    all_weapons["exsangue"] = exsangue;
    temp_wpn = {14, 0, 90, 10, 0, 0, 7, 25, 14, {1,1}, {1,2}, 0, 1000};
    Weapon Hauteclaire("Hauteclaire", "sword", temp_wpn);
    Hauteclaire.setDescription("Crystal encrusted sword, as described in the Franc legends of old.");
    all_weapons["Hauteclaire"] = Hauteclaire;
    temp_wpn = {16, 0, 100, 0, 0, 10, 9, 20, 12, {1,1}, {1,2}, 0, 1000};
    Weapon Secundus("Secundus", "sword", temp_wpn);
    Secundus.setDescription("The first sword made by the God. The first sword is only the second weapon of man, after his fists.");
    all_weapons["Secundus"] = Secundus;
    temp_wpn = {15, 0, 90, 10, 0, 0, 10, 30, 15, {1,1}, {1,2}, 0, 1000};
    Weapon Excalibur("Excalibur", "sword", temp_wpn);
    Excalibur.setEffective({"Demon"});
    Excalibur.setDescription("The king's sword. Found in a rock.");
    all_weapons["Excalibur"] = Excalibur;
    temp_wpn = {12, 0, 100, 0, 0, 0, 6, 20, 18, {1,1}, {1}, 0, 1000};
    Weapon Galatine("Galatine", "sword", temp_wpn);
    Galatine.setEffective("Armor");
    Galatine.setDescription("A pragmatic and honorable knight's thrusting sword. Usually used with off-hand parry daggers.");
    all_weapons["Galatine"] = Galatine;
    temp_wpn = {5, 0, 50, 0, 0, 0, 7, 25, 11, {1,2}, {1,2}, 1, 1000};
    Weapon Raijintou("Raijintou", "sword", temp_wpn);
    Raijintou.setDescription("The thunder god's sword. Rains lightning upon enemies at a distance.");
    all_weapons["Raijintou"] = Raijintou;
    temp_wpn = {7, 0, 45, 0, 0, 0, 1, 25, 11, {1,2}, {1,2}, 1, 1000};
    Weapon Fuujintou("Fuujintou", "sword", temp_wpn);
    Fuujintou.setDescription("The wind god's sword. Cuts enemies with wind at a distance.");
    all_weapons["Fuujintou"] = Fuujintou;
    temp_wpn = {16, 0, 60, 5, 5, 0, 7, 35, 18, {1,1}, {1,2}, 0, 1000};
    Weapon Honjou_Masamune("Honjou Masamune", "sword", temp_wpn);
    Honjou_Masamune.setDescription("Lost sword made by the most skilled swordsmith of the eastern lands.");
    all_weapons["Honjou_Masamune"] = Honjou_Masamune;
    temp_wpn = {20, 0, 30, 0, 0, 0, 20, 50, 20, {1,1}, {1,2}, 0, 1000};
    Weapon Raw_iron_slab("Raw Iron slab", "sword", temp_wpn); //Should be found inside a dragon skull? 
    Raw_iron_slab.setDescription("Barely a sword. Too big, too thick, too massive, too rough. Tempered by the malevolence of countless slain demons. Effective against demons and angels.");
    Raw_iron_slab.setEffective("DemonAngel");
    all_weapons["Raw_iron_slab"] = Raw_iron_slab;
// Should have deeps chinks. This isn't the first time demons roamed the earth. Has deep holes in it. Must be repaired before being usable. Who is willing to repair it? 

    temp_wpn = {2, 0, 60, 0, 0, 0, 4, 15, 1, {1,1}, {1,2}, 0, 1000};
    Weapon pitchfork("Pitchfork", "lance", temp_wpn);
    pitchfork.setDescription("Farming implement. Makeshift lance for desperate times.");
    all_weapons["pitchfork"] = pitchfork;
    temp_wpn = {3, 0, 70, 0, 0, 0, 3, 30, 5, {1,1}, {2}, 0, 1000};
    Weapon fighthing_stick("Fighthing stick", "lance", temp_wpn);
    fighthing_stick.setDescription("Simple two meter long wooden stick. Deadly in competent hands.");
    all_weapons["fighthing_stick"] = fighthing_stick;
    temp_wpn = {5, 0, 95, 0, 0, 0, 5, 40, 4, {1,1}, {1,2}, 0, 1000};
    Weapon iron_lance("Iron lance", "lance", temp_wpn);
    iron_lance.setDescription("Sharp iron tip on a wooden stick. Standard military fare.");
    all_weapons["iron_lance"] = iron_lance;
    temp_wpn = {7, 0, 80, 30, 0, 0, 3, 30, 9, {1,1}, {1,2}, 0, 1000};
    Weapon wrath_lance("Wrath lance", "lance", temp_wpn);
    wrath_lance.setDescription("Increases critical hit chance.");
    all_weapons["wrath_lance"] = wrath_lance;
    temp_wpn = {8, 0, 90, 0, 0, 0, 9, 35, 7, {1,1}, {1,2}, 0, 1000};
    Weapon steel_lance("Steel lance", "lance", temp_wpn);
    steel_lance.setDescription("Stronger and deadlier.");
    all_weapons["steel_lance"] = steel_lance;
    temp_wpn = {12, 0, 85, 0, 0, 0, 12, 25, 13, {1,1}, {1,2}, 0, 1000};
    Weapon damas_lance("Damas lance", "lance", temp_wpn);
    damas_lance.setDescription("Fancy and fatal.");
    all_weapons["damas_lance"] = damas_lance;
    temp_wpn = {6, 0, 60, 0, 0, 0, 15, 10, 8, {1,1}, {1,2}, 0, 1000};
    Weapon heavy_spear("Heavy spear", "lance", temp_wpn);
    heavy_spear.setEffective("Armor");
    heavy_spear.setDescription("Heavy enough to pierce armor.");
    all_weapons["heavy_spear"] = heavy_spear;
    temp_wpn = {8, 0, 90, 0, 0, 0, 8, 10, 10, {1,1}, {1,2}, 0, 1000};
    Weapon trident("Trident", "lance", temp_wpn);
    trident.setDescription("Fancy pitchfork");
    all_weapons["trident"] = trident;
    temp_wpn = {6, 0, 70, 0, 0, 0, 7, 20, 7, {1,1}, {1,2}, 0, 1000};
    Weapon merciful_lance("Merciful lance", "lance", temp_wpn);
    merciful_lance.setDescription("Infused with the life-giving tears of the goddess. Cannot reduce enemy HP below 1.");
    all_weapons["merciful_lance"] = merciful_lance;
    temp_wpn = {4, 0, 40, 0, 0, 0, 6, 25, 9, {1,2}, {1}, 0, 1000};
    Weapon javelin("Javelin", "lance", temp_wpn);
    javelin.setDescription("Simple throwing lance.");
    all_weapons["javelin"] = javelin;
    temp_wpn = {6, 0, 45, 0, 0, 0, 8, 23, 10, {1,2}, {1}, 0, 1000};
    Weapon pilum("Pilum", "lance", temp_wpn);
    pilum.setDescription("Reliable throwing lance.");
    all_weapons["pilum"] = pilum;
    temp_wpn = {9, 0, 50, 0, 0, 0, 11, 20, 11, {1,2}, {1}, 0, 1000};
    Weapon spear("Spear", "lance", temp_wpn);
    spear.setDescription("Strong throwing Lance. Strikes true.");
    all_weapons["spear"] = spear;
    temp_wpn = {14, 0, 60, 0, 0, 0, 9, 25, 15, {1,2}, {1,2}, 0, 1000};
    Weapon Vel("Vel", "lance", temp_wpn);
    Vel.setDescription("Legendary throwing lance. Said to be able to home in on the heart of your enemies.");
    all_weapons["Vel"] = Vel;
    temp_wpn = {8, 0, 90, 0, 0, 0, 6, 20, 9, {1,2}, {1,2}, 1, 1000};
    Weapon Gungnir("Gungnir", "lance", temp_wpn);
    Gungnir.setDescription("A lightning bolt resides in its tip. Can shoot lightning at enemies at a distance.");
    all_weapons["Gungnir"] = Gungnir;
    temp_wpn = {12, 0, 70, 0, 0, 0, 10, 32, 18, {1,2}, {1,2}, 0, 1000};
    Weapon Achilles("Achilles", "lance", temp_wpn);
    Achilles.setEffect("1%kill");
    Achilles.setDescription("Legendary throwing spear named after a fearless, immortal hero. Kills instantly if hits enemy's heel (1%).");
    all_weapons["Achilles"] = Achilles;
    temp_wpn = {16, 0, 50, 0, 0, 0, 14, 24, 17, {1,1}, {1,2}, 0, 1000};
    Weapon Tonbokiri("Tonbokiri", "lance", temp_wpn);
    Tonbokiri.setDescription("Cut a dragonfly that tried to land on its tip. Excessively sharp and dangerous.");
    all_weapons["Tonbokiri"] = Tonbokiri;

    temp_wpn = {8, 0, 40, 0, 0, 0, 8, 40, 4, {1,1}, {1,2}, 0, 1000};
    Weapon iron_axe("Iron axe", "axe", temp_wpn);
    iron_axe.setDescription("Iron cutting head on a stick. Simple, easy to repair. Bandits' favorite.");
    all_weapons["iron_axe"] = iron_axe;
    temp_wpn = {11, 0, 50, 0, 0, 0, 10, 35, 7, {1,1}, {1,2}, 0, 1000};
    Weapon steel_axe("Steel axe", "axe", temp_wpn);
    steel_axe.setDescription("Steel cutting head on a stick. Stronger. Smart bandits like it.");
    all_weapons["steel_axe"] = steel_axe;
    temp_wpn = {14, 0, 55, 0, 0, 0, 13, 25, 11, {1,1}, {1,2}, 0, 1000};
    Weapon damas_axe("Damas axe", "axe", temp_wpn);
    damas_axe.setDescription("Damas cutting head on a stick. Fancy and deadly. Scorned by dumb bandits.");
    all_weapons["damas_axe"] = damas_axe;
    temp_wpn = {14, 0, 65, 20, 0, 0, 11, 30, 10, {1,1}, {1,2}, 0, 1000};
    Weapon wrath_axe("Wrath axe", "axe", temp_wpn);
    wrath_axe.setDescription("Increases critical hit chance.");
    all_weapons["wrath_axe"] = wrath_axe;
    temp_wpn = {6, 0, 30, 0, 0, 0, 7, 25, 5, {1,2}, {1,2}, 0, 1000};
    Weapon throwing_axe("Throwing axe", "axe", temp_wpn);
    throwing_axe.setDescription("Smaller, balanced axe, made for throwing. Cheapest option for ranged attacks.");
    all_weapons["throwing_axe"] = throwing_axe;
    temp_wpn = {10, 0, 40, 0, 0, 0, 7, 25, 10, {1,2}, {1,2}, 0, 1000};
    Weapon tomahawk("tomahawk", "axe", temp_wpn);
    tomahawk.setDescription("Smaller, balanced axe, made for throwing. Cheapest option for ranged attacks.");
    all_weapons["tomahawk"] = tomahawk;
    temp_wpn = {7, 0, 60, 0, 0, 0, 9, 20, 3, {1,1}, {1,2}, 0, 1000};
    Weapon pick_axe("Pick axe", "axe", temp_wpn);
    pick_axe.setDescription("Mining implement. Weapon in time of need.");
    all_weapons["pick_axe"] = pick_axe;
    temp_wpn = {4, 0, 40, 0, 0, 0, 5, 25, 2, {1,1}, {1,2}, 0, 1000};
    Weapon hoe("Hoe", "axe", temp_wpn);
    hoe.setDescription("Farming implement. Can be used as an axe if need be.");
    all_weapons["hoe"] = hoe;
    temp_wpn = {9, 0, 50, 5, 0, 0, 6, 15, 1, {1,1}, {1,2}, 0, 1000};
    Weapon club("Club", "axe", temp_wpn);
    club.setDescription("Big and wooden. For when the old-world weapons just aren't quite old enough.");
    all_weapons["club"] = club;
    temp_wpn = {10, 0, 50, 0, 0, 0, 11, 20, 12, {1,1}, {1,2}, 0, 1000};
    Weapon halberd("Halberd", "axelance", temp_wpn);
    halberd.setEffective("Mounted");
    halberd.setDescription("Polearm specially designed to deal with cavalry. Is both a lance and an axe.");
    all_weapons["halberd"] = halberd;
    temp_wpn = {8, 0, 40, 0, 0, 0, 9, 30, 8, {1,1}, {1,2}, 0, 1000};
    Weapon hammer("Hammer", "axe", temp_wpn);
    hammer.setEffective("Armor");
    hammer.setDescription("Armor cannot stand the hammer's heavy crushing blows.");
    all_weapons["hammer"] = hammer;
    temp_wpn = {12, 0, 50, 0, 0, 0, 12, 25, 11, {1,1}, {1,2}, 0, 1000};
    Weapon war_pick("War pick", "axe", temp_wpn);
    war_pick.setEffective("Armor");
    war_pick.setDescription("In trained hands, the pick can penatrate armor.");
    all_weapons["war_pick"] = war_pick;
    temp_wpn = {15, 0, 45, 0, 0, 0, 14, 30, 14, {1,1}, {1,2}, 0, 1000};
    Weapon battleaxe("Battleaxe", "axe", temp_wpn);
    battleaxe.setDescription("Double-bladed, comically oversized axe.");
    all_weapons["battleaxe"] = battleaxe;
    temp_wpn = {17, 0, 60, 0, 0, 0, 17, 20, 17, {1,1}, {1,2}, 0, 1000};
    Weapon Vidyu("Vidyu", "axe", temp_wpn);
    Vidyu.setDescription("Battleaxe of the gods of the east.");
    all_weapons["Vidyu"] = Vidyu;
    temp_wpn = {19, 0, 70, 0, 0, 0, 5, 60, 20, {1,1}, {1,2}, 0, 1000};
    Weapon Hephaestus("Hephaestus", "axe", temp_wpn);
    Hephaestus.setDescription("The god's hammer. Used to forge Secundus, the first sword.");
    all_weapons["Hephaestus"] = Hephaestus;

    temp_wpn = {12, 0, 70, 0, 0, 0, 14, 35, 9, {1,1}, {1}, 0, 1000};
    Weapon adapted_crossbow("Adapted Crossbow", "Bow", temp_wpn);
    adapted_crossbow.setDescription("Crossbow adapted to be fired, reloaded and drawn using one hand and one stump. 1H assassin only.");
    all_weapons["adapted_crossbow"] = adapted_crossbow;
    temp_wpn = {8, 0, 60, 0, 0, 0, 18, 28, 14, {1,1}, {1}, 0, 1000};
    Weapon repeatable_crossbow("Repeatable Crossbow", "Bow", temp_wpn);
    repeatable_crossbow.setDescription("Crossbow that fires many bolts every attack.");
    all_weapons["repeatable_crossbow"] = repeatable_crossbow;
    temp_wpn = {10, 0, 85, 0, 0, 0, 12, 15, 7, {1,1}, {1}, 0, 1000};
    Weapon crossbow("Crossbow", "Bow", temp_wpn);
    crossbow.setDescription("Powerful and easier to use than a bow, although quite heavy.");
    all_weapons["crossbow"] = crossbow;
    temp_wpn = {4, 0, 90, 0, 0, 0, 4, 35, 5, {1,2}, {1}, 0, 1000};
    Weapon short_bow("Short bow", "Bow", temp_wpn);
    short_bow.setDescription("Simple and light bow.");
    all_weapons["short_bow"] = short_bow;
    temp_wpn = {8, 0, 70, 0, 0, 0, 10, 25, 11, {1,3}, {1}, 0, 1000};
    Weapon long_bow("Long bow", "Bow", temp_wpn);
    long_bow.setDescription("Long and heavy bow. Has increased range.");
    all_weapons["long_bow"] = long_bow;
    temp_wpn = {7, 0, 80, 0, 0, 0, 5, 30, 10, {1,1}, {1}, 0, 1000};
    Weapon composite_bow("Composite bow", "Bow", temp_wpn);
    composite_bow.setDescription("Bow made of laminated wood and animal horn. Very useful for horseback shooting.");
    all_weapons["composite_bow"] = composite_bow;
    temp_wpn = {18, 0, 50, 0, 0, 0, 19, 20, 11, {1,1}, {1}, 0, 1000};
    Weapon recurve_crossbow("Recurve Crossbow", "Bow", temp_wpn);
    recurve_crossbow.setDescription("Crossbow with recurve limbs. Heavy and powerful.");
    all_weapons["recurve_crossbow"] = recurve_crossbow;
    temp_wpn = {13, 0, 75, 0, 0, 0, 10, 24, 14, {1,1}, {1}, 0, 1000};
    Weapon recurve_bow("Recurve bow", "Bow", temp_wpn);
    recurve_bow.setDescription("Bow whose limbs curve away when unstrung. Shoots faster and deadlier arrows.");
    all_weapons["recurve_bow"] = recurve_bow;    
    temp_wpn = {19, 0, 69, 0, 0, 0, 9, 25, 17, {1,1}, {1}, 0, 1000};
    Weapon Apollo("Apollo", "Bow", temp_wpn);
    Apollo.setDescription("Bow crafted of sun rays, used by the most handsome ancient god.");
    all_weapons["Apollo"] = Apollo;
    temp_wpn = {18, 0, 96, 0, 0, 0, 4, 26, 19, {1,1}, {1}, 0, 1000};
    Weapon Artemis("Artemis", "Bow", temp_wpn);
    Artemis.setDescription("Bow of the ancient goddess of hunting.");
    all_weapons["Artemis"] = Artemis;    
    temp_wpn = {17, 0, 100, 0, 0, 0, 7, 35, 16, {1,1}, {1}, 0, 1000};
    Weapon Pinaka("Pinaka", "Bow", temp_wpn);
    Pinaka.setDescription("Bow of the god of death.");
    all_weapons["Pinaka"] = Pinaka;

    temp_wpn = {5, 0, 0, 10, 0, 5, 4, 45, 8, {1,1}, {1}, 0, 1000};
    Weapon adapted_shield("Adapted Shield", "shield", temp_wpn);
    adapted_shield.setDescription("Shield adapted to be used without one\'s weak hand. Straps on the forearm. 1H assassin only.");
    all_weapons["adapted_shield"] = adapted_shield;
    temp_wpn = {1, 0, 0, 5, 0, 1, 2, 15, 2, {1,1}, {1}, 0, 1000};
    Weapon pot_lid("Pot lid", "shield", temp_wpn);
    pot_lid.setDescription("Useful in the kitchen. For the poor militia man: life saving device.");
    all_weapons["pot_lid"] = pot_lid;
    temp_wpn = {2, 0, 0, 8, 0, 0, 3, 20, 7, {1,1}, {1}, 0, 1000};
    Weapon wooden_shield("Wooden Shield", "shield", temp_wpn);
    pot_lid.setDescription("Simple wood plank with a strap. Will probably save your life.");
    all_weapons["wooden_shield"] = wooden_shield;
    temp_wpn = {4, 0, 0, 10, 0, 4, 5, 0, 13, {1,1}, {1}, 0, 1000};
    Weapon cloak("Cloak", "shield", temp_wpn);
    cloak.setDescription("The Mousquetaire\"s offhand shield in time of need.");
    all_weapons["cloak"] = cloak;
    temp_wpn = {3, 0, 0, 9, 0, 2, 4, 25, 9, {1,1}, {1}, 0, 1000};
    Weapon leather_shield("Leather Shield", "shield", temp_wpn);
    leather_shield.setDescription("Wooden plank lined with leather. Smooth and slippery. Cheap way to redirect weapons away from the body.");
    all_weapons["leather_shield"] = leather_shield;
    temp_wpn = {8, 0, 0, 15, 0, 4, 4, 35, 12, {1,1}, {1}, 0, 1000};
    Weapon kite_shield("Kite Shield", "shield", temp_wpn);
    kite_shield.setDescription("Large wooden shield of recognizable shape, to increase protection.");
    all_weapons["kite_shield"] = kite_shield;    
    temp_wpn = {6, 0, 0, 6, 0, 6, 6, 36, 6, {1,1}, {1}, 0, 1000};
    Weapon iron_buckler("Iron Buckler", "shield", temp_wpn);
    iron_buckler.setDescription("Small round iron shield. Light, resistant.");
    all_weapons["iron_buckler"] = iron_buckler;
    temp_wpn = {7, 0, 0, 7, 0, 7, 7, 37, 7, {1,1}, {1}, 0, 1000};
    Weapon steel_buckler("Steel Buckler", "shield", temp_wpn);
    steel_buckler.setDescription("Small round steel shield. Heavier and more resistant.");
    all_weapons["steel_buckler"] = steel_buckler;
    temp_wpn = {5, 0, 0, 5, 0, 5, 9, 30, 10, {1,1}, {1}, 0, 1000};
    Weapon spiked_buckler("Spiked Buckler", "shield", temp_wpn);
    spiked_buckler.setDescription("Iron buckler with a prickly spike at its center. Damages attackers upon their attacks.");
    all_weapons["spiked_buckler"] = spiked_buckler;    
    temp_wpn = {11, 0, 0, 15, 0, 5, 13, 25, 7, {1,1}, {1}, 0, 1000};
    Weapon tower_shield("Tower Shield", "shield", temp_wpn);
    tower_shield.setDescription("Very large and heavy shield.");
    all_weapons["tower_shield"] = tower_shield;    
    temp_wpn = {12, 0, 0, 20, 0, 10, 10, 0, 14, {1,1}, {1}, 0, 1000};
    Weapon Master_shield("Master Shield", "shield", temp_wpn);
    Master_shield.setDescription("Shield of the princess of time.");
    all_weapons["Master_shield"] = Master_shield;
    temp_wpn = {9, 0, 0, 5, 0, 200, 9, 22, 13, {1,1}, {1}, 0, 1000};
    Weapon Goddess_hand("Goddess's Hand", "shield", temp_wpn);
    Goddess_hand.setDescription("Offers the protection of the goddess. Negates enemy critical hits.");
    all_weapons["Goddess_hand"] = Goddess_hand;
    temp_wpn = {15, 0, 0, 10, 0, 2, 13, 19, 12, {1,1}, {1}, 0, 1000};
    Weapon Ancile("Ancile", "shield", temp_wpn);
    Ancile.setDescription("Shield of the ancient god of war.");
    all_weapons["Ancile"] = Ancile;
    temp_wpn = {15, 0, 0, 10, 0, 2, 13, 19, 12, {1,1}, {1}, 0, 1000};
    Weapon rock_plate("Rock plate", "shield", temp_wpn);
    Ancile.setDescription("Almost a raw boulder. The powerful and dumb demon's favorite.");
    all_weapons["rock_plate"] = rock_plate;    
    temp_wpn = {15, 0, 0, 10, 0, 2, 13, 19, 12, {1,1}, {1}, 0, 1000};
    Weapon obsidian_plate("Obsidian plate", "shield", temp_wpn);
    Ancile.setDescription("Rock melted by demon-breath, then recystallized into a more demon-claw friendly shape. Even harder than rock, but more brittle.");
    all_weapons["obsidian_plate"] = obsidian_plate;
    temp_wpn = {15, 0, 0, 10, 0, 2, 13, 19, 12, {1,1}, {1}, 0, 1000};
    Weapon marble_plate_shield("Marble plate shield", "shield", temp_wpn);
    Ancile.setDescription("Kite shield lined in an ornate layer of sculpted marble. Magical.");
    all_weapons["marble_plate_shield"] = marble_plate_shield;
    temp_wpn = {15, 0, 0, 10, 0, 2, 13, 19, 12, {1,1}, {1}, 0, 1000};
    Weapon divine_protector("Divine Protector", "shield", temp_wpn);
    Ancile.setDescription("Large tower shield imbued with angelic properties. Surprinsgly light and effective against magic attacks.");
    all_weapons["divine_protector"] = divine_protector;

    // Daggers get ridiculously low hit cause they are short and stubby!!!! Kitchen knife is the only exception.
    temp_wpn = {2, 0, 10, 0, 10, 0, 5, 21, 16, {1,1}, {1}, 0, 1000};
    Weapon wristblade("Wristblade", "offhand", temp_wpn);
    wristblade.setDescription("Blade generally strapped to the weak forearm. Used by assassins to increase crit potential.");
    all_weapons["wristblade"] = wristblade;    
    temp_wpn = {2, 0, 10, 0, 10, 0, 5, 21, 16, {1,1}, {1}, 0, 1000};
    Weapon shieldhook("Shield Hook", "offhand", temp_wpn);
    wristblade.setDescription("Offhand weapon designed to grab on shields to drag them aside. Ignores shield bonuses.");
    wristblade.setEffect("ignoreshield");
    all_weapons["shieldhook"] = shieldhook;
    temp_wpn = {4, 0, 8, 0, 14, 0, 7, 16, 19, {1,1}, {1}, 0, 1000};
    Weapon retractable_wristblade("Retractable Wristblade", "offhand", temp_wpn);
    retractable_wristblade.setDescription("Blade generally strapped to the weak forearm. Controlled by hand movements. Used by assasins to stealthily increase crit potential.");
    all_weapons["retractable_wristblade"] = retractable_wristblade;
    temp_wpn = {1, 0, 0, 10, 0, 5, 4, 35, 7, {1,1}, {1}, 0, 1000};
    Weapon main_gauche("Main Gauche", "offhand", temp_wpn);
    main_gauche.setDescription("Dagger designed to parry using the left/weak/off-hand. Increases avoir and favor.");
    all_weapons["main_gauche"] = main_gauche;
    temp_wpn = {2, 0, 0, 22, 0, 4, 6, 28, 12, {1,1}, {1}, 0, 1000};
    Weapon swordbreaker("Swordbreaker", "offhand", temp_wpn);
    swordbreaker.setDescription("Dagger with slots designed to catch swords. Increases avoid and favor. Rarely does it actually break swords...");
    all_weapons["swordbreaker"] = swordbreaker;
    temp_wpn = {3, 0, 14, 0, 8, 0, 4, 20, 10, {1,1}, {1}, 0, 1000};
    Weapon trident_dagger("Trident Dagger", "offhand", temp_wpn);
    trident_dagger.setDescription("Dagger whose blade splits into three prongs. Designed to catch opposing swords. Increases avoid and favor.");
    all_weapons["trident_dagger"] = trident_dagger;
    temp_wpn = {3, 0, 5, 0, 0, 0, 2, 26, 8, {1,1}, {1}, 0, 1000};
    Weapon tantou("Tantou", "offhand", temp_wpn);
    tantou.setDescription("Eastern style short dagger. Very sharp and dangerous. Increases might.");
    all_weapons["tantou"] = tantou;
    temp_wpn = {5, 0, 8, 0, 0, 0, 4, 24, 11, {1,1}, {1}, 0, 1000};
    Weapon  kodachi("Kodachi", "offhand", temp_wpn);
    kodachi.setDescription("Dagger used by the legendary assasins of the East, the Ninja. Increases might.");
    all_weapons["kodachi"] = kodachi;
        temp_wpn = {4, 0, 10, 0, 0, 0, 3, 20, 10, {1,1}, {1}, 0, 1000};
    Weapon  dague("Dague", "offhand", temp_wpn);
    dague.setDescription("Messenic dagger. Simple and efficient. Beautiful engravings adorn the blade surface. ");
    all_weapons["dague"] = dague;
    temp_wpn = {7, 0, 0, 0, 0, 0, 8, 10, 15, {1,1}, {1}, 0, 1000};
    Weapon wakizashi("Wakizashi", "offhand", temp_wpn);
    wakizashi.setDescription("Dagger of the warriors of the East. Companion of the uchigatana. Very deadly.");
    all_weapons["wakizashi"] = wakizashi;    
    temp_wpn = {2, 0, 0, 0, 0, 0, 4, 15, 7, {1,1}, {1}, 0, 1000};
    Weapon Baquemard("Baquemard", "offhand", temp_wpn);
    Baquemard.setDescription("Legendary Dagger.");
    all_weapons["Baquemard"] = Baquemard;    
    temp_wpn = {0, 0, 0, 0, 0, 0, 7, 30, 10, {1,1}, {1}, 0, 1000};
    Weapon misericorde("Misericorde", "offhand", temp_wpn);
    misericorde.setDescription("Used to delivery the Coup de grace to wounded knights. Might of this dagger increases by one for each missing 3 health.");
    all_weapons["misericorde"] = misericorde;    
    temp_wpn = {8, 0, 10, 10, 0, 2, 9, 24, 17, {1,1}, {1}, 0, 1000};
    Weapon Carnwennan("Carnwennan", "offhand", temp_wpn);
    Carnwennan.setDescription("The king\'s dagger. Has the power to shroud its user in shadow.");
    all_weapons["Carnwennan"] = Carnwennan;    
    temp_wpn = {7, 0, 0, 0, 0, 0, 7, 8, 5, {1,1}, {1}, 0, 1000};
    Weapon flintlock("Flintlock", "offhand", temp_wpn);
    flintlock.setDescription("A gun!");
    all_weapons["flintlock"] = flintlock;

    temp_wpn = {7, 0, 0, 0, 0, 0, 7, 8, 5, {1,1}, {1}, 0, 1000};
    Weapon shrunken_head("Shrunken Head", "trinket", temp_wpn);
    shrunken_head.setDescription("Horrible. Infused with demonic energies.");
    all_weapons["shrunken_head"] = shrunken_head;        
    temp_wpn = {7, 0, 0, 0, 0, 0, 7, 8, 5, {1,1}, {1}, 0, 1000};
    Weapon crystal("Crystal", "trinket", temp_wpn);
    crystal.setDescription("");
    all_weapons["crystal"] = crystal;      
    temp_wpn = {7, 0, 0, 0, 0, 0, 7, 8, 5, {1,1}, {1}, 0, 1000};
    Weapon human_femur("Human femur", "trinket", temp_wpn);
    human_femur.setDescription("Taken from an ancient tomb. Used by demons to increase their magic power.");
    all_weapons["human_femur"] = human_femur;        
    temp_wpn = {7, 0, 0, 0, 0, 0, 7, 8, 5, {1,1}, {1}, 0, 1000};
    Weapon void_fragment("Void fragment", "trinket", temp_wpn);
    void_fragment.setDescription("");
    all_weapons["void_fragment"] = void_fragment;
    temp_wpn = {7, 0, 0, 0, 0, 0, 7, 8, 5, {1,1}, {1}, 0, 1000};
    Weapon demon_fang("Demon fang", "trinket", temp_wpn);
    demon_fang.setDescription("Ripped from the mouth of a dead demon.");
    all_weapons["demon_fang"] = demon_fang;
    temp_wpn = {7, 0, 0, 0, 0, 0, 7, 8, 5, {1,1}, {1}, 0, 1000};
    Weapon angel_feather("Angel feather", "trinket", temp_wpn);
    angel_feather.setDescription("Beautiful and fluffy. Given by incarnates and possessed to the worthy. Increases magical ability. Unusable by demons and demon incarnates.");
    all_weapons["angel_feather"] = angel_feather;
    temp_wpn = {7, 0, 0, 0, 0, 0, 7, 8, 5, {1,1}, {1}, 0, 1000};
    Weapon angel_hair("Angel hair", "trinket", temp_wpn);
    angel_hair.setDescription("Golden strands of angel hair held up by a silver thread. Increases magical ability. Unusable by demons and demon incarnates.");
    all_weapons["angel_hair"] = angel_hair;
    temp_wpn = {7, 0, 0, 0, 0, 0, 7, 8, 5, {1,1}, {1}, 0, 1000};
    Weapon vial_of_light("Vial of light", "trinket", temp_wpn);
    vial_of_light.setDescription("Light magically contained in glass. Shines brightest in darkness.");
    all_weapons["vial_of_light"] = vial_of_light;       
    temp_wpn = {7, 0, 0, 0, 0, 0, 7, 8, 5, {1,1}, {1}, 0, 1000};
    Weapon glass_orb("Glass orb", "trinket", temp_wpn);
    glass_orb.setDescription("Orb used to focus magical power.");
    all_weapons["glass_orb"] = glass_orb;
    temp_wpn = {7, 0, 0, 0, 0, 0, 7, 8, 5, {1,1}, {1}, 0, 1000};
    Weapon war_horn("War horn", "trinket", temp_wpn);
    war_horn.setDescription("Used to call for battle. Raise's close allies' combat abilities.");
    all_weapons["war_horn"] = war_horn;    

    temp_wpn = {0, 12, 76, 0, 0, 0, 11, 25, 7, {1,2}, {1,2}, 1, 1000};
    Weapon ball_lightning("Ball lightning", "elemental", temp_wpn);
    ball_lightning.setDescription("Lightning, compressed and explosive.");
    all_weapons["ball_lightning"] = ball_lightning;
    temp_wpn = {0, 6, 70, 0, 0, 0, 5, 34, 7, {1,2}, {1,2}, 1, 1000};
    Weapon icicle("Icicle", "elemental", temp_wpn);
    icicle.setDescription("Summons sharp and cold death spikes.");
    all_weapons["icicle"] = icicle;
    temp_wpn = {0, 14, 50, 0, 0, 0, 13, 20, 7, {1,2}, {1,2}, 1, 1000};
    Weapon volcano("Volcano", "elemental", temp_wpn);
    volcano.setDescription("Summons magma spewing mountains from under enemies' feets.");
    all_weapons["volcano"] = volcano;
    temp_wpn = {0, 4, 80, 0, 0, 0, 3, 40, 7, {1,2}, {1,2}, 1, 1000};
    Weapon wind_spear("wind_spear", "elemental", temp_wpn);
    wind_spear.setDescription("Invisible, swift and minuscule. Can pierce armor.");
    all_weapons["wind_spear"] = wind_spear;
    temp_wpn = {0, 6, 60, 0, 0, 0, 8, 30, 7, {1,2}, {1,2}, 1, 1000};
    Weapon ember("Ember", "elemental", temp_wpn);
    ember.setDescription("Summon smoldering embers.");    
    all_weapons["ember"] = ember;
    temp_wpn = {0, 13, 40, 0, 0, 0, 11, 25, 7, {1,2}, {1,2}, 1, 1000};
    Weapon tsunami("Tsunami", "elemental", temp_wpn);
    tsunami.setDescription("");
    all_weapons["tsunami"] = tsunami;
    temp_wpn = {0, 18, 50, 0, 0, 0, 11, 15, 7, {1,2}, {1,2}, 1, 1000};
    Weapon God_forge_fire("The God Forge's Fire", "elemental", temp_wpn);
    God_forge_fire.setDescription("The fire used to forge Secundus, contained magically. Can melt any and all metals.");
    all_weapons["God_forge_fire"] = God_forge_fire;
    temp_wpn = {0, 11, 50, 0, 0, 0, 9, 20, 7, {1,2}, {1,2}, 1, 1000};
    Weapon Fingolfin("Fingolfin", "elemental", temp_wpn);
    Fingolfin.setDescription("Distilled courage, strikes at darkness's heart, especially in times of despair. Effective against demons.");
    Fingolfin.setEffective("Demon");
    all_weapons["Fingolfin"] = Fingolfin;
    temp_wpn = {0, 6, 65, 0, 0, 0, 5, 28, 7, {1,2}, {1,2}, 1, 1000};
    Weapon Mithrandir("Mithrandir", "elemental", temp_wpn);
    Mithrandir.setDescription("Inspires courage in the hearts of men. Effective against demons.");
    Mithrandir.setEffective("Demon");
    all_weapons["Mithrandir"] = Mithrandir;

    temp_wpn = {0, 5, 70, 0, 0, 0, 5, 60, 7, {1,2}, {1,2}, 1, 1000};
    Weapon _void("Void", "demonic", temp_wpn);
    _void.setDescription("");
    all_weapons["void"] = _void;
    temp_wpn = {0, 5, 70, 0, 0, 0, 5, 60, 7, {1,2}, {1,2}, 1, 1000};
    Weapon nether("Nether", "demonic", temp_wpn);
    nether.setDescription("");
    all_weapons["nether"] = nether;
    temp_wpn = {0, 5, 70, 0, 0, 0, 5, 60, 7, {1,2}, {1,2}, 1, 1000};
    Weapon downfall("Downfall", "demonic", temp_wpn);
    downfall.setDescription("");
    all_weapons["downfall"] = downfall;
    temp_wpn = {0, 5, 70, 0, 0, 0, 5, 60, 7, {1,2}, {1,2}, 1, 1000};
    Weapon consume("Consume", "demonic", temp_wpn);
    consume.setDescription("");
    all_weapons["consume"] = consume;
    temp_wpn = {0, 5, 70, 0, 0, 0, 5, 60, 7, {1,2}, {1,2}, 1, 1000};
    Weapon putrify("Putrify", "demonic", temp_wpn);
    putrify.setDescription("Brings out the inner rot.");
    all_weapons["Putrify"] = putrify;
    temp_wpn = {0, 5, 70, 0, 0, 0, 5, 60, 7, {1,2}, {1,2}, 1, 1000};
    Weapon Glaurung("Glaurung", "demonic", temp_wpn);
    Glaurung.setDescription("");
    all_weapons["Glaurung"] = Glaurung;
    temp_wpn = {0, 5, 70, 0, 0, 0, 5, 60, 7, {1,2}, {1,2}, 1, 1000};
    Weapon Morgoth("Morgoth", "demonic", temp_wpn);
    Morgoth.setDescription("Dark power, in the shape of a massive dark hammer. Makes craters wherever it strikes.");
    all_weapons["Morgoth"] = Morgoth;
    temp_wpn = {0, 5, 70, 0, 0, 0, 5, 60, 7, {1,2}, {1,2}, 1, 1000};
    Weapon Gothmog("Gothmog", "demonic", temp_wpn);
    Gothmog.setDescription("Infernal whip of flame and shadows.");
    all_weapons["Gothmog"] = Gothmog;
    temp_wpn = {0, 5, 70, 0, 0, 0, 5, 60, 7, {1,2}, {1,2}, 1, 1000};
    Weapon Fatalis("Fatalis", "demonic", temp_wpn);
    Fatalis.setDescription("");
    all_weapons["Fatalis"] = Fatalis;
}
