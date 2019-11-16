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
        PositionComponent * positioncomponent; // Sets baground
        std::vector<SDL_Rect> srcrects; // background always first?
        std::vector<SDL_Rect> destrects; // background always first?
        int linespacing = 10;
        float spacingfactor = 1.1;
        int padding[4] = {10, 10, 10, 10}; //html style: up right down left
        int fontsize;

    public:
        TextComponent() {
        };

        void setFontsize(int in_fontsize) {
            fontsize = in_fontsize;
            linespacing = (int)(fontsize * spacingfactor);
        }

        void setSpacingfactor(float in_factor) {
            spacingfactor = in_factor;
        }

        void setLinespacing(int in_spacing) {
            linespacing = in_spacing;
        }


        TextComponent(int in_fontsize, std::vector<std::string> in_text, std::vector<SDL_Color> in_textColor) {
            text_lines = in_text;

            for (int i = 0; i < in_text.size(); i++) {
                if (in_textColor.size() == 1) {
                    addTextTexture(in_text[i], in_textColor[0]);
                } else {
                    addTextTexture(in_text[i], in_textColor[i]);
                }
            }

            setFontsize(in_fontsize);
        }

        TextComponent(int in_fontsize, std::vector<std::string> in_text, SDL_Color in_textColor) {
            text_lines = in_text;

            for (int i = 0; i < in_text.size(); i++) {
                addTextTexture(in_text[i], in_textColor);
            }

            setFontsize(in_fontsize);
        }

        TextComponent(int in_fontsize, std::string in_text, SDL_Color textColor) {
            text_lines.push_back(in_text);
            addTextTexture(in_text, textColor);
            setFontsize(in_fontsize);
        }

        std::vector<SDL_Texture *> getTextures() {
            return (textures);
        }

        void setDestrect() {

        }

        void addTextTexture(std::string in_text, SDL_Color in_textColor) {
            textures.push_back(TextureManager::loadFromRenderedText(in_text, in_textColor));
            srcrects.push_back(SDL_Rect{});
            destrects.push_back(SDL_Rect{});
        }

        void addBackgroundTexture(const char * filename) {
            textures.insert(textures.begin(), TextureManager::loadFromFile(filename));
            srcrects.insert(srcrects.begin(), SDL_Rect{});
            destrects.insert(destrects.begin(), SDL_Rect{});
        }

        void removeTexture(unsigned int index = -1) {
            if (index == -1) {
                textures.pop_back();
            } else {
                textures.erase(textures.begin() + index);
            }
        }
        void wraptext() {
            // Should wrap text inside the baground texture and pâdding.
        }
        void setRects() {
            srcrects[0].x = srcrects[0].y = 0;
            destrects[0].x = (int)positioncomponent->getPos().x + padding[3];
            destrects[0].y = (int)positioncomponent->getPos().y + padding[0];

            for (int i = 0; i < text_lines.size(); i++) {
                srcrects[i].h = fontsize;
                srcrects[i].w = text_lines[i].length() * fontsize;
                destrects[i].h = fontsize;
                destrects[i].w = text_lines[i].length() * fontsize;
                destrects[i].x = destrects[0].x;
                destrects[i].y = destrects[0].y + linespacing * i;
            }
        }

        virtual void init() override {
            positioncomponent = &entity->getComponent<PositionComponent>();
            setRects();
        }

        virtual void update() override {
        }

        void draw() override {

            for (int i = 0; i < textures.size(); i++) {
                TextureManager::draw(textures[i], srcrects[i], destrects[i]);
            }
        }
};

#endif /* TEXTCOMPONENT_HPP */