#ifndef KEYBOARDCONTROL_HPP
#define KEYBOARDCONTROL_HPP

#include "ECS.hpp"
#include "game.hpp"
#include "map.hpp"
#include "textcomponent.hpp"

struct LastPressed {
    std::vector<std::vector<SDL_Scancode>> keys;
    unsigned int pressed_frames = 0;
};

class KeyboardController : public Component {

    private:
        LastPressed lastpressed;
        Entity * textboxptr;
        PositionComponent * positioncomponent;
        int * tilesize;
        Game * game;
        Map * map;
        KeyboardInputMap inputmap;
    public:
        KeyboardController() = default;

        KeyboardController(Game * in_game) {
            setGame(in_game);
        }
        KeyboardController(Game * in_game, Map * in_map) : KeyboardController(in_game) {
            map = in_map;
        }

        void setGame(Game * in_game) {
            game = in_game;
        }
        void setTilesize(int * in_tilesize) {
            tilesize = in_tilesize;
        }

        int * getTilesize() {
            return (tilesize);
        }

        void updateInputmap() {
            inputmap = game->getKeyboardInputMap();
        }

        void init() override {
            positioncomponent = &entity->getComponent<PositionComponent>();
            inputmap = game->getKeyboardInputMap();
            Manager & manager = entity->getManager();
        }

        LastPressed getLastPressed() {
            return (lastpressed);
        }

        void switchCursor() {

        }

        void check_pressed(std::vector<std::vector<SDL_Scancode>>pressed) {
            if ((lastpressed.keys == pressed) && (!pressed.empty())) {
                lastpressed.pressed_frames++;
            } else {
                lastpressed.keys = pressed;
                lastpressed.pressed_frames = 0;
            }
        }

        void update() override {
            const Uint8 * kb_state = SDL_GetKeyboardState(NULL);
            std::vector<std::vector<SDL_Scancode>> pressed{};
            Entity * ontile = map->getTile(positioncomponent->getPos().x, positioncomponent->getPos().y);

            if (is_pressed(kb_state, inputmap.moveup) && !is_pressed(kb_state, inputmap.movedown)) {
                positioncomponent->addPos(Vector2D(0, -1));
                pressed.push_back(inputmap.moveup);
            } else if (!is_pressed(kb_state, inputmap.moveup) && is_pressed(kb_state, inputmap.movedown)) {
                positioncomponent->addPos(Vector2D(0, 1));
                pressed.push_back(inputmap.movedown);
            }

            if (!is_pressed(kb_state, inputmap.moveright) && is_pressed(kb_state, inputmap.moveleft)) {
                positioncomponent->addPos(Vector2D(-1, 0));
                pressed.push_back(inputmap.moveleft);
            } else if (is_pressed(kb_state, inputmap.moveright) && !is_pressed(kb_state, inputmap.moveleft)) {
                positioncomponent->addPos(Vector2D(1, 0));
                pressed.push_back(inputmap.moveright);
            }

            if (is_pressed(kb_state, inputmap.accept)) {
                std::string toset = "";
                Entity * setter;
                // printf("a\n");
                // printf("%d\n", lastpressed.pressed_frames);

                if ((game->getState() == "map") && (lastpressed.pressed_frames == 1)) {
                    printf("cursor Position, %d %d \n", positioncomponent->getPos().x, positioncomponent->getPos().y);

                    if (ontile) {
                        toset = "unitmove";
                        setter = ontile;
                    } else {
                        toset = "options";
                        setter = entity;
                    }
                } else if ((game->getState() == "unitmove") && (lastpressed.pressed_frames == 1)) {
                    toset = "unitmenu";
                    setter = entity;
                }

                if (toset != "") { game->setState(*setter, toset.c_str()); }

                pressed.push_back(inputmap.accept);
            }

            if (is_pressed(kb_state, inputmap.cancel)) {
                if ((game->getState() == "unitmenu") ||
                        (game->getState() == "options") ||
                        (game->getState() == "unitmove")) {
                    game->setState(*entity, "map");
                }

                pressed.push_back(inputmap.accept);
            }

            check_pressed(pressed);
            positioncomponent->setUpdatable(false);

        }
};



#endif /* KEYBOARDCONTROL_HPP */