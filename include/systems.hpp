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
        // SDL_Rect srcrect;
        // SDL_Rect destrect; // maybe should not be a member variable?
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
            // initRects(tilesize);
        }

        // void initRects(short unsigned int * tilesize) {
        //     srcrect.w = tilesize[0];
        //     destrect.w = tilesize[0];
        //     srcrect.h = tilesize[1];
        //     destrect.h = tilesize[1];
        // }

        void update(entityx::EntityManager & es, entityx::EventManager & events, entityx::TimeDelta dt) override {
            SDL_RenderClear(renderer);
            es.each<Map>([dt, this](entityx::Entity ent, Map & map) {
                map.draw();
                // initRects(map.getTilesize());
            });

            es.each<SpriteComponent, PositionComponent>([dt, this](entityx::Entity ent, SpriteComponent & sprite, PositionComponent & position) {
                if (!ent.component<KeyboardController>()) {
                    short int frames = sprite.getFrames();
                    short int speed = sprite.getSpeed();
                    short int slidepos[2];
                    short int objectivepos[2];
                    SDL_Rect srcrect = sprite.getSrcrect();
                    SDL_Rect destrect = sprite.getDestrect();
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
                            // SDL_Log("Move on the pixelspace");
                            slidepos[0] = (int)position.getPos()[0];
                            slidepos[1] = (int)position.getPos()[1];
                        } else { //move on the map.
                            // SDL_Log("Move on the map");
                            slidepos[0] = (int)(position.getPos()[0] * tilesize[0]);
                            slidepos[1] = (int)(position.getPos()[1] * tilesize[1]);
                            // SDL_Log("Rendering Keyboard Controller %d %d", position.getPos()[0], position.getPos()[1]);
                            // SDL_Log("Rendering Keyboard Controller %d %d", slidepos[0], slidepos[1]);
                        }

                    }

                    if (slidetype == "vector") { //for unit mvt on map.

                    }

                    destrect.x = slidepos[0];
                    destrect.y = slidepos[1];

                    sprite.setSrcrect(srcrect);
                    sprite.setDestrect(destrect);
                    sprite.draw();
                }
            });
            es.each<SpriteComponent, PositionComponent, KeyboardController>([dt, this](entityx::Entity ent, SpriteComponent & sprite, PositionComponent & position, KeyboardController & keyboard) {
                int kb_held = 0;
                int gp_held = 0;
                short int frames = sprite.getFrames();
                short int speed = sprite.getSpeed();
                short int slidepos[2];
                short int objectivepos[2];
                SDL_Rect srcrect = sprite.getSrcrect();
                SDL_Rect destrect = sprite.getDestrect();
                std::string slidetype = sprite.getSlidetype();
                short int slideint = sprite.getSlideint();
                float * slidefactors = sprite.getSlidefactors();

                //init
                if (tilesize[0] == 0 && tilesize[1] == 0) {
                    slidepos[0] = (int)position.getPos()[0];
                    slidepos[1] = (int)position.getPos()[1];
                } else {
                    slidepos[0] = (int)position.getPos()[0] * tilesize[0];
                    slidepos[1] = (int)position.getPos()[1] * tilesize[1];
                }

                // //initslide
                if (slidetype == "geometric") {
                    // SDL_Log("tilesize: %d, %d", tilesize[0], tilesize[1]);
                    srcrect.w = tilesize[0] * 2;
                    srcrect.h = tilesize[1] * 2;
                    destrect.w = tilesize[0] * 2;
                    destrect.h = tilesize[1] * 2;
                    sprite.setSrcrect(srcrect);
                    sprite.setDestrect(destrect);
                    slidepos[0] = objectivepos[0] = (int)position.getPos()[0] * tilesize[0] - destrect.w / 4;
                    slidepos[1] = objectivepos[1] = (int)position.getPos()[1] * tilesize[1] - destrect.h / 4;
                }

                if (sprite.isAnimated()) { //looping sprites.
                    std::string looping = sprite.getSs_looping();

                    if (looping == "pingpong") {
                        srcrect.x = srcrect.w * pingpong(static_cast<int>(SDL_GetTicks() / speed), frames, 0);
                    } else if ((looping == "linear") || (looping == "direct")) {
                        srcrect.x = srcrect.w * static_cast<int>((SDL_GetTicks() / speed) % frames);
                    } else if (looping == "reverse") {
                        srcrect.x = srcrect.w * (frames - static_cast<int>((SDL_GetTicks() / speed) % frames));
                    }
                }

                // } else {
                //     if (tilesize[0] == 0 && tilesize[1] == 0) { //move on the pixelspace
                //         // SDL_Log("Move on the pixelspace");
                //         slidepos[0] = (int)position.getPos()[0];
                //         slidepos[1] = (int)position.getPos()[1];
                //     } else { //move on the map.
                //         // SDL_Log("Move on the map");
                //         slidepos[0] = (int)(position.getPos()[0] * tilesize[0]);
                //         slidepos[1] = (int)(position.getPos()[1] * tilesize[1]);
                //         // SDL_Log("Rendering Keyboard Controller %d %d", position.getPos()[0], position.getPos()[1]);
                //         // SDL_Log("Rendering Keyboard Controller %d %d", slidepos[0], slidepos[1]);
                //     }

                // }

                // entityx::ComponentHandle<KeyboardController> keyboard = ent.component<KeyboardController>();
                // // SDL_Log("Rendering Keyboard Controller %d %d", slidepos[0], slidepos[1]);

                // if (keyboard) {
                //     // SDL_Log("Rendering Keyboard Controller %d %d", tilesize[0], tilesize[1]);
                //     // SDL_Log("Rendering Keyboard Controller %d %d", position.getPos()[0], position.getPos()[1]);
                //     // SDL_Log("Rendering Keyboard Controller %d %d", slidepos[0], slidepos[1]);
                //     kb_held = keyboard->getHeldmove();
                // }

                // entityx::ComponentHandle<GamepadController> gamepad = ent.component<GamepadController>();

                // if (gamepad) {
                //     SDL_Log("Rendering Gamepad Controller.");
                //     gp_held = gamepad->getHeldmove();
                // }

                // if (slidetype == "geometric") { //for cursor mvt on map.
                //     objectivepos[0] = (int)position.getPos()[0] * (tilesize[0]) - destrect.w / 4;
                //     objectivepos[1] = (int)position.getPos()[1] * (tilesize[1]) - destrect.h / 4;

                //     if ((gp_held > 25) || (kb_held > 25))  {
                //         slideint = 1;
                //     }

                //     if (objectivepos[0] != slidepos[0]) {
                //         slidepos[0] += geometricslide((objectivepos[0] - slidepos[0]), slidefactors[slideint]);
                //     }

                //     if (objectivepos[1] != slidepos[1]) {
                //         slidepos[1] += geometricslide((objectivepos[1] - slidepos[1]), slidefactors[slideint]);
                //     }

                //     if ((objectivepos[0] == slidepos[0]) && (objectivepos[1] == slidepos[1])) {
                //         position.setUpdatable(true);
                //         slideint = 0;
                //     } else {
                //         position.setUpdatable(false);
                //     }
                // }

                // if (slidetype == "vector") { //for unit mvt on map.

                // }

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
        entityx::ComponentHandle<Map> map;
        KeyboardInputMap keyboardInputMap;
        GamepadInputMap gamepadInputMap;
    public:
        ControlSystemx() {

        }

        ControlSystemx(Game * in_game, entityx::ComponentHandle<Map> in_map) {
            setGame(in_game);
            setMap(in_map);
        }

        ControlSystemx(entityx::ComponentHandle<Map> in_map, Game * in_game) {
            setGame(in_game);
            setMap(in_map);
        }

        ControlSystemx(Game * in_game) {
            setGame(in_game);
        }

        ControlSystemx(entityx::ComponentHandle<Map> in_map) {
            setMap(in_map);
        }

        void setGame(Game * in_game) {
            game = in_game;
            keyboardInputMap = game->getKeyboardInputMap();
            gamepadInputMap = game->getGamepadInputMap();
        }

        void setMap(entityx::ComponentHandle<Map> in_map) {
            map = in_map;
        }

        void update(entityx::EntityManager & es, entityx::EventManager & events, entityx::TimeDelta dt) override {
            es.each<KeyboardController, PositionComponent>([dt, this](entityx::Entity ent, KeyboardController & keyboard, PositionComponent & position) {
                const Uint8 * kb_state = SDL_GetKeyboardState(NULL);
                std::vector<std::vector<SDL_Scancode>> pressed_move{};
                std::vector<std::vector<SDL_Scancode>> pressed_button{};

                if (keyboard.is_pressed(kb_state, keyboardInputMap.moveup) && !keyboard.is_pressed(kb_state, keyboardInputMap.movedown)) {
                    position.addPos(0, -1);
                    pressed_move.push_back(keyboardInputMap.moveup);
                } else if (!keyboard.is_pressed(kb_state, keyboardInputMap.moveup) && keyboard.is_pressed(kb_state, keyboardInputMap.movedown)) {
                    position.addPos(0, 1);
                    pressed_move.push_back(keyboardInputMap.movedown);
                }

                if (!keyboard.is_pressed(kb_state, keyboardInputMap.moveright) && keyboard.is_pressed(kb_state, keyboardInputMap.moveleft)) {
                    position.addPos(-1, 0);
                    pressed_move.push_back(keyboardInputMap.moveleft);
                } else if (keyboard.is_pressed(kb_state, keyboardInputMap.moveright) && !keyboard.is_pressed(kb_state, keyboardInputMap.moveleft)) {
                    position.addPos(1, 0);
                    pressed_move.push_back(keyboardInputMap.moveright);
                }

                if (keyboard.is_pressed(kb_state, keyboardInputMap.accept)) {
                    pressed_button.push_back(keyboardInputMap.accept);
                    short int toset = -1;
                    entityx::Entity * setter;
                    entityx::Entity * ontile = map->getEnt(position.getPos()[0], position.getPos()[1]);

                    unsigned int frames_button = keyboard.getHeldbutton();

                    if ((game->getState() == GAME::STATE::MAP) && (frames_button == 1)) {
                        SDL_Log("cursor Position, %d %d \n", position.getPos()[0], position.getPos()[1]);

                        if (ontile) {
                            toset = GAME::STATE::UNITMOVE;
                            setter = ontile;
                        } else {
                            toset = GAME::STATE::OPTIONS;
                            setter = &ent;
                        }
                    } else if ((game->getState() == GAME::STATE::UNITMOVE) && (frames_button == 1)) {
                        toset = GAME::STATE::UNITMENU;
                        setter = &ent;
                    }

                    if (toset != -1) {
                        game->setState(*setter, toset);
                    }
                }

                if (keyboard.is_pressed(kb_state, keyboardInputMap.cancel)) {
                    pressed_button.push_back(keyboardInputMap.cancel);

                    if ((game->getState() == GAME::STATE::UNITMENU) ||
                            (game->getState() == GAME::STATE::OPTIONS) ||
                            (game->getState() == GAME::STATE::UNITMOVE)) {
                        game->setState(ent, GAME::STATE::MAP);
                    }
                }

                keyboard.check_move(pressed_move);
                keyboard.check_button(pressed_button);
            });
            es.each<GamepadController, PositionComponent>([dt, this](entityx::Entity ent, GamepadController & gamepad, PositionComponent & position) {

            });
        }

};

#endif /* SYSTEMS_HPP */