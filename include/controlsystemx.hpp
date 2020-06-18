#ifndef CONTROLSYSTEMX_HPP
#define CONTROLSYSTEMX_HPP

#include <SDL2/SDL.h>
#include <entityx/entityx.h>
#include "events.hpp"
#include "position.hpp"
#include "keyboardcontroller.hpp"
#include "gamepadcontroller.hpp"

struct Controllers {
    entityx::ComponentHandle<KeyboardController> keyboard;
    entityx::ComponentHandle<GamepadController> gamepad;
    entityx::ComponentHandle<MouseController> mouse;
};

class ControlSystemx: public entityx::System<ControlSystemx>, public entityx::Receiver<ControlSystemx> {
private:
    Game * game = nullptr;
    entityx::Entity * cursorx;
    entityx::Entity * mousex;
    SDL_Window * window;

    Settings * settings;
    entityx::EventManager * event_manager;
    entityx::ComponentHandle<Map> mapx;
    entityx::ComponentHandle<Unit> selected;

    KeyboardInputMap keyboardInputMap;
    GamepadInputMap gamepadInputMap;
    MouseInputMap mouseInputMap;

    std::vector<std::vector<entityx::ComponentHandle<Unit>>> unitmap;

    float min_held = 0.0f;
    float mouse_idletime = 0.0f;
    float mouse_disabletime = 0.5f;
    bool blockInput = false;
    int64_t kb_held = 0.0f;
    int64_t mouse_held = 0.0f;
    uint32_t previous_mouse;
    Point mouse_lastpos = {0, 0};

public:
    ControlSystemx();
    ControlSystemx(Game * in_game);
    void setGame(Game * in_game);
    void updateGame();
    void setMapfromGame();
    void updateSettings();

    entityx::Entity getInputent(Controllers in_controllers);
    void AIturn(uint8_t in_army);

    void configure(entityx::EventManager & in_events);
    void receive(const inputMenuRight & menuright);
    void receive(const inputMenuLeft & menuleft);
    void receive(const inputMinimap & minimap);
    void receive(const inputFaster & faster);
    void receive(const cursorMoved & moved);
    void receive(const inputPause & pause);
    void receive(const inputCancel & cancel);
    void receive(const inputAccept & accept);
    void receive(const turnBegin & begin);
    void receive(const disableMouse & disableM);
    void receive(const enableMouse & enableM);
    void receive(const disableCursor & disableC);
    void receive(const enableCursor & enableC);
    void receive(const switchControl & Switch);

    void SDL_update();
    void update(entityx::EntityManager & es, entityx::EventManager & events, entityx::TimeDelta dt) override;
};

#endif /* CONTROLSYSTEMX_HPP */