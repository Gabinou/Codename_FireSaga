#ifndef ECS_HPP
#define ECS_HPP

#include <iostream>
#include <vector>
#include <algorithm>
#include <bitset>
#include <array>
#include <memory>

class Component;
class Entity;

using ComponentID = std::size_t;

inline ComponentID getComponentTypeID() {
    static ComponentID lastID = 0;
    return(lastID++);
}

template <typename T> inline ComponentID getComponentTypeID() noexcept {
    static ComponentID typeID = getComponentTypeID();
    return typeID;
}

constexpr std::size_t maxcomponents = 32;

using ComponentBitSet = std::bitset<maxcomponents>;
using ComponentArray = std::array<Component*, maxcomponents>;

class Component {
    public:
        Entity* entity;
        
        virtual void init() {}
        virtual void update() {}
        virtual void draw() {}

        virtual ~Component() {}
};

class Entity{
    private: 
        bool active = true;
        std::vector<std::unique_ptr<Component>> components;
        
        ComponentArray componentarray;
        ComponentBitSet componentbitset;
    public:
        void update(){
            for(auto& c : components) c->update();
        }
        void draw() {
            for(auto& c : components) c->draw();
        }
        bool isActive() const{return(active);}
        void destroy() {active = false;}
        
        template <typename T> bool hasComponent() const {
            return componentbitset[getComponentTypeID<T>];
        }
        
        template <typename T, typename... TArgs>
        T& addComponent(TArgs&&... mArgs) {
            T* c(new T(std::forward<TArgs>(mArgs)...));
            c->entity = this;
            std::unique_ptr<Component> uPtr{c};
            components.emplace_back(std::move(uPtr));
        
            componentarray[getComponentTypeID<T>()] = c;
            componentbitset[getComponentTypeID<T>()] = true;
            
            c->init();
            return *c;
        }    

        template<typename T> T& getComponent() const{
            auto ptr(componentarray[getComponentTypeID<T>()]);
            return(*static_cast<T*>(ptr));
        }
      
};

class Manager {
    private:
        std::vector<std::unique_ptr<Entity>> entities;

    public:
        void update(){
            for (auto& e : entities){
                e->update();
            }
        }
        void draw(){
            for (auto& e : entities) {
                e->draw();
            }
        }
        void refresh() {
            entities.erase(std::remove_if(std::begin(entities), std::end(entities), [](const std::unique_ptr<Entity> &mEntity){
                return !mEntity->isActive();
            }),
            std::end(entities));
        }
        
        Entity& addEntity(){
            Entity* e = new Entity();
            std::unique_ptr<Entity> uPtr{e};
            entities.emplace_back(std::move(uPtr));
            return *e;
        }        
};




#endif /* ECS_HPP */