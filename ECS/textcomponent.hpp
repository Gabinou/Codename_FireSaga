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

class TextComponent : public Component{
    
    private:    
        SDL_Texture *texture;

    protected:
        PositionComponent *positioncomponent;
        SDL_Rect srcrect, destrect;
        Map * currentmap;
        int * currenttilesize;

    public:
        SpriteComponent() = default;
        
        TextComponent(Map * inmap, std::string text, SDL_Color textColor){
            setTexture(text);
            setMap(inmap);
        }
        
        SDL_Texture * getTexture() { 
            return(texture);
        }
        
        void setTexture(std::string text, SDL_Color textColor) {
            texture = TextureManager::loadFromRenderedText(text, textColor);
        }
        
        void setMap(Map * inmap) {
            currentmap = inmap;
            currenttilesize = currentmap->getTilesize();
            srcrect.w = destrect.w = currenttilesize[0];
            srcrect.h = destrect.h = currenttilesize[1];
        }

        virtual void init() override {
            positioncomponent = &entity->getComponent<PositionComponent>();
            srcrect.x = srcrect.y = 0;
            srcrect.w = srcrect.h = 32;
            destrect.w = (int)positioncomponent->getPos().x * destrect.w;
            destrect.h = (int)positioncomponent->getPos().y * destrect.h;
        }
        
        virtual void update() override {
        }
        
        void draw() override {
            TextureManager::draw(texture, srcrect, destrect);
        }
};

#endif /* SPRITECOMPONENT_HPP */