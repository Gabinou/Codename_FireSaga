#include "sprite.hpp"

Sprite::Sprite(const char * in_asset_name) {
    visible = true;
    asset_name = in_asset_name;
    setTexture(in_asset_name);
}

Sprite::Sprite(const char * in_asset_name, int in_picsize[2]) : Sprite(in_asset_name)  {
    srcrect.w = in_picsize[0];
    srcrect.h = in_picsize[1];
    destrect.w = in_picsize[0];
    destrect.h = in_picsize[1];
}

Sprite::Sprite(const char * in_asset_name, int inFrames, int inSpeed) : Sprite(in_asset_name) {
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

bool Sprite::isAnimated() {
    return (animated);
}

bool Sprite::isVisible() {
    return (visible);
}

unsigned char Sprite::getSs_looping() {
    return (ss_looping);
}

short int Sprite::getFrames() {
    return (frames);
}

short int Sprite::getSpeed() {
    return (speed);
}

void Sprite::show() {
    visible = true;
}

short int * Sprite::getSlidepos() {
    return (slidepos);
}

short int * Sprite::getObjpos() {
    return (objectivepos);
}

void Sprite::setSlidepos(short int * in_slidepos) {
    slidepos[0] = in_slidepos[0];
    slidepos[1] = in_slidepos[1];
}

void Sprite::setObjpos(short int * in_objectivepos) {
    objectivepos[0] = in_objectivepos[0];
    objectivepos[1] = in_objectivepos[1];
}

unsigned char Sprite::getSlidetype() {
    return (slidetype);
}

void Sprite::setSlidetype(unsigned char in_slidetype, short unsigned int * in_tilesize) {
    slidetype = in_slidetype;
    tilesize[0] = in_tilesize[0];
    tilesize[1] = in_tilesize[1];

    switch (slidetype) {
    case SLIDETYPE::GEOMETRIC:
        setSrcrect(tilesize[0] * 2, tilesize[1] * 2); // Manually entered from cursor png size.
        setDestrect(tilesize[0] * 2, tilesize[1] * 2);
        break;
    }
}

void Sprite::init(short int * in_position) {
    slidepos[0] = objectivepos[0] = (int)in_position[0] * tilesize[0] - destrect.w / 4;
    slidepos[1] = objectivepos[1] = (int)in_position[1] * tilesize[1] - destrect.h / 4;
}

void Sprite::setDestrectpos(short int * in_position) {
    destrect.x = in_position[0];
    destrect.y = in_position[1];
}

void Sprite::setTexture(const char * in_asset_name) {
    texture = loadTexture(Game::renderer, in_asset_name);
}

void Sprite::setAnimation(int inFrames, int inSpeed) {
    frames = inFrames;
    speed = inSpeed;
}

short int Sprite::getSlideint() {
    return (slideint);
}

void Sprite::setSrcrect(SDL_Rect in_rect) {
    srcrect = in_rect;
}

void Sprite::setSrcrect(int width, int height) {
    srcrect.w = width;
    srcrect.h = height;
}

void Sprite::setDestrect(SDL_Rect in_rect) {
    destrect = in_rect;
}

void Sprite::setDestrect(int width, int height) {
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
    if (visible) {
        SDL_RenderCopy(Game::renderer, texture, &srcrect, &destrect);
    }
}
