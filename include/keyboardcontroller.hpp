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
    public:

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
            InputMap inputMap = manager.getGame()->getInputMap();

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
                if (manager.getGame()->getState() == "map") {
                    Entity & textbox = manager.addEntity();
                    textbox.addComponent<PositionComponent>();
                    textbox.getComponent<PositionComponent>().setBounds(0, 2000, 0, 2000);
                    textbox.getComponent<PositionComponent>().setPos(
                        (int)(positioncomponent->getPos().x * tilesize[0]),
                        (int)(positioncomponent->getPos().y * tilesize[1]));
                    SDL_Color black = {255, 255, 255};
                    textbox.addComponent<SpriteComponent>("..//assets//textbox.png", (int []) {128, 128}); // because there is no map. Position component should
                    textbox.addComponent<TextComponent>(manager.getGame()->getFontsize(), std::vector<std::string> {"Attack", "Wait"}, black);
                    textbox.addGroup(manager.groupUI);
                    textboxptr = &textbox;
                    manager.getGame()->setState("unitmenu");
                    printf("%s \n", manager.getGame()->getState().c_str());
                    entity->getComponent<SpriteComponent>().hide();
                }
            }

            if (is_pressed(kb_state, inputMap.cancel)) {
                if (manager.getGame()->getState() == "unitmenu") {
                    textboxptr->destroy();
                    manager.getGame()->setState("map");
                    printf("%s \n", manager.getGame()->getState().c_str());
                }
            }

            check_pressed(pressed);
            positioncomponent->setUpdatable(false);
        }

};




#endif /* KEYBOARDCONTROL_HPP */