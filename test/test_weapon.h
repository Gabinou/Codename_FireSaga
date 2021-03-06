#include "minctest.h"
#include "weapon.h"
#include "platform.h"
#include "narrative.h"

// std_vector<Weapon> baseWeapons(std_vector<short int> toload) {
//     SDL_Log("Making some base weapons");
//     Weapon temp_wpn;
//     Weapon_stats temp_wpn_stats;
//     std_vector<Weapon> wpn_out;
//     wpn_out.resize(toload[toload.size() - 1] + 1);
//     short unsigned int index;

//     for (short unsigned int i = 0; i < toload.size(); i++) {
//         SDL_Log("Weapon: %d %s", i, itemNames[i].c_str());
//         index = toload[i];

//         switch (index) {
//             case ITEM_NAME_KITCHEN_KNIFE:
//                 // Pmight, Mmight, hit, dodge, crit, favor, wgt, uses, wpnlvl, range, hand, dmg_type, price
//                 temp_wpn_stats = {3, 0, 80, 0, 0, 0, 3, 30, 2, {1, 1}, {1, 2}, 0, 200, 0};
//                 temp_wpn = Weapon(ITEM_TYPE_SWORD, temp_wpn_stats, ITEM_NAME_KITCHEN_KNIFE);
//                 temp_wpn.setDescription("Practice sword, made of wood. It's crushing blows are still deadly.");
//                 wpn_out[ITEM_NAME_KITCHEN_KNIFE] = temp_wpn;
//                 break;
//             case ITEM_NAME_KITCHEN_KNIFE:
//                 temp_wpn_stats = {1, 0, 25, 0, 0, 0, 2, 10, 1, {1, 1}, {1}, 0, 100, 0};
//                 temp_wpn = Weapon(ITEM_TYPE_SWORD, temp_wpn_stats, ITEM_NAME_KITCHEN_KNIFE);
//                 temp_wpn.setDescription("The poor, desperate swordman's dagger.");
//                 wpn_out[ITEM_NAME_KITCHEN_KNIFE] = temp_wpn;
//                 break;
//             case ITEM_NAME_WRATH_SWORD:
//                 temp_wpn_stats = {7, 0, 70, 0, 25, 0, 7, 30, 9, {1, 1}, {1, 2}, 0, 800, 0};
//                 temp_wpn = Weapon(ITEM_TYPE_SWORD, temp_wpn_stats, ITEM_NAME_WRATH_SWORD);
//                 temp_wpn.setDescription("Increases critical hit chance.");
//                 wpn_out[ITEM_NAME_WRATH_SWORD] = temp_wpn;
//                 break;
//             case ITEM_NAME_FLEURET:
//                 temp_wpn_stats = {4, 0, 90, 0, 0, 0, 4, 20, 6, {1, 1}, {1}, 0, 370, 0};
//                 temp_wpn = Weapon(ITEM_TYPE_SWORD, temp_wpn_stats, ITEM_NAME_FLEURET);
//                 temp_wpn.setEffective(UNIT_TYPE_ARMOR);
//                 temp_wpn.setDescription("Simple and light thrust swords. Effective against armor. Usually used with off-hand parry daggers.");
//                 wpn_out[ITEM_NAME_FLEURET] = temp_wpn;
//                 break;
//             case ITEM_NAME_RAPIERE:
//                 temp_wpn_stats = {8, 0, 85, 5, 0, 0, 5, 25, 8, {1, 1}, {1}, 0, 470, 0};
//                 temp_wpn = Weapon(ITEM_TYPE_SWORD, temp_wpn_stats, ITEM_NAME_RAPIERE);
//                 temp_wpn.setEffective(UNIT_TYPE_ARMOR);
//                 temp_wpn.setDescription("Strong thrust swords. Effective against armor. Usually used with off-hand parry daggers.");
//                 wpn_out[ITEM_NAME_RAPIERE] = temp_wpn;
//                 break;
//             case ITEM_NAME_GLAIVE:
//                 temp_wpn_stats = {5, 0, 55, 0, 0, 0, 7, 30, 3, {1, 1}, {1}, 0, 450, 0};
//                 temp_wpn = Weapon(ITEM_TYPE_SWORD, temp_wpn_stats, ITEM_NAME_GLAIVE);
//                 temp_wpn.setDescription("Old-world style short sword.");
//                 wpn_out[ITEM_NAME_GLAIVE] = temp_wpn;
//                 break;
//             case ITEM_NAME_SABER:
//                 temp_wpn_stats = {7, 0, 60, 10, 0, 0, 6, 33, 8, {1, 1}, {1}, 0, 600, 0};
//                 temp_wpn = Weapon(ITEM_TYPE_SWORD, temp_wpn_stats, ITEM_NAME_SABER);
//                 temp_wpn.setDescription("Curved sword made for slashing.");
//                 wpn_out[ITEM_NAME_SABER] = temp_wpn;
//                 break;
//             case ITEM_NAME_FAUCHON:
//                 temp_wpn_stats = {8, 0, 60, 15, 0, 0, 8, 27, 10, {1, 1}, {1}, 0, 700, 0};
//                 temp_wpn = Weapon(ITEM_TYPE_SWORD, temp_wpn_stats, ITEM_NAME_FAUCHON);
//                 temp_wpn.setDescription("Curved sword made for slashing.");
//                 wpn_out[ITEM_NAME_FAUCHON] = temp_wpn;
//                 break;
//             case ITEM_NAME_IRON_SWORD:
//                 temp_wpn_stats = {6, 0, 70, 0, 0, 0, 6, 45, 6, {1, 1}, {1, 2}, 0, 500, 0};
//                 temp_wpn = Weapon(ITEM_TYPE_SWORD, temp_wpn_stats, ITEM_NAME_IRON_SWORD);
//                 temp_wpn.setDescription("Simple straight sword made of iron. Cheap and reliable.");
//                 wpn_out[ITEM_NAME_IRON_SWORD] = temp_wpn;
//                 break;
//             case ITEM_NAME_STEEL_SWORD:
//                 temp_wpn_stats = {9, 0, 60, 0, 0, 0, 9, 30, 10, {1, 1}, {1, 2}, 0, 650, 0};
//                 temp_wpn = Weapon(ITEM_TYPE_SWORD, temp_wpn_stats, ITEM_NAME_STEEL_SWORD);
//                 temp_wpn.setDescription("Simple straight sword made of steel. Strong and durable.");
//                 wpn_out[ITEM_NAME_STEEL_SWORD] = temp_wpn;
//                 break;
//             case ITEM_NAME_DAMAS_SWORD:
//                 temp_wpn_stats = {13, 0, 65, 0, 0, 0, 10, 25, 12, {1, 1}, {1, 2}, 0, 1020, 0};
//                 temp_wpn = Weapon(ITEM_TYPE_SWORD, temp_wpn_stats, ITEM_NAME_DAMAS_SWORD);
//                 temp_wpn.setDescription("Simple straight sword made of damascus steel. Beautifully rippled, deadly and expensive.");
//                 wpn_out[ITEM_NAME_DAMAS_SWORD] = temp_wpn;
//                 break;
//             case ITEM_NAME_MERCIFUL_BLADE:
//                 temp_wpn_stats = {4, 0, 70, 0, 0, 10, 6, 30, 9, {1, 1}, {1, 2}, 0, 800, 0};
//                 temp_wpn = Weapon(ITEM_TYPE_SWORD, temp_wpn_stats, ITEM_NAME_MERCIFUL_BLADE);
//                 temp_wpn.setEffect(ITEM_EFFECT_SPARE);
//                 temp_wpn.setDescription("Infused with the life-giving tears of the goddess. Cannot reduce enemy HP below 1.");
//                 wpn_out[ITEM_NAME_MERCIFUL_BLADE] = temp_wpn;
//                 break;
//             case ITEM_NAME_BROADSWORD:
//                 temp_wpn_stats = {10, 0, 40, 0, 0, 0, 10, 25, 6, {1, 1}, {1, 2}, 0, 940, 0};
//                 temp_wpn = Weapon(ITEM_TYPE_SWORD, temp_wpn_stats, ITEM_NAME_BROADSWORD);
//                 temp_wpn.setDescription("Blade of notable girth. Heavy and strong.");
//                 wpn_out[ITEM_NAME_BROADSWORD] = temp_wpn;
//                 break;
//             case ITEM_NAME_ESPADON:
//                 temp_wpn_stats = {12, 0, 40, 0, 0, 0, 13, 20, 7, {1, 1}, {1, 2}, 0, 1100, 0};
//                 temp_wpn = Weapon(ITEM_TYPE_SWORD, temp_wpn_stats, ITEM_NAME_ESPADON);
//                 temp_wpn.setDescription("Blade of considerable girth and length. Heavier and stronger.");
//                 wpn_out[ITEM_NAME_ESPADON] = temp_wpn;
//                 break;
//             case ITEM_NAME_OODACHI:
//                 temp_wpn_stats = {9, 0, 60, 10, 5, 0, 8, 25, 10, {1, 1}, {1, 2}, 0, 660, 0};
//                 temp_wpn = Weapon(ITEM_TYPE_SWORD, temp_wpn_stats, ITEM_NAME_OODACHI);
//                 temp_wpn.setDescription("Long, curved sword of the eastern lands.");
//                 wpn_out[ITEM_NAME_OODACHI] = temp_wpn;
//                 break;
//             case ITEM_NAME_UCHIGATANA:
//                 temp_wpn_stats = {7, 0, 75, 5, 0, 0, 5, 30, 8, {1, 1}, {1, 2}, 0, 880, 0};
//                 temp_wpn = Weapon(ITEM_TYPE_SWORD, temp_wpn_stats, ITEM_NAME_UCHIGATANA);
//                 temp_wpn.setDescription("Curved sword in the style of the eastern lands. Very sharp.");
//                 wpn_out[ITEM_NAME_UCHIGATANA] = temp_wpn;
//                 break;
//             case ITEM_NAME_EXSANGUE:
//                 temp_wpn_stats = {1, 0, 50, 0, 0, 0, 1, 20, 10, {1, 1}, {1, 2}, 0, 666, 0};
//                 temp_wpn = Weapon(ITEM_TYPE_SWORD, temp_wpn_stats, ITEM_NAME_EXSANGUE);
//                 temp_wpn.setEffect(ITEM_EFFECT_LIFESTEAL);
//                 temp_wpn.setDescription("Drains enemies of their blood. Heals HP equal to damage dealt.");
//                 wpn_out[ITEM_NAME_EXSANGUE] = temp_wpn;
//                 break;
//             case ITEM_NAME_HAUTECLAIRE:
//                 temp_wpn_stats = {14, 0, 90, 10, 0, 0, 7, 25, 14, {1, 1}, {1, 2}, 0, 777, 0};
//                 temp_wpn = Weapon(ITEM_TYPE_SWORD, temp_wpn_stats, ITEM_NAME_HAUTECLAIRE);
//                 temp_wpn.setDescription("Crystal encrusted sword, as described in the Franc legends of old.");
//                 wpn_out[ITEM_NAME_HAUTECLAIRE] = temp_wpn;
//                 break;
//             case ITEM_NAME_SECUNDUS:
//                 temp_wpn_stats = {16, 0, 100, 0, 0, 10, 9, 20, 12, {1, 1}, {1, 2}, 0, 69, 0};
//                 temp_wpn = Weapon(ITEM_TYPE_SWORD, temp_wpn_stats, ITEM_NAME_SECUNDUS);
//                 temp_wpn.setDescription("The first sword made by the God. The first weapon really only the second weapon of man, after his fists.");
//                 wpn_out[ITEM_NAME_SECUNDUS] = temp_wpn;
//                 break;
//             case ITEM_NAME_EXCALIBUR:
//                 temp_wpn_stats = {15, 0, 90, 10, 0, 0, 10, 30, 15, {1, 1}, {1, 2}, 0, 1, 0};
//                 temp_wpn = Weapon(ITEM_TYPE_SWORD, temp_wpn_stats, ITEM_NAME_EXCALIBUR);
//                 temp_wpn.setEffective(UNIT_TYPE_DEMON);
//                 temp_wpn.setDescription("The king's sword. Found in a rock.");
//                 wpn_out[ITEM_NAME_EXCALIBUR] = temp_wpn;
//                 break;
//             case ITEM_NAME_GALATINE:
//                 temp_wpn_stats = {12, 0, 100, 0, 0, 0, 6, 20, 18, {1, 1}, {1}, 0, 2000, 0};
//                 temp_wpn = Weapon(ITEM_TYPE_SWORD, temp_wpn_stats, ITEM_NAME_GALATINE);
//                 temp_wpn.setEffective(UNIT_TYPE_ARMOR);
//                 temp_wpn.setDescription("A pragmatic and honorable knight's thrusting sword. Usually used with off-hand parry daggers.");
//                 wpn_out[ITEM_NAME_GALATINE] = temp_wpn;
//                 break;
//             case ITEM_NAME_RAIJINTOU:
//                 temp_wpn_stats = {5, 0, 50, 0, 0, 0, 7, 25, 11, {1, 2}, {1, 2}, 1, 1400, 0};
//                 temp_wpn = Weapon(ITEM_TYPE_SWORD, temp_wpn_stats, ITEM_NAME_RAIJINTOU);
//                 temp_wpn.setDescription("The thunder god's sword. Rains lightning upon enemies at a distance.");
//                 wpn_out[ITEM_NAME_RAIJINTOU] = temp_wpn;
//                 break;
//             case ITEM_NAME_FUUJINTOU:
//                 temp_wpn_stats = {7, 0, 45, 0, 0, 0, 1, 25, 11, {1, 2}, {1, 2}, 1, 1640, 0};
//                 temp_wpn = Weapon(ITEM_TYPE_SWORD, temp_wpn_stats, ITEM_NAME_FUUJINTOU);
//                 temp_wpn.setDescription("The wind god's sword. Cuts enemies with wind at a distance.");
//                 wpn_out[ITEM_NAME_FUUJINTOU] = temp_wpn;
//                 break;
//             case ITEM_NAME_HONJOU_MASAMUNE:
//                 temp_wpn_stats = {16, 0, 60, 5, 5, 0, 7, 35, 18, {1, 1}, {1, 2}, 0, 2200, 0};
//                 temp_wpn = Weapon(ITEM_TYPE_SWORD, temp_wpn_stats, ITEM_NAME_HONJOU_MASAMUNE);
//                 temp_wpn.setDescription("Lost sword made by the most skilled swordsmith of the eastern lands.");
//                 wpn_out[ITEM_NAME_HONJOU_MASAMUNE] = temp_wpn;
//                 break;
//             case ITEM_NAME_RAW_IRON_SLAB:
//                 temp_wpn_stats = {20, 0, 30, 0, 0, 0, 20, 50, 20, {1, 1}, {1, 2}, 0, 9001, 0};
//                 temp_wpn = Weapon(ITEM_TYPE_SWORD, temp_wpn_stats, ITEM_NAME_RAW_IRON_SLAB); //Should be found inside a dragon skull?
//                 temp_wpn.setDescription("Barely a sword. Too big, too thick, too massive, too rough. Tempered by the malevolence of countless slain demons. Effective against demons and angels.");
//                 temp_wpn.setEffective(UNIT_TYPE_DEMON + UNIT_TYPE_ANGEL);
//                 wpn_out[ITEM_NAME_RAW_IRON_SLAB] = temp_wpn;
//                 // Should have deeps chinks. This isn't the first time demons roamed the earth. Has deep holes in it. Must be repaired before being usable. Who is willing to repair it?
//                 break;
//             case ITEM_NAME_KIRITO:
//                 temp_wpn_stats = {0, 0, 0, 0, 0, 0, 0, 10, 30, {1, 1}, {1, 2}, 0, 0};
//                 temp_wpn = Weapon(ITEM_TYPE_SWORD, temp_wpn_stats, ITEM_NAME_KIRITO); //Should be found inside a dragon skull?
//                 temp_wpn.setDescription("Useless sword of a fake black swordsman.");
//                 wpn_out[ITEM_NAME_KIRITO] = temp_wpn;
//                 break;
//             case ITEM_NAME_KILLY:
//                 break;

