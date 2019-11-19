#ifndef ECS_HPP
#define ECS_HPP

#include <iostream>
#include <vector>
#include <algorithm>
#include <bitset>
#include <array>
#include <memory>
#include "game.hpp"

class Component;
class Entity;
class Manager;

constexpr std::size_t maxcomponents = 32;
constexpr std::size_t maxgroups = 32;

using ComponentID = std::size_t;
using Group = std::size_t;
using ComponentBitSet = std::bitset<maxcomponents>;
using GroupBitSet = std::bitset<maxgroups>;
using ComponentArray = std::array<Component *, maxcomponents>;

inline ComponentID getNewComponentTypeID() {
    static ComponentID lastID = 0u;
    return (lastID++);
}

template <typename T> inline ComponentID getComponentTypeID() noexcept {
    static ComponentID typeID = getNewComponentTypeID();
    return typeID;
}

class Component {
    public:
        Entity * entity;

        virtual void init() {}
        virtual void update() {}
        virtual void draw() {}

        virtual ~Component() {}
};

class Entity {
    private:
        Manager & manager;
        bool active = true;
        std::vector<std::unique_ptr<Component>> components;

        ComponentArray componentarray;
        ComponentBitSet componentbitset;
        GroupBitSet groupbitset;

    public:
        Entity(Manager & in_manager) : manager(in_manager) {}

        void update() {
            for (auto & c : components) { c->update(); }
        }
        void draw() {
            for (auto & c : components) { c->draw(); }
        }
        bool isActive() const {return (active);}
        void destroy() {active = false;}

        Manager & getManager() {
            return (manager);
        }

        template <typename T> bool hasComponent() const {
            return componentbitset[getComponentTypeID<T>];
        }

        bool hasGroup(Group in_Group) {
            return (groupbitset[in_Group]);
        }

        void addGroup(Group in_Group);

        void delGroup(Group in_Group) {
            groupbitset[in_Group] = false;
        }


        template <typename T, typename... TArgs>
        T & addComponent(TArgs && ... mArgs) {
            T * c(new T(std::forward<TArgs>(mArgs)...));
            c->entity = this;
            std::unique_ptr<Component> uPtr{c};
            components.emplace_back(std::move(uPtr));
            componentarray[getComponentTypeID<T>()] = c;
            componentbitset[getComponentTypeID<T>()] = true;
            c->init();
            return *c;
        }

        template<typename T> T & getComponent() const {
            auto ptr(componentarray[getComponentTypeID<T>()]);
            return (*static_cast<T *>(ptr));
        }

};

class Manager {
    private:
        std::vector<std::unique_ptr<Entity>> entities;
        std::array<std::vector<Entity *>, maxgroups> groupedEntities;
        Game * game;
    public:

        void setGame(Game * in_game) {
            game = in_game;
        }
        Game * getGame() {
            return (game);
        }

        enum groupLabels : std::size_t {
            groupMap,
            groupUnits,
            groupUI
        };
        void update() {
            for (auto & e : entities) {
                e->update();
            }
        }
        void draw() {
            for (auto & e : entities) {
                e->draw();
            }
        }
        void refresh() {
            // How to erase? removes entites if they are not active.
            for (auto i(0u); i < maxgroups; i++) {
                auto & v(groupedEntities[i]);
                v.erase(std::remove_if(std::begin(v), std::end(v), [i](Entity * in_Entity) {
                    return (!in_Entity->isActive() || !in_Entity->hasGroup(i));
                }),
                std::end(v));
            }

            entities.erase(std::remove_if(std::begin(entities), std::end(entities), [](const std::unique_ptr<Entity> & in_Entity) {
                return (!in_Entity->isActive());
            }),
            std::end(entities));
        }

        void addtogroup(Entity * in_Entity, Group in_Group) {
            groupedEntities[in_Group].emplace_back(in_Entity);
        }

        std::vector<Entity *> & getGroup(Group in_Group) {
            return (groupedEntities[in_Group]);
        }

        Entity & addEntity() {
            Entity * e = new Entity(*this);
            std::unique_ptr<Entity> uPtr{e};
            entities.emplace_back(std::move(uPtr));
            return *e;
        }

};




#endif /* ECS_HPP */