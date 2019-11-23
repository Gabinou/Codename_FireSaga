#ifndef KEYBOARDCONTROL_HPP
#define KEYBOARDCONTROL_HPP
#include "ECS.hpp"
#include "textcomponent.hpp"
#include "game.hpp"
#include "keyboardinputmapping.hpp"

struct LastPressed {
    std::vector<std::vector<SDL_Scancode>> keys;
    unsigned int pressed_frames = 0;
};

class KeyboardController : public Component, Tilesize, KeyboardInputMapping {

    private:
        LastPressed lastpressed;
        bool textbox_shown = false;
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

        void check_pressed(std::vector<std::vector<SDL_Scancode>>pressed) {
            if ((lastpressed.keys == pressed) && (!pressed.empty())) {
                lastpressed.pressed_frames++;
            } else {
                lastpressed.keys = pressed;
                lastpressed.pressed_frames = 0;
            }
        }

        void update() override {
            const Uint8 * keyboard_state_array = SDL_GetKeyboardState(NULL);
            std::vector<std::vector<SDL_Scancode>> current_pressed{};

            if (is_pressed(keyboard_state_array, moveup) && !is_pressed(keyboard_state_array, movedown)) {
                positioncomponent->addPos(Vector2D(0, -1));
                current_pressed.push_back(moveup);
            } else if (!is_pressed(keyboard_state_array, moveup) && is_pressed(keyboard_state_array, movedown)) {
                positioncomponent->addPos(Vector2D(0, 1));
                current_pressed.push_back(movedown);
            }

            if (!is_pressed(keyboard_state_array, moveright) && is_pressed(keyboard_state_array, moveleft)) {
                positioncomponent->addPos(Vector2D(-1, 0));
                printf("Left\n");
                printf("%d\n", positioncomponent->getPos().x);
                current_pressed.push_back(moveleft);
            } else if (is_pressed(keyboard_state_array, moveright) && !is_pressed(keyboard_state_array, moveleft)) {
                positioncomponent->addPos(Vector2D(1, 0));
                printf("Right\n");
                printf("%d\n", positioncomponent->getPos().x);
                current_pressed.push_back(moveright);
            }

            if (is_pressed(keyboard_state_array, accept)) {
                if (textbox_shown == false) {
                    Manager & current_manager = entity->getManager();
                    Entity & textbox = current_manager.addEntity();

                    textbox.addComponent<PositionComponent>(
                        (int)positioncomponent->getPos().x * tilesize[0],
                        (int)positioncomponent->getPos().y * tilesize[1]);
                    SDL_Color black = {255, 255, 255};
                    textbox.addComponent<SpriteComponent>("..//assets//textbox.png", (int []) {128, 128}); // because there is no map. Position component should
                    textbox.addComponent<TextComponent>(current_manager.getGame()->fontsize, std::vector<std::string> {"Attack", "Wait"}, black);
                    textbox.addGroup(current_manager.groupUI);
                    textboxptr = &textbox;
                    textbox_shown = !textbox_shown;
                }
            }

            if (is_pressed(keyboard_state_array, cancel)) {
                if (textbox_shown == true) {
                    textboxptr->destroy();
                    textbox_shown = !textbox_shown;
                }
            }

            check_pressed(current_pressed);
            positioncomponent->setUpdatable(false);
        }

};




#endif /* KEYBOARDCONTROL_HPP */