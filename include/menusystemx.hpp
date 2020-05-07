#ifndef MENUSYSTEMX_HPP
#define MENUSYSTEMX_HPP

#include <SDL2/SDL.h>
#include <entityx/entityx.h>
#include "map.hpp"
#include "events.hpp"
#include "position.hpp"

class MenuSystemx: public entityx::System<MenuSystemx>, public entityx::Receiver<MenuSystemx> {
private:
    Game * game;
    Settings * settings;
    entityx::EventManager * event_manager;
    entityx::Entity selected;
    short int old_position[2];
    entityx::ComponentHandle<Map> mapx;
public:
    MenuSystemx();
    MenuSystemx(Game * in_game);

    void updateMap();
    void setMap(entityx::ComponentHandle<Map> in_map);

    void configure(entityx::EventManager & event_manager);
    void receive(const unitMenu & menu);
    void receive(const unitSelect & menu);
    void receive(const unitmenuSelect & select);
    void receive(const mapMenu & menu);
    void receive(const disableMenu & disable);
    void receive(const mapmenuSelect & menu);
    void receive(const return2Map & map);

    void update(entityx::EntityManager & es, entityx::EventManager & events, entityx::TimeDelta dt) override;
};

#endif /* MENUSYSTEMX_HPP */