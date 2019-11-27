#ifndef TILECOMPONENT_HPP
#define TILECOMPONENT_HPP

#include "ECS.hpp"
#include "SDL2/SDL.h"


class TileComponent : public Component {

    private:
        SDL_Texture * texture;
        SDL_Rect srcrect, destrect;
        Entity * onTile;
    public:
        TileComponent() = default;
        ~TileComponent() {
            SDL_DestroyTexture(texture);
        }

};

#endif /* TILECOMPONENT_HPP */