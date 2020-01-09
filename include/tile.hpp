#ifndef TILE_HPP
#define TILE_HPP
#include "shared.hpp"
/*! \file tile.hpp
* \brief Tile class. All different possible tiles, representing terrain and stuff Graphics linked to tile can be different or even unique. This tile only contains the gameplay information.
*/
/// \class tile
/// \brief tile class. Implemented to be as light as possible.
class tile {
    public:

        tile(std::string, std::vector<int>);
        tile();
        ~tile();

};

#endif /* TILE_HPP */