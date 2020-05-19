#include "camp.hpp"
#include "probability.hpp"
#include "stb_sprintf.h"

Camp::Camp() {

}

Camp::~Camp() {

}

void Camp::setGame(Game * in_game) {
    game = in_game;
};

void Camp::makePartyStack() {
    party_stack.clear();

    for (auto member : party) {
        if (member.first == member.second.getid()) {
            if (getURN() > 49) {
                party_stack.push_back(member.first);
            } else {
                party_stack.insert(party_stack.begin(), member.second.getid());
            }
        } else {
            SDL_Log("Party member's id is not the same as unordered map index");
        }
    }
}


void Camp::updateParty() {
    party = game->getParty();
}

void Camp::addLibrarian(unsigned short int in_unit) {
    switch (in_unit) {
        case UNIT::NAME::ERWIN:
        case UNIT::NAME::KIARA:
        case UNIT::NAME::SILOU:
            break;

        case UNIT::NAME::SERVIL:
        case UNIT::NAME::PERIGNON:
        case UNIT::NAME::POET:
        case UNIT::NAME::RELIABLE:
        case UNIT::NAME::COWARD:
        case UNIT::NAME::JAIGEN1H:
        case UNIT::NAME::HOTTIE:
            break;
    }
}

// void Camp::addCook(unsigned short int in_unit) {
// }
// void Camp::addGuard(unsigned short int in_unit) {
// }
// void Camp::addScribe(unsigned short int in_unit) {
// }
// void Camp::addStablehand(unsigned short int in_unit) {
// }
// void Camp::addStoragemaster(unsigned short int in_unit) {
// }
// void Camp::addAssistant(unsigned short int in_unit) {
// }
// void Camp::addClergymen(unsigned short int in_unit) {
// }
// void Camp::fillJobs() {


//     // librarians.reserve();
//     // cooks.reserve();
//     // guards.reserve();
//     // stablehands.reserve();
//     // storagemaster.reserve();
//     // clergymen.reserve();
// }

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
