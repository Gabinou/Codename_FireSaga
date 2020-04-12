#ifndef SYSTEMS_HPP
#define SYSTEMS_HPP

#include <SDL2/SDL.h>
#include "map.hpp"
// #include "game.hpp"
#include "spritecomponent.hpp"
#include "keyboardcontroller.hpp"
#include "gamepadcontroller.hpp"
#include <entityx/entityx.h>

class RenderSystemx: public entityx::System<RenderSystemx> {
    private:
        SDL_Renderer * renderer = NULL;
        SDL_Rect srcrect;
        SDL_Rect destrect;
        short unsigned int * tilesize;
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

        void setMap(entityx::ComponentHandle<Map> in_map) {
            tilesize = in_map->getTilesize();
            srcrect.w = tilesize[0];
            destrect.w = tilesize[0];
            srcrect.h = tilesize[1];
            destrect.h = tilesize[1];
        }

        void update(entityx::EntityManager & es, entityx::EventManager & events, entityx::TimeDelta dt) override {
            SDL_RenderClear(renderer);
            es.each<Map>([dt, this](entityx::Entity ent, Map & map) {
                map.draw();
            });

            es.each<SpriteComponent, PositionComponent>([dt, this](entityx::Entity ent, SpriteComponent & sprite, PositionComponent & position) {
                int kb_held = 0;
                int gp_held = 0;
                short int frames = sprite.getFrames();
                short int speed = sprite.getSpeed();
                short int slidepos[2];
                short int objectivepos[2];
                std::string slidetype = sprite.getSlidetype();
                short int slideint = sprite.getSlideint();
                float * slidefactors = sprite.getSlidefactors();

                if (sprite.isAnimated()) { //looping sprites.
                    std::string looping = sprite.getSs_looping();

                    if (looping == "pingpong") {
                        srcrect.x = srcrect.w * pingpong(static_cast<int>(SDL_GetTicks() / speed), frames, 0);
                    } else if ((looping == "linear") || (looping == "direct")) {
                        srcrect.x = srcrect.w * static_cast<int>((SDL_GetTicks() / speed) % frames);
                    } else if (looping == "reverse") {
                        srcrect.x = srcrect.w * (frames - static_cast<int>((SDL_GetTicks() / speed) % frames));
                    }
                } else {
                    if (tilesize[0] == 0 && tilesize[1] == 0) { //move on the pixelspace
                        slidepos[0] = (int)position.getPos()[0];
                        slidepos[1] = (int)position.getPos()[1];
                    } else { //move on the map.
                        slidepos[0] = (int)position.getPos()[0] * tilesize[0];
                        slidepos[1] = (int)position.getPos()[1] * tilesize[1];
                    }

                }

                entityx::ComponentHandle<KeyboardController> keyboard = ent.component<KeyboardController>();

                if (keyboard) {
                    kb_held = keyboard->getHeldmove();
                }

                entityx::ComponentHandle<GamepadController> gamepad = ent.component<GamepadController>();

                if (gamepad) {
                    gp_held = gamepad->getHeldmove();
                }

                if (slidetype == "geometric") { //for cursor mvt on map.
                    objectivepos[0] = (int)position.getPos()[0] * (tilesize[0]) - destrect.w / 4;
                    objectivepos[1] = (int)position.getPos()[1] * (tilesize[1]) - destrect.h / 4;

                    if ((gp_held > 25) || (kb_held > 25))  {
                        slideint = 1;
                    }

                    if (objectivepos[0] != slidepos[0]) {
                        slidepos[0] += geometricslide((objectivepos[0] - slidepos[0]), slidefactors[slideint]);
                    }

                    if (objectivepos[1] != slidepos[1]) {
                        slidepos[1] += geometricslide((objectivepos[1] - slidepos[1]), slidefactors[slideint]);
                    }

                    if ((objectivepos[0] == slidepos[0]) && (objectivepos[1] == slidepos[1])) {
                        position.setUpdatable(true);
                        slideint = 0;
                    } else {
                        position.setUpdatable(false);
                    }
                }

                if (slidetype == "vector") { //for unit mvt on map.

                }

                destrect.x = slidepos[0];
                destrect.y = slidepos[1];

                sprite.setSrcrect(srcrect);
                sprite.setDestrect(destrect);
                sprite.draw();
            });
            SDL_RenderPresent(renderer);
        }
};

