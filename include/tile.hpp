#ifndef TILE_HPP
#define TILE_HPP
using namespace std;
#include "shared.hpp"
#include "generic.hpp"
/*! \file tile.hpp
* \brief Tile class. All different possible tiles, representing terrain and stuff Graphics linked to tile can be different or even unique. This tile only contains the gameplay information.
*/
/// \class tile
/// \brief tile class. Implemented to be as light as possible.
class weapon: public generic {
    public:
    char def;
    char avoid;
    /// \var char healordmg
    /// \brief if Negative, damages for the value each turn. If positive, heals for the value each turn. 
    // DOUBLE DUTY: I think values above 87 and below -87 should be considered percents. Mapped values: 87 to 0%, 88 to , -88 to 99%... -87 to 100%
    char healordmg;
    /// \var string effect;
    /// \brief other effects applied by the tile, like poison, or teleport, or whatever.
    string effect;
    
    /// \var char frequency
    /// \brief At what frequency does the effect/healing/dmg occurs. NOTE: in reality its kind of the inverse of a frequency. if 2, occurs every 2 turns, if 3 occurs every 3 turns etc.
    char frequency;
    /// \var char start
    /// \brief turn where effect/dmg/heal start to be applied.
    char start;
    /// \var char end
    /// \brief turn where effect/dmg/heal ends.
    char end;
    
}

#endif /* TILE_HPP */