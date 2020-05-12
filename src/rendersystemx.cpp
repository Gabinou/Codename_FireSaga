
#include "rendersystemx.hpp"
// #ifndef STB_SPRINTF_IMPLEMENTATION
// #define STB_SPRINTF_IMPLEMENTATION
#include "stb_sprintf.h"
// #endif /* STB_SPRINTF_IMPLEMENTATION */


void RenderSystemx::setRenderer(SDL_Renderer * in_renderer) {
    if (in_renderer) {
        SDL_Log("Added renderer to renderSystemx");
        renderer = in_renderer;
    } else {
        SDL_Log("RenderSystemx: Renderer is invalid");
    }
}

RenderSystemx::RenderSystemx() {
    offset.x = DEFAULT::TILEMAP_XOFFSET;
    offset.y = DEFAULT::TILEMAP_YOFFSET;
}

RenderSystemx::RenderSystemx(SDL_Renderer * in_renderer, Game * in_game) {
    setRenderer(in_renderer);
    game = in_game;
    fps = in_game->getFPS();
}

void RenderSystemx::setMap(entityx::ComponentHandle<Map> in_map) {
    if (in_map) {
        mapx = in_map;
        tilesize = mapx->getTilesize();
    } else {
        SDL_Log("RenderSystemx: Map Handle is invalid");
    }
}

void RenderSystemx::setTilesize(const short int unsigned width, const short int unsigned height) {
    tilesize[0] = width;
    tilesize[1] = height;
}

void RenderSystemx::setLinespace(const short int unsigned in_linespace) {
    linespace = in_linespace;
}

void RenderSystemx::slideSprites(entityx::Entity * in_ent, short int * slidepos, short int * objectivepos, double dt) {
    if (in_ent->valid()) {
        entityx::ComponentHandle<GamepadController> gamepad = in_ent->component<GamepadController>();
        entityx::ComponentHandle<KeyboardController> keyboard = in_ent->component<KeyboardController>();
        entityx::ComponentHandle<Sprite> sprite = in_ent->component<Sprite>();
        entityx::ComponentHandle<Position> position = in_ent->component<Position>();
        Point cursor_pos;
        Point offset;
        double kb_held = 0.;
        double gp_held = 0.;
        short int slide_int = sprite->getSlideint();
        unsigned char slidetype = sprite->getSlidetype();
        float * slidefactors = sprite->getSlidefactors();
        short int scalefactor[2];
        SDL_Rect destrect = sprite->getDestrect();

        scalefactor[0] = tilesize[0];
        scalefactor[1] = tilesize[1];

        if (keyboard) {
            kb_held = keyboard->getHeldmove();
        }

        if (gamepad) {
            gp_held = gamepad->getHeldmove();
        }

        if ((!keyboard) && (!gamepad)) {
            if (!position->isonTilemap()) { //move on the menu space
                scalefactor[0] = 1;
                scalefactor[1] = 1;
            }

            cursor_pos = position->getPos();
            offset = position->getOffset();
            slidepos[0] = (int)((cursor_pos.x - offset.x) * scalefactor[0]);
            slidepos[1] = (int)((cursor_pos.y - offset.y) * scalefactor[1]);
        } else {
            if (!position->isonTilemap()) { //move on the menu space
                scalefactor[0] = linespace;
                scalefactor[1] = linespace;
            }

            switch (slidetype) {
                case SLIDETYPE::GEOMETRIC: //for cursor mvt on mapx.
                    if ((kb_held > cursor_fasttime) || (gp_held > cursor_fasttime)) {
                        slide_int = 1;
                        // SDL_Log("fast %.6f", cursor_fasttime);
                        // SDL_Log("kb_held: %.6f, gp_held: %.6f", gp_held, kb_held);
                    } else {
                        slide_int = 0;
                    }

                    cursor_pos = position->getPos();
                    offset = position->getOffset();
                    objectivepos[0] = (int)(cursor_pos.x - offset.x) * (scalefactor[0]) - destrect.w / 4;
                    objectivepos[1] = (int)(cursor_pos.y - offset.y) * (scalefactor[1]) - destrect.h / 4;

                    if (slide_wait > slide_step) {
                        if (objectivepos[0] != slidepos[0]) {
                            slidepos[0] += geometricslide((objectivepos[0] - slidepos[0]), slidefactors[slide_int]);
                        }

                        if (objectivepos[1] != slidepos[1]) {
                            slidepos[1] += geometricslide((objectivepos[1] - slidepos[1]), slidefactors[slide_int]);
                        }

                        slide_wait = 0.;
                    }

                    if ((objectivepos[0] == slidepos[0]) && (objectivepos[1] == slidepos[1])) {
                        position->setUpdatable(true);
                    } else {
                        position->setUpdatable(false);
                    }

                    break;
            }
        }
    } else {
        SDL_Log("slideSprites: Entity is invalid");
    }
}

