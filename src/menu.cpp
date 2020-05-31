
#include "menu.hpp"

entityx::ComponentHandle<Unit> Clickable::getUnit() {
    return (unit);
}

void Clickable::setUnit(entityx::ComponentHandle<Unit> in_unit) {
    unit = in_unit;
}

void Clickable::setEvent(void * in_event) {
    event = in_event;
}

void Clickable::setEventManager(entityx::EventManager * in_event_manager) {
    event_manager = in_event_manager;
}

