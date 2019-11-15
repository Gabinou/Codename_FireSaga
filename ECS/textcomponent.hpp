#ifndef TEXTCOMPONENT_HPP
#define TEXTCOMPONENT_HPP

#include "ECS.hpp"
#include "components.hpp"
#include "map.hpp"
#include "vector2D.hpp"
#include <vector>
#include "shared.hpp"
#include "positioncomponent.hpp"
#include "texturemanager.hpp"
#include "SDL2/SDL.h"


// This component should: use the position component to set its position.
// How will it interact with textBoxComponent if there is no position component?

class TextComponent : public Component {

    private:
        std::vector<SDL_Texture *> textures; // background always first
        std::vector<std::string> text_lines;
        std::vector<SDL_Color> text_colors;

    protected:
        PositionComponent * positioncomponent;
        SDL_Rect srcrect, destrect;
        std::vector<SDL_Rect> srcrects;
        std::vector<SDL_Rect> destrects;

    public:
        TextComponent() {
        };

        TextComponent(std::vector<std::string> in_text, std::vector<SDL_Color> in_textColor) {
            for (int i = in_text.begin(); i < in_text.end(); i++) {
                if (in_textColor.size() == 1) {
                    addTextTexture(in_text[i], in_textColor[0]);
                } else {
                    addTextTexture(in_text[i], in_textColor[i]);
                }
            }
        }

        TextComponent(std::vector<std::string> in_text, SDL_Color in_textColor) {
            for (int i = in_text.begin(); i < in_text.end(); i++) {
                addTextTexture(in_text[i], in_textColor);
            }
        }

        TextComponent(std::string text, SDL_Color textColor) {
            addTextTexture(text, textColor);
        }

        std::vector<SDL_Texture *> getTextures() {
            return (textures);
        }

        void addTextTexture(std::string in_text, SDL_Color in_textColor) {
            textures.push_back(TextureManager::loadFromRenderedText(in_text, in_textColor));
        }

        void addBackgroundTexture(std::string filename) {
            textures.insert(texture.begin(), TextureManager::loadFromFile(filename));
        }

        void removeTexture(unsigned int index = -1) {
            if (index == -1) {
                textures.pop_back();
            } else {
                textures.erase(textures.begin() + index);
            }
        }

        void setPosition(short unsigned int x, short unsigned int y) {
            positioncomponent = &entity->getComponent<PositionComponent>();
            positioncomponent->setPos(x, y);
        }

        virtual void init() override {
            positioncomponent = &entity->getComponent<PositionComponent>();
            printf("init text boc");
            srcrect[0].x = srcrect[0].y = 0;
            destrect[0].x = destrect[0].y = 60;
            srcrect[0].w = 1000;
            srcrect[0].h = 56;
            destrect[0].w = 200;
            destrect[0].h = 56;
        }

        virtual void update() override {
        }

        void draw() override {
            for (int i = textures.begin(); i < textures.end(); i++) {
                TextureManager::draw(textures[i], srcrects[i], destrects[i]);
            }
        }
};

#endif /* TEXTCOMPONENT_HPP */