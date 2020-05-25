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
    Settings * settings;
    entityx::ComponentHandle<Map> mapx;
    KeyboardInputMap keyboardInputMap;
    GamepadInputMap gamepadInputMap;
    MouseInputMap mouseInputMap;
    entityx::EventManager * event_manager;
    std::vector<std::vector<entityx::ComponentHandle<Unit>>> unitmap;
    entityx::ComponentHandle<Unit> selected;
    float min_held = 0.0f;
    float mouse_idletime = 0.0f;
    float mouse_disabletime = 0.5f;
    bool blockInput = false;
    double gp_held = 0.0f;
    double kb_held = 0.0f;
    double mouse_held = 0.0f;

public:
    ControlSystemx();
    ControlSystemx(Game * in_game);
    void updateMap();
    void updateSettings();

    unsigned int getHeldbutton(Controllers in_controllers);
    entityx::Entity getInputent(Controllers in_controllers);
    void AIturn(unsigned char in_army);

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