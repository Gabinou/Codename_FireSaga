#ifndef SYSTEMS_HPP
#define SYSTEMS_HPP

#include "ECS.h"
#include "spritecomponent.hpp"

class UpdateSystem: public ECS::EntitySystem {
    public:
        virtual void tick(ECS::World * world, float deltaTime) override {

        }
};

class DrawSystem: public ECS::EntitySystem {
    public:
        virtual void tick(ECS::World * world, float deltaTime) override {

            world->each<SpriteComponent>([&](ECS::Entity * ent, ECS::ComponentHandle<SpriteComponent> sprite) {
                sprite->draw();
            });
        }
};


#endif /* SYSTEMS_HPP */