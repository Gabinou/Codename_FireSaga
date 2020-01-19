#include "game.hpp"
#include "spritecomponent.hpp"
#include "positioncomponent.hpp"
#include "arrowcomponent.hpp"
#include "textcomponent.hpp"
#include "keyboardcontroller.hpp"
#include "gamepadcontroller.hpp"
#include "unitcomponent.hpp"

Map * mapp;

SDL_Renderer * Game::renderer = nullptr;
SDL_Event Game::event;
TTF_Font * Game::font = NULL;
Manager Game::manager;
Settings Game::settings;

std::unordered_map<std::string, Weapon> all_weapons;
std::unordered_map<int, Tile> all_tiles;

Entity & cursor = Game::manager.addEntity(); // FOR SOME REASON, having an 2, 4, 8 entities.... breaks something in the unitmove->unitmenu states move
Entity & player = Game::manager.addEntity();
Entity & player2 = Game::manager.addEntity();

Game::Game() {}
Game::~Game() {}

void Game::setFontsize(int in_fontsize) {
    fontsize = in_fontsize;
}

Game::getFontsize() {
    return(fontsize);
}

void Game::createUnitmenu(Entity & setting_entity) {
    printf("Creating unit menu\n");
    menu_entities.push(manager.getEntities().size());
    manager.addEntity();
    manager.getEntities()[menu_entities.top()]->addComponent<PositionComponent>();
    manager.getEntities()[menu_entities.top()]->getComponent<PositionComponent>().setBounds(0, 2000, 0, 2000);
    manager.getEntities()[menu_entities.top()]->getComponent<PositionComponent>().setPos(
        (int)(setting_entity.getComponent<PositionComponent>().getPos()[0] * tilesize[0]),
        (int)(setting_entity.getComponent<PositionComponent>().getPos()[1] * tilesize[1]));
    SDL_Color black = {255, 255, 255};
    manager.getEntities()[menu_entities.top()]->addComponent<SpriteComponent>("..//assets//textbox.png", (int []) {128, 128}); 
    manager.getEntities()[menu_entities.top()]->addComponent<TextComponent>(fontsize, std::vector<std::string> {"Attack", "Wait"}, black);
    manager.getEntities()[menu_entities.top()]->addGroup(manager.groupUI);
}

void Game::destroyUnitmenu() {
    if (!menu_entities.empty()) {
        manager.getEntities()[menu_entities.top()]->destroy();
        menu_entities.pop();
    }  else {
        printf("Could not destroy unit menu.");
    }
}

void Game::moveUnit(Entity & cursor) {
    int newPos[2];
    newPos[0] = cursor.getComponent<PositionComponent>().getPos()[0];
    newPos[1] = cursor.getComponent<PositionComponent>().getPos()[1];
    manager.getEntities()[unit_entities.top()]->getComponent<PositionComponent>();
}

