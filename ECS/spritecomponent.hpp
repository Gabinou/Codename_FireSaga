#ifndef SPRITECOMPONENT_HPP
#define SPRITECOMPONENT_HPP

#include "ECS.hpp"
#include "components.hpp"
#include "map.hpp"
#include "vector2D.hpp"
#include "shared.hpp"
#include "positioncomponent.hpp"
#include "texturemanager.hpp"
#include "SDL2/SDL.h"

class SpriteComponent : public Component{
    
    private:    
        SDL_Texture *texture;

    protected:
        PositionComponent *positioncomponent;
        SDL_Rect srcrect, destrect;
        Map * currentmap;
        Vector2D objectivepos;
        Vector2D slidepos;
        int * tilesize;
        bool animated = false;
        int frames = 0;
        int speed = 100;

    public:
        SpriteComponent() = default;
        
        SpriteComponent(Map * inmap, const char* path){
            setTexture(path);
            setMap(inmap);
        }
        
        SpriteComponent(Map * inmap, const char* path, int nFrames, int mSpeed){
            setTexture(path);
            setMap(inmap);
            animated = true;
            frames = nFrames;
            speed = mSpeed;
        }
        
        SDL_Texture * getTexture() { 
            return(texture);
        }
        
        void setTexture(const char * path) {
            texture = TextureManager::loadTexture(path);
        }
        
        void setMap(Map * inmap) {
            currentmap = inmap;
            tilesize = currentmap->getTilesize();
        }
        
        void setSrcrect(int width, int height){
            srcrect.w = width;  
            srcrect.h = height;
        }

        virtual void init() override {
            positioncomponent = &entity->getComponent<PositionComponent>();
            srcrect.x = srcrect.y = 0;
            srcrect.w = srcrect.h = 32;
            destrect.w = destrect.h = 32;
            slidepos.x = (int)positioncomponent->getPos().x * tilesize[0];
            slidepos.y = (int)positioncomponent->getPos().y * tilesize[1];
        }
        
        virtual void update() override {
            objectivepos.x = (int)positioncomponent->getPos().x * tilesize[0];
            objectivepos.y = (int)positioncomponent->getPos().y * tilesize[1]; 
            if (animated) {
                // iterate linearly through spritesheet
                // srcrect.x = srcrect.w * static_cast<int>((SDL_GetTicks()/speed) % frames); 
                // iterate ping-pongly through spritesheet
                srcrect.x = srcrect.w * std::min((2*frames - 1) - static_cast<int>((SDL_GetTicks()/speed) % (2*frames - 1)), static_cast<int>((SDL_GetTicks()/speed) % (2*frames - 1))); 
                printf("%d %d \n ",srcrect.w, std::min((2*frames - 1) - static_cast<int>((SDL_GetTicks()/speed) % (2*frames - 1)), static_cast<int>((SDL_GetTicks()/speed) % (2*frames - 1))));
            }
            destrect.x = slidepos.x;
            destrect.y = slidepos.y;
        }
        
        void draw() override {
            TextureManager::draw(texture, srcrect, destrect);
        }
};

#endif /* SPRITECOMPONENT_HPP */