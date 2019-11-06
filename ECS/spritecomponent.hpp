#ifndef SPRITECOMPONENT_HPP
#define SPRITECOMPONENT_HPP

#include "ECS.hpp"
#include "components.hpp"
#include "map.hpp"
#include "positioncomponent.hpp"
#include "texturemanager.hpp"
#include "SDL2/SDL.h"

class SpriteComponent : public Component{
    
    private:
        PositionComponent *positioncomponent;
        SDL_Texture *texture;
        SDL_Rect srcrect, destrect;
        Map * currentmap;
        int * currenttilesize;
        
    public:
        SpriteComponent() = default;
        
        SpriteComponent(Map * inmap, const char* path){
            setTexture(path);
            setMap(inmap);
        }
        
        SDL_Texture * getTexture() { 
            return(texture);
        }
        
        void setTexture(const char * path) {
            texture = TextureManager::loadTexture(path);
        }
        
        void setMap(Map * inmap) {
            currentmap = inmap;
            currenttilesize = currentmap->getTilesize();
        }

        void init() override {
            positioncomponent = &entity->getComponent<PositionComponent>();
            srcrect.x = srcrect.y = 0;
            srcrect.w = srcrect.h = 32;
            destrect.w = destrect.h = 32;
        }
        
        void update() override {
            destrect.x = (int)positioncomponent->position.x * currenttilesize[0];
            destrect.y = (int)positioncomponent->position.y * currenttilesize[1];
        }
        
        void draw() override {
            TextureManager::draw(texture, srcrect, destrect);
        }
};

#endif /* SPRITECOMPONENT_HPP */