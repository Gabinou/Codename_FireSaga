#ifndef TEXTBOXCOMPONENT_HPP
#define TEXTBOXCOMPONENT_HPP

#include "ECS.hpp"
#include "components.hpp"
#include <vector>


class TextBoxComponent : public Component {

    private:
        std::vector<TextComponent> textcomponents;
        TTF_Font * font;
        SpriteComponent backgroundsprite;
        std::vector<std::string> textlines;

    protected:

    public:
        TextBoxComponent() = default;

        TextBoxComponent(Map * inmap, const char * path) {
            backgroundsprite = SpriteComponent(inmap, path);
        }

        setFont(TTF_Font * in_font) {
            font = in_font;
        }

        TTF_Font getFont() {
            return (font);
        }

        setTextlines(std::vector<std::string> in_lines) {
            textlines = in_lines;
        }

        std::vector<std::string> getTextlines() {
            return (textlines);
        }

        virtual void init() override {
            positioncomponent = &entity->getComponent<PositionComponent>();
            srcrect.x = srcrect.y = 0;
            destrect.x = destrect.y = 60;
            srcrect.w = 1000;
            srcrect.h = 56;
            destrect.w = 200;
            destrect.h = 56;
        }

        virtual void update() override {
        }

        void draw() override {
            TextureManager::draw(texture, srcrect, destrect);
        }


};

#endif /* TEXTBOXCOMPONENT_HPP */