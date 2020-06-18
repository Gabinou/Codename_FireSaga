#ifndef RENDERSYSTEMX_HPP
#define RENDERSYSTEMX_HPP

#include <SDL2/SDL.h>
#include "events.hpp"
#include "map.hpp"
#include "sprite.hpp"
#include "text.hpp"
#include <entityx/entityx.h>

class RenderSystemx: public entityx::System<RenderSystemx>, public entityx::Receiver<RenderSystemx> {
    // I think the rendering system needs a way to buffer/transition animations. So that everything doesn't just appear suddenly.
private:
    Game * game;
    SDL_Renderer * renderer = NULL;
    entityx::Entity * fps;
    entityx::EventManager * event_manager;
    entityx::ComponentHandle<Map> mapx;
    uint16_t * tilesize;
    // uint16_t linespace;
    Point offset;
    int64_t frame_count = 0.;
    int64_t last_update = 0.;
    float slide_step = 0.01;
    float slide_wait = 0;
    float cursor_fasttime = 0.35;
public:
    RenderSystemx();
    RenderSystemx(SDL_Renderer * in_renderer, Game * in_game);

    void setRenderer(SDL_Renderer * in_renderer);
    void setMap(entityx::ComponentHandle<Map> in_map);
    void setTilesize(const uint16_t width, const uint16_t height);
    // void setLinespace(const uint16_t in_linespace);
    SDL_Rect loopSprites(entityx::ComponentHandle<Sprite> in_sprite);
    void slideSprites(entityx::Entity * in_ent, int16_t * slidepos, int16_t * objectivepos, int64_t dt);

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