//             case ITEM_NAME_PITCHFORK:
//                 temp_wpn_stats = {2, 0, 60, 0, 0, 0, 4, 15, 1, {1, 1}, {1, 2}, 0, 200, 0};
//                 temp_wpn = Weapon(ITEM_TYPE_LANCE, temp_wpn_stats, ITEM_NAME_PITCHFORK);
//                 temp_wpn.setDescription("Farming implement. Makeshift lance for desperate times.");
//                 wpn_out[ITEM_NAME_PITCHFORK] = temp_wpn;
//                 break;
//             case ITEM_NAME_FIGHTING_STICK:
//                 temp_wpn_stats = {3, 0, 70, 0, 0, 0, 3, 30, 5, {1, 1}, {2}, 0, 240, 0};
//                 temp_wpn = Weapon(ITEM_TYPE_LANCE, temp_wpn_stats, ITEM_NAME_FIGHTING_STICK);
//                 temp_wpn.setDescription("Simple two meter long wooden stick. Deadly in competent hands.");
//                 wpn_out[ITEM_NAME_FIGHTING_STICK] = temp_wpn;
//                 break;
//             case ITEM_NAME_IRON_LANCE:
//                 temp_wpn_stats = {5, 0, 95, 0, 0, 0, 5, 40, 4, {1, 1}, {1, 2}, 0, 520, 0};
//                 temp_wpn = Weapon(ITEM_TYPE_LANCE, temp_wpn_stats, ITEM_NAME_IRON_LANCE);
//                 temp_wpn.setDescription("Sharp iron tip on a wooden stick. Standard military fare.");
//                 wpn_out[ITEM_NAME_IRON_LANCE] = temp_wpn;
//                 break;
//             case ITEM_NAME_WRATH_LANCE:
//                 temp_wpn_stats = {7, 0, 80, 0, 30, 0, 3, 30, 9, {1, 1}, {1, 2}, 0, 700, 0};
//                 temp_wpn = Weapon(ITEM_TYPE_LANCE, temp_wpn_stats, ITEM_NAME_WRATH_LANCE);
//                 temp_wpn.setDescription("Increases critical hit chance.");
//                 wpn_out[ITEM_NAME_WRATH_LANCE] = temp_wpn;
//                 break;
//             case ITEM_NAME_STEEL_LANCE:
//                 temp_wpn_stats = {8, 0, 90, 0, 0, 0, 9, 35, 7, {1, 1}, {1, 2}, 0, 888, 0};
//                 temp_wpn = Weapon(ITEM_TYPE_LANCE, temp_wpn_stats, ITEM_NAME_STEEL_LANCE);
//                 temp_wpn.setDescription("Stronger and deadlier.");
//                 wpn_out[ITEM_NAME_STEEL_LANCE] = temp_wpn;
//                 break;
//             case ITEM_NAME_DAMAS_LANCE:
//                 temp_wpn_stats = {12, 0, 85, 0, 0, 0, 12, 25, 13, {1, 1}, {1, 2}, 0, 1100, 0};
//                 temp_wpn = Weapon(ITEM_TYPE_LANCE, temp_wpn_stats, ITEM_NAME_DAMAS_LANCE);
//                 temp_wpn.setDescription("Fancy and fatal.");
//                 wpn_out[ITEM_NAME_DAMAS_LANCE] = temp_wpn;
//                 break;
//             case ITEM_NAME_HEAVY_SPEAR:
//                 temp_wpn_stats = {6, 0, 60, 0, 0, 0, 15, 10, 8, {1, 1}, {1, 2}, 0, 650, 0};
//                 temp_wpn = Weapon(ITEM_TYPE_LANCE, temp_wpn_stats, ITEM_NAME_HEAVY_SPEAR);
//                 temp_wpn.setEffective(UNIT_TYPE_ARMOR);
//                 temp_wpn.setDescription("Heavy enough to pierce armor.");
//                 wpn_out[ITEM_NAME_HEAVY_SPEAR] = temp_wpn;
//                 break;
//             case ITEM_NAME_TRIDENT:
//                 temp_wpn_stats = {8, 0, 90, 0, 0, 0, 8, 10, 10, {1, 1}, {1, 2}, 0, 1040, 0};
//                 temp_wpn = Weapon(ITEM_TYPE_LANCE, temp_wpn_stats, ITEM_NAME_TRIDENT);
//                 temp_wpn.setDescription("Fancy pitchfork");
//                 wpn_out[ITEM_NAME_TRIDENT] = temp_wpn;
//                 break;
//             case ITEM_NAME_MERCIFUL_LANCE:
//                 temp_wpn_stats = {6, 0, 70, 0, 0, 0, 7, 20, 7, {1, 1}, {1, 2}, 0, 910, 0};
//                 temp_wpn = Weapon(ITEM_TYPE_LANCE, temp_wpn_stats, ITEM_NAME_MERCIFUL_LANCE);
//                 temp_wpn.setDescription("Infused with the life-giving tears of the goddess. Cannot reduce enemy HP below 1.");
//                 wpn_out[ITEM_NAME_MERCIFUL_LANCE] = temp_wpn;
//                 break;
//             case ITEM_NAME_JAVELIN:
//                 temp_wpn_stats = {4, 0, 40, 0, 0, 0, 6, 25, 9, {1, 2}, {1}, 0, 750, 0};
//                 temp_wpn = Weapon(ITEM_TYPE_LANCE, temp_wpn_stats, ITEM_NAME_JAVELIN);
//                 temp_wpn.setDescription("Simple throwing lance.");
//                 wpn_out[ITEM_NAME_JAVELIN] = temp_wpn;
//                 break;
//             case ITEM_NAME_PILUM:
//                 temp_wpn_stats = {6, 0, 45, 0, 0, 0, 8, 23, 10, {1, 2}, {1}, 0, 990, 0};
//                 temp_wpn = Weapon(ITEM_TYPE_LANCE, temp_wpn_stats, ITEM_NAME_PILUM);
//                 temp_wpn.setDescription("Reliable throwing lance.");
//                 wpn_out[ITEM_NAME_PILUM] = temp_wpn;
//                 break;
//             case ITEM_NAME_SPEAR:
//                 temp_wpn_stats = {9, 0, 50, 0, 0, 0, 11, 20, 11, {1, 2}, {1}, 0, 1300, 0};
//                 temp_wpn = Weapon(ITEM_TYPE_LANCE, temp_wpn_stats, ITEM_NAME_SPEAR);
//                 temp_wpn.setDescription("Strong throwing Lance. Strikes true.");
//                 wpn_out[ITEM_NAME_SPEAR] = temp_wpn;
//                 break;
//             case ITEM_NAME_VEL:
//                 temp_wpn_stats = {14, 0, 60, 0, 0, 0, 9, 25, 15, {1, 2}, {1, 2}, 0, 2000, 0};
//                 temp_wpn = Weapon(ITEM_TYPE_LANCE, temp_wpn_stats, ITEM_NAME_VEL);
//                 temp_wpn.setDescription("Legendary throwing lance. Said to be able to home in on the heart of your enemies.");
//                 wpn_out[ITEM_NAME_VEL] = temp_wpn;
//                 break;
//             case ITEM_NAME_GUNGNIR:
//                 temp_wpn_stats = {8, 0, 90, 0, 0, 0, 6, 20, 9, {1, 2}, {1, 2}, 1, 3000, 0};
//                 temp_wpn = Weapon(ITEM_TYPE_LANCE, temp_wpn_stats, ITEM_NAME_GUNGNIR);
//                 temp_wpn.setDescription("A lightning bolt resides in its tip. Can shoot lightning at enemies at a distance.");
//                 wpn_out[ITEM_NAME_GUNGNIR] = temp_wpn;
//                 break;
//             case ITEM_NAME_ACHILLES:
//                 temp_wpn_stats = {12, 0, 70, 0, 0, 0, 10, 32, 18, {1, 2}, {1, 2}, 0, 4000, 0};
//                 temp_wpn = Weapon(ITEM_TYPE_LANCE, temp_wpn_stats, ITEM_NAME_ACHILLES);
//                 temp_wpn.setEffect(ITEM_EFFECT_KILL1P);
//                 temp_wpn.setDescription("Legendary throwing spear named after a fearless, immortal hero. Kills instantly if hits enemy's heel (1%).");
//                 wpn_out[ITEM_NAME_ACHILLES] = temp_wpn;
//                 break;
//             case ITEM_NAME_TONBOKIRI:
//                 temp_wpn_stats = {16, 0, 50, 0, 0, 0, 14, 24, 17, {1, 1}, {1, 2}, 0, 5000, 0};
//                 temp_wpn = Weapon(ITEM_TYPE_LANCE, temp_wpn_stats, ITEM_NAME_TONBOKIRI);
//                 temp_wpn.setDescription("Cut a dragonfly that tried to land on its tip. Excessively sharp and dangerous.");
//                 wpn_out[ITEM_NAME_TONBOKIRI] = temp_wpn;
//                 break;

