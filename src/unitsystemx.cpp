
#include "unitsystemx.hpp"

UnitSystemx::UnitSystemx() {

}

UnitSystemx::UnitSystemx(Game * in_game) {
    SDL_Log("Adding Unitsystem");
    game = in_game;
    updateMap();
}

void UnitSystemx::updateMap() {
    mapx = game->getMap();
}

void UnitSystemx::configure(entityx::EventManager & in_events) {
    event_manager = &in_events;
    event_manager->subscribe<unitSelect>(*this);
    event_manager->subscribe<unitDeselect>(*this);
    event_manager->subscribe<unitDanger>(*this);
    event_manager->subscribe<unitMove>(*this);
    event_manager->subscribe<unitHover>(*this);
    event_manager->subscribe<unitDehover>(*this);
    event_manager->subscribe<unitWait>(*this);
    event_manager->subscribe<unitReturn>(*this);
    event_manager->subscribe<unitTalk>(*this);
    event_manager->subscribe<unitRescue>(*this);
    event_manager->subscribe<unitAttack>(*this);
    event_manager->subscribe<unitTrade>(*this);
    event_manager->subscribe<unitEscape>(*this);
    event_manager->subscribe<unitItems>(*this);
    event_manager->subscribe<refreshUnits>(*this);
}

void UnitSystemx::receive(const unitReturn & Return) {
    SDL_Log("Received unitReturn event");
    mapx->hideOverlay();

    entityx::Entity selector = Return.selector;
    entityx::ComponentHandle<Position> cursor_position = selector.component<Position>();

    Point new_position;

    entityx::ComponentHandle<Position> toreturn_position;
    entityx::ComponentHandle<Unit> unit = Return.unit;
    entityx::Entity toreturn = unit.entity();

    if (toreturn.valid()) {
        toreturn_position = toreturn.component<Position>();

        if (toreturn_position) {
            Point pos = toreturn_position->getTilemapPos();
            Point offset = toreturn_position->getOffset();
            new_position.x = pos.x - offset.x;
            new_position.y = pos.y - offset.x;
            SDL_Log("Old position %d, %d \n", old_position.x, old_position.y);
        } else {
            SDL_Log("Could not get returning unit's position component");
        }
    } else {
        SDL_Log("Could not get selected entity");
    }

    mapx->moveUnit(new_position.x, new_position.y, old_position.x, old_position.y);
    toreturn_position->setPos(old_position);
}

void UnitSystemx::receive(const refreshUnits & refresh) {
    SDL_Log("Received refreshUnits events");
    uint8_t army = refresh.army;
    std::vector<entityx::ComponentHandle<Unit>> units = mapx->getUnits(army);

    SDL_Log("units size: %d", units.size());

    for (uint16_t i = 0; i < units.size(); i++) {
        units[i]->refresh();
    }
}

void UnitSystemx::receive(const unitDehover & dehover) {
    SDL_Log("Received unitDehover event");

    entityx::Entity cursor = dehover.cursor;
    entityx::ComponentHandle<Unit> unit = dehover.unit;

    if (isPC(unit->getArmy())) {

    } else {
        event_manager->emit<return2Map>(cursor);
    }
}

void UnitSystemx::receive(const unitHover & hover) {
    SDL_Log("Received unitHover event");
}

void UnitSystemx::receive(const unitWait & wait) {
    SDL_Log("Received unitWait event");
    entityx::ComponentHandle<Unit> unit = wait.unit;
    entityx::Entity ent = unit.entity();
    entityx::ComponentHandle<Sprite> sprite = ent.component<Sprite>();
    unit->wait();
    sprite->darken();
    event_manager->emit<return2Map>(wait.selector);
}

void UnitSystemx::receive(const unitTalk & talk) {
    SDL_Log("unitTalk event received");
    entityx::ComponentHandle<Unit> unit = talk.unit;
    // event_manager->emit<talkMenu>(wait.selector);
    unit->wait();
    event_manager->emit<return2Map>(talk.selector);
}

void UnitSystemx::receive(const unitRescue & rescue) {
    SDL_Log("unitRescue event received");
    entityx::ComponentHandle<Unit> unit = rescue.unit;
    // event_manager->emit<rescueMenu>(wait.selector);
    unit->wait();
    event_manager->emit<return2Map>(rescue.selector);
}

void UnitSystemx::receive(const unitAttack & attack) {
    SDL_Log("unitAttack event received");
    entityx::ComponentHandle<Unit> unit = attack.unit;
    // event_manager->emit<attackMenu>(wait.selector);
    unit->wait();
    event_manager->emit<return2Map>(attack.selector);
}

void UnitSystemx::receive(const unitTrade & trade) {
    SDL_Log("unitTrade event received");
    entityx::ComponentHandle<Unit> unit = trade.unit;
    // event_manager->emit<tradeMenu>(wait.selector);
    unit->wait();
    event_manager->emit<return2Map>(trade.selector);
}

void UnitSystemx::receive(const unitEscape & escape) {
    SDL_Log("unitEscape event received");
    entityx::ComponentHandle<Unit> unit = escape.unit;
    unit->wait();
    event_manager->emit<return2Map>(escape.selector);
}

