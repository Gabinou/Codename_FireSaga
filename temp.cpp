#ifndef KEYBOARDCONTROL_HPP
#define KEYBOARDCONTROL_HPP

#include "ECS.hpp"
#include "game.hpp"
#include "map.hpp"
#include "utilities.hpp"

class KeyboardController : public Component {
    private:
        Entity * textboxptr;
        PositionComponent * positioncomponent;
        Game * game;
        Map * map;
        KeyboardInputMap inputmap;
        std::vector<std::vector<SDL_Scancode>> held_button;
        std::vector<std::vector<SDL_Scancode>> held_move;
        unsigned int frames_button = 0;
        unsigned int frames_move = 0;
        short unsigned int * tilesize;
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

        void setMap(Map * in_map) {
            map = in_map;
        }

        void setTilesize(short unsigned int * in_tilesize) {
            tilesize = in_tilesize;
        }

        short unsigned int * getTilesize() {
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

        void switchCursor() {

        }

        int getHeldmove() {
            return (frames_move);
        }

        void check_move(std::vector<std::vector<SDL_Scancode>>in_pressed) {
            if ((held_move == in_pressed) && (!in_pressed.empty())) {
                frames_move++;
            } else {
                held_move = in_pressed;
                frames_move = 0;
            }
        }

        void check_button(std::vector<std::vector<SDL_Scancode>>in_pressed) {
            if ((held_button == in_pressed) && (!in_pressed.empty())) {
                frames_button++;
            } else {
                held_button = in_pressed;
                frames_button = 0;
            }
        }

        bool is_pressed(const Uint8 * state_array, std::vector<SDL_Scancode> to_find) {
            for (auto it = std::begin(to_find); it != std::end(to_find); ++it) {
                if (state_array[*it]) {
                    return (true);
                }
            }

            return (false);
        }


        void update() override {
            const Uint8 * kb_state = SDL_GetKeyboardState(NULL);
            std::vector<std::vector<SDL_Scancode>> pressed_move{};
            std::vector<std::vector<SDL_Scancode>> pressed_button{};

            if (is_pressed(kb_state, inputmap.moveup) && !is_pressed(kb_state, inputmap.movedown)) {
                positioncomponent->addPos(0, -1);
                pressed_move.push_back(inputmap.moveup);
            } else if (!is_pressed(kb_state, inputmap.moveup) && is_pressed(kb_state, inputmap.movedown)) {
                positioncomponent->addPos(0, 1);
                pressed_move.push_back(inputmap.movedown);
            }

            if (!is_pressed(kb_state, inputmap.moveright) && is_pressed(kb_state, inputmap.moveleft)) {
                positioncomponent->addPos(-1, 0);
                pressed_move.push_back(inputmap.moveleft);
            } else if (is_pressed(kb_state, inputmap.moveright) && !is_pressed(kb_state, inputmap.moveleft)) {
                positioncomponent->addPos(1, 0);
                pressed_move.push_back(inputmap.moveright);
            }

            if (is_pressed(kb_state, inputmap.accept)) {
                pressed_button.push_back(inputmap.accept);
                short int toset = -1;
                Entity * setter;
                Entity * ontile = map->getTile(positioncomponent->getPos()[0], positioncomponent->getPos()[1]);

                if ((game->getState() == GAME::STATE::MAP) && (frames_button == 1)) {
                    SDL_Log("cursor Position, %d %d \n", positioncomponent->getPos()[0], positioncomponent->getPos()[1]);

                    if (ontile) {
                        toset = GAME::STATE::UNITMOVE;
                        setter = ontile;
                    } else {
                        toset = GAME::STATE::OPTIONS;
                        setter = entity;
                    }
                } else if ((game->getState() == GAME::STATE::UNITMOVE) && (frames_button == 1)) {
                    toset = GAME::STATE::UNITMENU;
                    setter = entity;
                }

                if (toset != -1) {
                    game->setState(*setter, toset);
                }
            }

            if (is_pressed(kb_state, inputmap.cancel)) {
                pressed_button.push_back(inputmap.cancel);

                if ((game->getState() == GAME::STATE::UNITMENU) ||
                        (game->getState() == GAME::STATE::OPTIONS) ||
                        (game->getState() == GAME::STATE::UNITMOVE)) {
                    game->setState(*entity, GAME::STATE::MAP);
                }
            }

            check_move(pressed_move);
            check_button(pressed_button);
        }
};



#endif /* KEYBOARDCONTROL_HPP */