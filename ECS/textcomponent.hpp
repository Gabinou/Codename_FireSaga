#ifndef TEXTCOMPONENT_HPP
#define TEXTCOMPONENT_HPP

#include <vector>
#include "ECS.hpp"
#include "map.hpp"
#include "shared.hpp"
#include "positioncomponent.hpp"
#include "SDL2/SDL.h"

// This component should: use the position component to set its position.
// How will it interact with textBoxComponent if there is no position component?

class TextComponent : public Component {

    private:
        std::vector<SDL_Texture *> textures;
        std::vector<std::string> text_lines;
        std::vector<SDL_Color> text_colors;
        PositionComponent * positioncomponent;
        std::vector<SDL_Rect> srcrects; // background always first?
        std::vector<SDL_Rect> destrects; // background always first?
        int linespacing = 10;
        float spacingfactor = 1.1;
        float sizefactor[2] = {0.75, 0.5}; // height, width
        int padding[4] = {10, 10, 10, 10}; //html style: up right down left
        int fontsize;

    public:
        TextComponent() {
        };

        void setFontsize(int in_fontsize) {
            fontsize = in_fontsize;
            linespacing = (int)(fontsize * sizefactor[0] * spacingfactor);
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

        void addTextTexture(std::string in_text, SDL_Color in_textColor) {
            textures.push_back(textToTexture(in_text, in_textColor, Game::font));
            srcrects.push_back(SDL_Rect{});
            destrects.push_back(SDL_Rect{});
        }

        void removeTexture(unsigned int index = -1) {
            if (index == -1) {
                textures.pop_back();
            } else {
                textures.erase(textures.begin() + index);
            }
        }
        void wraptext() {
            // Should wrap text inside the background texture and pâdding.
        }
        void initRects() {
            // printf("initted Rect");
            srcrects[0].x = srcrects[0].y = 0;
            destrects[0].x = (int)positioncomponent->getPos()[0] + padding[3];
            destrects[0].y = (int)positioncomponent->getPos()[1] + padding[0];

            for (int i = 0; i < textures.size(); i++) {
                srcrects[i].h = fontsize;
                srcrects[i].w = text_lines[i].length() * fontsize;
                destrects[i].h = (int)fontsize * sizefactor[0];
                destrects[i].w = (int)text_lines[i].length() * fontsize * sizefactor[1];
                destrects[i].x = destrects[0].x;
                destrects[i].y = destrects[0].y + linespacing * i;
            }
        }

        virtual void init() override {
            positioncomponent = &entity->getComponent<PositionComponent>();
            initRects();
        }

        virtual void update() override {
        }

        void draw() override {
            // Find a way to draw text letter by letter, word by word, etc. for future script.
            for (int i = 0; i < textures.size(); i++) {
                SDL_RenderCopy(Game::renderer, textures[i], &srcrects[i], &destrects[i]);
            }
        }
};

#endif /* TEXTCOMPONENT_HPP */