class ControlSystemx: public entityx::System<ControlSystemx> {
    private:
        short unsigned int * tilesize;
        SDL_Rect srcrect;
        SDL_Rect destrect;
        Game * game;
        KeyboardInputMap keyboardInputMap;
        GamepadInputMap gamepadInputMap;
    public:
        ControlSystemx() {

        }

        ControlSystemx(Game * in_game) {
            game = in_game;
            keyboardInputMap = game->getKeyboardInputMap();
            gamepadInputMap = game->getGamepadInputMap();
        }

        void update(entityx::EntityManager & es, entityx::EventManager & events, entityx::TimeDelta dt) override {
            es.each<KeyboardController, PositionComponent>([dt, this](entityx::Entity ent, KeyboardController & keyboard, PositionComponent & position) {

                // const Uint8 * kb_state = SDL_GetKeyboardState(NULL);
                // std::vector<std::vector<SDL_Scancode>> pressed_move{};
                // std::vector<std::vector<SDL_Scancode>> pressed_button{};

                // if (is_pressed(kb_state, inputmap.moveup) && !is_pressed(kb_state, inputmap.movedown)) {
                //     // positioncomponent->addPos(0, -1);
                //     pressed_move.push_back(inputmap.moveup);
                // } else if (!is_pressed(kb_state, inputmap.moveup) && is_pressed(kb_state, inputmap.movedown)) {
                //     // positioncomponent->addPos(0, 1);
                //     pressed_move.push_back(inputmap.movedown);
                // }

                // if (!is_pressed(kb_state, inputmap.moveright) && is_pressed(kb_state, inputmap.moveleft)) {
                //     // positioncomponent->addPos(-1, 0);
                //     pressed_move.push_back(inputmap.moveleft);
                // } else if (is_pressed(kb_state, inputmap.moveright) && !is_pressed(kb_state, inputmap.moveleft)) {
                //     // positioncomponent->addPos(1, 0);
                //     pressed_move.push_back(inputmap.moveright);
                // }

                // if (is_pressed(kb_state, inputmap.accept)) {
                //     pressed_button.push_back(inputmap.accept);
                //     short int toset = -1;
                //     // Entity * setter;
                //     // Entity * ontile = map->getTile(positioncomponent->getPos()[0], positioncomponent->getPos()[1]);

                //     if ((game->getState() == GAME::STATE::MAP) && (frames_button == 1)) {
                //         SDL_Log("cursor Position, %d %d \n", positioncomponent->getPos()[0], positioncomponent->getPos()[1]);

                //         // if (ontile) {
                //         //     toset = GAME::STATE::UNITMOVE;
                //         //     setter = ontile;
                //         // } else {
                //         //     toset = GAME::STATE::OPTIONS;
                //         //     setter = entity;
                //         // }
                //     } else if ((game->getState() == GAME::STATE::UNITMOVE) && (frames_button == 1)) {
                //         // toset = GAME::STATE::UNITMENU;
                //         // setter = entity;
                //     }

                //     if (toset != -1) {
                //         // game->setState(*setter, toset);
                //     }
                // }

                // if (is_pressed(kb_state, inputmap.cancel)) {
                //     pressed_button.push_back(inputmap.cancel);

                //     if ((game->getState() == GAME::STATE::UNITMENU) ||
                //             (game->getState() == GAME::STATE::OPTIONS) ||
                //             (game->getState() == GAME::STATE::UNITMOVE)) {
                //         // game->setState(*entity, GAME::STATE::MAP);
                //     }
                // }

                // check_move(pressed_move);
                // check_button(pressed_button);
            });
            es.each<GamepadController, PositionComponent>([dt, this](entityx::Entity ent, GamepadController & gamepad, PositionComponent & position) {

            });
        }

};

#endif /* SYSTEMS_HPP */