//             case ITEM_NAME_IRON_AXE:
//                 temp_wpn_stats = {8, 0, 40, 0, 0, 0, 8, 40, 4, {1, 1}, {1, 2}, 0, 360, 0};
//                 temp_wpn = Weapon(ITEM_TYPE_AXE, temp_wpn_stats, ITEM_NAME_IRON_AXE);
//                 temp_wpn.setDescription("Iron cutting head on a stick. Cheap, simple, easy to repair. Bandits' favorite.");
//                 wpn_out[ITEM_NAME_IRON_AXE] = temp_wpn;
//                 break;
//             case ITEM_NAME_Steel_Lance:
//                 temp_wpn_stats = {11, 0, 50, 0, 0, 0, 10, 35, 7, {1, 1}, {1, 2}, 0, 720, 0};
//                 temp_wpn = Weapon(ITEM_TYPE_AXE, temp_wpn_stats, ITEM_NAME_Steel_Lance);
//                 temp_wpn.setDescription("Steel cutting head on a stick. Stronger. Smart bandits like it.");
//                 wpn_out[ITEM_NAME_Steel_Lance] = temp_wpn;
//                 break;
//             case ITEM_NAME_DAMAS_AXE:
//                 temp_wpn_stats = {14, 0, 55, 0, 0, 0, 13, 25, 11, {1, 1}, {1, 2}, 0, 1080, 0};
//                 temp_wpn = Weapon(ITEM_TYPE_AXE, temp_wpn_stats, ITEM_NAME_DAMAS_AXE);
//                 temp_wpn.setDescription("Damas cutting head on a stick. Fancy and deadly. Scorned by dumb bandits.");
//                 wpn_out[ITEM_NAME_DAMAS_AXE] = temp_wpn;
//                 break;
//             case ITEM_NAME_WRATH_AXE:
//                 temp_wpn_stats = {10, 0, 65, 0, 20, 0, 11, 30, 10, {1, 1}, {1, 2}, 0, 920, 0};
//                 temp_wpn = Weapon(ITEM_TYPE_AXE, temp_wpn_stats, ITEM_NAME_WRATH_AXE);
//                 temp_wpn.setDescription("Increases critical hit chance.");
//                 wpn_out[ITEM_NAME_WRATH_AXE] = temp_wpn;
//                 break;
//             case ITEM_NAME_THROWING_AXE:
//                 temp_wpn_stats = {6, 0, 30, 0, 0, 0, 7, 25, 5, {1, 2}, {1, 2}, 0, 450, 0};
//                 temp_wpn = Weapon(ITEM_TYPE_AXE, temp_wpn_stats, ITEM_NAME_THROWING_AXE);
//                 temp_wpn.setDescription("Smaller, balanced axe, made for throwing. Cheapest option for ranged attacks.");
//                 wpn_out[ITEM_NAME_THROWING_AXE] = temp_wpn;
//                 break;
//             case ITEM_NAME_TOMAHAWK:
//                 temp_wpn_stats = {10, 0, 40, 0, 0, 0, 7, 25, 10, {1, 2}, {1, 2}, 0, 790, 0};
//                 temp_wpn = Weapon(ITEM_TYPE_AXE, temp_wpn_stats, ITEM_NAME_TOMAHAWK);
//                 temp_wpn.setDescription("Smaller, balanced axe, made for throwing. Cheapest option for ranged attacks.");
//                 wpn_out[ITEM_NAME_TOMAHAWK] = temp_wpn;
//                 break;
//             case ITEM_NAME_PICK_AXE:
//                 temp_wpn_stats = {7, 0, 60, 0, 0, 0, 9, 20, 3, {1, 1}, {1, 2}, 0, 650, 0};
//                 temp_wpn = Weapon(ITEM_TYPE_AXE, temp_wpn_stats, ITEM_NAME_PICK_AXE);
//                 temp_wpn.setDescription("Mining implement. Weapon in time of need.");
//                 wpn_out[ITEM_NAME_PICK_AXE] = temp_wpn;
//                 break;
//             case ITEM_NAME_HOE:
//                 temp_wpn_stats = {4, 0, 40, 0, 0, 0, 5, 25, 2, {1, 1}, {1, 2}, 0, 200, 0};
//                 temp_wpn = Weapon(ITEM_TYPE_AXE, temp_wpn_stats, ITEM_NAME_HOE);
//                 temp_wpn.setDescription("Farming implement. Can be used as an axe if need be.");
//                 wpn_out[ITEM_NAME_HOE] = temp_wpn;
//                 break;
//             case ITEM_NAME_CLUB:
//                 temp_wpn_stats = {9, 0, 50, 5, 0, 0, 6, 15, 1, {1, 1}, {1, 2}, 0, 100, 0};
//                 temp_wpn = Weapon(ITEM_TYPE_AXE, temp_wpn_stats, ITEM_NAME_CLUB);
//                 temp_wpn.setDescription("Big and wooden. For when the old-world weapons just aren't quite old enough.");
//                 wpn_out[ITEM_NAME_CLUB] = temp_wpn;
//                 break;
//             case ITEM_NAME_HALBERD:
//                 temp_wpn_stats = {10, 0, 50, 0, 0, 0, 11, 22, 12, {1, 1}, {1, 2}, 0, 1110, 0};
//                 temp_wpn = Weapon(ITEM_TYPE_AXE + ITEM_TYPE_LANCE, temp_wpn_stats, ITEM_NAME_HALBERD);
//                 temp_wpn.setEffective(UNIT_TYPE_MOUNTED);
//                 temp_wpn.setDescription("Polearm specially designed to deal with cavalry. Is both a lance and an axe.");
//                 wpn_out[ITEM_NAME_HALBERD] = temp_wpn;
//                 break;
//             case ITEM_NAME_HAMMER:
//                 temp_wpn_stats = {8, 0, 40, 0, 0, 0, 9, 30, 8, {1, 1}, {1, 2}, 0, 690, 0};
//                 temp_wpn = Weapon(ITEM_TYPE_AXE, temp_wpn_stats, ITEM_NAME_HAMMER);
//                 temp_wpn.setEffective(UNIT_TYPE_ARMOR);
//                 temp_wpn.setDescription("Armor cannot stand the hammer's heavy crushing blows.");
//                 wpn_out[ITEM_NAME_HAMMER] = temp_wpn;
//                 break;
//             case ITEM_NAME_WAR_PICK:
//                 temp_wpn_stats = {12, 0, 50, 0, 0, 0, 12, 25, 11, {1, 1}, {1, 2}, 0, 1310, 0};
//                 temp_wpn = Weapon(ITEM_TYPE_AXE, temp_wpn_stats, ITEM_NAME_WAR_PICK);
//                 temp_wpn.setEffective(UNIT_TYPE_ARMOR);
//                 temp_wpn.setDescription("In trained hands, the pick can penatrate armor.");
//                 wpn_out[ITEM_NAME_WAR_PICK] = temp_wpn;
//                 break;
//             case ITEM_NAME_BATTLEAXE:
//                 temp_wpn_stats = {15, 0, 45, 0, 0, 0, 14, 30, 14, {1, 1}, {1, 2}, 0, 1800, 0};
//                 temp_wpn = Weapon(ITEM_TYPE_AXE, temp_wpn_stats, ITEM_NAME_BATTLEAXE);
//                 temp_wpn.setDescription("Double-bladed, comically oversized axe.");
//                 wpn_out[ITEM_NAME_BATTLEAXE] = temp_wpn;
//                 break;
//             case ITEM_NAME_VIDYU:
//                 temp_wpn_stats = {17, 0, 60, 0, 0, 0, 17, 20, 17, {1, 1}, {1, 2}, 0, 2700, 0};
//                 temp_wpn = Weapon(ITEM_TYPE_AXE, temp_wpn_stats, ITEM_NAME_VIDYU);
//                 temp_wpn.setDescription("Battleaxe of the many-headed and armed gods of the east.");
//                 wpn_out[ITEM_NAME_VIDYU] = temp_wpn;
//                 break;
//             case ITEM_NAME_HEPHAESTUS:
//                 temp_wpn_stats = {19, 0, 70, 0, 0, 0, 5, 60, 20, {1, 1}, {1, 2}, 0, 10000, 0};
//                 temp_wpn = Weapon(ITEM_TYPE_AXE, temp_wpn_stats, ITEM_NAME_HEPHAESTUS);
//                 temp_wpn.setDescription("The god's hammer. Used to forge Secundus, the first sword. Basically a vaguely square stone fixed to a wooden handle.");
//                 wpn_out[ITEM_NAME_HEPHAESTUS] = temp_wpn;
//                 break;

