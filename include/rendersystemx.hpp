#ifndef RENDERSYSTEMX_HPP
#define RENDERSYSTEMX_HPP

#include <SDL2/SDL.h>
#include <queue>
#include "map.hpp"
#include "sprite.hpp"
#include "events.hpp"
#include "text.hpp"
#include <entityx/entityx.h>

class RenderSystemx: public entityx::System<RenderSystemx>, public entityx::Receiver<RenderSystemx> {
    // I think the rendering system needs a way to buffer/transition animations. So that everything doesn't just appear suddenly.
private:
    SDL_Renderer * renderer = NULL;
    short unsigned int * tilesize;
    short unsigned int linespace;
    short int offset[2];
    entityx::EventManager * event_manager;
    entityx::ComponentHandle<Map> mapx;
public:
    RenderSystemx();
    RenderSystemx(SDL_Renderer * in_renderer);

    void setRenderer(SDL_Renderer * in_renderer);
    void setMap(entityx::ComponentHandle<Map> in_map);
    void setTilesize(const short int unsigned width, const short int unsigned height);
    void setLinespace(const short int unsigned in_linespace);
    SDL_Rect loopSprites(entityx::ComponentHandle<Sprite> in_sprite);
    void slideSprites(entityx::Entity * in_ent, short int * slidepos, short int * objectivepos);

    void configure(entityx::EventManager & event_manager);
    void receive(const turnBegin & begin);
    void receive(const turnEnd & end);
    void receive(const unitHeal & heal);
    void receive(const unitDie & death);
    void receive(const unitWait & wait);
    void receive(const unitRefresh & refresh);

    void update(entityx::EntityManager & es, entityx::EventManager & events, entityx::TimeDelta dt) override;
};

#endif /* RENDERSYSTEMX_HPP */