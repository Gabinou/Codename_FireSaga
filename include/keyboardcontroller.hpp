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

class KeyboardController : public Component, Tilesize {

    private:
        LastPressed lastpressed;
        Entity * textboxptr;
        PositionComponent * positioncomponent;
        int * tilesize;
        Game * game;
    public:
        KeyboardController() = default;

        KeyboardController(Game * in_game) {
            game = in_game;
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

        void init() override {
            positioncomponent = &entity->getComponent<PositionComponent>();
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
            Manager & manager = entity->getManager();
            InputMap inputMap = game->getInputMap();

            if (is_pressed(kb_state, inputMap.moveup) && !is_pressed(kb_state, inputMap.movedown)) {
                positioncomponent->addPos(Vector2D(0, -1));
                pressed.push_back(inputMap.moveup);
            } else if (!is_pressed(kb_state, inputMap.moveup) && is_pressed(kb_state, inputMap.movedown)) {
                positioncomponent->addPos(Vector2D(0, 1));
                pressed.push_back(inputMap.movedown);
            }

            if (!is_pressed(kb_state, inputMap.moveright) && is_pressed(kb_state, inputMap.moveleft)) {
                positioncomponent->addPos(Vector2D(-1, 0));
                pressed.push_back(inputMap.moveleft);
            } else if (is_pressed(kb_state, inputMap.moveright) && !is_pressed(kb_state, inputMap.moveleft)) {
                positioncomponent->addPos(Vector2D(1, 0));
                pressed.push_back(inputMap.moveright);
            }

            if (is_pressed(kb_state, inputMap.accept)) {
                if (game->getState() == "map") {
                    printf("cursor Position, %d %d \n", positioncomponent->getPos().x, positioncomponent->getPos().y);
                    // printf("An entity is here?, %d \n", (entity->getComponent<SpriteComponent>().getMap()->tile s[positioncomponent->getPos().x][positioncomponent->getPos().y] == nullptr));
                    Map * current_map = entity->getComponent<SpriteComponent>().getMap();
                    // current_map->onTile(0, 0);
                    // current_map->getTile(positioncomponent->getPos().x, positioncomponent->getPos().y);
                    // printf("Does this work? %d \n", (current_map->getTiles)));
                    // current_map->getTiles();
                    // Entity * tile = entity->getComponent<SpriteComponent>().getMap()->tiles[positioncomponent->getPos().x][positioncomponent->getPos().y];
                    game->setState(*entity, "unitmenu");
                }
            }

            if (is_pressed(kb_state, inputMap.cancel)) {
                if (game->getState() == "unitmenu") {
                    game->setState(*entity, "map");
                }
            }

            check_pressed(pressed);
            positioncomponent->setUpdatable(false);
        }

};




#endif /* KEYBOARDCONTROL_HPP */