//             case ITEM_NAME_ADAPTED_CROSSBOW:
//                 temp_wpn_stats = {12, 0, 70, 0, 0, 0, 14, 35, 9, {1, 1}, {1}, 0, 2500, 0};
//                 temp_wpn = Weapon(ITEM_TYPE_BOW, temp_wpn_stats, ITEM_NAME_ADAPTED_CROSSBOW);
//                 temp_wpn.setDescription("Crossbow adapted to be fired, reloaded and drawn using one hand and one stump. 1H assassin only.");
//                 wpn_out[ITEM_NAME_ADAPTED_CROSSBOW] = temp_wpn;
//                 break;
//             case ITEM_NAME_REPEATABLE_CROSSBOW:
//                 temp_wpn_stats = {8, 0, 60, 0, 0, 0, 18, 28, 14, {1, 1}, {1}, 0, 2010, 0};
//                 temp_wpn = Weapon(ITEM_TYPE_BOW, temp_wpn_stats, ITEM_NAME_REPEATABLE_CROSSBOW);
//                 temp_wpn.setDescription("Crossbow that fires many bolts every attack.");
//                 wpn_out[ITEM_NAME_REPEATABLE_CROSSBOW] = temp_wpn;
//                 break;
//             case ITEM_NAME_CROSSBOW:
//                 temp_wpn_stats = {10, 0, 85, 0, 0, 0, 12, 15, 7, {1, 1}, {1}, 0, 1055, 0};
//                 temp_wpn = Weapon(ITEM_TYPE_BOW, temp_wpn_stats, ITEM_NAME_CROSSBOW);
//                 temp_wpn.setDescription("Powerful and easier to use than a bow, although quite heavy.");
//                 wpn_out[ITEM_NAME_CROSSBOW] = temp_wpn;
//                 break;
//             case ITEM_NAME_SHORT_BOW:
//                 temp_wpn_stats = {4, 0, 90, 0, 0, 0, 4, 35, 5, {1, 2}, {1}, 0, 400, 0};
//                 temp_wpn = Weapon(ITEM_TYPE_BOW, temp_wpn_stats, ITEM_NAME_SHORT_BOW);
//                 temp_wpn.setDescription("Simple and light bow.");
//                 wpn_out[ITEM_NAME_SHORT_BOW] = temp_wpn;
//                 break;
//             case ITEM_NAME_LONG_BOW:
//                 temp_wpn_stats = {8, 0, 70, 0, 0, 0, 10, 25, 11, {1, 3}, {1}, 0, 800, 0};
//                 temp_wpn = Weapon(ITEM_TYPE_BOW, temp_wpn_stats, ITEM_NAME_LONG_BOW);
//                 temp_wpn.setDescription("Long and heavy bow. Has increased range.");
//                 wpn_out[ITEM_NAME_LONG_BOW] = temp_wpn;
//                 break;
//             case ITEM_NAME_COMPOSITE_BOW:
//                 temp_wpn_stats = {7, 0, 80, 0, 0, 0, 5, 30, 10, {1, 1}, {1}, 0, 620, 0};
//                 temp_wpn = Weapon(ITEM_TYPE_BOW, temp_wpn_stats, ITEM_NAME_COMPOSITE_BOW);
//                 temp_wpn.setDescription("Bow made of laminated wood and animal horn. Very useful for horseback shooting.");
//                 wpn_out[ITEM_NAME_COMPOSITE_BOW] = temp_wpn;
//                 break;
//             case ITEM_NAME_RECURVE_CROSSBOW:
//                 temp_wpn_stats = {18, 0, 50, 0, 0, 0, 19, 20, 11, {1, 1}, {1}, 0, 760, 0};
//                 temp_wpn = Weapon(ITEM_TYPE_BOW, temp_wpn_stats, ITEM_NAME_RECURVE_CROSSBOW);
//                 temp_wpn.setDescription("Crossbow with recurve limbs. Heavy and powerful.");
//                 wpn_out[ITEM_NAME_RECURVE_CROSSBOW] = temp_wpn;
//                 break;
//             case ITEM_NAME_RECURVE_BOW:
//                 temp_wpn_stats = {13, 0, 75, 0, 0, 0, 10, 24, 14, {1, 1}, {1}, 0, 901, 0};
//                 temp_wpn = Weapon(ITEM_TYPE_BOW, temp_wpn_stats, ITEM_NAME_RECURVE_BOW);
//                 temp_wpn.setDescription("Bow whose limbs curve away when unstrung. Shoots faster and deadlier arrows.");
//                 wpn_out[ITEM_NAME_RECURVE_BOW] = temp_wpn;
//                 break;
//             case ITEM_NAME_APOLLO:
//                 temp_wpn_stats = {19, 0, 69, 0, 0, 0, 9, 25, 17, {1, 1}, {1}, 0, 8000, 0};
//                 temp_wpn = Weapon(ITEM_TYPE_BOW, temp_wpn_stats, ITEM_NAME_APOLLO);
//                 temp_wpn.setDescription("Bow crafted of sun rays, used by the most handsome ancient god.");
//                 wpn_out[ITEM_NAME_APOLLO] = temp_wpn;
//                 break;
//             case ITEM_NAME_ARTEMIS:
//                 temp_wpn_stats = {18, 0, 96, 0, 0, 0, 4, 26, 19, {1, 1}, {1}, 0, 7007, 0};
//                 temp_wpn = Weapon(ITEM_TYPE_BOW, temp_wpn_stats, ITEM_NAME_ARTEMIS);
//                 temp_wpn.setDescription("Bow of the ancient goddess of hunting.");
//                 wpn_out[ITEM_NAME_ARTEMIS] = temp_wpn;
//                 break;
//             case ITEM_NAME_PINAKA:
//                 temp_wpn_stats = {17, 0, 100, 0, 0, 0, 7, 35, 16, {1, 1}, {1}, 0, 8008, 0};
//                 temp_wpn = Weapon(ITEM_TYPE_BOW, temp_wpn_stats, ITEM_NAME_PINAKA);
//                 temp_wpn.setDescription("Bow of the god of death.");
//                 wpn_out[ITEM_NAME_PINAKA] = temp_wpn;
//                 break;

