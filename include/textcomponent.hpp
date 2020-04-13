#ifndef TEXTCOMPONENT_HPP
#define TEXTCOMPONENT_HPP

#include <vector>
#include "map.hpp"
#include "filesystem.hpp"
#include "SDL2/SDL.h"
#include "game.hpp"


class TextComponent {

    private:
        std::vector<SDL_Texture *> textures;
        std::vector<std::string> text_lines;
        std::vector<SDL_Rect> srcrects; // background always first?
        std::vector<SDL_Rect> destrects; // background always first?
        std::vector<SDL_Color> textColor;

        int linespacing = 10;
        int padding[4] = {10, 10, 10, 10}; //html style: up right down left
        int fontsize;

        bool visible = true;
        float spacingfactor = 1.1;
        float sizefactor[2] = {0.5, 0.5}; // height, width

    public:
        TextComponent() {};

        void setFontsize(int in_fontsize) {
            fontsize = in_fontsize;
            linespacing = (int)(fontsize * sizefactor[0] * spacingfactor);
        }

        void show() {
            visible = true;
        }

        void hide() {
            visible = false;
        }
        bool isVisible() {
            return (visible);
        }

        void setSpacingfactor(float in_factor) {
            spacingfactor = in_factor;
        }

        void setSizefactor(float in_factor[2]) {
            sizefactor[0] = in_factor[0];
            sizefactor[1] = in_factor[1];
        }

        void setLinespacing(int in_spacing) {
            linespacing = in_spacing;
        }

        TextComponent(int in_fontsize, std::vector<std::string> in_text, std::vector<SDL_Color> in_textColor) {
            text_lines = in_text;
            textColor = in_textColor;

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
            textColor.push_back(in_textColor);

            for (int i = 0; i < in_text.size(); i++) {
                addTextTexture(in_text[i], in_textColor);
            }

            setFontsize(in_fontsize);
        }

        TextComponent(int in_fontsize, std::string in_text, SDL_Color in_textColor) {
            text_lines.push_back(in_text);
            textColor.push_back(in_textColor);
            addTextTexture(in_text, in_textColor);
            setFontsize(in_fontsize);
        }

        void setText(std::string in_text) {
            if (text_lines.size() == 0) {
                text_lines.push_back(in_text);
            } else {
                text_lines[0] = in_text;
            }

            removeTexture();
            addTextTexture(in_text, textColor[0]);
        }

        std::vector<SDL_Texture *> getTextures() {
            return (textures);
        }

        void addTextTexture(std::string in_text, SDL_Color in_textColor) {
            textures.push_back(textToTexture(Game::renderer, in_text, in_textColor, Game::font));
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

        void removeTextures(unsigned int index = -1) {
            textures.clear();
        }

        void wraptext() {
            // Should wrap text inside the background texture and pâdding.
        }

        void setRects(short int * in_position) {
            setRects(in_position[0], in_position[1]);
        }

        void setRects(short int x, short int y) {
            srcrects[0].x = srcrects[0].y = 0;
            destrects[0].x = x + padding[3];
            destrects[0].y = y + padding[0];
            int hgt, wdt;

            for (int i = 0; i < textures.size(); i++) {
                SDL_QueryTexture(textures[i], NULL, NULL, &hgt, &wdt);

                srcrects[i].h = fontsize;
                srcrects[i].w = text_lines[i].length() * fontsize;
                // destrects[i].h = hgt;
                destrects[i].h = (int)fontsize * sizefactor[0];
                // destrects[i].w = wdt;
                destrects[i].w = (int)text_lines[i].length() * fontsize * sizefactor[1];
                destrects[i].x = destrects[0].x;
                destrects[i].y = destrects[0].y + linespacing * i;
            }
        }

        void draw() {
            // Find a way to draw text letter by letter, word by word, etc. for future script.
            if (visible) {
                for (int i = 0; i < textures.size(); i++) {
                    SDL_RenderCopy(Game::renderer, textures[i], &srcrects[i], &destrects[i]);
                }
            }
        }
};

#endif /* TEXTCOMPONENT_HPP */