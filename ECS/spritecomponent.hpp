#ifndef SPRITECOMPONENT_HPP
#define SPRITECOMPONENT_HPP

#include "ECS.hpp"
#include "components.hpp"
#include "map.hpp"
#include "vector2D.hpp"
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
        Vector2D objectivepos;
        Vector2D slidepos;

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
            slidepos.x = (int)positioncomponent->getPos().x * currenttilesize[0];
            slidepos.y = (int)positioncomponent->getPos().y * currenttilesize[1];
            srcrect.x = srcrect.y = 0;
            srcrect.w = srcrect.h = 32;
            destrect.w = destrect.h = 32;
        }
        
        void update() override {
            objectivepos.x = (int)positioncomponent->getPos().x * currenttilesize[0];
            objectivepos.y = (int)positioncomponent->getPos().y * currenttilesize[1];
            printf("ObjectivePos: %d, %d\n", objectivepos.x, objectivepos.y);
            if (objectivepos.x != slidepos.x){
                slidepos.x += (objectivepos.x - slidepos.x)/2 + 1;
            if (objectivepos.y != slidepos.y){
                slidepos.y += (objectivepos.y - slidepos.y)/2 + 1;
            }
            if ((objectivepos.x == slidepos.x) && (objectivepos.y == slidepos.y)) {
                positioncomponent->setUpdatable(true);
            }
            destrect.x = slidepos.x;
            destrect.y = slidepos.y;
        }
        
        void draw() override {
            TextureManager::draw(texture, srcrect, destrect);
        }
};

#endif /* SPRITECOMPONENT_HPP */