
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
    event_manager->subscribe<disableMenu>(*this);
    event_manager->subscribe<mapmenuSelect>(*this);
    event_manager->subscribe<return2Map>(*this);
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

void MenuSystemx::receive(const disableMenu & disable) {
    SDL_Log("Received disableMenu event");
    game->hideMenu(disable.menuind);
}


void MenuSystemx::receive(const unitSelect & select) {
    SDL_Log("Received unitSelect event ");
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

void MenuSystemx::receive(const mapmenuSelect & select) {
    SDL_Log("Received mapmenuSelect event ");
    entityx::Entity cursor = select.cursor;
    entityx::ComponentHandle<Position> position = cursor.component<Position>();

    Point cursorpos = position->getPos();
    short int * cursorbounds = position->getBounds();
    unsigned char menuind = cursorpos.y - cursorbounds[2];
    std::vector<unsigned char> mapmenuoptions = game->getMenuoptions(MENU::MAPMENU);

    SDL_Log("menuind: %d ", menuind);
    SDL_Log("unitmenuoptions[menuind]: %d ", mapmenuoptions[menuind]);

    switch (mapmenuoptions[menuind]) {
        case MENU::OPTION::OBJECTIVES:
            event_manager->emit<objectivesMenu>(cursor);
            break;

        case MENU::OPTION::UNITS:
            event_manager->emit<unitsMenu>(cursor);
            break;

        case MENU::OPTION::ENEMYUNITS:
            event_manager->emit<enemyunitsMenu>(cursor);
            break;

        case MENU::OPTION::OPTIONS:
            event_manager->emit<optionsMenu>(cursor);
            break;

        case MENU::OPTION::ITEMS:
            event_manager->emit<itemsMenu>(cursor);
            break;

        case MENU::OPTION::ENDTURN:
            event_manager->emit<disableMenu>(cursor, MENU::MAPMENU);
            event_manager->emit<turnEnd>();
            break;

    }
}

void MenuSystemx::receive(const mapMenu & menu) {
    SDL_Log("Received mapMenu event ");
    entityx::Entity * mapmenu = game->getMenu(MENU::MAPMENU);
    entityx::Entity cursor = menu.cursor;
    entityx::ComponentHandle<Position> position = cursor.component<Position>();
    Point new_position;

    game->makeMenuoptions(MENU::MAPMENU);

    if (!mapmenu->valid()) {
        game->makeMenu(MENU::MAPMENU);
    } else {
        game->makeMenutext(MENU::MAPMENU);
    }

    if (position) {
        Point cursorpos = position->getPos();
        Point offset = position->getOffset();
        new_position.x = cursorpos.x - offset.x;
        new_position.y = cursorpos.y - offset.y;
        SDL_Log("New position %d, %d \n", new_position.x, new_position.y);
    } else {
        SDL_Log("Could not get setter(unit) position component");
    }

    mapmenu->component<Position>()->setPos((new_position.x + 1) * settings->tilesize[0], new_position.y * settings->tilesize[1]);
    game->showMenu(MENU::MAPMENU);
    game->setCursorstate(MENU::MAPMENU);
    game->setState(GAME::STATE::MAPMENU);
}


void MenuSystemx::receive(const unitMenu & menu) {
    SDL_Log("Received unitMenu event ");
    mapx->hideOverlay();

    entityx::Entity cursor = menu.cursor;
    entityx::ComponentHandle<Position> selected_position;
    entityx::ComponentHandle<Position> cursor_position = cursor.component<Position>();
    entityx::Entity * unitmenu = game->getMenu(MENU::UNIT);
    Point new_position;

    if (selected.valid()) {
        selected_position = selected.component<Position>();

        if (selected_position) {
            Point selectedpos = selected_position->getPos();
            Point offset = selected_position->getOffset();
            old_position.x = selectedpos.x - offset.x;
            old_position.y = selectedpos.y - offset.y;
            SDL_Log("Old position %d, %d \n", old_position.x, old_position.y);
        } else {
            SDL_Log("Could not get selectedx unit component");
        }
    } else {
        SDL_Log("Selected entity is invalid.");
    }

    if (cursor_position) {
        Point cursorpos = cursor_position->getPos();
        Point offset = cursor_position->getOffset();
        new_position.x = cursorpos.x - offset.x;
        new_position.y = cursorpos.y - offset.y;
        SDL_Log("New position %d, %d \n", new_position.x, new_position.y);
    } else {
        SDL_Log("Could not get setter(unit) position component");
    }

    mapx->moveUnit(old_position.x, old_position.y, new_position.x, new_position.y);
    selected_position->setPos(new_position);
    game->setCursorlastpos(new_position.x, new_position.y);

    game->makeMenuoptions(MENU::UNIT);

    if (!unitmenu->valid()) {
        game->makeMenu(MENU::UNIT);
    } else {
        game->makeMenutext(MENU::UNIT);
    }

    unitmenu->component<Position>()->setPos((new_position.x + 1) * settings->tilesize[0], new_position.y * settings->tilesize[1]);
    game->showMenu(MENU::UNIT);
    game->setCursorstate(MENU::UNIT);
}

void MenuSystemx::receive(const unitmenuSelect & select) {
    SDL_Log("unitmenuSelect event received");
    entityx::Entity cursor = select.cursor;
    entityx::ComponentHandle<Unit> unit = select.unit;
    entityx::ComponentHandle<Position> position = cursor.component<Position>();

    Point cursorpos = position->getPos();
    short int * cursorbounds = position->getBounds();
    unsigned char menuind = cursorpos.y - cursorbounds[2];

    std::vector<unsigned char> unitmenuoptions = game->getMenuoptions(MENU::UNIT);
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

void MenuSystemx::receive(const return2Map & map) {
    SDL_Log("Received return2Map event");

    entityx::Entity cursor = map.cursor;

    if ((game->getState() == GAME::STATE::UNITMOVE)) {
        mapx->hideOverlay();
    }

    if ((game->getState() == GAME::STATE::MAPMENU)) {
        event_manager->emit<disableMenu>(cursor, MENU::MAPMENU);
        game->setCursorstate(MENU::MAP);
    }

    if ((game->getState() == GAME::STATE::UNITMENU) ||
            (game->getState() == GAME::STATE::OPTIONS)) {
        event_manager->emit<disableMenu>(cursor, MENU::UNIT);
        game->setCursorstate(MENU::MAP);
    }

    game->setState(GAME::STATE::MAP);
}

void MenuSystemx::update(entityx::EntityManager & es, entityx::EventManager & events, entityx::TimeDelta dt) {

}
