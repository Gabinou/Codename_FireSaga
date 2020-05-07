
#include "menusystemx.hpp"

MenuSystemx::MenuSystemx() {

}

MenuSystemx::MenuSystemx(Game * in_game) {
    SDL_Log("Adding Menusystem");
    game = in_game;
    settings = game->getSettings();
    updateMap();
}

void MenuSystemx::configure(entityx::EventManager & in_events) {
    event_manager = &in_events;
    event_manager->subscribe<unitSelect>(*this);
    event_manager->subscribe<unitMenu>(*this);
    event_manager->subscribe<unitmenuSelect>(*this);
    event_manager->subscribe<mapMenu>(*this);
}

void MenuSystemx::updateMap() {
    mapx = game->getMap();
}

void MenuSystemx::setMap(entityx::ComponentHandle<Map> in_map) {
    if (in_map) {
        mapx = in_map;
    } else {
        SDL_Log("RenderSystemx: Map Handle is invalid");
    }
}

void MenuSystemx::receive(const unitSelect & select) {
    SDL_Log("unitSelect event received");
    short int newstate = -1;
    entityx::ComponentHandle<Unit> unit = select.unit;
    selected = unit.entity();

    if (isPC(unit->getArmy())) {
        event_manager->emit<unitMove>(select.cursor, unit);
        newstate = GAME::STATE::UNITMOVE;
    } else {
        if (game->getState() == GAME::STATE::UNITMOVE) {
            event_manager->emit<unitDanger>(select.cursor, unit);
        } else {
            event_manager->emit<unitMove>(select.cursor, unit);
            newstate = GAME::STATE::UNITMOVE;
        }
    }

    if (newstate > 0) {
        game->setState(newstate);
    }
}

void MenuSystemx::receive(const mapMenu & menu) {
    SDL_Log("Received mapMenu event ");
    entityx::Entity * mapmenu = game->getMenu(MENU::MAPMENU);
    entityx::Entity cursor = menu.cursor;
    entityx::ComponentHandle<Position> cursorpos = cursor.component<Position>();
    short int new_position[2];

    game->makeMenuoptions(MENU::MAPMENU);

    if (!mapmenu->valid()) {
        game->makeMenu(MENU::MAPMENU);
    } else {
        game->makeMenutext(MENU::MAPMENU);
    }

    if (cursorpos) {
        new_position[0] = cursorpos->getPos()[0] - cursorpos->getOffset()[0];
        new_position[1] = cursorpos->getPos()[1] - cursorpos->getOffset()[1];
        SDL_Log("New position %d, %d \n", new_position[0], new_position[1]);
    } else {
        SDL_Log("Could not get setter(unit) position component");
    }

    mapmenu->component<Position>()->setPos((new_position[0] + 1) * settings->tilesize[0], new_position[1] * settings->tilesize[1]);
    game->showMenu(MENU::MAPMENU);
    game->setCursorstate(MENU::MAPMENU);
}


void MenuSystemx::receive(const unitMenu & menu) {
    SDL_Log("Received unitMenu event ");
    mapx->hideOverlay();

    entityx::Entity cursor = menu.cursor;
    entityx::ComponentHandle<Position> selectedpos;
    entityx::ComponentHandle<Position> cursorpos = cursor.component<Position>();
    entityx::Entity * unitmenu = game->getMenu(MENU::UNIT);
    short int new_position[2];

    if (selected.valid()) {
        selectedpos = selected.component<Position>();

        if (selectedpos) {
            old_position[0] = selectedpos->getPos()[0] - selectedpos->getOffset()[0];
            old_position[1] = selectedpos->getPos()[1] - selectedpos->getOffset()[1];
            SDL_Log("Old position %d, %d \n", old_position[0], old_position[1]);
        } else {
            SDL_Log("Could not get selectedx unit component");
        }
    } else {
        SDL_Log("Selected entity is invalid.");
    }

    if (cursorpos) {
        new_position[0] = cursorpos->getPos()[0] - selectedpos->getOffset()[0];
        new_position[1] = cursorpos->getPos()[1] - selectedpos->getOffset()[1];
        SDL_Log("New position %d, %d \n", new_position[0], new_position[1]);
    } else {
        SDL_Log("Could not get setter(unit) position component");
    }

    mapx->moveUnit(old_position[0], old_position[1], new_position[0], new_position[1]);
    selectedpos->setPos(new_position);
    game->setCursorlastpos(new_position[0], new_position[1]);

    game->makeMenuoptions(MENU::UNIT);

    if (!unitmenu->valid()) {
        game->makeMenu(MENU::UNIT);
    } else {
        game->makeMenutext(MENU::UNIT);
    }

    unitmenu->component<Position>()->setPos((new_position[0] + 1) * settings->tilesize[0], new_position[1] * settings->tilesize[1]);
    game->showMenu(MENU::UNIT);
    game->setCursorstate(MENU::UNIT);
}

void MenuSystemx::receive(const unitmenuSelect & select) {
    SDL_Log("unitmenuSelect event received");
    entityx::Entity cursor = select.cursor;
    entityx::ComponentHandle<Unit> unit = select.unit;
    entityx::ComponentHandle<Position> position = cursor.component<Position>();

    short int * cursorpos = position->getPos();
    short int * cursorbounds = position->getBounds();
    unsigned char menuind = cursorpos[1] - cursorbounds[2];

    unitmenuoptions = game->getMenuoptions(MENU::UNIT);
    SDL_Log("menuind: %d ", menuind);
    SDL_Log("unitmenuoptions[menuind]: %d ", unitmenuoptions[menuind]);

    if (unit) {
        switch (unitmenuoptions[menuind]) {
            case MENU::OPTION::ITEMS:
                event_manager->emit<unitItems>(cursor, unit);
                break;

            case MENU::OPTION::TALK:
                event_manager->emit<unitTalk>(cursor, unit);
                break;

            case MENU::OPTION::RESCUE:
                event_manager->emit<unitRescue>(cursor, unit);
                break;

            case MENU::OPTION::SEIZE:
                event_manager->emit<unitSeize>(unit);
                break;

            case MENU::OPTION::ESCAPE:
                event_manager->emit<unitEscape>(cursor, unit);
                break;

            case MENU::OPTION::ATTACK:
                event_manager->emit<unitAttack>(cursor, unit);
                break;

            case MENU::OPTION::TRADE:
                event_manager->emit<unitTrade>(cursor, unit);
                break;

            case MENU::OPTION::STAFF:
                event_manager->emit<unitStaff>(cursor, unit);
                break;

            case MENU::OPTION::WAIT:
                event_manager->emit<unitWait>(cursor, unit);
                break;
        }
    } else {
        SDL_Log("unitmenuSelect: could not get unit");
    }
}

void MenuSystemx::update(entityx::EntityManager & es, entityx::EventManager & events, entityx::TimeDelta dt) {

}