void Game::setState(Entity & setting_entity, std::string new_state) {
    printf("Game state changes from %s to %s\n", this->state.c_str(), new_state.c_str()); 

    if (new_state == "pause"){
        
    }
    if (this->state == "pause") {
         
    }
    if (this->state == "stats") {
        
    }
    if (this->state == "minimap") {
         
    }
    
    if (this->state == "map") {
        if (new_state == "unitmenu") {
            moveUnit(setting_entity);
            // Entity ** cursor = manager.getEntities()[unit_entities.top()];
        }
        if (new_state == "stats") {
            
        }
        if (new_state == "minimap") {
            
        }
        if (new_state == "unitmove") { // GREAT BIG FPS DROP HERE.
            std::vector<std::unique_ptr<Entity>> current_entities;
            std::vector<std::vector<int>> costmap;
            std::vector<std::vector<int>> movemapp;
            // std::vector<std::vector<int>> edges; 
            int start[2];
            std::string unitmvttype;
            int unit_move;
            unsigned char * range;

            for (int i = 0; i < manager.getEntities().size(); i++) {
                if (manager.getEntities()[i].get() == (Entity *)&setting_entity) {
                    unit_entities.push(i);
                }
            }
            unit_move = manager.getEntities()[unit_entities.top()]->getComponent<UnitComponent>().getStats().move; //0 for horse.

            start[0] = manager.getEntities()[unit_entities.top()]->getComponent<PositionComponent>().getPos()[0]; // Start is (+1,+1)?
            start[1] = manager.getEntities()[unit_entities.top()]->getComponent<PositionComponent>().getPos()[1]; // Start is (+1,+1)?
            start[0] = start[0] - 1;
            start[1] = start[1] - 1;

            unitmvttype = manager.getEntities()[unit_entities.top()]->getComponent<UnitComponent>().getMvttype();
            range = manager.getEntities()[unit_entities.top()]->getComponent<UnitComponent>().getRange();// THIS DOESNT WORK.

            costmap = mapp->makeMvtCostmap(unitmvttype);

            movemapp = movemap(costmap, start, unit_move, "matrix");
            mapp->setMap("move", movemapp);
            // plot2Dvector(movemapp);

            // edges = matrix_edges(movemapp);
            // plot2Dvector(edges);

            if ((range[0] > 0) && (range[1] > 0)) {
                std::vector<std::vector<int>> attackmapp = attackmap(movemapp, start, unit_move, range, "matrix");
                // std::vector<std::vector<int>> attackmapp = attackmap(movemapp, start, unit_move + (int)range[0] - 1, (int)range[1], "matrix");
                // plot2Dvector(attackmapp);
                mapp->setMap("attack", attackmapp);
            }
     
            mapp->showOverlay();
        }
        if (new_state == "options") {
            
        }
        if (new_state == "cutscene") {
            
        }
    }
    
    if (this->state == "options") {
        if (new_state == "map") {
  
        }
        if (new_state == "startmenu") {
  
        }
    }
    
    if (this->state == "startmenu") {
        if (new_state == "introvideo") {
        }
        if (new_state == "saves") {
        }
        if (new_state == "soundroom") {
        }
        if (new_state == "options") {
        }
    }
    
    if (this->state == "preparation") {
        if (new_state == "placement") {
        }
        if (new_state == "chooseunits") {
        }
        if (new_state == "equipment") {
        }
        if (new_state == "forge") {
        }        
        if (new_state == "storage") {
        }        
        if (new_state == "bonusexp") {
        }        
    }
    if ((this->state == "placement") ||
     (this->state == "chooseunits") ||
     (this->state == "equipment") ||
     (this->state == "forge") ||
     (this->state == "storage") ||
     (this->state == "bonusexp")){
        if (new_state == "preparation") {
        }
        if (new_state == "map") {
        }     
    }
    
    if (this->state == "unitmove") {
        if (new_state == "unitmenu") {
            mapp->hideOverlay();
            createUnitmenu(setting_entity); 
            int *new_position = setting_entity.getComponent<PositionComponent>().getPos();
            int *old_position = manager.getEntities()[unit_entities.top()]->getComponent<PositionComponent>().getPos();
            
            printf("Old position %d, %d \n", old_position[0], old_position[1]);
            printf("New position %d, %d \n", new_position[0], new_position[1]);

            manager.getEntities()[unit_entities.top()]->getComponent<PositionComponent>().setPos(            
                new_position[0],
                new_position[1]);
            
            Entity * ontile = mapp->getTile(old_position[0], old_position[1]);
            mapp->removeTile(old_position[0], old_position[1]);
            mapp->setTile(new_position[0], new_position[1], ontile);
        }
        if (new_state == "map") {
            mapp->hideOverlay();
        }
    }        
    
    if (this->state == "unitmenu") {
        if (new_state == "map") { 
            destroyUnitmenu();
        }
        if (new_state == "attack") { 
        
        }
        if (new_state == "trade") { 
        
        }
        if (new_state == "items") { 
        
        }
        if (new_state == "conversation") { 
        
        }
    }

    if (this->state == "conversation") {
        if (new_state == "map") { 
        
        }
    }
    
    if (this->state == "attack") {
        if (new_state == "battle") { 
        
        }
    }
    
    state = std::string(new_state);
}

void Game::setState(const char * new_state) {
    state = std::string(new_state);
}

std::string Game::getState() {
    return(state);
}

