#ifndef SPRITECOMPONENT_HPP
#define SPRITECOMPONENT_HPP

#include "ECS.hpp"
#include "components.hpp"
#include "positioncomponent.hpp"
#include "texturemanager.hpp"
#include "SDL2/SDL.h"

class SpriteComponent : public Component {
    
    private:
        PositionComponent *position;
        SDL_Texture *texture;
        
        SDL_Rect srcRect, destRect;
    public:
        SpriteComponent() = default;
        
        SpriteComponent(const char* path){
            printf("Loading texture\n");
            texture = TextureManager::LoadTexture(path);
            printf("Loaded texture\n");
        }
        
        void init() override {
            printf("Init sprite\n");
            
            position = &entity->getComponent<PositionComponent>();
            printf("Inited sprite\n");
            srcRect.x = srcRect.y = 0;
            srcRect.w = srcRect.h = 32;
            destRect.w = destRect.h = 32;

        }
        
        void update() override {
            printf("Update sprite\n");
            destRect.x = position->x();
            destRect.y = position->y();
        }
        
        void draw() override {
            printf("Drawing texture\n");
            TextureManager::Draw(texture, srcRect, destRect);
        }
};

#endif /* SPRITECOMPONENT_HPP */