
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

Weapon::Weapon(std::string in_name, short unsigned int in_type, Weapon_stats in_stats) : Weapon(in_name, in_stats) {
    type = in_type;
}

Weapon::Weapon(std::string in_name, short unsigned int in_type, Weapon_stats in_stats, unsigned char in_id) : Weapon(in_name, in_type, in_stats) {
    id = in_id;
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

void Weapon::setEffect(long unsigned int in_effect) {
    effect = in_effect;
}
long unsigned int Weapon::getEffect() {
    return(effect);
}
void Weapon::setEffective(short unsigned int in_effective) {
	effective = in_effective;
}
short unsigned int Weapon::getEffective() {
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
void Weapon::setName(std::string in_name) {
    name = in_name;
}
short unsigned int Weapon::getType() {
    return(type);
}
void Weapon::setType(short unsigned int in_type) {
    type = in_type;
}

void Weapon::write(const char * filename, const char * mode){
    FILE * fp;
    fp = fopen(filename, mode);
    fprintf(fp, "%s \n", name.c_str());
    std::vector<std::string> wpn_types = wpntype2str(type);
    for (short unsigned int i = 0; i < wpn_types.size(); i++) {
        fprintf(fp, "%s \n", wpn_types[i].c_str());
    }
    fprintf(fp, "%d", type);
    fprintf(fp, "\n");
    fprintf(fp, "%s \n", description.c_str());
    fprintf(fp, "%d \n", effect);
    fprintf(fp, "%d \n", effective);
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

std::vector<Weapon> all_weapons(WEAPON::END);

void baseWeapons2(){
    Weapon temp_wpn;
    Weapon_stats temp_wpn_stats;
    // Pmight, Mmight, hit, dodge, crit, favor, wgt, uses, wpnlvl, range, hand, dmg_type, cost
    temp_wpn_stats = {3, 0, 80, 0, 0, 0, 3, 30, 2, {1,1}, {1,2}, 0, 1000};
    temp_wpn = Weapon("Wooden sword", WPN_TYPE::SWORD, temp_wpn_stats, WEAPON::WOODEN_SWORD);
    temp_wpn.setDescription("Practice sword, made of wood. It's crushing blows are still deadly.");
    all_weapons[WEAPON::WOODEN_SWORD] = temp_wpn;
    temp_wpn_stats = {1, 0, 25, 0, 0, 0, 2, 10, 1, {1,1}, {1}, 0, 1000};
    temp_wpn = Weapon("Kitchen knife", WPN_TYPE::SWORD, temp_wpn_stats, WEAPON::KITCHEN_KNIFE);
    temp_wpn.setDescription("The poor, desperate swordman's dagger.");
    all_weapons[WEAPON::KITCHEN_KNIFE] = temp_wpn;
    temp_wpn_stats = {7, 0, 70, 0, 25, 0, 7, 30, 9, {1,1}, {1,2}, 0, 1000};
    temp_wpn = Weapon("Wrath sword", WPN_TYPE::SWORD, temp_wpn_stats, WEAPON::WRATH_SWORD);
    temp_wpn.setDescription("Increases critical hit chance.");
    all_weapons[WEAPON::WRATH_SWORD] = temp_wpn;
    temp_wpn_stats = {4, 0, 90, 0, 0, 0, 4, 20, 6, {1,1}, {1}, 0, 1000};
    temp_wpn = Weapon("Fleuret", WPN_TYPE::SWORD, temp_wpn_stats, WEAPON::FLEURET);
    temp_wpn.setEffective(UNIT_TYPE::ARMOR);
    temp_wpn.setDescription("Simple and light thrust swords. Effective against armor. Usually used with off-hand parry daggers.");
    all_weapons[WEAPON::FLEURET] = temp_wpn;
    temp_wpn_stats = {8, 0, 85, 5, 0, 0, 5, 25, 8, {1,1}, {1}, 0, 1000};
    temp_wpn = Weapon("Rapière", WPN_TYPE::SWORD, temp_wpn_stats, WEAPON::RAPIERE);
    temp_wpn.setEffective(UNIT_TYPE::ARMOR);
    temp_wpn.setDescription("Strong thrust swords. Effective against armor. Usually used with off-hand parry daggers.");
    all_weapons[WEAPON::RAPIERE] = temp_wpn;
    temp_wpn_stats = {5, 0, 55, 0, 0, 0, 7, 30, 3, {1,1}, {1}, 0, 1000};
    temp_wpn = Weapon("Glaive", WPN_TYPE::SWORD, temp_wpn_stats, WEAPON::GLAIVE);
    temp_wpn.setDescription("Old-world style short sword.");
    all_weapons[WEAPON::GLAIVE] = temp_wpn;
    temp_wpn_stats = {7, 0, 60, 10, 0, 0, 6, 33, 8, {1,1}, {1}, 0, 1000};
    temp_wpn = Weapon("Saber", WPN_TYPE::SWORD, temp_wpn_stats, WEAPON::SABER);
    temp_wpn.setDescription("Curved sword made for slashing.");
    all_weapons[WEAPON::SABER] = temp_wpn;
    temp_wpn_stats = {8, 0, 60, 15, 0, 0, 8, 27, 10, {1,1}, {1}, 0, 1000};
    temp_wpn = Weapon("Fauchon", WPN_TYPE::SWORD, temp_wpn_stats, WEAPON::FAUCHON);
    temp_wpn.setDescription("Curved sword made for slashing.");
    all_weapons[WEAPON::FAUCHON] = temp_wpn;
    temp_wpn_stats = {6, 0, 70, 0, 0, 0, 6, 45, 6, {1,1}, {1,2}, 0, 1000};
    temp_wpn = Weapon("Iron sword", WPN_TYPE::SWORD, temp_wpn_stats, WEAPON::IRON_SWORD);
    temp_wpn.setDescription("Simple straight sword made of iron. Cheap and reliable.");
    all_weapons[WEAPON::IRON_SWORD] = temp_wpn;
    temp_wpn_stats = {9, 0, 60, 0, 0, 0, 9, 30, 10, {1,1}, {1,2}, 0, 1000};
    temp_wpn = Weapon("Steel sword", WPN_TYPE::SWORD, temp_wpn_stats, WEAPON::STEEL_SWORD);
    temp_wpn.setDescription("Simple straight sword made of steel. Strong and durable.");
    all_weapons[WEAPON::STEEL_SWORD] = temp_wpn;
    temp_wpn_stats = {13, 0, 65, 0, 0, 0, 10, 25, 12, {1,1}, {1,2}, 0, 1000};
    temp_wpn = Weapon("Damas sword", WPN_TYPE::SWORD, temp_wpn_stats, WEAPON::DAMAS_SWORD);
    temp_wpn.setDescription("Simple straight sword made of damascus steel. Beautifully rippled, deadly and expensive.");
    all_weapons[WEAPON::DAMAS_SWORD] = temp_wpn;
    temp_wpn_stats = {4, 0, 70, 0, 0, 10, 6, 30, 9, {1,1}, {1,2}, 0, 1000};
    temp_wpn = Weapon("Merciful blade", WPN_TYPE::SWORD, temp_wpn_stats, WEAPON::MERCIFUL_BLADE);
    temp_wpn.setEffect(WPN_EFFECT::SPARE);
    temp_wpn.setDescription("Infused with the life-giving tears of the goddess. Cannot reduce enemy HP below 1.");
    all_weapons[WEAPON::MERCIFUL_BLADE] = temp_wpn;
    temp_wpn_stats = {10, 0, 40, 0, 0, 0, 10, 25, 6, {1,1}, {1,2}, 0, 1000};
    temp_wpn = Weapon("Broadsword", WPN_TYPE::SWORD, temp_wpn_stats, WEAPON::BROADSWORD);
    temp_wpn.setDescription("Blade of notable girth. Heavy and strong.");
    temp_wpn_stats = {12, 0, 40, 0, 0, 0, 13, 20, 7, {1,1}, {1,2}, 0, 1000};
    all_weapons[WEAPON::BROADSWORD] = temp_wpn;
    temp_wpn = Weapon("Espadon", WPN_TYPE::SWORD, temp_wpn_stats, WEAPON::ESPADON);
    temp_wpn.setDescription("Blade of considerable girth and length. Heavier and stronger.");
    all_weapons[WEAPON::ESPADON] = temp_wpn;
    temp_wpn_stats = {9, 0, 60, 10, 5, 0, 8, 25, 10, {1,1}, {1,2}, 0, 1000};
    temp_wpn = Weapon("Oodachi", WPN_TYPE::SWORD, temp_wpn_stats, WEAPON::OODACHI);
    temp_wpn.setDescription("Long, curved sword of the eastern lands.");
    all_weapons[WEAPON::OODACHI] = temp_wpn;
    temp_wpn_stats = {7, 0, 75, 5, 0, 0, 5, 30, 8, {1,1}, {1,2}, 0, 1000};
    temp_wpn = Weapon("Uchigatana", WPN_TYPE::SWORD, temp_wpn_stats, WEAPON::UCHIGATANA);
    temp_wpn.setDescription("Curved sword in the style of the eastern lands. Very sharp.");
    all_weapons[WEAPON::UCHIGATANA] = temp_wpn;
    temp_wpn_stats = {1, 0, 50, 0, 0, 0, 1, 20, 10, {1,1}, {1,2}, 0, 1000};
    temp_wpn = Weapon("Exsangue", WPN_TYPE::SWORD, temp_wpn_stats, WEAPON::EXSANGUE);
    temp_wpn.setEffect(WPN_EFFECT::LIFESTEAL);
    temp_wpn.setDescription("Drains enemies of their blood. Heals HP equal to damage dealt.");
    all_weapons[WEAPON::EXSANGUE] = temp_wpn;
    temp_wpn_stats = {14, 0, 90, 10, 0, 0, 7, 25, 14, {1,1}, {1,2}, 0, 1000};
    temp_wpn = Weapon("Hauteclaire", WPN_TYPE::SWORD, temp_wpn_stats, WEAPON::HAUTECLAIRE);
    temp_wpn.setDescription("Crystal encrusted sword, as described in the Franc legends of old.");
    all_weapons[WEAPON::HAUTECLAIRE] = temp_wpn;
    temp_wpn_stats = {16, 0, 100, 0, 0, 10, 9, 20, 12, {1,1}, {1,2}, 0, 1000};
    temp_wpn = Weapon("Secundus", WPN_TYPE::SWORD, temp_wpn_stats, WEAPON::SECUNDUS);
    temp_wpn.setDescription("The first sword made by the God. The first weapon really only the second weapon of man, after his fists.");
    all_weapons[WEAPON::SECUNDUS] = temp_wpn;
    temp_wpn_stats = {15, 0, 90, 10, 0, 0, 10, 30, 15, {1,1}, {1,2}, 0, 1000};
    temp_wpn = Weapon("Excalibur", WPN_TYPE::SWORD, temp_wpn_stats, WEAPON::EXCALIBUR);
    temp_wpn.setEffective(UNIT_TYPE::DEMON);
    temp_wpn.setDescription("The king's sword. Found in a rock.");
    all_weapons[WEAPON::EXCALIBUR] = temp_wpn;
    temp_wpn_stats = {12, 0, 100, 0, 0, 0, 6, 20, 18, {1,1}, {1}, 0, 1000};
    temp_wpn = Weapon("Galatine", WPN_TYPE::SWORD, temp_wpn_stats, WEAPON::GALATINE);
    temp_wpn.setEffective(UNIT_TYPE::ARMOR);
    temp_wpn.setDescription("A pragmatic and honorable knight's thrusting sword. Usually used with off-hand parry daggers.");
    all_weapons[WEAPON::GALATINE] = temp_wpn;
    temp_wpn_stats = {5, 0, 50, 0, 0, 0, 7, 25, 11, {1,2}, {1,2}, 1, 1000};
    temp_wpn = Weapon("Raijintou", WPN_TYPE::SWORD, temp_wpn_stats, WEAPON::RAIJINTOU);
    temp_wpn.setDescription("The thunder god's sword. Rains lightning upon enemies at a distance.");
    all_weapons[WEAPON::RAIJINTOU] = temp_wpn;
    temp_wpn_stats = {7, 0, 45, 0, 0, 0, 1, 25, 11, {1,2}, {1,2}, 1, 1000};
    temp_wpn = Weapon("Fuujintou", WPN_TYPE::SWORD, temp_wpn_stats, WEAPON::FUUJINTOU);
    temp_wpn.setDescription("The wind god's sword. Cuts enemies with wind at a distance.");
    all_weapons[WEAPON::FUUJINTOU] = temp_wpn;
    temp_wpn_stats = {16, 0, 60, 5, 5, 0, 7, 35, 18, {1,1}, {1,2}, 0, 1000};
    temp_wpn = Weapon("Honjou Masamune", WPN_TYPE::SWORD, temp_wpn_stats, WEAPON::HONJOU_MASAMUNE);
    temp_wpn.setDescription("Lost sword made by the most skilled swordsmith of the eastern lands.");
    all_weapons[WEAPON::HONJOU_MASAMUNE] = temp_wpn;
    temp_wpn_stats = {0, 0, 0, 0, 0, 0, 0, 10, 30, {1,1}, {1,2}, 0, 9001};
    temp_wpn = Weapon("Kirito", WPN_TYPE::SWORD, temp_wpn_stats, WEAPON::KIRITO); //Should be found inside a dragon skull? 
    temp_wpn.setDescription("Useless sword of a fake black swordsman.");
    all_weapons[WEAPON::KIRITO] = temp_wpn;
    temp_wpn_stats = {20, 0, 30, 0, 0, 0, 20, 50, 20, {1,1}, {1,2}, 0, 1000};
    temp_wpn = Weapon("Raw Iron slab", WPN_TYPE::SWORD, temp_wpn_stats, WEAPON::RAW_IRON_SLAB); //Should be found inside a dragon skull? 
    temp_wpn.setDescription("Barely a sword. Too big, too thick, too massive, too rough. Tempered by the malevolence of countless slain demons. Effective against demons and angels.");
    temp_wpn.setEffective(UNIT_TYPE::DEMON + UNIT_TYPE::ANGEL);
    all_weapons[WEAPON::RAW_IRON_SLAB] = temp_wpn;    
    // Should have deeps chinks. This isn't the first time demons roamed the earth. Has deep holes in it. Must be repaired before being usable. Who is willing to repair it? 

    temp_wpn_stats = {2, 0, 60, 0, 0, 0, 4, 15, 1, {1,1}, {1,2}, 0, 1000};
    temp_wpn = Weapon("Pitchfork", WPN_TYPE::LANCE, temp_wpn_stats, WEAPON::PITCHFORK);
    temp_wpn.setDescription("Farming implement. Makeshift lance for desperate times.");
    all_weapons[WEAPON::PITCHFORK] = temp_wpn;
    temp_wpn_stats = {3, 0, 70, 0, 0, 0, 3, 30, 5, {1,1}, {2}, 0, 1000};
    temp_wpn = Weapon("Fighthing stick", WPN_TYPE::LANCE, temp_wpn_stats, WEAPON::FIGHTING_STICK);
    temp_wpn.setDescription("Simple two meter long wooden stick. Deadly in competent hands.");
    all_weapons[WEAPON::FIGHTING_STICK] = temp_wpn;
    temp_wpn_stats = {5, 0, 95, 0, 0, 0, 5, 40, 4, {1,1}, {1,2}, 0, 1000};
    temp_wpn = Weapon("Iron lance", WPN_TYPE::LANCE, temp_wpn_stats, WEAPON::IRON_LANCE);
    temp_wpn.setDescription("Sharp iron tip on a wooden stick. Standard military fare.");
    all_weapons[WEAPON::IRON_LANCE] = temp_wpn;
    temp_wpn_stats = {7, 0, 80, 30, 0, 0, 3, 30, 9, {1,1}, {1,2}, 0, 1000};
    temp_wpn = Weapon("Wrath lance", WPN_TYPE::LANCE, temp_wpn_stats, WEAPON::WRATH_LANCE);
    temp_wpn.setDescription("Increases critical hit chance.");
    all_weapons[WEAPON::WRATH_LANCE] = temp_wpn;
    temp_wpn_stats = {8, 0, 90, 0, 0, 0, 9, 35, 7, {1,1}, {1,2}, 0, 1000};
    temp_wpn = Weapon("Steel lance", WPN_TYPE::LANCE, temp_wpn_stats, WEAPON::STEEL_LANCE);
    temp_wpn.setDescription("Stronger and deadlier.");
    all_weapons[WEAPON::STEEL_LANCE] = temp_wpn;
    temp_wpn_stats = {12, 0, 85, 0, 0, 0, 12, 25, 13, {1,1}, {1,2}, 0, 1000};
    temp_wpn = Weapon("Damas lance", WPN_TYPE::LANCE, temp_wpn_stats, WEAPON::DAMAS_LANCE);
    temp_wpn.setDescription("Fancy and fatal.");
    all_weapons[WEAPON::DAMAS_LANCE] = temp_wpn;
    temp_wpn_stats = {6, 0, 60, 0, 0, 0, 15, 10, 8, {1,1}, {1,2}, 0, 1000};
    temp_wpn = Weapon("Heavy spear", WPN_TYPE::LANCE, temp_wpn_stats, WEAPON::HEAVY_SPEAR);
    temp_wpn.setEffective(UNIT_TYPE::ARMOR);
    temp_wpn.setDescription("Heavy enough to pierce armor.");
    all_weapons[WEAPON::HEAVY_SPEAR] = temp_wpn;
    temp_wpn_stats = {8, 0, 90, 0, 0, 0, 8, 10, 10, {1,1}, {1,2}, 0, 1000};
    temp_wpn = Weapon("Trident", WPN_TYPE::LANCE, temp_wpn_stats, WEAPON::TRIDENT);
    temp_wpn.setDescription("Fancy pitchfork");
    all_weapons[WEAPON::TRIDENT] = temp_wpn;
    temp_wpn_stats = {6, 0, 70, 0, 0, 0, 7, 20, 7, {1,1}, {1,2}, 0, 1000};
    temp_wpn = Weapon("Merciful lance", WPN_TYPE::LANCE, temp_wpn_stats, WEAPON::MERCIFUL_LANCE);
    temp_wpn.setDescription("Infused with the life-giving tears of the goddess. Cannot reduce enemy HP below 1.");
    all_weapons[WEAPON::MERCIFUL_LANCE] = temp_wpn;
    temp_wpn_stats = {4, 0, 40, 0, 0, 0, 6, 25, 9, {1,2}, {1}, 0, 1000};
    temp_wpn = Weapon("Javelin", WPN_TYPE::LANCE, temp_wpn_stats, WEAPON::JAVELIN);
    temp_wpn.setDescription("Simple throwing lance.");
    all_weapons[WEAPON::JAVELIN] = temp_wpn;
    temp_wpn_stats = {6, 0, 45, 0, 0, 0, 8, 23, 10, {1,2}, {1}, 0, 1000};
    temp_wpn = Weapon("Pilum", WPN_TYPE::LANCE, temp_wpn_stats, WEAPON::PILUM);
    temp_wpn.setDescription("Reliable throwing lance.");
    all_weapons[WEAPON::PILUM] = temp_wpn;
    temp_wpn_stats = {9, 0, 50, 0, 0, 0, 11, 20, 11, {1,2}, {1}, 0, 1000};
    temp_wpn = Weapon("Spear", WPN_TYPE::LANCE, temp_wpn_stats, WEAPON::SPEAR);
    temp_wpn.setDescription("Strong throwing Lance. Strikes true.");
    all_weapons[WEAPON::SPEAR] = temp_wpn;
    temp_wpn_stats = {14, 0, 60, 0, 0, 0, 9, 25, 15, {1,2}, {1,2}, 0, 1000};
    temp_wpn = Weapon("Vel", WPN_TYPE::LANCE, temp_wpn_stats, WEAPON::VEL);
    temp_wpn.setDescription("Legendary throwing lance. Said to be able to home in on the heart of your enemies.");
    all_weapons[WEAPON::VEL] = temp_wpn;
    temp_wpn_stats = {8, 0, 90, 0, 0, 0, 6, 20, 9, {1,2}, {1,2}, 1, 1000};
    temp_wpn = Weapon("Gungnir", WPN_TYPE::LANCE, temp_wpn_stats, WEAPON::GUNGNIR);
    temp_wpn.setDescription("A lightning bolt resides in its tip. Can shoot lightning at enemies at a distance.");
    all_weapons[WEAPON::GUNGNIR] = temp_wpn;
    temp_wpn_stats = {12, 0, 70, 0, 0, 0, 10, 32, 18, {1,2}, {1,2}, 0, 1000};
    temp_wpn = Weapon("Achilles", WPN_TYPE::LANCE, temp_wpn_stats, WEAPON::ACHILLES);
    temp_wpn.setEffect(WPN_EFFECT::KILL1P);
    temp_wpn.setDescription("Legendary throwing spear named after a fearless, immortal hero. Kills instantly if hits enemy's heel (1%).");
    all_weapons[WEAPON::ACHILLES] = temp_wpn;
    temp_wpn_stats = {16, 0, 50, 0, 0, 0, 14, 24, 17, {1,1}, {1,2}, 0, 1000};
    temp_wpn = Weapon("Tonbokiri", WPN_TYPE::LANCE, temp_wpn_stats, WEAPON::TONBOKIRI);
    temp_wpn.setDescription("Cut a dragonfly that tried to land on its tip. Excessively sharp and dangerous.");
    all_weapons[WEAPON::TONBOKIRI] = temp_wpn;

    temp_wpn_stats = {8, 0, 40, 0, 0, 0, 8, 40, 4, {1,1}, {1,2}, 0, 1000};
    temp_wpn = Weapon("Iron axe", WPN_TYPE::AXE, temp_wpn_stats, WEAPON::IRON_AXE);
    temp_wpn.setDescription("Iron cutting head on a stick. Simple, easy to repair. Bandits' favorite.");
    all_weapons[WEAPON::IRON_AXE] = temp_wpn;
    temp_wpn_stats = {11, 0, 50, 0, 0, 0, 10, 35, 7, {1,1}, {1,2}, 0, 1000};
    temp_wpn = Weapon("Steel axe", WPN_TYPE::AXE, temp_wpn_stats, WEAPON::STEEL_AXE);
    temp_wpn.setDescription("Steel cutting head on a stick. Stronger. Smart bandits like it.");
    all_weapons[WEAPON::STEEL_AXE] = temp_wpn;
    temp_wpn_stats = {14, 0, 55, 0, 0, 0, 13, 25, 11, {1,1}, {1,2}, 0, 1000};
    temp_wpn = Weapon("Damas axe", WPN_TYPE::AXE, temp_wpn_stats, WEAPON::DAMAS_AXE);
    temp_wpn.setDescription("Damas cutting head on a stick. Fancy and deadly. Scorned by dumb bandits.");
    all_weapons[WEAPON::DAMAS_AXE] = temp_wpn;
    temp_wpn_stats = {14, 0, 65, 20, 0, 0, 11, 30, 10, {1,1}, {1,2}, 0, 1000};
    temp_wpn = Weapon("Wrath axe", WPN_TYPE::AXE, temp_wpn_stats, WEAPON::WRATH_AXE);
    temp_wpn.setDescription("Increases critical hit chance.");
    all_weapons[WEAPON::WRATH_AXE] = temp_wpn;
    temp_wpn_stats = {6, 0, 30, 0, 0, 0, 7, 25, 5, {1,2}, {1,2}, 0, 1000};
    temp_wpn = Weapon("Throwing axe", WPN_TYPE::AXE, temp_wpn_stats, WEAPON::THROWING_AXE);
    temp_wpn.setDescription("Smaller, balanced axe, made for throwing. Cheapest option for ranged attacks.");
    all_weapons[WEAPON::THROWING_AXE] = temp_wpn;
    temp_wpn_stats = {10, 0, 40, 0, 0, 0, 7, 25, 10, {1,2}, {1,2}, 0, 1000};
    temp_wpn = Weapon("tomahawk", WPN_TYPE::AXE, temp_wpn_stats, WEAPON::TOMAHAWK);
    temp_wpn.setDescription("Smaller, balanced axe, made for throwing. Cheapest option for ranged attacks.");
    all_weapons[WEAPON::TOMAHAWK] = temp_wpn;
    temp_wpn_stats = {7, 0, 60, 0, 0, 0, 9, 20, 3, {1,1}, {1,2}, 0, 1000};
    temp_wpn = Weapon("Pick axe", WPN_TYPE::AXE, temp_wpn_stats, WEAPON::PICK_AXE);
    temp_wpn.setDescription("Mining implement. Weapon in time of need.");
    all_weapons[WEAPON::PICK_AXE] = temp_wpn;
    temp_wpn_stats = {4, 0, 40, 0, 0, 0, 5, 25, 2, {1,1}, {1,2}, 0, 1000};
    temp_wpn = Weapon("Hoe", WPN_TYPE::AXE, temp_wpn_stats, WEAPON::HOE);
    temp_wpn.setDescription("Farming implement. Can be used as an axe if need be.");
    all_weapons[WEAPON::HOE] = temp_wpn;
    temp_wpn_stats = {9, 0, 50, 5, 0, 0, 6, 15, 1, {1,1}, {1,2}, 0, 1000};
    temp_wpn = Weapon("Club", WPN_TYPE::AXE, temp_wpn_stats, WEAPON::CLUB);
    temp_wpn.setDescription("Big and wooden. For when the old-world weapons just aren't quite old enough.");
    all_weapons[WEAPON::CLUB] = temp_wpn;
    temp_wpn_stats = {10, 0, 50, 0, 0, 0, 11, 20, 12, {1,1}, {1,2}, 0, 1000};
    temp_wpn = Weapon("Halberd", WPN_TYPE::AXE + WPN_TYPE::LANCE, temp_wpn_stats, WEAPON::HALBERD);
    temp_wpn.setEffective(UNIT_TYPE::MOUNTED);
    temp_wpn.setDescription("Polearm specially designed to deal with cavalry. Is both a lance and an axe.");
    all_weapons[WEAPON::HALBERD] = temp_wpn;
    temp_wpn_stats = {8, 0, 40, 0, 0, 0, 9, 30, 8, {1,1}, {1,2}, 0, 1000};
    temp_wpn = Weapon("Hammer", WPN_TYPE::AXE, temp_wpn_stats, WEAPON::HAMMER);
    temp_wpn.setEffective(UNIT_TYPE::ARMOR);
    temp_wpn.setDescription("Armor cannot stand the hammer's heavy crushing blows.");
    all_weapons[WEAPON::HAMMER] = temp_wpn;
    temp_wpn_stats = {12, 0, 50, 0, 0, 0, 12, 25, 11, {1,1}, {1,2}, 0, 1000};
    temp_wpn = Weapon("War pick", WPN_TYPE::AXE, temp_wpn_stats, WEAPON::WAR_PICK);
    temp_wpn.setEffective(UNIT_TYPE::ARMOR);
    temp_wpn.setDescription("In trained hands, the pick can penatrate armor.");
    all_weapons[WEAPON::WAR_PICK] = temp_wpn;
    temp_wpn_stats = {15, 0, 45, 0, 0, 0, 14, 30, 14, {1,1}, {1,2}, 0, 1000};
    temp_wpn = Weapon("Battleaxe", WPN_TYPE::AXE, temp_wpn_stats, WEAPON::BATTLEAXE);
    temp_wpn.setDescription("Double-bladed, comically oversized axe.");
    all_weapons[WEAPON::BATTLEAXE] = temp_wpn;
    temp_wpn_stats = {17, 0, 60, 0, 0, 0, 17, 20, 17, {1,1}, {1,2}, 0, 1000};
    temp_wpn = Weapon("Vidyu", WPN_TYPE::AXE, temp_wpn_stats, WEAPON::VIDYU);
    temp_wpn.setDescription("Battleaxe of the many-headed and armed gods of the east.");
    all_weapons[WEAPON::VIDYU] = temp_wpn;
    temp_wpn_stats = {19, 0, 70, 0, 0, 0, 5, 60, 20, {1,1}, {1,2}, 0, 1000};
    temp_wpn = Weapon("Hephaestus", WPN_TYPE::AXE, temp_wpn_stats, WEAPON::HEPHAESTUS);
    temp_wpn.setDescription("The god's hammer. Used to forge Secundus, the first sword. Basically a vaguely square stone fixed to a wooden handle.");
    all_weapons[WEAPON::HEPHAESTUS] = temp_wpn;

    temp_wpn_stats = {12, 0, 70, 0, 0, 0, 14, 35, 9, {1,1}, {1}, 0, 1000};
    temp_wpn = Weapon("Adapted Crossbow", WPN_TYPE::BOW, temp_wpn_stats, WEAPON::ADAPTED_CROSSBOW);
    temp_wpn.setDescription("Crossbow adapted to be fired, reloaded and drawn using one hand and one stump. 1H assassin only.");
    all_weapons[WEAPON::ADAPTED_CROSSBOW] = temp_wpn;
    temp_wpn_stats = {8, 0, 60, 0, 0, 0, 18, 28, 14, {1,1}, {1}, 0, 1000};
    temp_wpn = Weapon("Repeatable Crossbow", WPN_TYPE::BOW, temp_wpn_stats, WEAPON::REPEATABLE_CROSSBOW);
    temp_wpn.setDescription("Crossbow that fires many bolts every attack.");
    all_weapons[WEAPON::REPEATABLE_CROSSBOW] = temp_wpn;
    temp_wpn_stats = {10, 0, 85, 0, 0, 0, 12, 15, 7, {1,1}, {1}, 0, 1000};
    temp_wpn = Weapon("Crossbow", WPN_TYPE::BOW, temp_wpn_stats, WEAPON::CROSSBOW);
    temp_wpn.setDescription("Powerful and easier to use than a bow, although quite heavy.");
    all_weapons[WEAPON::CROSSBOW] = temp_wpn;
    temp_wpn_stats = {4, 0, 90, 0, 0, 0, 4, 35, 5, {1,2}, {1}, 0, 1000};
    temp_wpn = Weapon("Short bow", WPN_TYPE::BOW, temp_wpn_stats, WEAPON::SHORT_BOW);
    temp_wpn.setDescription("Simple and light bow.");
    all_weapons[WEAPON::SHORT_BOW] = temp_wpn;
    temp_wpn_stats = {8, 0, 70, 0, 0, 0, 10, 25, 11, {1,3}, {1}, 0, 1000};
    temp_wpn = Weapon("Long bow", WPN_TYPE::BOW, temp_wpn_stats, WEAPON::LONG_BOW);
    temp_wpn.setDescription("Long and heavy bow. Has increased range.");
    all_weapons[WEAPON::LONG_BOW] = temp_wpn;
    temp_wpn_stats = {7, 0, 80, 0, 0, 0, 5, 30, 10, {1,1}, {1}, 0, 1000};
    temp_wpn = Weapon("Composite bow", WPN_TYPE::BOW, temp_wpn_stats, WEAPON::COMPOSITE_BOW);
    temp_wpn.setDescription("Bow made of laminated wood and animal horn. Very useful for horseback shooting.");
    all_weapons[WEAPON::COMPOSITE_BOW] = temp_wpn;
    temp_wpn_stats = {18, 0, 50, 0, 0, 0, 19, 20, 11, {1,1}, {1}, 0, 1000};
    temp_wpn = Weapon("Recurve Crossbow", WPN_TYPE::BOW, temp_wpn_stats, WEAPON::RECURVE_CROSSBOW);
    temp_wpn.setDescription("Crossbow with recurve limbs. Heavy and powerful.");
    all_weapons[WEAPON::RECURVE_CROSSBOW] = temp_wpn;
    temp_wpn_stats = {13, 0, 75, 0, 0, 0, 10, 24, 14, {1,1}, {1}, 0, 1000};
    temp_wpn = Weapon("Recurve bow", WPN_TYPE::BOW, temp_wpn_stats, WEAPON::RECURVE_BOW);
    temp_wpn.setDescription("Bow whose limbs curve away when unstrung. Shoots faster and deadlier arrows.");
    all_weapons[WEAPON::RECURVE_BOW] = temp_wpn;
    temp_wpn_stats = {19, 0, 69, 0, 0, 0, 9, 25, 17, {1,1}, {1}, 0, 1000};
    temp_wpn = Weapon("Apollo", WPN_TYPE::BOW, temp_wpn_stats, WEAPON::APOLLO);
    temp_wpn.setDescription("Bow crafted of sun rays, used by the most handsome ancient god.");
    all_weapons[WEAPON::APOLLO] = temp_wpn;
    temp_wpn_stats = {18, 0, 96, 0, 0, 0, 4, 26, 19, {1,1}, {1}, 0, 1000};
    temp_wpn = Weapon("Artemis", WPN_TYPE::BOW, temp_wpn_stats, WEAPON::ARTEMIS);
    temp_wpn.setDescription("Bow of the ancient goddess of hunting.");
    all_weapons[WEAPON::ARTEMIS] = temp_wpn;
    temp_wpn_stats = {17, 0, 100, 0, 0, 0, 7, 35, 16, {1,1}, {1}, 0, 1000};
    temp_wpn = Weapon("Pinaka", WPN_TYPE::BOW, temp_wpn_stats, WEAPON::PINAKA);
    temp_wpn.setDescription("Bow of the god of death.");
    all_weapons[WEAPON::PINAKA] = temp_wpn;

    temp_wpn_stats = {5, 0, 0, 10, 0, 5, 4, 45, 8, {1,1}, {1}, 0, 1000};
    temp_wpn = Weapon("Adapted Shield", WPN_TYPE::SHIELD, temp_wpn_stats, WEAPON::ADAPTED_SHIELD);
    temp_wpn.setDescription("Shield adapted to be used without one\'s weak hand. Straps on the forearm. 1H assassin only.");
    all_weapons[WEAPON::ADAPTED_SHIELD] = temp_wpn;
    temp_wpn_stats = {1, 0, 0, 5, 0, 1, 2, 15, 2, {1,1}, {1}, 0, 1000};
    temp_wpn = Weapon("Pot lid", WPN_TYPE::SHIELD, temp_wpn_stats, WEAPON::POT_LID);
    temp_wpn.setDescription("Useful in the kitchen. For the poor militia man: life saving device.");
    all_weapons[WEAPON::POT_LID] = temp_wpn;
    temp_wpn_stats = {2, 0, 0, 8, 0, 0, 3, 20, 7, {1,1}, {1}, 0, 1000};
    temp_wpn = Weapon("Wooden Shield", WPN_TYPE::SHIELD, temp_wpn_stats, WEAPON::WOODEN_SHIELD);
    temp_wpn.setDescription("Simple wood plank with a strap. Will probably save your life.");
    all_weapons[WEAPON::WOODEN_SHIELD] = temp_wpn;
    temp_wpn_stats = {4, 0, 0, 10, 0, 4, 5, 0, 13, {1,1}, {1}, 0, 1000};
    temp_wpn = Weapon("Cloak", WPN_TYPE::SHIELD, temp_wpn_stats, WEAPON::CLOAK);
    temp_wpn.setDescription("The Mousquetaire\"s offhand shield in time of need.");
    all_weapons[WEAPON::CLOAK] = temp_wpn;
    temp_wpn_stats = {3, 0, 0, 9, 0, 2, 4, 25, 9, {1,1}, {1}, 0, 1000};
    temp_wpn = Weapon("Leather Shield", WPN_TYPE::SHIELD, temp_wpn_stats, WEAPON::LEATHER_SHIELD);
    temp_wpn.setDescription("Wooden plank lined with leather. Smooth and slippery. Cheap way to redirect weapons away from the body.");
    all_weapons[WEAPON::LEATHER_SHIELD] = temp_wpn;
    temp_wpn_stats = {8, 0, 0, 15, 0, 4, 4, 35, 12, {1,1}, {1}, 0, 1000};
    temp_wpn = Weapon("Kite Shield", WPN_TYPE::SHIELD, temp_wpn_stats, WEAPON::KITE_SHIELD);
    temp_wpn.setDescription("Large wooden shield of recognizable shape, to increase protection.");
    all_weapons[WEAPON::KITE_SHIELD] = temp_wpn;
    temp_wpn_stats = {6, 0, 0, 6, 0, 6, 6, 36, 6, {1,1}, {1}, 0, 1000};
    temp_wpn = Weapon("Iron Buckler", WPN_TYPE::SHIELD, temp_wpn_stats, WEAPON::IRON_BUCKLER);
    temp_wpn.setDescription("Small round iron shield. Light, resistant.");
    all_weapons[WEAPON::IRON_BUCKLER] = temp_wpn;
    temp_wpn_stats = {7, 0, 0, 7, 0, 7, 7, 37, 7, {1,1}, {1}, 0, 1000};
    temp_wpn = Weapon("Steel Buckler", WPN_TYPE::SHIELD, temp_wpn_stats, WEAPON::STEEL_BUCKLER);
    temp_wpn.setDescription("Small round steel shield. Heavier and more resistant.");
    all_weapons[WEAPON::STEEL_BUCKLER] = temp_wpn;
    temp_wpn_stats = {5, 0, 0, 5, 0, 5, 9, 30, 10, {1,1}, {1}, 0, 1000};
    temp_wpn = Weapon("Spiked Buckler", WPN_TYPE::SHIELD, temp_wpn_stats, WEAPON::SPIKED_BUCKLER);
    temp_wpn.setDescription("Iron buckler with a prickly spike at its center. Damages attackers upon their attacks.");
    all_weapons[WEAPON::SPIKED_BUCKLER] = temp_wpn;
    temp_wpn_stats = {11, 0, 0, 15, 0, 5, 13, 25, 7, {1,1}, {1}, 0, 1000};
    temp_wpn = Weapon("Scutum", WPN_TYPE::SHIELD, temp_wpn_stats, WEAPON::SCUTUM);
    temp_wpn.setDescription("Very large and heavy shield.");
    all_weapons[WEAPON::SCUTUM] = temp_wpn;
    temp_wpn_stats = {12, 0, 0, 20, 0, 10, 10, 0, 14, {1,1}, {1}, 0, 1000};
    temp_wpn = Weapon("Master Shield", WPN_TYPE::SHIELD, temp_wpn_stats, WEAPON::MASTER_SHIELD);
    temp_wpn.setDescription("Shield of the princess of time.");
    all_weapons[WEAPON::MASTER_SHIELD] = temp_wpn;
    temp_wpn_stats = {9, 0, 0, 5, 0, 200, 9, 22, 13, {1,1}, {1}, 0, 1000};
    temp_wpn = Weapon("Goddess's Hand", WPN_TYPE::SHIELD, temp_wpn_stats, WEAPON::GODDESS_HAND);
    temp_wpn.setDescription("Offers the protection of the goddess. Negates enemy critical hits.");
    all_weapons[WEAPON::GODDESS_HAND] = temp_wpn;
    temp_wpn_stats = {15, 0, 0, 10, 0, 2, 13, 19, 12, {1,1}, {1}, 0, 1000};
    temp_wpn = Weapon("Ancile", WPN_TYPE::SHIELD, temp_wpn_stats, WEAPON::ANCILE);
    temp_wpn.setDescription("Shield of the ancient god of war.");
    all_weapons[WEAPON::ANCILE] = temp_wpn;
    temp_wpn_stats = {15, 0, 0, 10, 0, 2, 13, 19, 12, {1,1}, {1}, 0, 1000};
    temp_wpn = Weapon("Rock plate", WPN_TYPE::SHIELD, temp_wpn_stats, WEAPON::ROCK_PLATE);
    temp_wpn.setDescription("Almost a raw boulder. The powerful and dumb demon's favorite.");
    all_weapons[WEAPON::ROCK_PLATE] = temp_wpn;
    temp_wpn_stats = {15, 0, 0, 10, 0, 2, 13, 19, 12, {1,1}, {1}, 0, 1000};
    temp_wpn = Weapon("Obsidian plate", WPN_TYPE::SHIELD, temp_wpn_stats, WEAPON::OBSIDIAN_PLATE);
    temp_wpn.setDescription("Rock melted by demon-breath, then recystallized into a more demon-claw friendly shape. Even harder than rock, but more brittle.");
    all_weapons[WEAPON::OBSIDIAN_PLATE] = temp_wpn;
    temp_wpn_stats = {15, 0, 0, 10, 0, 2, 13, 19, 12, {1,1}, {1}, 0, 1000};
    temp_wpn = Weapon("Marble plate shield", WPN_TYPE::SHIELD, temp_wpn_stats, WEAPON::MARBLE_PLATE_SHIELD);
    temp_wpn.setDescription("Kite shield lined in an ornate layer of sculpted marble. Magical.");
    all_weapons[WEAPON::MARBLE_PLATE_SHIELD] = temp_wpn;
    temp_wpn_stats = {15, 0, 0, 10, 0, 2, 13, 19, 12, {1,1}, {1}, 0, 1000};
    temp_wpn = Weapon("Divine Protector", WPN_TYPE::SHIELD, temp_wpn_stats, WEAPON::DIVINE_PROTECTOR);
    temp_wpn.setDescription("Large tower shield imbued with angelic properties. Surprinsgly light and effective against magic attacks.");
    all_weapons[WEAPON::DIVINE_PROTECTOR] = temp_wpn;

    // Daggers get ridiculously low hit cause they are short and stubby!!!! Kitchen knife is the only exception.
    temp_wpn_stats = {2, 0, 10, 0, 10, 0, 5, 21, 16, {1,1}, {1}, 0, 1000};
    temp_wpn = Weapon("Wristblade", WPN_TYPE::OFFHAND, temp_wpn_stats, WEAPON::WRISTBLADE);
    temp_wpn.setDescription("Blade generally strapped to the weak forearm. Used by assassins to increase crit potential.");
    all_weapons[WEAPON::WRISTBLADE] = temp_wpn;
    temp_wpn_stats = {2, 0, 10, 0, 10, 0, 5, 21, 16, {1,1}, {1}, 0, 1000};
    temp_wpn = Weapon("Shield Hook", WPN_TYPE::OFFHAND, temp_wpn_stats, WEAPON::SHIELD_HOOK);
    temp_wpn.setDescription("Offhand weapon designed to grab on shields to drag them aside. Ignores shield bonuses.");
    temp_wpn.setEffect(WPN_EFFECT::IGNORE_SHIELD);
    all_weapons[WEAPON::SHIELD_HOOK] = temp_wpn;
    temp_wpn_stats = {4, 0, 8, 0, 14, 0, 7, 16, 19, {1,1}, {1}, 0, 1000};
    temp_wpn = Weapon("Retractable Wristblade", WPN_TYPE::OFFHAND, temp_wpn_stats, WEAPON::RETRACTABLE_WRISTBLADE);
    temp_wpn.setDescription("Blade generally strapped to the weak forearm. Controlled by hand movements. Used by assasins to stealthily increase crit potential.");
    all_weapons[WEAPON::RETRACTABLE_WRISTBLADE] = temp_wpn;
    temp_wpn_stats = {1, 0, 0, 10, 0, 5, 4, 35, 7, {1,1}, {1}, 0, 1000};
    temp_wpn = Weapon("Main Gauche", WPN_TYPE::OFFHAND, temp_wpn_stats, WEAPON::MAIN_GAUCHE);
    temp_wpn.setDescription("Dagger designed to parry using the left/weak/off-hand. Increases avoir and favor.");
    all_weapons[WEAPON::MAIN_GAUCHE] = temp_wpn;
    temp_wpn_stats = {2, 0, 0, 22, 0, 4, 6, 28, 12, {1,1}, {1}, 0, 1000};
    temp_wpn = Weapon("Swordbreaker", WPN_TYPE::OFFHAND, temp_wpn_stats, WEAPON::SWORDBREAKER);
    temp_wpn.setDescription("Dagger with slots designed to catch swords. Increases avoid and favor. Rarely does it actually break swords...");
    all_weapons[WEAPON::SWORDBREAKER] = temp_wpn;
    temp_wpn_stats = {3, 0, 14, 0, 8, 0, 4, 20, 10, {1,1}, {1}, 0, 1000};
    temp_wpn = Weapon("Trident Dagger", WPN_TYPE::OFFHAND, temp_wpn_stats, WEAPON::TRIDENT_DAGGER);
    temp_wpn.setDescription("Dagger whose blade splits into three prongs. Designed to catch opposing swords. Increases avoid and favor.");
    all_weapons[WEAPON::TRIDENT_DAGGER] = temp_wpn;
    temp_wpn_stats = {3, 0, 5, 0, 0, 0, 2, 26, 8, {1,1}, {1}, 0, 1000};
    temp_wpn = Weapon("Tantou", WPN_TYPE::OFFHAND, temp_wpn_stats, WEAPON::TANTOU);
    temp_wpn.setDescription("Eastern style short dagger. Very sharp and dangerous. Increases might.");
    all_weapons[WEAPON::TANTOU] = temp_wpn;
    temp_wpn_stats = {5, 0, 8, 0, 0, 0, 4, 24, 11, {1,1}, {1}, 0, 1000};
    temp_wpn = Weapon("Kodachi", WPN_TYPE::OFFHAND, temp_wpn_stats, WEAPON::KODACHI);
    temp_wpn.setDescription("Dagger used by the legendary assasins of the East, the Ninja. Increases might.");
    all_weapons[WEAPON::KODACHI] = temp_wpn;
    temp_wpn_stats = {4, 0, 10, 0, 0, 0, 3, 20, 10, {1,1}, {1}, 0, 1000};
    temp_wpn = Weapon("Dague", WPN_TYPE::OFFHAND, temp_wpn_stats, WEAPON::DAGUE);
    temp_wpn.setDescription("Messenic dagger. Simple and efficient. Beautiful engravings adorn the blade surface. ");
    all_weapons[WEAPON::DAGUE] = temp_wpn;
    temp_wpn_stats = {7, 0, 0, 0, 0, 0, 8, 10, 15, {1,1}, {1}, 0, 1000};
    temp_wpn = Weapon("Wakizashi", WPN_TYPE::OFFHAND, temp_wpn_stats, WEAPON::WAKIZASHI);
    temp_wpn.setDescription("Dagger of the warriors of the East. Companion of the uchigatana. Very deadly.");
    all_weapons[WEAPON::WAKIZASHI] = temp_wpn;
    temp_wpn_stats = {2, 0, 0, 0, 0, 0, 4, 15, 7, {1,1}, {1}, 0, 1000};
    temp_wpn = Weapon("Baquemard", WPN_TYPE::OFFHAND, temp_wpn_stats, WEAPON::BRAQUEMARD);
    temp_wpn.setDescription("Legendary Dagger.");
    all_weapons[WEAPON::BRAQUEMARD] = temp_wpn;
    temp_wpn_stats = {0, 0, 0, 0, 0, 0, 7, 30, 10, {1,1}, {1}, 0, 1000};
    temp_wpn = Weapon("Misericorde", WPN_TYPE::OFFHAND, temp_wpn_stats, WEAPON::MISERICORDE);
    temp_wpn.setDescription("Used to delivery the Coup de grace to wounded knights. Might of this dagger increases by one for each missing 3 health.");
    all_weapons[WEAPON::MISERICORDE] = temp_wpn;
    temp_wpn_stats = {8, 0, 10, 10, 0, 2, 9, 24, 17, {1,1}, {1}, 0, 1000};
    temp_wpn = Weapon("Carnwennan", WPN_TYPE::OFFHAND, temp_wpn_stats, WEAPON::CARNWENNAN);
    temp_wpn.setDescription("The king\'s dagger. Has the power to shroud its user in shadow.");
    all_weapons[WEAPON::CARNWENNAN] = temp_wpn;
    temp_wpn_stats = {7, 0, 0, 0, 0, 0, 7, 8, 5, {1,1}, {1}, 0, 1000};
    temp_wpn = Weapon("Flintlock", WPN_TYPE::OFFHAND, temp_wpn_stats, WEAPON::FLINTLOCK);
    temp_wpn.setDescription("A gun!");
    all_weapons[WEAPON::FLINTLOCK] = temp_wpn;

    temp_wpn_stats = {7, 0, 0, 0, 0, 0, 7, 8, 5, {1,1}, {1}, 0, 1000};
    temp_wpn = Weapon("Shrunken Head", WPN_TYPE::TRINKET, temp_wpn_stats, WEAPON::SHRUNKEN_HEAD);
    temp_wpn.setDescription("Horrible. Infused with demonic energies.");
    all_weapons[WEAPON::SHRUNKEN_HEAD] = temp_wpn;
    temp_wpn_stats = {7, 0, 0, 0, 0, 0, 7, 8, 5, {1,1}, {1}, 0, 1000};
    temp_wpn = Weapon("Crystal", WPN_TYPE::TRINKET, temp_wpn_stats, WEAPON::CRYSTAL);
    temp_wpn.setDescription("");
    all_weapons[WEAPON::CRYSTAL] = temp_wpn;
    temp_wpn_stats = {7, 0, 0, 0, 0, 0, 7, 8, 5, {1,1}, {1}, 0, 1000};
    temp_wpn = Weapon("Human femur", WPN_TYPE::TRINKET, temp_wpn_stats, WEAPON::HUMAN_FEMUR);
    temp_wpn.setDescription("Taken from an ancient tomb. Used by demons to increase their magic power.");
    all_weapons[WEAPON::HUMAN_FEMUR] = temp_wpn;
    temp_wpn_stats = {7, 0, 0, 0, 0, 0, 7, 8, 5, {1,1}, {1}, 0, 1000};
    temp_wpn = Weapon("Void fragment", WPN_TYPE::TRINKET, temp_wpn_stats, WEAPON::VOID_FRAGMENT);
    temp_wpn.setDescription("");
    all_weapons[WEAPON::VOID_FRAGMENT] = temp_wpn;
    temp_wpn_stats = {7, 0, 0, 0, 0, 0, 7, 8, 5, {1,1}, {1}, 0, 1000};
    temp_wpn = Weapon("Demon fang", WPN_TYPE::TRINKET, temp_wpn_stats, WEAPON::DEMON_FANG);
    temp_wpn.setDescription("Ripped from the mouth of a dead demon. Unusable by demons.");
    all_weapons[WEAPON::DEMON_FANG] = temp_wpn;
    temp_wpn_stats = {7, 0, 0, 0, 0, 0, 7, 8, 5, {1,1}, {1}, 0, 1000};
    temp_wpn = Weapon("Angel feather", WPN_TYPE::TRINKET, temp_wpn_stats, WEAPON::ANGEL_FEATHER);
    temp_wpn.setDescription("Beautiful and fluffy. Given by incarnates and possessed to the worthy. Increases magical ability. Unusable by demons and demon incarnates.");
    all_weapons[WEAPON::ANGEL_FEATHER] = temp_wpn;
    temp_wpn_stats = {7, 0, 0, 0, 0, 0, 7, 8, 5, {1,1}, {1}, 0, 1000};
    temp_wpn = Weapon("Angel hair", WPN_TYPE::TRINKET, temp_wpn_stats, WEAPON::ANGEL_HAIR);
    temp_wpn.setDescription("Golden strands of angel hair held up by a silver thread. Increases magical ability. Unusable by demons and demon incarnates.");
    all_weapons[WEAPON::ANGEL_HAIR] = temp_wpn;
    temp_wpn_stats = {7, 0, 0, 0, 0, 0, 7, 8, 5, {1,1}, {1}, 0, 1000};
    temp_wpn = Weapon("Vial of light", WPN_TYPE::TRINKET, temp_wpn_stats, WEAPON::VIAL_OF_LIGHT);
    temp_wpn.setDescription("Light magically contained in glass. Shines brightest in darkness.");
    all_weapons[WEAPON::VIAL_OF_LIGHT] = temp_wpn;
    temp_wpn_stats = {7, 0, 0, 0, 0, 0, 7, 8, 5, {1,1}, {1}, 0, 1000};
    temp_wpn = Weapon("Glass orb", WPN_TYPE::TRINKET, temp_wpn_stats, WEAPON::GLASS_ORB);
    temp_wpn.setDescription("Orb used to focus magical power.");
    all_weapons[WEAPON::GLASS_ORB] = temp_wpn;
    temp_wpn_stats = {7, 0, 0, 0, 0, 0, 7, 8, 5, {1,1}, {1}, 0, 1000};
    temp_wpn = Weapon("War horn", WPN_TYPE::TRINKET, temp_wpn_stats, WEAPON::WAR_HORN);
    temp_wpn.setDescription("Used to call for battle. Raise's close allies' combat abilities.");
    all_weapons[WEAPON::WAR_HORN] = temp_wpn;

    temp_wpn_stats = {0, 12, 76, 0, 0, 0, 11, 25, 7, {1,2}, {1,2}, 1, 1000};
    temp_wpn = Weapon("Ball lightning", WPN_TYPE::ELEMENTAL, temp_wpn_stats, WEAPON::BALL_LIGHTNING);
    temp_wpn.setDescription("Lightning, compressed and explosive.");
    all_weapons[WEAPON::BALL_LIGHTNING] = temp_wpn;
    temp_wpn_stats = {0, 6, 70, 0, 0, 0, 5, 34, 7, {1,2}, {1,2}, 1, 1000};
    temp_wpn = Weapon("Icicle", WPN_TYPE::ELEMENTAL, temp_wpn_stats, WEAPON::ICICLE);
    temp_wpn.setDescription("Summons sharp and cold death spikes.");
    all_weapons[WEAPON::ICICLE] = temp_wpn;
    temp_wpn_stats = {0, 14, 50, 0, 0, 0, 13, 20, 7, {1,2}, {1,2}, 1, 1000};
    temp_wpn = Weapon("Volcano", WPN_TYPE::ELEMENTAL, temp_wpn_stats, WEAPON::VOLCANO);
    temp_wpn.setDescription("Summons magma spewing mountains from under enemies' feets.");
    all_weapons[WEAPON::VOLCANO] = temp_wpn;
    temp_wpn_stats = {0, 4, 80, 0, 0, 0, 3, 40, 7, {1,2}, {1,2}, 1, 1000};
    temp_wpn = Weapon("wind_spear", WPN_TYPE::ELEMENTAL, temp_wpn_stats, WEAPON::WIND_SPEAR);
    temp_wpn.setDescription("Invisible, swift and minuscule. Can pierce armor.");
    all_weapons[WEAPON::WIND_SPEAR] = temp_wpn;
    temp_wpn_stats = {0, 6, 60, 0, 0, 0, 8, 30, 7, {1,2}, {1,2}, 1, 1000};
    temp_wpn = Weapon("Ember", WPN_TYPE::ELEMENTAL, temp_wpn_stats, WEAPON::EMBER);
    temp_wpn.setDescription("Summon smoldering embers.");    
    all_weapons[WEAPON::EMBER] = temp_wpn;
    temp_wpn_stats = {0, 13, 40, 0, 0, 0, 11, 25, 7, {1,2}, {1,2}, 1, 1000};
    temp_wpn = Weapon("Tsunami", WPN_TYPE::ELEMENTAL, temp_wpn_stats, WEAPON::TSUNAMI);
    temp_wpn.setDescription("");
    all_weapons[WEAPON::TSUNAMI] = temp_wpn;
    temp_wpn_stats = {0, 18, 50, 0, 0, 0, 11, 15, 7, {1,2}, {1,2}, 1, 1000};
    temp_wpn = Weapon("The God Forge's Fire", WPN_TYPE::ELEMENTAL, temp_wpn_stats, WEAPON::GOD_FORGE_FIRE);
    temp_wpn.setDescription("The fire used to forge Secundus, contained magically. Can melt any and all metals.");
    all_weapons[WEAPON::GOD_FORGE_FIRE] = temp_wpn;
    temp_wpn_stats = {0, 11, 50, 0, 0, 0, 9, 20, 7, {1,2}, {1,2}, 1, 1000};
    temp_wpn = Weapon("Fingolfin", WPN_TYPE::ELEMENTAL, temp_wpn_stats, WEAPON::FINGOLFIN);
    temp_wpn.setDescription("Distilled courage. Strikes at 7 times at darkness's heart. Effective against demons.");
    temp_wpn.setEffective(UNIT_TYPE::DEMON);
    all_weapons[WEAPON::FINGOLFIN] = temp_wpn;
    temp_wpn_stats = {0, 6, 65, 0, 0, 0, 5, 28, 7, {1,2}, {1,2}, 1, 1000};
    temp_wpn = Weapon("Mithrandir", WPN_TYPE::ELEMENTAL, temp_wpn_stats, WEAPON::MITHRANDIR);
    temp_wpn.setDescription("Inspires courage in the hearts of men. Effective against demons.");
    temp_wpn.setEffective(UNIT_TYPE::DEMON);
    all_weapons[WEAPON::MITHRANDIR] = temp_wpn;

    temp_wpn_stats = {0, 5, 70, 0, 0, 0, 5, 60, 7, {1,2}, {1,2}, 1, 1000};
    temp_wpn = Weapon("Void", WPN_TYPE::DEMONIC, temp_wpn_stats, WEAPON::VOIDD);
    temp_wpn.setDescription("");
    all_weapons[WEAPON::VOIDD] = temp_wpn;
    temp_wpn_stats = {0, 5, 70, 0, 0, 0, 5, 60, 7, {1,2}, {1,2}, 1, 1000};
    temp_wpn = Weapon("Nether", WPN_TYPE::DEMONIC, temp_wpn_stats, WEAPON::NETHER);
    temp_wpn.setDescription("");
    all_weapons[WEAPON::NETHER] = temp_wpn;
    temp_wpn_stats = {0, 5, 70, 0, 0, 0, 5, 60, 7, {1,2}, {1,2}, 1, 1000};
    temp_wpn = Weapon("Downfall", WPN_TYPE::DEMONIC, temp_wpn_stats, WEAPON::DOWNFALL);
    temp_wpn.setDescription("");
    temp_wpn.setDescription("");
    all_weapons[WEAPON::DOWNFALL] = temp_wpn;
    temp_wpn_stats = {0, 5, 70, 0, 0, 0, 5, 60, 7, {1,2}, {1,2}, 1, 1000};
    temp_wpn = Weapon("Consume", WPN_TYPE::DEMONIC, temp_wpn_stats, WEAPON::CONSUME);
    temp_wpn.setDescription("");
    all_weapons[WEAPON::CONSUME] = temp_wpn;
    temp_wpn_stats = {0, 5, 70, 0, 0, 0, 5, 60, 7, {1,2}, {1,2}, 1, 1000};
    temp_wpn = Weapon("Putrify", WPN_TYPE::DEMONIC, temp_wpn_stats, WEAPON::PUTRIFY);
    temp_wpn.setDescription("Brings out the inner rot.");
    all_weapons[WEAPON::PUTRIFY] = temp_wpn;
    temp_wpn_stats = {0, 5, 70, 0, 0, 0, 5, 60, 7, {1,2}, {1,2}, 1, 1000};
    temp_wpn = Weapon("Glaurung", WPN_TYPE::DEMONIC, temp_wpn_stats, WEAPON::GLAURUNG);
    temp_wpn.setDescription("");
    all_weapons[WEAPON::GLAURUNG] = temp_wpn;
    temp_wpn_stats = {0, 5, 70, 0, 0, 0, 5, 60, 7, {1,2}, {1,2}, 1, 1000};
    temp_wpn = Weapon("Morgoth", WPN_TYPE::DEMONIC, temp_wpn_stats, WEAPON::MORGOTH);
    temp_wpn.setDescription("Dark power, in the shape of a massive dark hammer. Makes craters wherever it strikes.");
    all_weapons[WEAPON::MORGOTH] = temp_wpn;
    temp_wpn_stats = {0, 5, 70, 0, 0, 0, 5, 60, 7, {1,2}, {1,2}, 1, 1000};
    temp_wpn = Weapon("Gothmog", WPN_TYPE::DEMONIC, temp_wpn_stats, WEAPON::GOTHMOG);
    temp_wpn.setDescription("Infernal whip of flame and shadows.");
    all_weapons[WEAPON::GOTHMOG] = temp_wpn;
    temp_wpn_stats = {0, 5, 70, 0, 0, 0, 5, 60, 7, {1,2}, {1,2}, 1, 1000};
    temp_wpn = Weapon("Fatalis", WPN_TYPE::DEMONIC, temp_wpn_stats, WEAPON::FATALIS);
    temp_wpn.setDescription("");
    all_weapons[WEAPON::FATALIS] = temp_wpn;
}