void Game::baseWeapons(){
    Weapon_stats temp_wpn;
    Unit_stats temp_unit;
    // dmg, hit, dodge, crit, favor, wgt, uses, wpnlvl, range, hand, dmg_type
    // For shields, dmg is def? 
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
    Excalibur.setDescription("The king's sword. Found in a rock. To be thrown in a lake when broken.");
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
    Weapon Raw_iron_slab("Raw Iron slab", "sword", temp_wpn);
    Raw_iron_slab.setDescription("Barely a sword. Too big, too thick, too massive, too rough. Tempered by the malevolence of countless slain demons. Effective against demons and angels.");
    Raw_iron_slab.setEffective("DemonAngel");
    all_weapons["Raw_iron_slab"] = Raw_iron_slab;
    //Should be found inside a dragon skull? 

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
    battleaxe.setDescription("Comically oversized axe.");
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

    // Daggers get ridiculously low hit cause they are short and stubby!!!! Kitchen knife is the only exception.
    temp_wpn = {2, 0, 10, 0, 10, 0, 5, 21, 16, {1,1}, {1}, 0, 1000};
    Weapon wristblade("Wristblade", "offhand", temp_wpn);
    wristblade.setDescription("Blade generally strapped to the weak forearm. Used by assassins to increase crit potential.");
    all_weapons["wristblade"] = wristblade;
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
    swordbreaker.setDescription("Dagger with slots designed to not break, but catch swords. Increases avoid and favor. Rarely does it actually break swords... ");
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
    shrunken_head.setDescription("");
    all_weapons["shrunken_head"] = shrunken_head;        
    temp_wpn = {7, 0, 0, 0, 0, 0, 7, 8, 5, {1,1}, {1}, 0, 1000};
    Weapon crystal("Crystal", "trinket", temp_wpn);
    crystal.setDescription("");
    all_weapons["crystal"] = crystal;      
    temp_wpn = {7, 0, 0, 0, 0, 0, 7, 8, 5, {1,1}, {1}, 0, 1000};
    Weapon human_femur("Human femur", "trinket", temp_wpn);
    human_femur.setDescription("");
    all_weapons["human_femur"] = human_femur;        
    temp_wpn = {7, 0, 0, 0, 0, 0, 7, 8, 5, {1,1}, {1}, 0, 1000};
    Weapon void_fragment("Void fragment", "trinket", temp_wpn);
    void_fragment.setDescription("");
    all_weapons["void_fragment"] = void_fragment;
    temp_wpn = {7, 0, 0, 0, 0, 0, 7, 8, 5, {1,1}, {1}, 0, 1000};
    Weapon demon_fang("Demon fang", "trinket", temp_wpn);
    demon_fang.setDescription("");
    all_weapons["demon_fang"] = demon_fang;
    temp_wpn = {7, 0, 0, 0, 0, 0, 7, 8, 5, {1,1}, {1}, 0, 1000};
    Weapon angel_feather("Angel feather", "trinket", temp_wpn);
    angel_feather.setDescription("");
    all_weapons["angel_feather"] = angel_feather;
    temp_wpn = {7, 0, 0, 0, 0, 0, 7, 8, 5, {1,1}, {1}, 0, 1000};
    Weapon angel_hair("Angel hair", "trinket", temp_wpn);
    angel_hair.setDescription("Golden strands of angel hair held up by a silver thread.");
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

void Game::baseTiles(){
    unsigned int * temp_stats[3] = {0, 0, 0};// dodge, def, heal

    // Fliers always have 1 movement because they dismount inside.
    // foot_slow, foot_fast, mages, riders_slow, riders_fast, fliers, armors, pirates, bandits;
    Movement_cost temp_cost = {1, 1, 1, 1, 1, 1, 1, 1, 1};
    Tile plain("Plain", temp_cost, temp_stats);
    all_tiles[10] = plain;
    temp_stats[0] = (unsigned int *)10;
    temp_stats[1] = (unsigned int *)1;
    temp_cost = {2, 1, 2, 3, 2, 1, 3, 2, 2};
    Tile bush("Bush", temp_cost, temp_stats);
    all_tiles[11] = bush;
    temp_stats[0] = (unsigned int *)15;
    temp_stats[1] = (unsigned int *)2;
    temp_cost = {3, 2, 3, 4, 3, 1, 3, 3, 3};
    Tile forest("Forest", temp_cost, temp_stats);
    all_tiles[12] = forest;
    temp_stats[0] = (unsigned int *)0;
    temp_stats[1] = (unsigned int *)0;
    temp_cost = {0, 0, 0, 0, 0, 1, 0, 0, 0};
    Tile thicket("Thicket", temp_cost, temp_stats);
    all_tiles[13] = thicket;
    temp_cost = {0, 0, 0, 0, 0, 1, 0, 0, 0};
    Tile snag("Snag", temp_cost, temp_stats);
    all_tiles[14] = snag;
    temp_cost = {1, 1, 1, 1, 1, 1, 1, 1, 1};
    Tile bridge("Bridge", temp_cost, temp_stats);
    all_tiles[15] = bridge;
    temp_cost = {0, 0, 0, 0, 0, 1, 0, 2, 0};
    Tile sea("Sea", temp_cost, temp_stats);
    all_tiles[20] = sea;
    temp_cost = {0, 0, 0, 0, 0, 1, 0, 3, 0};
    Tile lake("Lake", temp_cost, temp_stats);
    all_tiles[21] = lake;
    temp_cost = {5, 4, 5, 0, 0, 1, 0, 2, 4};
    Tile river("River", temp_cost, temp_stats);
    all_tiles[22] = river;
    temp_cost = {0, 0, 0, 0, 0, 1, 0, 0, 0};
    Tile waterfall("Waterfall", temp_cost, temp_stats);
    all_tiles[23] = waterfall;
    temp_cost = {0, 0, 0, 0, 0, 1, 0, 0, 0};
    Tile cliff("cliff", temp_cost, temp_stats);
    all_tiles[24] = cliff;

    temp_stats[0] = (unsigned int *)15;
    temp_stats[1] = (unsigned int *)2;
    temp_cost = {3, 2, 3, 4, 3, 1, 4, 2, 2};
    Tile hill("hill", temp_cost, temp_stats);
    all_tiles[30] = hill;
    temp_stats[0] = (unsigned int *)25;
    temp_stats[1] = (unsigned int *)3;
    temp_cost = {4, 3, 4, 0, 4, 1, 0, 3, 3};
    Tile mountain("mountain", temp_cost, temp_stats);
    all_tiles[31] = mountain;
    temp_stats[0] = (unsigned int *)30;
    temp_stats[1] = (unsigned int *)4;
    temp_cost = {0, 5, 0, 0, 0, 1, 0, 5, 5};
    Tile peak("Peak", temp_cost, temp_stats);
    all_tiles[32] = peak;

    temp_stats[0] = (unsigned int *)0;
    temp_stats[1] = (unsigned int *)0;
    temp_cost = {3, 2, 1, 4, 3, 1, 3, 2, 2};
    Tile sand("Sand", temp_cost, temp_stats);
    all_tiles[40] = sand;
    temp_cost = {3, 2, 1, 4, 3, 1, 3, 2, 2};
    Tile wasteland("Wasteland", temp_cost, temp_stats);
    all_tiles[41] = wasteland;
    temp_stats[0] = (unsigned int *)5;
    temp_stats[1] = (unsigned int *)2;
    temp_cost = {0, 0, 0, 0, 0, 1, 0, 3, 3};
    Tile rock("Rock", temp_cost, temp_stats);
    all_tiles[42] = rock;
    temp_stats[0] = (unsigned int *)0;
    temp_stats[1] = (unsigned int *)0;
    temp_cost = {0, 0, 0, 0, 0, 1, 0, 0, 0};
    Tile bones("Bones", temp_cost, temp_stats);
    all_tiles[43] = bones;

    temp_cost = {1, 1, 1, 1, 1, 1, 1, 1, 1};
    Tile floor("Floor", temp_cost, temp_stats);
    all_tiles[50] = floor;
    temp_stats[0] = (unsigned int *)10;
    temp_stats[1] = (unsigned int *)1;
    temp_stats[2] = (unsigned int *)10;
    temp_cost = {2, 2, 2, 3, 3, 1, 3, 2, 2};
    Tile pillar("Pillar", temp_cost, temp_stats);
    all_tiles[51] = pillar;
    temp_stats[0] = (unsigned int *)0;
    temp_stats[1] = (unsigned int *)0;
    temp_stats[2] = (unsigned int *)0;
    temp_cost = {0, 0, 0, 0, 0, 1, 0, 0, 0};
    Tile wall("Wall", temp_cost, temp_stats);
    all_tiles[52] = wall;
    temp_cost = {0, 0, 0, 0, 0, 1, 0, 0, 0};
    Tile door("Door", temp_cost, temp_stats);
    all_tiles[53] = door;
    temp_stats[0] = (unsigned int *)20;
    temp_stats[1] = (unsigned int *)2;
    temp_stats[2] = (unsigned int *)15;
    temp_cost = {0, 0, 0, 0, 0, 1, 0, 0, 0};
    Tile throne("Throne", temp_cost, temp_stats);
    all_tiles[54] = throne;

    temp_stats[0] = (unsigned int *)10;
    temp_stats[1] = (unsigned int *)1;
    temp_stats[2] = (unsigned int *)10;
    temp_cost = {0, 0, 0, 0, 0, 1, 0, 0, 0};
    Tile gate("Gate", temp_cost, temp_stats);
    all_tiles[60] = gate;
    temp_cost = {0, 0, 0, 0, 0, 1, 0, 0, 0};
    Tile fence("Fence", temp_cost, temp_stats);
    all_tiles[61] = fence;
    temp_stats[0] = (unsigned int *)10;
    temp_stats[1] = (unsigned int *)1;
    temp_stats[2] = (unsigned int *)10;
    temp_cost = {1, 1, 1, 1, 1, 1, 1, 1, 1};
    Tile shop("Shop", temp_cost, temp_stats);
    all_tiles[62] = shop;
    temp_cost = {1, 1, 1, 1, 1, 1, 1, 1, 1};
    Tile armory("Armory", temp_cost, temp_stats);
    all_tiles[63] = armory;
    temp_cost = {1, 1, 1, 1, 1, 1, 1, 1, 1};
    Tile village("Village", temp_cost, temp_stats);
    all_tiles[64] = village;
    temp_stats[0] = (unsigned int *)10;
    temp_stats[1] = (unsigned int *)2;
    temp_stats[2] = (unsigned int *)10;
    temp_cost = {2, 2, 2, 2, 2, 1, 2, 2, 2};
    Tile fort("Fort", temp_cost, temp_stats);
    all_tiles[65] = fort;
    temp_stats[0] = (unsigned int *)0;
    temp_stats[1] = (unsigned int *)0;
    temp_stats[2] = (unsigned int *)0;
    temp_cost = {0, 0, 0, 0, 0, 1, 0, 0, 0};
    Tile castle("Castle", temp_cost, temp_stats);
    all_tiles[66] = castle;

    temp_cost = {2, 2, 2, 3, 2, 1, 2, 2, 2};
    Tile snow("Snow", temp_cost, temp_stats);
    all_tiles[70] = snow;
    temp_cost = {4, 3, 4, 0, 0, 1, 0, 3, 3};
    Tile glacier("Glacier", temp_cost, temp_stats);
    all_tiles[71] = glacier;
}

void Game::baseUnits(){
    manager.addEntity();
    Unit_stats temp;
    //hp,str,mag,skl,spd,luck,def,res,con,move 
    temp = {17,  6,  2,  7,  7,   7,  4,  5,  6, 5};
    manager.getEntities().back()->addComponent<UnitComponent>("Main", "Mercenary", temp);
    temp = {48, 14, 25, 32, 34,  28, 19, 40, 15, 0};
    manager.getEntities().back()->getComponent<UnitComponent>().setCaps(temp);
    temp = {60, 50, 20, 60, 70,  40, 30, 20,  10, 0};
    manager.getEntities().back()->getComponent<UnitComponent>().setGrowths(temp);
    manager.getEntities().back()->getComponent<UnitComponent>().setExp(0); 
    manager.getEntities().back()->getComponent<UnitComponent>().write("unit_test.txt");
    all_units["Main"] = manager.getEntities().size() - 1;

    manager.addEntity();
    temp = {18,  6,  2,  7,  7,   7,  4,  5,  6, 7};
    manager.getEntities().back()->addComponent<UnitComponent>("Reliable", "Cavalier", temp);
    temp = {48, 14, 25, 32, 34,  28, 19, 40, 15};
    manager.getEntities().back()->getComponent<UnitComponent>().setCaps(temp);
    temp = {60, 50, 20, 60, 70,  40, 30, 20,  10, 0};
    manager.getEntities().back()->getComponent<UnitComponent>().setGrowths(temp);
    manager.getEntities().back()->getComponent<UnitComponent>().setExp(100);
    all_units["Reliable"] = manager.getEntities().size() - 1;

    manager.addEntity();
    temp = {19,  6,  2,  7,  7,   7,  4,  5,  6,  7};
    manager.getEntities().back()->addComponent<UnitComponent>("Coward", "Cavalier", temp);
    temp = {48, 14, 25, 32, 34,  28, 19, 40, 15};
    manager.getEntities().back()->getComponent<UnitComponent>().setCaps(temp);
    temp = {60, 50, 20, 60, 70,  40, 30, 20,  10, 0};
    manager.getEntities().back()->getComponent<UnitComponent>().setGrowths(temp);
    manager.getEntities().back()->getComponent<UnitComponent>().setExp(200);
    all_units["Coward"] = manager.getEntities().size() - 1;

    manager.addEntity();
    temp = {20,  6,  2,  7,  7,   7,  4,  5,  6,  6};
    manager.getEntities().back()->addComponent<UnitComponent>("1H Jaigen", "Fencermaster", temp);
    temp = {48, 14, 25, 32, 34,  28, 19, 40, 15};
    manager.getEntities().back()->getComponent<UnitComponent>().setCaps(temp);
    temp = {60, 50, 20, 60, 70,  40, 30, 20,  10, 0};
    manager.getEntities().back()->getComponent<UnitComponent>().setGrowths(temp);
    manager.getEntities().back()->getComponent<UnitComponent>().setExp(2200);
    all_units["1H Jaigen"] = manager.getEntities().size() - 1;

    manager.addEntity();
    temp = {14,  6,  2,  7,  7,   7,  4,  5,  6,  5};
    manager.getEntities().back()->addComponent<UnitComponent>("Lovely", "Priestess", temp);
    temp = {48, 14, 25, 32, 34,  28, 19, 40, 15};
    manager.getEntities().back()->getComponent<UnitComponent>().setCaps(temp);
    temp = {60, 50, 20, 60, 70,  40, 30, 20,  10, 0};
    manager.getEntities().back()->getComponent<UnitComponent>().setGrowths(temp);
    manager.getEntities().back()->getComponent<UnitComponent>().setExp(100);
    all_units["Lovely"] = manager.getEntities().size() - 1;
    
    manager.addEntity();
    temp = {16,  6,  2,  7,  7,   7,  4,  5,  6,  6};
    manager.getEntities().back()->addComponent<UnitComponent>("Hottie", "Thief", temp);
    temp = {48, 14, 25, 32, 34,  28, 19, 40, 15};
    manager.getEntities().back()->getComponent<UnitComponent>().setCaps(temp);
    temp = {60, 50, 20, 60, 70,  40, 30, 20,  10, 0};
    manager.getEntities().back()->getComponent<UnitComponent>().setGrowths(temp);
    manager.getEntities().back()->getComponent<UnitComponent>().setExp(2200);
    all_units["Hottie"] = manager.getEntities().size() - 1;

    manager.addEntity();
    temp = {22,  4,  5,  7,  6,   8,  4,  6,  5, 5}; // 4 or 5?
    manager.getEntities().back()->addComponent<UnitComponent>("Servil", "Knight", temp);
    temp = {48, 14, 25, 32, 34,  28, 19, 40, 15};
    manager.getEntities().back()->getComponent<UnitComponent>().setCaps(temp);
    temp = {60, 50, 20, 60, 70,  40, 30, 20,  10, 0};
    manager.getEntities().back()->getComponent<UnitComponent>().setGrowths(temp); 
    manager.getEntities().back()->getComponent<UnitComponent>().setExp(500);
    all_units["Servil"] = manager.getEntities().size() - 1;

    manager.addEntity();
    temp = {34,  4,  5,  7,  6,   8,  4,  6,  5, 5};
    manager.getEntities().back()->addComponent<UnitComponent>("Pérignon", "Mage", temp);
    temp = {48, 14, 25, 32, 34,  28, 19, 40, 15};
    manager.getEntities().back()->getComponent<UnitComponent>().setCaps(temp);
    temp = {60, 50, 20, 60, 70,  40, 30, 20,  10, 0};
    manager.getEntities().back()->getComponent<UnitComponent>().setGrowths(temp);
    manager.getEntities().back()->getComponent<UnitComponent>().setExp(1200);
    all_units["Pérignon"] = manager.getEntities().size() - 1;

    manager.addEntity();
    temp = {15,  4,  5,  7,  6,   8,  4,  6,  5, 5};
    manager.getEntities().back()->addComponent<UnitComponent>("Poet", "Mage", temp);
    temp = {48, 14, 25, 32, 34,  28, 19, 40, 15};
    manager.getEntities().back()->getComponent<UnitComponent>().setCaps(temp);
    temp = {60, 50, 20, 60, 70,  40, 30, 20,  10, 0};
    manager.getEntities().back()->getComponent<UnitComponent>().setGrowths(temp);
    manager.getEntities().back()->getComponent<UnitComponent>().setExp(400);
    all_units["Poet"] = manager.getEntities().size() - 1;

    manager.addEntity();
    temp = {15,  4,  5,  7,  6,   8,  4,  6,  5, 5};
    manager.getEntities().back()->addComponent<UnitComponent>("Silou", "Mage", temp);
    temp = {48, 14, 25, 32, 34,  28, 19, 40, 15};
    manager.getEntities().back()->getComponent<UnitComponent>().setCaps(temp);
    temp = {60, 50, 20, 60, 70,  40, 30, 20,  10, 0};
    manager.getEntities().back()->getComponent<UnitComponent>().setGrowths(temp);
    manager.getEntities().back()->getComponent<UnitComponent>().setExp(400);
    all_units["Silou"] = manager.getEntities().size() - 1;

    manager.addEntity();
    temp = {15,  4,  5,  7,  6,   8,  4,  6,  5, 5};
    manager.getEntities().back()->addComponent<UnitComponent>("Poet", "Mage", temp);
    temp = {48, 14, 25, 32, 34,  28, 19, 40, 15};
    manager.getEntities().back()->getComponent<UnitComponent>().setCaps(temp);
    temp = {60, 50, 20, 60, 70,  40, 30, 20,  10, 0};
    manager.getEntities().back()->getComponent<UnitComponent>().setGrowths(temp);
    manager.getEntities().back()->getComponent<UnitComponent>().setExp(400);
    all_units["Arm Thief"] = manager.getEntities().size() - 1;

    manager.addEntity();
    temp = {15,  4,  5,  7,  6,   8,  4,  6,  5, 5};
    manager.getEntities().back()->addComponent<UnitComponent>("Poet", "Mage", temp);
    temp = {48, 14, 25, 32, 34,  28, 19, 40, 15};
    manager.getEntities().back()->getComponent<UnitComponent>().setCaps(temp);
    temp = {60, 50, 20, 60, 70,  40, 30, 20,  10, 0};
    manager.getEntities().back()->getComponent<UnitComponent>().setGrowths(temp);
    manager.getEntities().back()->getComponent<UnitComponent>().setExp(400);
    all_units["Mage2"] = manager.getEntities().size() - 1;

    manager.addEntity();
    temp = {15,  4,  5,  7,  6,   8,  4,  6,  5, 5};
    manager.getEntities().back()->addComponent<UnitComponent>("Silou", "Mage", temp);
    temp = {48, 14, 25, 32, 34,  28, 19, 40, 15};
    manager.getEntities().back()->getComponent<UnitComponent>().setCaps(temp);
    temp = {60, 50, 20, 60, 70,  40, 30, 20,  10, 0};
    manager.getEntities().back()->getComponent<UnitComponent>().setGrowths(temp);
    manager.getEntities().back()->getComponent<UnitComponent>().setExp(400);
    all_units["Mage2"] = manager.getEntities().size() - 1;

    manager.addEntity();
    temp = {15,  4,  5,  7,  6,   8,  4,  6,  5, 5};
    manager.getEntities().back()->addComponent<UnitComponent>("Silou", "Mage", temp);
    temp = {48, 14, 25, 32, 34,  28, 19, 40, 15};
    manager.getEntities().back()->getComponent<UnitComponent>().setCaps(temp);
    temp = {60, 50, 20, 60, 70,  40, 30, 20,  10, 0};
    manager.getEntities().back()->getComponent<UnitComponent>().setGrowths(temp);
    manager.getEntities().back()->getComponent<UnitComponent>().setExp(400);
    all_units["Mage3"] = manager.getEntities().size() - 1;
}

void Game::init(const char * title, int xpos, int ypos, int width, int height, bool fullscreen) {
    int flags = 0;

    if (fullscreen) {
        flags = SDL_WINDOW_FULLSCREEN;
    }

    if (TTF_Init() == -1) {
        printf("TTF_Init: %s\n", TTF_GetError());
        exit(2);
    }
            
    Game::font = TTF_OpenFont("../fonts/arial.ttf", Game::fontsize); // Size translates to pixel size? 
    //The srcrect does not change size with font pointsize.

    if (Game::font == NULL) {
        printf("Failed to load lazy font! SDL_ttf Error: %s\n", TTF_GetError());
    }

    if (SDL_Init(SDL_INIT_EVERYTHING) == 0) {
        printf("SDL subsystems initialized.\n");
        window = SDL_CreateWindow(title, xpos, ypos, width, height, flags);

        if (window) {
            printf("Window created.\n");
        }

        if (TTF_Init() == -1) {
            printf("SDL_ttf could not initialize! SDL_ttf Error: %s\n", TTF_GetError());
        }

        renderer = SDL_CreateRenderer(window, -1, 0);

        if (renderer) {
            SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
            printf("Renderer created.\n");
        }

        isRunning = true;
    } else {
        isRunning = false;
    }

    printf("Making weapons\n");
    baseWeapons();

    printf("Making units\n");
    baseUnits();

    printf("Making tiles\n");
    baseTiles();

    printf("Creating map \n");
    tilesize[0] = 32;
    tilesize[1] = 32;
    mapp =  new Map(tilesize[0], tilesize[1]); // mapp is a pointer
    mapp->loadTilemap("..//maps//test_tilemap.txt");

    cursor.addComponent<PositionComponent>(6, 6);

    manager.getEntities()[all_units["Silou"]]->addComponent<PositionComponent>(6, 6);
    manager.getEntities()[all_units["Silou"]]->getComponent<PositionComponent>().setMap(mapp);

    SDL_Color black = {255, 255, 255};
    cursor.addComponent<KeyboardController>(this, mapp);

    if( SDL_NumJoysticks() < 1 ) {
        printf( "No joysticks connected.\n" );
    } else {
        cursor.addComponent<GamepadController>(this, mapp);
    }

    manager.getEntities()[all_units["Silou"]]->addComponent<SpriteComponent>(mapp, "..//assets//horse.png");

    cursor.addComponent<SpriteComponent>(mapp, "..//assets//cursors.png", 10, 50);
    cursor.getComponent<SpriteComponent>().setSlidetype("geometric");
    
    manager.getEntities()[all_units["Silou"]]->addGroup(manager.groupUnits);
    cursor.addGroup(manager.groupUI);

    this->setState("map");
    
};

void Game::update() {
    manager.refresh();
    manager.update();
}
 
KeyboardInputMap Game::getKeyboardInputMap() {
    return(keyboardInputMap);
}

GamepadInputMap Game::getGamepadInputMap() {
    return(gamepadInputMap);
}

void Game::handleEvents() {
    SDL_PollEvent(&event);

    switch (event.type) {
    case SDL_QUIT:
        isRunning = false;
        break;

    default:
        break;
    }
}

std::vector<Entity *> & units = Game::manager.getGroup(Game::manager.groupUnits);
std::vector<Entity *> & uxs = Game::manager.getGroup(Game::manager.groupUI);

void Game::render() {
    SDL_RenderClear(renderer);
    // Add stuff to render. Paint the background First.

    mapp->drawMap();

    for (auto& u : units) {
        u->draw();
    }

    for (auto& u : uxs) {
        u->draw();
    }

    SDL_RenderPresent(renderer);
}
void Game::clean() {
    SDL_DestroyWindow(window);
    SDL_DestroyRenderer(renderer);
    SDL_Quit();
    printf("Game cleanded.");
    SDL_Delay(5000);
}
bool Game::running() {
    return (isRunning);
}
