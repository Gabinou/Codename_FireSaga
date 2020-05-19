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

    optimal_librarians = std::min(max_librarians, floor(frac_librarians * party_size));
    optimal_cooks = std::min(max_cooks, std::max(floor(frac_cooks * party_size), 1));
    optimal_guards = std::min(max_guards, std::max(floor(frac_guards * party_size), 1));
    optimal_stablehands = std::min(max_stablehands, std::max(floor(frac_stablehands * party_size), 1));
    optimal_storagemaster = std::min(max_storagemaster, std::max(floor(frac_storagemaster * party_size), 1));
    optimal_clergymen = std::min(max_clergymen, floor(frac_clergymen * party_size));
    
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
