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

void Camp::addLibrarian(unsigned short int in_unit) {

}

void Camp::fillJobs() {
    // librarians.reserve();
    // cooks.reserve();
    // guards.reserve();
    // stablehands.reserve();
    // storagemaster.reserve();
    // clergymen.reserve();
}

void Camp::makeJobNumbers() {
    party_size = party.size();

    optimal_librarians = std::min((unsigned char)max_librarians, (unsigned char)floor(frac_librarians * party_size));
    optimal_cooks = std::min((unsigned char)max_cooks, (unsigned char)std::max((unsigned char)floor(frac_cooks * party_size), (unsigned char)1));
    optimal_guards = std::min((unsigned char)max_guards, (unsigned char)std::max((unsigned char)floor(frac_guards * party_size), (unsigned char)1));
    optimal_stablehands = std::min((unsigned char)max_stablehands, (unsigned char)std::max((unsigned char)floor(frac_stablehands * party_size), (unsigned char)1));
    optimal_storagemaster = std::min((unsigned char)max_storagemaster, (unsigned char)std::max((unsigned char)floor(frac_storagemaster * party_size), (unsigned char)1));
    optimal_clergymen = std::min((unsigned char)max_clergymen, (unsigned char)floor(frac_clergymen * party_size));

    if (game->getChapter() < 10) {
        optimal_assistant = 0;
    } else {
        optimal_assistant = 1;
    }

    if (game->getChapter() < 10) {
        optimal_assistant = 0;
    } else {
        optimal_assistant = 1;
    }


}
