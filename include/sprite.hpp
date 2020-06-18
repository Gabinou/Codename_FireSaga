#ifndef SPRITE_HPP
#define SPRITE_HPP

#include "map.hpp"
#include "filesystem.hpp"
#include "position.hpp"
#include "keyboardcontroller.hpp"
#include "gamepadcontroller.hpp"
#include "game.hpp"
#include "utilities.hpp"
#include "SDL2/SDL.h"

class Sprite {

protected:
    SDL_Rect srcrect = {0, 0, 32, 32}; //x,y,w,h
    SDL_Rect destrect = {0, 0, 32, 32}; //x,y,w,h
    SDL_Texture * texture;

    int16_t frames = 10, speed = 50;
    int16_t objectivepos[2];
    int16_t slidepos[2];
    int16_t slideint = 0; // for slide_type = "geometric"
    uint16_t tilesize[2] = {32, 32};
    float slidefactors[2] = {2, 1.025}; // for slide_type = "geometric"

    bool visible = true;
    bool animated = false;
    std::string asset_name;
    uint8_t ss_looping = LOOPING::PINGPONG; //ss: spritesheet
    uint8_t slidetype = 0;
public:
    Sprite();
    Sprite(const char * in_asset_name);
    Sprite(const char * in_asset_name, int32_t in_picsize[2]);
    Sprite(const char * in_asset_name, int32_t inFrames, int32_t inSpeed);

    void init(int16_t * in_position);
    void init(Point in_position);

    SDL_Texture * getTexture();

    float * getSlidefactors();

    void hide();
    void show();
    void animate();
    void still();
    bool isAnimated();
    bool isVisible();

    uint8_t getSs_looping();

    int16_t getFrames();
    int16_t getSpeed();

    int16_t * getSlidepos();
    int16_t * getObjpos();
    void setSlidepos(int16_t * in_slidepos);
    void setObjpos(int16_t * in_objectivepos);

    void setTilesize(uint16_t * in_tilesize);
    void animatedRects();

    void darken();
    void lighten();

    uint8_t getSlidetype();
    void setSlidetype(uint8_t in_slidetype);
    void setDestrectpos(int16_t * in_position);
    void setDestrectpos(Point in_position);
    void setTexture(const char * in_asset_name);

    void setAnimation(int32_t inFrames, int32_t inSpeed) ;

    int16_t getSlideint();

    void setSrcrect(SDL_Rect in_rect);
    void setSrcrect(int32_t width, int32_t height);
    void setDestrect(SDL_Rect in_rect);
    void setDestrect(int32_t width, int32_t height);

    SDL_Rect getSrcrect();
    SDL_Rect getDestrect();

    void draw();
};

#endif /* SPRITE_HPP */