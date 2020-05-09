
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
    offset[0] = DEFAULT::TILEMAP_XOFFSET;
    offset[1] = DEFAULT::TILEMAP_YOFFSET;
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

void RenderSystemx::slideSprites(entityx::Entity * in_ent, short int * slidepos, short int * objectivepos) {
    if (in_ent->valid()) {
        entityx::ComponentHandle<GamepadController> gamepad = in_ent->component<GamepadController>();
        entityx::ComponentHandle<KeyboardController> keyboard = in_ent->component<KeyboardController>();
        entityx::ComponentHandle<Sprite> sprite = in_ent->component<Sprite>();
        entityx::ComponentHandle<Position> position = in_ent->component<Position>();
        short int kb_held = 0;
        short int gp_held = 0;
        short int slideint = sprite->getSlideint();
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

            slidepos[0] = (int)(position->getPos()[0] * scalefactor[0]);
            slidepos[1] = (int)(position->getPos()[1] * scalefactor[1]);
        } else {
            if (!position->isonTilemap()) { //move on the menu space
                scalefactor[0] = linespace;
                scalefactor[1] = linespace;
            }

            switch (slidetype) {
                case SLIDETYPE::GEOMETRIC: //for cursor mvt on mapx.
                    objectivepos[0] = (int)(position->getPos()[0]) * (scalefactor[0]) - destrect.w / 4;
                    objectivepos[1] = (int)(position->getPos()[1]) * (scalefactor[1]) - destrect.h / 4;

                    if ((gp_held > DEFAULT::CURSOR_FAST) || (kb_held > DEFAULT::CURSOR_FAST))  {
                        slideint = 1;
                    }

                    if (objectivepos[0] != slidepos[0]) {
                        slidepos[0] += geometricslide((objectivepos[0] - slidepos[0]), slidefactors[slideint]);
                    }

                    if (objectivepos[1] != slidepos[1]) {
                        slidepos[1] += geometricslide((objectivepos[1] - slidepos[1]), slidefactors[slideint]);
                    }

                    if ((objectivepos[0] == slidepos[0]) && (objectivepos[1] == slidepos[1]) && ((kb_held > DEFAULT::CURSOR_PAUSE) || (kb_held < 2)) && ((gp_held > DEFAULT::CURSOR_PAUSE) || (gp_held < 2))) {
                        position->setUpdatable(true);
                        slideint = 0;
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
        entityx::ComponentHandle<Sprite> sprite = ent.component<Sprite>();

        if (!ent.has_component<Text>()) {
            short int * slidepos = sprite->getSlidepos();
            short int * objectivepos = sprite->getObjpos();
            SDL_Rect srcrect = sprite->getSrcrect();
            SDL_Rect destrect = sprite->getDestrect();

            if (sprite->isAnimated()) {
                srcrect = loopSprites(sprite);
            }

            slideSprites(&ent, slidepos, objectivepos);

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
        short int * pos = position->getPos();
        // SDL_Log("unit menu position: %d %d", position[0], position[1]);

        if (ent.has_component<Sprite>()) {
            ent.component<Sprite>()->setDestrectpos(pos);
            ent.component<Sprite>()->draw();
        }

        text->setRects(pos);
        text->draw();
    }

    cursor_wait += dt;

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


        if (cursor_wait > cursor_deadtime) {
            slideSprites(&ent, slidepos, objectivepos);
            cursor_wait = 0.;
        }

        destrect.x = slidepos[0];
        destrect.y = slidepos[1];

        sprite->setSlidepos(slidepos);
        sprite->setObjpos(objectivepos);
        sprite->setSrcrect(srcrect);
        sprite->setDestrect(destrect);
        sprite->draw();
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