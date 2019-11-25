#ifndef KEYBOARDCONTROL_HPP
#define KEYBOARDCONTROL_HPP

#include "ECS.hpp"
#include "game.hpp"
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
                    // idea: The entity calling the game change state should pass itself to the game. Then, the game would make the changes according to which entity passed the change state command. Implementing it.
                    printf("%d \n", manager.getEntities().size());
                    // Entity & textbox = manager.addEntity();

                    // manager.getEntities()[2]->addComponent<PositionComponent>();
                    // manager.getEntities()[2]->getComponent<PositionComponent>().setBounds(0, 2000, 0, 2000);
                    // manager.getEntities()[2]->getComponent<PositionComponent>().setPos(
                    // (int)(positioncomponent->getPos().x * tilesize[0]),
                    // (int)(positioncomponent->getPos().y * tilesize[1]));
                    // SDL_Color black = {255, 255, 255};
                    // manager.getEntities()[2]->addComponent<SpriteComponent>("..//assets//textbox.png", (int []) {128, 128}); // because there is no map. Position component should
                    // manager.getEntities()[2]->addComponent<TextComponent>(manager.getGame()->getFontsize(), std::vector<std::string> {"Attack", "Wait"}, black);
                    // manager.getEntities()[2]->addGroup(manager.groupUI);
                    // textboxptr = &textbox;
                    game->setState(*entity, "unitmenu");
                    // printf("%s \n", manager.getGame()->getState().c_str());
                    // entity->getComponent<SpriteComponent>().hide();
                }
            }

            if (is_pressed(kb_state, inputMap.cancel)) {
                if (game->getState() == "unitmenu") {
                    // textboxptr->destroy();
                    game->setState(*entity, "map");
                    // // entity->getComponent<SpriteComponent>().show();
                    // printf("%s \n", manager.getGame()->getState().c_str());
                }
            }

            check_pressed(pressed);
            positioncomponent->setUpdatable(false);
        }

};




#endif /* KEYBOARDCONTROL_HPP */