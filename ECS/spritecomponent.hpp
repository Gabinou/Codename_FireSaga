#ifndef SPRITECOMPONENT_HPP
#define SPRITECOMPONENT_HPP

#include "ECS.hpp"
#include "components.hpp"
#include "map.hpp"
#include "positioncomponent.hpp"
#include "texturemanager.hpp"
#include "SDL2/SDL.h"

class SpriteComponent : public Component, Tilesize{
    
    private:
        PositionComponent *positioncomponent;
        SDL_Texture *texture;
        SDL_Rect srcrect, destrect;
        // const Map * currentmap;
        int * currenttilesize;
        
    public:
        SpriteComponent() = default;
        
        SpriteComponent(const char* path){
            setTexture(path);
        }
        
        SDL_Texture * getTexture() { 
            return(texture);
        }
        
        void setTexture(const char * path) {
            texture = TextureManager::loadTexture(path);
        }
        
        void setMap(Map const & inmap) {
            currenttilesize = (inmap.getTilesize());
            printf("%d", currenttilesize[0]);
            printf("%d", currenttilesize[1]);
        }

        void init() override {
            positioncomponent = &entity->getComponent<PositionComponent>();
            srcrect.x = srcrect.y = 0;
            srcrect.w = srcrect.h = 32;
            destrect.w = destrect.h = 32;
        }
        
        void update() override {
            // currentmap->getTilesize()[0];
            // printf("%d \n", currentmap->getTilesize()[0]);
            // printf("%d \n", a[0]);
            // static int * currenttilesize = (getTilesize());
            destrect.x = positioncomponent->position.x * currenttilesize[0];
            // destrect.y = (int)positioncomponent->position.y * currenttilesize[1];
        }
        
        void draw() override {
            TextureManager::draw(texture, srcrect, destrect);
        }
};

#endif /* SPRITECOMPONENT_HPP */