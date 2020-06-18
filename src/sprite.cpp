#include "sprite.hpp"

Sprite::Sprite() {

}

Sprite::Sprite(const char * in_asset_name) {
    setTexture(in_asset_name);
}

Sprite::Sprite(const char * in_asset_name, int32_t in_picsize[2]) : Sprite(in_asset_name)  {
    setSrcrect(in_picsize[0], in_picsize[1]);
    setDestrect(in_picsize[0], in_picsize[1]);
}

Sprite::Sprite(const char * in_asset_name, int32_t inFrames, int32_t inSpeed) : Sprite(in_asset_name) {
    animated = true;
    frames = inFrames;
    speed = inSpeed;
}

SDL_Texture * Sprite::getTexture() {
    return (texture);
}

float * Sprite::getSlidefactors() {
    return (slidefactors);
}

void Sprite::hide() {
    visible = false;
}

void Sprite::animate() {
    animated = true;
}

void Sprite::still() {
    animated = false;
}

bool Sprite::isAnimated() {
    return (animated);
}

bool Sprite::isVisible() {
    return (visible);
}

uint8_t Sprite::getSs_looping() {
    return (ss_looping);
}

int16_t Sprite::getFrames() {
    return (frames);
}

int16_t Sprite::getSpeed() {
    return (speed);
}

void Sprite::show() {
    visible = true;
}

int16_t * Sprite::getSlidepos() {
    return (slidepos);
}

int16_t * Sprite::getObjpos() {
    return (objectivepos);
}

void Sprite::setSlidepos(int16_t * in_slidepos) {
    slidepos[0] = in_slidepos[0];
    slidepos[1] = in_slidepos[1];
}

void Sprite::setObjpos(int16_t * in_objectivepos) {
    objectivepos[0] = in_objectivepos[0];
    objectivepos[1] = in_objectivepos[1];
}

uint8_t Sprite::getSlidetype() {
    return (slidetype);
}

void Sprite::setSlidetype(uint8_t in_slidetype) {
    slidetype = in_slidetype;

    if (animated) {
        animatedRects();
    }
}

void Sprite::animatedRects() {
    switch (slidetype) {
        case SLIDETYPE::GEOMETRIC:
            setSrcrect(tilesize[0] * 2, tilesize[1] * 2);
            setDestrect(tilesize[0] * 2, tilesize[1] * 2);
            break;

        default:
            SDL_Log("slidetype is invalid");
    }
}

void Sprite::setTilesize(uint16_t * in_tilesize) {
    tilesize[0] = in_tilesize[0];
    tilesize[1] = in_tilesize[1];

    if (animated) {
        animatedRects();
    }
}

void Sprite::init(int16_t * in_position) {
    slidepos[0] = objectivepos[0] = (int32_t)in_position[0] * tilesize[0] - destrect.w / 4;
    slidepos[1] = objectivepos[1] = (int32_t)in_position[1] * tilesize[1] - destrect.h / 4;
}

void Sprite::init(Point in_position) {
    slidepos[0] = objectivepos[0] = (int32_t)in_position.x * tilesize[0] - destrect.w / 4;
    slidepos[1] = objectivepos[1] = (int32_t)in_position.y * tilesize[1] - destrect.h / 4;
}

void Sprite::setDestrectpos(int16_t * in_position) {
    destrect.x = in_position[0];
    destrect.y = in_position[1];
}

void Sprite::setDestrectpos(Point in_position) {
    destrect.x = in_position.x;
    destrect.y = in_position.y;
}

void Sprite::setTexture(const char * in_asset_name) {
    asset_name = in_asset_name;
    texture = loadTexture(Game::renderer, in_asset_name);
}

void Sprite::setAnimation(int32_t inFrames, int32_t inSpeed) {
    animated = true;
    frames = inFrames;
    speed = inSpeed;
}

void Sprite::darken() {
    SDL_SetTextureColorMod(texture, 128, 128, 128);
}

void Sprite::lighten() {
    SDL_SetTextureColorMod(texture, 255, 255, 255);
}


int16_t Sprite::getSlideint() {
    return (slideint);
}

void Sprite::setSrcrect(SDL_Rect in_rect) {
    srcrect = in_rect;
}

void Sprite::setSrcrect(int32_t width, int32_t height) {
    srcrect.w = width;
    srcrect.h = height;
}

void Sprite::setDestrect(SDL_Rect in_rect) {
    destrect = in_rect;
}

void Sprite::setDestrect(int32_t width, int32_t height) {
    destrect.w = width;
    destrect.h = height;
}

SDL_Rect Sprite::getSrcrect() {
    return (srcrect);
}

SDL_Rect Sprite::getDestrect() {
    return (destrect);
}

void Sprite::draw() {
    SDL_RenderCopy(Game::renderer, texture, &srcrect, &destrect);
}
