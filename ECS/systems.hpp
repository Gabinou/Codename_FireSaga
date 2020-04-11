#ifndef SYSTEMS_HPP
#define SYSTEMS_HPP

#include <SDL2/SDL.h>
#include "ECS.h"
#include "spritecomponent.hpp"

class Manager : public EntityX {

    public:
        systems.add<RenderSystem>;

    void update(deltaTime dt) {
        systems.update<RenderSystem>(dt);
    }

    Manager manager;
};


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
            // world->each<UnitComponent>([&](ECS::Entity * ent, ECS::ComponentHandle<UnitComponent> unit) {
            //     ComponentHandle<UnitComponent> sprite = ent->get<SpriteComponent>();
            // -sprite->draw();

            // }
            // Iterate over Menu component? which are boxes?
            SDL_RenderPresent(renderer);
        }
};


#endif /* SYSTEMS_HPP */