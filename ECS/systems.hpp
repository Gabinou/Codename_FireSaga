#ifndef SYSTEMS_HPP
#define SYSTEMS_HPP

#include "ECS.h"
#include "spritecomponent.hpp"

class UpdateSystem: public ECS::EntitySystem {
    public:
        virtual void tick(ECS::World * world, float deltaTime) override {

        }
};

class InitSystem: public ECS::EntitySystem {
    public:
        virtual void tick(ECS::World * world, float deltaTime) override {

        }
};

class RenderSystem: public ECS::EntitySystem {
    public:
        // No Need a draw system. I think I need a Render system
        virtual void tick(ECS::World * world, float deltaTime) override {
            SDL_RenderClear(renderer);
            world->each<Map>([&](ECS::Entity * ent, ECS::ComponentHandle<Map> map) {
                map->draw();
            });
            // world->each<SpriteComponent>([&](ECS::Entity * ent, ECS::ComponentHandle<SpriteComponent> sprite) {
            //     sprite->draw();
            // });
            SDL_RenderPresent(renderer);
            
        }
};


#endif /* SYSTEMS_HPP */