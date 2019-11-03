#include "tilesize.hpp"

Tilesize::Tilesize() {
    tilesize[0] = 32;
    tilesize[1] = 32;
}

Tilesize::~Tilesize() {

}

Tilesize::Tilesize(const short int unsigned width, const short int unsigned height) {
    tilesize[0] = width;
    tilesize[1] = height;
}
    
void Tilesize::setTilesize(const short int unsigned width, const short int unsigned height) {
    tilesize[0] = width;
    tilesize[1] = height;
}

int * Tilesize::getTilesize() {
    return(tilesize);
}