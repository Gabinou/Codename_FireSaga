// Code créé par Gabriel Taillon
// Note on standards. Fuck all that complicated bullcrap. I'm writing for c++11. More convenient and the rest seems like bullshit.
/*! \page handle main.cpp
* \file main.cpp 
* \brief Main game algorithm page.
*/
using namespace std;
#include "shared.hpp"
#include "generic.hpp"
#include "weapon.hpp"
#include "unit.hpp"
#include "unit_class.hpp"
#include "inventory_item.hpp"

// Skill idea: divine shield! Bubble!

main() {
    printf("TESTING THIS BITCH\n");
    wpn_indexes["SWORD"] = 0;
    wpn_indexes["Sword"] = 0;
    wpn_indexes["sword"] = 0;
    wpn_indexes["swd"] = 0;
    wpn_indexes["SWD"] = 0;
    wpn_indexes["LANCE"] = 1;
    wpn_indexes["Lance"] = 1;
    wpn_indexes["lance"] = 1;
    wpn_indexes["lnc"] = 1;
    wpn_indexes["LNC"] = 1;
    wpn_indexes["AXE"] = 2;
    wpn_indexes["Axe"] = 2;
    wpn_indexes["axe"] = 2;
    wpn_indexes["Ax"] = 2;
    wpn_indexes["AX"] = 2;
    wpn_indexes["ax"] = 2;
    wpn_indexes["bow"] = 3;
    wpn_indexes["Bow"] = 3;
    wpn_indexes["BOW"] = 3;
    wpn_indexes["WIND"] = 4;
    wpn_indexes["Wind"] = 4;
    wpn_indexes["wind"] = 4;
    wpn_indexes["wnd"] = 4;
    wpn_indexes["WND"] = 4;
    wpn_indexes["Fire"] = 5;
    wpn_indexes["FIRE"] = 5;
    wpn_indexes["fire"] = 5;
    wpn_indexes["fir"] = 5;
    wpn_indexes["FIRE"] = 5;
    wpn_indexes["Thunder"] = 6;
    wpn_indexes["THUNDER"] = 6;
    wpn_indexes["thunder"] = 6;
    wpn_indexes["tnd"] = 6;
    wpn_indexes["TND"] = 6;
    wpn_indexes["Tnd"] = 6;
    wpn_indexes["Thnd"] = 6;
    wpn_indexes["thnd"] = 6;
    wpn_indexes["THND"] = 6;
    wpn_indexes["Dark"] = 7;
    wpn_indexes["dark"] = 7;
    wpn_indexes["DARK"] = 7;
    wpn_indexes["drk"] = 7;
    wpn_indexes["DRK"] = 7;
    wpn_indexes["Drk"] = 7;
    wpn_indexes["Light"] = 8;
    wpn_indexes["light"] = 8;
    wpn_indexes["LIGHT"] = 8;
    wpn_indexes["lgt"] = 8;
    wpn_indexes["LGT"] = 8;
    wpn_indexes["Lgt"] = 8;
    wpn_indexes["staff"] = 9;
    wpn_indexes["Staff"] = 9;
    wpn_indexes["STAFF"] = 9;
    wpn_indexes["stf"] = 9;
    wpn_indexes["STF"] = 9;
    wpn_indexes["Stf"] = 9;
    read_all_weapons("weapons.txt");
    read_all_units("units_FE1.txt");
    write_all_units("units_test.txt");
    
    // inventory_item("Rapier", 10);
    // printf("%d \f", inventory_items["Rapier_0000"]);
    // write_all_weapons("weapons2.txt");
    
    // all_unit_classes["Lord"] = unit_class("Lord", 0, 0, 0, 0, {40, 25, 20, 40, 70, 80, 20, 20,  0,  0, 0}, {""}, {"Sword"});
    // // all_unit_classes["Mercenary"] = unit_class("Mercenary", 0, 0, 0, 0, {40, 25, 20, 40, 70, 80, 20, 20,  0,  0}, {"Hero"});
    // all_unit_classes["Mercenary"] = unit_class();
    // all_unit_classes["Mercenary"].read("classes_FE1.txt", 0);
    // // all_unit_classes["Lord"] = unit_class();
    // // unit_class();
    // // all_unit_classes["Lord"].write("classes.txt");
    
    
    // read_all_unit_classes("classes_FE1.txt");
    // write_all_unit_classes("classes.txt");
    
    //// TEST FOR SIZE OF DATA.
    // std::unordered_map<string, std::vector<char>> testsupport({ {"Marth", {1,1,1,1,1,1,1,1,1,1,1}} });
    // string a = "Marth";
    // std::vector<char> b = {1,1,1,1,1,1,1,1,1,1,1};
    // char c[11] = {1,1,1,1,1,1,1,1,1,1,1};
    // char d[14] = "Marth";
    // printf("%d \n", sizeof(a));
    // printf("%d \n", sizeof(b));
    // printf("%d \n", sizeof(c));
    // printf("%d \n", sizeof(d));
    // printf("%d \n", sizeof(testsupport));
    
    
    
    // all_weapons["Rapier"] = weapon("Rapier", "swd", id++, 600,
            // // dmg  hit  crt wght uses  exp
              // {5,  90,  10,   7,  30,   2},
              // {1, 1}, // range
              // std::vector<char>(LEN(unit_stats), 0), {"Knight", "Cavalier"}, 0, 1); 
    // all_weapons["Bronze Sword"] = weapon("Bronze Sword", "swd", id++, 450,
           // // dmg  hit  crt wght uses  exp
              // {3,  80,   0,   5,  45,   1},
              // {1, 1}, // range
              // std::vector<char>(LEN(unit_stats), 0), {"", ""}, 0, 1);
    // all_weapons["Iron Sword"] =  weapon("Iron Sword", "swd", id++, 450,
           // // dmg hit  crt wght uses  exp
              // {5,  80,    0,   7,  45,  1},
              // {1, 1}, // range              
              // std::vector<char>(LEN(unit_stats), 0), {"", ""}, 0, 1);
    // all_weapons["Iron Lance"] = weapon("Iron Lance", "lance", id++, 450,
           // // dmg hit  crt wght uses  exp
           // // dmg hit  crt wght uses  exp
              // {6,  80,   0,   8,   40,  1},
              // {1, 1}, // range
              // std::vector<char>(LEN(unit_stats), 0), {"", ""}, 0, 1);
    // all_weapons["Steel Sword"] = weapon("Steel Sword", "swd", id++, 500,
           // // dmg hit  crt wght uses  exp
              // {8,  70,   0,   9,   35,  1},
              // {1, 1}, // range              
              // std::vector<char>(LEN(unit_stats), 0), {"", ""}, 0, 1);
    // all_weapons["Lame de Damas"] = weapon("Lame de Damas", "swd", id++, 1000,
            // // Other names: Acier de Damas. Damas Sword. Damascus Sword. Damas Sword. Damas steel sword.
            // // dmg hit  crt wght uses  exp
              // {15,  65,  0,   8,  25,   1},
              // {1, 1}, // range
              // std::vector<char>(LEN(unit_stats), 0), {"", ""}, 0, 3);
    // Unordered map convention: "name" is the immutable original object.
    // Copies have "name_id"
   
    // all_units["Marth"] = unit("Marth", "Prince", id++, 
                            // /*HP Str Mag Skl Spd Lck Def Res Con Mov
        // stats_base*/        {18,  8,  2,  9, 10,  7,  5,  2,  6,  5},
        // /*Growths*/         {60, 50, 20, 70, 60, 40, 30, 20,  0,  0},
        // /*Skills*/          {18,  8,  2},
        // /*Love_pts*/        { 0,  0,  0,  0,  0},
        // /*Love_growths*/    { 0,  0,  0,  0,  0},
                           // //swd lnc axe bow wnd firetnd drk lgt staff
        // /*Weapon_exp*/      { 1,  0,  0,  0,  0,  0,  0,  0,  0,  0},
        // /*Position*/        { 1,  1,  1},
        // /*Equipped*/        {0},
        // /*Equipment*/       {inventory_items("Rapier", 0)},
        // /*Weapons*/         {},
        // /*Items*/           {},
        // /*Exp*/             0, 
        // /*Lovers*/          {"Sheeda", "", "", "", ""},
        // /*Mounted*/         0,
        // /*Flying*/          0,
        // /*Armored*/         0,
        // /*Promoted*/        0);

    // all_units["Sheeda"] = unit("Sheeda", "Pegasus Knight", id++, 
                            // /*HP Str Mag Skl Spd Lck Def Res Con Mov
        // stats_base*/        {18,  7,  2,  7, 18,  8,  4,  4,  5,  7},
        // /*Growths*/         {40, 25, 20, 40, 70, 80, 20, 20,  0,  0},
        // /*Skills*/          {18,  8,  2},
        // /*Love_pts*/        { 0,  0,  0,  0,  0},
        // /*Love_growths*/    { 0,  0,  0,  0,  0},
                           // //swd lnc axe bow wnd fir tnd drk lgt staff
        // /*Weapon_exp*/      { 0,  1,  0,  0,  0,  0,  0,  0,  0,  0},
        // /*Position*/        { 1,  2,  1},
        // /*Equipped*/        {1},
        // /*Equipment*/       {inventory_item("Iron Lance", 10), inventory_item("Iron Lance", 10)},
        // /*Weapons*/         {},
        // /*Items*/           {},
        // /*Exp*/             0, 
        // /*Lovers*/          {"Marth", "", "", "", ""},
        // /*Mounted*/         1,
        // /*Flying*/          1,
        // /*Armored*/         0,
        // /*Promoted*/        0);
           
        // all_units["Sheeda2"] = unit("Sheeda2", "Pegasus Knight", id++, 
                            // /*HP Str Mag Skl Spd Lck Def Res Con Mov
        // stats_base*/        {18,  7,  2,  7, 18,  8,  4,  4,  5,  7},
        // /*Growths*/         {40, 25, 20, 40, 70, 80, 20, 20,  0,  0},
        // /*Skills*/          {18,  8,  2},
        // /*Love_pts*/        { 0,  0,  0,  0,  0},
        // /*Love_growths*/    { 0,  0,  0,  0,  0},
                           // //swd lnc axe bow wnd fir tnd drk lgt staff
        // /*Weapon_exp*/      { 1,  0,  0,  0,  0,  0,  0,  0,  0,  0},
        // /*Position*/        { 1,  2,  1},
        // /*Equipped*/        {0},
        // /*Equipment*/       {inventory_item("Iron Lance", 10), inventory_item("Iron Sword", 10)},
        // /*Weapons*/         {},
        // /*Items*/           {},
        // /*Exp*/             0, 
        // /*Lovers*/          {"Marth", "", "", "", ""},
        // /*Mounted*/         1,
        // /*Flying*/          1,
        // /*Armored*/         0,
        // /*Promoted*/        0);
                
           
           
    // std::ofstream out("units.txt");
    // out << all_units["Marth"]; // This is the c++ way of printing an object to file.
    // out << all_units["Sheeda"];
    // out.close();
    // remove("units.txt");
    // all_units["Marth"].write("units.txt");
    
    // printf("where is the mistake %d \n", all_units["Sheeda"].flying);
    // printf("where is the mistake %d \n", all_units["Sheeda2"].flying);
    // printf("%d \n", all_units["Sheeda"].get_equipped()[0]);
    // write_all_units("units.txt", "cpp");
    
    // printf("%s \n", all_weapons["Iron Sword"].name);
    // printf("%s \n", all_weapons["Rapier"].name);
    // all_weapons["Iron Sword"].write("weaponrite.txt");
    // all_units["Sheeda"].write("unitrite.txt");
    // write_all_weapons("weaponrite.txt");
    
    // all_units["Sheeda_test"].read("units.txt", 20);
    // all_weapons["Sheeda_test"].read("weapons.txt", 20);
    // printf("%s \n", all_units["Sheeda"].name);
    
    
    
    
    // printf("%s \n", all_units["Sheeda"].supports["Marth"]);
    // printf("%s \n", all_units["Marth"].name);
    // printf("%s \n", all_units["Sheeda"].type);
    // printf("%d \n", all_units["Sheeda"].stats_base[0]);
    // printf("%d \n", all_units["Sheeda"].stats_base[1]);
    // printf("%d \n", all_units["Sheeda"].stats_base[2]);
    // printf("%d \n", all_units["Sheeda"].growths[0]);
    // printf("%d \n", all_units["Sheeda"].stats[0]);
    // printf("this is get_equipped %d \n", all_units["Sheeda"].get_equipped()[0]);
    // printf("%s \n", all_units["Sheeda"].equipment[0].name);
    // cout << all_units["Sheeda"].equipment[0].name << endl;
    // printf("%s \n", all_units["Sheeda"].equipment[1].name);
    // printf("%s \n", all_units["Sheeda"].lovers[0]);
    // printf("%d \n", all_units["Sheeda"].exp);
    // printf("%d \n", all_units["Sheeda"].mounted);
    // printf("%d \n", all_units["Sheeda"].flying);
    // printf("%d \n", all_units["Sheeda"].armored);
    // printf("%d \n", all_units["Sheeda"].promoted);

    
}
