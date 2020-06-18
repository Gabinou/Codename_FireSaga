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
    std::vector<SDL_Color> text_color;
    std::vector<SDL_Rect> srcrects; // background always first?
    std::vector<SDL_Rect> destrects; // background always first?

    int16_t linespacing = 10;
    int16_t padding[4] = {10, 10, 10, 10}; //html style: up right down left
    int16_t fontsize;

    bool visible = true;
    float spacingfactor = 1.1;
    float sizefactor[2] = {0.5, 0.5}; // height, width

public:
    Text();
    Text(int16_t in_fontsize);

    void setFontsize(int16_t in_fontsize);

    void show();
    void hide();
    bool isVisible();

    void setSpacingfactor(float in_factor);
    void setSizefactor(float in_factor[2]);
    void setLinespacing(int16_t in_spacing);
    int16_t getLinespacing();

    void setColor(std::vector<SDL_Color> in_text_color);
    void setColor(SDL_Color in_text_color);

    void setText(const char * in_text);
    void setText(std::string in_text);
    void setText(std::vector<std::string> in_text);

    std::vector<SDL_Texture *> getTextures();
    void makeTextures();
    void addTextTexture(std::string in_text, SDL_Color in_text_color);
    void removeTexture(int16_t index = -1);
    void removeTextures();
    void wraptext();

    void setRects(int16_t * in_position);
    void setRects(Point in_position);
    void setRects(int16_t x, int16_t y);

    void draw();
};

#endif /* TEXT_HPP */