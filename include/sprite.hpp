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

        short int frames = 10, speed = 50;
        short int objectivepos[2];
        short int slidepos[2];
        short int slideint = 0; // for slide_type = "geometric"
        short unsigned int tilesize[2];
        float slidefactors[2] = {2, 1.025}; // for slide_type = "geometric"

        bool visible;
        bool animated = false;
        std::string asset_name;
        unsigned char ss_looping = LOOPING::PINGPONG; //ss: spritesheet
        unsigned char slidetype = 0;
    public:
        Sprite() = default;
        Sprite(const char * in_asset_name);
        Sprite(const char * in_asset_name, int in_picsize[2]);
        Sprite(const char * in_asset_name, int inFrames, int inSpeed);

        void init(short int * in_position);

        SDL_Texture * getTexture();

        float * getSlidefactors();

        void hide();
        void show();
        bool isAnimated();
        bool isVisible();

        unsigned char getSs_looping();

        short int getFrames();
        short int getSpeed();

        short int * getSlidepos();
        short int * getObjpos();
        void setSlidepos(short int * in_slidepos);
        void setObjpos(short int * in_objectivepos);

        unsigned char getSlidetype();
        void setSlidetype(unsigned char in_slidetype, short unsigned int * in_tilesize);
        void setDestrectpos(short int * in_position);
        void setTexture(const char * in_asset_name);

        void setAnimation(int inFrames, int inSpeed) ;

        short int getSlideint();

        void setSrcrect(SDL_Rect in_rect);
        void setSrcrect(int width, int height);
        void setDestrect(SDL_Rect in_rect);
        void setDestrect(int width, int height);

        SDL_Rect getSrcrect();
        SDL_Rect getDestrect();

        void draw();
};

#endif /* SPRITE_HPP */