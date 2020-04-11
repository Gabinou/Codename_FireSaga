#ifndef SYSTEMS_HPP
#define SYSTEMS_HPP

#include <SDL2/SDL.h>
#include "map.hpp"
#include <entityx/entityx.h>

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

class RenderSystemx: public entityx::System<RenderSystemx> {
    private:
        SDL_Renderer * renderer = NULL;
    public:
        void setRenderer(SDL_Renderer * in_renderer) {
            if (in_renderer) {
                SDL_Log("Added renderer to renderSystemx");
                renderer = in_renderer;
            } else {
                SDL_Log("In renderer is null");
            }
        };

        RenderSystemx() {

        }

        RenderSystemx(SDL_Renderer * in_renderer) {
            setRenderer(in_renderer);
        }

        void update(entityx::EntityManager & es, entityx::EventManager & events, entityx::TimeDelta dt) override {
            SDL_RenderClear(renderer);
            es.each<Map>([dt](entityx::Entity ent, Map & map) {
                map.draw();
            });
            // es.each<Sprite>([dt](entityx::Entity ent, SpriteComponent & sprite) {
            //     sprite.draw();
            // });
            SDL_RenderPresent(renderer);
        }
};

// class RenderSystem: public ECS::EntitySystem {
//     private:
//         SDL_Renderer * renderer = NULL;
//     public:
//         void setRenderer(SDL_Renderer * in_renderer) {
//             renderer = in_renderer;
//         };

//         RenderSystem(SDL_Renderer * in_renderer) {
//             renderer = in_renderer;
//         }

//         virtual void tick(ECS::World * world, float deltaTime) override {
//             SDL_RenderClear(renderer);
//             world->each<Map>([&](ECS::Entity * ent, ECS::ComponentHandle<Map> map) {
//                 map->draw();
//             });
//             // world->each<UnitComponent>([&](ECS::Entity * ent, ECS::ComponentHandle<UnitComponent> unit) {
//             //     ComponentHandle<UnitComponent> sprite = ent->get<SpriteComponent>();
//             // -sprite->draw();

//             // }
//             // Iterate over Menu component? which are boxes?
//             SDL_RenderPresent(renderer);
//         }
// };

#endif /* SYSTEMS_HPP */