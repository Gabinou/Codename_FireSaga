
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
    event_manager->subscribe<menuSelect>(*this);
    event_manager->subscribe<mapMenu>(*this);
    event_manager->subscribe<disableMenu>(*this);
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
        event_manager->emit<unitMove>(select.selector, unit);
        newstate = GAME::STATE::UNITMOVE;
    } else {
        if (game->getState() == GAME::STATE::UNITMOVE) {
            event_manager->emit<unitDanger>(select.selector, unit);
        } else {
            event_manager->emit<unitMove>(select.selector, unit);
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
    entityx::Entity selector = menu.selector;
    entityx::ComponentHandle<Position> position = selector.component<Position>();
    Point new_position;

    game->makeMenuoptions(MENU::MAPMENU);

    if (!mapmenu->valid()) {
        game->makeMenu(MENU::MAPMENU);
    } else {
        game->makeMenutext(MENU::MAPMENU);
    }

    if (position) {
        Point cursorpos = position->getTilemapPos();
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
    game->setMousestate(MENU::MAPMENU);
    game->setState(GAME::STATE::MAPMENU);
}


void MenuSystemx::receive(const unitMenu & menu) {
    SDL_Log("Received unitMenu event ");
    mapx->hideOverlay();

    entityx::Entity selector = menu.selector;
    entityx::ComponentHandle<Position> selected_position;
    entityx::ComponentHandle<Position> cursor_position = selector.component<Position>();
    entityx::Entity * unitmenu = game->getMenu(MENU::UNIT);
    Point new_position;

    if (selected.valid()) {
        selected_position = selected.component<Position>();

        if (selected_position) {
            Point selectedpos = selected_position->getTilemapPos();
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
        Point cursorpos = cursor_position->getTilemapPos();
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
    game->setMousestate(MENU::UNIT);
}

void MenuSystemx::receive(const menuSelect & select) {
    SDL_Log("Received menuSelect event ");
    entityx::Entity selector = select.selector;
    char menu = select.menu;
    unsigned char menuind;
    entityx::ComponentHandle<Unit> unit = select.unit;
    entityx::ComponentHandle<Position> position = selector.component<Position>();

    Point selected;
    Point offset;
    short int * bounds;
    std::vector<unsigned char> menuoptions = game->getMenuoptions(menu);

    selected = position->getTilemapPos();
    offset = position->getOffset();
    bounds = position->getTilemapBounds();
    // SDL_Log("Boundsmin: %d %d", boundsmin.x, boundsmin.y);
    // SDL_Log("Boundsmax: %d %d", boundsmax.x, boundsmax.y);
    // SDL_Log("Selected: %d %d", selected.x, selected.y);
    // SDL_Log("offset: %d %d", offset.x, offset.y);
    // SDL_Log("onTilemap: %d", position->isonTilemap());

    // if (!position->isonTilemap()) {
    //     selected = position->pixel2tilemap(selected);
    //     offset = position->pixel2tilemap(offset);
    //     boundsmin = mapx->pixel2tilemap(boundsmin);
    //     // boundsmax = mapx->pixel2tilemap(boundsmax);
    // }

    // SDL_Log("Boundsmin: %d %d", boundsmin.x, boundsmin.y);
    // SDL_Log("Boundsmax: %d %d", boundsmax.x, boundsmax.y);
    // SDL_Log("Selected: %d %d", selected.x, selected.y);
    // SDL_Log("offset: %d %d", offset.x, offset.y);

    menuind = selected.y - bounds[2];
    unsigned char menuoption = menuoptions[menuind];

    SDL_Log("menuind: %d", menuind);
    SDL_Log("Menuoption: %d", menuoption);

    switch (menu) {
        case MENU::UNIT:
            if (unit) {
                switch (menuoption) {
                    case MENU::OPTION::ITEMS:
                        event_manager->emit<unitItems>(selector, unit);
                        break;

                    case MENU::OPTION::TALK:
                        event_manager->emit<unitTalk>(selector, unit);
                        break;

                    case MENU::OPTION::RESCUE:
                        event_manager->emit<unitRescue>(selector, unit);
                        break;

                    case MENU::OPTION::SEIZE:
                        event_manager->emit<unitSeize>(unit);
                        break;

                    case MENU::OPTION::ESCAPE:
                        event_manager->emit<unitEscape>(selector, unit);
                        break;

                    case MENU::OPTION::ATTACK:
                        event_manager->emit<unitAttack>(selector, unit);
                        break;

                    case MENU::OPTION::TRADE:
                        event_manager->emit<unitTrade>(selector, unit);
                        break;

                    case MENU::OPTION::STAFF:
                        event_manager->emit<unitStaff>(selector, unit);
                        break;

                    case MENU::OPTION::WAIT:
                        event_manager->emit<unitWait>(selector, unit);
                        break;
                }
            } else {
                SDL_Log("menuSelect: could not get unit");
            }

            break;

            switch (menuoption) {
                case MENU::MAPMENU:
                    switch (menuoption) {
                        case MENU::OPTION::OBJECTIVES:
                            event_manager->emit<objectivesMenu>(selector);
                            break;

                        case MENU::OPTION::UNITS:
                            event_manager->emit<unitsMenu>(selector);
                            break;

                        case MENU::OPTION::ENEMYUNITS:
                            event_manager->emit<enemyunitsMenu>(selector);
                            break;

                        case MENU::OPTION::OPTIONS:
                            event_manager->emit<optionsMenu>(selector);
                            break;

                        case MENU::OPTION::ITEMS:
                            event_manager->emit<itemsMenu>(selector);
                            break;

                        case MENU::OPTION::ENDTURN:
                            event_manager->emit<disableMenu>(selector, MENU::MAPMENU);
                            event_manager->emit<turnEnd>();
                            break;
                    }

                    break;

                default:
                    SDL_Log("menu not found.");
            }
    }
}

void MenuSystemx::receive(const return2Map & map) {
    SDL_Log("Received return2Map event");

    entityx::Entity selector = map.selector;

    if ((game->getState() == GAME::STATE::UNITMOVE)) {
        mapx->hideOverlay();
    }

    if ((game->getState() == GAME::STATE::MAPMENU)) {
        event_manager->emit<disableMenu>(selector, MENU::MAPMENU);
        game->setCursorstate(MENU::MAP);
        game->setMousestate(MENU::MAP);
    }

    if ((game->getState() == GAME::STATE::UNITMENU) ||
            (game->getState() == GAME::STATE::OPTIONS)) {
        event_manager->emit<disableMenu>(selector, MENU::UNIT);
        game->setCursorstate(MENU::MAP);
        game->setMousestate(MENU::MAP);
    }

    game->setState(GAME::STATE::MAP);
}

void MenuSystemx::update(entityx::EntityManager & es, entityx::EventManager & events, entityx::TimeDelta dt) {

}
