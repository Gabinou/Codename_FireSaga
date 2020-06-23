#ifndef UNITSYSTEMX_HPP
#define UNITSYSTEMX_HPP

#include <SDL2/SDL.h>
#include "events.hpp"
#include "enums.hpp"
#include "map.hpp"
#include "position.hpp"
#include "sprite.hpp"
#include <entityx/entityx.h>

class UnitSystemx: public entityx::System<UnitSystemx>, public entityx::Receiver<UnitSystemx> {
private:
    Game * game;
    entityx::ComponentHandle<Map> mapx;
    entityx::EventManager * event_manager;
    Point old_position;
    std::vector<uint8_t> unitmenuoptions;
    entityx::Entity selected_ent;
    std::vector<entityx::ComponentHandle<Unit>> defenders;
public:
    UnitSystemx();
    UnitSystemx(Game * in_game);

    void updateMap();
    void setMap(entityx::ComponentHandle<Map> in_map);
    void fight(entityx::Entity attacker_ent, entityx::Entity defender_ent);
    void attack(entityx::Entity attacker_ent, entityx::Entity defender_ent);
    bool canDouble(entityx::Entity attacker_ent, entityx::Entity defender_ent);
    bool canRetaliate(entityx::Entity attacker_ent, entityx::Entity defender_ent);
    void setDefenders(std::vector<entityx::ComponentHandle<Unit>> in_defenders);

    void configure(entityx::EventManager & event_manager);
    void receive(const turnBegin & begin);
    void receive(const turnEnd & end);
    void receive(const unitHeal & heal);
    void receive(const unitDie & death);
    void receive(const unitWait & wait);
    void receive(const unitRefresh & refreshes);
    void receive(const unitTalk & talk);
    void receive(const unitRescue & rescue);
    void receive(const unitAttack & attack);
    void receive(const unitTrade & trade);
    void receive(const unitEscape & escape);
    void receive(const unitStaff & staff);
    void receive(const unitItems & item);
    void receive(const unitHover & hover);
    void receive(const unitDehover & dehover);
    void receive(const unitReturn & Return);
    void receive(const unitSelect & select);
    void receive(const defenderSelect & select);
    void receive(const unitDeselect & select);
    void receive(const unitMove & move);
    void receive(const unitDanger & danger);
    void receive(const refreshUnits & refresh);

    void update(entityx::EntityManager & es, entityx::EventManager & events, entityx::TimeDelta dt) override;
};

#endif /* UNITSYSTEMX_HPP */