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
        
        SDL_Rect srcRect, destRect;
    public:
        SpriteComponent() = default;
        
        SpriteComponent(const char* path){
            setTex(path);
        }
        
        void setTex(const char * path) {
            texture = TextureManager::LoadTexture(path);
        }
        
        void init() override {
            positioncomponent = &entity->getComponent<PositionComponent>();
            srcRect.x = srcRect.y = 0;
            srcRect.w = srcRect.h = 32;
            destRect.w = destRect.h = 32;

        }
        
        void update() override {
            destRect.x = (int)positioncomponent->position.x;
            destRect.y = (int)positioncomponent->position.y;
        }
        
        void draw() override {
            TextureManager::Draw(texture, srcRect, destRect);
        }
};

#endif /* SPRITECOMPONENT_HPP */