//             case ITEM_NAME_ADAPTED_SHIELD:
//                 temp_wpn_stats = {5, 0, 0, 10, 0, 5, 4, 45, 8, {1, 1}, {1}, 0, 3003, 0};
//                 temp_wpn = Weapon(ITEM_TYPE_SHIELD, temp_wpn_stats, ITEM_NAME_ADAPTED_SHIELD);
//                 temp_wpn.setDescription("Shield adapted to be used without one\'s weak hand. Straps on the forearm. 1H assassin only.");
//                 wpn_out[ITEM_NAME_ADAPTED_SHIELD] = temp_wpn;
//                 break;
//             case ITEM_NAME_POT_LID:
//                 temp_wpn_stats = {1, 0, 0, 5, 0, 1, 2, 15, 2, {1, 1}, {1}, 0, 111, 0};
//                 temp_wpn = Weapon(ITEM_TYPE_SHIELD, temp_wpn_stats, ITEM_NAME_POT_LID);
//                 temp_wpn.setDescription("Useful in the kitchen. For the poor militia man: life saving device.");
//                 wpn_out[ITEM_NAME_POT_LID] = temp_wpn;
//                 break;
//             case ITEM_NAME_IRON_SWORD:
//                 temp_wpn_stats = {2, 0, 0, 8, 0, 0, 3, 20, 7, {1, 1}, {1}, 0, 222, 0};
//                 temp_wpn = Weapon(ITEM_TYPE_SHIELD, temp_wpn_stats, ITEM_NAME_IRON_SWORD);
//                 temp_wpn.setDescription("Simple wood plank with a strap. Will probably save your life.");
//                 wpn_out[ITEM_NAME_IRON_SWORD] = temp_wpn;
//                 break;
//             case ITEM_NAME_CLOAK:
//                 temp_wpn_stats = {4, 0, 0, 10, 0, 4, 5, 0, 13, {1, 1}, {1}, 0, 10, 0};
//                 temp_wpn = Weapon(ITEM_TYPE_SHIELD, temp_wpn_stats, ITEM_NAME_CLOAK);
//                 temp_wpn.setDescription("The Mousquetaire\"s offhand shield in time of need.");
//                 wpn_out[ITEM_NAME_CLOAK] = temp_wpn;
//                 break;
//             case ITEM_NAME_LEATHER_SHIELD:
//                 temp_wpn_stats = {3, 0, 0, 9, 0, 2, 4, 25, 9, {1, 1}, {1}, 0, 440, 0};
//                 temp_wpn = Weapon(ITEM_TYPE_SHIELD, temp_wpn_stats, ITEM_NAME_LEATHER_SHIELD);
//                 temp_wpn.setDescription("Wooden plank lined with leather. Smooth and slippery. Cheap way to redirect weapons away from the body.");
//                 wpn_out[ITEM_NAME_LEATHER_SHIELD] = temp_wpn;
//                 break;
//             case ITEM_NAME_KITE_SHIELD:
//                 temp_wpn_stats = {8, 0, 0, 15, 0, 4, 4, 35, 12, {1, 1}, {1}, 0, 810, 0};
//                 temp_wpn = Weapon(ITEM_TYPE_SHIELD, temp_wpn_stats, ITEM_NAME_KITE_SHIELD);
//                 temp_wpn.setDescription("Large wooden shield of recognizable shape, to increase protection.");
//                 wpn_out[ITEM_NAME_KITE_SHIELD] = temp_wpn;
//                 break;
//             case ITEM_NAME_IRON_BUCKLER:
//                 temp_wpn_stats = {6, 0, 0, 6, 0, 6, 6, 36, 6, {1, 1}, {1}, 0, 630, 0};
//                 temp_wpn = Weapon(ITEM_TYPE_SHIELD, temp_wpn_stats, ITEM_NAME_IRON_BUCKLER);
//                 temp_wpn.setDescription("Small round iron shield. Light, resistant.");
//                 wpn_out[ITEM_NAME_IRON_BUCKLER] = temp_wpn;
//                 break;
//             case ITEM_NAME_STEEL_BUCKLER:
//                 temp_wpn_stats = {7, 0, 0, 7, 0, 7, 7, 37, 7, {1, 1}, {1}, 0, 940, 0};
//                 temp_wpn = Weapon(ITEM_TYPE_SHIELD, temp_wpn_stats, ITEM_NAME_STEEL_BUCKLER);
//                 temp_wpn.setDescription("Small round steel shield. Heavier and more resistant.");
//                 wpn_out[ITEM_NAME_STEEL_BUCKLER] = temp_wpn;
//                 break;
//             case ITEM_NAME_SPIKED_BUCKLER:
//                 temp_wpn_stats = {5, 0, 0, 5, 0, 5, 9, 30, 10, {1, 1}, {1}, 0, 1100, 0};
//                 temp_wpn = Weapon(ITEM_TYPE_SHIELD, temp_wpn_stats, ITEM_NAME_SPIKED_BUCKLER);
//                 temp_wpn.setDescription("Iron buckler with a prickly spike at its center. Damages attackers upon their attacks.");
//                 wpn_out[ITEM_NAME_SPIKED_BUCKLER] = temp_wpn;
//                 break;
//             case ITEM_NAME_SCUTUM:
//                 temp_wpn_stats = {11, 0, 0, 15, 0, 5, 13, 25, 7, {1, 1}, {1}, 0, 1440, 0};
//                 temp_wpn = Weapon(ITEM_TYPE_SHIELD, temp_wpn_stats, ITEM_NAME_SCUTUM);
//                 temp_wpn.setDescription("Very large and heavy shield.");
//                 wpn_out[ITEM_NAME_SCUTUM] = temp_wpn;
//                 break;
//             case ITEM_NAME_MASTER_SHIELD:
//                 temp_wpn_stats = {12, 0, 0, 20, 0, 10, 10, 0, 14, {1, 1}, {1}, 0, 80, 0};
//                 temp_wpn = Weapon(ITEM_TYPE_SHIELD, temp_wpn_stats, ITEM_NAME_MASTER_SHIELD);
//                 temp_wpn.setDescription("Shield of the princess of time.");
//                 wpn_out[ITEM_NAME_MASTER_SHIELD] = temp_wpn;
//                 break;
//             case ITEM_NAME_GODDESS_HAND:
//                 temp_wpn_stats = {9, 0, 0, 5, 0, 200, 9, 22, 13, {1, 1}, {1}, 0, 420, 0};
//                 temp_wpn = Weapon(ITEM_TYPE_SHIELD, temp_wpn_stats, ITEM_NAME_GODDESS_HAND);
//                 temp_wpn.setDescription("Offers the protection of the goddess. Negates enemy critical hits.");
//                 wpn_out[ITEM_NAME_GODDESS_HAND] = temp_wpn;
//                 break;
//             case ITEM_NAME_ANCILE:
//                 temp_wpn_stats = {15, 0, 0, 10, 0, 2, 13, 19, 12, {1, 1}, {1}, 0, 30000, 0};
//                 temp_wpn = Weapon(ITEM_TYPE_SHIELD, temp_wpn_stats, ITEM_NAME_ANCILE);
//                 temp_wpn.setDescription("Shield of the ancient god of war.");
//                 wpn_out[ITEM_NAME_ANCILE] = temp_wpn;
//                 break;
//             case ITEM_NAME_BOULDER:
//                 temp_wpn_stats = {15, 0, 0, 10, 0, 2, 13, 19, 12, {1, 1}, {1}, 0, 0, 0};
//                 temp_wpn = Weapon(ITEM_TYPE_SHIELD, temp_wpn_stats, ITEM_NAME_BOULDER);
//                 temp_wpn.setDescription("Almost a raw boulder. The powerful and dumb demon's favorite.");
//                 wpn_out[ITEM_NAME_BOULDER] = temp_wpn;
//                 break;
//             case ITEM_NAME_OBSIDIAN_SLAB:
//                 temp_wpn_stats = {15, 0, 0, 10, 0, 2, 13, 19, 12, {1, 1}, {1}, 0, 1, 0};
//                 temp_wpn = Weapon(ITEM_TYPE_SHIELD, temp_wpn_stats, ITEM_NAME_OBSIDIAN_SLAB);
//                 temp_wpn.setDescription("Rock melted by demon-breath, then recystallized into a more demon-claw friendly shape. Even harder than rock, but more brittle.");
//                 wpn_out[ITEM_NAME_OBSIDIAN_SLAB] = temp_wpn;
//                 break;
//             case ITEM_NAME_MARBLE_PLATE_SHIELD:
//                 temp_wpn_stats = {15, 0, 0, 10, 0, 2, 13, 19, 12, {1, 1}, {1}, 0, 3000, 0};
//                 temp_wpn = Weapon(ITEM_TYPE_SHIELD, temp_wpn_stats, ITEM_NAME_MARBLE_PLATE_SHIELD);
//                 temp_wpn.setDescription("Kite shield lined in an ornate layer of sculpted marble. Magical.");
//                 wpn_out[ITEM_NAME_MARBLE_PLATE_SHIELD] = temp_wpn;
//                 break;
//             case ITEM_NAME_DIVINE_PROTECTOR:
//                 temp_wpn_stats = {8, 8, 0, 10, 0, 2, 11, 22, 12, {1, 1}, {1}, 0, 4000, 0};
//                 temp_wpn = Weapon(ITEM_TYPE_SHIELD, temp_wpn_stats, ITEM_NAME_DIVINE_PROTECTOR);
//                 temp_wpn.setDescription("Large tower shield imbued with angelic properties. Surprinsgly light and effective against magic attacks.");
//                 wpn_out[ITEM_NAME_DIVINE_PROTECTOR] = temp_wpn;
//                 break;

