#include "camp.hpp"
#include "stb_sprintf.h"

Camp::Camp() {

}

Camp::~Camp() {

}

void Camp::setGame(Game * in_game) {
    game = in_game;
};

void Camp::updateParty() {
    party = game->getParty();
}

void Camp::makeJobNumbers() {

}
