
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
}

void UnitSystemx::receive(const unitReturn & Return) {
    SDL_Log("Received unitReturn event");
    mapx->hideOverlay();

    entityx::Entity cursor = Return.cursor;
    entityx::ComponentHandle<Position> cursor_position = cursor.component<Position>();

    short int new_position[2];

    entityx::ComponentHandle<Position> toreturn_position;
    entityx::ComponentHandle<Unit> unit = Return.unit;
    entityx::Entity toreturn = unit.entity();

    if (toreturn.valid()) {
        toreturn_position = toreturn.component<Position>();

        if (toreturn_position) {
            new_position[0] = toreturn_position->getPos()[0] - toreturn_position->getOffset()[0];
            new_position[1] = toreturn_position->getPos()[1] - toreturn_position->getOffset()[1];
            SDL_Log("Old position %d, %d \n", old_position[0], old_position[1]);
        } else {
            SDL_Log("Could not get returning unit's position component");
        }
    } else {
        SDL_Log("Could not get selected entity");
    }

    mapx->moveUnit(new_position[0], new_position[1], old_position[0], old_position[1]);
    toreturn_position->setPos(old_position);
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
    event_manager->emit<return2Map>(wait.cursor);
}

void UnitSystemx::receive(const unitTalk & talk) {
    SDL_Log("unitTalk event received");
    entityx::ComponentHandle<Unit> unit = talk.unit;
    // event_manager->emit<talkMenu>(wait.cursor);
    unit->wait();
    event_manager->emit<return2Map>(talk.cursor);
}

void UnitSystemx::receive(const unitRescue & rescue) {
    SDL_Log("unitRescue event received");
    entityx::ComponentHandle<Unit> unit = rescue.unit;
    // event_manager->emit<rescueMenu>(wait.cursor);
    unit->wait();
    event_manager->emit<return2Map>(rescue.cursor);
}

void UnitSystemx::receive(const unitAttack & attack) {
    SDL_Log("unitAttack event received");
    entityx::ComponentHandle<Unit> unit = attack.unit;
    // event_manager->emit<attackMenu>(wait.cursor);
    unit->wait();
    event_manager->emit<return2Map>(attack.cursor);
}

void UnitSystemx::receive(const unitTrade & trade) {
    SDL_Log("unitTrade event received");
    entityx::ComponentHandle<Unit> unit = trade.unit;
    // event_manager->emit<tradeMenu>(wait.cursor);
    unit->wait();
    event_manager->emit<return2Map>(trade.cursor);
}

void UnitSystemx::receive(const unitEscape & escape) {
    SDL_Log("unitEscape event received");
    entityx::ComponentHandle<Unit> unit = escape.unit;
    unit->wait();
    event_manager->emit<return2Map>(escape.cursor);
}

void UnitSystemx::receive(const unitItems & items) {
    SDL_Log("unitItems event received");
    entityx::ComponentHandle<Unit> unit = items.unit;
    // event_manager->emit<itemMenu>(wait.cursor);
    unit->wait();
    event_manager->emit<return2Map>(items.cursor);
}

void UnitSystemx::receive(const unitDeselect & deselect) {
    SDL_Log("unitDeselect event received");
    entityx::ComponentHandle<Unit> unit = deselect.unit;
    entityx::Entity cursor = deselect.cursor;

    if (isPC(unit->getArmy())) {
        event_manager->emit<return2Map>(cursor);
    } else {
        switch (game->getState()) {
            case GAME::STATE::UNITMOVE:
                if (unit->isDanger()) {
                    event_manager->emit<unitDanger>(cursor, unit);
                } else {
                    event_manager->emit<return2Map>(cursor);
                }

                break;
        }
    }
}

void UnitSystemx::receive(const unitSelect & select) {
    SDL_Log("unitSelect event received");
    short int newstate = -1;
    entityx::ComponentHandle<Unit> unit = select.unit;
    selected = unit.entity();
    entityx::ComponentHandle<Position> position = selected.component<Position>();
    old_position[0] = position->getPos()[0] - position->getOffset()[0];
    old_position[1] = position->getPos()[1] - position->getOffset()[1];
}

void UnitSystemx::receive(const unitDanger & danger) {
    SDL_Log("unitDanger event received");
    std::vector<std::vector<short int>> costmapp;
    std::vector<std::vector<short int>> movemapp;
    std::vector<std::vector<short int>> attackmapp;
    std::vector<std::vector<short int>> dangermapp;
    short unsigned int unit_move;
    short unsigned int start[2];
    unsigned char * range;

    entityx::ComponentHandle<Unit> unit = danger.unit;
    entityx::Entity cursor = danger.cursor;
    entityx::ComponentHandle<Position> cursorpos = cursor.component<Position>();

    if (cursorpos) {
        start[0] = cursorpos->getPos()[0] - cursorpos->getOffset()[0];
        start[1] = cursorpos->getPos()[1] - cursorpos->getOffset()[1];
    } else {
        SDL_Log("Could not get cursor position component");
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


    if (!unit->isDanger()) {
        if (!mapx->isDanger()) {
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
    std::vector<std::vector<short int>> costmapp;
    std::vector<std::vector<short int>> movemapp;
    std::vector<std::vector<short int>> attackmapp;
    entityx::Entity cursor = move.cursor;
    entityx::ComponentHandle<Position> cursorpos = cursor.component<Position>();
    entityx::ComponentHandle<Unit> unit = move.unit;
    short unsigned int * start;
    short unsigned int * offset;
    short unsigned int nooffset[2];
    short unsigned int unit_move;
    short unsigned int current_unit_id;
    unsigned char * range;

    if (cursorpos) {
        start = (short unsigned int *)cursorpos->getPos();
        offset = (short unsigned int *)cursorpos->getOffset();
    } else {
        SDL_Log("Could not get cursor position component");
    }

    if (unit) {
        unit_move = unit->getStats().move;
        range = unit->getRange();
    } else {
        SDL_Log("Could not get unit component");
    }

    nooffset[0] = start[0] - offset[0];
    nooffset[1] = start[1] - offset[1];
    SDL_Log("nooffset: %d %d", nooffset[0], nooffset[1]);
    SDL_Log("unitmove: %d", unit_move);

    costmapp = mapx->makeMvtCostmap(unit);

    movemapp = movemap(costmapp, nooffset, unit_move, "matrix");
    mapx->setOverlay(MAP::OVERLAY::MOVE, movemapp);

    attackmapp = attackmap(movemapp, nooffset, unit_move, range, "matrix");
    mapx->setOverlay(MAP::OVERLAY::ATTACK, attackmapp);

    mapx->showOverlay();
}

void UnitSystemx::update(entityx::EntityManager & es, entityx::EventManager & events, entityx::TimeDelta dt) {

}