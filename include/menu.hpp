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
public:
    Clickable() {}

    //overload for all input cases manually
    void click(entityx::Entity in_entity, entityx::ComponentHandle<Unit> in_unit) {
        event_manager->emit<T>(in_entity, in_unit);
    }
    void click(entityx::Entity in_entity1, entityx::Entity in_entity2) {
        event_manager->emit<T>(in_entity1, in_entity2);
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