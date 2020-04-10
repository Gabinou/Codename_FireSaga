#ifndef SYSTEMS_HPP
#define SYSTEMS_HPP

#include <SDL2/SDL.h>
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
    private:
        SDL_Renderer * renderer = NULL;
    public:
        void setRenderer(SDL_Renderer * in_renderer) {
            renderer = in_renderer;
        };

        RenderSystem(SDL_Renderer * in_renderer) {
            renderer = in_renderer;
        }

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