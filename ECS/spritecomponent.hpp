#ifndef SPRITECOMPONENT_HPP
#define SPRITECOMPONENT_HPP

#include "ECS.hpp"
#include "components.hpp"
#include "positioncomponent.hpp"
#include "texturemanager.hpp"
#include "SDL2/SDL.h"

class SpriteComponent : public Component {
    
    private:
        PositionComponent *positioncomponent;
        SDL_Texture *texture;
        SDL_Rect srcrect, destrect;
        
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

        void init() override {
            positioncomponent = &entity->getComponent<PositionComponent>();
            srcrect.x = srcrect.y = 0;
            srcrect.w = srcrect.h = 32;
            destrect.w = destrect.h = 32;

        }
        
        void update() override {
            destrect.x = (int)positioncomponent->position.x;
            destrect.y = (int)positioncomponent->position.y;
        }
        
        void draw() override {
            TextureManager::draw(texture, srcrect, destrect);
        }
};

#endif /* SPRITECOMPONENT_HPP */