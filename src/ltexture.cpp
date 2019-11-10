#include "ltexture.hpp"


LTexture::LTexture(){
    
}

LTexture::~LTexture(){
    
}

void LTexture::free(){
    
}


bool LTexture::loadFromRenderedText(std::string textureText, SDL_Color textColor)
{
    //Get rid of preexisting texture
    free();

    //Render text surface

    SDL_Surface* textSurface = TTF_RenderText_Solid(Game::font, textureText.c_str(), textColor);
    if (textSurface == NULL) {
        printf( "Unable to render text surface! SDL_ttf Error: %s\n", TTF_GetError() );
    } else {
        //Create texture from surface pixels
        mTexture = SDL_CreateTextureFromSurface( Game::renderer, textSurface );
        if( mTexture == NULL )
        {
            printf( "Unable to create texture from rendered text! SDL Error: %s\n", SDL_GetError() );
        }
        else
        {
            //Get image dimensions
            mWidth = textSurface->w;
            mHeight = textSurface->h;
        }

        //Get rid of old surface
        SDL_FreeSurface(textSurface);
    }
    
    // //Return success
    // return mTexture != NULL;
    return(false);
}