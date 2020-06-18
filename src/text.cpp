#include "text.hpp"

Text::Text() {

};

Text::Text(int16_t in_fontsize) {
    setFontsize(in_fontsize);
}

void Text::setFontsize(int16_t in_fontsize) {
    fontsize = in_fontsize;
    linespacing = (int16_t)(fontsize * sizefactor[0] * spacingfactor);
}

void Text::show() {
    visible = true;
}

void Text::hide() {
    visible = false;
}

bool Text::isVisible() {
    return (visible);
}

void Text::setSpacingfactor(float in_factor) {
    spacingfactor = in_factor;
}

void Text::setSizefactor(float in_factor[2]) {
    sizefactor[0] = in_factor[0];
    sizefactor[1] = in_factor[1];
}

void Text::setLinespacing(int16_t in_spacing) {
    linespacing = in_spacing;
}

int16_t Text::getLinespacing() {
    return (linespacing);
}

void Text::setColor(std::vector<SDL_Color> in_text_color) {
    text_color = in_text_color;
}

void Text::setColor(SDL_Color in_text_color) {
    text_color.clear();
    text_color.push_back(in_text_color);
}

void Text::setText(std::vector<std::string> in_text) {
    text_lines = in_text;
}

void Text::setText(const char * in_text) {
    text_lines.clear();
    text_lines.push_back(std::string(in_text));
}

void Text::setText(std::string in_text) {
    text_lines.clear();
    text_lines.push_back(in_text);
}

std::vector<SDL_Texture *> Text::getTextures() {
    return (textures);
}

void Text::makeTextures() {
    removeTextures();

    for (int i = 0; i < text_lines.size(); i++) {
        if (text_color.size() == 1) {
            addTextTexture(text_lines[i], text_color[0]);
        } else {
            addTextTexture(text_lines[i], text_color[i]);
        }
    }
}


void Text::addTextTexture(std::string in_text, SDL_Color in_text_color) {
    textures.push_back(textToTexture(Game::renderer, in_text, in_text_color, Game::font));
    srcrects.push_back(SDL_Rect{});
    destrects.push_back(SDL_Rect{});
}

void Text::removeTexture(int16_t index) {
    if (index == -1) {
        textures.pop_back();
    } else {
        textures.erase(textures.begin() + index);
    }
}

void Text::removeTextures() {
    textures.clear();
}

void Text::wraptext() {
    // Should wrap text inside the background texture and pâdding.
}

void Text::setRects(Point in_position) {
    setRects(in_position.x, in_position.y);
}

void Text::setRects(int16_t * in_position) {
    setRects(in_position[0], in_position[1]);
}

void Text::setRects(int16_t x, int16_t y) {
    srcrects[0].x = srcrects[0].y = 0;
    destrects[0].x = x + padding[3];
    destrects[0].y = y + padding[0];
    int hgt, wdt;

    for (int i = 0; i < textures.size(); i++) {
        SDL_QueryTexture(textures[i], NULL, NULL, &hgt, &wdt);

        srcrects[i].h = fontsize;
        srcrects[i].w = text_lines[i].length() * fontsize;
        // destrects[i].h = hgt;
        destrects[i].h = (int)fontsize * sizefactor[0];
        // destrects[i].w = wdt;
        destrects[i].w = (int)text_lines[i].length() * fontsize * sizefactor[1];
        destrects[i].x = destrects[0].x;
        destrects[i].y = destrects[0].y + linespacing * i;
    }
}

void Text::draw() {
    // Find a way to draw text letter by letter, word by word, etc. for future script.
    for (int i = 0; i < textures.size(); i++) {
        SDL_RenderCopy(Game::renderer, textures[i], &srcrects[i], &destrects[i]);
    }
}