//             case ITEM_NAME_WRISTBLADE:
//                 temp_wpn_stats = {2, 0, 10, 0, 10, 0, 5, 21, 16, {1, 1}, {1}, 0, 925, 0};
//                 temp_wpn = Weapon(ITEM_TYPE_OFFHAND, temp_wpn_stats, ITEM_NAME_WRISTBLADE);
//                 temp_wpn.setDescription("Blade generally strapped to the weak forearm. Used by assassins to increase crit potential.");
//                 wpn_out[ITEM_NAME_WRISTBLADE] = temp_wpn;
//                 break;
//             case ITEM_NAME_SHIELD_HOOK:
//                 temp_wpn_stats = {2, 0, 10, 0, 10, 0, 5, 21, 16, {1, 1}, {1}, 0, 830, 0};
//                 temp_wpn = Weapon(ITEM_TYPE_OFFHAND, temp_wpn_stats, ITEM_NAME_SHIELD_HOOK);
//                 temp_wpn.setDescription("Offhand weapon designed to grab on shields to drag them aside. Ignores shield bonuses.");
//                 temp_wpn.setEffect(ITEM_EFFECT_IGNORE_SHIELD);
//                 wpn_out[ITEM_NAME_SHIELD_HOOK] = temp_wpn;
//                 break;
//             case ITEM_NAME_RETRACTABLE_WRISTBLADE:
//                 temp_wpn_stats = {4, 0, 8, 0, 14, 0, 7, 16, 19, {1, 1}, {1}, 0, 1025, 0};
//                 temp_wpn = Weapon(ITEM_TYPE_OFFHAND, temp_wpn_stats, ITEM_NAME_RETRACTABLE_WRISTBLADE);
//                 temp_wpn.setDescription("Blade generally strapped to the weak forearm. Controlled by hand movements. Used by assasins to stealthily increase crit potential.");
//                 wpn_out[ITEM_NAME_RETRACTABLE_WRISTBLADE] = temp_wpn;
//                 break;
//             case ITEM_NAME_MAIN_GAUCHE:
//                 temp_wpn_stats = {1, 0, 0, 10, 0, 5, 4, 35, 7, {1, 1}, {1}, 0, 825, 0};
//                 temp_wpn = Weapon(ITEM_TYPE_OFFHAND, temp_wpn_stats, ITEM_NAME_MAIN_GAUCHE);
//                 temp_wpn.setDescription("Dagger designed to parry using the left/weak/off-hand. Increases avoir and favor.");
//                 wpn_out[ITEM_NAME_MAIN_GAUCHE] = temp_wpn;
//                 break;
//             case ITEM_NAME_SWORDBREAKER:
//                 temp_wpn_stats = {2, 0, 0, 22, 0, 4, 6, 28, 12, {1, 1}, {1}, 0, 612, 0};
//                 temp_wpn = Weapon(ITEM_TYPE_OFFHAND, temp_wpn_stats, ITEM_NAME_SWORDBREAKER);
//                 temp_wpn.setDescription("Dagger with slots designed to catch swords. Increases avoid and favor. Rarely does it actually break swords...");
//                 wpn_out[ITEM_NAME_SWORDBREAKER] = temp_wpn;
//                 break;
//             case ITEM_NAME_TRIDENT_DAGGER:
//                 temp_wpn_stats = {3, 0, 14, 0, 8, 0, 4, 20, 10, {1, 1}, {1}, 0, 724, 0};
//                 temp_wpn = Weapon(ITEM_TYPE_OFFHAND, temp_wpn_stats, ITEM_NAME_TRIDENT_DAGGER);
//                 temp_wpn.setDescription("Dagger whose blade splits into three prongs. Designed to catch opposing swords. Increases avoid and favor.");
//                 wpn_out[ITEM_NAME_TRIDENT_DAGGER] = temp_wpn;
//                 break;
//             case ITEM_NAME_TANTOU:
//                 temp_wpn_stats = {3, 0, 5, 0, 0, 0, 2, 26, 8, {1, 1}, {1}, 0, 852, 0};
//                 temp_wpn = Weapon(ITEM_TYPE_OFFHAND, temp_wpn_stats, ITEM_NAME_TANTOU);
//                 temp_wpn.setDescription("Eastern style short dagger. Very sharp and dangerous. Increases might.");
//                 wpn_out[ITEM_NAME_TANTOU] = temp_wpn;
//                 break;
//             case ITEM_NAME_KODACHI:
//                 temp_wpn_stats = {5, 0, 8, 0, 0, 0, 4, 24, 11, {1, 1}, {1}, 0, 992, 0};
//                 temp_wpn = Weapon(ITEM_TYPE_OFFHAND, temp_wpn_stats, ITEM_NAME_KODACHI);
//                 temp_wpn.setDescription("Dagger used by the legendary assasins of the East, the Ninja. Increases might.");
//                 wpn_out[ITEM_NAME_KODACHI] = temp_wpn;
//                 break;
//             case ITEM_NAME_DAGUE:
//                 temp_wpn_stats = {4, 0, 10, 0, 0, 0, 3, 20, 10, {1, 1}, {1}, 0, 880, 0};
//                 temp_wpn = Weapon(ITEM_TYPE_OFFHAND, temp_wpn_stats, ITEM_NAME_DAGUE);
//                 temp_wpn.setDescription("Messenic dagger. Simple and efficient. Beautiful engravings adorn the blade surface. ");
//                 wpn_out[ITEM_NAME_DAGUE] = temp_wpn;
//                 break;
//             case ITEM_NAME_WAKIZASHI:
//                 temp_wpn_stats = {7, 0, 0, 0, 0, 0, 8, 10, 15, {1, 1}, {1}, 0, 1300, 0};
//                 temp_wpn = Weapon(ITEM_TYPE_OFFHAND, temp_wpn_stats, ITEM_NAME_WAKIZASHI);
//                 temp_wpn.setDescription("Dagger of the warriors of the East. Companion of the uchigatana. Very deadly.");
//                 wpn_out[ITEM_NAME_WAKIZASHI] = temp_wpn;
//                 break;
//             case ITEM_NAME_BRAQUEMARD:
//                 temp_wpn_stats = {2, 0, 0, 0, 0, 0, 4, 15, 7, {1, 1}, {1}, 0, 2400, 0};
//                 temp_wpn = Weapon(ITEM_TYPE_OFFHAND, temp_wpn_stats, ITEM_NAME_BRAQUEMARD);
//                 temp_wpn.setDescription("Legendary Dagger.");
//                 wpn_out[ITEM_NAME_BRAQUEMARD] = temp_wpn;
//                 break;
//             case ITEM_NAME_MISERICORDE:
//                 temp_wpn_stats = {0, 0, 0, 0, 0, 0, 7, 30, 10, {1, 1}, {1}, 0, 1500, 0};
//                 temp_wpn = Weapon(ITEM_TYPE_OFFHAND, temp_wpn_stats, ITEM_NAME_MISERICORDE);
//                 temp_wpn.setDescription("Used to delivery the Coup de grace to wounded knights. Might of this dagger increases by one for each missing 3 health.");
//                 wpn_out[ITEM_NAME_MISERICORDE] = temp_wpn;
//                 break;
//             case ITEM_NAME_CARNWENNAN:
//                 temp_wpn_stats = {8, 0, 10, 10, 0, 2, 9, 24, 17, {1, 1}, {1}, 0, 3312, 0};
//                 temp_wpn = Weapon(ITEM_TYPE_OFFHAND, temp_wpn_stats, ITEM_NAME_CARNWENNAN);
//                 temp_wpn.setDescription("The king\'s dagger. Has the power to shroud its user in shadow.");
//                 wpn_out[ITEM_NAME_CARNWENNAN] = temp_wpn;
//                 break;
//             case ITEM_NAME_FLINTLOCK:
//                 temp_wpn_stats = {7, 0, 0, 0, 0, 0, 7, 8, 5, {1, 1}, {1}, 0, 10000, 0};
//                 temp_wpn = Weapon(ITEM_TYPE_OFFHAND, temp_wpn_stats, ITEM_NAME_FLINTLOCK);
//                 temp_wpn.setDescription("A gun!");
//                 wpn_out[ITEM_NAME_FLINTLOCK] = temp_wpn;
//                 break;
//             case ITEM_NAME_CIBO:
//                 temp_wpn_stats = {7, 0, 0, 0, 0, 0, 7, 8, 5, {1, 1}, {1}, 0, 10000, 0};
//                 temp_wpn = Weapon(ITEM_TYPE_OFFHAND, temp_wpn_stats, ITEM_NAME_FLINTLOCK);
//                 temp_wpn.setDescription("A gun!");
//                 wpn_out[ITEM_NAME_FLINTLOCK] = temp_wpn;
//                 break;

//             case ITEM_NAME_SHRUNKEN_HEAD:
//                 temp_wpn_stats = {7, 0, 0, 0, 0, 0, 7, 8, 5, {1, 1}, {1}, 0, 1000, 0};
//                 temp_wpn = Weapon(ITEM_TYPE_TRINKET, temp_wpn_stats, ITEM_NAME_SHRUNKEN_HEAD);
//                 temp_wpn.setDescription("Horrible. Infused with demonic energies.");
//                 wpn_out[ITEM_NAME_SHRUNKEN_HEAD] = temp_wpn;
//                 break;
//             case ITEM_NAME_CRYSTAL:
//                 temp_wpn_stats = {7, 0, 0, 0, 0, 0, 7, 8, 5, {1, 1}, {1}, 0, 1000, 0};
//                 temp_wpn = Weapon(ITEM_TYPE_TRINKET, temp_wpn_stats, ITEM_NAME_CRYSTAL);
//                 temp_wpn.setDescription("");
//                 wpn_out[ITEM_NAME_CRYSTAL] = temp_wpn;
//                 break;
//             case ITEM_NAME_GLASS_ORB:
//                 temp_wpn_stats = {7, 0, 0, 0, 0, 0, 7, 8, 5, {1, 1}, {1}, 0, 1000, 0};
//                 temp_wpn = Weapon(ITEM_TYPE_TRINKET, temp_wpn_stats, ITEM_NAME_GLASS_ORB);
//                 temp_wpn.setDescription("Orb used to focus magical power.");
//                 wpn_out[ITEM_NAME_GLASS_ORB] = temp_wpn;
//                 break;
//             case ITEM_NAME_HUMAN_FEMUR:
//                 temp_wpn_stats = {7, 0, 0, 0, 0, 0, 7, 8, 5, {1, 1}, {1}, 0, 1000, 0};
//                 temp_wpn = Weapon(ITEM_TYPE_TRINKET, temp_wpn_stats, ITEM_NAME_HUMAN_FEMUR);
//                 temp_wpn.setDescription("Taken from an ancient tomb. Used by demons to increase their magic power.");
//                 wpn_out[ITEM_NAME_HUMAN_FEMUR] = temp_wpn;
//                 break;
//             case ITEM_NAME_VOID_FRAGMENT:
//                 temp_wpn_stats = {7, 0, 0, 0, 0, 0, 7, 8, 5, {1, 1}, {1}, 0, 1000, 0};
//                 temp_wpn = Weapon(ITEM_TYPE_TRINKET, temp_wpn_stats, ITEM_NAME_VOID_FRAGMENT);
//                 temp_wpn.setDescription("");
//                 wpn_out[ITEM_NAME_VOID_FRAGMENT] = temp_wpn;
//                 break;
//             case ITEM_NAME_DEMON_FANG:
//                 temp_wpn_stats = {7, 0, 0, 0, 0, 0, 7, 8, 5, {1, 1}, {1}, 0, 1000, 0};
//                 temp_wpn = Weapon(ITEM_TYPE_TRINKET, temp_wpn_stats, ITEM_NAME_DEMON_FANG);
//                 temp_wpn.setDescription("Ripped from the mouth of a dead demon. Unusable by demons.");
//                 wpn_out[ITEM_NAME_DEMON_FANG] = temp_wpn;
//                 break;
//             case ITEM_NAME_ANGEL_FEATHER:
//                 temp_wpn_stats = {7, 0, 0, 0, 0, 0, 7, 8, 5, {1, 1}, {1}, 0, 1000, 0};
//                 temp_wpn = Weapon(ITEM_TYPE_TRINKET, temp_wpn_stats, ITEM_NAME_ANGEL_FEATHER);
//                 temp_wpn.setDescription("Beautiful and fluffy. Given by incarnates and possessed to the worthy. Increases magical ability. Unusable by demons and demon incarnates.");
//                 wpn_out[ITEM_NAME_ANGEL_FEATHER] = temp_wpn;
//                 break;
//             case ITEM_NAME_ANGEL_HAIR:
//                 temp_wpn_stats = {7, 0, 0, 0, 0, 0, 7, 8, 5, {1, 1}, {1}, 0, 1000, 0};
//                 temp_wpn = Weapon(ITEM_TYPE_TRINKET, temp_wpn_stats, ITEM_NAME_ANGEL_HAIR);
//                 temp_wpn.setDescription("Golden strands of angel hair held up by a silver thread. Increases magical ability. Unusable by demons and demon incarnates.");
//                 wpn_out[ITEM_NAME_ANGEL_HAIR] = temp_wpn;
//                 break;
//             case ITEM_NAME_VIAL_OF_LIGHT:
//                 temp_wpn_stats = {7, 0, 0, 0, 0, 0, 7, 8, 5, {1, 1}, {1}, 0, 1000, 0};
//                 temp_wpn = Weapon(ITEM_TYPE_TRINKET, temp_wpn_stats, ITEM_NAME_VIAL_OF_LIGHT);
//                 temp_wpn.setDescription("Light magically contained in glass. Shines brightest in darkness.");
//                 wpn_out[ITEM_NAME_VIAL_OF_LIGHT] = temp_wpn;
//                 break;
//             case ITEM_NAME_MAIDENS_HANDKERCHIEF:
//                 break;

//             case ITEM_NAME_WAR_HORN:
//                 temp_wpn_stats = {7, 0, 0, 0, 0, 0, 7, 8, 5, {1, 1}, {1}, 0, 1000, 0};
//                 temp_wpn = Weapon(ITEM_TYPE_TRINKET, temp_wpn_stats, ITEM_NAME_WAR_HORN);
//                 temp_wpn.setDescription("Used to call for battle. Raise's close allies' combat abilities.");
//                 wpn_out[ITEM_NAME_WAR_HORN] = temp_wpn;
//                 break;

//             case ITEM_NAME_BALL_LIGHTNING:
//                 temp_wpn_stats = {0, 12, 76, 0, 0, 0, 11, 25, 7, {1, 2}, {1, 2}, 1, 1000, 0};
//                 temp_wpn = Weapon(ITEM_TYPE_ELEMENTAL, temp_wpn_stats, ITEM_NAME_BALL_LIGHTNING);
//                 temp_wpn.setDescription("Lightning, compressed and explosive.");
//                 wpn_out[ITEM_NAME_BALL_LIGHTNING] = temp_wpn;
//                 break;
//             case ITEM_NAME_ICICLE:
//                 break;
//                 temp_wpn_stats = {0, 6, 70, 0, 0, 0, 5, 34, 7, {1, 2}, {1, 2}, 1, 1000, 0};
//                 temp_wpn = Weapon(ITEM_TYPE_ELEMENTAL, temp_wpn_stats, ITEM_NAME_ICICLE);
//                 temp_wpn.setDescription("Summons sharp and cold death spikes.");
//                 wpn_out[ITEM_NAME_ICICLE] = temp_wpn;

