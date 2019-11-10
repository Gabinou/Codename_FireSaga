#ifndef LTEXTURE_HPP
#define LTEXTURE_HPP

#include "SDL2/SDL.h"
#include "SDL2/SDL_image.h"
#include "SDL2/SDL_ttf.h"
#include "game.hpp"
#include <stdio.h>
#include <string>
#include <cmath>

//Texture wrapper class
class LTexture
{
    private:
        SDL_Texture* mTexture;

        int mWidth;
        int mHeight;
    public:
        LTexture();

        ~LTexture();

        bool loadFromFile(std::string path);
        
        //Creates image from font string
        bool loadFromRenderedText( std::string textureText, SDL_Color textColor );

        //Deallocates texture
        void free();

        //Set color modulation
        void setColor( Uint8 red, Uint8 green, Uint8 blue );

        void setBlendMode( SDL_BlendMode blending );

        void setAlpha( Uint8 alpha );
        
        void render( int x, int y, SDL_Rect* clip = NULL, double angle = 0.0, SDL_Point* center = NULL, SDL_RendererFlip flip = SDL_FLIP_NONE );

        int getWidth();
        int getHeight();
};
#endif /* LTEXTURE_HPP */