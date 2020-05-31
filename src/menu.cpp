
#include "menu.hpp"

void Clickable::click(entityx::Entity mouse) {
    // switch (eventID) {
    //     case EVENTID::UNITATTACK:
    //         event_manager->emit<unitAttack>(mouse, unit);
    //         break;
    // }
    EMIT_EVENT(unitWait);
}