//             case ITEM_NAME_VOLCANO:
//                 temp_wpn_stats = {0, 14, 50, 0, 0, 0, 13, 20, 7, {1, 2}, {1, 2}, 1, 1000, 0};
//                 temp_wpn = Weapon(ITEM_TYPE_ELEMENTAL, temp_wpn_stats, ITEM_NAME_VOLCANO);
//                 temp_wpn.setDescription("Summons magma spewing mountains from under enemies' feets.");
//                 wpn_out[ITEM_NAME_VOLCANO] = temp_wpn;
//                 break;
//             case ITEM_NAME_WIND_SPEAR:
//                 temp_wpn_stats = {0, 4, 80, 0, 0, 0, 3, 40, 7, {1, 2}, {1, 2}, 1, 1000, 0};
//                 temp_wpn = Weapon(ITEM_TYPE_ELEMENTAL, temp_wpn_stats, ITEM_NAME_WIND_SPEAR);
//                 temp_wpn.setDescription("Invisible, swift and minuscule. Can pierce armor.");
//                 wpn_out[ITEM_NAME_WIND_SPEAR] = temp_wpn;
//                 break;
//             case ITEM_NAME_EMBER:
//                 temp_wpn_stats = {0, 6, 60, 0, 0, 0, 8, 30, 7, {1, 2}, {1, 2}, 1, 1000, 0};
//                 temp_wpn = Weapon(ITEM_TYPE_ELEMENTAL, temp_wpn_stats, ITEM_NAME_EMBER);
//                 temp_wpn.setDescription("Summon smoldering embers.");
//                 wpn_out[ITEM_NAME_EMBER] = temp_wpn;
//                 break;
//             case ITEM_NAME_TSUNAMI:
//                 temp_wpn_stats = {0, 13, 40, 0, 0, 0, 11, 25, 7, {1, 2}, {1, 2}, 1, 1000, 0};
//                 temp_wpn = Weapon(ITEM_TYPE_ELEMENTAL, temp_wpn_stats, ITEM_NAME_TSUNAMI);
//                 temp_wpn.setDescription("");
//                 wpn_out[ITEM_NAME_TSUNAMI] = temp_wpn;
//                 break;
//             case ITEM_NAME_GOD_FORGE_FIRE:
//                 temp_wpn_stats = {0, 18, 50, 0, 0, 0, 11, 15, 7, {1, 2}, {1, 2}, 1, 1000, 0};
//                 temp_wpn = Weapon(ITEM_TYPE_ELEMENTAL, temp_wpn_stats, ITEM_NAME_GOD_FORGE_FIRE);
//                 temp_wpn.setDescription("The fire used to forge Secundus, contained magically. Can melt any and all metals.");
//                 wpn_out[ITEM_NAME_GOD_FORGE_FIRE] = temp_wpn;
//                 break;
//             case ITEM_NAME_FINGOLFIN:
//                 temp_wpn_stats = {0, 11, 50, 0, 0, 0, 9, 20, 7, {1, 2}, {1, 2}, 1, 1000, 0};
//                 temp_wpn = Weapon(ITEM_TYPE_ELEMENTAL, temp_wpn_stats, ITEM_NAME_FINGOLFIN);
//                 temp_wpn.setDescription("Distilled courage. Strikes at 7 times at darkness's heart. Effective against demons.");
//                 temp_wpn.setEffective(UNIT_TYPE_DEMON);
//                 wpn_out[ITEM_NAME_FINGOLFIN] = temp_wpn;
//                 break;
//             case ITEM_NAME_MITHRANDIR:
//                 temp_wpn_stats = {0, 6, 65, 0, 0, 0, 5, 28, 7, {1, 2}, {1, 2}, 1, 1000, 0};
//                 temp_wpn = Weapon(ITEM_TYPE_ELEMENTAL, temp_wpn_stats, ITEM_NAME_MITHRANDIR);
//                 temp_wpn.setDescription("Inspires courage in the hearts of men. Effective against demons.");
//                 temp_wpn.setEffective(UNIT_TYPE_DEMON);
//                 wpn_out[ITEM_NAME_MITHRANDIR] = temp_wpn;
//                 break;

//             case ITEM_NAME_VOIDD:
//                 temp_wpn_stats = {0, 5, 70, 0, 0, 0, 5, 60, 7, {1, 2}, {1, 2}, 1, 1000, 0};
//                 temp_wpn = Weapon(ITEM_TYPE_DEMONIC, temp_wpn_stats, ITEM_NAME_VOIDD);
//                 temp_wpn.setDescription("");
//                 wpn_out[ITEM_NAME_VOIDD] = temp_wpn;
//                 break;
//             case ITEM_NAME_NETHER:
//                 temp_wpn_stats = {0, 5, 70, 0, 0, 0, 5, 60, 7, {1, 2}, {1, 2}, 1, 1000, 0};
//                 temp_wpn = Weapon(ITEM_TYPE_DEMONIC, temp_wpn_stats, ITEM_NAME_NETHER);
//                 temp_wpn.setDescription("");
//                 wpn_out[ITEM_NAME_NETHER] = temp_wpn;
//                 break;
//             case ITEM_NAME_DOWNFALL:
//                 temp_wpn = Weapon(ITEM_TYPE_DEMONIC, temp_wpn_stats, ITEM_NAME_DOWNFALL);
//                 temp_wpn.setDescription("");
//                 temp_wpn.setDescription("");
//                 wpn_out[ITEM_NAME_DOWNFALL] = temp_wpn;
//                 break;

//             case ITEM_NAME_CONSUME:
//                 temp_wpn_stats = {0, 5, 70, 0, 0, 0, 5, 60, 7, {1, 2}, {1, 2}, 1, 1000, 0};
//                 temp_wpn = Weapon(ITEM_TYPE_DEMONIC, temp_wpn_stats, ITEM_NAME_CONSUME);
//                 temp_wpn.setDescription("");
//                 wpn_out[ITEM_NAME_CONSUME] = temp_wpn;
//                 break;
//             case ITEM_NAME_PUTRIFY:
//                 temp_wpn_stats = {0, 5, 70, 0, 0, 0, 5, 60, 7, {1, 2}, {1, 2}, 1, 1000, 0};
//                 temp_wpn = Weapon(ITEM_TYPE_DEMONIC, temp_wpn_stats, ITEM_NAME_PUTRIFY);
//                 temp_wpn.setDescription("Brings out the inner rot.");
//                 wpn_out[ITEM_NAME_PUTRIFY] = temp_wpn;
//                 break;
//             case ITEM_NAME_GLAURUNG:
//                 temp_wpn_stats = {0, 5, 70, 0, 0, 0, 5, 60, 7, {1, 2}, {1, 2}, 1, 1000, 0};
//                 temp_wpn = Weapon(ITEM_TYPE_DEMONIC, temp_wpn_stats, ITEM_NAME_GLAURUNG);
//                 temp_wpn.setDescription("");
//                 wpn_out[ITEM_NAME_GLAURUNG] = temp_wpn;
//                 break;
//             case ITEM_NAME_MORGOTH:
//                 temp_wpn_stats = {0, 5, 70, 0, 0, 0, 5, 60, 7, {1, 2}, {1, 2}, 1, 1000, 0};
//                 temp_wpn = Weapon(ITEM_TYPE_DEMONIC, temp_wpn_stats, ITEM_NAME_MORGOTH);
//                 temp_wpn.setDescription("Dark power, in the shape of a massive dark hammer. Makes craters wherever it strikes.");
//                 wpn_out[ITEM_NAME_MORGOTH] = temp_wpn;
//                 temp_wpn_stats = {0, 5, 70, 0, 0, 0, 5, 60, 7, {1, 2}, {1, 2}, 1, 1000, 0};
//                 break;
//             case ITEM_NAME_GOTHMOG:
//                 temp_wpn = Weapon(ITEM_TYPE_DEMONIC, temp_wpn_stats, ITEM_NAME_GOTHMOG);
//                 temp_wpn.setDescription("Infernal whip of flame and shadows.");
//                 wpn_out[ITEM_NAME_GOTHMOG] = temp_wpn;
//                 break;

//             case ITEM_NAME_FATALIS:
//                 temp_wpn_stats = {0, 5, 70, 0, 0, 0, 5, 60, 7, {1, 2}, {1, 2}, 1, 1000, 0};
//                 temp_wpn = Weapon(ITEM_TYPE_DEMONIC, temp_wpn_stats, ITEM_NAME_FATALIS);
//                 temp_wpn.setDescription("");
//                 wpn_out[ITEM_NAME_FATALIS] = temp_wpn;
//                 break;
//         }
//     }

//     return (wpn_out);
// }

// void baseWeapons() {
//     SDL_Log("Making all base weapons");
//     std_vector<short int> toload;

//     for (short int i = ITEM_NAME_KITCHEN_KNIFE; i < ITEM_NAME_FATALIS; i++) {
//         toload.push_back(i);
//     }

//     all_weapons = baseWeapons(toload);
// }


