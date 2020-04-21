#ifndef TEXT_HPP
#define TEXT_HPP

#include <vector>
#include "map.hpp"
#include "filesystem.hpp"
#include "SDL2/SDL.h"
#include "game.hpp"


class Text {

private:
    std::vector<SDL_Texture *> textures;
    std::vector<std::string> text_lines;
    std::vector<SDL_Rect> srcrects; // background always first?
    std::vector<SDL_Rect> destrects; // background always first?
    std::vector<SDL_Color> textColor;

    short int linespacing = 10;
    short int padding[4] = {10, 10, 10, 10}; //html style: up right down left
    short int fontsize;

    bool visible = true;
    float spacingfactor = 1.1;
    float sizefactor[2] = {0.5, 0.5}; // height, width

public:
    Text();
    Text(short int in_fontsize, std::string in_text, SDL_Color in_textColor);
    Text(short int in_fontsize, std::vector<std::string> in_text, SDL_Color in_textColor);
    Text(short int in_fontsize, std::vector<std::string> in_text, std::vector<SDL_Color> in_textColor);

    void setFontsize(short int in_fontsize);

    void show();
    void hide();
    bool isVisible();

    void setSpacingfactor(float in_factor);
    void setSizefactor(float in_factor[2]);
    void setLinespacing(short int in_spacing);
    short int getLinespacing();

    void setText(std::string in_text);

    std::vector<SDL_Texture *> getTextures();
    void addTextTexture(std::string in_text, SDL_Color in_textColor);
    void removeTexture(short int index = -1);
    void removeTextures(short int index = -1);
    void wraptext();

    void setRects(short int * in_position);
    void setRects(short int x, short int y);

    void draw();
};

#endif /* TEXT_HPP */