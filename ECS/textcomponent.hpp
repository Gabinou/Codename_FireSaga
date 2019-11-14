#ifndef TEXTCOMPONENT_HPP
#define TEXTCOMPONENT_HPP

#include "ECS.hpp"
#include "components.hpp"
#include "map.hpp"
#include "vector2D.hpp"
#include "shared.hpp"
#include "positioncomponent.hpp"
#include "texturemanager.hpp"
#include "SDL2/SDL.h"

class TextComponent : public Component {

    private:
        SDL_Texture * texture;

    protected:
        PositionComponent * positioncomponent;
        SDL_Rect srcrect, destrect;

    public:
        TextComponent() {
        };

        TextComponent(std::string text, SDL_Color textColor) {
            setTexture(text, textColor);
        }

        SDL_Texture * getTexture() {
            return (texture);
        }

        void setTexture(std::string text, SDL_Color textColor) {
            texture = TextureManager::loadFromRenderedText(text, textColor);
        }

        virtual void init() override {
            positioncomponent = &entity->getComponent<PositionComponent>();
            srcrect.x = srcrect.y = 0;
            destrect.x = destrect.y = 60;
            srcrect.w = 1000;
            srcrect.h = 28;
            destrect.w = 200;
            destrect.h = 28;
        }

        virtual void update() override {
        }

        void draw() override {
            TextureManager::draw(texture, srcrect, destrect);
        }
};

#endif /* TEXTCOMPONENT_HPP */