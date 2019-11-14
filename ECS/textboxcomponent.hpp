#ifndef TEXTBOXCOMPONENT_HPP
#define TEXTBOXCOMPONENT_HPP

#include "ECS.hpp"
#include "components.hpp"
#include <vector>


// NOTE: Maybe this component should focus only on printing lines of text. Then, the background can be added manually using another component, instead of including it here.
// So maybe this should become a text LINES components or something.

class TextBoxComponent : public Component {

    private:
        std::vector<SDL_Texture *> textures;
        std::vector<TextComponent *> textcomponents;
        TTF_Font * font;
        SpriteComponent backgroundsprite;
        std::vector<std::string> textlines;
        PositionComponent * positioncomponent;

    protected:

    public:
        TextBoxComponent() = default;

        TextBoxComponent(Map * inmap, const char * path) {
            // backgroundsprite = SpriteComponent(inmap, path);
        }

        void setFont(TTF_Font * in_font) {
            font = in_font;
        }

        TTF_Font * getFont() {
            return (font);
        }

        void setTextlines(std::vector<std::string> in_lines) {
            textlines = in_lines;
        }

        std::vector<std::string> getTextlines() {
            return (textlines);
        }

        virtual void init() override {
            positioncomponent = &entity->getComponent<PositionComponent>();
        }

        virtual void update() override {
        }

        void draw() override {
            // TextureManager::draw(texture, srcrect, destrect);
        }


};

#endif /* TEXTBOXCOMPONENT_HPP */