SDL_Rect RenderSystemx::loopSprites(entityx::ComponentHandle<Sprite> in_sprite) {
    SDL_Rect srcrect;

    if (in_sprite) {
        short int frames = in_sprite->getFrames();
        short int speed = in_sprite->getSpeed();
        srcrect = in_sprite->getSrcrect();
        unsigned char looping = in_sprite->getSs_looping();

        switch (looping) {
            case LOOPING::PINGPONG:
                srcrect.x = srcrect.w * pingpong(static_cast<int>(SDL_GetTicks() / speed), frames, 0);
                break;

            case LOOPING::LINEAR:
            case LOOPING::DIRECT:
                srcrect.x = srcrect.w * static_cast<int>((SDL_GetTicks() / speed) % frames);
                break;

            case LOOPING::REVERSE:
                srcrect.x = srcrect.w * (frames - static_cast<int>((SDL_GetTicks() / speed) % frames));
                break;
        }
    }

    return (srcrect);
}

void RenderSystemx::update(entityx::EntityManager & es, entityx::EventManager & events, entityx::TimeDelta dt) {
    SDL_RenderClear(renderer);
    last_update += dt;
    frame_count++;

    if (game->getSettings()->FPS.show) {

        if (last_update >= 0.5) {
            Settings * settings = game->getSettings();
            const float current = frame_count / last_update;
            char buffer[DEFAULT::BUFFER_SIZE];
            stbsp_sprintf(buffer, "%.1f", current);
            fps->component<Text>()->setText(buffer);
            fps->component<Text>()->makeTextures();
            last_update = 0.;
            frame_count = 0.;
        }
    }

    for (entityx::Entity ent : es.entities_with_components<Map>()) {
        entityx::ComponentHandle<Map> mapx = ent.component<Map>();
        mapx->draw();
    }

    for (entityx::Entity ent : es.entities_with_components<Sprite, Position>()) {

        if (!ent.has_component<Text>() && !ent.has_component<KeyboardController>() && !ent.has_component<GamepadController>() && !ent.has_component<MouseController>()) {
            entityx::ComponentHandle<Sprite> sprite = ent.component<Sprite>();
            short int * slidepos = sprite->getSlidepos();
            short int * objectivepos = sprite->getObjpos();
            SDL_Rect srcrect = sprite->getSrcrect();
            SDL_Rect destrect = sprite->getDestrect();

            if (sprite->isAnimated()) {
                srcrect = loopSprites(sprite);
            }

            slideSprites(&ent, slidepos, objectivepos, dt);

            destrect.x = slidepos[0];
            destrect.y = slidepos[1];

            sprite->setSlidepos(slidepos);
            sprite->setObjpos(objectivepos);
            sprite->setSrcrect(srcrect);
            sprite->setDestrect(destrect);
            sprite->draw();
        }
    }

    for (entityx::Entity ent : es.entities_with_components<Text, Position>()) {
        entityx::ComponentHandle<Position> position = ent.component<Position>();
        entityx::ComponentHandle<Text> text = ent.component<Text>();
        Point pos = position->getPos();
        // SDL_Log("unit menu position: %d %d", position[0], position[1]);

        if (ent.has_component<Sprite>()) {
            ent.component<Sprite>()->setDestrectpos(pos);
            ent.component<Sprite>()->draw();
        }

        text->setRects(pos);
        text->draw();
    }

    slide_wait += dt;

    for (entityx::Entity ent : es.entities_with_components<KeyboardController>()) {
        entityx::ComponentHandle<Sprite> sprite = ent.component<Sprite>();

        // SDL_Log("Rendering Cursor");

        short int * slidepos = sprite->getSlidepos();
        short int * objectivepos = sprite->getObjpos();
        SDL_Rect srcrect = sprite->getSrcrect();
        SDL_Rect destrect = sprite->getDestrect();

        if (sprite->isAnimated()) { //looping sprites.
            srcrect = loopSprites(sprite);
        }

        slideSprites(&ent, slidepos, objectivepos, dt);

        destrect.x = slidepos[0];
        destrect.y = slidepos[1];

        sprite->setSlidepos(slidepos);
        sprite->setObjpos(objectivepos);
        sprite->setSrcrect(srcrect);
        sprite->setDestrect(destrect);
        sprite->draw();
    }

    for (entityx::Entity ent : es.entities_with_components<MouseController>()) {
        entityx::ComponentHandle<Sprite> sprite = ent.component<Sprite>();
        entityx::ComponentHandle<MouseController> mouse = ent.component<MouseController>();

        if ((sprite) && (mouse)) {
            // if ()
            // {  }
            SDL_Rect destrect = sprite->getDestrect();
            Point pos = mouse->getPixelPos();
            destrect.x = pos.x;
            destrect.y = pos.y;
            sprite->setDestrect(destrect);
            sprite->draw();
        }
    }

    SDL_RenderPresent(renderer);
}

void RenderSystemx::configure(entityx::EventManager & in_events) {
    event_manager = &in_events;
    event_manager->subscribe<turnBegin>(*this);
    event_manager->subscribe<turnEnd>(*this);
    event_manager->subscribe<unitHeal>(*this);
    event_manager->subscribe<unitWait>(*this);
    event_manager->subscribe<unitDie>(*this);
    event_manager->subscribe<unitRefresh>(*this);
}

void RenderSystemx::receive(const turnBegin & begin) {
    SDL_Log("Received turnBegin event");

}

void RenderSystemx::receive(const turnEnd & end) {

}

void RenderSystemx::receive(const unitHeal & heal) {

}

void RenderSystemx::receive(const unitDie & death) {

}

void RenderSystemx::receive(const unitWait & wait) {

}

void RenderSystemx::receive(const unitRefresh & refresh) {

}