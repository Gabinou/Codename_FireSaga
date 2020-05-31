#ifndef MENU_HPP
#define MENU_HPP

#include "structs.hpp"
#include "enums.hpp"
#include "unit.hpp"
#include "events.hpp"
#include "entityx/entityx.h"

template<typename T> class Clickable {
private:
    Point rect; //.x is width, .y is height
    entityx::EventManager * event_manager;
    entityx::ComponentHandle<Unit> unit;
    typedef T event;
public:
    Clickable() {}

    void click(entityx::Entity mouse) {
        SDL_Log("Clicking");
        event_manager->emit<T>(mouse, unit);
    }

    void setUnit(entityx::ComponentHandle<Unit> in_unit) {
        unit = in_unit;
    }
    entityx::ComponentHandle<Unit> getUnit() {
        return (unit);
    }

    void setEventManager(entityx::EventManager * in_event_manager) {
        event_manager = in_event_manager;
    }
};

class Buttons {
private:
    std::vector<entityx::Entity> buttons;
public:
    void addButton(entityx::Entity in_button);
};

#endif /* MENU_HPP */