void test_weapon() {
    SDL_Log("test_weapon");
    SDL_Log("Init weapons");
    struct Weapon wpn1 = Weapon_default;
    struct Item item1 = Item_default;
    struct Weapon wpn2 = Weapon_default;
    struct Item item2 = Item_default;
    struct Weapon wpn3 = Weapon_default;
    struct Item item3 = Item_default;
    wpn1.item = malloc(sizeof(struct Item));
    wpn2.item = malloc(sizeof(struct Item));
    wpn3.item = malloc(sizeof(struct Item));
    *wpn1.item = Item_default;
    *wpn2.item = Item_default;
    *wpn3.item = Item_default;
    struct Weapon_stats in_wpn_stats = {
        .Pmight = 3,
        .Mmight =  0,
        .combat.hit = 80,
        .combat.crit = 0,
        .combat.dodge =  0,
        .combat.favor = 0,
        .wgt = 3,
        .uses = 30,
        .range = {2, 1},
        .hand = {1, 1},
        .dmg_type = 2,
    };
    struct Weapon_stats out_wpn_stats;
    struct Unit_stats in_stats = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11};
    struct Unit_stats out_stats;
    struct Infusion out_infusion;
    int16_t infuse_power = 5;
    uint16_t infuse_type = ITEM_TYPE_ELEMENTAL;
    uint64_t in_effect;
    bool in_sellable = false;
    uint16_t in_type = ITEM_TYPE_SWORD + ITEM_TYPE_LANCE;

    in_effect = ITEM_EFFECT_KILL1P + ITEM_EFFECT_BRAVE2X + ITEM_EFFECT_BREAK_SHIELD;
    SDL_Log("Making item 1");
    wpn1.item->type = in_type;
    SDL_Log("Item 1 wpn_stats");
    wpn1.stats = in_wpn_stats;
    SDL_Log("Item 1 id");
    wpn1.item->id = ITEM_NAME_WOODEN_SWORD;
    SDL_Log("Item 1 name");
    wpn1.item->name = "Wooden Sword";
    SDL_Log("Item 1 description");
    char * in_description = "Practice sword, made of wood. It's crushing blows are still deadly.";
    char * out_description;
    wpn1.item->description = in_description;
    wpn1.item->passive_effect = in_effect;
    wpn1.item->bonus_stats = in_stats;
    wpn1.item->malus_stats = in_stats;
    wpn1.item->sellable = in_sellable;
    SDL_Log("Infuse Weapon");
    Weapon_Infuse(&wpn1, infuse_power, infuse_type);
    out_wpn_stats = wpn1.stats;
    SDL_Log("Starting tests");
    lok(in_wpn_stats.Pmight == out_wpn_stats.Pmight);
    lok(in_wpn_stats.Mmight == out_wpn_stats.Mmight);
    lok(in_wpn_stats.combat.hit == out_wpn_stats.combat.hit);
    lok(in_wpn_stats.combat.crit == out_wpn_stats.combat.crit);
    lok(in_wpn_stats.combat.dodge == out_wpn_stats.combat.dodge);
    lok(in_wpn_stats.combat.favor == out_wpn_stats.combat.favor);
    lok(in_wpn_stats.wgt == out_wpn_stats.wgt);
    lok(in_wpn_stats.uses == out_wpn_stats.uses);
    lok(in_wpn_stats.prof == out_wpn_stats.prof);
    lok(in_wpn_stats.range[0] == out_wpn_stats.range[0]);
    lok(in_wpn_stats.range[1] == out_wpn_stats.range[1]);
    lok(in_wpn_stats.hand[0] == out_wpn_stats.hand[0]);
    lok(in_wpn_stats.hand[1] == out_wpn_stats.hand[1]);
    lok(in_wpn_stats.dmg_type == out_wpn_stats.dmg_type);
    out_description = wpn1.item->description;
    lok(strcmp(in_description, out_description) == 0);
    lok(strcmp(wpn1.item->name, "Wooden Sword") == 0);
    lok(wpn1.item->passive_effect == in_effect);
    lok(wpn1.item->type == in_type);
    lok(wpn1.item->sellable == in_sellable);
    out_infusion = wpn1.infused;
    lok(out_infusion.power == infuse_power);
    lok(out_infusion.type == infuse_type);
    SDL_Log("Infusion: %d", out_infusion.power);
    SDL_Log("Infusion: %d", wpn1.infused.power);
    out_stats = wpn1.item->bonus_stats;
    lok(in_stats.hp == out_stats.hp);
    lok(in_stats.str == out_stats.str);
    lok(in_stats.mag == out_stats.mag);
    lok(in_stats.agi == out_stats.agi);
    lok(in_stats.dex == out_stats.dex);
    lok(in_stats.luck == out_stats.luck);
    lok(in_stats.def == out_stats.def);
    lok(in_stats.res == out_stats.res);
    lok(in_stats.con == out_stats.con);
    lok(in_stats.move == out_stats.move);
    lok(in_stats.prof == out_stats.prof);
    out_stats = wpn1.item->malus_stats;
    lok(in_stats.hp == out_stats.hp);
    lok(in_stats.str == out_stats.str);
    lok(in_stats.mag == out_stats.mag);
    lok(in_stats.agi == out_stats.agi);
    lok(in_stats.dex == out_stats.dex);
    lok(in_stats.luck == out_stats.luck);
    lok(in_stats.def == out_stats.def);
    lok(in_stats.res == out_stats.res);
    lok(in_stats.con == out_stats.con);
    lok(in_stats.move == out_stats.move);
    lok(in_stats.prof == out_stats.prof);
    if (PHYSFS_stat("saves", NULL) == 0) {
        PHYSFS_mkdir("saves");
    }

    PHYSFS_mount("saves", NULL, 1);
    writeJSON("saves"PHYSFS_SEPARATOR"weapon_test.json", &wpn1, false);
    readJSON("saves"PHYSFS_SEPARATOR"weapon_test.json", &wpn3);
    out_wpn_stats = wpn3.stats;
    lok(in_wpn_stats.Pmight == out_wpn_stats.Pmight);
    lok(in_wpn_stats.Mmight == out_wpn_stats.Mmight);
    lok(in_wpn_stats.combat.hit == out_wpn_stats.combat.hit);
    lok(in_wpn_stats.combat.crit == out_wpn_stats.combat.crit);
    lok(in_wpn_stats.combat.dodge == out_wpn_stats.combat.dodge);
    lok(in_wpn_stats.combat.favor == out_wpn_stats.combat.favor);
    lok(in_wpn_stats.wgt == out_wpn_stats.wgt);
    lok(in_wpn_stats.uses == out_wpn_stats.uses);
    lok(in_wpn_stats.prof == out_wpn_stats.prof);
    lok(in_wpn_stats.range[0] == out_wpn_stats.range[0]);
    lok(in_wpn_stats.range[1] == out_wpn_stats.range[1]);
    lok(in_wpn_stats.hand[0] == out_wpn_stats.hand[0]);
    lok(in_wpn_stats.hand[1] == out_wpn_stats.hand[1]);
    lok(in_wpn_stats.dmg_type == out_wpn_stats.dmg_type);
    out_description = wpn3.item->description;
    lok(strcmp(in_description, out_description) == 0);
    lok(strcmp(wpn3.item->name, "Wooden Sword") == 0);
    lok(wpn3.item->passive_effect == in_effect);
    lok(wpn3.item->type == in_type);
    lok(wpn3.item->sellable == in_sellable);
    out_infusion = wpn3.infused;
    lok(out_infusion.power == infuse_power);
    lok(out_infusion.type == infuse_type);
    out_stats = wpn3.item->bonus_stats;
    lok(in_stats.hp == out_stats.hp);
    lok(in_stats.str == out_stats.str);
    lok(in_stats.mag == out_stats.mag);
    lok(in_stats.agi == out_stats.agi);
    lok(in_stats.dex == out_stats.dex);
    lok(in_stats.luck == out_stats.luck);
    lok(in_stats.def == out_stats.def);
    lok(in_stats.res == out_stats.res);
    lok(in_stats.con == out_stats.con);
    lok(in_stats.move == out_stats.move);
    lok(in_stats.prof == out_stats.prof);
    out_stats = wpn3.item->malus_stats;
    lok(in_stats.hp == out_stats.hp);
    lok(in_stats.str == out_stats.str);
    lok(in_stats.mag == out_stats.mag);
    lok(in_stats.agi == out_stats.agi);
    lok(in_stats.dex == out_stats.dex);
    lok(in_stats.luck == out_stats.luck);
    lok(in_stats.def == out_stats.def);
    lok(in_stats.res == out_stats.res);
    lok(in_stats.con == out_stats.con);
    lok(in_stats.move == out_stats.move);
    lok(in_stats.prof == out_stats.prof);
    writeJSON("saves"PHYSFS_SEPARATOR"weapon_rewrite.json", &wpn3, false);
    lok(fequal("saves"PHYSFS_SEPARATOR"weapon_test.json", "saves"PHYSFS_SEPARATOR"weapon_rewrite.json"));
    out_wpn_stats = wpn3.stats;
    lok(in_wpn_stats.Pmight == out_wpn_stats.Pmight);
    lok(in_wpn_stats.Mmight == out_wpn_stats.Mmight);
    lok(in_wpn_stats.combat.hit == out_wpn_stats.combat.hit);
    lok(in_wpn_stats.combat.crit == out_wpn_stats.combat.crit);
    lok(in_wpn_stats.combat.dodge == out_wpn_stats.combat.dodge);
    lok(in_wpn_stats.combat.favor == out_wpn_stats.combat.favor);
    lok(in_wpn_stats.wgt == out_wpn_stats.wgt);
    lok(in_wpn_stats.uses == out_wpn_stats.uses);
    lok(in_wpn_stats.prof == out_wpn_stats.prof);
    lok(in_wpn_stats.range[0] == out_wpn_stats.range[0]);
    lok(in_wpn_stats.range[1] == out_wpn_stats.range[1]);
    lok(in_wpn_stats.hand[0] == out_wpn_stats.hand[0]);
    lok(in_wpn_stats.hand[1] == out_wpn_stats.hand[1]);
    lok(in_wpn_stats.dmg_type == out_wpn_stats.dmg_type);
    out_description = wpn3.item->description;
    lok(strcmp(in_description, out_description) == 0);
    lok(strcmp(wpn3.item->name, "Wooden Sword") == 0);
    lok(wpn3.item->passive_effect == in_effect);
    lok(wpn3.item->type == in_type);
    lok(wpn3.item->sellable == in_sellable);
    out_infusion = wpn3.infused;
    lok(out_infusion.power == infuse_power);
    lok(out_infusion.type == infuse_type);
    out_stats = wpn3.item->bonus_stats;
    lok(in_stats.hp == out_stats.hp);
    lok(in_stats.str == out_stats.str);
    lok(in_stats.mag == out_stats.mag);
    lok(in_stats.agi == out_stats.agi);
    lok(in_stats.dex == out_stats.dex);
    lok(in_stats.luck == out_stats.luck);
    lok(in_stats.def == out_stats.def);
    lok(in_stats.res == out_stats.res);
    lok(in_stats.con == out_stats.con);
    lok(in_stats.move == out_stats.move);
    lok(in_stats.prof == out_stats.prof);
    out_stats = wpn3.item->malus_stats;
    lok(in_stats.hp == out_stats.hp);
    lok(in_stats.str == out_stats.str);
    lok(in_stats.mag == out_stats.mag);
    lok(in_stats.agi == out_stats.agi);
    lok(in_stats.dex == out_stats.dex);
    lok(in_stats.luck == out_stats.luck);
    lok(in_stats.def == out_stats.def);
    lok(in_stats.res == out_stats.res);
    lok(in_stats.con == out_stats.con);
    lok(in_stats.move == out_stats.move);
    lok(in_stats.prof == out_stats.prof);
    writeJSON("saves"PHYSFS_SEPARATOR"weapon_rewrite.json", &wpn3, false);
    lok(fequal("saves"PHYSFS_SEPARATOR"weapon_test.json", "saves"PHYSFS_SEPARATOR"weapon_rewrite.json"));
}