void UnitSystemx::receive(const unitItems & items) {
    SDL_Log("unitItems event received");
    entityx::ComponentHandle<Unit> unit = items.unit;
    // event_manager->emit<itemMenu>(wait.selector);
    unit->wait();
    event_manager->emit<return2Map>(items.selector);
}

void UnitSystemx::receive(const unitDeselect & deselect) {
    SDL_Log("unitDeselect event received");
    entityx::ComponentHandle<Unit> unit = deselect.unit;
    entityx::Entity selector = deselect.selector;

    if (isPC(unit->getArmy())) {
        event_manager->emit<return2Map>(selector);
    } else {
        switch (game->getState()) {
            case GAME::STATE::UNITMOVE:
                if (unit->showsDanger()) {
                    event_manager->emit<unitDanger>(selector, unit);
                } else {
                    event_manager->emit<return2Map>(selector);
                }

                break;

            default:
                SDL_Log("gamestate is invalid");
        }
    }
}

void UnitSystemx::receive(const unitSelect & select) {
    SDL_Log("unitSelect event received");
    int16_t newstate = -1;
    entityx::ComponentHandle<Unit> unit = select.unit;
    selected = unit.entity();
    entityx::ComponentHandle<Position> position = selected.component<Position>();
    Point pos = position->getTilemapPos();
    Point offset = position->getOffset();
    old_position.x = pos.x - offset.x;
    old_position.y = pos.y - offset.y;
}

void UnitSystemx::receive(const unitDanger & danger) {
    SDL_Log("unitDanger event received");
    std::vector<std::vector<int16_t>> costmapp;
    std::vector<std::vector<int16_t>> movemapp;
    std::vector<std::vector<int16_t>> attackmapp;
    std::vector<std::vector<int16_t>> dangermapp;
    uint32_t unit_move;
    uint32_t start[2];
    int8_t * range;

    entityx::ComponentHandle<Unit> unit = danger.unit;
    entityx::Entity selector = danger.selector;
    entityx::ComponentHandle<Position> cursor_position = selector.component<Position>();

    if (cursor_position) {
        Point cursorpos = cursor_position->getTilemapPos();
        Point offset = cursor_position->getOffset();
        start[0] = cursorpos.x - offset.x;
        start[1] = cursorpos.y - offset.y;
    } else {
        SDL_Log("Could not get selector position component");
    }

    if (unit) {
        unit_move = unit->getStats().move;
        range = unit->getRange();
    } else {
        SDL_Log("Could not get unit component");
    }

    costmapp = mapx->makeMvtCostmap(unit);
    movemapp = movemap(costmapp, start, unit_move, "matrix");
    attackmapp = attackmap(movemapp, start, unit_move, range, "matrix");
    dangermapp = matrix_plus(attackmapp, movemapp);


    if (!unit->showsDanger()) {
        if (!mapx->showsDanger()) {
            mapx->setDanger(dangermapp);
            mapx->showDanger();
        } else {
            mapx->addDanger(dangermapp);
        }

        unit->showDanger();
    } else {
        mapx->subDanger(dangermapp);
        unit->hideDanger();
    }
}

void UnitSystemx::receive(const unitMove & move) {
    // SDL_Log("Received unitMove event");
    std::vector<std::vector<int16_t>> costmapp;
    std::vector<std::vector<int16_t>> movemapp;
    std::vector<std::vector<int16_t>> attackmapp;
    entityx::Entity selector = move.selector;
    entityx::ComponentHandle<Position> cursorpos = selector.component<Position>();
    entityx::ComponentHandle<Unit> unit = move.unit;
    Point start;
    Point offset;
    uint32_t nooffset[2];
    uint32_t unit_move;
    uint32_t current_unit_id;
    int8_t * range;

    if (cursorpos) {
        start = cursorpos->getTilemapPos();
        offset = cursorpos->getOffset();
        nooffset[0] = start.x - offset.x;
        nooffset[1] = start.y - offset.y;
    } else {
        SDL_Log("Could not get selector position component");
    }

    if (unit) {
        unit_move = unit->getStats().move;
        range = unit->getRange();
    } else {
        SDL_Log("Could not get unit component");
    }

    SDL_Log("unitname: %s", unit->getName().c_str());
    SDL_Log("nooffset: %d %d", nooffset[0], nooffset[1]);
    SDL_Log("unitmove: %d", unit_move);
    SDL_Log("range: %d %d", range[0], range[1]);

    costmapp = mapx->makeMvtCostmap(unit);

    movemapp = movemap(costmapp, nooffset, unit_move, "matrix");
    mapx->setOverlay(MAP::OVERLAY::MOVE, movemapp);

    attackmapp = attackmap(movemapp, nooffset, unit_move, range, "matrix");
    mapx->setOverlay(MAP::OVERLAY::ATTACK, attackmapp);

    mapx->showOverlay();
}

void UnitSystemx::update(entityx::EntityManager & es, entityx::EventManager & events, entityx::TimeDelta dt) {

}